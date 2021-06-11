#pragma once

#include <MutilaDebug.h>
#include <stdint.h>
#include <EEPROM.h>

template <class T>
class PersistentSetting {
public:
    PersistentSetting(uint16_t eepromOffset, T min, T max, T def, T inc) : 
        _eepromOffset(eepromOffset),
        _min(min),
        _max(max),
        _def(def),
        _inc(inc)
    {
        this->load();
    }

    // Load value from EEPROM - return the loaded value
    T load() {
        uint8_t* ptr = (uint8_t*)(&_value);
        for (uint8_t i=0; i<sizeof(T); i++) {
            ptr[i] = EEPROM.read(_eepromOffset+i);
        }
        if (_value < _min || _value > _max) {
            _value = _def;
        }
        return _value;
    }

    // save the setting to EEPROM - only updates EEPROM if value
    // in member differs from value in EEPROM (courtesy of the 
    // EEPROM.update() function).
    void save()
    {
        uint8_t* ptr = (uint8_t*)(&_value);
        DB(F("EEPROM write at "));
        DB(_eepromOffset);
        for (uint8_t i=0; i<sizeof(T); i++) {
            DB('+');
            DB(i);
            EEPROM.update(_eepromOffset+i, ptr[i]);
        }
        DBLN('.');
    }

    T get() { return _value; }
    bool set(T v) { if (v >= _min && v<=_max) { _value = v; return true; } else { return false; } }

    virtual void increment() {
        _value += _inc;
        if (_value > _max) {
            _value = _min;
        }
    }

    size_t size() { return sizeof(T); }

private:
    uint16_t _eepromOffset;

protected:
    T _value;
    T _min;
    T _max;
    T _def;
    T _inc;
};
