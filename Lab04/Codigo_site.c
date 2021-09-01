//Luiz Felipe de Moura 00305017
//Pedro Barros Baitelli 00301234

//vers�o utilizando o c�digo exemplo do site 
//data=15.3 code = 546 Time: 00:00:01

/*
Inicializa��o do display:
- Function set: define se ser� trabalhado com 8 ou 4 bits (queremos 4)
	Manda 0010
	Bota 0 no RS
	1 no Enable
	Coloca o valor e baixa o Enable ou Baixa o Enable e coloca o valor
*/

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

void main(void) {

	functionSet();
	entryModeSet(1, 0); // increment and no shift
	displayOnOffControl(1, 1, 1); // display on, cursor on and blinking on
	sendString("Luiz");
	setDdRamAddress(0x40); // set address to start of second line
	sendString("Pedro");

	// The program can be controlled from some of the switches on port 2.
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

void returnHome(void) {
	//0000 0010 = 0x02
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
	//0001 1101 -> 0x1D
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

//recebe 0x40 = 0100 0000
void setDdRamAddress(char address) {
	//RS		R/W		D7		D6		D5		D4		E 
	//0			 0		1			1			0			0			1 -> isso equivale a 0xC
	//RS		R/W		D7		D6		D5		D4		E
	//0			 0		1			1			0			0			0
	RS = 0;
	DB7 = 1;
	DB6 = getBit(address, 6);
	DB5 = getBit(address, 5);
	DB4 = getBit(address, 4);
	E = 1;
	E = 0;
	
	//RS		R/W		D7		D6		D5		D4		E
	//0			 0		0			0			0			0			1 -> isso equivale a 0x0
	//RS		R/W		D7		D6		D5		D4		E
	//0			 0		0			0			0			0			0
	DB7 = getBit(address, 3);
	DB6 = getBit(address, 2);
	DB5 = getBit(address, 1);
	DB4 = getBit(address, 0);
	E = 1;
	E = 0;
	
	delay();
}

// --------------------------------------------------------------------

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