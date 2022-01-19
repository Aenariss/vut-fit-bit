/* VUT FIT IMP 2021/2022 */
/* PROJEKT -- Světelná Tabule (FITkit3) */
/* Autor: Vojtěch Fiala (xfiala61) */

/* Tento program je založen na ukázkovém programu 'IMP_projekt - had_tabule_test', který vytvořil Ing. Václav Šimek */
/* Dále tento program vychází z demonstračního cvičení k FITkitu3 a demonstračního souboru k k němu poskytnutému, který vytvořil Ing. Michal Bidlo Ph.D. */
/* U jednotlivých převzatých funkcí a částí funkcí bude uveden autor */

#include "MK60D10.h"
#include "letters.h"
/* Macros for bit-level registers manipulation */
/* Taken from 'IMP_projekt - had_tabule_test' */
#define GPIO_PIN_MASK	0x1Fu
#define GPIO_PIN(x)		(((1)<<(x & GPIO_PIN_MASK)))

/* Button address definitions */
/* Taken from FITkit3 demo */
#define BTN_SW2 0x400     // Port E, bit 10
#define BTN_SW3 0x1000    // Port E, bit 12
#define BTN_SW4 0x8000000 // Port E, bit 27
#define BTN_SW5 0x4000000 // Port E, bit 26
#define BTN_SW6 0x800     // Port E, bit 11

#define TWO_DISPLAYS 16	// Length of the whole matrix display (2x8)

/* Configuration of the necessary MCU peripherals */
/* Taken from 'IMP_projekt - had_tabule_test' */
void SystemConfig() {
	/* Turn on all port clocks */
	SIM->SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;
	SIM->SCGC1 = SIM_SCGC1_UART5_MASK;

	/* Set corresponding PTA pins (column activators of 74HC154) for GPIO functionality */
	PORTA->PCR[8] = ( 0|PORT_PCR_MUX(0x01) );  // A0
	PORTA->PCR[10] = ( 0|PORT_PCR_MUX(0x01) ); // A1
	PORTA->PCR[6] = ( 0|PORT_PCR_MUX(0x01) );  // A2
	PORTA->PCR[11] = ( 0|PORT_PCR_MUX(0x01) ); // A3

	/* Set corresponding PTA pins (rows selectors) for GPIO functionality */
	PORTA->PCR[26] = ( 0|PORT_PCR_MUX(0x01) );  // R0
	PORTA->PCR[24] = ( 0|PORT_PCR_MUX(0x01) );  // R1
	PORTA->PCR[9] = ( 0|PORT_PCR_MUX(0x01) );   // R2
	PORTA->PCR[25] = ( 0|PORT_PCR_MUX(0x01) );  // R3
	PORTA->PCR[28] = ( 0|PORT_PCR_MUX(0x01) );  // R4
	PORTA->PCR[7] = ( 0|PORT_PCR_MUX(0x01) );   // R5
	PORTA->PCR[27] = ( 0|PORT_PCR_MUX(0x01) );  // R6
	PORTA->PCR[29] = ( 0|PORT_PCR_MUX(0x01) );  // R7

	/* Set corresponding PTE pins (output enable of 74HC154) for GPIO functionality */
	PORTE->PCR[28] = ( 0|PORT_PCR_MUX(0x01) ); // #EN

	/* Taken from FITkit3 demo */
    PORTE->PCR[10] = PORT_PCR_MUX(0x01); // SW2
    PORTE->PCR[11] = PORT_PCR_MUX(0x01); // SW6
    PORTE->PCR[12] = PORT_PCR_MUX(0x01); // SW3
    PORTE->PCR[27] = PORT_PCR_MUX(0x01); // SW4
    PORTE->PCR[26] = PORT_PCR_MUX(0x01); // SW5

	/* Change corresponding PTA and PTE port pins as outputs */
	PTA->PDDR = GPIO_PDDR_PDD(0x3F000FC0);
	PTE->PDDR |= GPIO_PDDR_PDD( GPIO_PIN(28) );

}

