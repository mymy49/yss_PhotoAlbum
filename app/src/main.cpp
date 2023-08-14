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

#include <yss.h>
#include <bsp.h>

triggerId gTriggerId;

// Interrupt Service Routine에서는 문맥 전환을 유발하는 
// thread::yield(), thread::delay() 등의 함수 호출을하면 안됩니다.
// 장치의 데이터 전송도 불가능합니다.
// 위와 같은 작업을 인터럽트와 연계해서 처리하기 위해서는 trigger 기능을 연동하면 됩니다.
void isr_exti0(void)
{
	trigger::run(gTriggerId);
}

// trigger::run()이 발생되면 첫 쓰레드 진입에서 높은 우선순위로 먼처 스케줄링 됩니다.
// 처음 1회 높은 우선순위로 스케줄링 된 후, 문맥전환이 발생되면 쓰레드가 return 될 때까지는 일반 라운드로빈 스케줄링됩니다.
void trigger_blinkLed(void)
{
	for(uint32_t i=0;i<5;i++)
	{
		Led::on(true, 80);
		thread::delay(100);

		Led::on(false, 80);
		thread::delay(100);
	}
}

int main(void)
{
	// 운영체체 초기화
	initializeYss();
	
	// 보드 초기화
	initializeBoard();

	// 외부인터럽트 초기화 (USER 버튼)
	// PC13에 하강 엣지로 설정
	exti.add(gpioC, 13, Exti::FALLING, isr_exti0);
	exti.enable(13);
	
	// trigger 등록
	gTriggerId = trigger::add(trigger_blinkLed, 512);

	while(1)
	{
		thread::yield();
	}
}

