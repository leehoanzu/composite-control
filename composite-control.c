#include <tv_pickit2_shift_1_proteus.c>
//!#include <tv_pickit2_shift_1.c>
#include <tv_pickit2_shift_key4x4_138.c>
#include <tv_pickit2_shift_ds18b20.c>
#include <tv_pickit2_shift_ds1307_i2c.c>

signed int8  i,  dv_ktl, ch_ktl, tr_ktl, j;
float lm35a, k_cach, clk_cach ;
unsigned int16 gt_do, kqadc, kq_kc ;
unsigned int8 solan, ch_ds18a, dv_ds18a, dv_rt, ch_rt, dv_kc, ch_kc, bdn;

#int_timer1 // khai bao ngat timer
void interrupt_timer1()
{
	bdn++;
	set_timer1(3036);
}
void doc_nhiet_do_lm35a() // do nhiet do lm35a
{
//!	 unsigned int16 lm35a
//!	 unsigned int8 solan
	set_adc_channel(0);
	lm35a = 0;
	for ( i = 0; i < solan; i++ )
		lm35a = lm35a + read_adc();
  	
  lm35a = lm35a /2.046;
  lm35a = lm35a / solan;	  	
  gt_do = lm35a * 10;
}

void lcd_hienthi_so_z_toado_xy(signed int8 lcd_so, x1, y1 ) // hien thi ki tu lon
{
	unsigned int8 i;
	lcd_goto_xy(x1, y1);
	for( i = 0; i < 6; i++)
	{
		if (i == 3)
			lcd_goto_xy(x1 + 1, y1);
		lcd_data(lcd_so_x[lcd_so][i]);  	
		hien_thi_8led_7doan_quet();
	}
}

void giai_ma_ktl(unsigned int16 x) // giai ma ki tu lon cho lcd
{
  	
	dv_ktl = x % 10;
	ch_ktl = x / 10 % 10;
	tr_ktl = x / 100;
	// xoa so 0 ki tu lon
	if ( tr_ktl == 0)
	{
		tr_ktl = 10;
		if( ch_ktl == 0)	ch_ktl = 10;
	}
  	
	ch_rt = (giay_ds13 / 16 % 16) ; //realtime
	dv_rt = (giay_ds13 % 16) ;
	// xoa so 0 vo nghia
	if ( ch_rt == 0)
		ch_rt = 10;  
}

void giaima_8led_7doan(unsigned int16 gt_do) // giai ma 8led 7 doan 7quet 
{
	led_7dq[0] = ma7doan[ gt_do % 10 ];
	led_7dq[1] = ma7doan[ gt_do / 10 % 10 ] & 0x7f;
	led_7dq[2] = ma7doan[ gt_do / 100 ];
	led_7dq[4] = ma7doan[ giay_ds13 % 16 ];
	led_7dq[5] = ma7doan[ giay_ds13/ 16 % 16 ];
	led_7dq[6] = 0xc8 & 0x7f;
	led_7dq[7] = 0xc6;
	if ( led_7dq[2] == 0xc0)
	{
		led_7dq[2] = 0xc0;
		if ( led_7dq[1] == 0xc0 )	led_7dq[1] = 0xff;
	}  	
  	
	if ( led_7dq[5] == 0xc0 )	led_7dq[5] = 0xff;
}

void hien_thi_lcd() // hien thi lcd
{
	//hien thi ds18a
	lcd_goto_xy ( 3, 0);
	lcd_data(dv_ds18a);
	lcd_goto_xy ( 3, 18);
	lcd_data(ch_ds18a);
}

void giai_ma_lcd(unsigned int16 ds18a) // giai ma cho lcd
{
	ch_ds18a = (ds18a / 10 % 10) + 0x30;
	dv_ds18a = (ds18a % 10) +0x30;
	// xoa so 0 vo nghia
	if ( ch_ds18a == 0x30)
		ch_ds18a = 0x20;

}

void giai_ma_hienthi_4led ( float tam )
{
	kq_kc = ( unsigned int16 ) tam;
	dv_kc = ma7doan[kq_kc % 10 ];
	ch_kc = ma7doan[kq_kc /10 % 10];
	if ( ch_kc == 0xc0 ) ch_kc = 0xff;
	xuat_4led_7doan_4so ( 0x47, ch_kc, dv_kc, ma7doan[7]);
}

