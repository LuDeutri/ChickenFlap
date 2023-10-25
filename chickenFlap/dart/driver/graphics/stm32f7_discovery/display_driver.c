#include "../../../../../chickenFlap/dart/driver/graphics/stm32f7_discovery/display_driver.h"

#ifdef BOARD_STM32F769I_DISCO

DisplayRenderCallback display_callback;
void* display_callback_arg;

font_t display_default_fonts[5];

util_time_t display_last_touch;
int display_touch_x = 0;
int display_touch_y = 0;

void display_init(DisplayRenderCallback callback, void* arg) {
	display_set_callback(callback, arg);

	// Initialize HAL
	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER_FOREGROUND, LCD_FB_START_ADDRESS);
	BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER_FOREGROUND);

	(&display_default_fonts[0])->driver = &Font24;
	(&display_default_fonts[1])->driver = &Font20;
	(&display_default_fonts[2])->driver = &Font16;
	(&display_default_fonts[3])->driver = &Font12;
	(&display_default_fonts[4])->driver = &Font8;

	// Initialize touch
	BSP_TS_Init(getWidth(), getHeight());
}

void display_set_callback(DisplayRenderCallback callback, void* arg) {
	display_callback = callback;
	display_callback_arg = arg;
}

void display_touch_update()  {
	TS_StateTypeDef ts;
	if (BSP_TS_GetState(&ts) == TS_OK && ts.touchDetected > 0) {
		display_touch_x = ts.touchX[0];
		display_touch_y = ts.touchY[0];
		display_last_touch = millis();
	}
}

void display_update() {
	display_touch_update();

	if (display_callback != NULL)
		display_callback(false, display_callback_arg);
}

void display_flush() {
	display_touch_update();

	if (display_callback != NULL)
		display_callback(true, display_callback_arg);
}

int display_get_width() {
	return BSP_LCD_GetXSize();
}

int display_get_height() {
	return BSP_LCD_GetYSize();
}

bool display_has_touch() {
	return true;
}

font_t* display_default_font(int size) {
	switch (size) {
	case 24:
		return &display_default_fonts[0];
	case 20:
		return &display_default_fonts[1];
	case 16:
		return &display_default_fonts[2];
	case 12:
		return &display_default_fonts[3];
	case 8:
		return &display_default_fonts[4];
	default:
		if (size > 24)
			return display_default_font(24);
		return display_default_font(8);
	}
}

int display_font_width(font_t* font) {
	DART_NOT_NULL_RETURN(font, DART_ERROR_INVALID_VALUE, 0);
	return ((sFONT*)font->driver)->Width;
}

int display_font_height(font_t* font) {
	DART_NOT_NULL_RETURN(font, DART_ERROR_INVALID_VALUE, 0);
	return ((sFONT*)font->driver)->Height;
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
	// NOP
}
void display_set_backlight_frequency(int frequency) {
	// NOP
}

void display_start_frame() {
	// Do nothing
}

void display_stop_frame() {
	// Do nothing
}

void display_clear(color_t color) {
	BSP_LCD_Clear(color);
}

void display_set_front_color(color_t color) {
	BSP_LCD_SetTextColor(color);
}

void display_set_back_color(color_t color) {
	BSP_LCD_SetBackColor(color);
}

void display_set_font(font_t* font) {
	DART_NOT_NULL(font, DART_ERROR_INVALID_VALUE);
	BSP_LCD_SetFont((sFONT*)font->driver);
}

void display_draw_text(int x, int y, const char* str, text_alignment_horz_t alignmentHorz, text_alignment_vert_t alignmentVert) {
	DART_ASSERT(x >= 0 && x < display_get_width(), DART_ERROR_INVALID_VALUE);
	DART_ASSERT(y >= 0 && y < display_get_height(), DART_ERROR_INVALID_VALUE);

	switch (alignmentHorz) {
	default:
	case TextAlignmentLeft:
		BSP_LCD_DisplayStringAt(x, y, (uint8_t*)str, LEFT_MODE);
		break;
	case TextAlignmentCenterHorz:
		BSP_LCD_DisplayStringAt(x, y, (uint8_t*)str, CENTER_MODE);
		break;
	case TextAlignmentRight:
		BSP_LCD_DisplayStringAt(x, y, (uint8_t*)str, RIGHT_MODE);
		break;
	}
}

void display_draw_text_at_line(int line, const char* str) {
	DART_ASSERT(line >= 0, DART_ERROR_INVALID_VALUE);
	BSP_LCD_DisplayStringAtLine(line, (uint8_t*)str);
}

void display_draw_rectangle(int x, int y, int width, int height) {
	DART_ASSERT(x >= 0 && x < display_get_width(), DART_ERROR_INVALID_VALUE);
	DART_ASSERT(y >= 0 && y < display_get_height(), DART_ERROR_INVALID_VALUE);

	BSP_LCD_FillRect(x, y, width, height);
}

void display_draw_line(int x1, int y1, int x2, int y2) {
	DART_ASSERT(x1 >= 0 && x1 < display_get_width(), DART_ERROR_INVALID_VALUE);
	DART_ASSERT(y1 >= 0 && y1 < display_get_height(), DART_ERROR_INVALID_VALUE);
	DART_ASSERT(x2 >= 0 && x2 < display_get_width(), DART_ERROR_INVALID_VALUE);
	DART_ASSERT(y2 >= 0 && y2 < display_get_height(), DART_ERROR_INVALID_VALUE);

	if (x1 == x2 && y1 == y2)
		BSP_LCD_DrawVLine(x1, y1, 1);
	else if (x1 == x2) {
		if (y2 > y1)
			BSP_LCD_DrawVLine(x1, y1, y2 - y1);
		else
			BSP_LCD_DrawVLine(x1, y2, y1 - y2);
	}
	else if (y1 == y2) {
		if (x2 > x1)
			BSP_LCD_DrawHLine(x1, y1, x2 - x1);
		else
			BSP_LCD_DrawHLine(x2, y1, x1 - x2);
	}
	else
		BSP_LCD_DrawLine(x1, y1, x2, y2);
}

bool display_has_draw_line_width() {
	return false;
}

void display_draw_line_width(int x1, int y1, int x2, int y2, int width) {
	DART_NOT_IMPLEMENTED();
}

void display_draw_bitmap(int x, int y, const uint8_t* data) {
	DART_ASSERT(x >= 0 && x < display_get_width(), DART_ERROR_INVALID_VALUE);
	DART_ASSERT(y >= 0 && y < display_get_height(), DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(data, DART_ERROR_INVALID_VALUE);

	BSP_LCD_DrawBitmap(x, y, (uint8_t*)data);
}
#endif
