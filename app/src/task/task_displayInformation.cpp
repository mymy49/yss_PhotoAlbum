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
		frame.drawStringToCenterAligned("정보");
		lcd.drawBmp({140, 0}, frame.getBmp888());

		// 본문 쓰기
		frame.setSize(440, 20);
		frame.setBackgroundColor(bgColor);
		frame.setFont(font);
		frame.setFontColor(0xFF, 0xFF, 0xFF);

		pos.x = 20;
		pos.y = 55;
		frame.clear();
		frame.drawString({0, 0}, "본 소프트웨어는 이순신 OS를 위한 예제 프로젝트입니다.");
		lcd.drawBmp(pos, frame.getBmp888());

		pos.y += 25;
		frame.clear();
		frame.drawString({0, 0}, "전자 앨범과 탁상 시계 기능을 구현 할 예정입니다.");
		lcd.drawBmp(pos, frame.getBmp888());

		pos.y += 25;
		frame.clear();
		frame.drawString({0, 0}, "이순신 OS에서 권장하는 프로그래밍 패턴을 제공하기 위해 진행합니다.");
		lcd.drawBmp(pos, frame.getBmp888());

		pos.y += 25;
		frame.clear();
		frame.drawString({0, 0}, "실무에 적용하는 예제 코드를 참고하시고 직접 활용 하는데 사용해보세요.");
		lcd.drawBmp(pos, frame.getBmp888());

		pos.y += 25;
		frame.clear();
		frame.drawString({0, 0}, "감사합니다.");
		lcd.drawBmp(pos, frame.getBmp888());
	}

	void thread_displayInformation(void)
	{
		bool anyKeyFlag = false;

		lcd.lock();
		drawPage();
		lcd.unlock();
		
		// key 이벤트에 등록한다.
		key::addPushHandler(Key::getAnyKey, anyKeyFlag);

		// 백라이트를 Fade in 한다.
		fadeinBackLight();		

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

	error displayInformation(FunctionQueue *obj)
	{
		(void)obj;

		lock();	// unlock()을 만날 때까지 외부에서 이 함수를 강제 종료 시키지 못한다.
		clearTask();	// 이전에 등록된 쓰레드 등을 전부 제거한다.

		fadeoutBackLight(); // 백라이트를 Fade out 한다.
		
		addThread(thread_displayInformation, 1024);	// thread_displayInformation() 함수를 스케줄러에 등록한다.
													// addThread() 함수를 통해 등록된 쓰레드는 clearTask() 함수 호출시 종료 된다.

		unlock();	// 외부에서 강제로 종료가 가능하다.

		return error::ERROR_NONE;
	}
}


