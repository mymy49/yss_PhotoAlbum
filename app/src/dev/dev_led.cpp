////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License_ver_3.0
// 본 소스 코드의 소유권은 홍윤기에게 있습니다.
// 어떠한 형태든 기여는 기증으로 받아들입니다.
// 본 소스 코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
// 본 소스 코드를 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
// 본 소스 코드의 상업적 또는 비 상업적 이용이 가능합니다.
// 본 소스 코드의 내용을 임의로 수정하여 재배포하는 행위를 금합니다.
// 본 소스 코드의 내용을 무단 전재하는 행위를 금합니다.
// 본 소스 코드의 사용으로 인해 발생하는 모든 사고에 대해서 어떠한 법적 책임을 지지 않습니다.
//
// Home Page : http://cafe.naver.com/yssoperatingsystem
// Copyright 2022. 홍윤기 all right reserved.
//
////////////////////////////////////////////////////////////////////////////////////////

#include <dev/led.h>
#include <yss.h>
#include <bsp.h>
#include <util/Period.h>

#define FADE_STEP		(50)

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

