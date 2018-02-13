// *****************************************************************************
// Filename    : GPIO.h
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
// Description : GPIO reg header file definition
//
// License     : GNU General Public License v3.0
//
// *****************************************************************************

#ifndef __GPIO_H__
#define __GPIO_H__



#define GPIOA_BASE 0x40010800UL
#define GPIOB_BASE 0x40010C00UL
#define GPIOC_BASE 0x40011000UL
#define GPIOD_BASE 0x40011400UL
#define GPIOE_BASE 0x40011800UL

typedef struct {

	unsigned int MODE0 :2;	
	unsigned int CN0   :2;
	unsigned int MODE1 :2;
	unsigned int CN1   :2;
	unsigned int MODE2 :2;
	unsigned int CN2   :2;
	unsigned int MODE3 :2;	
	unsigned int CN3   :2;
	unsigned int MODE4 :2;	
	unsigned int CN4   :2;
	unsigned int MODE5 :2;
	unsigned int CN5   :2;
	unsigned int MODE6 :2;
	unsigned int CN6   :2;
	unsigned int MODE7 :2;
	unsigned int CN7   :2;
	
}GPIOx_CRLStruct;

typedef struct {
	
	unsigned int MODE8  :2;	
	unsigned int CN8    :2;
	unsigned int MODE9  :2;
	unsigned int CN9    :2;
	unsigned int MODE10 :2;
	unsigned int CN10   :2;
	unsigned int MODE11 :2;	
	unsigned int CN11   :2;
	unsigned int MODE12 :2;	
	unsigned int CN12   :2;
	unsigned int MODE13 :2;
	unsigned int CN13   :2;
	unsigned int MODE14 :2;
	unsigned int CN14   :2;
	unsigned int MODE15 :2;
	unsigned int CN15   :2;	
	
}GPIOx_CRHStruct;

typedef struct {

	unsigned int IDR0     :1;
	unsigned int IDR1     :1;
	unsigned int IDR2     :1;
	unsigned int IDR3     :1;
	unsigned int IDR4     :1;
	unsigned int IDR5     :1;
	unsigned int IDR6     :1;
	unsigned int IDR7     :1;
	unsigned int IDR8     :1;
	unsigned int IDR9     :1;
	unsigned int IDR10    :1;
	unsigned int IDR11    :1;
	unsigned int IDR12    :1;
	unsigned int IDR13    :1;
	unsigned int IDR14    :1;
	unsigned int IDR15    :1;	
	unsigned int RESERVED :16;
}GPIOx_IDRStruct;


typedef struct {
	
	unsigned int ODR0     :1;
	unsigned int ODR1     :1;
	unsigned int ODR2     :1;
	unsigned int ODR3     :1;
	unsigned int ODR4     :1;
	unsigned int ODR5     :1;
	unsigned int ODR6     :1;
	unsigned int ODR7     :1;
	unsigned int ODR8     :1;
	unsigned int ODR9     :1;
	unsigned int ODR10    :1;
	unsigned int ODR11    :1;
	unsigned int ODR12    :1;
	unsigned int ODR13    :1;
	unsigned int ODR14    :1;
	unsigned int ODR15    :1;		
	unsigned int RESERVED :16;	

}GPIOx_ODRStruct;

typedef struct {
	unsigned int BSR0  :1;
	unsigned int BSR1  :1;
	unsigned int BSR2  :1;
	unsigned int BSR3  :1;
	unsigned int BSR4  :1;
	unsigned int BSR5  :1;
	unsigned int BSR6  :1;
	unsigned int BSR7  :1;
	unsigned int BSR8  :1;
	unsigned int BSR9  :1;
	unsigned int BSR10 :1;
	unsigned int BSR11 :1;
	unsigned int BSR12 :1;
	unsigned int BSR13 :1;
	unsigned int BSR14 :1;
	unsigned int BSR15 :1;	
	unsigned int BR0   :1;
	unsigned int BR1   :1;
	unsigned int BR2   :1;
	unsigned int BR3   :1;
	unsigned int BR4   :1;
	unsigned int BR5   :1;
	unsigned int BR6   :1;
	unsigned int BR7   :1;
	unsigned int BR8   :1;
	unsigned int BR9   :1;
	unsigned int BR10  :1;
	unsigned int BR11  :1;
	unsigned int BR12  :1;
	unsigned int BR13  :1;
	unsigned int BR14  :1;
	unsigned int BR15  :1;
	
}GPIOx_BSRStruct;

typedef struct {
	
	unsigned int BR0      :1;
	unsigned int BR1      :1;
	unsigned int BR2      :1;
	unsigned int BR3      :1;
	unsigned int BR4      :1;
	unsigned int BR5      :1;
	unsigned int BR6      :1;
	unsigned int BR7      :1;
	unsigned int BR8      :1;
	unsigned int BR9      :1;
	unsigned int BR10     :1;
	unsigned int BR11     :1;
	unsigned int BR12     :1;
	unsigned int BR13     :1;
	unsigned int BR14     :1;
	unsigned int BR15     :1;	
	unsigned int RESERVED :16;
	
}GPIOx_BRRStruct;

