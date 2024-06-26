#include <reg52.h>
#include "lcd.h"

sbit r4 = P1^0;
sbit r3 = P1^1;
sbit r2 = P1^2;
sbit r1 = P1^3;
sbit c1 = P1^4;
sbit c2 = P1^5;
sbit c3 = P1^6;
sbit c4 = P1^7;

void row_scan();
void delay(unsigned char);
void send_num(unsigned int);
void calculate();
void clear_calculator();
unsigned int a = 0, b = 0;
unsigned char operator, flag = 0;

void main() {
    P1 = 0xf0; // Rows as output and columns as input
    lcd_init(); // Initialize LCD
    lcd_cmd(0x80); // Set cursor to beginning of first line
    lcd_string("CALCULATOR"); // Display title on first line
    lcd_cmd(0xc0); // Set cursor to beginning of second line
    lcd_string("USING 8051"); // Display subtitle on second line
    delay(200); // Delay for stability
    lcd_cmd(0x01); // Clear LCD screen
    while (1) {
        row_scan(); // Continuously scan for button presses
    }
}

void row_scan() {
    r1 = 0; r2 = r3 = r4 = 1; // Scan ROW 1
    if (c1 == 0) { delay(32); while (c1 == 0) { flag = 1; lcd_dat('x'); operator = 'x'; } }
    if (c2 == 0) { delay(32); while (c2 == 0) { lcd_dat('7'); (flag == 0) ? (a = a*10 + 7) : (b = b*10 + 7); } }
    if (c3 == 0) { delay(32); while (c3 == 0) { lcd_dat('4'); (flag == 0) ? (a = a*10 + 4) : (b = b*10 + 4); } }
    if (c4 == 0) { delay(32); while (c4 == 0) { lcd_dat('1'); (flag == 0) ? (a = a*10 + 1) : (b = b*10 + 1); } }

    r2 = 0; r1 = r3 = r4 = 1; // Scan ROW 2
    if (c1 == 0) { delay(32); while (c1 == 0) { lcd_dat('0'); (flag == 0) ? (a = a*10 + 0) : (b = b*10 + 0); } }
    if (c2 == 0) { delay(32); while (c2 == 0) { lcd_dat('8'); (flag == 0) ? (a = a*10 + 8) : (b = b*10 + 8); } }
    if (c3 == 0) { delay(32); while (c3 == 0) { lcd_dat('5'); (flag == 0) ? (a = a*10 + 5) : (b = b*10 + 5); } }
    if (c4 == 0) { delay(32); while (c4 == 0) { lcd_dat('2'); (flag == 0) ? (a = a*10 + 2) : (b = b*10 + 2); } }

    r3 = 0; r1 = r2 = r4 = 1; // Scan ROW 3
    if (c1 == 0) { delay(32); while (c1 == 0) { flag = 1; lcd_dat('/'); operator = '/'; } }
    if (c2 == 0) { delay(32); while (c2 == 0) { lcd_dat('9'); (flag == 0) ? (a = a*10 + 9) : (b = b*10 + 9); } }
    if (c3 == 0) { delay(32); while (c3 == 0) { lcd_dat('6'); (flag == 0) ? (a = a*10 + 6) : (b = b*10 + 6); } }
    if (c4 == 0) { delay(32); while (c4 == 0) { lcd_dat('3'); (flag == 0) ? (a = a*10 + 3) : (b = b*10 + 3); } }

    r4 = 0; r1 = r2 = r3 = 1; // Scan ROW 4
    if (c1 == 0) { delay(32); while (c1 == 0) { flag = 1; lcd_dat('-'); operator = '-'; } }
    if (c2 == 0) { delay(32); while (c2 == 0) { lcd_cmd(0x01); flag = a = b = 0; } } // Clear calculator
    if (c3 == 0) { delay(32); while (c3 == 0) { lcd_dat('='); calculate(); } } // Calculate result
    if (c4 == 0) { delay(32); while (c4 == 0) { flag = 1; lcd_dat('+'); operator = '+'; } }
}

void delay(unsigned char t) {
    unsigned int j;
    unsigned char i;
    for (i = 0; i < t; i++)
        for (j = 0; j < 700; j++);
}

void calculate() {
    lcd_cmd(0xc0); // Set cursor to beginning of second line
    switch (operator) {
        case '+': send_num(a + b); break;
        case '-': (a >= b) ? send_num(a - b) : (lcd_dat('-'), send_num(b - a)); break;
        case '/': (b != 0) ? send_num(a / b) : (lcd_cmd(0x01), lcd_string("INVALID")); break;
        case 'x': send_num(a * b); break;
    }
}

void send_num(unsigned int x) {
    if (x > 9999) {
        lcd_string("OVERFLOW!"); // Display incase of overflow
    } else {
        lcd_dat(x / 10000 + '0'); // Display ten-thousands digit
        lcd_dat((x / 1000) % 10 + '0'); // Display thousands digit
        lcd_dat((x / 100) % 10 + '0'); // Display hundreds digit
        lcd_dat((x / 10) % 10 + '0'); // Display tens digit
        lcd_dat(x % 10 + '0'); // Display units digit
    }
}
