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

#include <../font/Noto_Sans_CJK_HK_DemiLight_32.h>
#include <../font/Noto_Sans_CJK_HK_Medium_24.h>
#include <../font/Noto_Sans_CJK_HK_DemiLight_14.h>

namespace Task
{
	static void drawItemBackground(void)
	{
		// 배경색 칠하기
		lcd.setBrushColor(0x30, 0x30, 0x30);
		lcd.fillRect({20, 50}, Size{440, 250});
	}
	
	static void drawPage(void)
	{
		Font font(Font_Noto_Sans_CJK_HK_DemiLight_14);
		Color titleColor(0x30, 0x30, 0xFF);
		Color bgColor(0x00, 0x00, 0x00);
		Position pos;

		font.setSpaceWidth(5);
		
		drawBasicBackground();
				
		// 제목 쓰기
		frame.setFont(Font_Noto_Sans_CJK_HK_Medium_24);
		frame.setSize(200, 40);
		frame.setBackgroundColor(titleColor);
		frame.setFontColor(0xFF, 0xFF, 0xFF);
		frame.clear();
		frame.drawStringToCenterAligned("파일 탐색기");
		lcd.drawBmp({140, 0}, frame.getBmp888());

		drawItemBackground();
	}

	void thread_handleFileExplorer(void)
	{
		bool anyKeyFlag = false;

		lcd.lock();
		drawPage();
		lcd.unlock();

		// 백라이트를 Fade in 한다.
		fadeinBackLight();		
		
		// key 이벤트에 등록한다.
		key::addPushHandler(Key::getAnyKey, anyKeyFlag);


		while(1)
		{
			if(anyKeyFlag)
			{	// 아무 버튼이나 눌리면
				anyKeyFlag = false;
				fq.lock();
				fq.add(Task::handleMainPage);	// 메인 페이지 처리 task를 등록한다.
				fq.unlock();

				while(1)	// clearTask() 함수에 의해 현재 쓰레드가 종료될 때까지 대기한다.
					thread::yield();
			}
		}
	}
	
	error handleFileExplorer(FunctionQueue *obj)
	{
		lock();	// unlock()을 만날 때까지 외부에서 이 함수를 강제 종료 시키지 못한다.
		clearTask();	// 이전에 등록된 쓰레드 등을 전부 제거한다.
		
		fadeoutBackLight(); // 백라이트를 Fade out 한다.

		addThread(thread_handleFileExplorer, 1024);	// thread_handleMainPage() 함수를 스케줄러에 등록한다.
												// addThread() 함수를 통해 등록된 쓰레드는 clearTask() 함수 호출시 종료 된다.

		unlock();

		return error::ERROR_NONE;
	}
}


