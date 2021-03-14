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

/*	
	PLT1001 LED Matrix display driver Arduino library
	Written originally by Embedded Adventures
	v2.0 
*/

#include <MutilaDebug.h>
#include "PLT1001.h"

PLT1001::PLT1001() {
	_currentFont = 1;
}

void PLT1001::begin(SoftwareSerial *ss, unsigned long baud) {
	_mySerial = ss;
    _mySerial->begin(baud);
    delay(PLT1001_DELAY*10);
}

void PLT1001::title() {
    _mySerial->print("title\r");
	//DBLN("title");
}

void PLT1001::paint() {
	delay(PLT1001_DELAY);
    _mySerial->print("paint\r");
}

void PLT1001::clear() {
	delay(PLT1001_DELAY*5);
    _mySerial->print("clear\r");
}

void PLT1001::panels(uns8 _num_panels) {
  delay(PLT1001_DELAY);
    _mySerial->print("panel ");
    _mySerial->print(_num_panels);
    _mySerial->print("\r");
}

void PLT1001::command(uns8 i2c_cmd, String s, uns8 param) {
    s += (String)param;
    s += "\r";
    _mySerial->print(s);
}

//Circles, pixel (when param_rad = -1)
void PLT1001::command(uns8 i2c_cmd, String s, uns8 param_clr, uns16 param_x, uns16 param_y, int param_rad) {
    s += " ";
    s += (String)(param_clr & 0x0F) + " ";
    s += (String)param_x + " ";
    s += (String)param_y;
    if (param_rad > 0)
        s += " " + (String)param_rad;
    s += "\r";
    _mySerial->print(s);
}

//Rectangles or lines
void PLT1001::command(uns8 i2c_cmd, String s, uns8 param_clr, uns16 param_x1, uns16 param_y1, uns16 param_x2, uns16 param_y2) {
    s += (String)param_clr + " ";
    s += (String)param_x1 + " ";
    s += (String)param_y1 + " ";
    s += (String)param_x2 + " ";
    s += (String)param_y2 + "\r";
    _mySerial->print(s);
    //DBLN(s);
}

//For text
void PLT1001::command(uns8 i2c_cmd, String s, uns8 param_clr, uns8 param_x, uns8 param_y, char* text) {
    s += (String)(param_clr & 0x0F) + " ";
    s += (String)param_x + " ";
    s += (String)param_y + " ";
    s += text;
    s += "\r";
    _mySerial->print(s);
}

//For text
void PLT1001::command(uns8 i2c_cmd, String s, uns8 param_clr, uns8 param_x, uns8 param_y, const char* text) {
    s += (String)(param_clr & 0x0F) + " ";
    s += (String)param_x + " ";
    s += (String)param_y + " \"";
    s += text;
    s += "\"\r";
    _mySerial->print(s);
}

//Scroll commands
void PLT1001::command(uns8 i2c_cmd, String s, uns8 param_clr, uns16 param_x1, uns16 param_y1, uns16 param_x2, uns16 param_y2, const char* text) {
    s += (String)param_clr + " ";
    s += (String)param_x1 + " ";
    s += (String)param_y1 + " ";
    s += (String)param_x2 + " ";
    s += (String)param_y2 + " ";
    s += text;
    s += "\r";
    _mySerial->print(s);
}

void PLT1001::command(uns8 i2c_cmd, String s, uns8 param_clr, uns16 param_x1, uns16 param_y1, uns16 param_x2, uns16 param_y2, char* text) {
    s += (String)param_clr + " ";
    s += (String)param_x1 + " ";
    s += (String)param_y1 + " ";
    s += (String)param_x2 + " ";
    s += (String)param_y2 + " ";
    s += text;
    s += "\r";
    _mySerial->print(s);
}

void PLT1001::enableActiveHigh(bool en) {
	if (en) {
		command(CMD_ENABLEACTIVE, 1);
	}
	else {
		command(CMD_ENABLEACTIVE, 0);
	}
	delay(PLT1001_DELAY);
}

void PLT1001::setFont(uns8 font) {
	if (font <= 7) {
		command(CMD_FONT, font);
		_currentFont = font;
	}	
	delay(PLT1001_DELAY);
}

void PLT1001::setScrollspeed(uns8 speed) {
	command(CMD_SCROLLSPEED, speed);
}

