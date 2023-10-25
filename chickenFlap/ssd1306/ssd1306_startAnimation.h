/*
 * Desribes the start animation on the OLED screen
 *
 *  Created on: 20.01.2022
 *      Author: lucad
 */

#ifndef SSD1306_SSD1306_STARTANIMATION_H_
#define SSD1306_SSD1306_STARTANIMATION_H_

#include <string.h>
#include <stdio.h>

#include "../dart/hal_wrapper/hal_wrapper.h"
#include "ssd1306_conf.h"
#include "ssd1306.h"
#include "../components/error.h"
#include "../components/config.h"

typedef struct{
	bool enable;
	bool sloganFinished;
	bool movieFinished;
	int16_t roundSlogan;
	uint8_t roundMovie;
	util_time_t animationStartTime;
	bool drawHeartFinished;
} startAnimation_t;
extern startAnimation_t startAnimation;

typedef struct{
	uint8_t tableHeight;
	uint8_t tableWidth;
	uint8_t pictureWidth;
	const uint8_t *data;
	bool displayUpdateWhileBuilding; // If true you can follow the building of the screen, if false it will be shown in finish state
	util_time_t holdingtime; // time to hold the finished build
	int8_t buildDirection; // 1: build from top to bottom, -1: build from bottom to top
} pictureDef_t;

extern pictureDef_t heartDef;
extern pictureDef_t chickenStandardDef;
extern pictureDef_t chickenStrechedDef;
extern pictureDef_t chickenFlapOpenedDef;
extern pictureDef_t chickenFlapClosedDef;

void ssd1306StartAnimation_init();
void ssd1306StartAnimation();

void ssd1306StartAnimation_slogan();
void ssd1306StartAnimation_movieScene();

void drawPicture(uint8_t startX, uint8_t startY,  pictureDef_t pictureDef);


#endif /* STARTANIMATION_H_ */
