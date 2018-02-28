/* game.c
 *
 *
 */

#include <pic32mx.h>
#include <stdint.h>
#include <stdlib.h>
#include "testHeader.h"

int game (int difficulty) {

	//POT
	unsigned int volt;

	// Movespeed
	double speedY = 100;//100 + (difficulty * 50);

	// Variables for for-loops
	int i;
	int j;
	int n;

	//How long each frame is (100 frames/s -> frame = 10000 microseconds)
	long timeElapsed = 10000; 

	// PixelMap
	unsigned char pM[pMM][pMN];
	clearMatrix(pMM, pMN, pM);

	// Road
	double roadX = 14;
	double roadY = -5;
	unsigned char road[roadTemplateM][roadTemplateN];
	clearMatrix(roadTemplateM, roadTemplateN, road);
	insertSprite(roadTemplateM, roadTemplateN, road, roadTemplateM, roadTemplateN, roadTemplate, 0, 0);

	// Car
	double carSpeedY = 0;
	double carX = 4;
	double carY = 120;
	unsigned char car[carTemplateM][carTemplateN];
	clearMatrix(carTemplateM, carTemplateN, car);
	insertSprite(carTemplateM, carTemplateN, car, carTemplateM, carTemplateN, carTemplate, 0, 0);

	// Array of Obstacles
	int nObs = 5;
	double obsX[nObs];
	double obsY[nObs];
	unsigned char obs[nObs][16][16];
	
	// Fill list of templates
	unsigned char templateList[5][16][16] = {
		{template1},
		{template2}, 
		{truck},
		{dog}, 
		{house},
	}; 

	//Assign all obstacles a template
	for (i = 0; i < nObs; i++) {
		obsX[i] = 20;
		obsY[i] = 30;
		clearMatrix(16, 16, obs[i]);
		insertSprite(16, 16, obs[i], 16, 16, templateList[i], 0, 0);
	} 

	// Random number
	int r = 0; 


	delay(1000000);
	while (1) {
		
		// 
		if(((getbtns() & 0x01) != 0)) {
			PauseMenu();
		}


		if(((getbtns() & 0x04) != 0)) {
			carSpeedY = -30;
		}

		if(((getbtns() & 0x04) == 0)) {
			carSpeedY = 0;
		}

		if(((getbtns() & 0x02) != 0)) {
			carSpeedY = 30;
		}

		if(((getbtns() & 0x02) == 0)) {
			carSpeedY = 0;
		}
	

		AD1CON1 |= (0x1 << 1);
		while(!(AD1CON1 & (0x1 << 1)));
		while(!(AD1CON1 & 0x1));
			
		/* Get the analog value */
		volt = ADC1BUF0;
		carX = volt / 32;
		if (carX >= 32-carTemplateN) {
			carX = 32-carTemplateN;
		} 

		// Frameupdate
		if (timerEnd()) {

			if (speedY < 400 + (difficulty*20)) {
				speedY = speedY + 0.5;
			}

			clearMatrix(pMM, pMN, pM);

			// Insert obstacles
			for (i = 0; i < nObs; i++) {
				insertSprite(pMM, pMN, pM, 16, 16, obs[i], (int) obsX[i], (int) obsY[i]);
			}
			// Hit detection
			if (carHit(pMM, pMN, pM, carTemplateM, carTemplateN, car, carX, carY)) {
			//	return 1;
			} 
			// Insert road and car
			insertSprite(pMM, pMN, pM, roadTemplateM, roadTemplateN, road, (int) roadX, (int) roadY);
			insertSprite(pMM, pMN, pM, carTemplateM, carTemplateN, car, carX, carY);
		
			display_image(0, pM);
			
			for (i = 0; i < nObs; i++) {
				moveSprite(0, speedY, timeElapsed, &obsX[i], &obsY[i]);
				if (obsY[i] > pMM) {
					srand(TMR2);
					r = rand() % 200;
					obsY[i] = obsY[i] - (pMM+16) - r;

					srand(TMR2+1);
					r = rand() % 32;
					obsX[i] = r;

					srand(TMR2+2);
					r = rand() % 5;
		
					if (r == 0) {
						clearMatrix(16, 16, obs[i]);
						insertSprite(16, 16, obs[i], 16, 16, templateList[r], 0, 0);
					}
					else if (r == 1) {
						clearMatrix(16, 16, obs[i]);
						insertSprite(16, 16, obs[i], 16, 16, template1, 0, 0);
					}
					else if (r == 2) {
						clearMatrix(16, 16, obs[i]);
						insertSprite(16, 16, obs[i], 16, 16, dog, 0, 0);
					}
					else if (r == 3) {
						clearMatrix(16, 16, obs[i]);
						insertSprite(16, 16, obs[i], 16, 16, house, 0, 0);
					}
					else {
						clearMatrix(16, 16, obs[i]);
						insertSprite(16, 16, obs[i], 16, 16, template2, 0, 0);
					}
				}
			}

			moveSprite(0, speedY, timeElapsed, &roadX, &roadY);
			if (roadY > 5) {
				roadY = roadY - 10;
			}

			if (carY > 0 && carY <= 128) {
				moveSprite(0, carSpeedY, timeElapsed, &carX, &carY);
			}
			
				
			/*
			if (carX > 0 || carX < 20) {
				if ((volt/32) >= 16) {
					moveSprite(volt/128, 0, timeElapsed, &carX, &carY);
				}
				else {
					moveSprite(-volt/128, 0, timeElapsed, &carX, &carY);
				}
			}
			*/
		}
	}
}