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

#include <dev/key.h>
#include <yss.h>
#include <bsp.h>
#include <util/Period.h>

namespace Key
{
	void initialize(void)
	{
	}

	bool getLeft(void)
	{
		return !gpioC.getInputData(0);
	}

	bool getRight(void)
	{
		return !gpioC.getInputData(1);
	}

	bool getCancel(void)
	{
		return !gpioC.getInputData(2);
	}

	bool getEnter(void)
	{
		return !gpioC.getInputData(3);
	}

	bool getAnyKey(void)
	{
		return (getLeft() || getRight() || getCancel() || getEnter());
	}
}

