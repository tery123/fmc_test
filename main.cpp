#include "mbed.h"
 
Serial debug0(PD_8, PD_9);

InterruptIn button(PC_13);

volatile bool Read_flag;

SRAM_HandleTypeDef hsram1;

static void MX_GPIO_Init(void);
static void MX_FMC_Init(void);
static void Error_Handler(void);

uint8_t get_from_sram_3(uint32_t a);

uint32_t fsmc_addr;

void wait_ms_(uint32_t t)
{
	for (uint32_t i = 0; i < t; ++i)
	{
		wait_us(1000);
	}
}


uint8_t get_from_sram_3(uint32_t a)
{	
	uint8_t Sram_rx[4];

	fsmc_addr = 0x60000000 | a;

	HAL_SRAM_Read_8b(&hsram1, (uint32_t*)(0x60000000|a), Sram_rx, 1);

	return Sram_rx[0];
}

void Get_8b()
{	
	debug0.printf("Get_8b start\r\n");

	get_from_sram_3(0x80);
	get_from_sram_3(0x81);
	get_from_sram_3(0x82);
	get_from_sram_3(0x83);

}

int main() {
	
	MX_GPIO_Init();
 	MX_FMC_Init();

	debug0.baud(115200);

	Read_flag = false;

	//thread0.start(callback(&queue, &EventQueue::dispatch_forever));

	//button.rise(queue.event(Get_8b)); 
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


void Error_Handler()
{
	debug0.printf("Initail SRAM fail\r\n");
}

HAL_StatusTypeDef HAL_SRAM_Read_8b_alpha(SRAM_HandleTypeDef *hsram, uint32_t *pAddress, uint8_t *pDstBuffer, uint32_t BufferSize)
{
  uint32_t size;
  __IO uint8_t *psramaddress = (uint8_t *)pAddress;
  uint8_t * pdestbuff = pDstBuffer;
  HAL_SRAM_StateTypeDef state = hsram->State;

  debug0.printf("1\r\n");

  /* Check the SRAM controller state */
  if ((state == HAL_SRAM_STATE_READY) || (state == HAL_SRAM_STATE_PROTECTED))
  {	debug0.printf("2\r\n");
    /* Process Locked */
    __HAL_LOCK(hsram);

    /* Update the SRAM controller state */
    hsram->State = HAL_SRAM_STATE_BUSY;
    debug0.printf("3\r\n");
    /* Read data from memory */
    for (size = BufferSize; size != 0U; size--)
    {
    debug0.printf("4\r\n");
      *pdestbuff = *psramaddress;
      pdestbuff++;
      psramaddress++;
    }
    debug0.printf("5\r\n");
    /* Update the SRAM controller state */
    hsram->State = state;

    /* Process unlocked */
    __HAL_UNLOCK(hsram);
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}