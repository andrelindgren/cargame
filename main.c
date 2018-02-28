#include <pic32mx.h>
#include <stdint.h>

#define DISPLAY_VDD PORTFbits.RF6  // bit 6 in portF
#define DISPLAY_VBATT PORTFbits.RF5 // bit 5 in portF
#define DISPLAY_COMMAND_DATA PORTFbits.RF4 // bit 4 in portF
#define DISPLAY_RESET PORTGbits.RG9 // bit 9 in portG


#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x40  //0100 0000
#define DISPLAY_VBATT_PORT PORTF //
#define DISPLAY_VBATT_MASK 0x20 // 0010 0000
#define DISPLAY_COMMAND_DATA_PORT PORTF
#define DISPLAY_COMMAND_DATA_MASK 0x10  // 0001 0000
#define DISPLAY_RESET_PORT PORTG   
#define DISPLAY_RESET_MASK 0x200  // 0010 0000 0000


char textbuffer[4][16];

static const uint8_t const font[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 94, 0, 0, 0, 0,
	0, 0, 4, 3, 4, 3, 0, 0,
	0, 36, 126, 36, 36, 126, 36, 0,
	0, 36, 74, 255, 82, 36, 0, 0,
	0, 70, 38, 16, 8, 100, 98, 0,
	0, 52, 74, 74, 52, 32, 80, 0,
	0, 0, 0, 4, 3, 0, 0, 0,
	0, 0, 0, 126, 129, 0, 0, 0,
	0, 0, 0, 129, 126, 0, 0, 0,
	0, 42, 28, 62, 28, 42, 0, 0,
	0, 8, 8, 62, 8, 8, 0, 0,
	0, 0, 0, 128, 96, 0, 0, 0,
	0, 8, 8, 8, 8, 8, 0, 0,
	0, 0, 0, 0, 96, 0, 0, 0,
	0, 64, 32, 16, 8, 4, 2, 0,
	0, 62, 65, 73, 65, 62, 0, 0,
	0, 0, 66, 127, 64, 0, 0, 0,
	0, 0, 98, 81, 73, 70, 0, 0,
	0, 0, 34, 73, 73, 54, 0, 0,
	0, 0, 14, 8, 127, 8, 0, 0,
	0, 0, 35, 69, 69, 57, 0, 0,
	0, 0, 62, 73, 73, 50, 0, 0,
	0, 0, 1, 97, 25, 7, 0, 0,
	0, 0, 54, 73, 73, 54, 0, 0,
	0, 0, 6, 9, 9, 126, 0, 0,
	0, 0, 0, 102, 0, 0, 0, 0,
	0, 0, 128, 102, 0, 0, 0, 0,
	0, 0, 8, 20, 34, 65, 0, 0,
	0, 0, 20, 20, 20, 20, 0, 0,
	0, 0, 65, 34, 20, 8, 0, 0,
	0, 2, 1, 81, 9, 6, 0, 0,
	0, 28, 34, 89, 89, 82, 12, 0,
	0, 0, 126, 9, 9, 126, 0, 0,
	0, 0, 127, 73, 73, 54, 0, 0,
	0, 0, 62, 65, 65, 34, 0, 0,
	0, 0, 127, 65, 65, 62, 0, 0,
	0, 0, 127, 73, 73, 65, 0, 0,
	0, 0, 127, 9, 9, 1, 0, 0,
	0, 0, 62, 65, 81, 50, 0, 0,
	0, 0, 127, 8, 8, 127, 0, 0,
	0, 0, 65, 127, 65, 0, 0, 0,
	0, 0, 32, 64, 64, 63, 0, 0,
	0, 0, 127, 8, 20, 99, 0, 0,
	0, 0, 127, 64, 64, 64, 0, 0,
	0, 127, 2, 4, 2, 127, 0, 0,
	0, 127, 6, 8, 48, 127, 0, 0,
	0, 0, 62, 65, 65, 62, 0, 0,
	0, 0, 127, 9, 9, 6, 0, 0,
	0, 0, 62, 65, 97, 126, 64, 0,
	0, 0, 127, 9, 9, 118, 0, 0,
	0, 0, 38, 73, 73, 50, 0, 0,
	0, 1, 1, 127, 1, 1, 0, 0,
	0, 0, 63, 64, 64, 63, 0, 0,
	0, 31, 32, 64, 32, 31, 0, 0,
	0, 63, 64, 48, 64, 63, 0, 0,
	0, 0, 119, 8, 8, 119, 0, 0,
	0, 3, 4, 120, 4, 3, 0, 0,
	0, 0, 113, 73, 73, 71, 0, 0,
	0, 0, 127, 65, 65, 0, 0, 0,
	0, 2, 4, 8, 16, 32, 64, 0,
	0, 0, 0, 65, 65, 127, 0, 0,
	0, 4, 2, 1, 2, 4, 0, 0,
	0, 64, 64, 64, 64, 64, 64, 0,
	0, 0, 1, 2, 4, 0, 0, 0,
	0, 0, 48, 72, 40, 120, 0, 0,
	0, 0, 127, 72, 72, 48, 0, 0,
	0, 0, 48, 72, 72, 0, 0, 0,
	0, 0, 48, 72, 72, 127, 0, 0,
	0, 0, 48, 88, 88, 16, 0, 0,
	0, 0, 126, 9, 1, 2, 0, 0,
	0, 0, 80, 152, 152, 112, 0, 0,
	0, 0, 127, 8, 8, 112, 0, 0,
	0, 0, 0, 122, 0, 0, 0, 0,
	0, 0, 64, 128, 128, 122, 0, 0,
	0, 0, 127, 16, 40, 72, 0, 0,
	0, 0, 0, 127, 0, 0, 0, 0,
	0, 120, 8, 16, 8, 112, 0, 0,
	0, 0, 120, 8, 8, 112, 0, 0,
	0, 0, 48, 72, 72, 48, 0, 0,
	0, 0, 248, 40, 40, 16, 0, 0,
	0, 0, 16, 40, 40, 248, 0, 0,
	0, 0, 112, 8, 8, 16, 0, 0,
	0, 0, 72, 84, 84, 36, 0, 0,
	0, 0, 8, 60, 72, 32, 0, 0,
	0, 0, 56, 64, 32, 120, 0, 0,
	0, 0, 56, 64, 56, 0, 0, 0,
	0, 56, 64, 32, 64, 56, 0, 0,
	0, 0, 72, 48, 48, 72, 0, 0,
	0, 0, 24, 160, 160, 120, 0, 0,
	0, 0, 100, 84, 84, 76, 0, 0,
	0, 0, 8, 28, 34, 65, 0, 0,
	0, 0, 0, 126, 0, 0, 0, 0,
	0, 0, 65, 34, 28, 8, 0, 0,
	0, 0, 4, 2, 4, 2, 0, 0,
	0, 120, 68, 66, 68, 120, 0, 0,
};

