#include    <18f4550.h>
#device     adc=10
#fuses      nowdt,put,hs,noprotect,nolvp,cpudiv1
#use        delay(clock=20000000)
#use        i2c(master,slow,sda=pin_b0,scl=pin_b1)

//#use        rs232(baud=9600, xmit=pin_c6,rcv=pin_c7)
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


#define     trigger      pin_e2
#define     echo         pin_e1
//   ic74138_a    pin_b6   //output giai ma 2 sang 4 tich cuc muc 0
//   ic74138_b    pin_b7   //output giai ma 2 sang 4 tich cuc muc 0

//          encoder  pin_c0   //rc0/t13cki

//          cb_kc    pin_a2   //ra2/an2
//          pir      pin_a3   //ra3/an3
//          cbxung   pin_a4   //ra4/t0cki
//          ledthu   pin_a4   //ra4/t0cki//sw802

//          cb_kcsa_trigger  pin_e2   //re2/sdo  dung chung
//          cb_kcsa_rcho     pin_e1   //re1

//          cb_tcs3200_out    pin_e1   //re1 //cam bien mau tcs3200
//          cb_tcs3200_s0     b12      //lay tu mbi5026
//          cb_tcs3200_s1     b13      //lay tu mbi5026
//          cb_tcs3200_s2     b14      //lay tu mbi5026
//          cb_tcs3200_s3     b15      //lay tu mbi5026

//          cb_power  sw803
//          cb_gas      ain3_pcf8591t   //re1
//          cb_quang    ain2_pcf8591t   
//          cb_lm35b    ain1_pcf8591t   //ra1/an1
//          cb_lm35a    ain0_pcf8591t   //re1/an0

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#define      enable_573a      pin_d1         //cs0 cua a
#define      enable_573b      pin_d0         //cs1 cua b
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#define      ssdo             pin_e2         //chung tat ca
#define      ssck             pin_e0         //chung tat ca
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#define      rck_4led7doan    pin_d7         //a - 4 byte
#define      g_4led7doan      pin_d6         //a - 4 byte
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    
#define      rck_matranled    pin_d7         //b - 3 byte dao
#define      g_matranled      pin_d6         //b - 3 byte dao
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#define      rck_32led        pin_d5         //a - 4 byte
#define      g_32led          pin_d4         //a - 4 byte
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
#define      rck_8ledquet     pin_d5         //b - 2 byte dao
#define      g_8ledquet       pin_d4         //b - 2 byte dao
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#define      rck_lcd20x4      pin_d3         //a - 2 byte dao
#define      g_lcd20x4         pin_d2         //a - 2 byte dao
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
#define      rck_buzerelay    pin_d3     //b - 1 byte 
#define      g_buzerelay      pin_d2         //b - 1 byte 

void xuat_lcd20x4(unsigned int8 lcd_signal,lcd_ins_hthi);

void xuat_buzzer_relay();
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx   
#define     chot_2_ic_74573_a_b  output_d(0xfc)

unsigned int8 tin_hieu_dk_74573_a;
#bit g_4a         = tin_hieu_dk_74573_a.6
#bit g_32a        = tin_hieu_dk_74573_a.4
#bit g_lcda       = tin_hieu_dk_74573_a.2
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx         
#define     mo_32_led_don        g_32a=0; //output_low(g_32led) 
#define     tat_32_led_don       g_32a=1; //output_high(g_32led) 
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx         
#define     mo_4_led_7doan       g_4a=0;  //output_low(g_4led7doan)
#define     tat_4_led_7doan      g_4a=1;  //output_high(g_4led7doan)
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
#define     mo_glcd_lcd           g_lcda=0;   //output_low(g_lcd20x4)
#define     tat_glcd_lcd          g_lcda=1;   // output_low(g_lcd20x4)

