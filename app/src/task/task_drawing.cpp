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

#include <task.h>
#include <yss.h>
#include <bsp.h>
#include <util/key.h>

#include <../font/Noto_Sans_CJK_HK_Medium_24.h>
#include <../font/Noto_Sans_CJK_HK_DemiLight_14.h>

namespace Task
{
	void drawBasicBackground(void)
	{
		Color titleColor(0x30, 0x30, 0xFF);
		Color bgColor(0x00, 0x00, 0x00);

		// 테두리색 칠하기
		lcd.setBackgroundColor(titleColor);
		lcd.clear();
		
		// 배경색 칠하기
		lcd.setBrushColor(bgColor);
		lcd.fillRect({10, 10}, Size{460, 300});

		// 제목 배경 칠하기
		lcd.setBrushColor(titleColor);
		lcd.fillRect({140, 10}, Size{200, 30});
	}
}


