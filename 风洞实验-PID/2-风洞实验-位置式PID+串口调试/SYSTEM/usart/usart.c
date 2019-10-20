#include "sys.h"
#include "usart.h"
#include "pid.h"


//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void uart_init(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
		
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
     } 
} 
#endif	


/*m^n����������ֵ:m^n�η�*/
double Usart_Pow(double m,u8 n)
{
	double result=1;	 
	while(n--) result*=m;    
	return result;
}

/*���ڵ��Բ�������*/
void Usart1_Dispose()	//�������ݴ���
{
	u8 i,n,digit=0,scope=0;
	u8 usart_flog=0,point_flog=0;
	u8 x=0,x1=0,x2=0,x3=0,x4=0;
	u8 buf_1[]="KP:";
	u8 buf_2[]="KI:";
	u8 buf_3[]="KD:";
	u8 buf_4[]="EX:";
	double temp=0;
	
	if((USART_RX_STA&0x8000)==0x8000)	//���ݽ������
	{
		/*�ж����ݸ�ʽ*/
		for(i=0;i<3;i++)		
		{
			if(USART_RX_BUF[i]==buf_1[i])  x1++;
			if(USART_RX_BUF[i]==buf_2[i])  x2++;
			if(USART_RX_BUF[i]==buf_3[i])  x3++;
			if(USART_RX_BUF[i]==buf_4[i])  x4++;
		}
		
		if(x1==3||x2==3||x3==3||x4==3)	//��ʽ��ȷ,��������
		{
			if(x1==3)
			{
				usart_flog = 'P';
				printf("P: ");
			}	
			else if(x2==3)
			{
				usart_flog = 'I';
				printf("I: ");
			}
			else if(x3==3)
			{
				usart_flog = 'D';
				printf("D: ");
			}
			else if(x4==3)
			{
				usart_flog = 'E';
				printf("EX: ");
			}
			
			/*�ж����ݷ�Χ*/
			i = 3;
			while(USART_RX_BUF[i]!='\0')
			{
				if((USART_RX_BUF[i]>='0'&&USART_RX_BUF[i]<='9') || USART_RX_BUF[i]=='.')
				{
					x++;
				}
				i++;
			}
			if(x+3==i)	//��Χ����
			{
				scope = 1;		//��Ƿ�Χ����
			}
			else
			{
				printf("�����쳣,�����·��ͣ�����\r\n");
			}
			
			/*��������*/
			if(scope)
			{
				i = 3;
				while(USART_RX_BUF[i]!='\0')
				{
					if(USART_RX_BUF[i]=='.')  point_flog = i;	//���С���㣨�ӱ�ʾ��
					i++;
				}
				digit = i;		//��¼λ�����ӱ�ʾ��
				
				if(point_flog!=0)	//��С����
				{
					/*������������*/
					n = point_flog-1-3;
					temp = 0;
					for(i=3;i<point_flog;i++)	
					{
						temp += (USART_RX_BUF[i]-'0')*Usart_Pow(10,n--);	
					}
					/*����С������*/
					n = 1;
					for(i=point_flog+1;i<digit;i++)
					{
						temp += (USART_RX_BUF[i]-'0') * Usart_Pow(0.1,n++);
					}
					printf("%lf\t",temp);
				}
				else	//û��С����
				{
					n = digit-1-3;
					temp = 0;
					for(i=3;i<digit;i++)	
					{
						temp += (USART_RX_BUF[i]-'0')*Usart_Pow(10,n--);	
					}
					printf("%lf\t",temp);	
				}
				printf("\r\n");	
			}
			
			switch(usart_flog)
			{
				case'P':pid.Kp = temp;printf("Kp = %lf\r\n",pid.Kp);
					break;
				case'I':pid.Ki = temp;printf("Ki = %lf\r\n",pid.Ki);
					break;
				case'D':pid.Kd = temp;printf("Kd = %lf\r\n",pid.Kd);
					break;
				case'E':pid.expect = temp;printf("EX = %lf\r\n",pid.expect);
					break;
			}
		}
		else  printf("��ʽ���������·��ͣ�����\r\n");
		
		printf("\r\n");
		
		/*�������BUF*/
		i=0;
		while(USART_RX_BUF[i]!='\0')
		{
			USART_RX_BUF[i] = 0;
			i++;
		}
		USART_RX_STA = 0;		//����״̬������λ����0
	}
}
