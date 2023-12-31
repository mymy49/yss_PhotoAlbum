////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License V3.3
//
// 본 소스 코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
//
// 본 소스 코드를 :
//		- 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
//		- 상업적 또는 비 상업적 이용이 가능합니다.
//		- 본 저작권 표시 주석을 제외한 코드의 내용을 임의로 수정하여 사용하는 것은 허용합니다.
//		- 사용자가 수정한 코드를 사용자의 고객사에게 상호간 전달은 허용합니다.
//		- 그러나 수정하여 다수에게 재배포하는 행위를 금지합니다. 
//		- 사용으로 인해 발생하는 모든 사고에 대해서 어떠한 법적 책임을 지지 않습니다.
//		- 어떤 형태의 기여든지, 그것은 기증으로 받아들입니다.
//
// 본 소스 코드는 프리웨어로 앞으로도 유료로 전환하지 않을 것입니다.
// 사용자 또는 부품의 제조사가 요구하는 업데이트가 있을 경우 후원금을 받아 
// 요구하는 사항을 업데이트 할 예정입니다.
//
// Home Page : http://cafe.naver.com/yssoperatingsystem
// Copyright 2023. 홍윤기 all right reserved.
//
////////////////////////////////////////////////////////////////////////////////////////

#include <config.h>

#if USE_GUI == true

#include <gui/BrushRgb565.h>
#include <gui/Bmp565.h>

#define PI (float)3.14159265358979323846

BrushRgb565::BrushRgb565(void)
{
}

BrushRgb565::~BrushRgb565(void)
{
}

void BrushRgb565::drawBmp(Position pos, const Bmp565 *image)
{
	uint16_t *fb = (uint16_t *)image->data, *src;
	uint16_t width = image->width;
	uint16_t height = image->height;
	int16_t xs = pos.x, ys = pos.y;

	if (xs + width > mSize.width)
		width = mSize.width - xs;
	if (ys + height > mSize.height)
		height = mSize.height - ys;

	width += xs;
	height += ys;

	for (int16_t y = ys; y < height; y++)
	{
		src = fb;
		fb += image->width;

		for (int16_t x = xs; x < width; x++)
		{
			drawDot(x, y, *src++);
		}
	}
}

void BrushRgb565::drawBmp(Position pos, const Bmp565 &image)
{
	drawBmp(pos, &image);
}

uint8_t BrushRgb565::drawChar(Position pos, uint32_t utf8)
{
	if (mFont.setChar(utf8))
		return 0;

	YssFontInfo *fontInfo = mFont.getFontInfo();
	uint8_t *fontFb = mFont.getFrameBuffer(), color;
	int32_t  index = 0;
	uint16_t width = fontInfo->width, height = fontInfo->height, offset = 0;
	int16_t xs = pos.x, ys = pos.y + (int8_t)fontInfo->ypos;

	if (xs + width > mSize.width)
	{
		width = mSize.width - xs;
		offset = fontInfo->width - width;
	}
	if (ys + height > mSize.height)
		height = mSize.height - ys;

	width += xs;
	height += ys;

	for (int32_t  y = ys; y < height; y++)
	{
		for (int32_t  x = xs; x < width; x++, index++)
		{
			if (index % 2 == 0)
			{
				color = fontFb[index / 2] & 0x0f;
				drawDot(x, y, mFontColorTable[color]);
			}
			else
			{
				color = (fontFb[index / 2] >> 4) & 0x0f;
				drawDot(x, y, mFontColorTable[color]);
			}
		}
		index += offset;
	}

	return fontInfo->width;
}

void BrushRgb565::updateFontColor(void)
{
	for(uint8_t i=0;i<16;i++)
		mFontColorTable[i] = mFontColor.calculateFontColorLevel(mBgColor, i).getRgb565Code();
}

#endif