const uint8_t const icon[] = {
	255, 255, 255, 255, 255, 255, 127, 187,
	68, 95, 170, 93, 163, 215, 175, 95,
	175, 95, 175, 95, 223, 111, 175, 247,
	59, 237, 242, 254, 171, 254, 1, 255,
	255, 255, 15, 211, 109, 58, 253, 8,
	178, 77, 58, 199, 122, 197, 242, 173,
	242, 237, 186, 215, 40, 215, 41, 214,
	35, 175, 91, 212, 63, 234, 149, 111,
	171, 84, 253, 252, 254, 253, 126, 184,
	195, 52, 201, 22, 225, 27, 196, 19,
	165, 74, 36, 146, 72, 162, 85, 8,
	226, 25, 166, 80, 167, 216, 167, 88,
	106, 149, 161, 95, 135, 91, 175, 87,
	142, 123, 134, 127, 134, 121, 134, 121,
	132, 59, 192, 27, 164, 74, 177, 70,
	184, 69, 186, 69, 254, 80, 175, 217,
};

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

void display_init() {
	DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
	delay(10);
	DISPLAY_VDD_PORT &= ~DISPLAY_VDD_MASK;
	delay(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_RESET_PORT &= ~DISPLAY_RESET_MASK;
	delay(10);
	DISPLAY_RESET_PORT |= DISPLAY_RESET_MASK;
	delay(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_VBATT_PORT &= ~DISPLAY_VBATT_MASK;
	delay(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}

void display_string(int line, char *s) {
	int i;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;
	
	for(i = 0; i < 16; i++)	
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';
}

void display_image(int x, const uint8_t *data) {
	int i, j;
	
	for(i = 0; i < 4; i++) {
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(x & 0xF);
		spi_send_recv(0x10 | ((x >> 4) & 0xF));
		
		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
		
		for(j = 0; j < 32; j++)
			spi_send_recv(~data[i*32 + j]);
	}
}

void display_update() {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
		
		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;
			
			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

int getbtns (void){
		int btns = PORTD & 0x0e0;
		btns = btns >> 5;
	return btns;
	}
	
int timerEnd(void) {
	if(IFS(0) & 0x0100) {
		IFS(0) = 0;
			return(1);
	}
	else {
		return(0);
	}
}
	
	
	void difficultyMenu(void);
	void mainMenu(void);
	void GameMenu(void);
	void PauseMenu(void);
	void LostMenu(void);
	void ruleMenu(void);
	
void difficultyMenu(void) {
	delay(1000000);
	unsigned int volt;
	int ental = 0;
	while (1) {
		
		
		
		AD1CON1 |= (0x1 << 1);
		while(!(AD1CON1 & (0x1 << 1)));
		while(!(AD1CON1 & 0x1));
	
		
	
		/* Get the analog value */
		volt = ADC1BUF0;
		
		ental = volt / 32 / 3.2;
	
		display_string(0, "choose difficulty");
		display_string(1, "1. choose");
		display_string(2, "2. quit");
		
		if (ental == 0) {
			display_string(3, "0");
		}
		else if (ental == 1) {
			display_string(3, "1");
		}
		else if (ental == 2) {
			display_string(3, "2");
		}
		else if (ental == 3) {
			display_string(3, "3");
		}
		else if (ental == 4) {
			display_string(3, "4");
		}
		else if (ental == 5) {
			display_string(3, "5");
		}
		else if (ental == 6) {
			display_string(3, "6");
		}
		else if (ental == 7) {
			display_string(3, "7");
		}
		else if (ental == 8) {
			display_string(3, "8");
		}
		else if (ental == 9) {
			display_string(3, "9");
		}
		
		
		display_update();
		if (((getbtns() & 0x04) != 0)) {
			 GameMenu();
		}
		if(((getbtns() & 0x02) != 0)) {
			mainMenu();
		}
	}		
}
	
	
void mainMenu(void) {
	delay(1000000);
	while (1) {
			
		display_string(0, "Car Game");
		display_string(1, "1. Start");
		display_string(2, "2. Rules");
		display_string(3, "");
		display_update();
		if (((getbtns() & 0x04) != 0)) {
			difficultyMenu();
		}
		if(((getbtns() & 0x02) != 0)) {
			ruleMenu();
		}
	}		
}
void GameMenu(void){
	delay(1000000);
	while (1) {
		display_string(0, "game running");
		display_string(1, "");
		display_string(2, "");
		display_string(3, "");
		display_update();	
		if(((getbtns() & 0x04) != 0)) {
			LostMenu();
		}	
		if(((getbtns() & 0x02) != 0)) {
			PauseMenu();
		}			
	}
}	
void PauseMenu(void){
	delay(1000000);
	while(1) {
		display_string(0, "Pause");
		display_string(1, "1. Continue");
		display_string(2, "2. Quit");
		display_string(3, "");
		display_update();
		if(((getbtns() & 0x04) != 0)) {
			GameMenu();
		}
		if(((getbtns() & 0x02) != 0)) {
			mainMenu();
		}
	}
}
void LostMenu(void){
	delay(1000000);
	while(1) {
		display_string(0, "You Lose");
		display_string(1, "1. Try again");
		display_string(2, "2. Quit");
		display_string(3, "");
		display_update();
		if(((getbtns() & 0x04) != 0)) {
			difficultyMenu();
		}
		if(((getbtns() & 0x02) != 0)) {
			mainMenu();
		}	
	}
}	
void ruleMenu(void){
	delay(1000000);
	while(1) {
		display_string(0, "Controls");
		display_string(1, "Steer with wheel");
		display_string(2, "Don't get hit");
		display_string(3, "Good Luck!");
		display_update();
		if(((getbtns() & 0x04) != 0)) {
			mainMenu();
		}
	}
}	
int main(void) {
	/* Set up peripheral bus clock */
	OSCCON &= ~0x180000;
	OSCCON |= 0x080000;
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	
	/* Clear SPIROV*/
	SPI2STATCLR &= ~0x40;
	/* Set CKP = 1, MSTEN = 1; */
        SPI2CON |= 0x60;
	
	/* Turn on SPI */
	SPI2CONSET = 0x8000;
	
	

	T2CON = 0x070;
	TMR2 = 0x0;
	PR2 = (80000000 / 256) / 10;
	IFS(0) = 0;
	T2CONSET = 0x8000;
	
	int timeoutcount = 0;

	
	
	
	
	
	
	
	
	
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
	
	/* Set up output pins */
	ODCE = 0x0;
	TRISECLR = 0xFF;
	
	/* Turn on ADC */
	AD1CON1 |= (0x1 << 15);

	
	display_init();
    int btnspressed = 0;	
	int meny = 1;
	
	mainMenu();
	
	while (1){
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		if(timerEnd()){
			timeoutcount++;
		}
		
		if (((getbtns() & 0x04) != 0)) {
		
		btnspressed = 1;
		//timeoutcount = 0;
		
		}
		if (((getbtns() & 0x02) != 0)) {
		
		btnspressed = 2;
		//timeoutcount = 0;
		}
		if (((getbtns() & 0x01) != 0)) {
		btnspressed = 3;
		//timeoutcount = 0;
		}
		
		
		
		if((btnspressed == 1) && (meny == 2)){
			meny = 4;
			btnspressed = 0;
			display_string(0, "game running");
			display_string(1, "");
			display_string(2, "");
			display_string(3, "");
			display_update();
	
		}
		
		if((btnspressed == 1) && (meny == 3)){
			meny = 4;
			btnspressed = 0;
			display_string(0, "game running");
			display_string(1, "");
			display_string(2, "");
			display_string(3, "");
			display_update();	
	
		}
		
	
		
		
		if((btnspressed == 1) && (meny == 5)){
			display_string(0, "Car Game");
			display_string(1, "3. start");
			display_string(2, "");
			display_string(3, "");
			display_update(); 
			btnspressed = 0;
			meny = 1;	
		}
		
		if((btnspressed == 1) && (meny == 6)){
			display_string(0, "Car Game");
			display_string(1, "3. start");
			display_string(2, "");
			display_string(3, "");
			display_string(3, "");
			display_update(); 
			btnspressed = 0;
			meny = 1; 
		}
		
		if((btnspressed == 2) && (meny == 2)){
			display_string(0, "Car Game");
			display_string(1, "3. start");
			display_string(2, "");
			display_string(3, "");
			display_update();
			meny = 1;
			btnspressed = 0;
		}
		if((btnspressed == 2) && (meny == 3)){
			display_string(0, "Car Game");
			display_string(1, "3. start");
			display_string(2, "");
			display_string(3, "");
			display_update();
			meny = 1;
			btnspressed = 0;
		
		}
		
		
		if((btnspressed == 3) && (meny == 4)) {
			display_string(0, "pause");
			display_string(1, "1. continue");
			display_string(2, "2. quit");
			display_string(3, "");
			display_update();
			meny = 3;
			btnspressed = 3;
		}
		
		display_update();
		
	}
}	

	