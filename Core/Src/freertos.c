/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "iwdg.h"
#include "usbd_cdc_if.h"
#include "ADF4351.h"
#include "gpio.h"
#include "i2c.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
float freq;
int atten;

extern I2C_HandleTypeDef hi2c1;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId mainTaskHandle;
osThreadId usbcomTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
float FreqGet(char *freqstr);       //获取USB串口收到的频率MHz
float AttenGet(char *attenstr);     //获取USB串口收到的衰减值DB
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void MainTask(void const * argument);
void USBComTask(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of mainTask */
  osThreadDef(mainTask, MainTask, osPriorityNormal, 0, 128);
  mainTaskHandle = osThreadCreate(osThread(mainTask), NULL);

  /* definition and creation of usbcomTask */
  osThreadDef(usbcomTask, USBComTask, osPriorityNormal, 0, 128);
  usbcomTaskHandle = osThreadCreate(osThread(usbcomTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_MainTask */
/**
* @brief Function implementing the mainTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MainTask */
void MainTask(void const * argument)
{
  /* USER CODE BEGIN MainTask */
    int countnum=0;

//  test new EEPROM 	
//	char FAtemp[4] = {0xFF,0xFF,0xFF,0xFF};	
//	HAL_I2C_Mem_Write(&hi2c1,AT24C02_Write,0,I2C_MEMADD_SIZE_8BIT, (uint8_t *)&FAtemp,4,10000);
//	HAL_Delay(30);	
//	HAL_I2C_Mem_Write(&hi2c1,AT24C02_Write,8,I2C_MEMADD_SIZE_8BIT, (uint8_t *)&FAtemp,4,10000);	
	
#ifdef EEPROM_SAVE 					
	usb_printf("EEPROM AT24C02 SAVE FUNCTION\r\n");
	HAL_I2C_Mem_Read(&hi2c1,AT24C02_Read,0,I2C_MEMADD_SIZE_8BIT,(uint8_t *)&freq,4,10000);
	HAL_Delay(30);	
	HAL_I2C_Mem_Read(&hi2c1,AT24C02_Read,8,I2C_MEMADD_SIZE_8BIT,(uint8_t *)&atten,4,10000);		
#endif
	if((atten < 0) || (atten > 32))		atten = ATTENINIT;  		//衰减超出
	if((freq < 35) || (freq >= 4400))  	freq  = FREQUENCYINIT;    	//场强超出
    HMC_WriteData(atten);  
    ADF4351Init();
    ADF4351WriteFreq(freq);   
  /* Infinite loop */
  for(;;)
  {      
    if(countnum % 20 == 0)   HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);    //指示灯
      
    if(countnum % 50 == 0) HAL_IWDG_Refresh(&hiwdg);    					//0.5秒喂狗，1.6秒溢出
    
    //if(countnum % 100 == 10) usb_printf("countnum=%d\r\n", countnum);     //1秒,输出计数，测试用
	  
    if(countnum % 1000 == 20){
		usb_printf("Current Frequency=%.2f\r\n", freq);        				//10秒,调试输出  
		usb_printf("Current atten value=%d\r\n", atten);  
	} 
	
    if(countnum % 3600000 == 10000){	                 //定时10小时间隔，刷新频率，初始化
		//atten = 20;                                	//衰减20/2=10dB 测试用
        HMC_WriteData(atten);  
        ADF4351Init();
        ADF4351WriteFreq(freq);    
    }
           
    if(countnum % 360000 == 5000){	                     //定时1小时间隔，刷新频率
		//atten = 20;                                	//衰减20/2=10dB 测试用
        HMC_WriteData(atten);  
        ADF4351WriteFreq(freq);    
    }      
      
    countnum++;
    if(countnum > 8640000)   countnum = 0;      		//24小时清0
      
    osDelay(10);                                		//10毫秒定时
  }
  /* USER CODE END MainTask */
}

/* USER CODE BEGIN Header_USBComTask */
/**
* @brief Function implementing the usbcomTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_USBComTask */
void USBComTask(void const * argument)
{
  /* USER CODE BEGIN USBComTask */
  const char *strptr;	    //指针变量；
  char tempstr[30];         //临时字符串（USB串口接收到的数据缓存）    
  int i;                    //临时变量
  float FAtemp;
  uint32_t NotifyValue;
  
  int attentemp;
  float freqtemp;
  //uint8_t j;

  /* Infinite loop */
  for(;;)
  {
      NotifyValue=ulTaskNotifyTake(pdTRUE,portMAX_DELAY);       //等待通知  from usbd_cdc_if.c
	  if(NotifyValue == 1){	
          if(RxLen){                                          //接收到usb口的指令
            usb_printf("recevied command= %s  rxlen = %d\r\n", pRx, RxLen);     //收到的命令
            if((RxLen < 2) || (RxLen > 10)){                          //命令长度不合法
                usb_printf("Length Wrong!!\r\n");
            }else{
                for (i = 0; i < RxLen; i++ )   tempstr[i] = *(pRx+i);	   //拷贝接收到的数据。 
                tempstr[RxLen] = 0;
                if(tempstr[0]==102) tempstr[0]=70;      //f-->F
                if(tempstr[0]== 97) tempstr[0]=65;       //a-->A
                /*判断频率数据*/
                strptr = strstr (tempstr, "F");         //判断收到频率数据
                if (strptr){
                    FAtemp = FreqGet(tempstr);
                    if(FAtemp != -1){                   //获取当前的频率
                        ADF4351WriteFreq(FAtemp);       //设置新的频率
						freq = FAtemp;
                        usb_printf("Get Frequency！%.2f\r\n", FAtemp);     
                        
						/*********************   IIC EEPROM frequency write   ************************/  
						if(HAL_I2C_Mem_Write(&hi2c1,AT24C02_Write,0,I2C_MEMADD_SIZE_8BIT, (uint8_t *)&FAtemp,4,10000)==HAL_OK)
						{
							usb_printf("EEPROM AT24C02 Write Test OK 1 \n");
						}
						else
						{
							usb_printf("EEPROM AT24C02 Write Test False");
							HAL_Delay(50);
						}
						HAL_Delay(30);
						HAL_I2C_Mem_Read(&hi2c1,AT24C02_Read,0,I2C_MEMADD_SIZE_8BIT,(uint8_t *)&freqtemp,4,10000);
					   
						usb_printf("%.2f  ",freqtemp);
						if (FAtemp == freqtemp)		usb_printf("frequency data write and read ok!\r\n");
						
						
                    } else{
                        usb_printf("Frequency wrong!(F35-F4400 MHz)\r\n");
                      }   
                    tempstr[0] = 0;                     //将临时字符串清0
				}    
                
                /*判断衰减数据*/
                strptr = strstr (tempstr, "A");         //判断收到衰减数据
                if (strptr){
                    FAtemp = AttenGet(tempstr);
                    if(FAtemp != -1){                   //设置新的衰减数据                    
                        usb_printf("Get attenuation！%.2f\r\n", FAtemp);   
                        atten =  (int) (FAtemp*2); 
                        usb_printf("set atten value=%d\r\n", atten);            
                        HMC_WriteData(atten);    
						
						    /*********************   IIC EEPROM atten write   ************************/  
						if(HAL_I2C_Mem_Write(&hi2c1,AT24C02_Write,8,I2C_MEMADD_SIZE_8BIT, (uint8_t *)&atten,4,10000)==HAL_OK)
						{
							usb_printf("EEPROM AT24C02 Write Test OK 1 \n");
						}
						else
						{
							usb_printf("EEPROM AT24C02 Write Test False");
							HAL_Delay(50);
						}
						HAL_Delay(30);
						HAL_I2C_Mem_Read(&hi2c1,AT24C02_Read,8,I2C_MEMADD_SIZE_8BIT,(uint8_t *)&attentemp,4,10000);
					   
						usb_printf("%d  ",attentemp);
						if (atten == attentemp)		usb_printf("atten data write and read ok!\r\n");
						
                    } else{
                        usb_printf("attenuation data wrong!(A0.5-32)\r\n");
                      } 
                    tempstr[0] = 0;                     //将临时字符串清0
                }           
            }
            RxLen = 0;
        }
    } 
      
    osDelay(20);
  }
  /* USER CODE END USBComTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE BEGIN Header_USBComTask */
/**
* @brief 获取字符串中的频率，
* @param freqstr: 字符串   格式：F2546.1
* @retval 正常：浮点数代表频率，单位MHz; 不正常返回：-1
*/
float FreqGet(char *freqstr)
{
    const char *i;
    float freqtemp;
	i = strstr (freqstr , "F");							        //截取场强数据 +CSQ: 21,99
    
    freqtemp = atof(i+1);
    if((freqtemp < 35) || (freqtemp >= 4400))  freqtemp = -1;     //场强超出，设成一个标志为10
    return freqtemp;
}

/**
* @brief 获取字符串中的衰减值
* @param freqstr: 字符串   格式：a11   11 dB
* @retval 正常：浮点数代表衰减值，单位dB  不正常返回：-1
*/
float AttenGet(char *attenstr)
{
    const char *i;
    float attentemp;
	i = strstr (attenstr , "A");							    //截取场强数据 +CSQ: 21,99
    
    attentemp = atof(i+1);
    if((attentemp < 0) ||(attentemp > 32))  attentemp = -1;      //场强超出，设成一个标志为10
    return attentemp;
}


