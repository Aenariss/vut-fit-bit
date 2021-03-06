#include "letters.h"

/* Function to fill the given original field with the given letter */
/* I need to know where to start, so that I do not overwrite already written numbers - to do that I use curr_field_length */
void fillField(int curr_field_length, char field[HEIGHT][MAX], char template[HEIGHT][WIDTH]) {
	for (int i=0; i < HEIGHT; i++) {
			for (int k = curr_field_length; k < curr_field_length+8; k++) {
				field[i][k] = template[i][k-curr_field_length];
			}
		}
}

/*********************************
 * TEMPLATES FOR LETTER PRINTING *
 https://www.shutterstock.com/image-vector/led-display-font-digital-scoreboard-alphabet-1440128090   <-- Inspiration
 ********************************/

 /* Function to print letter A */
 /* Takes a field into which the letter is to be added and the position to which to append the letter */
void addA(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'0', '1', '1', '1', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '1', '1', '1', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addB(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'1', '1', '1', '1', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '1', '1', '1', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '1', '1', '1', '0', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addC(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'0', '1', '1', '1', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'0', '1', '1', '1', '0', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addD(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'1', '1', '1', '1', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '1', '1', '1', '0', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addE(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'1', '1', '1', '1', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '1', '1', '1', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '1', '1', '1', '1', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addF(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'1', '1', '1', '1', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '1', '1', '1', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addG(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'1', '1', '1', '1', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '1', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '1', '1', '1', '0', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addH(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '1', '1', '1', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addI(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'0', '0', '1', '1', '1', '0', '0', '0'},
									{'0', '0', '0', '1', '0', '0', '0', '0'},
									{'0', '0', '0', '1', '0', '0', '0', '0'},
									{'0', '0', '0', '1', '0', '0', '0', '0'},
									{'0', '0', '0', '1', '0', '0', '0', '0'},
									{'0', '0', '0', '1', '0', '0', '0', '0'},
									{'0', '0', '1', '1', '1', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addJ(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'0', '1', '1', '1', '0', '0', '0', '0'},
									{'0', '0', '0', '1', '0', '0', '0', '0'},
									{'0', '0', '0', '1', '0', '0', '0', '0'},
									{'0', '0', '0', '1', '0', '0', '0', '0'},
									{'0', '0', '0', '1', '0', '0', '0', '0'},
									{'1', '0', '0', '1', '0', '0', '0', '0'},
									{'0', '1', '1', '0', '0', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addK(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '1', '0', '0', '0', '0'},
									{'1', '0', '1', '0', '0', '0', '0', '0'},
									{'1', '1', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '1', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '1', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addL(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '1', '1', '1', '1', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addM(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '1', '0', '1', '1', '0', '0', '0'},
									{'1', '0', '1', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addN(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '1', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '1', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '1', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addO(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'0', '1', '1', '1', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'0', '1', '1', '1', '0', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addP(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'0', '1', '1', '1', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '1', '1', '1', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addQ(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'0', '1', '1', '1', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '1', '0', '0', '0', '0'},
									{'0', '1', '1', '0', '1', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addR(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'0', '1', '1', '1', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '1', '1', '1', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'0', '0', '0', '0', '1', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addS(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'0', '1', '1', '1', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'0', '1', '1', '1', '0', '0', '0', '0'},
									{'0', '0', '0', '0', '1', '0', '0', '0'},
									{'0', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '1', '1', '1', '0', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addT(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'1', '1', '1', '1', '1', '0', '0', '0'},
									{'0', '0', '1', '0', '0', '0', '0', '0'},
									{'0', '0', '1', '0', '0', '0', '0', '0'},
									{'0', '0', '1', '0', '0', '0', '0', '0'},
									{'0', '0', '1', '0', '0', '0', '0', '0'},
									{'0', '0', '1', '0', '0', '0', '0', '0'},
									{'0', '0', '1', '0', '0', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addU(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'0', '1', '1', '1', '0', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addV(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'0', '1', '0', '1', '0', '0', '0', '0'},
									{'0', '1', '0', '1', '0', '0', '0', '0'},
									{'0', '0', '1', '0', '0', '0', '0', '0'},
									{'0', '0', '1', '0', '0', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addW(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '1', '0', '1', '0', '0', '0'},
									{'1', '0', '1', '0', '1', '0', '0', '0'},
									{'1', '0', '1', '0', '1', '0', '0', '0'},
									{'1', '1', '0', '1', '1', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addX(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'0', '1', '0', '1', '0', '0', '0', '0'},
									{'0', '0', '1', '0', '0', '0', '0', '0'},
									{'0', '1', '0', '1', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addY(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'1', '0', '0', '0', '1', '0', '0', '0'},
									{'0', '1', '0', '1', '0', '0', '0', '0'},
									{'0', '0', '1', '0', '0', '0', '0', '0'},
									{'0', '0', '1', '0', '0', '0', '0', '0'},
									{'0', '0', '1', '0', '0', '0', '0', '0'},
									{'0', '0', '1', '0', '0', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}

void addZ(char field[HEIGHT][MAX], int curr_field_length) {
	char template[WIDTH][HEIGHT] = {{'1', '1', '1', '1', '1', '0', '0', '0'},
									{'0', '0', '0', '0', '1', '0', '0', '0'},
									{'0', '0', '0', '1', '0', '0', '0', '0'},
									{'0', '0', '1', '0', '0', '0', '0', '0'},
									{'0', '1', '0', '0', '0', '0', '0', '0'},
									{'1', '0', '0', '0', '0', '0', '0', '0'},
									{'1', '1', '1', '1', '1', '0', '0', '0'},
									{'0', '0', '0', '0', '0', '0', '0', '0'}};

	fillField(curr_field_length, field, template);
}
/****************************************
 * END OF TEMPLATES FOR LETTER PRINTING *
 ****************************************/
