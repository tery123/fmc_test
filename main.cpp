#include "mbed.h"

Serial debug0(PD_8, PD_9);

InterruptIn button(PC_13);

SRAM_HandleTypeDef hsram1;

static void MX_GPIO_Init(void);
static void MX_FMC_Init(void);
static void Error_Handler(void);
/*
One time read 4 bytes by 8 bits data width
*/
bool get_from_sram_4(uint32_t addr,uint8_t* RX_data)
{
  return HAL_SRAM_Read_8b(&hsram1, (uint32_t*)(0x60000000|addr), RX_data, 4);
}

void Get_8b()
{
  uint8_t *ptr = new uint8_t[4]();

  debug0.printf("Get_8b start\r\n");

  if (get_from_sram_4(0x80,ptr))//address0x80~0x83
  {
    debug0.printf("Get_8b function success\r\n");
    debug0.printf("Byte 0=%X\r\n",*ptr);
    debug0.printf("Byte 1=%X\r\n",*(ptr+1));
    debug0.printf("Byte 2=%X\r\n",*(ptr+2));
    debug0.printf("Byte 3=%X\r\n",*(ptr+3));
  }
  else
    debug0.printf("Get_8b function fail\r\n");

  delete [] ptr;
}

int main() {

  MX_GPIO_Init();
  MX_FMC_Init();

  debug0.baud(115200);

  button.rise(Get_8b);

  debug0.printf("Start\r\n");

  while(1){}

}

void MX_GPIO_Init()
{
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FMC_NORSRAM_DEVICE;
  hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_8;
  hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_DISABLE;
  hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  hsram1.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
  hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 255;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

static void Error_Handler()
{
  debug0.printf("Initail SRAM fail\r\n");
}