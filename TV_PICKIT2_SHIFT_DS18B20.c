#define     touch_pin pin_a5
#include    <touch.c>
#define       skip_rom              0xcc
#define       convert_t             0x44
#define       read_scratchpad       0xbe
#define       write_scratchpad      0x4e
#define       copy_scratchpad       0x48    
unsigned int8     ds18al, ds18ah, ds18a_tam;
unsigned int16    ds18a;
void  khoi_tao_ds18b20()
{                         
      touch_present();
      touch_write_byte(skip_rom);      
      touch_write_byte(write_scratchpad);    
      touch_write_byte(0x0);       // ghi 2 byte rong
      touch_write_byte(0x0);       //
      touch_write_byte(0x1f);      //cau hinh do phan giai 9 bit

      touch_present();
      touch_write_byte(skip_rom);      
      touch_write_byte(copy_scratchpad); 
}
void  doc_giatri_ds18b20()
{ 
      touch_write_byte(skip_rom);
      touch_write_byte (convert_t);   
      touch_present();
      touch_write_byte(skip_rom);
      touch_write_byte(read_scratchpad);  
      ds18al = touch_read_byte();
      ds18ah = touch_read_byte();      
      ds18a   =  make16(ds18ah,ds18al);        
}
            

