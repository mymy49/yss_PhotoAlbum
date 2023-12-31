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

#ifndef DEV_KEY__H_
#define DEV_KEY__H_

#include <stdint.h>

namespace Key
{
	// Button를 초기화 한다.
	void initialize(void);
	
	// 좌측 버튼 키의 현재 상태를 얻는다.
	//
	// 반환
	//		좌측 버튼이 눌렸을 경우 true, 안눌렸을 경우 false를 반환한다.
	bool getLeft(void);

	// 우측 버튼 키의 현재 상태를 얻는다.
	//
	// 반환
	//		우측 버튼이 눌렸을 경우 true, 안눌렸을 경우 false를 반환한다.
	bool getRight(void);

	// 취소 버튼 키의 현재 상태를 얻는다.
	//
	// 반환
	//		우측 버튼이 눌렸을 경우 true, 안눌렸을 경우 false를 반환한다.
	bool getCancel(void);

	// 엔터 버튼 키의 현재 상태를 얻는다.
	//
	// 반환
	//		우측 버튼이 눌렸을 경우 true, 안눌렸을 경우 false를 반환한다.
	bool getEnter(void);

	// 전체 버튼 키 중에 하나라도 눌린 현재 상태를 얻는다.
	//
	// 반환
	//		버튼이 눌렸을 경우 true, 안눌렸을 경우 false를 반환한다.
	bool getAnyKey(void);
}

#endif
