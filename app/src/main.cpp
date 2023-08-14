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
#include <util/runtime.h>
#include <string.h>
#include <stdio.h>

void thread_handleRxMethod(void)
{
	uint32_t count;
	uint8_t *data, str[4] = {'\n', 0, '\n', '\r'};

	while(1)
	{
		// RX 수신 카운트를 얻는다.
		count = usart2.getRxCount();

		if(count)
		{	// 수신된 데이터가 있는 경우

			// 수신된 데이터의 버퍼를 얻는다.
			data = (uint8_t*)usart2.getRxBuffer();
			
			for(uint32_t i=0;i<count;i++)
			{	// 수신된 카운터 만큼 루프를 돈다.
				str[1] = *data++;
				usart2.lock();		// USART2를 다른 쓰레드에서도 접근하기 때문에 lock()으로 막는다.
				usart2.send(str, 4);
				usart2.unlock();	// USART2를 다른 쓰레드에서도 접근이 가능하도록 unlock()으로 풀어준다.
			}
			// 수신된 데이터의 처리된 양만큼 버퍼를 비워준다.
			usart2.releaseRxBuffer(count);
		}
		
		// 특별히 이 곳에서 오래 머무를 필요가 없다면 처리 효율을 높이기 위해
		// thread::yield() 함수를 호출해서 다른 쓰레드에 CPU 점유를 양보한다.
		thread::yield();
	}
}

int main(void)
{
	char str[32];
	uint32_t len;

	// 운영체체 초기화
	initializeYss();
	
	// 보드 초기화
	initializeBoard();

	thread::add(thread_handleRxMethod, 512);

	while(1)
	{
		sprintf(str, "%d\r", (uint32_t)runtime::getMsec());	// 현재 동작시간을 ms 단위로 얻어서 str에 출력한다.
		len = strlen(str);
		
		usart2.lock();		// USART2를 다른 쓰레드에서도 접근하기 때문에 lock()으로 막는다.
		usart2.send(str, len);
		usart2.unlock();	// USART2를 다른 쓰레드에서도 접근이 가능하도록 unlock()으로 풀어준다.
	}
}

