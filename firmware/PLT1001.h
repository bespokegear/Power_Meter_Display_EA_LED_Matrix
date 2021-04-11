/*

Copyright (c) 2016, Embedded Adventures
All rights reserved.

Contact us at source [at] embeddedadventures.com
www.embeddedadventures.com

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

- Neither the name of Embedded Adventures nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
THE POSSIBILITY OF SUCH DAMAGE.

*/

/*	PLT1001 LED Matrix display driver Arduino library
	Written originally by Embedded Adventures
	v2.0
*/

/*  Modified by Matthew Gates, 2017-04-09
 *  - Changed HardwareSerial to SoftwareSerial
 *  - I2C code removal for smaller flash footprint
 */

#ifndef __PLT1001_H_
#define	__PLT1001_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

#ifndef		uns8
	#define		uns8	uint8_t
#endif

#ifndef		uns16
	#define		uns16	uint16_t
#endif

#define 	PLT1001_ADDR		0x20
#define		PLT1001_DELAY		2

//Command defines 	->			I2C,	Serial
#define		CMD_TITLE			0x00,	(String)"title\r"
#define		CMD_VERSION			0x01,	(String)""
#define		CMD_BOARD			0x02,	(String)""
#define		CMD_NUM_DISPLAYS	0x03,	(String)""
#define		CMD_PAINT			0x04,	(String)"paint\r"
#define		CMD_CLEAR			0x05,	(String)"clear\r"

#define		CMD_ENABLEACTIVE	0x06,	(String)"enableactive "
#define		CMD_SET_DISPLAYS	0x07,	(String)"displays "
#define		CMD_TEST			0x08,	(String)""
#define		CMD_INVERT			0x09,	
#define		CMD_BRIGHTNESS		0x0A,	(String)""
#define		CMD_FONT			0x0B,	(String)"font "
#define		CMD_SCROLLSPEED		0x0C,	(String)"scrollspeed "

#define		CMD_PIXEL			0x0D,	(String)"pixel "

#define		CMD_CIRCLE			0x0E,	(String)"circle"

#define		CMD_LINE			0x0F,	(String)"line "
#define		CMD_RECT			0x0F,	(String)"rect "

#define		CMD_TEXT			0x10,	(String)"text "

#define		CMD_SCROLL			0x11,	(String)"scroll "
#define		CMD_SCROLLSTOP		0x12,	(String)""

class PLT1001 {
private:
	SoftwareSerial *_mySerial;

protected:
	uns8			_currentFont;

private:
	void	command(uns8 i2c_cmd, String s, uns8 param);
	void	command(uns8 i2c_cmd, String s, uns8 param_clr, uns16 param_x, uns16 param_y, int param_rad = -1);
	void	command(uns8 i2c_cmd, String s, uns8 param_clr, uns16 param_x1, uns16 param_y1, uns16 param_x2, uns16 param_y2);
	void	command(uns8 i2c_cmd, String s, uns8 param_clr, uns8 param_x, uns8 param_y, char* text);
	void	command(uns8 i2c_cmd, String s, uns8 param_clr, uns8 param_x, uns8 param_y, const char* text);
	void	command(uns8 i2c_cmd, String s, uns8 param_clr, uns16 param_x1, uns16 param_y1, uns16 param_x2, uns16 param_y2, char* text);
	void	command(uns8 i2c_cmd, String s, uns8 param_clr, uns16 param_x1, uns16 param_y1, uns16 param_x2, uns16 param_y2, const char* text);
	
public:
	PLT1001();
	
	void	begin(SoftwareSerial* ss, unsigned long baud=115200);
	void	title();
	void	paint();
	void	clear();
	void  panels(uns8 num_panels);
	void	enableActiveHigh(bool en);
	void	setFont(uns8 font);
	void	setScrollspeed(uns8 speed);
	
	void	pixel(uns8 color, uns16 x, uns16 y);
	
	void	line(uns8 color, uns16 x1, uns16 y1, uns16 x2, uns16 y2);
	void	rectangle(uns8 color, uns16 x, uns16 y, uns16 wd, uns16 ht);
	
	void 	circle(uns8 color, uns16 x, uns16 y, uns8 radius);
	void 	filledCircle(uns8 color, uns16 x, uns16 y, uns8 radius);
	void 	circle2(uns8 color, uns16 x, uns16 y, uns8 radius);
	void 	filledCircle2(uns8 color, uns16 x, uns16 y, uns8 radius);	
	
	void 	text(uns8 color, uns16 x, uns16 y, char* text);
	void 	text(uns8 color, uns16 x, uns16 y, const char* text);
	void 	text(uns8 color, uns16 x, uns16 y, String text);
	void 	text(uns8 color, uns16 x, uns16 y, uns8 text);
	
	void 	scroll(uns8 color, uns16 x, uns16 y, uns16 wd, uns16 ht, char* text);
	void 	scroll(uns8 color, uns16 x, uns16 y, uns16 wd, uns16 ht, String text);
	void 	scroll(uns8 color, uns16 x, uns16 y, uns16 wd, uns16 ht, const char* text);
	
	//Not supported for UART/Serial communication
	void 	text_rightJustified(uns8 color, uns16 x, uns16 y, char* text);
	void 	text_centerJustified(uns8 color, uns16 x, uns16 y, char* text);
	void 	text_vertical(uns8 color, uns16 x, uns16 y, char* text);
	void 	text_column(uns8 color, uns16 x, uns16 y, char* text);
	void 	text_vertColumn(uns8 color, uns16 x, uns16 y, char* text);
	
	//Not yet supported
	//void 	scrollVertical(uns8 color, uns16 x, uns16 y, uns16 ht, uns16 wd, char* text);
	//void 	scrollColumn(uns8 color, uns16 x, uns16 y, uns16 ht, uns16 wd, char* text);
	//void 	scrollVerticalColumn(uns8 color, uns16 x, uns16 y, uns16 ht, uns16 wd, char* text);
};

#endif
