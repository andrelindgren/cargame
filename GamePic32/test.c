/*
 * test.c
 *
 * by André Lindgren
 * Created 2018-02-20
 *
 * Used to test prototypes of code for cargame
 *
 * Remember to clear matrices before inserting the template 
 * for the first time!
 */


#include <pic32mx.h>
#include <stdint.h>
#include <stdlib.h>
#include "testHeader.h"

void* stdout;


//INTERNET CODE
#define DISPLAY_VDD PORTFbits.RF6
#define DISPLAY_VBATT PORTFbits.RF5
#define DISPLAY_COMMAND_DATA PORTFbits.RF4
#define DISPLAY_RESET PORTGbits.RG9

//#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x40
//#define DISPLAY_VBATT_PORT PORTF
#define DISPLAY_VBATT_MASK 0x20
//#define DISPLAY_COMMAND_DATA_PORT PORTF
#define DISPLAY_COMMAND_DATA_MASK 0x10 // 0001 0000
#define DISPLAY_RESET_PORT PORTG
#define DISPLAY_RESET_MASK 0x200

void delay(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));  // while bit spitbe = 0 , 
	SPI2BUF = data;
	while(!(SPI2STAT & 0x01)); // while bit SPIRBf = 0 , 
	return SPI2BUF;
}

// COPIED
void display_init() {
	PORTF &= ~DISPLAY_COMMAND_DATA_MASK; // NOT 
	delay(10);
	PORTF &= ~DISPLAY_VDD_MASK; // NOT
	delay(1000000);
	
	spi_send_recv(0xAE); // 1010 1110
	DISPLAY_RESET_PORT &= ~DISPLAY_RESET_MASK; // NOT
	delay(10);
	DISPLAY_RESET_PORT |= DISPLAY_RESET_MASK;
	delay(10);
	
	spi_send_recv(0x8D); // 1000 1101
	spi_send_recv(0x14); // 0001 0100
	
	spi_send_recv(0xD9); // 1101 1001
	spi_send_recv(0xF1); // 1111 0001
	
	PORTF &= ~DISPLAY_VBATT_MASK;
	delay(10000000);
	
	spi_send_recv(0xA1); // 1010 0001
	spi_send_recv(0xC8); // 1100 1000
	
	spi_send_recv(0xDA); // 1101 1010
	spi_send_recv(0x20); // 0010 0000
	
	spi_send_recv(0xAF); // 1010 1111
}

// COPIED
void display_image(int x, unsigned char pixelMap[128][32]) {
	int i, j, k;
	unsigned char pixelBuffer = 0;
	for(i = 0; i < 4; i++) {
		PORTF &= ~DISPLAY_COMMAND_DATA_MASK; // NOT!!!!
		spi_send_recv(0x22); // 0010 0010
		spi_send_recv(i);
		
		spi_send_recv(x & 0xF); // x & 1111 // Bits 0:3 of x
		spi_send_recv(0x10 | ((x >> 4) & 0xF)); // 0001 0000 | (x & 1111) // Bits 4:7 of x
		
		PORTF |= DISPLAY_COMMAND_DATA_MASK;
		

		// I CREATED THIS
		for(j = 0; j < 128; j++) {
			pixelBuffer = 0;
			for (k = 0; k < 8; k++) {
				if (pixelMap[j][k+((3-i)*8)] == 1) {
					pixelBuffer = pixelBuffer | (pixelMap[j][k+((3-i)*8)] << (7-k));
				}
			}
			spi_send_recv(pixelBuffer);
		}
	}
}








// For polling timer
int timerEnd(void) {
	if(IFS(0) & 0x0100) {
		IFS(0) = 0;
		return(1);
	}
	else {
		return(0);
	}
}





int main (void) {


	// COPIED
	/* PORTB.2 is analog pin with potentiometer*/
	AD1PCFG = ~(1 << 2);
	TRISBSET = (1 << 2);
	/* Use pin 2 for positive */
	AD1CHS = (0x2 << 16);
	
	/* Data format in uint32, 0 - 1024
	Manual sampling, auto conversion when sampling is done
	FORM = 0x4; SSRC = 0x7; CLRASAM = 0x0; ASAM = 0x0; */
	AD1CON1 = (0x4 << 8) | (0x7 << 5);
	
	AD1CON2 = 0x0;
	AD1CON3 |= (0x1 << 15);
	
	//Dont need
	/* Set up output pins */
	ODCE = 0x0;
	TRISECLR = 0xFF;
	
	/* Turn on ADC */
	AD1CON1 |= (0x1 << 15);		






	// COPIED
	/* Set up peripheral bus clock */
	OSCCON &= ~0x180000;
	OSCCON |= 0x080000;
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;

	/* ODC
	 * For outputs pin only. 
	 * 0: for normal digital output 
	 * 1: For higher then VDD voltage (For our use all should be 0)
	 */
	ODCE = 0x0;     
	TRISECLR = 0xFF; // TRISE xxxx 0000 0000 (outputs)
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;   // PORTF 1111 1111 1111 1111
	PORTG = (1 << 9); // PORTG 0001 0000 0000
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;  // TRISF xxxx x000 xxxx (outputs)
	TRISGCLR = 0x200; // TRISG xx0x xxxx xxxx (outputs)
	
	/* Set up input pins */
	TRISDSET = (1 << 8); // TRISD xxxx 1xxx xxxx (inputs)
	TRISFSET = (1 << 1); // TRISF xxxx xxxx xxx1 (inputs)
	
	/* Set up SPI as master */
	SPI2CON = 0; // 0000 0000 0000
	SPI2BRG = 4;
	
	/* Clear SPIROV*/
	SPI2STATCLR &= ~0x40;
	/* Set CKP = 1, MSTEN = 1; */
    SPI2CON |= 0x60; // SPI2CON = 0110 0000
	
	/* Turn on SPI */
	SPI2CONSET = 0x8000; // SPI2CON = 1000 0000 0110 0000







	//Timer
	T2CON = 0x070;
	TMR2 = 0x0;
	PR2 = (80000000 / 256) / 100; // 100 times per second
	IFS(0) = 0;
	T2CONSET = 0x8000;

	//
	// START OF GAME
	//

	// COPIED
	display_init();

	mainMenu();

	return 0;
}


