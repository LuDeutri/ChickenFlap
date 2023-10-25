#include "../../../../../chickenFlap/dart/driver/graphics/eve2/display_driver.h"

#ifdef USE_EVE2_DISPLAY

eve2_font_t eve2_fonts[32];
font_t eve2_display_fonts[32];

DisplayRenderCallback display_callback;
void* display_callback_arg;

font_t* display_font = NULL;
color_t display_front_color = COLOR_BLACK;
color_t display_back_color = COLOR_BLACK;

util_time_t display_last_touch = 0;
int display_touch_x = 0;
int display_touch_y = 0;

void display_init(DisplayRenderCallback callback, void* arg) {
	display_set_callback(callback, arg);
	display_font = display_default_font(18);
}

void display_set_callback(DisplayRenderCallback callback, void* arg) {
	display_callback = callback;
	display_callback_arg = arg;
}

void display_update() {
	// We always regenerate the whole display list
	display_flush();
}

void display_flush() {
	// Update touch screen
	uint32_t touch = EVE_memRead32(REG_TOUCH_DIRECT_XY);
	if ((touch & 0x80000000) == 0) {
		display_last_touch = millis();
		display_touch_x = (touch >> 16) & 0x03FF;
		display_touch_y = touch & 0x03FF;
	}

	// Clear colors
	display_front_color = COLOR_TRANSPARENT;
	display_back_color = COLOR_TRANSPARENT;

	// Start display list
	EVE_start_cmd_burst();
	EVE_cmd_dl(CMD_DLSTART);
	EVE_cmd_dl(TAG(0));

	// Generate display list by callback
	if (display_callback != NULL)
		display_callback(true, display_callback_arg);

	// Set display list as active
	EVE_cmd_dl(DL_DISPLAY);
	EVE_cmd_dl(CMD_SWAP);

	// End executing list
	EVE_end_cmd_burst();
	EVE_cmd_start();
}

int display_get_width() {
	return EVE_HSIZE;
}

int display_get_height() {
	return 116;
}

bool display_has_touch() {
	return true;
}

font_t* display_default_font(int size) {
	eve2_font_t font;
	switch(size) {
	case 16:
		font.index = 26;
		font.size = 16;
		font.width = 6;
		font.height = 12;
		break;
	case 18:
		font.index = 27;
		font.size = 18;
		font.width = 8;
		font.height = 14;
		break;
	case 20:
		font.index = 27;
		font.size = 18;
		font.width = 9;
		font.height = 16;
		break;
	case 22:
		font.index = 29;
		font.size = 22;
		font.width = 11;
		font.height = 21;
		break;
	case 24:
		font.index = 30;
		font.size = 24;
		font.width = 12;
		font.height = 26;
		break;
	case 26:
		font.index = 31;
		font.size = 26;
		font.width = 14;
		font.height = 28;
		break;
	default:
		return display_default_font(18);
	}

	// Create font in memory
	eve2_fonts[font.index] = font;
	font_t* display_font = &eve2_display_fonts[font.index];
	display_font->driver = &eve2_fonts[font.index];
	return display_font;
}

int display_font_width(font_t* font) {
	DART_NOT_NULL_RETURN(font, DART_ERROR_INVALID_VALUE, 0);
	DART_NOT_NULL_RETURN(font->driver, DART_ERROR_INVALID_VALUE, 0);
	return ((eve2_font_t*)font->driver)->width;
}

int display_font_height(font_t* font) {
	DART_NOT_NULL_RETURN(font, DART_ERROR_INVALID_VALUE, 0);
	DART_NOT_NULL_RETURN(font->driver, DART_ERROR_INVALID_VALUE, 0);
	return ((eve2_font_t*)font->driver)->height;
}

bool display_is_touched() {
	return (millis() - display_last_touch) < 100;
}

int display_get_touch_x() {
	return display_touch_x;
}

int display_get_touch_y() {
	return display_touch_y;
}

