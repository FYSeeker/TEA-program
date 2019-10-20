#include "sys.h"
#include "usart.h"
#include "control.h"


 
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
  
void uart_init(u32 bound)
{
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
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
	u8 usart_flag=0,point_flag=0;
	u8 x=0;
	u8 x1=0,x2=0,x3=0,x4=0,x5=0;
	u8 x6=0,x7=0,x8=0,x9=0,x10=0,x11=0;
	u8 buf_1[]="UP:";
	u8 buf_2[]="UD:";
	u8 buf_3[]="VP:";
	u8 buf_4[]="VI:";
	u8 buf_5[]="s1:";
	u8 buf_6[]="s2:";
	u8 buf_7[]="s3:";
	u8 buf_8[]="s4:";
	u8 buf_9[]="s5:";
	u8 buf_10[]="s6:";
	u8 buf_11[]="st:";
	double temp=0;
	static s8 Steertemp = 20;
	
	if((USART_RX_STA&0x8000)==0x8000)	//���ݽ������
	{
		/*�ж����ݸ�ʽ*/
		for(i=0;i<3;i++)		
		{
			if(USART_RX_BUF[i]==buf_1[i])  x1++;
			if(USART_RX_BUF[i]==buf_2[i])  x2++;
			if(USART_RX_BUF[i]==buf_3[i])  x3++;
			if(USART_RX_BUF[i]==buf_4[i])  x4++;
			if(USART_RX_BUF[i]==buf_5[i])  x5++;
			if(USART_RX_BUF[i]==buf_6[i])  x6++;
			if(USART_RX_BUF[i]==buf_7[i])  x7++;
			if(USART_RX_BUF[i]==buf_8[i])  x8++;
			if(USART_RX_BUF[i]==buf_9[i])  x9++;
			if(USART_RX_BUF[i]==buf_10[i]) x10++;
			if(USART_RX_BUF[i]==buf_11[i]) x11++;
		}
		
		if(x1==3||x2==3||x3==3||x4==3||x5==3||x6==3||x7==3||x8==3||x9==3||x10==3||x11==3)	//��ʽ��ȷ,��������
		{
			if(x1==3)
			{
				usart_flag = 1;
				printf("UP: ");
			}	
			else if(x2==3)
			{
				usart_flag = 2;
				printf("UD: ");
			}
			else if(x3==3)
			{
				usart_flag = 3;
				printf("VP: ");
			}
			else if(x4==3)
			{
				usart_flag = 4;
				printf("VI: ");
			}
			else if(x5==3)
			{
				usart_flag = 5;
				printf("��ʽ��ȷ��");
			}
			else if(x6==3)
			{
				usart_flag = 6;
				printf("��ʽ��ȷ��");
			}
			else if(x7==3)
			{
				usart_flag = 7;
				printf("��ʽ��ȷ��");
			}
			else if(x8==3)
			{
				usart_flag = 8;
				printf("��ʽ��ȷ��");
			}
			else if(x9==3)
			{
				usart_flag = 9;
				printf("��ʽ��ȷ��");
			}
			else if(x10==3)
			{
				usart_flag = 10;
				printf("��ʽ��ȷ��");
			}
			else if(x11==3)
			{
				usart_flag = 11;
				printf("��ʽ��ȷ��");
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
					if(USART_RX_BUF[i]=='.')  point_flag = i;	//���С���㣨�ӱ�ʾ��
					i++;
				}
				digit = i;		//��¼λ�����ӱ�ʾ��
				
				if(point_flag!=0)	//��С����
				{
					/*������������*/
					n = point_flag-1-3;
					temp = 0;
					for(i=3;i<point_flag;i++)	
					{
						temp += (USART_RX_BUF[i]-'0')*Usart_Pow(10,n--);	
					}
					/*����С������*/
					n = 1;
					for(i=point_flag+1;i<digit;i++)
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
			
			switch(usart_flag)
			{
				/*PID����*/
				case 1:upPD.kp = temp;printf("UP = %f\r\n",upPD.kp);
					break;
				case 2:upPD.kd = temp;printf("UD = %f\r\n",upPD.kd);
					break;
				case 3:velocityPI.kp = temp;printf("VP = %f\r\n",velocityPI.kp);
					break;
				case 4:velocityPI.ki = temp;printf("VI = %f\r\n",velocityPI.ki);
					break;
				
				/*ǰ�����ҿ���*/
				case 5:Steer = -Steertemp;		//ƽ�⳵ǰ��
					break;
				case 6:Steer = Steertemp;		//ƽ�⳵����
					break;
				case 7:TurnValue=-2000;		//ƽ�⳵ת����ٶ� 
					break;
				case 8:TurnValue=2000;
					break;
				
				/*�˶��ٶȿ���*/
				case 9:Steertemp+=10;
					if(Steertemp>100)  Steertemp = 100;
					break;
				case 10:Steertemp-=10;
					if(Steertemp<0)  Steertemp = 0;
					break;

				default:Steer = 0, TurnValue = 0;
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

