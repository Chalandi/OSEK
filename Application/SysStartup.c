// *****************************************************************************
// Filename    : SysStartup.c
// 
// Core        : STM32F100RB Cortex(R)-M3
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
// Description : Crt0 for STM32F100RB
//
// License     : GNU General Public License v3.0
//
// *****************************************************************************

#if defined(__GNUC__) && !defined(__CC_ARM)

// ***************************************************************************************
//=========================================================================================
// types definitions
//=========================================================================================
typedef struct {

    unsigned long  sourceAddr;  /* Source Address (section in ROM memory) */
    unsigned long  targetAddr;  /* Target Address (section in RAM memory) */
    unsigned long  size;        /* length of section (bytes) */

} runtimeCopyTable_t;

typedef struct {

    unsigned long  Addr;  /* source Address (section in RAM memory) */
    unsigned long  size;  /* length of section (bytes) */

} runtimeClearTable_t;

//=========================================================================================
// linker variables
//=========================================================================================
extern const runtimeCopyTable_t __RUNTIME_COPY_TABLE[];
extern const runtimeClearTable_t __RUNTIME_CLEAR_TABLE[];
extern unsigned long __CTOR_LIST__[];

//=========================================================================================
// defines
//=========================================================================================
#define __STARTUP_RUNTIME_COPYTABLE   (runtimeCopyTable_t*)(&__RUNTIME_COPY_TABLE[0])
#define __STARTUP_RUNTIME_CLEARTABLE  (runtimeClearTable_t*)(&__RUNTIME_CLEAR_TABLE[0])
#define __STARTUP_RUNTIME_CTORS       (unsigned long*)(&__CTOR_LIST__[0])

#elif defined(__CC_ARM)

//=============================================================================
// Load region symbols (.data)
//=============================================================================
extern unsigned int Image$$ER_RW_DATA$$RW$$Base;
extern unsigned int Image$$ER_RW_DATA$$RW$$Length;

#define ROM_DATA_SECTION_BASE (unsigned int)&Image$$ER_RW_DATA$$RW$$Base
#define ROM_DATA_SECTION_SIZE (unsigned int)&Image$$ER_RW_DATA$$RW$$Length

//=============================================================================
// Execution region symbols (.data)
//=============================================================================
extern unsigned int Load$$ER_RW_DATA$$RW$$Base;

#define RAM_DATA_SECTION_BASE (unsigned int)&Load$$ER_RW_DATA$$RW$$Base

//=============================================================================
// Execution region symbols (.bss)
//=============================================================================
extern unsigned int Image$$ER_RW_DATA$$ZI$$Base;
extern unsigned int Image$$ER_RW_DATA$$ZI$$Length;
#define BSS_SECTION_BASE (unsigned int)&Image$$ER_RW_DATA$$ZI$$Base
#define BSS_SECTION_SIZE (unsigned int)&Image$$ER_RW_DATA$$ZI$$Length

#else
#error Error: Compiler startup-code dialect is not supported
#endif

//=============================================================================
// function prototype
//=============================================================================
void SysStartup_Init(void);
static void SysStartup_InitRam(void);
static void SysStartup_Memset(unsigned char* ptr,int value, unsigned int size);
static void SysStartup_Memcpy(unsigned char* target, const unsigned char* source, unsigned int size);
static void SysStartup_RunApplication(void);
static void SysStartup_Unexpected_Exit(void);
static void SysStartup_InitSystemClock(void);

//=============================================================================
// extern function prototype
//=============================================================================
int main(void)__attribute__((weak));

//=============================================================================
// macros
//=============================================================================
#if defined(__GNUC__) && !defined(__CC_ARM)
#define ENABLE_INTERRUPTS()  __asm("CPSIE I")
#define DISABLE_INTERRUPTS() __asm("CPSID I")
#elif defined(__CC_ARM)
#define ENABLE_INTERRUPTS()  __asm{CPSIE I}
#define DISABLE_INTERRUPTS() __asm{CPSID I}
#else
#error Error: Compiler inline assembly dialect is not supported
#endif

void SysStartup_Init(void);
static void SysStartup_InitRam(void);
#if defined(__GNUC__) && !defined(__CC_ARM)
static void SysStartup_InitCtors(void);
#elif defined(__CC_ARM)
#else
#error Error: Compiler startup-code dialect is not supported
#endif

//-----------------------------------------------------------------------------
/// \brief  Memset function
///
/// \param  unsigned char* ptr: pointer to the target memory
///                 int value : initliaziation value 
///         unsigned int size : size of the memory region to set
///
/// \return void
//-----------------------------------------------------------------------------
static void SysStartup_Memset(unsigned char* ptr,int value, unsigned int size)
{
  for(unsigned int i=0u;i<size;i++)
  {
    *((unsigned char*)(ptr+i)) = value;
  }
}
//-----------------------------------------------------------------------------
/// \brief  Memcpy function
///
/// \param         unsigned char* target: pointer to the target memory region
///         const unsigned char* source : pointer to the source memory region
///                   unsigned int size : size of the memory region to copy
///
/// \return void
//-----------------------------------------------------------------------------
static void SysStartup_Memcpy(unsigned char* target, const unsigned char* source, unsigned int size)
{
  for(unsigned int i=0u;i<size;i++)
  {
    *((unsigned char*)(target+i)) = *((const unsigned char*)(source+i));
  }
}
//-----------------------------------------------------------------------------
/// \brief  SysStartup_Init function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------
void SysStartup_Init(void)
{
  SysStartup_InitSystemClock(); /* Configure the system clock */

  SysStartup_InitRam();         /* Init .bss and .data sections */

#if defined(__GNUC__) && !defined(__CC_ARM)
  /* Initialize the non-local C++ objects */
  SysStartup_InitCtors();
#elif defined(__CC_ARM)
#else
#error Error: Compiler startup-code dialect is not supported
#endif

  SysStartup_RunApplication();  /* Call main function */  
}

