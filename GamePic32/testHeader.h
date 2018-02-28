/*
 * testHeader.h
 * Created 2018-02-21
 * by André Lindgren
 * 
 * Header for all test files
 */
void clearMatrix(const unsigned char m, const unsigned char n, unsigned char matrix[m][n]);

void insertSprite(const unsigned char m, const unsigned char n, unsigned char pxlMap[m][n], const unsigned char m2, const unsigned char n2, unsigned char sprite[m2][n2], int x, int y);

unsigned char carHit(const unsigned char m, const unsigned char n, unsigned char pxlMap[m][n], const unsigned char carM, const unsigned char carN, unsigned char car[carM][carN], int x, int y);

void moveSprite(double speedX, double speedY, long lastTime, double* x, double* y);


extern const unsigned char pMM;
extern const unsigned char pMN;

extern unsigned char template1[16][16];

extern unsigned char truck[16][16];

extern unsigned char house[16][16];

extern unsigned char dog[16][16];

extern unsigned char template2[16][16];

extern const unsigned char carTemplateM;
extern const unsigned char carTemplateN;
extern unsigned char carTemplate[8][4];

extern const unsigned char roadTemplateM;
extern const unsigned char roadTemplateN;
extern unsigned char roadTemplate[135][2];