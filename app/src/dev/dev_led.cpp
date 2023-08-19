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

#include <dev/led.h>
#include <yss.h>
#include <bsp.h>
#include <util/Period.h>

#define FADE_STEP		(15)

namespace Led
{
	bool gLedFlag;
	triggerId gTriggerId;
	uint32_t gFadeTime;
	Mutex gMutex;
	Pwm *gPwm;

	void trigger_fadeLed(void)
	{
		gMutex.lock();
		
		// 루프의 실행 주기를 관리하는 Period에 주기 시간을 us 단위로 설정
		Period period(gFadeTime * 1000 / FADE_STEP);
		float ratio;
		
		if(gLedFlag)
		{
			ratio = 0.f;

			period.reset();
			for(uint32_t i=0;i<FADE_STEP;i++)
			{
				// 설정된 주기 시간만큼 대기
				period.wait();
				
				ratio += (float)(1.f / (float)FADE_STEP);

				// PWM 출력 비율 설정
				gPwm->setRatio(ratio);
			}
		}
		else
		{
			ratio = 1.f;

			period.reset();
			for(uint32_t i=0;i<FADE_STEP;i++)
			{
				// 설정된 주기 시간만큼 대기
				period.wait();
				
				ratio -= (float)(1.f / (float)FADE_STEP);

				// PWM 출력 비율 설정
				gPwm->setRatio(ratio);
			}
		}

		gMutex.unlock();		
	}

	void initialize(void)
	{
		using namespace define::gpio;

		// PWM 초기화
		gpioA.setAsAltFunc(5, altfunc::PA5_TIM2_CH1);

		gPwm = &pwm2Ch1;	// TIM2 CH1
		
		gPwm->enableClock();		// 클럭 활성화
		gPwm->initialize(10000);	// PWM 주파수를 10kHz로 설정
		gPwm->start();				// PWM 출력 시작
		
		// trigger_fadeLed() 함수를 트리거로 등록
		gTriggerId = trigger::add(trigger_fadeLed, 512);
	}

	void on(bool on, uint32_t fadeTime)
	{
		// trigger_fadeLed() 함수에서 FADE IN 또느 FADE OUT을 진행중에 다시 상태를 설정하면 현재 이곳에서 대기한다.
		gMutex.lock();

		if(gLedFlag != on)
		{	// 마지막 상태와 다른 설정값이 오면 trigger_fadeLed() 함수를 run 한다.
			gFadeTime = fadeTime;
			gLedFlag = on;
			trigger::run(gTriggerId);
		}

		gMutex.unlock();
	}
}

