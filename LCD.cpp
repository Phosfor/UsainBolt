/*
 * LCD.cpp
 *
 *  Created on: 07.05.2015
 *      Author: gfu-mri
 */

#include "LCD.h"

LCD::LCD() {
	// verwendete Pins auf Ausgang schalten
	uint8_t pins = (0x0F << LCD_DB) |           // 4 Datenleitungen
				   (1<<LCD_RS) |                // R/S Leitung
				   (1<<LCD_EN);                 // Enable Leitung
	LCD_DDR |= pins;

	// initial alle Ausgänge auf Null
	LCD_PORT &= ~pins;

	// warten auf die Bereitschaft des LCD
	_delay_ms( 15 );

	// Soft-Reset muss 3mal hintereinander gesendet werden zur Initialisierung
	out4( LCD_SOFT_RESET );
	_delay_ms( 5 );

	enablePulse();
	_delay_ms( 1 );

	enablePulse();
	_delay_ms( 1 );

	// 4-bit Modus aktivieren
	out4( LCD_SET_FUNCTION |
		  LCD_FUNCTION_4BIT );
	_delay_ms( 5 );

	// 4-bit Modus / 2 Zeilen / 5x7
	sendCommand( LCD_SET_FUNCTION |
			 	 LCD_FUNCTION_4BIT |
			 	 LCD_FUNCTION_2LINE |
			 	 LCD_FUNCTION_5X7 );

	// Display ein / Cursor aus / Blinken aus
	sendCommand( LCD_SET_DISPLAY |
			 	 LCD_DISPLAY_ON |
			 	 LCD_CURSOR_OFF |
			 	 LCD_BLINKING_OFF);

	// Cursor inkrement / kein Scrollen
	sendCommand( LCD_SET_ENTRY |
			 	 LCD_ENTRY_INCREASE |
			 	 LCD_ENTRY_NOSHIFT );

	clear();
}

LCD::~LCD() {
	// TODO Auto-generated destructor stub
}

// Erzeugt einen Enable-Puls
void LCD::enablePulse()
{
    LCD_PORT |= (1<<LCD_EN);     // Enable auf 1 setzen
    _delay_us( 1 );  // kurze Pause
    LCD_PORT &= ~(1<<LCD_EN);    // Enable auf 0 setzen
}

// Sendet eine 4-bit Ausgabeoperation an das LCD
void LCD::out4( uint8_t data )
{
    data &= 0xF0;                       // obere 4 Bit maskieren

    LCD_PORT &= ~(0xF0>>(4-LCD_DB));    // Maske löschen
    LCD_PORT |= (data>>(4-LCD_DB));     // Bits setzen
    enablePulse();
}

// Sendet ein Datenbyte an das LCD
void LCD::sendData( uint8_t data )
{
    LCD_PORT |= (1<<LCD_RS);    // RS auf 1 setzen

    out4( data );            // zuerst die oberen,
    out4( data<<4 );         // dann die unteren 4 Bit senden

    _delay_us( 46 );
}

// Sendet einen Befehl an das LCD
void LCD::sendCommand( uint8_t data )
{
    LCD_PORT &= ~(1<<LCD_RS);    // RS auf 0 setzen

    out4( data );             // zuerst die oberen,
    out4( data<<4 );           // dann die unteren 4 Bit senden

    _delay_us( 42 );
}

// Sendet den Befehl zur Löschung des Displays
void LCD::clear( void )
{
    sendCommand( LCD_CLEAR_DISPLAY );
    _delay_ms( 2 );
}

// Sendet den Befehl: Cursor Home
void LCD::home( void )
{
	sendCommand( LCD_CURSOR_HOME );
    _delay_ms( 2 );
}

void LCD::setCursor(uint8_t column, uint8_t row)
{
#define LCD_DDADR_LINE1         0x00
#define LCD_DDADR_LINE2         0x40
#define LCD_DDADR_LINE3         0x10
#define LCD_DDADR_LINE4         0x50

	uint8_t data = LCD_SET_DDADR;
	switch(row)
	{
	case 1:
		data += LCD_DDADR_LINE1; break;
	case 2:
		data += LCD_DDADR_LINE2; break;/*
	case 3:
		data += LCD_DDADR_LINE3; break;
	case 4:
		data += LCD_DDADR_LINE4; break;*/
	}
	data += column;
	sendCommand(data);
}

// Schreibt einen String auf das LCD
void LCD::print( const char *data )
{
    while( *data != '\0' )
        sendData( *data++ );
}
