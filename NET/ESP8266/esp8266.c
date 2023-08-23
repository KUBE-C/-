//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//�����豸����
#include "esp8266.h"

//Ӳ������
#include "Delay.h"
#include "usart.h"

//C��
#include <string.h>
#include <stdio.h>

#define ESP8266_WIFI_INFO		"AT+CWJAP=\"TP-LINK_4B5DA4\",\"\"\r\n"
//#define ESP8266_WIFI_INFO		"AT+CWJAP=\"wind\",\"12345678\"\r\n"

#define ESP8266_TCP1_INFO		"AT+CIPSTART=\"TCP\",\"10.101.129.100\",8800\r\n"

#define ESP8266_TCP2_INFO		"AT+CIPSTART=\"TCP\",\"192.168.103.31\",8082\r\n"

#define ESP8266_TCP3_INFO		"AT+CIPSTART=\"TCP\",\"10.101.129.103\",1234\r\n"

unsigned char esp8266_buf[256];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;


extern u8 humidityH;	  //ʪ����������

//==========================================================
//	�������ƣ�	ESP8266_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_Clear(void)
{

	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;

}

//==========================================================
//	�������ƣ�	ESP8266_WaitRecive
//
//	�������ܣ�	�ȴ��������
//
//	��ڲ�����	��
//
//	���ز�����	REV_OK-�������		REV_WAIT-���ճ�ʱδ���
//
//	˵����		ѭ�����ü���Ƿ�������
//==========================================================
_Bool ESP8266_WaitRecive(void)
{

	if(esp8266_cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;
		
	if(esp8266_cnt == esp8266_cntPre)				//�����һ�ε�ֵ�������ͬ����˵���������
	{
		esp8266_cnt = 0;							//��0���ռ���
			
		return REV_OK;								//���ؽ�����ɱ�־
	}
		
	esp8266_cntPre = esp8266_cnt;					//��Ϊ��ͬ
	
	return REV_WAIT;								//���ؽ���δ��ɱ�־

}

//==========================================================
//	�������ƣ�	ESP8266_SendCmd
//
//	�������ܣ�	��������
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
	
	unsigned char timeOut = 200;

	Usart_SendString(USART3, (unsigned char *)cmd, strlen((const char *)cmd));

	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//����յ�����
		{
			if(strstr((const char *)esp8266_buf, res) != NULL)		//����������ؼ���
			{
				ESP8266_Clear();									//��ջ���
				
				return 0;
			}
		}
		
		Delay_ms(10);
	}
	
	return 1;

}

//==========================================================
//	�������ƣ�	ESP8266_SendData
//
//	�������ܣ�	��������
//
//	��ڲ�����	data������
//				len������
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];
	
	ESP8266_Clear();								//��ս��ջ���
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n",len);		//��������
	if(!ESP8266_SendCmd(cmdBuf, ">"))				//�յ���>��ʱ���Է�������
	{
		Usart_SendString(USART3, data, len);		//�����豸������������

	}

}

//==========================================================
//	�������ƣ�	ESP8266_GetIPD
//
//	�������ܣ�	��ȡƽ̨���ص�����
//
//	��ڲ�����	�ȴ���ʱ��(����10ms)
//
//	���ز�����	ƽ̨���ص�ԭʼ����
//
//	˵����		��ͬ�����豸���صĸ�ʽ��ͬ����Ҫȥ����
//				��ESP8266�ķ��ظ�ʽΪ	"+IPD,x:yyy"	x�������ݳ��ȣ�yyy����������
//==========================================================
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{

	char *ptrIPD = NULL;
	
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//����������
		{
            UsartPrintf(USART_DEBUG,(char *)esp8266_buf);
			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//������IPD��ͷ
			if(ptrIPD == NULL)											//���û�ҵ���������IPDͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
			{
				//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
                //UsartPrintf(USART_DEBUG, "\"IPD\" found\r\n");
				ptrIPD = strchr(ptrIPD, ':');							//�ҵ�':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
                    //UsartPrintf(USART_DEBUG, "printf\r\n");
					return (unsigned char *)(ptrIPD);
				}
				else
                {
                    //UsartPrintf(USART_DEBUG," null");
					return NULL;
				}
			}
		}
		
		Delay_ms(5);		
		timeOut--	;	//��ʱ�ȴ�
	} while(timeOut > 0);
	
	return NULL;														//��ʱ��δ�ҵ������ؿ�ָ��

}


//==========================================================
//	�������ƣ�	ESP8266_Init
//
//	�������ܣ�	��ʼ��ESP8266
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_Init(void)
{
	
//	GPIO_InitTypeDef GPIO_Initure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//ESP8266��λ����
//	GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Initure.GPIO_Pin = GPIO_Pin_13;					//GPIOC13-��λ
//	GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOC, &GPIO_Initure);
//	
//	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
//	Delay_ms(250);
//	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
//	Delay_ms(500);
	
	ESP8266_Clear();
	
	UsartPrintf(USART_DEBUG, " 0. AT\r\n");
	while(ESP8266_SendCmd("AT\r\n", "OK")){
		Delay_ms(500);
    }
	
	UsartPrintf(USART_DEBUG, " 1. AT+RST\r\n");
	while(ESP8266_SendCmd("AT+RST\r\n", ""))
		Delay_ms(500);
	
	
	UsartPrintf(USART_DEBUG, " 2. CWMODE\r\n");
	while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"))
		Delay_ms(500);
	
	UsartPrintf(USART_DEBUG, " 3. AT+CWDHCP\r\n");
	while(ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))
		Delay_ms(500);
	
	UsartPrintf(USART_DEBUG, " 4. CWJAP\r\n");
	while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "OK"))
		Delay_ms(500);
	
//    UsartPrintf(USART_DEBUG, " 5. CIPSTART\r\n");
//	while(ESP8266_SendCmd(ESP8266_TCP1_INFO, "OK"))
//		Delay_ms(500);
//    UsartPrintf(USART_DEBUG, " TCP CONNECT\r\n");
	
//			UsartPrintf(USART_DEBUG, " 5. CIPSTART\r\n");
//	while(ESP8266_SendCmd(ESP8266_TCP2_INFO, "OK"))
//		Delay_ms(500);
//				
	UsartPrintf(USART_DEBUG, " 5. CIPSTART\r\n");
	while(ESP8266_SendCmd(ESP8266_TCP3_INFO, "OK"))
		Delay_ms(500);

	UsartPrintf(USART_DEBUG, " 6. CIPMODE\r\n");
	while(ESP8266_SendCmd("AT+CIPMODE=0\r\n", "OK"))
		Delay_ms(500);
	
	UsartPrintf(USART_DEBUG, " 7. ESP8266 Init OK\r\n");
	
}

//==========================================================
//	�������ƣ�	USART3_IRQHandler
//
//	�������ܣ�	����3�շ��ж�
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void USART3_IRQHandler(void)
{

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //�����ж�
	{
		if(esp8266_cnt >= sizeof(esp8266_buf))	esp8266_cnt = 0; //��ֹ���ڱ�ˢ��
		esp8266_buf[esp8266_cnt++] = USART3->DR;
		
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
	}

}
