// Standard includes
#include "pebble.h"

// App-specific data
Window *window; // All apps must have at least one window
TextLayer *time_lH;
TextLayer *lLine_AU;
TextLayer *lLine_AD;
TextLayer *time_lM;
TextLayer *time_lM_A;
TextLayer *time_lS;
TextLayer *time_lS_A;

// комментарий супер пупер
// еще один коммент из другово места

static void handle_battery(BatteryChargeState charge_state) {
  static char battery_text[] = "full";

  if (charge_state.is_charging) {
    snprintf(battery_text, sizeof(battery_text), "onch");
  } else {
    snprintf(battery_text, sizeof(battery_text), "%d%%", charge_state.charge_percent);
  }
  text_layer_set_text(lLine_AU, battery_text);
}



// Called once per second
static void handle_second_tick(struct tm* tick_time, TimeUnits units_changed) {
  static char time_tH[] = "00", time_tM[] = "00", time_tS[] = "00", 
              day_m[] ="00", day_n[] = "00", day_y[] = "00";
  
  strftime(time_tH, sizeof(time_tH), "%H", tick_time);
  strftime(time_tM, sizeof(time_tM), "%M", tick_time);
  strftime(time_tS, sizeof(time_tS), "%S", tick_time);
  strftime(day_n, sizeof(day_n), "%a", tick_time);
  strftime(day_y, sizeof(day_y), "%d", tick_time);
  
  snprintf(day_m, sizeof(day_n) + sizeof(day_y), "%s %s", day_n, day_y);
    
  text_layer_set_text(time_lH, time_tH);
  text_layer_set_text(time_lM, time_tM);
  text_layer_set_text(time_lS, time_tS);
  text_layer_set_text(time_lS_A, day_m);

  handle_battery(battery_state_service_peek());
}

static void handle_bluetooth(bool connected) {
  text_layer_set_text(lLine_AD, connected ? "On" : "Off");
}

// Handle the start-up of the app
static void do_init(void) {

  // Create our app's base window
  window = window_create();
  window_stack_push(window, true);
  window_set_background_color(window, GColorBlack);

  Layer *root_layer = window_get_root_layer(window);
  GRect frame = layer_get_frame(root_layer);
  
//  ResHandle font_handle = resource_get_handle(RESOURCE_ID_FONT_AVANTE_EB_52);
  
  // Init Line #1  ############################################################
  // Init the text layer used to show the H
  time_lH = text_layer_create(GRect(1, 3, 94 /* width */, 52/* height */));
  text_layer_set_text_color(time_lH, GColorBlack);
  text_layer_set_background_color(time_lH, GColorWhite);
  text_layer_set_font(time_lH, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(time_lH, GTextAlignmentCenter);

  // Init the text layer used to show the Add Info
  lLine_AU = text_layer_create(GRect(97, 3, 46 /* width */, 25/* height */));
  text_layer_set_text_color(lLine_AU, GColorBlack);
  text_layer_set_background_color(lLine_AU, GColorWhite);
  text_layer_set_font(lLine_AU, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(lLine_AU, GTextAlignmentCenter);
  text_layer_set_text(lLine_AU, "XX");
  
  // Init the text layer used to show the Add Info
  lLine_AD = text_layer_create(GRect(97, 30, 46 /* width */, 25/* height */));
  text_layer_set_text_color(lLine_AD, GColorBlack);
  text_layer_set_background_color(lLine_AD, GColorWhite);
  text_layer_set_font(lLine_AD, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(lLine_AD, GTextAlignmentCenter);
  text_layer_set_text(lLine_AD, "XX");
  
  // Init Line #2  ############################################################
  // Init the text layer used to show the Add Info
  time_lM_A = text_layer_create(GRect(1, 58, 46, 52));
  text_layer_set_text_color(time_lM_A, GColorBlack);
  text_layer_set_background_color(time_lM_A, GColorWhite);
  text_layer_set_font(time_lM_A, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD)); 
  text_layer_set_text_alignment(time_lM_A, GTextAlignmentCenter);
  text_layer_set_text(time_lM_A, "XX");
  
  // Init the text layer used to show the M
  time_lM = text_layer_create(GRect(49, 58, 94, 52));
  text_layer_set_text_color(time_lM, GColorBlack);
  text_layer_set_background_color(time_lM, GColorWhite);
  text_layer_set_font(time_lM, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD)); 
  text_layer_set_text_alignment(time_lM, GTextAlignmentCenter);
  
  // Init Line #3  ############################################################
  // Init the text layer used to show the S
  time_lS = text_layer_create(GRect(1, 113, 94, 52));
  text_layer_set_text_color(time_lS, GColorBlack);
  text_layer_set_background_color(time_lS, GColorWhite);
  text_layer_set_font(time_lS, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD)); 
  text_layer_set_text_alignment(time_lS, GTextAlignmentCenter);
  
  // Init the text layer used to show the Add Info
  time_lS_A = text_layer_create(GRect(97, 113, 46, 52));
  text_layer_set_text_color(time_lS_A, GColorBlack);
  text_layer_set_background_color(time_lS_A, GColorWhite);
  text_layer_set_font(time_lS_A, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD)); 
  text_layer_set_text_alignment(time_lS_A, GTextAlignmentCenter);
  text_layer_set_text(time_lS_A, "XX");

  handle_bluetooth(bluetooth_connection_service_peek());

  // Ensures time is displayed immediately (will break if NULL tick event accessed).
  // (This is why it's a good idea to have a separate routine to do the update itself.)
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  handle_second_tick(current_time, SECOND_UNIT);

  tick_timer_service_subscribe(SECOND_UNIT, &handle_second_tick);
  battery_state_service_subscribe(&handle_battery);
  bluetooth_connection_service_subscribe(&handle_bluetooth);

  layer_add_child(root_layer, text_layer_get_layer(time_lH));
  layer_add_child(root_layer, text_layer_get_layer(lLine_AU));
  layer_add_child(root_layer, text_layer_get_layer(lLine_AD));
  layer_add_child(root_layer, text_layer_get_layer(time_lM));
  layer_add_child(root_layer, text_layer_get_layer(time_lM_A));
  layer_add_child(root_layer, text_layer_get_layer(time_lS));
  layer_add_child(root_layer, text_layer_get_layer(time_lS_A));  
}

static void do_deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  text_layer_destroy(time_lH);
  text_layer_destroy(lLine_AU);
  text_layer_destroy(lLine_AD);
  text_layer_destroy(time_lM);
  text_layer_destroy(time_lM_A);
  text_layer_destroy(time_lS);
  text_layer_destroy(time_lS_A);  
  window_destroy(window);
}

// The main event/run loop for our app
int main(void) {
  do_init();
  app_event_loop();
  do_deinit();
}