/* Initialize the MCU - basic clock settings, turning the watchdog off */
/* Taken from FITkit3 demo */
void MCUInit(void)  {
    MCG_C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
    SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}


/* Variable delay loop */
/* Taken from 'IMP_projekt - had_tabule_test' */
void delay(int t1, int t2)
{
	int i, j;

	for(i=0; i<t1; i++) {
		for(j=0; j<t2; j++);
	}
}

/* Conversion of requested column number into the 4-to-16 decoder control.  */
/* used to select the column I want to light up */
/* Taken from 'IMP_projekt - had_tabule_test' */
void column_select(unsigned int col_num)
{
	unsigned i, result, col_sel[4];

	for (i =0; i<4; i++) {
		result = col_num / 2;	  // Whole-number division of the input number
		col_sel[i] = col_num % 2;
		col_num = result;

		switch(i) {

			// Selection signal A0
			case 0:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(8))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(8)));
				break;

			// Selection signal A1
			case 1:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(10))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(10)));
				break;

			// Selection signal A2
			case 2:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(6))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(6)));
				break;

			// Selection signal A3
			case 3:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(11))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(11)));
				break;

			// Otherwise nothing to do...
			default:
				break;
		}
	}
}

/* Function to turn off all rows and cols */
void clear() {
	PTA->PDOR = 0x00;
}

/* Function to select the row I want to light up */
/* Turns on only the desired row, the others are turned off */
void row_select(unsigned int row_num)
{
	switch(row_num) {

		// Selection signal R0
		case 0:
			PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(29));

		// Selection signal R1
		case 1:
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(26));
			PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(29));
			break;

		// Selection signal R2
		case 2:
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(24));
			PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(29));
			break;

		// Selection signal R3
		case 3:
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(9));
			PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(29));
			break;

		// R4
		case 4:
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(25));
			PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(29));
			break;

		// R5
		case 5:
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(28));
			PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(29));
			break;

		// R6
		case 6:
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(7));
			PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(29));
			break;

		// R7
		case 7:
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(27));
			PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(29));
			break;

		// If a wrong row number was given, break
		default:
			break;
		}
}

/* Function to ease turning on a light by its index position */
void select_spot(unsigned int x, unsigned int y) {
	// Check if the given coordinates are valid
	if (x >= 0 && x < TWO_DISPLAYS && y >= 0 && y < TWO_DISPLAYS) {
		column_select(x);
		row_select(y);
	}

	// Use what I learned at the 3rd computer lab
	// aka if the led blinks often enough, human eye wont perceive it as blinking
	delay(350,1);
}

/* Function to be called from the main infinity loop to light up the lights that make up a letter */
/* Takes a whole 2D char array, the length of its rows and a 'movement' number to achieve moving on the display */
void showField(char field[HEIGHT][MAX], int length, int z) {
	for (int k = 0; k < length; k++) {
		for (int i=0; i < HEIGHT; i++) {
			if (field[i][k] == '1')
				select_spot(z-7-k, 7-i);
		}
	}
	clear();
}

