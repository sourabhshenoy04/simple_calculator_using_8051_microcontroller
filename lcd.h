#ifndef LCD_H
#define LCD_H

#include <reg52.h>

#define lcd P2

sbit rs = P3^0;
sbit rw = P3^1;
sbit en = P3^2;

void lcd_init();
void lcd_cmd(unsigned char);
void lcd_dat(unsigned char);
void msdelay(unsigned char);
void lcd_string(unsigned char *);

// LCD initialization
void lcd_init()
{
    lcd_cmd(0x80);
    lcd_cmd(0x38);
    lcd_cmd(0x01);
    lcd_cmd(0x0c);
    lcd_cmd(0x06);
}

// Send commands
void lcd_cmd(unsigned char ch)
{
    lcd = ch;
    rs = 0;
    rw = 0;
    en = 1;
    msdelay(20);
    en = 0;
}

// Send data
void lcd_dat(unsigned char ch)
{
    lcd = ch;
    rs = 1;
    rw = 0;
    en = 1;
    msdelay(20);
    en = 0;
}

// Delay
void msdelay(unsigned char t)
{
    unsigned int i, j;
    for(i = 0; i < t; i++)
        for(j = 0; j < 1275; j++);
}

// Send a string of data
void lcd_string(unsigned char *p)
{
    while(*p != '\0')
    {
        lcd_dat(*p);
        p++;
    }
}

#endif
