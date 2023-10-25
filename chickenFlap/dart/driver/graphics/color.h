#pragma once

#include "../../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

typedef uint32_t color_t;

#define COLOR_ALPHA(c) ((c >> 24) & 0xFF)
#define COLOR_R(c) ((c >> 16) & 0xFF)
#define COLOR_G(c) ((c >> 8) & 0xFF)
#define COLOR_B(c) (c & 0xFF)

#define COLOR_BLUE          ((color_t) 0xFF0000FF)
#define COLOR_GREEN         ((color_t) 0xFF00FF00)
#define COLOR_RED           ((color_t) 0xFFFF0000)
#define COLOR_CYAN          ((color_t) 0xFF00FFFF)
#define COLOR_MAGENTA       ((color_t) 0xFFFF00FF)
#define COLOR_YELLOW        ((color_t) 0xFFFFFF00)
#define COLOR_LIGHTBLUE     ((color_t) 0xFF8080FF)
#define COLOR_LIGHTGREEN    ((color_t) 0xFF80FF80)
#define COLOR_LIGHTRED      ((color_t) 0xFFFF8080)
#define COLOR_LIGHTCYAN     ((color_t) 0xFF80FFFF)
#define COLOR_LIGHTMAGENTA  ((color_t) 0xFFFF80FF)
#define COLOR_LIGHTYELLOW   ((color_t) 0xFFFFFF80)
#define COLOR_DARKBLUE      ((color_t) 0xFF000080)
#define COLOR_DARKGREEN     ((color_t) 0xFF008000)
#define COLOR_DARKRED       ((color_t) 0xFF800000)
#define COLOR_DARKCYAN      ((color_t) 0xFF008080)
#define COLOR_DARKMAGENTA   ((color_t) 0xFF800080)
#define COLOR_DARKYELLOW    ((color_t) 0xFF808000)
#define COLOR_WHITE         ((color_t) 0xFFFFFFFF)
#define COLOR_LIGHTGRAY     ((color_t) 0xFFD3D3D3)
#define COLOR_GRAY          ((color_t) 0xFF808080)
#define COLOR_DARKGRAY      ((color_t) 0xFF404040)
#define COLOR_BLACK         ((color_t) 0xFF000000)
#define COLOR_BROWN         ((color_t) 0xFFA52A2A)
#define COLOR_ORANGE        ((color_t) 0xFFFFA500)
#define COLOR_TRANSPARENT   ((color_t) 0x00000000)
#define COLOR_RED_DART		((color_t) 0xFFBE2026)