void display_set_brightness(float brightness) {
	EVE_memWrite8(REG_PWM_DUTY, min((uint8_t)(brightness * 128), 128));
}

void display_set_backlight_frequency(int frequency) {
	DART_ASSERT(frequency >= 250, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(frequency <= 10000, DART_ERROR_INVALID_VALUE);
	EVE_memWrite32(REG_PWM_HZ, (uint32_t)frequency);
}

void display_start_frame() {

}

void display_stop_frame() {

}

void display_clear(color_t color) {
	EVE_cmd_dl(DL_CLEAR_RGB | (color & 0xFFFFFF));
	EVE_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);
}

void display_set_front_color(color_t color) {
	if (display_front_color != color) {
		EVE_cmd_dl(DL_COLOR_RGB | (color & 0xFFFFFF));
		EVE_cmd_dl(COLOR_A(color >> 24));
		display_front_color = color;
	}
}

void display_set_back_color(color_t color) {
	display_back_color = color;
}

void display_set_font(font_t* font) {
	DART_NOT_NULL(font, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(font->driver, DART_ERROR_INVALID_VALUE);

	display_font = font;
}

void display_draw_text(int x, int y, const char* str, text_alignment_horz_t alignmentHorz, text_alignment_vert_t alignmentVert) {
	DART_ASSERT(x >= 0 && x < display_get_width(), DART_ERROR_INVALID_VALUE);
	DART_ASSERT(y >= 0 && y < display_get_height(), DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(display_font, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(display_font->driver, DART_ERROR_INVALID_VALUE);

	int options = 0;
	if (alignmentHorz == TextAlignmentCenterHorz)
		options |= EVE_OPT_CENTERX;
	else if (alignmentHorz == TextAlignmentRight)
		options |= EVE_OPT_RIGHTX;

	if (alignmentVert == TextAlignmentCenterVert)
		options |= EVE_OPT_CENTERY;
	else if (alignmentVert == TextAlignmentBottom)
		DART_NOT_IMPLEMENTED();

	EVE_cmd_text(x, y, ((eve2_font_t*)display_font->driver)->index, options, str);
}

void display_draw_text_at_line(int line, const char* str) {
	DART_ASSERT(line >= 0, DART_ERROR_INVALID_VALUE);

	display_draw_text(8, 8 + line * (display_font_height(display_font) + 2), str, TextAlignmentLeft, TextAlignmentTop);
}

void display_draw_rectangle(int x, int y, int width, int height) {
	DART_ASSERT(x >= 0 && x < display_get_width(), DART_ERROR_INVALID_VALUE);
	DART_ASSERT(y >= 0 && y < display_get_height(), DART_ERROR_INVALID_VALUE);

	EVE_cmd_rect(x, y, x + width, y + height, 1);
}

void display_draw_line(int x1, int y1, int x2, int y2) {
	display_draw_line_width(x1, y1, x2, y2, 1);
}

bool display_has_draw_line_width() {
	return true;
}

void display_draw_line_width(int x1, int y1, int x2, int y2, int width) {
	DART_ASSERT(x1 >= 0 && x1 < display_get_width(), DART_ERROR_INVALID_VALUE);
	DART_ASSERT(y1 >= 0 && y1 < display_get_height(), DART_ERROR_INVALID_VALUE);
	DART_ASSERT(x2 >= 0 && x2 < display_get_width(), DART_ERROR_INVALID_VALUE);
	DART_ASSERT(y2 >= 0 && y2 < display_get_height(), DART_ERROR_INVALID_VALUE);

	EVE_cmd_line(x1, y1, x2, y2, width);
}

void display_draw_bitmap(int x, int y, const uint8_t* data) {
	DART_ASSERT(x >= 0 && x < display_get_width(), DART_ERROR_INVALID_VALUE);
	DART_ASSERT(y >= 0 && y < display_get_height(), DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(data, DART_ERROR_INVALID_VALUE);
}
#endif
