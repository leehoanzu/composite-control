#include <TV_PICKIT2_SHIFT_LCD.C>
#define   giay_htai      0x55
#define   phut_htai      0x33
#define   gio_htai       0x14
#define   thu_htai       7
#define   ngay_htai      0x20
#define   thang_htai     0x12
#define   nam_htai       0x23      
#define   ma_ds          0x96

#define  addr_wr_13b07   0xd0
#define  addr_rd_13b07   0xd1
#define  addr_mem        0x00

unsigned  char    nam_ds13,thang_ds13,ngay_ds13,thu_ds13,gio_ds13,
                  phut_ds13,giay_ds13,ma_ds13,control_ds13,giaytam;
                               
void thiet_lap_thoi_gian_hien_tai()
{     giay_ds13   =   giay_htai;       phut_ds13  =    phut_htai;   
      gio_ds13    =   gio_htai;        thu_ds13   =    thu_htai;
      ngay_ds13   =   ngay_htai;       thang_ds13 =    thang_htai;   
      nam_ds13    =   nam_htai;   
      control_ds13 = 0x90;             ma_ds13      =   ma_ds;
}
 
void nap_thoi_gian_htai_vao_ds1307()
{     i2c_start();         
      i2c_write(addr_wr_13b07);    
      i2c_write(0x00);
      i2c_write(giay_ds13);        i2c_write(phut_ds13);      
      i2c_write(gio_ds13);         i2c_write(thu_ds13);      
      i2c_write(ngay_ds13);        i2c_write(thang_ds13);            
      i2c_write(nam_ds13);         i2c_write(control_ds13);   
      i2c_write(ma_ds13);
      i2c_stop();
}
    
void doc_thoi_gian_tu_realtime()
{   
      i2c_start();
      i2c_write(addr_wr_13b07);
      i2c_write(addr_mem);                     
      i2c_start();                
                
      i2c_write(addr_rd_13b07);
      giay_ds13      =  i2c_read();              
      phut_ds13      =  i2c_read();             
      gio_ds13       =  i2c_read();             
      thu_ds13       =  i2c_read(); 
      ngay_ds13      =  i2c_read(); 
      thang_ds13     =  i2c_read(); 
      nam_ds13       =  i2c_read(); 
      control_ds13   =  i2c_read();          
      ma_ds13        =  i2c_read(0);     //not ack
      i2c_stop();
}    

   
void doc_giay_tu_realtime()
{   
      i2c_start();
      i2c_write(addr_wr_13b07);
      i2c_write(addr_mem);                     
      i2c_start();                
                
      i2c_write(addr_rd_13b07);
      giay_ds13      =  i2c_read(0);              
      i2c_stop();
}  
        
 
void hien_thi_thoi_gian_ds1307 ( )
{     lcd_command (0x8c);   
      lcd_data(gio_ds13/16  +0x30);    lcd_data(gio_ds13%16  +0x30);
      lcd_data(' ');
      lcd_data(phut_ds13/16 +0x30);    lcd_data(phut_ds13%16 +0x30);
      lcd_data(' ');
      lcd_data(giay_ds13/16 +0x30);    lcd_data(giay_ds13%16 +0x30);
      lcd_command (0xcc);  
      lcd_data(ngay_ds13/16 +0x30);    lcd_data(ngay_ds13%16 +0x30);
      lcd_data(' ');
      lcd_data(thang_ds13/16 +0x30);   lcd_data(thang_ds13%16 +0x30);
      lcd_data(' ');
      lcd_data(nam_ds13/16 +0x30);     lcd_data(nam_ds13%16 +0x30); 
      lcd_command (0x88); 
//!      lcd_data(thu_ds13/16 + 0x30);	  	
//!      if ( thu_ds13 == 8)	lcd_data("cn");
//!      else						lcd_data(thu_ds13%16 + 0x30);
}
 

