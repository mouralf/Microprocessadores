#include <reg51.h>

sbit DB7 = P1^7;
sbit DB6 = P1^6;
sbit DB5 = P1^5;
sbit DB4 = P1^4;
sbit RS = P1^3;
sbit E = P1^2;

sbit clear = P2^4;
sbit ret = P2^5;				  
sbit left = P2^6;
sbit right = P2^7;

void returnHome(void);
void entryModeSet(bit id, bit s);
void displayOnOffControl(bit display, bit cursor, bit blinking);
void cursorOrDisplayShift(bit sc, bit rl);
void functionSet(void);
void setDdRamAddress(char address);

void sendChar(char c);
void sendString(char* str);
bit getBit(char c, char bitNumber);
void delay(void);

char code LISTA[] = {
'N','Y','A','H',' ','H','U','S','T','O','N',0, 7, 90, 9, 11, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0,  //7,90; 9,11; 0,0; 0,0; 0, 0; 0,0
'J','A','G','G','E','R',' ','E','A','T','O','N',0, 8, 20, 9, 5, 0, 0, 8, 70,9,40, 0, 0, 0, 0, //8,20; 9,05; 0,0; 8,70; 9,40; 0,0; 0,0
'G','U','S','T','A','V','O',' ','R','I','B','E','I','R','O',0, 7, 23, 5, 82, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0,  //7,23; 5,82; 0,0; 0,0; 0,0; 0,0; 2,0
'K','E','L','V','I','N',' ','H','O','E','F','L','E','R',0, 8, 98, 8, 84, 8, 99, 0, 0, 0, 0, 7,58, 9,34, //8,98; 8,84; 8,99; 0,0; 0,0; 7,58; 9,34
'V','I','C','E','N','T',' ','M','I','L','O','U',0, 7, 87, 5, 54, 9, 23, 0, 0, 8, 34, 0,0, 8,70, //7,87; 5,54; 9,23; 0,0; 8,34; 0,0; 8,70
'A','N','G','E','L','O',' ','N','A','R','V','A','E','Z',0, 7, 1, 6, 9, 9, 0, 0, 0, 0, 0, 8,65, 8,21, //7,01; 6,89; 9,00; 0,0; 0,0; 8,65; 8,21
'A','U','R','E','L','I','A','N',' ','G','R','A','U','D',0, 4, 21, 7, 20, 8, 68, 0, 0, 9, 0, 0, 0, 0, 0, //4,21; 7,20; 8,68; 0,0; 9,0; 0,0;
'Y','U','T','O',' ','H','O','R','I','G','A','M','E',0, 8, 02, 6, 77, 9, 3, 0, 0, 9, 35, 9, 50, 9,30 //8,02; 6,77; 9,03; 0,0; 9,35; 9,50; 9,30
};

void main(void) {
	char teste[5] = {'p', 'e', 'n', 'i', 's'};
	unsigned char j = 0, letrasNome;
	unsigned int notasFinais[8] = {3718, 3615};
	char code LISTA [] = {'N','Y','A','H',' '};
	
	functionSet();
	entryModeSet(1, 0); // increment and no shift
	displayOnOffControl(1, 1, 1); // display on, cursor on and blinking on
	
	letrasNome = 0;
			//pega o nome de cada competidor do podio
	while (LISTA[letrasNome] != ' '){
		sendChar(LISTA[letrasNome]); //armazena em RESULT o nome do competidor (ordem de podio)
		letrasNome++;
}
			sendChar(' ');
	for (j = 0; j<2; j++){
			sendChar((notasFinais[j]/1000)+48); //armazena em RESULT o dígito da dezena
			sendChar(((notasFinais[j]/100)%10)+48); //armazena em RESULT o dígito da unidade
			sendChar(',');
			sendChar(((notasFinais[j]-(notasFinais[j]/100)*100)/10)+48); ////armazena em RESULT o dígito da dezena após a vírgula
			sendChar (((notasFinais[j]-(notasFinais[j]/100)*100)%10)+48); //armazena em RESULT o dígito da unidade após a virgula
			setDdRamAddress(0x40); // set address to start of second line

	}
	

	

	// The program can be controlled via some of the switches on port 2.
	// If switch 5 is closed the cursor returns home (address 0).
	// Otherwise, switches 6 and 7 are read - if both switches are open or both switches 
	//      are closed, the display does not shift.
	// If switch 7 is closed, continuously shift left.
	// If switch 6 is closed, continuously shift right.
	while (1) {
		if (ret == 0) {
			returnHome();
		}
		else {	
			if (left == 0 && right == 1) {
				cursorOrDisplayShift(1, 0); // shift display left
			}
			else if (left == 1 && right == 0) {
				cursorOrDisplayShift(1, 1); // shift display right
			}	
		}	
	}
}

