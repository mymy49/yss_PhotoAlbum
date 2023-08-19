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

namespace Speaker
{
	typedef struct
	{
		uint32_t chunkId;
		uint32_t chunkSize;
		uint32_t format;
		uint32_t subChunkId;
		uint32_t subChunkSize;
		uint16_t AudioFormat;
		uint16_t numOfCh;
		uint32_t sampleRate;
		uint32_t byteRate;
		uint16_t blockAlign;
		uint16_t bitPerSample;
		uint32_t subChunkId2;
		uint32_t subChunkSize2;
		uint16_t data;
	}WaveFile_t;

	void setDac(int16_t dac);

	Timer *gTimer;
	Dac *gDac;
	uint32_t gLen, gIndex;
	uint16_t *gData;

	void isr_timer(void)
	{
		if(gLen != gIndex)
			setDac(gData[gIndex++]);
		else
		{
			setDac(0);
			gTimer->stop();
		}
	}

	void initialize(void)
	{
		using namespace define::gpio;

		// PWM 초기화
		gpioA.setAsAnalog(4);
		
		gDac = &dac1;
		gDac->enableClock();
		gDac->initialize();
		gDac->enableChannel1();

		// TIMER 초기화
		gTimer = &timer7;
		gTimer->enableClock();
		gTimer->initialize(16000);
		gTimer->setUpdateIsr(isr_timer);
		gTimer->enableInterrupt();

		setDac(0);
	}

	void setDac(int16_t dac)
	{
		dac /= 8;
		dac += 2047;
		if(dac > 4095)
			dac = 4095;
		else if(dac < 0)
			dac = 0;

		dac1.setOutputChannel1(dac);
	}

	error play(const void *src)
	{
		WaveFile_t *wave = (WaveFile_t*)src;
		
		// 재생 가능 여부 확인
		if(	wave->chunkId != 0x46464952 ||		// RIFF
			wave->format != 0x45564157 ||		// WAVE
			wave->subChunkId != 0x20746D66 ||	// fmt 
			wave->subChunkId2 != 0x61746164 ||	// data
			wave->numOfCh != 1 ||
			wave->AudioFormat != 1 ||
			wave->bitPerSample != 16)
			return error::NOT_SUPPORTED_FORMAT;
		
		gData = &wave->data;
		gLen = wave->subChunkSize2 / 2;
		gTimer->start();

		return error::ERROR_NONE;
	}
}

