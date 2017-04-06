#include <string.h>
#include <Arduino.h>
#include <MutilaDebug.h>
#include "Parser.h"
#include "Config.h"

RIDisplayCommandParser Parser;

RIDisplayCommandMapper::RIDisplayCommandMapper() : _count(0)
{
}

void RIDisplayCommandMapper::add(const char* id, RIDisplayCommand cmd, uint8_t maxData)
{
    if (_count < RIDCP_MAX_IDS) {
        DB(F("RIDisplayCommandMapper::add "));
        DB(id);
        DB(F(" -> "));
        DBLN(cmd);
        strncpy(_id[_count], id, 2);
        _cmd[_count] = cmd;
        _maxData[_count] = maxData;
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

RIDisplayCommandParser::RIDisplayCommandParser()
{
}

void RIDisplayCommandParser::begin()
{
    // Do this here rather than the constructor so that
    // we can expect Serial to be initialized for debugging
    // output...
    _mapper.add("P", Power, 4);
    _mapper.add("V", VoltageAndCurrent, 8);
    _mapper.add("TI", Timer, 4);
    _mapper.add("CL", Clear, 0);
    _mapper.add("MP", MaxGraphPower, 4);
    _mapper.add("ST", String, 12);
    _mapper.add("WN", Winner, 1);
    _mapper.add("CD", Countdown, 1);

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
                if (_buf[1] != 'A') {
                    DBLN(F("RST HDR1"));
                    reset();
                }
                break;
            case 2:
                if (_buf[2] != 'A') {
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
            DB("FIRE! data=");
            DBLN(_buf+_dataOffset);
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


