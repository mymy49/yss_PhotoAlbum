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

#include <drv/peripheral.h>

#if defined(STM32F4_N) || defined(STM32F0_N) || defined(STM32F7_N) || defined(STM32F1_N)

#include <yss/instance.h>
#include <config.h>
#include <yss.h>

#if defined(STM32F446xx)
#include <targets/st/bitfield_stm32f446xx.h>
#elif defined(STM32F429xx)
#include <targets/st/bitfield_stm32f429xx.h>
#elif defined(STM32F030xC)
#include <targets/st/bitfield_stm32f030xx.h>
#elif defined(STM32F767xx)
#include <targets/st/bitfield_stm32f767xx.h>
#elif defined(STM32F746xx)
#include <targets/st/bitfield_stm32f746xx.h>
#elif defined(STM32F103xB)
#include <targets/st/bitfield_stm32f103xx.h>
#endif

#if SPI2_ENABLE || SPI3_ENABLE
static uint32_t getApb1ClockFrequency(void)
{
	return clock.getApb1ClockFrequency();
}
#endif

#if SPI1_ENABLE || SPI4_ENABLE || SPI5_ENABLE
static uint32_t getApb2ClockFrequency(void)
{
	return clock.getApb2ClockFrequency();
}
#endif

#if SPI1_ENABLE && defined(SPI1)
static void enableSpi1Clock(bool en)
{
	clock.lock();
    clock.enableApb2Clock(RCC_APB2ENR_SPI1EN_Pos, en);
	clock.unlock();
}

static void enableSpi1Interrupt(bool en)
{
	nvic.lock();
	nvic.enableInterrupt(SPI1_IRQn, en);
	nvic.unlock();
}

static void resetSpi1(void)
{
	clock.lock();
    clock.resetApb2(RCC_APB2RSTR_SPI1RST_Pos);
	clock.unlock();
}

static const Drv::Setup gDrvSpi1Setup = 
{
	enableSpi1Clock,		//void (*clockFunc)(bool en);
	enableSpi1Interrupt,	//void (*nvicFunc)(bool en);
	resetSpi1,				//void (*resetFunc)(void);
	getApb2ClockFrequency,	//uint32_t (*getClockFreq)(void);
};