void do_kc () // do khoang cach
{
	set_adc_channel (2);
	delay_ms ( 1 );
	kqadc = 0;
	for ( j = 0; j < 20; j++ )
	{
		kqadc = kqadc + read_adc ();
   	hien_thi_8led_7doan_quet();
	}
	kqadc = kqadc / 20;
  	
	if ( kqadc <= 80 )	k_cach = 80;
		else if ((kqadc > 80) & (kqadc < 87) )
		{
			clk_cach = kqadc - 80;
			k_cach = 80 - clk_cach/0.77;
		}
		else if ( kqadc == 87) k_cach = 70;
		else if ((kqadc > 87) & (kqadc < 106) )
		{
			clk_cach = kqadc - 87;
			k_cach = 70 - clk_cach/2.11;
		}
		else if ( kqadc == 106) k_cach = 60;
		else if ((kqadc > 106) & (kqadc < 124) )
		{
			clk_cach = kqadc - 106;
			k_cach = 60 - clk_cach / 2;
		}
		else if ( kqadc == 124) k_cach = 50;
		else if ((kqadc > 124) & (kqadc < 150) )
		{
			clk_cach = kqadc - 124;
			k_cach = 50 - clk_cach / 2.88;
		}
		else if ( kqadc == 150) k_cach = 40;
		else if ((kqadc > 150) & (kqadc < 196) )
		{
			clk_cach = kqadc - 150;
			k_cach = 40 - clk_cach/5.11;
		}
		else if ( kqadc == 196) k_cach = 30;
		else if ((kqadc > 196) & (kqadc < 276) )
		{
			clk_cach = kqadc - 196;
			k_cach = 30 - clk_cach/8.88;
		}
		else if ( kqadc == 276) k_cach = 20;
		else if ((kqadc > 276) & (kqadc < 490) )
		{
			clk_cach = kqadc - 276;
			k_cach = 20 - clk_cach/8,88;
		}
		else if ( kqadc == 490) k_cach =10;	  	
	  	
		hien_thi_8led_7doan_quet();
}

void hien_thi_ktl() // hien thi ki tu lon len lcd
{
	lcd_hienthi_so_z_toado_xy( dv_rt, 0, 8 );
	lcd_hienthi_so_z_toado_xy( tr_ktl, 2, 2);
	lcd_hienthi_so_z_toado_xy( ch_ktl, 2, 8 );
	lcd_hienthi_so_z_toado_xy( dv_ktl, 2, 11);
}

void main ()
{
	set_up_port_ic_chot();
  	
	//do nhiet do lm35a lm35b
	setup_adc(adc_clock_div_32);
//!	setup_adc_ports(an0_to_an1 | vss_vdd);
	setup_adc_ports(an0_to_an1 | vss_vdd); //khi su dung 1 kenh
  	
	setup_lcd(); // khoi tao lcd
  	
	khoi_tao_ds18b20(); 
  	
	//set timer
	setup_timer_1(t1_internal | t1_div_by_8);
	set_timer1(3036);
	enable_interrupts(global);
	enable_interrupts(int_timer1);
  	
	lcd_command(0x40);
	for( i = 0; i < 64; i++)
		lcd_data(lcd_ma_8doan[i]);
  	
  	
	doc_thoi_gian_tu_realtime ();  // realtime
	if (ma_ds13 != ma_ds)
   {
      thiet_lap_thoi_gian_hien_tai ();
      nap_thoi_gian_htai_vao_ds1307 () ;
   }   
   doc_thoi_gian_tu_realtime ();
   giaytam = giay_ds13;   

	solan = 10; gt_do = 0; bdn = 0;
	while ( true )
	{
		doc_nhiet_do_lm35a();
		//giai_ma_do_nhietdo_lm35(lm35a);
	  	
		giai_ma_ktl(gt_do); // giai ma ki tu lon
		lcd_hienthi_so_z_toado_xy( ch_rt, 0, 5);
		//hien thi len lcd
		hien_thi_ktl();
		lcd_goto_xy(3, 5);
		lcd_data(3);
	  	
		// hien thi 8 led 7 doan 
		giaima_8led_7doan(gt_do);
		if ( bdn < 30)
			hien_thi_8led_7doan_quet();
		else
			bdn = 30 -bdn;
	  	
		//do 18b20
		if(touch_present())	// do nhiet do ds18b20
		{
			doc_giatri_ds18b20();
			ds18a = ds18a >> 4;
			ds18a = ds18a & 0x0ff;
		}
		// hien thi tren lcd nhiet do tu ds18b20
		giai_ma_lcd(ds18a);
		hien_thi_lcd();
	  	
		giai_ma_hienthi_4led ( k_cach ); // hien thi khoang cach len 4 led 7 doan
	  	
		//realtime
		if (ma_ds13 != ma_ds) 
	   {
	      thiet_lap_thoi_gian_hien_tai ();
	      nap_thoi_gian_htai_vao_ds1307 () ;
	   }
	   // doc thoi gian thuc
	   doc_thoi_gian_tu_realtime ();
	}
}
