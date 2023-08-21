/*
	Copyright 2023. 홍윤기 all right reserved.

	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the
	"Software"), to deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to
	the following conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
	LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef BSP__H_
#define BSP__H_

#include <dev/led.h>
#include <dev/speaker.h>
#include <mod/spi_tft_lcd/MSP3520.h>
#include <gui/Bmp888Buffer.h>

extern MSP3520 lcd;
extern Bmp888Buffer frame;

// 보드의 장치들을 초기화 한다.
void initializeBoard(void);

// LCD의 백라이트 밝기를 조절한다.
//
// float dimming
//		밝기를 설정한다. 0 ~ 1까지 값으로 1이 가장 밝은 값이다. 
void setLcdBackLight(float dimming);

#endif

