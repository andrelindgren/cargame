#include <pic32mx.h>
#include <stdint.h>
#include "testHeader.h"

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

void display_update() {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		PORTF &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
		PORTF |= DISPLAY_COMMAND_DATA_MASK;
		
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

void mainMenu(void) {
	delay(1000000);
	while (1) {
			
		display_string(0, "Car Game");
		display_string(1, "1. Start");
		display_string(2, "2. Rules");
		display_string(3, "");
		display_update();
		if (((getbtns() & 0x04) != 0)) {
			chooselevel();
		}
		if(((getbtns() & 0x02) != 0)) {
			ruleMenu();
		}
	}		
}

void chooselevel(void){
	delay(1000000);
	while(1) {
		display_string(0, "City");
		display_string(1, "Forest");
		display_string(2, "Ocean");
		display_string(3, "");
		display_update();
		if(((getbtns() & 0x04) != 0)){
			difficultyMenu(0);
		}
		
		if(((getbtns() & 0x02) != 0)){
			difficultyMenu(1);
		}
		
		if(((getbtns() & 0x01) != 0)){
			difficultyMenu(2);
		}
	}
}

	
void difficultyMenu(int level) {
	delay(1000000);
	unsigned int volt;
	int ental = 0;
	int gameLost = 0;
	while (1) {
		
		AD1CON1 |= (0x1 << 1);
		while(!(AD1CON1 & (0x1 << 1)));
		while(!(AD1CON1 & 0x1));
	
		/* Get the analog value */
		volt = ADC1BUF0;
		
		ental = volt / 32 / 3.2;
	
		display_string(0, "Choose Difficulty");
		display_string(1, "1. Start Game");
		display_string(2, "2. Quit");
		
		if (ental == 0) {
			display_string(3, "Difficulty = 0");
		}
		else if (ental == 1) {
			display_string(3, "Difficulty = 1");
		}
		else if (ental == 2) {
			display_string(3, "Difficulty = 2");
		}
		else if (ental == 3) {
			display_string(3, "Difficulty = 3");
		}
		else if (ental == 4) {
			display_string(3, "Difficulty = 4");
		}
		else if (ental == 5) {
			display_string(3, "Difficulty = 5");
		}
		else if (ental == 6) {
			display_string(3, "Difficulty = 6");
		}
		else if (ental == 7) {
			display_string(3, "Difficulty = 7");
		}
		else if (ental == 8) {
			display_string(3, "Difficulty = 8");
		}
		else if (ental == 9) {
			display_string(3, "Difficulty = 9");
		}
		
		display_update();
		if (((getbtns() & 0x04) != 0)) {
			gameLost = game(level, ental);
			if (gameLost == 1) {
				LostMenu();
			} 
			else if ( gameLost == 2) {
				winMenu();
			}
		}
		if(((getbtns() & 0x02) != 0)) {
			mainMenu();
		}
	}		
}

int PauseMenu(void){
	delay(1000000);
	while(1) {
		display_string(0, "Pause");
		display_string(1, "1. Continue");
		display_string(2, "2. Quit");
		display_string(3, "");
		display_update();
		if(((getbtns() & 0x04) != 0)) {
			return 0;
		}
		if(((getbtns() & 0x02) != 0)) {
			return 1;
		}
	}
}

void winMenu(void){
	delay(1000000);
	while(1) {
		display_string(0, "Congratulations");
		display_string(1, "You win!");
		display_string(2, "1. Play again");
		display_string(3, "");
		display_update();
		if(((getbtns() & 0x04) != 0))
			chooselevel();
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
			difficultyMenu(1);
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


