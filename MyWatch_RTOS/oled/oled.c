#include "oled.h"
#include "font.h"    
#include "i2c.h"

uint8_t CMD_Data[]={
0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F,
					
0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12,
					
0xD8, 0x30, 0x8D, 0x14, 0xAF};    //初始化命令
//显示浮点数
//x,y :起点坐标
//num:数值	 
//a:整数的位数
//b:小数的位数
//size:字体大小
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
//通过I2C写入命令    
void WriteCmd(void)
{
	uint8_t i = 0;
	for(i=0; i<27; i++)
	{
		HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,CMD_Data+i,1,0x100);
	}
}
//向设备写控制命令
void OLED_WR_CMD(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,&cmd,1,0x100);    
}
//向设备写数据
void OLED_WR_DATA(uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&data,1,0x100);
}
//初始化oled屏幕
void OLED_Init(void)
{ 	
	HAL_Delay(200);
 
	WriteCmd();
}
//清屏
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

//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC命令
	OLED_WR_CMD(0X14);  //DCDC ON
	OLED_WR_CMD(0XAF);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC命令
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
		OLED_WR_CMD(0xb0+i);    //设置页地址（0~7）
		OLED_WR_CMD(0x00);      //设置显示位置—列低地址
		OLED_WR_CMD(0x10);      //设置显示位置—列高地址   
		for(n=0;n<132;n++)
			OLED_WR_DATA(1); 
	} //更新显示
}
unsigned int oled_pow(uint8_t m,uint8_t n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}

uint16_t my_pow(uint8_t x, uint8_t y)
{
	if (y == 0)return 1; 
	return y % 2 == 0 ? my_pow(x, y / 2) * my_pow(x, y / 2) : my_pow(x, y / 2) * my_pow(x, y / 2) * x;
}
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2)
{         	
	uint8_t temp=len - 1;		
	unsigned char c[len + 1];
	for(uint8_t t=0;t<len;t++)
	{
		c[t] = num / my_pow(10,temp) + '0';
		num = num % my_pow(10,temp);
		temp--;
	}
	c[len] = '\0';
	OLED_ShowString(x,y,c,8);
} 
//在指定位置显示一个字符,包括部分字符
//x:0~131
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值	
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

//在指定位置清除一个字符,包括部分字符
//x:0~131
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
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
 
//显示字符串
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

//清除字符串
//x,y:坐标
//len:清屏长度
//Char_size:字体大小
void OLED_ClearStr(uint8_t x,uint8_t y,uint8_t len,uint8_t Char_Size)
{
	for (uint8_t i = 0; i < len ; i++)
	{
		if(x>120){x=0;y+=2;}
		OLED_ClearChar(x,y,Char_Size);
		x += 8;
	}
}
	

//显示图片
//x,y:位置坐标
//w,h：图片宽和高
//Image：图片数组
void OLED_ShowImage(uint8_t x,uint8_t y,uint8_t w,uint8_t h,const uint8_t *Image)
{ 
	uint16_t j = 0; 
	
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

//清除图片
//x,y:位置坐标
//w,h：图片宽和高
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

