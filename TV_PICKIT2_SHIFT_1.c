#include    <18f4550.h>
#device     adc=10
#fuses      nowdt,put,hs,noprotect,nolvp,cpudiv1
#use        i2c(master,slow,sda=pin_b0,scl=pin_b1)
#use        delay(clock=20000000)
#use        rs232(baud=9600, xmit=pin_c6,rcv=pin_c7)
//nut nhan:
#define  bt0     pin_b5
#define  bt1     pin_b4
#define  bt2     pin_b3
#define  bt3     pin_b2

#define  on      bt0       //dk led don, motor
#define  off     bt1
#define  inv     bt2

#define  up      bt0    //dk led don
#define  dw      bt1
#define  clr     bt2
#define  mod     bt3

#define  stop    bt3

#define  on1     bt0     
#define  off1    bt1
#define  on2     bt2       
#define  off2    bt3


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#define      ssdo             pin_e2         //chung tat ca
#define      ssck             pin_e0         //chung tat ca

#define      g_all            pin_d0         //a - 4 byte
#define      rck_lcd20x4      pin_d2         //a - 2 byte dao
#define      rck_buzerelay    pin_d3     //b - 1 byte 
#define      rck_32led        pin_d4         //a - 4 byte
#define      rck_8ledquet     pin_d5         //b - 2 byte dao
#define      rck_4led7doan    pin_d6         //a - 4 byte
#define      rck_matranled    pin_d7         //b - 3 byte dao

void xuat_lcd20x4(unsigned int8 lcd_signal,lcd_ins_hthi);
void xuat_buzzer_relay();

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh kiem tra phim bt1
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
unsigned int8 dl_btn=0;
int1 phim_bt0(unsigned int16 dl)
{  
    if(!input(bt0))
    {  
      dl_btn++;
      if(dl_btn==1) return(1);
      else
      {
         if (dl_btn>= dl)
         dl_btn = 0;
      }
    }
    else   
    {
      return(0);
    }
} 
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh kiem tra phim bt1
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int1 phim_bt1(unsigned int16 dl )
{  
    if (!input(bt1))
    {  
      dl_btn++;
      if(dl_btn==1) return(1);
      else
      {
         if (dl_btn>= dl)
         dl_btn =0;
      }
    }
    else   
    {
      return(0);
    }
} 
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh kiem tra phim bt2      
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                                    
int1 phim_bt2(unsigned int16 dl )
{                                        
    if (!input(bt2))
    {  
      dl_btn++;
      if(dl_btn==1) return(1);
      else
      {
         if (dl_btn>= dl)
         dl_btn =0;
      }
    }
    else   
    {
      return(0);
    }                                          
}      

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh kiem tra phim bt3
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int1 phim_bt3(unsigned int16 dl,  unsigned int8 solan  )
{  
    if (!input(bt3))
    {  
      dl_btn++;
      if(dl_btn==1) return(1);
      else
      {
         if (dl_btn>= dl)
         dl_btn =solan;
      }
    }
    else   
    {
      //dl_btn =0;
      return(0);
    }
} 
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//khai bao du lieu 16 bit cho module relay, triac, buzzer, dong co
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
unsigned int16 rbdc;
#bit step_motor_enable  = rbdc.0    //1=ena,0=dis
#bit step_motor_in1     = rbdc.1    
#bit step_motor_in2     = rbdc.2
#bit step_motor_in3     = rbdc.3
#bit step_motor_in4     = rbdc.4
#bit dc_enable          = rbdc.5    //1=ena,0=dis
#bit pwrkey             = rbdc.6    //khoi tao sim900

#bit buzzer             = rbdc.8    //1=on,0=off
#bit triac_1            = rbdc.9    //1=on,0=off
#bit triac_2            = rbdc.10   //1=on,0=off
#bit relay_1            = rbdc.11   //0=on,1=off
#bit relay_2            = rbdc.12   //0=on,1=off



