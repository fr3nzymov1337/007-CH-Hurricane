#include "../client.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* MENUTILTE_CH = "007 Hook v.III\0";

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern bool oglSubtractive;

extern menu_entrys menuEntry[1000];
extern int menuSelect, menuItems, MenuActive;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct start_end
{
	int xe_i;
	int ye_i;
	int xe_d;
	int ye_d;
	////////////
	int xs_i;
	int ys_i;
	int xs_d;
	int ys_d;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct start_end MenuModule1(int x, int y, int w, int h, int box, int len, int r, int g, int b, int a)
{
	struct start_end ends;
	
	FillRGBA( x+1,       y,         w-1,    box, 0,60,220,200);

	int i=0;
	while(i<=len)
	{
		if(i>=(len-1));
		else
		{
//			oglSubtractive = true;
			FillRGBA( x-i,       y+i+1,         w-1,      box, 0,60,220,50);//relleno
//			oglSubtractive = false;
		}

//		oglSubtractive = true;
		FillRGBA( x-i,       y+i,         box,    box, 0,60,220,200);  //raya derecha
		FillRGBA( x+w-i,     y+i,         box,    box, 0,60,220,200);  //raya izquierda
//		oglSubtractive = false;

		if(i==0)//rescata los valores maximos
		{
			ends.xs_i=x-i-1;
			ends.ys_i=y+i+1;
			//ends.xs_d=x+w-i;
			//ends.ys_d=y+i+1;
			ends.xs_d=x+w-i;
			ends.ys_d=y+i;
		}

		if(i==len)//rescata los valores maximos
		{
			ends.xe_i=x-i-1;
			ends.ye_i=y+i+1;
			ends.xe_d=x+w-i;
			ends.ye_d=y+i+1;
		}

		i++;
	}

	int y_offs = h-1;
//	oglSubtractive = true;
	FillRGBA( x-len+1,       y+len,         w-1,    box, 0,60,220,200);
//	oglSubtractive = false;

	return ends;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct start_end MenuModule2a(int x, int y, int w, int h, int box, int len, int r, int g, int b, int a)
{
	struct start_end ends;
	
//	oglSubtractive = true;
	FillRGBA( x+1,       y,         w-1,    box, 0,60,220,200);
//	oglSubtractive = false;

	int i=0;
	while(i<=len)
	{
		if(i==0)//rescata los valores maximos
		{
			ends.xs_i=x-i-1;
			ends.ys_i=y+i+1;
			ends.xs_d=x+w-i;
			ends.ys_d=y+i+1;
		}

		if(i==len)//rescata los valores maximos
		{
			ends.xe_i=x-i-1;
			ends.ye_i=y+i+1;
			ends.xe_d=x+w-i;
			ends.ye_d=y+i+1;
		}

		i++;
	}

	int y_offs = h-1;
//	oglSubtractive = true;
	//FillRGBA( x-len+1,       y+len,         w-1,    box, 0,60,220,200);
//	oglSubtractive = false;

	return ends;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct start_end MenuModule2b(int x, int y, int w, int h, int box, int len, int r, int g, int b, int a)
{
	struct start_end ends;
	
//	oglSubtractive = true;
	//FillRGBA( x+1,       y,         w-1,    box, 0,60,220,200);
//	oglSubtractive = false;

	int i=0;
	while(i<=len)
	{
		if(i==0)//rescata los valores maximos
		{
			ends.xs_i=x-i-1;
			ends.ys_i=y+i+1;
			ends.xs_d=x+w-i;
			ends.ys_d=y+i+1;
		}

		if(i==len)//rescata los valores maximos
		{
			ends.xe_i=x-i-1;
			ends.ye_i=y+i+1;
			ends.xe_d=x+w-i;
			ends.ye_d=y+i+1;
		}

		i++;
	}

	int y_offs = h-1;
//	oglSubtractive = true;
	FillRGBA( x-len+1,       y+len,         w-1,    box, 0,60,220,200);
//	oglSubtractive = false;

	return ends;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MenuModule1Fill(int x, int y, int w, int h, int box, int len, int r, int g, int b, int a)
{
	int i=0;
	while(i<=len)
	{
		if(i>=(len-1));
		else
		{
//			oglSubtractive = true;
			FillRGBA( x-i,       y+i+1,         w-1,      box, 0,60,220,50);//relleno
//		    oglSubtractive = false;
		}
		i++;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawDiagonal1(int x, int y, int len, int box, int r, int g, int b, int a)
{
	int c=0;
	while(c<=len){

//		oglSubtractive = true;
		FillRGBA( x-c,     y+c,      box,   box, r,g,b,a);
//		oglSubtractive = false;
		c++;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawDiagonal2(int x, int y, int len, int box, int r, int g, int b, int a)
{
	int c=0;
	while(c<=len){

//		oglSubtractive = true;
		FillRGBA( x+c,     y-c,      box,   box, r,g,b,a);
//		oglSubtractive = false;
		c++;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct start_end TraceDiagonal(int x, int y, int w, int len)
{
	struct start_end ends;
	
	int c=0;
	while(c<=len){

		//FillRGBA( x-c,       y+c,         1,    1, 255,255,255,255);  //raya derecha
		//FillRGBA( x+w-c,     y+c,         1,    1, 255,255,255,255);  //raya izquierda

		if(c==0)//rescata los valores maximos
		{
			ends.xs_i=x-c;
			ends.ys_i=y+c;
			ends.xs_d=x+w-c+1;
			ends.ys_d=y+c-1;
		}

		if(c==len)//rescata los valores maximos
		{
			ends.xe_i=x-c-1;
			ends.ye_i=y+c;
			ends.xe_d=x+w-c;
			ends.ye_d=y+c;
		}

		c++;
	}
	return ends;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct start_end Carcaza1_a(int x, int y, int w, int box, int len1, int offx1, int offy1)
{
	struct start_end ends;

	int c1=0;/* / */
	while(c1<=len1)
	{
		if(c1==0);
		else
		{

//			oglSubtractive = true;
			if(c1!=len1)FillRGBA( x-c1,     y+c1,      66+c1+1,   box, 0,60,220,50);//relleno de entrada seleccionada
//			oglSubtractive = false;
		}

		if(c1==0)
		{
			ends.xs_i=x-c1;
			ends.ys_i=y+c1;
			ends.xs_d=x+w-c1;
			ends.ys_d=y+c1;
		}

		if(c1==len1)
		{
			ends.xe_i=x-c1;
			ends.ye_i=y+c1;
			ends.xe_d=x+w-c1;
			ends.ye_d=y+c1;
		}
		
//		oglSubtractive = true;
		FillRGBA( x-c1,     y+c1,      box,   box, 0,60,220,200);
//		oglSubtractive = false;
		c1++;
	}
	return ends;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Carcaza1_b(int x, int y, int box, int len1, int offx1, int offy1)
{
	int c1=0;/* \ izquierda abajo */
	int aux_c1=0;
	while(c1<=len1)
	{
		//if(c1==0);
		//else
		{
//			oglSubtractive = true;
			FillRGBA( x-c1,     y-c1-aux_c1-2,      66+c1,   box+1, 0,60,220,50);//relleno de entrada seleccionada
//			oglSubtractive = false;
		}
		
		aux_c1=c1+1;
//		oglSubtractive = true;
		FillRGBA( x-c1,     y-c1-aux_c1,      box,   box+1, 0,60,220,200);
//		oglSubtractive = false;
		c1++;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Carcaza2_a(int x, int y, int box, int len1, int offx1, int offy1)
{
	int c1=0;/* \ derecha arriba */
	int aux_c1=0;
	while(c1<=len1)
	{
		//if(c1==0);
		//else
		{
//			oglSubtractive = true;
			if(c1==len1)
			{
				FillRGBA( x+c1+1,     y+c1+aux_c1+2,      -75-c1,   box, 0,60,220,50);//relleno de entrada seleccionada
			}
			else if(c1==(len1-1))
			{
				FillRGBA( x+c1+1,     y+c1+aux_c1+2,      -75-c1,   box+1, 0,60,220,50);//relleno de entrada seleccionada
			}
	        else{
				FillRGBA( x+c1+1,     y+c1+aux_c1+2,      -70-c1,   box+1, 0,60,220,50);//relleno de entrada seleccionada
			}
//			oglSubtractive = false;
		}

		aux_c1=c1+1;
//		oglSubtractive = true;
		FillRGBA( x+c1,     y+c1+aux_c1,      box,   box+1, 0,60,220,200);
//		oglSubtractive = false;
		c1++;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct start_end Carcaza2_b(int x, int y, int w, int box, int len1, int offx1, int offy1)
{
	struct start_end ends;
	int c1=0;/* / */
	while(c1<=len1)
	{
		if(c1==0)
		{
			ends.xs_i=x+c1;
			ends.ys_i=y-c1;
			ends.xs_d=x+w+c1;
			ends.ys_d=y-c1;
		}

		if(c1==len1)
		{
			ends.xe_i=x+c1;
			ends.ye_i=y-c1;
			ends.xe_d=x+w+c1;
			ends.ye_d=y-c1;
		}

		if(c1==0);
		else
		{
//			oglSubtractive = true;
			FillRGBA( x+c1,     y-c1,      -70-c1,   box, 0,60,220,50);//relleno de entrada seleccionada
//			oglSubtractive = false;
		}

//		oglSubtractive = true;
		FillRGBA( x+c1,     y-c1,      box,   box, 0,60,220,200);
//		oglSubtractive = false;
		c1++;
	}
	return ends;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TrapezoidalMenu(int x, int y, int w, int h)
{
	int xx = x+115;
	int offs = 3;

	struct start_end ends1;
	struct start_end ends2;
	struct start_end ends3;
	struct start_end ends4;
	struct start_end ends5;
	struct start_end ends6;
	struct start_end ends7;
	struct start_end ends8;
	struct start_end ends9;

	int box=1;
	int r=0;
	int g=60;
	int b=220;
	int a=200;
	int len1=18;//longitud de un item (fija)
	int len2=0;//longitud total de todos los items (variable)
	int lenI=0;//longitud de la linea izquierda aditiva (variable)
	int lenD=0;//longitud de la linea derecha aditiva (variable)

	int len3=0;//longitud desde el primer elemento hasta el seleccionado
	int len4=0;//longitud desde el ultimo elemento hasta el seleccionado

	///////////////////////////////////////////////////////////////////////////////////
	//TITLE
	///////////////////////////////////////////////////////////////////////////////////

	//recuadro del titulo (fijo)
	ends1 = MenuModule1(x,y,w+30,h,box,len1,r,g,b,a);
	DrawConString( ends1.xs_i+10,ends1.ys_i+1, 255,255,255, MENUTILTE_CH );

	///////////////////////////////////////////////////////////////////////////////////

	int ENTRY_oFF = 32;

	//punto de salida de la linea lateral izquierda
	int entry_x=ends1.xe_i;
	int entry_y=ends1.ye_i;

	//tracediagonal es la que da la separacion entre el titulo y los items !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	ends2 = TraceDiagonal(entry_x,entry_y,w,len1); //-------------------------> a partir de una linea izquierda imaginaria ke sale del cuadro del menu

	//posicion fija de la primera entrada (entrada simple)[p1]
	int x_firstentry_s1 =ends2.xe_i+ENTRY_oFF;//+24; //--------------------------> desde ak se desplazan las entradas del menu !!
	int y_firstentry_s1 =ends2.ye_i;

	//posicion fija de la primera entrada (entrada seleccionada)[p2]
	int x_firstentry_s2 =ends2.xe_i+20; //------------------> el punto p2 de la carcaza izquierda de la seleccionada
	int y_firstentry_s2 =ends2.ye_i;

	int const_len = 18;//altura fija de cada entrada

	////////////////////////////////////

	len2 = (len1*(menuItems+1)+len1);
	len3 = (len1*menuSelect)+len1+(len1/2);
	int down_items = (menuItems-menuSelect)-1;
	len4 = len1*down_items;
	len4 = len4+(len1/2)+len1;
	DrawDiagonal1(entry_x,entry_y,len3,box,0,60,220,200);

	ends8 = TraceDiagonal(entry_x+ENTRY_oFF,entry_y,w,len2);

	///////////////////////////////////////////////////////////////////////////////////
	//DATE
	///////////////////////////////////////////////////////////////////////////////////

	const char* DATE_CH = "Date: ";
	ends9 = MenuModule1(ends8.xe_i,ends8.ye_i,w+30,h,box,len1,r,g,b,a);
	SYSTEMTIME st;
    GetLocalTime(&st);
	DrawConString( ends9.xs_i+4,ends9.ys_i+1, 255,255,255, "%s %02d:%02d:%02d",DATE_CH,st.wDay,st.wMonth,st.wYear );

	///////////////////////////////////////////////////////////////////////////////////

	DrawDiagonal2(ends9.xs_d,ends9.ys_d,len4,box, 0,60,220, 200);

	//////////////////////////////////////////////////////////////////////////////////////////////////////

	for(int i=0;i<menuItems;i++)
	{
		if( i == menuSelect )
		{//entrada seleccionada
			ends5 = TraceDiagonal(x_firstentry_s1-(const_len*i), y_firstentry_s1+(const_len*i), w,len1);

			int ap1_x=ends5.xs_i;
			int ap1_y=ends5.ys_i;
			int ap2_x=ends5.xs_i-15;
			int ap2_y=ends5.ys_i;
			int ap3_x=ends5.xe_i;
			int ap3_y=ends5.ye_i;

			ends6 = Carcaza1_a( ap2_x, ap2_y, w,box, 8, 0, 0);/* / */
		    Carcaza1_b( ap3_x, ap3_y, box, 4, 0, 0);/* \ izquierda abajo */

			FillRGBA(ends6.xe_i, ends6.ye_i+1,      -(ENTRY_oFF/2),   1, 0,60,220,200);//eje izquierdo

			//lineas de tapado
			FillRGBA(ap2_x, ap2_y,      15+1,   box, 0,60,220, 200);//p2TOp1 +offs
			FillRGBA(ap3_x, ap3_y,      2,   box, 0,60,220, 200);//p3 +offs

			int bp1_x=ends5.xe_d;
			int bp1_y=ends5.ye_d;
			int bp2_x=ends5.xe_d+15;
			int bp2_y=ends5.ye_d;
			int bp3_x=ends5.xs_d;
			int bp3_y=ends5.ys_d;
			Carcaza2_a( bp3_x, bp3_y, box, 4, 0, 0);/* \ derecha arriba */
			ends7 = Carcaza2_b( bp2_x, bp2_y, w,box, 8, 0, 0);/* / */

			FillRGBA(ends7.xe_i, ends7.ye_i-1,      (ENTRY_oFF/2),   1, 0,60,220,200);//eje derecho

			//lineas de tapado
			FillRGBA(bp1_x-1, bp1_y,      15+2,   box, 0,60,220, 200);//p1 -offs TOp2
			FillRGBA(bp3_x, bp3_y+1,      -2,   box, 0,60,220, 200);// p3 -offs

			///////////////////////////////

			if(i==0)
			   ends4 = MenuModule2a( x_firstentry_s1-(const_len*i), y_firstentry_s1+(const_len*i),                 w,h,box,len1,r,g,b,a);
			else if(i==(menuItems-1))
				ends4 = MenuModule2b( x_firstentry_s1-(const_len*i), y_firstentry_s1+(const_len*i),                 w,h,box,len1,r,g,b,a);

			DrawConString( x_firstentry_s1-(const_len*i), y_firstentry_s1+(const_len*i), 0,255,0, menuEntry[i].title );
		}

		else if( i!=menuItems )
		{//entradas simples

			ends3 = MenuModule1( x_firstentry_s1-(const_len*i), y_firstentry_s1+(const_len*i),                 w,h,box,len1,r,g,b,a);
			DrawConString( x_firstentry_s1-(const_len*i), y_firstentry_s1+(const_len*i), 255,255,255 ,menuEntry[i].title );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


