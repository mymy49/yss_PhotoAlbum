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
#include <../font/Noto_Sans_CJK_HK_Medium_18.h>
#include <../font/Noto_Sans_CJK_HK_DemiLight_12.h>
#include <../font/Noto_Sans_CJK_HK_DemiLight_10.h>

namespace Task
{
	error displayLogo(FunctionQueue *obj)
	{
		(void)obj;

		lock();	// unlock()을 만날 때까지 외부에서 이 함수를 강제 종료 시키지 못한다.
		clearTask();	// 이전에 등록된 쓰레드 등을 전부 제거한다.
		
		// LCD를 전부 하얀색으로 채운다.
		lcd.lock();
		lcd.setBackgroundColor(0xFF, 0xFF, 0xFF);
		lcd.clear();
		
		// 프레임 버퍼의 배경색을 회색으로 바꾼다.
		frame.setBackgroundColor(0x80, 0x80, 0x80);
		
		frame.setSize(480, 2);	// 가로선을 그리기 위해 프레임 버퍼의 크기를 조절한다.
		frame.clear();			// frame을 배경색으로 채운다.
		lcd.drawBmp({0, 120}, frame.getBmp888());	// 네모 박스의 위쪽 가로선을 그린다.
		lcd.drawBmp({0, 200}, frame.getBmp888());	// 네모 박스의 아래쪽 가로선을 그린다.

		frame.setSize(2, 80);	// 세로선을 그리기 위해 프레임 버퍼의 크기를 조절한다.
		frame.clear();
		lcd.drawBmp({0, 120}, frame.getBmp888());	// 네모 박스의 왼쪽 세로선을 그린다.
		lcd.drawBmp({478, 120}, frame.getBmp888());	// 네모 박스의 오른쪽 세로선을 그린다.

		// 제목
		frame.setSize(300, 20);
		frame.setBackgroundColor(0xFF, 0xFF, 0xFF);
		frame.clear();
		frame.setFont(Font_Noto_Sans_CJK_HK_Medium_18);
		frame.setFontColor(0x00, 0x00, 0x00);
		frame.drawString({0, 0}, "yss Embedded Operating System");
		lcd.drawBmp({10, 40 + 120}, frame.getBmp888());

		// Cafe
		frame.setSize(50, 14);
		frame.setBackgroundColor(0xFF, 0xFF, 0xFF);
		frame.clear();
		frame.setFont(Font_Noto_Sans_CJK_HK_DemiLight_12);
		frame.setFontColor(0x00, 0x00, 0x00);
		frame.drawString({0, 0}, "Cafe");
		lcd.drawBmp({10, 10 + 120}, frame.getBmp888());

		// 주소
		frame.setSize(250, 12);
		frame.setBackgroundColor(0xFF, 0xFF, 0xFF);
		frame.clear();
		frame.setFont(Font_Noto_Sans_CJK_HK_DemiLight_10);
		frame.setFontColor(0x00, 0x00, 0x00);
		frame.drawString({0, 0}, "http://cafe.naver.com/yssoperatingsystem");
		lcd.drawBmp({10, 60 + 120}, frame.getBmp888());
		
		// 컵 그리기
		frame.setSize(90, 31);
		frame.setBackgroundColor(0xFF, 0xFF, 0xFF);
		frame.clear();
		frame.setBrushColor(188, 215, 72);
		frame.fillCircle({74, 15}, 15);
		frame.setBrushColor(255, 255, 255);
		frame.fillCircle({74, 15}, 8);
		frame.setBrushColor(0, 199, 60);
		frame.fillRect({0, 0}, Size{75, 26});
		frame.fillTriangle({0, 26}, {5, 30}, {5, 26});
		frame.fillTriangle({74, 26}, {69, 30}, {69, 26});
		frame.fillRect({5, 26}, Position{69, 30});
		lcd.drawBmp({305, 49 + 120}, frame.getBmp888());
		
		// 컵의 김 올라오는 것 그리기
		frame.setSize(46, 22);
		frame.setBackgroundColor(0xFF, 0xFF, 0xFF);
		frame.clear();
		frame.setBrushColor(31, 145, 173);
		frame.fillTriangle({0, 21}, {15, 0}, {30, 21});
		frame.fillTriangle({45, 0}, {30, 21}, {15, 0});
		lcd.drawBmp({324, 14 + 120}, frame.getBmp888());

		// 원 두 개 #1
		frame.setSize(15, 15);
		frame.setBackgroundColor(0xFF, 0xFF, 0xFF);
		frame.clear();
		frame.setBrushColor(240, 220, 137);
		frame.fillCircle({9, 4}, 5);
		frame.setBrushColor(0, 0, 0);
		frame.drawCircle({4, 9}, 5);
		lcd.drawBmp({244, 59 + 120}, frame.getBmp888());

		// 원 두 개 #2
		frame.setSize(15, 15);
		frame.setBackgroundColor(0xFF, 0xFF, 0xFF);
		frame.clear();
		frame.setBrushColor(240, 137, 116);
		frame.fillCircle({9, 4}, 5);
		frame.setBrushColor(0, 0, 0);
		frame.drawCircle({4, 9}, 5);
		lcd.drawBmp({286, 10 + 120}, frame.getBmp888());

		// 원 두 개 #3
		frame.setSize(15, 15);
		frame.setBackgroundColor(0xFF, 0xFF, 0xFF);
		frame.clear();
		frame.setBrushColor(136, 216, 172);
		frame.fillCircle({9, 4}, 5);
		frame.setBrushColor(0, 0, 0);
		frame.drawCircle({4, 9}, 5);
		lcd.drawBmp({445, 53 + 120}, frame.getBmp888());

		// 세모 두 개 #1
		frame.setSize(15, 15);
		frame.setBackgroundColor(0xFF, 0xFF, 0xFF);
		frame.clear();
		frame.setBrushColor(136, 216, 172);
		frame.fillTriangle({4, 0}, {13, 1}, {10, 9});
		frame.setBrushColor(0, 0, 0);
		frame.drawTriangle({0, 1}, {9, 2}, {6, 10});
		lcd.drawBmp({235, 18 + 120}, frame.getBmp888());

		// 세모 두 개 #2
		frame.setSize(15, 15);
		frame.setBackgroundColor(0xFF, 0xFF, 0xFF);
		frame.clear();
		frame.setBrushColor(240, 137, 116);
		frame.fillTriangle({4, 0}, {13, 1}, {10, 9});
		frame.setBrushColor(0, 0, 0);
		frame.drawTriangle({0, 1}, {9, 2}, {6, 10});
		lcd.drawBmp({392, 22 + 120}, frame.getBmp888());
		
		// 지그제그 세개
		frame.setSize(30, 5);
		frame.setBackgroundColor(0xFF, 0xFF, 0xFF);
		frame.clear();
		frame.setBrushColor(0, 0, 0);
		frame.drawLine({0, 4}, {4, 0});
		frame.drawLine({4, 0}, {9, 4});
		frame.drawLine({9, 4}, {14, 0});
		frame.drawLine({14, 0}, {19, 4});
		frame.drawLine({19, 4}, {24, 0});
		frame.drawLine({24, 0}, {29, 4});
		lcd.drawBmp({412, 37 + 120}, frame.getBmp888());

		// 십자
		frame.setSize(10, 10);
		frame.setBackgroundColor(0xFF, 0xFF, 0xFF);
		frame.clear();
		frame.setBrushColor(0, 0, 0);
		frame.drawLine({0, 4}, {9, 4});
		frame.drawLine({4, 0}, {4, 9});
		lcd.drawBmp({434, 11 + 120}, frame.getBmp888());

		lcd.unlock();
		
		// 백라이트를 Fade in 한다.
		fadeinBackLight();
		
		// 5초간 로고 화면에서 대기한다.
		thread::delay(5000);
		
		unlock();

		return error::ERROR_NONE;
	}
}


