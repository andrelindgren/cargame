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
void winMenu(void){
	delay(1000000);
	while(1) {
		display_string(0, "Congratulations");
		display_string(1, "You win!");
		display_string(2, "1. Play again");
		display_string(3, "");
		if(((getbtns() & 0x04) != 0))
			choselevel();
	}
}
void choselevel(void){
	delay(1000000);
	while(1) {
		display_string(0, "City");
		display_string(1, "Forest");
		display_string(2, "Ocean");
		display_string(3, "");
		if(((getbtns() & 0x04) != 0)){
		}
		
		if(((getbtns() & 0x02) != 0)){
		}
		
		if(((getbtns() & 0x01) != 0)){
		}
	}
}		
	
	
	
	
	
	