/* Function to create the string message that will be projected onto the display */
/* Takes an empty 2d char array, the message I want to show and its length */
void createString(char field[HEIGHT][MAX], char* str, int str_length) {
	int curr_field_length = 0;
	for (int i=0; i < str_length; i++) {
		// Switch for each capital letter of the english alphabet
		switch(str[i]) {
			case 'A':
				// On each match, the number is 'appended' to the field
				addA(field, curr_field_length);
				break;
			case 'B':
				addB(field, curr_field_length);
				break;
			case 'C':
				addC(field, curr_field_length);
				break;
			case 'D':
				addD(field, curr_field_length);
				break;
			case 'E':
				addE(field, curr_field_length);
				break;
			case 'F':
				addF(field, curr_field_length);
				break;
			case 'G':
				addG(field, curr_field_length);
				break;
			case 'H':
				addH(field, curr_field_length);
				break;
			case 'I':
				addI(field, curr_field_length);
				break;
			case 'J':
				addJ(field, curr_field_length);
				break;
			case 'K':
				addK(field, curr_field_length);
				break;
			case 'L':
				addL(field, curr_field_length);
				break;
			case 'M':
				addM(field, curr_field_length);
				break;
			case 'N':
				addN(field, curr_field_length);
				break;
			case 'O':
				addO(field, curr_field_length);
				break;
			case 'P':
				addP(field, curr_field_length);
				break;
			case 'Q':
				addQ(field, curr_field_length);
				break;
			case 'R':
				addR(field, curr_field_length);
				break;
			case 'S':
				addS(field, curr_field_length);
				break;
			case 'T':
				addT(field, curr_field_length);
				break;
			case 'U':
				addU(field, curr_field_length);
				break;
			case 'V':
				addV(field, curr_field_length);
				break;
			case 'W':
				addW(field, curr_field_length);
				break;
			case 'X':
				addX(field, curr_field_length);
				break;
			case 'Y':
				addY(field, curr_field_length);
				break;
			case 'Z':
				addZ(field, curr_field_length);
				break;
			default:	// If given char was something template of which I havent programmed, w/e, break
				break;
		}
		// Move current index by 8, because each template is 8 chars wide
		curr_field_length += 8;
	}
}

/* Main driver function */
int main() {

	MCUInit();
	SystemConfig();
	char field[HEIGHT][MAX];	// Definition of the big array the message is going to be stored into
	int max_length;				// Max length to be used in the function to print the field
	int for_loop_limit;			// Limit of the main for loop
	int str_length;				// Length of given string
	for (;;) { // Never leave main
		// Checking if a button was pressed was, again, taken from FITkit3 demo
		if (!(GPIOE_PDIR & BTN_SW2)) {
			char *display_text = "TEST";	// string to be shown
			str_length = 4;					// length of the string
			max_length = str_length * 8;	// max length is basically length of string times 8, because each letter has template of width 8
			for_loop_limit = max_length * 2 + 20;	// Loop limit has to be higher than max_length for the "fade out" effect so that the text doesnt just disappear, but runs "off" the display
			createString(field, display_text, str_length);
			for (int i=0; i<for_loop_limit; i++) {
				for (int k=0; k < 12; k++) {	// Call the print function at least a few time, otherwise it just blinks and doesnt look good
					showField(field, max_length, i);
				}
			}
		}
		else if (!(GPIOE_PDIR & BTN_SW3)) {
			char *display_text = "VUT";	// Message
			str_length = 3;
			max_length = str_length * 8;
			for_loop_limit = max_length * 2 + 10;
			createString(field, display_text, str_length);
			for (int i=0; i<for_loop_limit; i++) {
				for (int k=0; k < 12; k++) {
					showField(field, max_length, i);
				}
			}
		}
		else if (!(GPIOE_PDIR & BTN_SW5)) {
			char *display_text = "FIT";
			str_length = 3;
			max_length = str_length * 8;
			for_loop_limit = max_length * 2 + 10;
			createString(field, display_text, str_length);
			for (int i=0; i<for_loop_limit; i++) {
				for (int k=0; k < 12; k++) {
					showField(field, max_length, i);
				}
			}
		}
		else if (!(GPIOE_PDIR & BTN_SW4)) {
			char *display_text = "MSG";
			str_length = 3;
			max_length = str_length * 8;
			for_loop_limit = max_length * 2 +10;
			createString(field, display_text, str_length);
			for (int i=0; i<for_loop_limit; i++) {
				for (int k=0; k < 12; k++) {
					showField(field, max_length, i);
				}
			}
		}
		else if (!(GPIOE_PDIR & BTN_SW6)) {
			char *display_text = "IMP";
			str_length = 3;
			max_length = str_length * 8;
			for_loop_limit = max_length * 2 +10;
			createString(field, display_text, str_length);
			for (int i=0; i<for_loop_limit; i++) {
				for (int k=0; k < 12; k++) {
					showField(field, max_length, i);
				}
			}
		}
	}
    return 0;
}
