/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog
   This file was modified 2018-02-13 by Alexander Nordström

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";
int timerEnd(void) {
	if(IFS(0) & 0x0100) {
		IFS(0) = 0;
		return(1);
	}
	else {
		return(0);
	}
}
/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

int timeoutcount = 0;
/* Lab-specific initialization goes here */
void labinit( void )
{
	volatile int* trise = (volatile int*) 0xbf886100;
	TRISD = TRISD | 0xfe0;
	*trise = *trise & 0xffffff00;
	T2CON = 0x070;
	TMR2 = 0x0;
	PR2 = (80000000 / 256) / 100;
	IFS(0) = 0;
	T2CONSET = 0x8000;

  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
	if ((getbtns() & 0x04) != 0) {
	 mytime = (mytime & 0xf0fff) | (getsw() << 12);
	}

	if ((getbtns() & 0x02) != 0) {
	 mytime = (mytime & 0xff0ff) | (getsw() << 8);
	}

	if ((getbtns() & 0x01) != 0) {
	 mytime = (mytime & 0xfff0f) | (getsw() << 4);
	}

	if(timerEnd()){
		timeoutcount++;
	}


	if(timeoutcount == 10){
		volatile int* LED = (volatile int*) 0xbf886110;
		*LED = *LED + 1;
		//delay( 1000 );
		time2string( textstring, mytime );
		display_string( 3, textstring );
		display_update();
 		tick( &mytime );
 		display_image(96, icon);
 		timeoutcount = 0;
	}

}
