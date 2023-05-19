unsigned int8 lcd_control;  
#bit lcd_p = lcd_control.3
#bit lcd_rs = lcd_control.2
#bit lcd_rw = lcd_control.1
#bit lcd_e  = lcd_control.0

#define lcd_function_set       0x38
#define lcd_display_control    0x0c
#define lcd_clear_display      0x01
#define lcd_entry_mode         0x06
#define lcd_shift_left         0x18
#define lcd_shift_right        0x1c

#define lcd_addr_line1         0x80
#define lcd_addr_line2         0xc0
#define lcd_addr_line3         0x94
#define lcd_addr_line4         0xd4

void lcd_xuat_8bit(int8 lcd_data_x)
{
      lcd_e=1; xuat_lcd20x4(lcd_control,lcd_data_x);     
      lcd_e=0; xuat_lcd20x4(lcd_control,lcd_data_x); 
}
void lcd_command(int8 lcd_data_x)   
  {
      lcd_rs = 0;
      lcd_xuat_8bit(lcd_data_x);
      delay_us(20);
  }
void lcd_data(int8 lcd_data_x)       
  {
      lcd_rs = 1;     
      lcd_xuat_8bit(lcd_data_x);
      delay_us(20);
  }
  
void setup_lcd ()
{                     
         lcd_e  = 0; 
         lcd_rw = 0; 
         lcd_rs = 1; 
         lcd_p =0;
         lcd_command(lcd_function_set);         
         delay_us(40);
         lcd_command(lcd_display_control);   
         delay_us(40);
         lcd_command(lcd_clear_display);      
         delay_ms(2);
         lcd_command(lcd_entry_mode);       
         delay_us(40);                
}

const unsigned char lcd_so_x[16][6] ={
                  0,1,2,5,3,4,               // so 0
                  1,2,32,3,7,3,              // so 1
                  6,6,2,5,3,3,               // so 2
                  6,6,2,3,3,4,               // so 3
                  7,3,7,32,32,7,             // so 4
                  7,6,6,3,3,4,               // so 5
                  0,6,6,5,3,4,               // so 6
                  1,1,7,32,32,7,             // so 7
                  0,6,2,5,3,4,               // so 8
                  0,6,2,3,3,4						// so 9
                  32,32,32,32,32,32,			// xoa so 0 
                  7,6,6,3,3,4,					// chu s
                  7,6,7,7,32,32,					// chu p
                  32,32,32,32,3,32,				//dau cham
                  1,7,1,32,7,32					//chu t
                  7,32,32,7,3,3					// chu l
                  };		  	
                  
const unsigned char lcd_so_xx[10][9] ={
                  0,1,2,7,32,7,5,3,4,               // so 0
                  1,2,32,3,7,3,              // so 1
                  6,6,2,5,3,3,               // so 2
                  6,6,2,3,3,4,               // so 3
                  7,3,7,32,32,7,             // so 4
                  7,6,6,3,3,4,               // so 5
                  0,6,6,5,3,4,               // so 6
                  1,1,7,32,32,7,             // so 7
                  0,6,2,5,3,4,               // so 8
                  0,6,2,3,3,4};             // so 9                  
                  
const unsigned char lcd_ma_8doan[] = {
   0x07,0x0f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,  //doan f - 0
   0x1f,0x1f,0x1f,0x00,0x00,0x00,0x00,0x00,  //doan a - 1
   0x1c,0x1e,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,  //doan b - 2
   0x00,0x00,0x00,0x00,0x00,0x1f,0x1f,0x1f,  //doan d - 3
   0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1e,0x1c,  //doan c - 4
   0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x0f,0x07,  //doan e - 5
   0x1f,0x1f,0x1f,0x00,0x00,0x00,0x1f,0x1f,  //doan g+d-6 
   0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f}; //doan i  -7
   
void lcd_goto_xy(signed int8 x, signed int8 y)
{ 
   const unsigned int8 lcd_vitri[]={0x80,0xc0,0x94,0xd4};
   lcd_command(lcd_vitri[x]+y);
}

void xxx()
{ 
   signed int8 x;
   x= lcd_so_x[0][0];
   x= lcd_ma_8doan[0];
}

