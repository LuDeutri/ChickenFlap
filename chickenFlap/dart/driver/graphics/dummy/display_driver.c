#include "../../../../../chickenFlap/dart/driver/graphics/dummy/display_driver.h"

#ifdef USE_DUMMY_DISPLAY

font_t defaultFont;

void display_init(DisplayRenderCallback callback, void* arg) {
	display_set_callback(callback, arg);
}

void display_set_callback(DisplayRenderCallback callback, void* arg) {
	// Do nothing
}

void display_update() {
	// Do nothing
}

void display_flush() {
	// Do nothing
}

int display_get_width() {
	return 800;
}

int display_get_height() {
	return 400;
}

bool display_has_touch() {
	return false;
}

font_t* display_default_font(int size) {
	return &defaultFont;
}

int display_font_width(font_t* font) {
	return 12;
}

int display_font_height(font_t* font) {
	return 20;
}

void display_start_frame() {
	// Do nothing
}

void display_stop_frame() {
	// Do nothing
}

void display_clear(color_t color) {
	// Do nothing
}

void display_set_front_color(color_t color) {
	// Do nothing
}

void display_set_back_color(color_t color) {
	// Do nothing
}

void display_set_font(font_t* font) {
	// Do nothing
}

void display_draw_text(int x, int y, const char* str, text_alignment_horz_t alignmentHorz, text_alignment_vert_t alignmentVert) {
	// Do nothing
}

void display_draw_text_at_line(int line, const char* str) {
	// Do nothing
}

void display_draw_rectangle(int x, int y, int width, int height) {
	// Do nothing
}

void display_draw_line(int x1, int y1, int x2, int y2) {
	// Do nothing
}

bool display_has_draw_line_width() {
	return false;
}

void display_draw_line_width(int x1, int y1, int x2, int y2, int width) {
	// Do nothing
}

void display_draw_bitmap(int x, int y, const uint8_t* data) {
	// Do nothing
}

bool display_is_touched() {
	return false;
}

int display_get_touch_x() {
	return 0;
}

int display_get_touch_y() {
	return 0;
}

void display_set_brightness(float brightness) {
	
}

void display_set_backlight_frequency(int frequency) {

}
#endif
