/*
 * matrix.c
 *
 * by André Lindgren
 * Created 2018-02-21
 *
 * Handles matrices like sprites and pixelMap
 */

#include <stdlib.h>
#include <stdio.h>

void clearMatrix(const unsigned char m, const unsigned char n, unsigned char matrix[m][n]) {
	unsigned char i;
	unsigned char j;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			matrix[i][j] = 0;
		}
	}
}

void insertSprite(const unsigned char m, const unsigned char n, unsigned char pxlMap[m][n], const unsigned char m2, const unsigned char n2, unsigned char sprite[m2][n2], int x, int y) {
	unsigned char i;
	unsigned char j;
	for(i = 0; i < m2; i++) {
		for (j = 0; j < n2; j++) {
			if (y+i >= 0 && y+i < m && x+j >= 0 && x+j < n) {
				if (sprite[i][j] == 1) {
					pxlMap[y+i][x+j] = sprite[i][j];
				}
			}
		}
	}
}

unsigned char carHit(const unsigned char m, const unsigned char n, unsigned char pxlMap[m][n], const unsigned char carM, const unsigned char carN, unsigned char car[carM][carN], int x, int y) {
	unsigned char i;
	unsigned char j;
	for (i = 0; i < carM; i++) {
		for (j = 0; j < carN; j++) {
			if (y+i >= 0 && y+i < m && x+j >= 0 && x+j < n) {
				if (car[i][j] == 1 && pxlMap[y+i][x+j] == 1) {
					return 1;
				}
			}
		}
	}
	return 0;
}

/*
 * speedX = y/frame 
 */
void moveSprite(double speedX, double speedY, long lastTime, double* x, double* y) {
	*x = *x + speedX * (((double) lastTime)/1000000);
	*y = *y + speedY * (((double) lastTime)/1000000);
}