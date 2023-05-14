// *****************************************************************************
// Filename    : IntVect.c
// 
// Core        : STM32F100RB Cortex™-M3  
// 
// Board       : STM32VLDISCOVERY
//
// Compiler    : ARM® Compiler v5.06 for μVision (Keil)
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 04.09.2017
// 
// Description : Interrupt vector for STM32F100RB.
//
// License     : GNU General Public License v3.0
// 
// *****************************************************************************

//=============================================================================
// linker symbol
//=============================================================================

#if defined(__GNUC__) && !defined(__CC_ARM)
extern void __initial_stack_pointer(void);
#elif defined(__CC_ARM)
extern unsigned int Image$$ER_STACK$$ZI$$Limit;
#else
#error Error: Compiler startup-code dialect is not supported
#endif

//=============================================================================
// extern function prototype
//=============================================================================
extern void SysStartup_Init(void);
extern void OsDispatcher(void);
extern void OsSysTickTimerIsr(void);
extern void OsCat2IsrWrapper(void);

//=============================================================================
// local function prototype
//=============================================================================
void undefinedHandler(void);
void HardFaultdHandler(void);

#if defined(__GNUC__) && !defined(__CC_ARM)
#elif defined(__CC_ARM)
/* Disable the warning D-1296 */
#pragma diag_suppress 1296
#else
#error Error: Compiler startup-code dialect is not supported
#endif

#if defined(__GNUC__) && !defined(__CC_ARM)
const volatile unsigned int IntVector[128U] __attribute__((section(".isr_vector")));
#elif defined(__CC_ARM)
const volatile unsigned int IntVector[128U] __attribute__((section ("INTVECT")));
#else
#error Error: Compiler startup-code dialect is not supported
#endif

