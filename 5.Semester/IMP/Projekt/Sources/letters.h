#ifndef __LETTERS_H__
#define __LETTERS_H__

#define HEIGHT 8	// I have 16x8 display, which means 8 is the max height
#define WIDTH 8		// To be used with letter templates, which are 8 'lights' long
#define MAX 64		// Max display length to be displayed in 1 message (aka 64/8 characters)

void fillField(int curr_field_length, char field[HEIGHT][MAX], char template[HEIGHT][WIDTH]);

void addA(char field[HEIGHT][MAX], int curr_field_length);
void addB(char field[HEIGHT][MAX], int curr_field_length);
void addC(char field[HEIGHT][MAX], int curr_field_length);
void addD(char field[HEIGHT][MAX], int curr_field_length);
void addE(char field[HEIGHT][MAX], int curr_field_length);
void addF(char field[HEIGHT][MAX], int curr_field_length);
void addG(char field[HEIGHT][MAX], int curr_field_length);
void addH(char field[HEIGHT][MAX], int curr_field_length);
void addI(char field[HEIGHT][MAX], int curr_field_length);
void addJ(char field[HEIGHT][MAX], int curr_field_length);
void addK(char field[HEIGHT][MAX], int curr_field_length);
void addL(char field[HEIGHT][MAX], int curr_field_length);
void addM(char field[HEIGHT][MAX], int curr_field_length);
void addN(char field[HEIGHT][MAX], int curr_field_length);
void addO(char field[HEIGHT][MAX], int curr_field_length);
void addP(char field[HEIGHT][MAX], int curr_field_length);
void addQ(char field[HEIGHT][MAX], int curr_field_length);
void addR(char field[HEIGHT][MAX], int curr_field_length);
void addS(char field[HEIGHT][MAX], int curr_field_length);
void addT(char field[HEIGHT][MAX], int curr_field_length);
void addU(char field[HEIGHT][MAX], int curr_field_length);
void addV(char field[HEIGHT][MAX], int curr_field_length);
void addW(char field[HEIGHT][MAX], int curr_field_length);
void addX(char field[HEIGHT][MAX], int curr_field_length);
void addY(char field[HEIGHT][MAX], int curr_field_length);
void addZ(char field[HEIGHT][MAX], int curr_field_length);

#endif