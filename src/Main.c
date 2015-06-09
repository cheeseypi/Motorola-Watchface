#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static GFont s_time_font;
static GFont s_date_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static BitmapLayer *s_banner_layer;
static GBitmap *s_banner_bitmap;
static BitmapLayer *s_Lbanner_layer;
static GBitmap *s_Lbanner_bitmap;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void update_date() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  static char buffer[] = "Day, Mon 00";
  
  strftime(buffer, sizeof("Day, Mon 00"), "%a, %b %d", tick_time);
  

  // Display this time on the TextLayer
  text_layer_set_text(s_date_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  update_time();
  update_date();
}

static void main_window_load(Window *window) {
  // Create GBitmap, then set to created BitmapLayer
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_MAIN_BACKGROUND);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  // Create GBitmap, then set to created BitmapLayer
  s_banner_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BANNER);
  s_banner_layer = bitmap_layer_create(GRect(0, 0, 144, 55));
  bitmap_layer_set_bitmap(s_banner_layer, s_banner_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_banner_layer));
  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorClear);
  
  // Create date TextLayer
  s_date_layer = text_layer_create(GRect(0, 105, 144, 25));
  text_layer_set_background_color(s_date_layer, GColorBlack);
  text_layer_set_text_color(s_date_layer, GColorClear);
  
  // Create GBitmap, then set to created BitmapLayer
  s_Lbanner_bitmap = gbitmap_create_with_resource(RESOURCE_ID_LOWER_BANNER);
  s_Lbanner_layer = bitmap_layer_create(GRect(0, 117, 144, 55));
  bitmap_layer_set_bitmap(s_Lbanner_layer, s_Lbanner_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_Lbanner_layer));

  // Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_Motorola_ScreenType_42));
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_Motorola_ScreenType_21));
  
  // Apply to TextLayer
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_font(s_date_layer, s_date_font);
  
  // Improve the layout to be more like a watchface
  //text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  
  // Destroy TextLayer
  text_layer_destroy(s_date_layer);
  
  // Unload GFont
  fonts_unload_custom_font(s_time_font);
  
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
  
  // Destroy GBitmap
  gbitmap_destroy(s_banner_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_banner_layer);
  
  // Destroy GBitmap
  gbitmap_destroy(s_Lbanner_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_Lbanner_layer);
}

static void init(){
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Set original time
  update_time();
  update_date();
}

static void deinit(){
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void){
  init();
  app_event_loop();
  deinit();
}