unsigned int8 tin_hieu_dk_74573_b;  
#bit g_mtb        = tin_hieu_dk_74573_b.6
#bit g_8b         = tin_hieu_dk_74573_b.4
#bit g_rbdcb      = tin_hieu_dk_74573_b.2
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
#define     mo_led_matran        g_mtb=0; //output_low(g_matranled)
#define     tat_led_matran       g_mtb=1; //output_low(g_matranled)
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx       
#define     mo_8_led_quet        g_8b=0;  //output_low(g_8ledquet)
#define     tat_8_led_quet       g_8b=1;  //output_low(g_8ledquet)
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx         
#define     mo_relay_buzzer_dc   g_rbdcb=0;  //output_low(g_buzerelay)
#define     tat_relay_buzzer_dc  g_rbdcb=1;  //output_low(g_buzerelay)

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh kiem tra phim bt1
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
unsigned int8 dl_btn=0;
int1 phim_bt0(unsigned int16 dl)
{  
    if (!input(bt0))
    {  
      dl_btn++;
      
      if (dl_btn>= dl)
         {
            dl_btn =0;
            return(1);
         }
      else return(0);
    }
    else    return(0);
} 
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh kiem tra phim bt1
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int1 phim_bt1(unsigned int16 dl )
{  
    if (!input(bt1))
    {  
      dl_btn++;
      
      if (dl_btn>= dl)
         {
            dl_btn =0;
            return(1);
         }
      else return(0);
    }
    else    return(0);
} 
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh kiem tra phim bt2      
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                                    
int1 phim_bt2(unsigned int16 dl )
{                                        
    if (!input(bt2))
    {  
      dl_btn++;
      
      if (dl_btn>= dl)
         {
            dl_btn =0;
            return(1);
         }
      else return(0);
    }
    else    return(0);
                                         
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

//ham 103
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con khoi tao ic chot
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx         
#define     cho_ic_74573_a_goi_du_lieu     output_high(enable_573a)
#define     chot_ic_74573_a_goi_du_lieu    output_low(enable_573a)

#define     cho_ic_74573_b_goi_du_lieu     output_high(enable_573b)
#define     chot_ic_74573_b_goi_du_lieu    output_low(enable_573b)
void mo_ic_74573_a_thong_dl();

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
//ham 102
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    
//xuat 1 byte ra thanh ghi dich
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    
void xuat_1byte(unsigned int8 bytexuat)
{
   unsigned int8   sb,x;   
   #bit bseri  = x.7
   x = bytexuat;
   for (sb=0;sb<8;sb++)
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
      mo_32_led_don;      
      mo_ic_74573_a_thong_dl();            
      output_high(rck_32led);    output_low(rck_32led);
      chot_ic_74573_a_goi_du_lieu;      
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

//ham 304
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con mo 32 led don sang
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void mo_32led_don()
{        
     xuat_32led_don_1dw(0xffffffff);
}
//ham 305
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con tat 32 led don 
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void tat_32led_don()
{        
     xuat_32led_don_1dw(0);
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
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
//ham 101
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    
//ham khoi tao cac port va ic chot
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    
   void set_up_port_ic_chot()
{     
      set_tris_d(0x00);    
      set_tris_e(0x00);
      output_d(0xff);
      tin_hieu_dk_74573_a=0xff;  
      tin_hieu_dk_74573_b=0xff;      
      chot_ic_74573_a_goi_du_lieu;
      chot_ic_74573_b_goi_du_lieu;  
      rbdc=0;
}
//ham 104
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    
//xuat các thieu dieu khien 
//giu nguyen cac trang thai cua 2 module khac
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    
void mo_ic_74573_a_thong_dl()
{     
      output_d(0x00);
      output_bit(g_32led,g_32a);
      output_bit(g_4led7doan,g_4a);
      output_bit(g_lcd20x4,g_lcda);  
      cho_ic_74573_a_goi_du_lieu;
}
void mo_ic_74573_b_thong_dl()
{     
      output_d(0x00);
      output_bit(g_matranled,g_mtb);
      output_bit(g_8ledquet,g_8b);
      output_bit(g_buzerelay,g_rbdcb); 
      cho_ic_74573_b_goi_du_lieu;
}
//ham 401
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con xuat 4 byte ra 4 led 7 doan
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void xuat_4led_7doan_4so(unsigned int bl743,bl742,bl741,bl740)
{     
      xuat_1byte(bl740);   xuat_1byte(bl741);
      xuat_1byte(bl742);   xuat_1byte(bl743);
      
      mo_4_led_7doan;
      mo_ic_74573_a_thong_dl();
       
      output_low(rck_4led7doan);   output_high(rck_4led7doan); 
      chot_ic_74573_a_goi_du_lieu;
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
      
    mo_glcd_lcd;
    mo_ic_74573_a_thong_dl();            
    output_high(rck_lcd20x4);     
    output_low(rck_lcd20x4);
    chot_ic_74573_a_goi_du_lieu;
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
      xuat_1byte(~ma);   xuat_1byte(~so_hthi);
      
      mo_8_led_quet;
      mo_ic_74573_b_thong_dl();
      
      output_high(rck_8ledquet); output_low(rck_8ledquet);
      chot_ic_74573_b_goi_du_lieu;
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
//chuong trinh con hien thi led theo thu tu
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
const unsigned char ttledquet[8]= {0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};
void xuat_8led_7doan_quet_2(unsigned int thutuled,so_hthi)
{         
      xuat_1byte(~ (ttledquet[thutuled]));   xuat_1byte(~so_hthi);     
      mo_8_led_quet;
      mo_ic_74573_b_thong_dl();
      output_high(rck_8ledquet); output_low(rck_8ledquet);
      chot_ic_74573_b_goi_du_lieu;
}
//ham 504
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//chuong trinh con hien thi 8 so tren 8 led quet - co kiem tra
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
unsigned int8 led_7dq[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
unsigned int8 tt8led=0;
void hien_thi_8led_7doan_quet()
{     
      for(tt8led=0;tt8led<8;tt8led++)
         if (led_7dq[tt8led]!=0xff)
         {           
            xuat_8led_7doan_quet_2(tt8led, led_7dq[tt8led]);
//!            delay_us(100);  //thuc te
            delay_ms(2);     //mo phong proteus
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
            //delay_us(100);  //thuc te
            delay_ms(20);     //mo phong proteus
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
void xuat_2byte(unsigned int16 byte2xuat)
{
   int8 sbb;
   unsigned int16   xx;   //short  bseri;
   #bit bserix  = xx.15
   xx = byte2xuat;
   for (sbb=0;sbb<16;sbb++)
      {                                                 
         output_bit(ssdo,bserix);    
         output_low(ssck);         
         output_high(ssck);
         xx= xx<<1;         
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
      
      mo_led_matran;
      mo_ic_74573_b_thong_dl();
      
      output_high(rck_matranled); output_low(rck_matranled); 
      chot_ic_74573_b_goi_du_lieu;
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
      output_high(rck_matranled); 
      output_low(rck_matranled);
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
      mo_relay_buzzer_dc;
      mo_ic_74573_b_thong_dl();
      
      output_high(rck_buzerelay);   output_low(rck_buzerelay);
      chot_ic_74573_b_goi_du_lieu;
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


