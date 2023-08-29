
#include "comm.h"
#include "user.h"
#include "hx711.h"
#include "sk6812.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdarg.h>
#include "string.h"

extern UART_HandleTypeDef huart1;
extern CAN_HandleTypeDef hcan;

static CAN_TxHeaderTypeDef  protocol_tx_message;
static uint8_t              protocol_can_send_data[8];

void usart_printf(const char *fmt,...)
{
	static uint8_t tx_buf[256] = {0};
	static va_list ap;
	static uint16_t len;
	va_start(ap, fmt);
	//return length of string
	//�����ַ�������
	len = vsprintf((char *)tx_buf, fmt, ap);
	va_end(ap);
	HAL_UART_Transmit(&huart1,tx_buf,len,0xFF);
}

uint8_t Protocol_Send_CAN(void)
{
	uint32_t send_mail_box;
	protocol_tx_message.StdId = can_id+0x200;
	protocol_tx_message.IDE = CAN_ID_STD;
	protocol_tx_message.RTR = CAN_RTR_DATA;
	protocol_tx_message.DLC = 0x08;
	protocol_can_send_data[0] = ALL_HIT;
	if(HAL_CAN_AddTxMessage(&hcan, &protocol_tx_message, protocol_can_send_data, &send_mail_box) != HAL_OK)
	{
		//return HAL_ERROR;
	}
	//return HAL_OK;
	
}

/* Exported functions --------------------------------------------------------*/
/**
 *	@brief	CAN ��ʼ��
 */
void CAN_Init(void)
{
	// ����CAN��ʶ���˲���
	CAN_FilterTypeDef sFilterConfig;
	sFilterConfig.FilterIdHigh = 0;						
	sFilterConfig.FilterIdLow = 0;							
	sFilterConfig.FilterMaskIdHigh = 0;					// ������
	sFilterConfig.FilterMaskIdLow = 0;						// ������
	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO1;	// ������������FIFO0
	sFilterConfig.FilterBank = 0;							// ���ù�����0
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;		// ��ʶ������ģʽ
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;		// 32λ��
	sFilterConfig.FilterActivation = ENABLE;				// �����˲���
	sFilterConfig.SlaveStartFilterBank = 14;
	
	HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);
	HAL_CAN_Start(&hcan); // ����CAN
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO1_MSG_PENDING); // ʹ�ܽ����ж�
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rx_header;
	uint8_t rx_data[8];
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &rx_header, rx_data);
	color = (rx_header.StdId==0x200+can_id) ? WHITE : color; //����⵽��ͻ���Ʊ��ɫ
	switch (rx_header.StdId)
	{
		case 0x200:
			break;
	}
}

void Comm_Task(void)
{
	CAN_Init();
	osDelay(1000);
	uint8_t cnt = 0;
	while(1)
	{
		if(++cnt == 50) //500ms
		{
			usart_printf("\ncan_id:%2d\ncolor:%s\nall_hit:%1d\nall_press:%1d\n",
						can_id,COLOR_NAME(color),ALL_HIT,ALL_PRESS);
			cnt = 0;
		}
		Protocol_Send_CAN();
		osDelay(10);
	}
}