//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx   
const unsigned char ma7doan[16]= {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
0x80,0x90,0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};
#define     maso0    0xc0
#define     maso1    0xf9
#define     maso2    0xa4
#define     maso3    0xb0
#define     maso4    0x99
#define     maso5    0x92
#define     maso6    0x82
#define     maso7    0xf8

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
//ham 102 xuat 1 byte ra thanh ghi dich
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    
void xuat_1byte(unsigned int8 x)
{
   unsigned int8 sb;   
   #bit bseri  = x.7
   for(sb=0;sb<8;sb++)
      {                                                 
         output_bit(ssdo,bseri);    
         output_low(ssck); output_high(ssck);
         x= x<<1;         
     }
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
//ham 301
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con xuat 4 byte ra 32 led don
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void xuat_32led_don_4byte(unsigned int8 bld3,bld2,bld1,bld0)
{     
      xuat_1byte(bld3);          
      xuat_1byte(bld2);
      xuat_1byte(bld1);          
      xuat_1byte(bld0);            
                      
      output_high(rck_32led);    
      output_low(rck_32led);
    
}
//ham 302
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con xuat 2 word 16 bitra 32 led don
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void xuat_32led_don_2word(unsigned int16 wld1, unsigned int16 wld0)
{     
     unsigned int8  b3,b2,b1,b0;
     b3 = wld1>>8;  b2 = wld1; 
     b1 = wld0>>8;  b0 = wld0;
     xuat_32led_don_4byte(b3,b2,b1,b0);
}
//ham 303
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con xuat 1 double word ra 32 led don
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void xuat_32led_don_1dw(unsigned long long dwld)
{     
     unsigned int16  wd1,wd0;
     wd1 = dwld>>16;  wd0 = dwld;
     xuat_32led_don_2word(wd1,wd0);
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    
//xuat 1 bit roi tra lai ket qua sau khi dich di 1 bit
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    
unsigned  int8 xuat_1bit(unsigned int8 bytexuat)
{
      unsigned int8   xbitx;   
      #bit bserix  = xbitx.0
      xbitx = bytexuat;
                                            
      output_bit(ssdo,bserix);    
      output_low(ssck); output_high(ssck);
      xbitx= xbitx>>1;   
      return(xbitx);  
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    
//ham 103: xuat 1 word ra thanh ghi dich
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    
void xuat_1word(unsigned int16 w)
{  
   unsigned int8 b1,b0;
   b1 = w>>8; b0 = w;
   xuat_1byte(b1);    
   xuat_1byte(b0);
}   
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx     
//ham 104: xoa cac module 
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  
void xoa_tat_6module()                          
{                                                                                              
   xuat_1word(0);                             
   xuat_1word(0); 
   xuat_1word(0);                             
   xuat_1word(0);                                    
   
   output_high(rck_buzerelay);   
   output_low(rck_buzerelay);
   
   output_high(rck_32led);
   output_low(rck_32led); 
   
   output_high(rck_8ledquet);   
   output_low(rck_8ledquet);   
                                 
   output_high(rck_matranled);          
   output_low(rck_matranled);   
                           
   output_high(rck_lcd20x4);   
   output_low(rck_lcd20x4);      
                                                                          
   xuat_1word(0xffff); 
   xuat_1word(0xffff); 
   output_high(rck_4led7doan);       
   output_low(rck_4led7doan);                                                                      
}             
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
//ham 101
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    
//ham khoi tao cac port va ic chot
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    
void set_up_port_ic_chot()
{     
   set_tris_a(0xff);    set_tris_b(0x3c);                                                   
   set_tris_c(0xf1);    set_tris_e(0x00);                                               
   set_tris_d(0x00);    
   output_d(0x00); 
   xoa_tat_6module();
   port_b_pullups(0xff);          
   rbdc=0;               
            
   setup_adc(adc_clock_div_32);        
   setup_adc_ports(an0_to_an2|vss_vdd); 
   output_low(g_all);        
}
//ham 104

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con xuat 4 byte ra 4 led 7 doan
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void xuat_4led_7doan_4so(unsigned int bl743,bl742,bl741,bl740)
{     
      xuat_1byte(bl740);   xuat_1byte(bl741);
      xuat_1byte(bl742);   xuat_1byte(bl743);       
      output_low(rck_4led7doan);   
      output_high(rck_4led7doan);

}
//ham 404
void xuat_4led_7doan_3so(unsigned int bl742,bl741,bl740)
{     
     xuat_4led_7doan_4so(0xff,bl742,bl741,bl740);
}
//ham 403
void xuat_4led_7doan_2so(unsigned int bl741,bl740)
{     
     xuat_4led_7doan_4so(0xff,0xff,bl741,bl740);
}
//ham 402
void xuat_4led_7doan_1so(unsigned int bl740)
{     
     xuat_4led_7doan_4so(0xff,0xff,0xff,bl740);
}

//ham 405
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con hien thi du lieu 16 bit tren 4 led 7 doan
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
unsigned char donvi4,chuc4,tram4,ngan4;
void  xuat_4led_7doan_giaima_xoa_so0 (unsigned int16 tam)
{         
      donvi4 = ma7doan[tam %10];         
      chuc4  = ma7doan[tam/10%10];
      tram4  = ma7doan[tam/100%10]; 
      ngan4  = ma7doan[tam/1000%10];       
      if (ngan4==0xc0)
      {
         ngan4=0xff;
         if (tram4==0xc0)
         {
            tram4=0xff;
            if (chuc4==0xc0)   chuc4=0xff;
         }
      }
      xuat_4led_7doan_4so(ngan4,tram4,chuc4,donvi4);   
}

void  xuat_4led_7doan_2_so (unsigned int8 bh,bl)
{         
 xuat_4led_7doan_4so(ma7doan[bh/10],ma7doan[bh%10],ma7doan[bl/10],ma7doan[bl%10]);   
}

unsigned int8 lcddata=0; 
unsigned int8 lcdcontrol=0;
unsigned int8 glcddata=0;
unsigned int8 glcdcontrol=0; 
//ham 701
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con xuat du lieu 4 byte ra glcd va lcd
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void xuat_glcd_lcd()
{      
    xuat_1byte(glcdcontrol);             
    xuat_1byte(glcddata);
    xuat_1byte(lcdcontrol);       
    xuat_1byte(lcddata);      
         
    output_high(rck_lcd20x4);     
    output_low(rck_lcd20x4);
}
//ham 702
void xuat_lcd20x4(unsigned int8 lcd_signal,lcd_ins_hthi)
{     
    lcdcontrol = ~lcd_signal;
    lcddata    = ~lcd_ins_hthi;      
    xuat_glcd_lcd();
}
//ham 703
void xuat_glcd128x64(unsigned int8 glcd_signal,glcd_ins_hthi)
{     
    glcdcontrol = ~glcd_signal;
    glcddata    = ~glcd_ins_hthi;     
    xuat_glcd_lcd();
}
//ham 501
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con xuat 2 byte ra dk 1 led quet sang
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void xuat_8led_7doan_quet_1(unsigned int ma,so_hthi)
{     
      xuat_1byte(~ma);   
      xuat_1byte(~so_hthi);
      output_high(rck_8ledquet); 
      output_low(rck_8ledquet);
}
//ham 502
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con xuat 2 byte ra tat 8 led quet
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void xuat_8led_7doan_quet_tat_led()
{
   xuat_8led_7doan_quet_1(0xff,0xff);
}
//ham 503
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con hienthi led theo thu tu
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
const unsigned char ttledquet[8]= {0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};
void xuat_8led_7doan_quet_2(unsigned int thutuled,so_hthi)
{         
      xuat_1byte(~ (ttledquet[thutuled]));   
      xuat_1byte(~so_hthi);     
      output_high(rck_8ledquet); 
      output_low(rck_8ledquet);
}
//ham 504
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con hien thi 8 so tren 8 led quet - co kiem tra
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
unsigned char led_7dq[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
unsigned int8 tt8led=0;
void hien_thi_8led_7doan_quet()
{     
      for(tt8led=0;tt8led<8;tt8led++)
         if (led_7dq[tt8led]!=0xff)
         {           
            xuat_8led_7doan_quet_2(tt8led, led_7dq[tt8led]);
            delay_us(100);
            xuat_8led_7doan_quet_tat_led();           
            }
}
//ham 505
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con hien thi 8 so tren 8 led quet - khong kiem tra
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void hien_thi_8led_7doan_quet_all()
{     
      for(tt8led=0;tt8led<8;tt8led++)         
         {           
            xuat_8led_7doan_quet_2(tt8led, led_7dq[tt8led]);
            delay_us(100);
            xuat_8led_7doan_quet_tat_led();           
            }
}

void delay_quet_8led(unsigned int16 dl)
{ 
      unsigned int8 i;
      for (i=0; i<dl;i++)
      hien_thi_8led_7doan_quet_all();
} 

void giai_ma_gan_cho_8led_quet_16_xoa(unsigned int16 x)
{     
      led_7dq[0]= ma7doan [x %10];    
      led_7dq[1]= ma7doan [x/10%10];
      led_7dq[2]= ma7doan [x/100%10];    
      led_7dq[3]= ma7doan [x/1000%10];
      led_7dq[4]= ma7doan [x/10000%10];
      if (led_7dq[4]==maso0) 
      {
            led_7dq[4]=0xff;
            if (led_7dq[3]==maso0) 
            {
               led_7dq[3]=0xff;
               if (led_7dq[2]==maso0) 
               {
                  led_7dq[2]=0xff;
                  if (led_7dq[1]==maso0) 
                  {
                     led_7dq[1]=0xff;
                  }
               }
            }
      }
            
}


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx         
void xuat_2byte(unsigned int16 x)
{
   int8 sbb;
   #bit bserix  = x.15
   for (sbb=0;sbb<16;sbb++)
      {                                                 
         output_bit(ssdo,bserix);    
         output_low(ssck);         
         output_high(ssck);
         x= x<<1;         
     }
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con xuat ra led ma tran
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void xuat_matranled(unsigned int16 mahang,unsigned int16 macot1,
unsigned int16 macot2,unsigned int16 macot3)
{      
      xuat_2byte(macot3);
      xuat_2byte(macot2);
      xuat_2byte(macot1);
      xuat_2byte(mahang);     

      output_high(rck_matranled); 
      output_low(rck_matranled);
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con xuat ra led ma tran
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void tat_matranled()
{      
      xuat_2byte(0);
      xuat_2byte(0);
      xuat_2byte(0);
      xuat_2byte(0);           
      output_high(rck_matranled); output_low(rck_matranled); 
}
unsigned  int8 hieu_chinh_4bit_cao(unsigned int8 xzy)
{
      int1  btg;
      unsigned int8   bx;   
      #bit bit4  = bx.4
      #bit bit5  = bx.5
      #bit bit6  = bx.6
      #bit bit7  = bx.7
      bx=xzy;
      btg=bit4;   bit4=bit7;  bit7=btg;
      btg=bit5;   bit5=bit6;  bit6=btg;      
      return(bx);  
}
//ham 601
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//module dieu khien relay,triac, buzzer, dong co, sim
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void xuat_buzzer_relay()
{     
   unsigned int8 rbdc1,rbdc2;
      rbdc1=rbdc; rbdc2=rbdc>>8;      
      xuat_1byte(rbdc2);      
      xuat_1byte(rbdc1);
      
      output_high(rck_buzerelay);   
      output_low(rck_buzerelay);
}
//ham 602
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void buzzer_on()
{    
      buzzer=1;
      xuat_buzzer_relay();         
}
//ham 603
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void buzzer_off()
{     
      buzzer=0;
      xuat_buzzer_relay();
}
//ham 604
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void relay_1_on()
{    
      relay_1=1;
      xuat_buzzer_relay();         
}
//ham 603
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void relay_1_off()
{    
      relay_1=0;
      xuat_buzzer_relay();         
}
//ham 604
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void relay_2_on()
{    
      relay_2=1;
      xuat_buzzer_relay();         
}
//ham 607
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void relay_2_off()
{    
      relay_2=0;
      xuat_buzzer_relay();         
}
//ham 608
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void relay_1_relay_2_on()
{    
      relay_1=1;  relay_2=1;
      xuat_buzzer_relay();         
}
//ham 609
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void relay_1_relay_2_off()
{    
      relay_1=0;  relay_2=0;
      xuat_buzzer_relay();         
}
//ham 610
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void triac_1_on()
{    
      triac_1=1;
      xuat_buzzer_relay();         
}
//ham 611
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void triac_1_off()
{    
      triac_1=0;
      xuat_buzzer_relay();         
}
//ham 612
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void triac_2_on()
{    
      triac_2=1;
      xuat_buzzer_relay();         
}
//ham 613
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void triac_2_off()
{    
      triac_2=0;
      xuat_buzzer_relay();         
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void ggmmaa()
{
   unsigned int8   x;   
   x=ma7doan[1];
}

