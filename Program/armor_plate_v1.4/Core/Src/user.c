
#include "user.h"
#include "hx711.h"
#include "sk6812.h"
#include "cmsis_os.h"
#include <stdio.h>
#include "usart.h"
#include "flash.h"

/*private variables*/
uint16_t press_cnt;
uint16_t up_cnt;
uint16_t setting_cnt;
uint8_t mode;
uint8_t can_id = 0;
uint8_t state = 0;
uint16_t time_cnt = 0; 

void User_Task(void)
{
	mode = STANDBY;
	can_id = (Flash_Read16(FLASH_SAVE_ADDR)>>8)&0xFF;
	color = Flash_Read16(FLASH_SAVE_ADDR)&0xFF;
	osDelay(1000);
	while(1)
	{
		//����ģʽת��
		switch(mode)
		{
			case STANDBY:
				press_cnt = 0;
				up_cnt = 0;
				setting_cnt = 0;
				light_on = 1;
				if(ALL_PRESS)mode = PRESSING;  //��⵽��ѹ�����PRESSINGģʽ
				break;
			
			case PRESSING:
				press_cnt++;
				if(!ALL_PRESS)
					mode = PRESS_UP;
				if(press_cnt>SET_CANID_PRESS_TIME_MS) //����һ��ʱ�䣬��������canidģʽ
				{	
					can_id = 0;
					mode = SETTING;
				}
				break;
				
			case PRESS_UP:
				up_cnt++;
				if(ALL_PRESS)
					mode = PRESSING;
				if(up_cnt>UP_TIME_MS) //���� ���������󷵻�Ĭ��ģʽ
					mode = STANDBY;
				//��סһ��������ſ�
				if(press_cnt<SET_CANID_PRESS_TIME_MS && press_cnt>CHANGE_COLOR_PRESS_TIME_MS && up_cnt>UP_TIME_MS)
				{
					color = (color+1)%COLOR_COUNT; //�ı���ɫ
					uint16_t tmp = (color | (can_id << 8));
					Flash_Write16(FLASH_SAVE_ADDR,tmp); //������ɫֵ��flash
				}
				break;
			
			//����can id
			case SETTING:
				light_on = 0; //��������ģʽ��رյ�
				setting_cnt++;
				if(ALL_HIT)
				{
					can_id++; //��һ��canid��һ
					uint16_t tmp = (color | (can_id << 8));
					Flash_Write16(FLASH_SAVE_ADDR,tmp); //����can_id��flash
					osDelay(200); //�ȴ���˸��������
				}
				if(setting_cnt>SETTING_MAX_TIME_MS) //���������󷵻�Ĭ��ģʽ
					mode = STANDBY;
				break;
		}
	
#ifdef DEBUG_USER
		char buf[30];
		sprintf(buf,"m:%05d,%05d,%05d\n",press_cnt,up_cnt,setting_cnt>SETTING_MAX_TIME);
		HAL_UART_Transmit(&huart1,(uint8_t*)buf,20,0xFFFF);
#endif
		
		//�����ϵĵƵ���˸�����Ĵ�����ʾcan_id
		uint16_t blink_cnt = BLINK_LED_TIME_MS*can_id*2;
		if(++time_cnt%BLINK_LED_TIME_MS==0 && time_cnt<=blink_cnt)
			state = !state & 0x1;
		//���泣��һ��ʱ�䣬����ʱ�������
		else if(time_cnt>blink_cnt+STOP_BLINK_LED_TIME_MS)
			time_cnt = 0;
		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,(GPIO_PinState)state);
		
		osDelay(1); //���Ƹ���ʱ�䣬ȷ����ʱ׼ȷ
	}
}