typedef struct {
	
	unsigned int LCK0     :1;
	unsigned int LCK1     :1;
	unsigned int LCK2     :1;
	unsigned int LCK3     :1;
	unsigned int LCK4     :1;
	unsigned int LCK5     :1;
	unsigned int LCK6     :1;
	unsigned int LCK7     :1;
	unsigned int LCK8     :1;
	unsigned int LCK9     :1;
	unsigned int LCK10    :1;
	unsigned int LCK11    :1;
	unsigned int LCK12    :1;
	unsigned int LCK13    :1;
	unsigned int LCK14    :1;
	unsigned int LCK15    :1;	
	unsigned int LCKK     :1;	
	unsigned int RESERVED :16;
	
}GPIOx_LCKStruct;


//Macro
#define GPIOx_CRL(base)   (*((volatile GPIOx_CRLStruct *)(base + 0x00u)))
#define GPIOx_CRH(base)   (*((volatile GPIOx_CRHStruct *)(base + 0x04u)))
#define GPIOx_IDR(base)   (*((volatile GPIOx_IDRStruct *)(base + 0x08u)))		
#define GPIOx_ODR(base)   (*((volatile GPIOx_ODRStruct *)(base + 0x0Cu)))		
#define GPIOx_BSR(base)   (*((volatile GPIOx_BSRStruct *)(base + 0x10u)))
#define GPIOx_BRR(base)   (*((volatile GPIOx_BRRStruct *)(base + 0x14u)))
#define GPIOx_LCK(base)   (*((volatile GPIOx_LCKStruct *)(base + 0x18u)))
	


/* Port A registers */
#define GPIOA_CRL  GPIOx_CRL(GPIOA_BASE)
#define GPIOA_CRH  GPIOx_CRH(GPIOA_BASE)
#define GPIOA_IDR  GPIOx_IDR(GPIOA_BASE)
#define GPIOA_ODR  GPIOx_ODR(GPIOA_BASE)
#define GPIOA_BSR  GPIOx_BSR(GPIOA_BASE)
#define GPIOA_BRR  GPIOx_BRR(GPIOA_BASE)
#define GPIOA_LCK  GPIOx_LCK(GPIOA_BASE)


/* Port B registers */
#define GPIOB_CRL  GPIOx_CRL(GPIOB_BASE)
#define GPIOB_CRH  GPIOx_CRH(GPIOB_BASE)
#define GPIOB_IDR  GPIOx_IDR(GPIOB_BASE)
#define GPIOB_ODR  GPIOx_ODR(GPIOB_BASE)
#define GPIOB_BSR  GPIOx_BSR(GPIOB_BASE)
#define GPIOB_BRR  GPIOx_BRR(GPIOB_BASE)
#define GPIOB_LCK  GPIOx_LCK(GPIOB_BASE)


/* Port C registers */
#define GPIOC_CRL  GPIOx_CRL(GPIOC_BASE)
#define GPIOC_CRH  GPIOx_CRH(GPIOC_BASE)
#define GPIOC_IDR  GPIOx_IDR(GPIOC_BASE)
#define GPIOC_ODR  GPIOx_ODR(GPIOC_BASE)
#define GPIOC_BSR  GPIOx_BSR(GPIOC_BASE)
#define GPIOC_BRR  GPIOx_BRR(GPIOC_BASE)
#define GPIOC_LCK  GPIOx_LCK(GPIOC_BASE)


/* Port D registers */
#define GPIOD_CRL  GPIOx_CRL(GPIOD_BASE)
#define GPIOD_CRH  GPIOx_CRH(GPIOD_BASE)
#define GPIOD_IDR  GPIOx_IDR(GPIOD_BASE)
#define GPIOD_ODR  GPIOx_ODR(GPIOD_BASE)
#define GPIOD_BSR  GPIOx_BSR(GPIOD_BASE)
#define GPIOD_BRR  GPIOx_BRR(GPIOD_BASE)
#define GPIOD_LCK  GPIOx_LCK(GPIOD_BASE)


/* Port E registers */
#define GPIOE_CRL  GPIOx_CRL(GPIOE_BASE)
#define GPIOE_CRH  GPIOx_CRH(GPIOE_BASE)
#define GPIOE_IDR  GPIOx_IDR(GPIOE_BASE)
#define GPIOE_ODR  GPIOx_ODR(GPIOE_BASE)
#define GPIOE_BSR  GPIOx_BSR(GPIOE_BASE)
#define GPIOE_BRR  GPIOx_BRR(GPIOE_BASE)
#define GPIOE_LCK  GPIOx_LCK(GPIOE_BASE)


#endif
