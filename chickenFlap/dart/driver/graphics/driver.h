#pragma once


#include "../../../../chickenFlap/dart/driver/graphics/color.h"
#include "../../../../chickenFlap/dart/driver/graphics/dummy/display_driver.h"
#include "../../../../chickenFlap/dart/driver/graphics/eve2/display_driver.h"
#include "../../../../chickenFlap/dart/driver/graphics/font.h"
#include "../../../../chickenFlap/dart/driver/graphics/stm32f7_discovery/display_driver.h"
#include "../../../../chickenFlap/dart/driver/graphics/windows/display_driver.h"
#include "../../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FONT8 (display_default_font(8))
#define FONT12 (display_default_font(12))
#define FONT16 (display_default_font(16))
#define FONT18 (display_default_font(18))
#define FONT20 (display_default_font(20))
#define FONT22 (display_default_font(22))
#define FONT24 (display_default_font(24))


typedef enum {
    TextAlignmentLeft,
    TextAlignmentRight,
	TextAlignmentCenterHorz,
} text_alignment_horz_t;

typedef enum {
    TextAlignmentTop,
    TextAlignmentBottom,
	TextAlignmentCenterVert,
} text_alignment_vert_t;

/**
* Defines the callback that is called when the display wants to refresh its buffer.
*/
typedef void (*DisplayRenderCallback)(bool full, void* arg);

/**
* Initializes the display. The given callback is called when the display wants to refresh its buffer with the given user-provided argument.
*/
void display_init(DisplayRenderCallback callback, void* arg);

/**
* Sets the callback which is called when the displya wants to refresh its buffer.
*/
void display_set_callback(DisplayRenderCallback callback, void* arg);

/**
* Updates the display.
*/
void display_update();

/**
* Forces the display update itself.
*/
void display_flush();

/**
* Returns the width of the display in pixel.
*/
int display_get_width();
/**
* Returns the height of the display in pixel.
*/
int display_get_height();

/**
 * Returns if the display has touch input.
 */
bool display_has_touch();

font_t* display_default_font(int size);
int display_font_width(font_t* font);
int display_font_height(font_t* font);

bool display_is_touched();
int display_get_touch_x();
int display_get_touch_y();

void display_set_brightness(float brightness);
void display_set_backlight_frequency(int frequency);

void display_start_frame();
void display_stop_frame();

void display_clear(color_t color);
void display_set_front_color(color_t color);
void display_set_back_color(color_t color);
void display_set_font(font_t* font);
void display_draw_text(int x, int y, const char* str, text_alignment_horz_t alignmentHorz, text_alignment_vert_t alignmentVert);
void display_draw_text_at_line(int line, const char* str);
void display_draw_rectangle(int x, int y, int width, int height);
void display_draw_line(int x1, int y1, int x2, int y2);
bool display_has_draw_line_width();
void display_draw_line_width(int x1, int y1, int x2, int y2, int width);
void display_draw_bitmap(int x, int y, const uint8_t* data);

#ifdef __cplusplus
}
#endif
