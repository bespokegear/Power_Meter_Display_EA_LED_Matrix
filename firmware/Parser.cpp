#include <string.h>
#include <Arduino.h>
#include <MutilaDebug.h>
#include "Matrix.h"
#include "Parser.h"
#include "Config.h"
#include "SmallTextMode.h"
#include "TextMode.h"
#include "TextControlMode.h"
#include "ClearMode.h"
#include "CountdownMode.h"
#include "TimerMode.h"
#include "VoltageAndCurrentMode.h"
#include "WinnerMode.h"
#include "PowerMode.h"
#include "SetMaxPowerMode.h"
#include "SetIDMode.h"
#include "DumpConfigMode.h"
#include "Settings.h"

RIDisplayCommandParser Parser;

RIDisplayCommandMapper::RIDisplayCommandMapper() : _count(0)
{
}

void RIDisplayCommandMapper::add(const char* id, RIDisplayCommand cmd, uint8_t maxData, DisplayMode* mode)
{
    if (_count < RIDCP_MAX_IDS) {
        DB(F("RIDisplayCommandMapper::add "));
        DBLN(id);
        strncpy(_id[_count], id, 2);
        _cmd[_count] = cmd;
        _maxData[_count] = maxData;
        _modes[_count] = mode;
        _count++;
    } else {
        DBLN(F("RIDisplayCommandMapper::add FULL"));
    }
}

RIDisplayCommand RIDisplayCommandMapper::getCmd(const char* id)
{
    for (uint8_t i=0; i<_count; i++) {
        if (strncmp(id, _id[i], 2) == 0) {
            return _cmd[i];
        }
    }
    return None;
}

uint8_t RIDisplayCommandMapper::getMaxData(RIDisplayCommand cmd)
{
    for (uint8_t i=0; i<_count; i++) {
        if (cmd == _cmd[i]) {
            return _maxData[i];
        }
    }
    return 0;
}

DisplayMode* RIDisplayCommandMapper::getMode(RIDisplayCommand cmd)
{
    for (uint8_t i=0; i<_count; i++) {
        if (cmd == _cmd[i]) {
            return _modes[i];
        }
    }
    return NULL;
}

RIDisplayCommandParser::RIDisplayCommandParser()
{
}

void RIDisplayCommandParser::begin()
{
    // Do this here rather than the constructor so that
    // we can expect Serial to be initialized for debugging
    // output...
    setID(DisplayID0.get(), DisplayID1.get());
    _mapper.add("CD", Countdown,            1,  &CountdownMode);
    _mapper.add("CL", Clear,                0,  &ClearMode);
    _mapper.add("DC", DumpConfig,           0,  &DumpConfigMode);
    _mapper.add("ID", SetID,                2,  &SetIDMode);
    _mapper.add("MP", MaxGraphPower,        4,  &SetMaxPowerMode);
    _mapper.add("P",  Power,                5,  &PowerMode);
    _mapper.add("ST", String,               RIDCP_BUFFER_LEN-6, &SmallTextMode);
    _mapper.add("TC", TextControl,          2,  &TextControlMode);
    _mapper.add("TE", Text,                 RIDCP_BUFFER_LEN-6, &TextMode);
    _mapper.add("TI", Timer,                4,  &TimerMode);
    _mapper.add("V",  VoltageAndCurrent,    8,  &VoltageAndCurrentMode);
    _mapper.add("WN", Winner,               1,  &WinnerMode);

    // Make sure the buffer is reset
    reset();
}

void RIDisplayCommandParser::update()
{
    if (millis() > _timeout && _ptr > 0) {
        DBLN(F("TIMEOUT"));
        reset();
    }

    bool fire = false;

    if (Serial.available() > 0) {
        int c = Serial.read();
        if (c == '\n' || c == '\r') {
            fire = true;
        } else {
            _buf[_ptr] = c;
            DB('+');
            DB(_buf[_ptr]);
            switch(_ptr++) {
            case 0:
                if (_buf[0] != 'a') {
                    DBLN(F("RST HDR0"));
                    reset();
                } else {
                    _last = millis();
                    _timeout = _last + CMD_TIMEOUT_MS;
                }
                break;
            case 1:
                if (_buf[1] != _id0 && _id0 != '*') {
                    DBLN(F("RST HDR1"));
                    reset();
                }
                break;
            case 2:
                if (_buf[2] != _id1 && _id1 != '*') {
                    DBLN(F("RST HDR2"));
                    reset();
                }
                break;
            case 3:
                _cmd = _mapper.getCmd(_buf+3);
                if (_cmd != None) {
                    DB(F("1-byte cmd=#"));
                    DBLN(_cmd);
                    _dataOffset = 4;
                }
                break;
            case 4:
                if (_cmd == None) {
                    _cmd = _mapper.getCmd(_buf+3);
                    if (_cmd != None) {
                        DB(F("2-byte cmd=#"));
                        DBLN(_cmd);
                        _dataOffset = 5;
                    } else {
                        DB(F("RST CMD"));
                        reset();
                    }
                }
                break;
            default:
                break;
            }
            DB(F("_buf now="));
            DBLN(_buf);
        }

        // Double check we didn't exceed the buffer size.  Shouldn't ever
        // trigger cos we have set the mapper data lengths to be small 
        // enough that we never do this, but just in case...
        if (_ptr >= RIDCP_BUFFER_LEN) {
            fire = true;
        }

        // Test to see if we've had enough data for the current command...
        if (_cmd != None) {
            DB(F("we have command id=#"));
            DB(_cmd);
            DB(F(" data we have: "));
            DB(_ptr-_dataOffset);
            DB('/');
            DBLN(_mapper.getMaxData(_cmd));
            if (_ptr-_dataOffset >= _mapper.getMaxData(_cmd)) {
                fire = true;
            }
        }

        if (fire) {
            DB(F("READY! AIM! "));
            DisplayMode* mode = _mapper.getMode(_cmd);
            if (mode != NULL) {
                DBLN(F("FIRE!"));
                Matrix.startMode(mode, _buf+_dataOffset);
            } else {
                DBLN(F("[click]"));
            }
            reset();
        }
    }
}

bool RIDisplayCommandParser::valid()
{
    return false;
}

bool RIDisplayCommandParser::complete()
{
    return false;
}

void RIDisplayCommandParser::reset()
{
    _last = 0;
    memset(_buf, 0, RIDCP_BUFFER_LEN);
    _ptr = 0;
    _dataOffset = 0;
    _cmd = None;
    _timeout = 0;
}

void RIDisplayCommandParser::setID(uint8_t id0, uint8_t id1)
{
    _id0 = id0;
    _id1 = id1;
    if ((_id0 < 'A' || id0 > 'Z') && id0 != '*') _id0 = '*';
    if ((_id1 < 'A' || id1 > 'Z') && id1 != '*') _id1 = '*';
}

void RIDisplayCommandParser::dumpID()
{
    Serial.print(F("Display ID is: "));
    Serial.print((char)_id0);
    Serial.println((char)_id1);
}


