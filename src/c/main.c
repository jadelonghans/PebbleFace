#include <pebble.h>

Window *my_window;
TextLayer *time_layer;

static void update_time(){
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
   // Write the current hours and minutes into a buffer
  static char time_buffer[8];
  strftime(time_buffer, sizeof(time_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);
  
  // Display this time on the TextLayer
  text_layer_set_text(time_layer, time_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
   update_time();
}

static void main_window_load( Window *window){
  //Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  //Create the TextLayer with specific bounds
  time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorBlack);
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(time_layer));
}

static void main_window_unload( Window *window){
   // Destroy TextLayer
  text_layer_destroy(time_layer);
}

void handle_init(void) {
  // create main Window element and assign to pointer
  my_window = window_create();
  
  //Register with tick_timer service
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  //set handlers to manage the elements inside the Window
  window_set_window_handlers(my_window, (WindowHandlers){
    .load = main_window_load,
    .unload = main_window_unload 
  });
  
  // show the Windows on the watch, with animated=true
  window_stack_push(my_window, true);
  
  //to show time when watchface launches
  update_time();
}

void handle_deinit(void) {
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
