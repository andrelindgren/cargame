/* game.c
 *
 *
 */

#include <pic32mx.h>
#include <stdint.h>
#include <stdlib.h>
#include "testHeader.h"

void setupLevel(int level) {
	int i;
	for (i = 0; i < 5; i++) {
		clearMatrix(16, 16, templateList[i]);
	}
	// City
	if (level == 0) {
		insertSprite(16, 16, templateList[0], 16, 16, truck, 0, 0);
		insertSprite(16, 16, templateList[1], 16, 16, house, 0, 0);
		insertSprite(16, 16, templateList[2], 16, 16, skyscraper, 0, 0);
	} 
	// Forest
	else if (level == 1) {
		insertSprite(16, 16, templateList[0], 16, 16, tree, 0, 0);
		insertSprite(16, 16, templateList[1], 16, 16, dog, 0, 0);
		insertSprite(16, 16, templateList[2], 16, 16, moose, 0, 0);
	}
	// Ocean
	else if (level == 2) {
		insertSprite(16, 16, templateList[0], 16, 16, crab, 0, 0);
		insertSprite(16, 16, templateList[1], 16, 16, beachball, 0, 0);
		insertSprite(16, 16, templateList[2], 16, 16, palmtree, 0, 0);
	}
}


int game (int level, int difficulty) {

	int notFreeSpace = 1;

	//POT
	unsigned int volt;

	// Movespeed
	double speedY = 50 + (difficulty * 30);

	// Variables for for-loops
	int i;
	int j;
	int n;

	//How long each frame is (100 frames/s -> frame = 10000 microseconds)
	long timeElapsed = 10000;
	int runTime = 0;

	// PixelMap
	unsigned char pM[pMM][pMN];
	clearMatrix(pMM, pMN, pM);

	double roadX = 14;
	double roadY = -5;

	unsigned char road[roadTemplateM][roadTemplateN];
	clearMatrix(roadTemplateM, roadTemplateN, road);
	insertSprite(roadTemplateM, roadTemplateN, road, roadTemplateM, roadTemplateN, roadTemplate, 0, 0);

	unsigned char dirt[dirtRoadTemplateM][dirtRoadTemplateN];
	clearMatrix(dirtRoadTemplateM, dirtRoadTemplateN, dirt);
	insertSprite(dirtRoadTemplateM, dirtRoadTemplateN, dirt, dirtRoadTemplateM, dirtRoadTemplateN, dirtRoadTemplate, 0, 0);

	unsigned char ocean[oceanTemplateM][oceanTemplateN];
	clearMatrix(oceanTemplateM, oceanTemplateN, ocean);
	insertSprite(oceanTemplateM, oceanTemplateN, ocean, oceanTemplateM, oceanTemplateN, oceanTemplate, 0, 0);
	// Road
	if (level == 0) {
		roadX = 14;
		roadY = -5;
		
	}
	else {
		roadX = 0;
		roadY = -16;
	}
	// Goal
	double goalX = 0;
	double goalY = -100;
	unsigned char goal[goalTemplateM][goalTemplateN];
	clearMatrix(goalTemplateM, goalTemplateN, goal);
	insertSprite(goalTemplateM, goalTemplateN, goal, goalTemplateM, goalTemplateN, goalTemplate, 0, 0);

	// Car
	double carSpeedY = 0;
	double carX = 4;
	double carY = 120;
	unsigned char car[carTemplateM][carTemplateN];
	clearMatrix(carTemplateM, carTemplateN, car);
	insertSprite(carTemplateM, carTemplateN, car, carTemplateM, carTemplateN, carTemplate, 0, 0);

	//Boat
	unsigned char boat[boatTemplateM][boatTemplateN];
	clearMatrix(boatTemplateM, boatTemplateN, boat);
	insertSprite(boatTemplateM, boatTemplateN, boat, boatTemplateM, boatTemplateN, boatTemplate, 0, 0);

	// Array of Obstacles
	int nObs = 3;
	double obsX[nObs];
	double obsY[nObs];
	unsigned char obs[nObs][16][16];
	
	// Fill list of templates
	setupLevel(level);

	//Assign all obstacles a template
	for (i = 0; i < nObs; i++) {
		obsX[i] = i*12;
		obsY[i] = (-100) - 100*i;
		clearMatrix(16, 16, obs[i]);
		insertSprite(16, 16, obs[i], 16, 16, templateList[i], 0, 0);
	} 

	// Random number
	int r = 0; 

	delay(1000000);
	while (1) {
		
		// Button 3: Pausemenu
		if(((getbtns() & 0x01) != 0)) {
			if (PauseMenu() == 0) {
				return 1;
			}
		}

		if((((getbtns() & 0x04) == 0) && (((getbtns() & 0x02) == 0)))) {
			if (carSpeedY < -0.1) {
				carSpeedY = carSpeedY*(0.9);
			}
			if (carSpeedY > 0.1) {
				carSpeedY = carSpeedY*(0.9);
			}
		}


		if(((getbtns() & 0x04) != 0)) {
			carSpeedY = carSpeedY - 0.01;
		}

		if(((getbtns() & 0x02) != 0)) {
			carSpeedY = carSpeedY + 0.01;
		}
	

		AD1CON1 |= (0x1 << 1);
		while(!(AD1CON1 & (0x1 << 1)));
		while(!(AD1CON1 & 0x1));
			
		/* Get the analog value */
		volt = ADC1BUF0;
		carX = volt / 32;

		if (level == 2) {
			if (carX >= 32-boatTemplateN) {
				carX = 32-boatTemplateN;
			}
		}
		else {
			if (carX >= 32-carTemplateN) {
				carX = 32-carTemplateN;
			}
		}
		//Fix for pot hardware glitching

		

		// Frameupdate
		if (timerEnd()) {
			runTime++;


			if (speedY < 100 + (difficulty*40)) {
				speedY = speedY + 0.1;
			}

			clearMatrix(pMM, pMN, pM);

			// Insert obstacles
			for (i = 0; i < nObs; i++) {
				insertSprite(pMM, pMN, pM, 16, 16, obs[i], (int) obsX[i], (int) obsY[i]);
			}
			
			// Hit detection
			if (level == 2) {
				if (carHit(pMM, pMN, pM, boatTemplateM, boatTemplateN, boat, carX, carY)) {
					return 0;
				} 
			}
			else {
				if (carHit(pMM, pMN, pM, carTemplateM, carTemplateN, car, carX, carY)) {
					return 0;
				} 
			}

			
			// Insert goal
			insertSprite(pMM, pMN, pM, goalTemplateM, goalTemplateN, goal, (int) goalX, (int) goalY);
			// Insert road and car
			if (level == 0) {
				insertSprite(pMM, pMN, pM, roadTemplateM, roadTemplateN, road, (int) roadX, (int) roadY);
				insertSprite(pMM, pMN, pM, carTemplateM, carTemplateN, car, carX, carY);
			} 
			else if (level == 1) {
				for (i = 0; i < 9; i++) {
					insertSprite(pMM, pMN, pM, dirtRoadTemplateM, dirtRoadTemplateN, dirt, (int) roadX, (int) roadY + i*16);
				}
				insertSprite(pMM, pMN, pM, carTemplateM, carTemplateN, car, carX, carY);
			}
			else {
				for (i = 0; i < 9; i++) {
					insertSprite(pMM, pMN, pM, oceanTemplateM, oceanTemplateN, ocean, (int) roadX, (int) roadY + i*16);
				}
				insertSprite(pMM, pMN, pM, boatTemplateM, boatTemplateN, boat, carX, carY);
			}

			
	
			display_image(0, pM);
			
			for (i = 0; i < nObs; i++) {
				moveSprite(0, speedY, timeElapsed, &obsX[i], &obsY[i]);
				if (obsY[i] > pMM) {
					notFreeSpace = 1;
					while (notFreeSpace) {

						srand(TMR2);
						r = rand() % 400;
						obsY[i] = -r;
	
						srand(TMR2+1);
						r = rand() % (32 - (16));
						obsX[i] = r;
	
						srand(TMR2+2);
						r = rand() % 3;
						
						clearMatrix(16, 16, obs[i]);
						insertSprite(16, 16, obs[i], 16, 16, templateList[r], 0, 0);
						
						j = 0;
						while (j < nObs) {
							if (j != i && ((obsX[j] > obsX[i] + 16) || (obsX[j] + 16 < obsX[i]) || (obsY[j] > obsY[i] + 16) || (obsY[j] + 16 < obsY[i]))) {
								notFreeSpace = 0;
							}
							j++;
						}
					}
				}
			}

			if (runTime > 100) {
				moveSprite(0, speedY, timeElapsed, &goalX, &goalY);
				if (goalY > carY) {
					return 2;
				}
			}

			moveSprite(0, speedY, timeElapsed, &roadX, &roadY);
			if (level == 0 && roadY > 5) {
				roadY = roadY - 10;
			}
			else if (level != 0 && roadY > 0) {
				roadY = roadY - 16;
			}

			moveSprite(0, carSpeedY, timeElapsed, &carX, &carY);
			if (carY > 120) {
				carY = 120;
			}
			if (carY < 8) {
				carY = 8;
			}
		}
	}
}