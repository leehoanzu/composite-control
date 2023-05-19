const unsigned char maquetkey[4]= {0x3f,0x7f,0xbf,0xff};
unsigned int8  mpt1=0,mpt2=0;  
unsigned int key_nhan()
{     signed int8 maphim,hang,cot;
      maphim=hang=0xff;
      for(cot=0;cot<4;cot++)
            {   
               output_b(maquetkey[cot]);
               if         (!input(pin_b2))      {hang=3;   break;}
               else    if (!input(pin_b3))      {hang=2;   break;}
               else    if (!input(pin_b4))      {hang=1;   break;}
               else    if (!input(pin_b5))      {hang=0;   break;}
               delay_ms(30); //them vao de xu ly loi qua tai cpu
            }          
         if (hang!=0xff)   maphim   = cot*4 + hang;
         return(maphim);
}

unsigned int key_4x4_dw()
{     
      mpt1=key_nhan();       
      if (mpt1!=0xff)   
      {  delay_ms(1);  
         mpt1=key_nhan();
         do{mpt2=key_nhan(); delay_ms(30);}
         while (mpt2==mpt1);
      }             
      return(mpt1);
}

unsigned int key_4x4_up()
{            
      mpt1=key_nhan();
      if (mpt1!=0xff)   
      {  
         if (mpt1!=mpt2)
            {
               return(mpt1);  //phim moi
               mpt2=mpt1;
            }
         else
         {
            delay_ms(1);
            mpt1=key_nhan();
            do
            {
               mpt2=key_nhan();
               }
            while (mpt2!=mpt1);
               return(mpt1);     //thoat vi trung ma phim da nhan
               mpt2=mpt1;           
         }
      }  
      else 
      {
         return(mpt1);  //thoat vi khong nhan
         mpt2=mpt1;
      }      
}