// LCD Module instructions -------------------------------------------
// To understand why the pins are being set to the particular values in the functions
// below, see the instruction set.
// A full explanation of the LCD Module: HD44780.pdf

void returnHome(void) {
	RS = 0;
	DB7 = 0;
	DB6 = 0;
	DB5 = 0;
	DB4 = 0;
	E = 1;
	E = 0;
	DB5 = 1;
	E = 1;
	E = 0;
	delay();
}	

void entryModeSet(bit id, bit s) {
	RS = 0;
	DB7 = 0;
	DB6 = 0;
	DB5 = 0;
	DB4 = 0;
	E = 1;
	E = 0;
	DB6 = 1;
	DB5 = id;
	DB4 = s;
	E = 1;
	E = 0;
	delay();
}

void displayOnOffControl(bit display, bit cursor, bit blinking) {
	DB7 = 0;
	DB6 = 0;
	DB5 = 0;
	DB4 = 0;
	E = 1;
	E = 0;
	DB7 = 1;
	DB6 = display;
	DB5 = cursor;
	DB4 = blinking;
	E = 1;
	E = 0;
	delay();
}

void cursorOrDisplayShift(bit sc, bit rl) {
	RS = 0;
	DB7 = 0;
	DB6 = 0;
	DB5 = 0;
	DB4 = 1;
	E = 1;
	E = 0;
	DB7 = sc;
	DB6 = rl;
	E = 1;
	E = 0;
	delay();
}

void functionSet(void) {
	// The high nibble for the function set is actually sent twice. Why? See 4-bit operation
	// on pages 39 and 42 of HD44780.pdf.
	DB7 = 0;
	DB6 = 0;
	DB5 = 1;
	DB4 = 0;
	RS = 0;
	E = 1;
	E = 0;
	delay();
	E = 1;
	E = 0;
	DB7 = 1;
	E = 1;
	E = 0;
	delay();
}

void setDdRamAddress(char address) {
	RS = 0;
	DB7 = 1;
	DB6 = getBit(address, 6);
	DB5 = getBit(address, 5);
	DB4 = getBit(address, 4);
	E = 1;
	E = 0;
	DB7 = getBit(address, 3);
	DB6 = getBit(address, 2);
	DB5 = getBit(address, 1);
	DB4 = getBit(address, 0);
	E = 1;
	E = 0;
	delay();
}

void sendChar(char c) {
	DB7 = getBit(c, 7);
	DB6 = getBit(c, 6);
	DB5 = getBit(c, 5);
	DB4 = getBit(c, 4);
	RS = 1;
	E = 1;
	E = 0;
	DB7 = getBit(c, 3);
	DB6 = getBit(c, 2);
	DB5 = getBit(c, 1);
	DB4 = getBit(c, 0);
	E = 1;
	E = 0;
	delay();
}

// -- End of LCD Module instructions
// --------------------------------------------------------------------

void sendString(char* str) {
	int index = 0;
	while (str[index] != 0) {
		sendChar(str[index]);
		index++;
	}
}

bit getBit(char c, char bitNumber) {
	return (c >> bitNumber) & 1;
}

void delay(void) {
	char c;
	for (c = 0; c < 50; c++);
}