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

#ifndef YSS_DRV_I2C__H_
#define YSS_DRV_I2C__H_

#include "peripheral.h"

#if defined(STM32F7)

typedef volatile uint32_t	YSS_I2C_Peri;

#elif defined(STM32F1) || defined(STM32F4)

typedef volatile uint32_t	YSS_I2C_Peri;

#include <targets/st_gigadevice/define_i2c_stm32_gd32f1_f4.h>

#elif defined(STM32F0_N) || defined(STM32F7_N)

typedef I2C_TypeDef			YSS_I2C_Peri;

#elif defined(STM32F4_N) || defined(GD32F1) || defined(STM32F1_N)

typedef I2C_TypeDef			YSS_I2C_Peri;
#define I2C_NOT_USE_DMA
#else

#define YSS_DRV_I2C_UNSUPPORTED
typedef volatile uint32_t	YSS_I2C_Peri;

#endif

#include "Drv.h"
#include "Dma.h"
#include <yss/error.h>

class I2c : public Drv
{
  public:
	struct Setup
	{
		YSS_I2C_Peri *dev;
		Dma &txDma;
		Dma::DmaInfo txDmaInfo;
		Dma &rxDma;
		Dma::DmaInfo rxDmaInfo;
	};

	error initializeAsMain(uint8_t speed);

	error initializeAsSub(void *rcvBuf, uint16_t rcvBufSize, uint8_t addr1, uint8_t addr2 = 0);

	error send(uint8_t addr, void *src, uint32_t size, uint32_t timeout = 500);

	error receive(uint8_t addr, void *des, uint32_t size, uint32_t timeout = 500);

	void stop(void);

	// 아래 함수는 시스템 함수로 사용자 호출을 금한다.
	I2c(const Drv::Setup drvSetup, const Setup setup);

	void isr(void);

private :
	YSS_I2C_Peri *mDev;

#if defined(I2C_NOT_USE_DMA)
	uint32_t mDataCount;
	uint8_t *mDataBuf, mAddr;
	bool mDir;
#else
	Dma *mTxDma, *mRxDma;
	Dma::DmaInfo mTxDmaInfo, mRxDmaInfo;
#endif
};

#endif