static const Dma::DmaInfo gSpi1TxDmaInfo = 
{
#if defined(STM32F1_N) || defined(GD32F1)
	(define::dma::priorityLevel::LOW << DMA_CCR_PL_Pos) |	 // uint32_t controlRegister1
	(define::dma::size::BYTE << DMA_CCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_CCR_PSIZE_Pos) |
	DMA_CCR_MINC_Msk | 
	(define::dma::dir::MEM_TO_PERI << DMA_CCR_DIR_Pos) | 
	DMA_CCR_TCIE_Msk | 
	DMA_CCR_TEIE_Msk | 
	DMA_CCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI1->DR,											// void *dataRegister;
#elif defined(STM32F4_N) || defined(STM32F7_N)
	(define::dma2::stream3::SPI1_TX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::MEM_TO_PERI << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI1->DR,											// void *dataRegister;
#endif
};

static const Dma::DmaInfo gSpi1RxDmaInfo = 
{
#if defined(STM32F1_N) || defined(GD32F1)
	(define::dma::priorityLevel::LOW << DMA_CCR_PL_Pos) |		// uint32_t controlRegister1
	(define::dma::size::BYTE << DMA_CCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_CCR_PSIZE_Pos) |
	DMA_CCR_MINC_Msk | 
	(define::dma::dir::PERI_TO_MEM << DMA_CCR_DIR_Pos) | 
	DMA_CCR_TCIE_Msk | 
	DMA_CCR_TEIE_Msk | 
	DMA_CCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI1->DR,											// void *dataRegister;
#elif defined(STM32F4_N) || defined(STM32F7_N)
	(define::dma2::stream0::SPI1_RX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::PERI_TO_MEM << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI1->DR,											//void *dataRegister;
#endif
};

#if defined(STM32F0_N) || defined(STM32F1_N)
static const Spi::Setup gSpi1Setup = 
{
	SPI1,			//YSS_SPI_Peri *peri;
	dmaChannel3,	//Dma &txDma;
	gSpi1TxDmaInfo,	//Dma::DmaInfo txDmaInfo;
	dmaChannel2,	//Dma &rxDma;
	gSpi1RxDmaInfo,	//Dma::DmaInfo rxDmaInfo;
};
#else
static const Spi::Setup gSpi1Setup = 
{
	SPI1,			//YSS_SPI_Peri *peri;
	dmaChannel12,	//Dma &txDma;
	gSpi1TxDmaInfo,	//Dma::DmaInfo txDmaInfo;
	dmaChannel9,	//Dma &rxDma;
	gSpi1RxDmaInfo,	//Dma::DmaInfo rxDmaInfo;
	
};
#endif

Spi spi1(gDrvSpi1Setup, gSpi1Setup);

extern "C"
{
	void SPI1_IRQHandler(void)
	{
		spi1.isr();
	}
}
#endif

#if SPI2_ENABLE && defined(SPI2)
static void enableSpi2Clock(bool en)
{
	clock.lock();
    clock.enableApb1Clock(RCC_APB1ENR_SPI2EN_Pos, en);
	clock.unlock();
}

static void enableSpi2Interrupt(bool en)
{
	nvic.lock();
	nvic.enableInterrupt(SPI2_IRQn, en);
	nvic.unlock();
}

static void resetSpi2(void)
{
	clock.lock();
    clock.resetApb1(RCC_APB1RSTR_SPI2RST_Pos);
	clock.unlock();
}

static const Drv::Setup gDrvSpi2Setup = 
{
	enableSpi2Clock,		//void (*clockFunc)(bool en);
	enableSpi2Interrupt,	//void (*nvicFunc)(bool en);
	resetSpi2,				//void (*resetFunc)(void);
	getApb1ClockFrequency	//uint32_t (*getClockFreq)(void);
};

static const Dma::DmaInfo gSpi2TxDmaInfo = 
{
#if defined(STM32F1_N) || defined(STM32F0_N) || defined(GD32F1)
	(define::dma::priorityLevel::LOW << DMA_CCR_PL_Pos) |		 // uint32_t controlRegister1
	(define::dma::size::BYTE << DMA_CCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_CCR_PSIZE_Pos) |
	DMA_CCR_MINC_Msk | 
	(define::dma::dir::MEM_TO_PERI << DMA_CCR_DIR_Pos) | 
	DMA_CCR_TCIE_Msk | 
	DMA_CCR_TEIE_Msk | 
	DMA_CCR_EN_Msk ,
	0x0F << (4 * 4),											// uint32_t controlRegister2
	0x03 << (4 * 4),											// uint32_t controlRegister3
	(void*)&SPI2->DR,											//void *dataRegister;
#elif defined(STM32F4_N) || defined(STM32F7_N)
	(define::dma1::stream4::SPI2_TX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::MEM_TO_PERI << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI2->DR,											//void *dataRegister;
#endif
};

static const Dma::DmaInfo gSpi2RxDmaInfo = 
{
#if defined(STM32F1_N) || defined(STM32F0_N) || defined(GD32F1)
	(define::dma::priorityLevel::LOW << DMA_CCR_PL_Pos) |		// uint32_t controlRegister1
	(define::dma::size::BYTE << DMA_CCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_CCR_PSIZE_Pos) |
	DMA_CCR_MINC_Msk | 
	(define::dma::dir::PERI_TO_MEM << DMA_CCR_DIR_Pos) | 
	DMA_CCR_TCIE_Msk | 
	DMA_CCR_TEIE_Msk | 
	DMA_CCR_EN_Msk ,
	0x0F << (4 * 3),											// uint32_t controlRegister2
	0x03 << (4 * 3),											// uint32_t controlRegister3
	(void*)&SPI2->DR,											//void *dataRegister;
#elif defined(STM32F4_N) || defined(STM32F7_N)
	(define::dma1::stream3::SPI2_RX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::PERI_TO_MEM << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI2->DR,											//void *dataRegister;
#endif
};

static const Spi::Setup gSpi2Setup = 
{
	SPI2,			//YSS_SPI_Peri *peri;
	dmaChannel5,	//Dma &txDma;
	gSpi2TxDmaInfo,	//Dma::DmaInfo txDmaInfo;
	dmaChannel4,	//Dma &rxDma;
	gSpi2RxDmaInfo	//Dma::DmaInfo rxDmaInfo;
};

Spi spi2(gDrvSpi2Setup, gSpi2Setup);

extern "C"
{
	void SPI2_IRQHandler(void)
	{
		spi2.isr();
	}
}
#endif

#if SPI3_ENABLE && defined(SPI3)
static void enableSpi3Clock(bool en)
{
	clock.lock();
    clock.enableApb1Clock(RCC_APB1ENR_SPI3EN_Pos, en);
	clock.unlock();
}

static void enableSpi3Interrupt(bool en)
{
	nvic.lock();
	nvic.enableInterrupt(SPI3_IRQn, en);
	nvic.unlock();
}

static void resetSpi3(void)
{
	clock.lock();
    clock.resetApb1(RCC_APB1RSTR_SPI3RST_Pos);
	clock.unlock();
}

static const Drv::Setup gDrvSpi3Setup = 
{
	enableSpi3Clock,		//void (*clockFunc)(bool en);
	enableSpi3Interrupt,	//void (*nvicFunc)(bool en);
	resetSpi3,				//void (*resetFunc)(void);
	getApb1ClockFrequency	//uint32_t (*getClockFreq)(void);
};

static const Dma::DmaInfo gSpi3TxDmaInfo = 
{
#if defined(STM32F1) || defined(GD32F1)
	(define::dma::priorityLevel::LOW << DMA_CCR_PL_Pos) |		// uint32_t controlRegister1
	(define::dma::size::BYTE << DMA_CCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_CCR_PSIZE_Pos) |
	DMA_CCR_MINC_Msk | 
	(define::dma::dir::MEM_TO_PERI << DMA_CCR_DIR_Pos) | 
	DMA_CCR_TCIE_Msk | 
	DMA_CCR_TEIE_Msk | 
	DMA_CCR_EN_Msk ,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI3[SPI_REG::DR],									//void *dataRegister;
#elif defined(STM32F4_N) || defined(STM32F7_N)
	(define::dma1::stream5::SPI3_TX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::MEM_TO_PERI << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI3->DR,											//void *dataRegister;
#endif
};

static const Dma::DmaInfo gSpi3RxDmaInfo = 
{
#if defined(STM32F1) || defined(GD32F1)
	(define::dma::priorityLevel::LOW << DMA_CCR_PL_Pos) |		// uint32_t controlRegister1
	(define::dma::size::BYTE << DMA_CCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_CCR_PSIZE_Pos) |
	DMA_CCR_MINC_Msk | 
	(define::dma::dir::PERI_TO_MEM << DMA_CCR_DIR_Pos) | 
	DMA_CCR_TCIE_Msk | 
	DMA_CCR_TEIE_Msk | 
	DMA_CCR_EN_Msk ,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI3[SPI_REG::DR],									//void *dataRegister;
#elif defined(STM32F4_N) || defined(STM32F7_N)
	(define::dma1::stream0::SPI3_RX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::PERI_TO_MEM << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI3->DR,											//void *dataRegister;
#endif
};

static const Spi::Setup gSpi3Setup = 
{
	SPI3,			//YSS_SPI_Peri *peri;
	dmaChannel6,	//Dma &txDma;
	gSpi3TxDmaInfo,	//Dma::DmaInfo txDmaInfo;
	dmaChannel1,	//Dma &rxDma;
	gSpi3RxDmaInfo	//Dma::DmaInfo rxDmaInfo;
};

Spi spi3(gDrvSpi3Setup, gSpi3Setup);

extern "C"
{
	void SPI3_IRQHandler(void)
	{
		spi3.isr();
	}
}
#endif



#if SPI4_ENABLE && defined(SPI4)
static void enableSpi4Clock(bool en)
{
	clock.lock();
    clock.enableApb1Clock(RCC_APB2ENR_SPI4EN_Pos, en);
	clock.unlock();
}

static void enableSpi4Interrupt(bool en)
{
	nvic.lock();
	nvic.enableInterrupt(SPI4_IRQn, en);
	nvic.unlock();
}

static void resetSpi4(void)
{
	clock.lock();
    clock.resetApb2(RCC_APB2RSTR_SPI4RST_Pos);
	clock.unlock();
}

static const Drv::Setup gDrvSpi4Setup = 
{
	enableSpi4Clock,		//void (*clockFunc)(bool en);
	enableSpi4Interrupt,	//void (*nvicFunc)(bool en);
	resetSpi4,				//void (*resetFunc)(void);
	getApb2ClockFrequency	//uint32_t (*getClockFreq)(void);
};

static const Dma::DmaInfo gSpi4TxDmaInfo = 
{
	(define::dma2::stream1::SPI4_TX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::MEM_TO_PERI << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)SPI4->DR,											//void *dataRegister;
};

static const Dma::DmaInfo gSpi4RxDmaInfo = 
{
	(define::dma2::stream0::SPI4_RX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::PERI_TO_MEM << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	DMA_SxFCR_DMDIS,											// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI4->DR,											//void *dataRegister;
};

static const Spi::Setup gSpi4Setup = 
{
	SPI4,			//YSS_SPI_Peri *peri;
	dmaChannel10,	//Dma &txDma;
	gSpi4TxDmaInfo,	//Dma::DmaInfo txDmaInfo;
	dmaChannel9,	//Dma &rxDma;
	gSpi4RxDmaInfo	//Dma::DmaInfo rxDmaInfo;
};

Spi spi4(gDrvSpi4Setup, gSpi4Setup);

extern "C"
{
	void SPI4_IRQHandler(void)
	{
		spi4.isr();
	}
}
#endif



#if SPI5_ENABLE && defined(SPI5)
static void enableSpi5Clock(bool en)
{
	clock.lock();
    clock.enableApb2Clock(RCC_APB2ENR_SPI5EN_Pos, en);
	clock.unlock();
}

static void enableSpi5Interrupt(bool en)
{
	nvic.lock();
	nvic.enableInterrupt(SPI5_IRQn, en);
	nvic.unlock();
}

static void resetSpi5(void)
{
	clock.lock();
    clock.resetApb2(RCC_APB2RSTR_SPI5RST_Pos);
	clock.unlock();
}

static const Drv::Setup gDrvSpi5Setup = 
{
	enableSpi5Clock,		//void (*clockFunc)(bool en);
	enableSpi5Interrupt,	//void (*nvicFunc)(bool en);
	resetSpi5,				//void (*resetFunc)(void);
	getApb2ClockFrequency	//uint32_t (*getClockFreq)(void);
};

static const Dma::DmaInfo gSpi5TxDmaInfo = 
{
	(define::dma2::stream4::SPI5_TX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::MEM_TO_PERI << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI5->DR,											//void *dataRegister;
};

static const Dma::DmaInfo gSpi5RxDmaInfo = 
{
	(define::dma2::stream3::SPI5_RX << DMA_SxCR_CHSEL_Pos) |	// uint32_t controlRegister1
	(define::dma::burst::SINGLE << DMA_SxCR_MBURST_Pos) | 
	(define::dma::burst::SINGLE << DMA_SxCR_PBURST_Pos) | 
	(define::dma::priorityLevel::LOW << DMA_SxCR_PL_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_MSIZE_Pos) |
	(define::dma::size::BYTE << DMA_SxCR_PSIZE_Pos) |
	DMA_SxCR_MINC_Msk | 
	(define::dma::dir::PERI_TO_MEM << DMA_SxCR_DIR_Pos) | 
	DMA_SxCR_TCIE_Msk | 
	DMA_SxCR_TEIE_Msk | 
	DMA_SxCR_EN_Msk,
	0,															// uint32_t controlRegister2
	0,															// uint32_t controlRegister3
	(void*)&SPI5->DR,											//void *dataRegister;
};

static const Spi::Setup gSpi5Setup = 
{
	SPI5,			//YSS_SPI_Peri *peri;
	dmaChannel13,	//Dma &txDma;
	gSpi5TxDmaInfo,	//Dma::DmaInfo txDmaInfo;
	dmaChannel12,	//Dma &rxDma;
	gSpi5RxDmaInfo	//Dma::DmaInfo rxDmaInfo;
};

Spi spi5(gDrvSpi5Setup, gSpi5Setup);

extern "C"
{
	void SPI5_IRQHandler(void)
	{
		spi5.isr();
	}
}
#endif

#endif