//-----------------------------------------------------------------------------
/// \brief  SysStartup_InitRam function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------
static void SysStartup_InitRam(void)
{
#if defined(__GNUC__) && !defined(__CC_ARM)

  unsigned long index = 0;

  /* Clear Table */
  while((__STARTUP_RUNTIME_CLEARTABLE)[index].Addr != (unsigned long)-1 && (__STARTUP_RUNTIME_CLEARTABLE)[index].size != (unsigned long)-1)
  {
    for(unsigned int cpt = 0; cpt < (__STARTUP_RUNTIME_CLEARTABLE)[index].size; cpt++)
    {
      *(volatile unsigned char*)((unsigned long)((__STARTUP_RUNTIME_CLEARTABLE)[index].Addr) + cpt) = 0;
    }
    index++;
  }

  /* Copy Table */
  index = 0;
  while((__STARTUP_RUNTIME_COPYTABLE)[index].sourceAddr != (unsigned long)-1 &&
        (__STARTUP_RUNTIME_COPYTABLE)[index].targetAddr != (unsigned long)-1 &&
        (__STARTUP_RUNTIME_COPYTABLE)[index].size       != (unsigned long)-1
       )
  {
    for(unsigned int cpt = 0; cpt < (__STARTUP_RUNTIME_COPYTABLE)[index].size; cpt++)
    {
      *(volatile unsigned char*)((unsigned long)((__STARTUP_RUNTIME_COPYTABLE)[index].targetAddr) + cpt) = 
               *(volatile unsigned char*)((unsigned long)((__STARTUP_RUNTIME_COPYTABLE)[index].sourceAddr) + cpt);
    }
    index++;
  }

#elif defined(__CC_ARM)

  /* Copy .data from ROM to RAM */
  if(ROM_DATA_SECTION_SIZE > 0)
  {
    SysStartup_Memcpy((unsigned char*)ROM_DATA_SECTION_BASE,(const unsigned char*)RAM_DATA_SECTION_BASE,ROM_DATA_SECTION_SIZE);
  }

  /* Init .bss section */
  if(BSS_SECTION_SIZE > 0)
  {
    SysStartup_Memset((unsigned char*)BSS_SECTION_BASE,0,BSS_SECTION_SIZE);
  }

#else

#error Error: Compiler startup-code dialect is not supported

#endif
}

#if defined(__GNUC__) && !defined(__CC_ARM)

//-----------------------------------------------------------------------------------------
/// \brief Startup_InitCtors function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------------------
static void SysStartup_InitCtors(void)
{
  for(unsigned long entry = 1; entry <= (__STARTUP_RUNTIME_CTORS)[0]; entry++)
  {
    ((void (*)(void))((__STARTUP_RUNTIME_CTORS)[entry]))();
  }
}

#elif defined(__CC_ARM)
#else
#error Error: Compiler startup-code dialect is not supported
#endif

//-----------------------------------------------------------------------------
/// \brief  SysStartup_RunApplication function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------
static void SysStartup_RunApplication(void)
{
  if((unsigned int) main != 0) /* check the weak function */
  {
    /* Enable the interrupts */
    ENABLE_INTERRUPTS();

    /* Call the main function */
    (void)main();
  }

  SysStartup_Unexpected_Exit(); /* Catch unexpected exit from main or if main dosent exist */
}

//-----------------------------------------------------------------------------
/// \brief  SysStartup_Unexpected_Exit function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------
static void SysStartup_Unexpected_Exit(void)
{
  for(;;);
}

//-----------------------------------------------------------------------------
/// \brief  SysStartup_InitSystemClock function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------
static void SysStartup_InitSystemClock(void)
{
  /* Using 24 MHz from PLL as system clock */
  #define RCC_BASE 0x40021000UL
  #define RCC_CR  (*((volatile unsigned int *)(RCC_BASE + 0x00u)))
  #define RCC_CFG (*((volatile unsigned int *)(RCC_BASE + 0x04u)))
  #define RCC_APB2RSTR (*((volatile unsigned int *)(RCC_BASE + 0x0Cu)))
  #define RCC_APB2ENR (*((volatile unsigned int *)(RCC_BASE + 0x18u)))

  /* Setup the PLL prescaler to x6 (max 24 MHz) */
  RCC_CFG = 0x00100000UL;

  /* Enable the PLL */
  RCC_CR |= (1<<24);

  /* Set PLL (24 MHz) as system clock */
  RCC_CFG |= 0x00000002UL;

  /* Enable the Clock for GPIO Port A */
  RCC_APB2ENR |= (1<<2);

  /* Enable the Clock for GPIO Port C */
  RCC_APB2ENR |= (1<<4);

  /* Enable the Clock for alternate function */
  RCC_APB2ENR |= (1<<0);

 /* Enable the Clock for USART1 */
  RCC_APB2ENR |= (1<<14);
}
