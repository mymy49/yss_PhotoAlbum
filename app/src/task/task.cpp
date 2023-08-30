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

#include "../inc/task.h"
#include <yss.h>

#define MAX_TASK_THREAD		4

namespace Task
{
	uint32_t gThreadCnt;
	threadId gThreadId[MAX_TASK_THREAD];
	FunctionQueue *gFq;
	Mutex gMutex;

#if USE_GUI && YSS_L_HEAP_USE
	Frame *gFrame;
#endif

	void setFunctionQueue(FunctionQueue &obj)
	{
		gFq = &obj;
	}

	void lock(void)
	{
		gMutex.lock();
	}

	void unlock(void)
	{
		gMutex.unlock();
	}

	void addThread(void (*func)(void), uint32_t stackSize)
	{
		if(gThreadCnt < MAX_TASK_THREAD)
			gThreadId[gThreadCnt++] = thread::add(func, stackSize);
	}

	void clearTask(void)
	{
		for(uint32_t i=0;i<gThreadCnt;i++)
		{
			if(gThreadId[i])
			{
				thread::remove(gThreadId[i]);
				gThreadId[i] = 0;
			}
		}

#if USE_GUI && YSS_L_HEAP_USE
		if(gFrame)
		{
			delete gFrame;
			gFrame = 0;
		}
#endif
		gThreadCnt = 0;
	}

#if USE_GUI && YSS_L_HEAP_USE
	void setFrame(Frame *obj)
	{
		setSystemFrame(obj);
		gFrame = obj;
	}
#endif
}