void PLT1001::pixel(uns8 color, uns16 x, uns16 y) {
	command(CMD_PIXEL, color, x, y);
	delay(PLT1001_DELAY*2);
}

void PLT1001::line(uns8 color, uns16 x1, uns16 y1, uns16 x2, uns16 y2) {
	command(CMD_LINE, color & 0x0F, x1, y1, x2, y2);
	delay(PLT1001_DELAY*2);
}

void PLT1001::rectangle(uns8 color, uns16 x, uns16 y, uns16 wd, uns16 ht) {
	color = (color & 0x0F) | 0x10;
	command(CMD_RECT, color, x, y, wd, ht);
	delay(PLT1001_DELAY*10);
}

void PLT1001::circle(uns8 color, uns16 x, uns16 y, uns8 radius) {
	color = (color & 0x0F) | 0x10;
	command(CMD_CIRCLE, color, x, y, radius);
	delay(PLT1001_DELAY*5);
}

void PLT1001::filledCircle(uns8 color, uns16 x, uns16 y, uns8 radius) {
	color = (color & 0x0F) | 0x20;
	command(CMD_CIRCLE + "f", color, x, y, radius);
	delay(PLT1001_DELAY*5);
}

void PLT1001::circle2(uns8 color, uns16 x, uns16 y, uns8 radius) {
	color = (color & 0x0F) | 0x30;
	command(CMD_CIRCLE + "2", color, x, y, radius);
	delay(PLT1001_DELAY*5);
}

void PLT1001::filledCircle2(uns8 color, uns16 x, uns16 y, uns8 radius) {
	color = (color & 0x0F) | 0x40;
	command(CMD_CIRCLE, color, x, y, radius);
	delay(PLT1001_DELAY*5);
}

void PLT1001::text(uns8 color, uns16 x, uns16 y, String text) {
	color |= 0x10;
	const char* cText = text.c_str();
	command(CMD_TEXT, color, x, y, cText);
	delay(PLT1001_DELAY*10);
}

void PLT1001::text(uns8 color, uns16 x, uns16 y, const char* text) {
	color |= 0x10;
	command(CMD_TEXT, color, x, y, text);
	delay(PLT1001_DELAY*10);
}

void PLT1001::text(uns8 color, uns16 x, uns16 y, uns8 text) {
	color |= 0x10;
	command(CMD_TEXT, color, x, y, text);
	delay(PLT1001_DELAY*10);
}

void PLT1001::text_rightJustified(uns8 color, uns16 x, uns16 y, char* text) {
	color |= 0x20;
	command(CMD_TEXT, color, x, y, text);
	delay(PLT1001_DELAY*10);
}

void PLT1001::text_centerJustified(uns8 color, uns16 x, uns16 y, char* text) {
	color |= 0x30;
	command(CMD_TEXT, color, x, y, text);
	delay(PLT1001_DELAY*10);
}

//Text rotated 90 degrees
void PLT1001::text_vertical(uns8 color, uns16 x, uns16 y, char* text) {
	color |= 0x40;
	command(CMD_TEXT, color, x, y, text);
	delay(PLT1001_DELAY*10);
}

void PLT1001::text_column(uns8 color, uns16 x, uns16 y, char* text) {
	color |= 0x50;
	command(CMD_TEXT, color, x, y, text);
	delay(PLT1001_DELAY*12);
}

void PLT1001::text_vertColumn(uns8 color, uns16 x, uns16 y, char* text) {
	color |= 0x60;
	command(CMD_TEXT, color, x, y, text);
	delay(PLT1001_DELAY*12);
}

void PLT1001::scroll(uns8 color, uns16 x, uns16 y, uns16 wd, uns16 ht, char* text) {
	color &= 0x0F;
	command(CMD_SCROLL, color, x, y, wd, ht, text);
	delay(PLT1001_DELAY*12);
}

void PLT1001::scroll(uns8 color, uns16 x, uns16 y, uns16 wd, uns16 ht, const char* text) {
	color &= 0x0F;
	command(CMD_SCROLL, color, x, y, wd, ht, text);
	delay(PLT1001_DELAY*12);
}

void PLT1001::scroll(uns8 color, uns16 x, uns16 y, uns16 wd, uns16 ht, String text) {
	color &= 0x0F;
	const char* cText = text.c_str();
	command(CMD_SCROLL, color, x, y, wd, ht, cText);
	delay(PLT1001_DELAY*12);
}
