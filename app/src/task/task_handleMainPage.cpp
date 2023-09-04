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
#include <task.h>

#include <dev/key.h>

#include <../bmp/Folder.h>
#include <../bmp/Folder_s.h>
#include "../bmp/Information.h"
#include "../bmp/Information_s.h"
#include <../font/Noto_Sans_CJK_HK_DemiLight_16.h>
#include <util/key.h>

#define ITEM_COUNT	2

namespace Task
{
	static int8_t gSelect;

	void drawItem(uint8_t select)
	{
		if(select == 0)
			lcd.drawBmp({10, 10}, &Folder_s);
		else
			lcd.drawBmp({10, 10}, &Folder);

		frame.setSize(110, 20);
		frame.setBackgroundColor(0x00, 0x00, 0x00);
		frame.setFontColor(0xFF, 0xFF, 0xFF);
		frame.setFont(Font_Noto_Sans_CJK_HK_DemiLight_16);
		frame.clear();
		frame.drawStringToCenterAligned("파일 탐색기");
		lcd.drawBmp({10, 110}, frame.getBmp888());

		if(select == 1)
			lcd.drawBmp({130, 10}, &Information_s);
		else
			lcd.drawBmp({130, 10}, &Information);

		frame.setSize(110, 20);
		frame.setBackgroundColor(0x00, 0x00, 0x00);
		frame.setFontColor(0xFF, 0xFF, 0xFF);
		frame.setFont(Font_Noto_Sans_CJK_HK_DemiLight_16);
		frame.clear();
		frame.drawStringToCenterAligned("정보");
		lcd.drawBmp({130, 110}, frame.getBmp888());
	}

	void drawMenuBackground(void)
	{
		lcd.setBackgroundColor(0x00, 0x00, 0x00);
		lcd.clear();

		drawItem(gSelect);
	}

	void thread_handleMainPage(void)
	{
		bool leftKeyFlag = false, rightKeyFlag = false, enterKeyFlag = false, ableFlag;

		lcd.lock();
		drawMenuBackground();
		lcd.unlock();
		
		key::addPushHandler(Key::getLeft, leftKeyFlag);
		key::addPushHandler(Key::getRight, rightKeyFlag);
		key::addPushHandler(Key::getEnter, enterKeyFlag);

		// 백라이트를 Fade in 한다.
		fadeinBackLight();

		while(1)
		{
			if(rightKeyFlag)
			{
				rightKeyFlag = false;
				gSelect++;
				if(gSelect >= ITEM_COUNT)
					gSelect = 0;
				
				drawItem(gSelect);
			}

			if(leftKeyFlag)
			{
				leftKeyFlag = false;
				gSelect--;
				if(gSelect < 0)
					gSelect = ITEM_COUNT-1;
				
				drawItem(gSelect);
			}

			if(enterKeyFlag)
			{
				enterKeyFlag = false;
				ableFlag = false;

				fq.lock();
				switch(gSelect)
				{
				case 0 : // 파일 탐색기
					// 구현 안됨
					break;
				
				case 1 : // 정보
					ableFlag = true;
					fq.add(Task::displayInformation);
					break;
				}
				fq.unlock();
				
				if(ableFlag)
				{
					while(1)	// clearTask() 함수에 의한 쓰레드 종료 대기
						thread::yield();
				}
			}

			thread::yield();
		}
	}

	error handleMainPage(FunctionQueue *obj)
	{
		lock();	// unlock()을 만날 때까지 외부에서 이 함수를 강제 종료 시키지 못한다.
		clearTask();	// 이전에 등록된 쓰레드 등을 전부 제거한다.
		
		fadeoutBackLight(); // 백라이트를 Fade out 한다.

		addThread(thread_handleMainPage, 512);	// thread_handleMainPage() 함수를 스케줄러에 등록한다.
												// addThread() 함수를 통해 등록된 쓰레드는 clearTask() 함수 호출시 종료 된다.

		unlock();	// 외부에서 강제로 종료가 가능하다.

		return error::ERROR_NONE;
	}
}


