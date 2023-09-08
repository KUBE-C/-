//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

#include "Data.h"
#include "usart.h"
#include "Led.h"
#include "AutoControl.h"
#include "Stepper.h"
#include "LED.h"
#include "Delay.h"
#include "Lcd.h"
#include "Beep.h"

//�����豸
#include "esp8266.h"
//C��
#include <string.h>
#include <stdio.h>
#include "cJSON.h"

//��������
void Data_Recieve(void)
{
    unsigned char *dataPtr = NULL;
    char len[lenSize]; // ����
    
    dataPtr = (unsigned char *)ESP8266_GetIPD(3,len);
    unsigned short dataLen =(unsigned short)atoi(len);
    
    if(dataPtr != NULL)
    {
        UsartPrintf(USART_DEBUG,(char *)dataPtr);
        //UsartPrintf(USART_DEBUG," %d",dataLen);
        Data_RevPro(dataPtr,dataLen);
    }
}


//==========================================================
//	�������ƣ�	Data_RevPro
//
//	�������ܣ�	�������ݼ��
//
//==========================================================
void Data_RevPro(unsigned char *req, unsigned short req_len) // ��json���д�����ִ����Ӧ�Ĳ���
{
    char *req_payload = (char *)req;
    
    //unsigned short req_len = 0;
    int value1, value2, value3;
    
    cJSON *json, *json_value;//����cJSON
	
    //����json��ʽreq_payload�����ݰ�
        json = cJSON_Parse(req_payload);
        if(!json) UsartPrintf(USART_DEBUG,"Error before: [%s]\n",cJSON_GetErrorPtr());
        else
        {
            json_value = cJSON_GetObjectItem(json, "TEM1");
            if (json_value) // �����¶���ֵ1
            {
                 value1 = atoi(json_value->valuestring);
                if (value1)
                {
                    UsartPrintf(USART_DEBUG, "json_value: [%d]\n", value1);
                    tempControl.TEM_FirstMaxValue = value1;
                }
            }
            
            json_value = cJSON_GetObjectItem(json, "TEM2");
            if (json_value) // �����¶���ֵ2
            {
                 value2 = atoi(json_value->valuestring);
                if (value2)
                {
                    UsartPrintf(USART_DEBUG, "json_value: [%d]\n", value2);
                    tempControl.TEM_SecondMaxValue = value2;
                }
            }
            
            json_value = cJSON_GetObjectItem(json, "LIGHT");
            if (json_value) // ���ù�����ֵ
            {
                 value3 = atoi(json_value->valuestring);
                if (value3)
                {
                    UsartPrintf(USART_DEBUG, "json_value: [%d]\n", value3);
                    lightControl.ENV_MaxValue = value3;
                    
                }
            }
           
            // ������ֵ
            value_change(value1,value2,value3);
            
            json_value = cJSON_GetObjectItem(json, "LED1");
            if(json_value)
            {
                int value = atoi(json_value->valuestring);
                if (value)
                {
                    UsartPrintf(USART_DEBUG, "json_value: [%d]\n", value);
                    LED1_ON();//��LED1
                    
                }
                else 
                {
                    LED1_OFF();//�ر�LED1
                }
            }
            
            json_value = cJSON_GetObjectItem(json, "AutoControl");
            if(json_value)
            {
                int value = atoi(json_value->valuestring);
                if (value)
                {
                    UsartPrintf(USART_DEBUG, "json_value: [%d]\n", value);
                    AutoControl_mode = 1; // �Զ�ģʽ
                }
             
            }
           
            if (AutoControl_mode == 0)
            {
                //�ɵ��� LEDlevel
                json_value = cJSON_GetObjectItem(json, "LEDlevel");
                if(json_value)
                {
                    int value = atoi(json_value->valuestring);
                    if (value)
                    {
                        UsartPrintf(USART_DEBUG, "json_value: [%d]\n", value);
                        TIM_SetCompare2(TIM3,value);
                    }
                 
                }
            }
      
            
        }
        cJSON_Delete(json);//ɾcJSON��
        
        ESP8266_Clear();//��ջ���
        
        //dataPtr = strchr(req_payload, '}');					//����'}'

//    if(dataPtr != NULL && result != -1)					//����ҵ���
//    {
//        dataPtr++;
//        
//        while(*dataPtr >= '0' && *dataPtr <= '9')		//�ж��Ƿ����·��������������
//        {
//            numBuf[num++] = *dataPtr++;
//        }
//        
//        num = atoi((const char *)numBuf);				//תΪ��ֵ��ʽ
//        
//    }
}


