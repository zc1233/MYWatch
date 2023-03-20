#include "oled.h"
#include "font.h"    
#include "i2c.h"

uint8_t CMD_Data[]={
0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F,
					
0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12,
					
0xD8, 0x30, 0x8D, 0x14, 0xAF};    //��ʼ������
//��ʾ������
//x,y :�������
//num:��ֵ	 
//a:������λ��
//b:С����λ��
//size:�����С
void OLED_SHowfloat(uint8_t x,uint8_t y,float num,uint8_t a,uint8_t b,uint8_t size)
{
    if(num<0)
    {
        OLED_ShowChar(x,y,'-',size);
        x+=size/2;
        num=-num;
        
    OLED_ShowNum(x,y,num,a,16);
    OLED_ShowChar(x+a*(size/2),y,'.',16);
    for(int i=1;i<=b;i++)
        {
            OLED_ShowNum(x+(a+i)*(size/2),y,(uint16_t)(num*10)%10,1,size);
            num*=10;
        }
    }
    
    else
    {
    OLED_ShowNum(x,y,num,a,16);
    OLED_ShowChar(x+a*(size/2),y,'.',16);
    for(int i=1;i<=b;i++)
        {
            OLED_ShowNum(x+(a+i)*(size/2),y,(uint16_t)(num*10)%10,1,size);
            num*=10;
        }
    }
}
//ͨ��I2Cд������    
void WriteCmd(void)
{
	uint8_t i = 0;
	for(i=0; i<27; i++)
	{
		HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,CMD_Data+i,1,0x100);
	}
}
//���豸д��������
void OLED_WR_CMD(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,&cmd,1,0x100);    
}
//���豸д����
void OLED_WR_DATA(uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&data,1,0x100);
}
//��ʼ��oled��Ļ
void OLED_Init(void)
{ 	
	HAL_Delay(200);
 
	WriteCmd();
}
//����
void OLED_Clear(void)
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_CMD(0xb0+i);
		OLED_WR_CMD (0x00); 
		OLED_WR_CMD (0x10); 
		for(n=0;n<132;n++)
			OLED_WR_DATA(0);
	} 
}

//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC����
	OLED_WR_CMD(0X14);  //DCDC ON
	OLED_WR_CMD(0XAF);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC����
	OLED_WR_CMD(0X10);  //DCDC OFF
	OLED_WR_CMD(0XAE);  //DISPLAY OFF
}		   			 
void OLED_Set_Pos(uint8_t x, uint8_t y) 
{ 
	x += 2;
	OLED_WR_CMD(0xb0+y);
	OLED_WR_CMD(((x&0xf0)>>4)|0x10);
	OLED_WR_CMD(x&0x0f);
} 
 
void OLED_On(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_CMD(0xb0+i);    //����ҳ��ַ��0~7��
		OLED_WR_CMD(0x00);      //������ʾλ�á��е͵�ַ
		OLED_WR_CMD(0x10);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<132;n++)
			OLED_WR_DATA(1); 
	} //������ʾ
}
unsigned int oled_pow(uint8_t m,uint8_t n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~131
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ	
		if(x>127-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_DATA(F8X16[c*16+i]);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_DATA(F8X16[c*16+i+8]);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_DATA(F6x8[c][i]);
				
			}
}

//��ָ��λ�����һ���ַ�,���������ַ�
//x:0~131
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ClearChar(uint8_t x,uint8_t y,uint8_t Char_Size)
{      	
	unsigned char i=0;	
		if(x>127-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_DATA(0);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_DATA(0);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_DATA(0);
				
			}
}
 
//��ʾ�ַ���
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

//����ַ���
//x,y:����
//len:��������
//Char_size:�����С
void OLED_ClearStr(uint8_t x,uint8_t y,uint8_t len,uint8_t Char_Size)
{
	for (uint8_t i = 0; i < len ; i++)
	{
		if(x>120){x=0;y+=2;}
		OLED_ClearChar(x,y,Char_Size);
		x += 8;
	}
}
	

//��ʾͼƬ
//x,y:λ������
//w,h��ͼƬ��͸�
//Image��ͼƬ����
void OLED_ShowImage(uint8_t x,uint8_t y,uint8_t w,uint8_t h,uint8_t *Image)
{ 
	uint8_t j = 0; 
	
	if (h % 8 == 0) h = h / 8;
	else h = h / 8 + 1;
 	
	for (uint8_t i = 0; i < h; i++)
	{
		OLED_Set_Pos(x, y + i);
		for (uint8_t k = x; k < x + w; k++)
		{
			OLED_WR_DATA(Image[j++]);
		}
	}
}

//���ͼƬ
//x,y:λ������
//w,h��ͼƬ��͸�
void OLED_ClearImage(uint8_t x,uint8_t y,uint8_t w,uint8_t h)
{ 
	uint8_t j = 0; 
	
	if (h % 8 == 0) h = h / 8;
	else h = h / 8 + 1;
 	
	for (uint8_t i = 0; i < h; i++)
	{
		OLED_Set_Pos(x, y + i);
		for (uint8_t k = x; k < x + w; k++)
		{
			OLED_WR_DATA(0);
		}
	}
}

