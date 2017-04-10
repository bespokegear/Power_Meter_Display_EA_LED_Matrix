#pragma once

#include <stdint.h>
#include "DisplayMode.h"

#define RIDCP_BUFFER_LEN    24
#define RIDCP_MAX_IDS       8

enum RIDisplayCommand {
    None,
    Power,
    VoltageAndCurrent,
    MaxGraphPower,
    Timer,
    Countdown,
    Winner,
    Clear,
    String
};

typedef void (*Callback)(const char*);

class RIDisplayCommandMapper {
public:
    RIDisplayCommandMapper();
    void add(const char* id, RIDisplayCommand cmd, uint8_t maxData, DisplayMode* mode);
    RIDisplayCommand getCmd(const char* id);
    uint8_t getMaxData(RIDisplayCommand cmd);
    DisplayMode* getMode(RIDisplayCommand cmd);

private:
    uint8_t _count;
    char _id[RIDCP_MAX_IDS][3];
    RIDisplayCommand _cmd[RIDCP_MAX_IDS];
    uint8_t _maxData[RIDCP_MAX_IDS];
    DisplayMode* _modes[RIDCP_MAX_IDS];

};

class RIDisplayCommandParser {
public:
    RIDisplayCommandParser();
    void begin();
    void update();

    bool valid();
    bool complete();
    void reset();
    void setID(uint8_t id0, uint8_t id1);

private:
    RIDisplayCommandMapper _mapper;
    uint8_t _id0;
    uint8_t _id1;
    char _buf[RIDCP_BUFFER_LEN];
    uint8_t _ptr;
    char _dataOffset;
    unsigned long _last;
    unsigned long _timeout;
    RIDisplayCommand _cmd;

};

extern RIDisplayCommandParser Parser;

