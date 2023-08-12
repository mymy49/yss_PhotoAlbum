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

#include <drv/mcu.h>

#if defined(STM32F4_N) || defined(STM32F7_N) || defined(GD32F1) || defined(STM32F0_N)

#include <drv/Exti.h>
#include <yss/thread.h>
#include <yss/reg.h>

#if defined(STM32F030xC)
#include <targets/st/stm32f030xc.h>
#endif

Exti::Exti(void (*clockFunc)(bool en), void (*nvicFunc)(bool en)) : Drv(clockFunc, nvicFunc)
{
}

error Exti::add(Gpio &gpio, uint8_t pin, uint8_t mode, void (*func)(void))
{
	volatile uint32_t* peri = (volatile uint32_t*)EXTI;

	if (pin > 15)
		return error::INDEX_OVER;

	mTriggerFlag[pin] = false;
	mIsr[pin] = func;
	gpio.setExti(pin);

	setBitData(EXTI->RTSR, (Exti::RISING & mode) == Exti::RISING, pin);
	setBitData(EXTI->FTSR, (Exti::FALLING & mode) == Exti::FALLING, pin);

	return error::ERROR_NONE;
}

error Exti::add(Gpio &gpio, uint8_t pin, uint8_t mode, int32_t  trigger)
{
	volatile uint32_t* peri = (volatile uint32_t*)EXTI;

	if (pin > 15)
		return error::INDEX_OVER;

	mTriggerFlag[pin] = true;
	mTriggerNum[pin] = trigger;
	gpio.setExti(pin);
	
	setBitData(EXTI->RTSR, (Exti::RISING & mode) == Exti::RISING, pin);
	setBitData(EXTI->FTSR, (Exti::FALLING & mode) == Exti::FALLING, pin);

	return error::ERROR_NONE;
}

void Exti::enable(uint8_t pin, bool enable)
{
	setBitData(EXTI->IMR, enable, pin);	
}

void Exti::isr(int32_t  num)
{
#if !defined(__MCU_SMALL_SRAM_NO_SCHEDULE)
	if (mTriggerFlag[num])
		trigger::run(mTriggerNum[num]);
	else if(mIsr[num])
		mIsr[num]();
#else
	if (mIsr[num])
		mIsr[num]();
#endif
}

#endif