const volatile unsigned int IntVector[128U] =
{
#if defined(__GNUC__) && !defined(__CC_ARM)
  (unsigned int)__initial_stack_pointer,     // 0x0000, initial stack pointer
#elif defined(__CC_ARM)
  (unsigned int)&Image$$ER_STACK$$ZI$$Limit, // 0x0000, initial stack pointer
#else
#error Error: Compiler startup-code dialect is not supported
#endif
  (unsigned int)&SysStartup_Init,            // 0x0004, reset
  (unsigned int)&undefinedHandler,           // 0x0008, nmi exception
  (unsigned int)&HardFaultdHandler,          // 0x000C, hard fault exception
  (unsigned int)&undefinedHandler,           // 0x0010, memory management exception
  (unsigned int)&undefinedHandler,           // 0x0014, bus fault exception
  (unsigned int)&undefinedHandler,           // 0x0018, usage fault exception
  (unsigned int)&undefinedHandler,           // 0x001C, reserved
  (unsigned int)&undefinedHandler,           // 0x0020, reserved
  (unsigned int)&undefinedHandler,           // 0x0024, reserved
  (unsigned int)&undefinedHandler,           // 0x0028, reserved
  (unsigned int)&undefinedHandler,           // 0x002C, svc handler
  (unsigned int)&undefinedHandler,           // 0x0030, debug monitor
  (unsigned int)&undefinedHandler,           // 0x0034, reserved
  (unsigned int)&OsDispatcher,               // 0x0038, pending svc,
  (unsigned int)&OsCat2IsrWrapper,           // 0x003C, system tick handler,
  (unsigned int)&undefinedHandler,           // 0x0040, wwdg irq handler,
  (unsigned int)&undefinedHandler,           // 0x0044, pvd irq handler,
  (unsigned int)&undefinedHandler,           // 0x0048, tamper irq handler,
  (unsigned int)&undefinedHandler,           // 0x004C, rtc irq handler,
  (unsigned int)&undefinedHandler,           // 0x0050, flash irq handler,
  (unsigned int)&undefinedHandler,           // 0x0054, rcc irq handler,
  (unsigned int)&undefinedHandler,           // 0x0058, exti0 irq handler,&OsTrap,                     
  (unsigned int)&undefinedHandler,           // 0x005C, exti1 irq handler,
  (unsigned int)&undefinedHandler,           // 0x0060, exti2 irq handler,
  (unsigned int)&undefinedHandler,           // 0x0064, exti3 irq handler,
  (unsigned int)&undefinedHandler,           // 0x0068, exti4 irq handler,
  (unsigned int)&undefinedHandler,           // 0x006C, dma_channel1 irq handler,
  (unsigned int)&undefinedHandler,           // 0x0070, dma_channel2 irq handler,
  (unsigned int)&undefinedHandler,           // 0x0074, dma_channel3 irq handler,
  (unsigned int)&undefinedHandler,           // 0x0078, dma_channel4 irq handler,
  (unsigned int)&undefinedHandler,           // 0x007C, dma_channel5 irq handler,
  (unsigned int)&undefinedHandler,           // 0x0080, dma_channel6 irq handler,
  (unsigned int)&undefinedHandler,           // 0x0084, dma_channel7 irq handler,
  (unsigned int)&undefinedHandler,           // 0x0088, adc irq handler,
  (unsigned int)&undefinedHandler,           // 0x008C, usb_hp_can_tx irq handler,
  (unsigned int)&undefinedHandler,           // 0x0090, usb_lp_can_rx0 irq handler,
  (unsigned int)&undefinedHandler,           // 0x0094, can_rx1 irq handler,
  (unsigned int)&undefinedHandler,           // 0x0098, can_sce irq handler,
  (unsigned int)&undefinedHandler,           // 0x009C, exti9_5 irq handler,
  (unsigned int)&undefinedHandler,           // 0x00A0, tim1_brk irq handler,
  (unsigned int)&undefinedHandler,           // 0x00A4, tim1_up irq handler,
  (unsigned int)&undefinedHandler,           // 0x00A8, tim1_trg_com irq handler,
  (unsigned int)&undefinedHandler,           // 0x00AC, tim1_cc irq handler,
  (unsigned int)&undefinedHandler,           // 0x00B0, tim2 irq handler,
  (unsigned int)&undefinedHandler,           // 0x00B4, tim3 irq handler,
  (unsigned int)&undefinedHandler,           // 0x00B8, tim4 irq handler,
  (unsigned int)&undefinedHandler,           // 0x00BC, i2c1_ev irq handler,
  (unsigned int)&undefinedHandler,           // 0x00C0, i2c1_er irq handler,
  (unsigned int)&undefinedHandler,           // 0x00C4, i2c2_ev irq handler,
  (unsigned int)&undefinedHandler,           // 0x00C8, i2c2_er irq handler,
  (unsigned int)&undefinedHandler,           // 0x00CC, spi1 irq handler,
  (unsigned int)&undefinedHandler,           // 0x00D0, spi2 irq handler,
  (unsigned int)&undefinedHandler,           // 0x00D4, usart1 irq handler,
  (unsigned int)&undefinedHandler,           // 0x00D8, usart2 irq handler,
  (unsigned int)&undefinedHandler,           // 0x00DC, usart3 irq handler,
  (unsigned int)&undefinedHandler,           // 0x00E0, exti15_10 irq handler,
  (unsigned int)&undefinedHandler,           // 0x00E4, rtcalarm irq handler,
  (unsigned int)&undefinedHandler,           // 0x00E8, usbwakeup irq handler,
  (unsigned int)&undefinedHandler,           // 0x00EC, reserved
  (unsigned int)&undefinedHandler,           // 0x00F0, reserved
  (unsigned int)&undefinedHandler,           // 0x00F4, reserved
  (unsigned int)&undefinedHandler,           // 0x00F8, reserved
  (unsigned int)&undefinedHandler,           // 0x00FC, reserved
  (unsigned int)&undefinedHandler,           // 0x0100, reserved
  (unsigned int)&undefinedHandler,           // 0x0104, reserved
  (unsigned int)&undefinedHandler,           // 0x0108, this is for boot in ram mode for medium density devices.
  (unsigned int)&undefinedHandler,           // 0x010C, unused
  (unsigned int)&undefinedHandler,           // 0x0110, unused
  (unsigned int)&undefinedHandler,           // 0x0114, unused
  (unsigned int)&undefinedHandler,           // 0x0118, unused
  (unsigned int)&undefinedHandler,           // 0x011C, unused
  (unsigned int)&undefinedHandler,           // 0x0120, unused
  (unsigned int)&undefinedHandler,           // 0x0124, unused
  (unsigned int)&undefinedHandler,           // 0x0128, unused
  (unsigned int)&undefinedHandler,           // 0x012C, unused
  (unsigned int)&undefinedHandler,           // 0x0130, unused
  (unsigned int)&undefinedHandler,           // 0x0134, unused
  (unsigned int)&undefinedHandler,           // 0x0138, unused
  (unsigned int)&undefinedHandler,           // 0x013C, unused
  (unsigned int)&undefinedHandler,           // 0x0140, unused
  (unsigned int)&undefinedHandler,           // 0x0144, unused
  (unsigned int)&undefinedHandler,           // 0x0148, unused
  (unsigned int)&undefinedHandler,           // 0x014C, unused
  (unsigned int)&undefinedHandler,           // 0x0150, unused
  (unsigned int)&undefinedHandler,           // 0x0154, unused
  (unsigned int)&undefinedHandler,           // 0x0158, unused
  (unsigned int)&undefinedHandler,           // 0x015C, unused
  (unsigned int)&undefinedHandler,           // 0x0160, unused
  (unsigned int)&undefinedHandler,           // 0x0164, unused
  (unsigned int)&undefinedHandler,           // 0x0168, unused
  (unsigned int)&undefinedHandler,           // 0x016C, unused
  (unsigned int)&undefinedHandler,           // 0x0170, unused
  (unsigned int)&undefinedHandler,           // 0x0174, unused
  (unsigned int)&undefinedHandler,           // 0x0178, unused
  (unsigned int)&undefinedHandler,           // 0x017C, unused
  (unsigned int)&undefinedHandler,           // 0x0180, unused
  (unsigned int)&undefinedHandler,           // 0x0184, unused
  (unsigned int)&undefinedHandler,           // 0x0188, unused
  (unsigned int)&undefinedHandler,           // 0x018C, unused
  (unsigned int)&undefinedHandler,           // 0x0190, unused
  (unsigned int)&undefinedHandler,           // 0x0194, unused
  (unsigned int)&undefinedHandler,           // 0x0198, unused
  (unsigned int)&undefinedHandler,           // 0x019C, unused
  (unsigned int)&undefinedHandler,           // 0x01A0, unused
  (unsigned int)&undefinedHandler,           // 0x01A4, unused
  (unsigned int)&undefinedHandler,           // 0x01A8, unused
  (unsigned int)&undefinedHandler,           // 0x01AC, dummy
  (unsigned int)&undefinedHandler,           // 0x01B0, dummy
  (unsigned int)&undefinedHandler,           // 0x01B4, dummy
  (unsigned int)&undefinedHandler,           // 0x01B8, dummy
  (unsigned int)&undefinedHandler,           // 0x01BC, dummy
  (unsigned int)&undefinedHandler,           // 0x01C0, dummy
  (unsigned int)&undefinedHandler,           // 0x01C4, dummy
  (unsigned int)&undefinedHandler,           // 0x01C8, dummy
  (unsigned int)&undefinedHandler,           // 0x01CC, dummy
  (unsigned int)&undefinedHandler,           // 0x01D0, dummy
  (unsigned int)&undefinedHandler,           // 0x01D4, dummy
  (unsigned int)&undefinedHandler,           // 0x01D8, dummy
  (unsigned int)&undefinedHandler,           // 0x01DC, dummy
  (unsigned int)&undefinedHandler,           // 0x01E0, dummy
  (unsigned int)&undefinedHandler,           // 0x01E4, dummy
  (unsigned int)&undefinedHandler,           // 0x01E8, dummy
  (unsigned int)&undefinedHandler,           // 0x01EC, dummy
  (unsigned int)&undefinedHandler,           // 0x01F0, dummy
  (unsigned int)&undefinedHandler,           // 0x01F4, dummy
  (unsigned int)&undefinedHandler,           // 0x01F8, dummy
  (unsigned int)&undefinedHandler            // 0x01FC, dummy
};


void undefinedHandler(void)
{
	for(;;);
}

void HardFaultdHandler(void)
{
	for(;;);
}
