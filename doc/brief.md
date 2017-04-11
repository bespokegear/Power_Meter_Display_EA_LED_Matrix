Background
----------

Matrix display unit is usd for pedal powered challenges and other purposes. It contains an Embedded Adventures PLT-1001 connected to an Arduino Nano/328 by software serial on pins 11 & 12. The display responds to commands sent using serial at 115200 baud. Commands are a simple string constructed like this:

    aAACP----

Where:
    * a = The command header character
    * AA = The address of the display which should process this message
    * C[C] = a one or two character command code (see below)
    * P = a zero or more length parameter to the command
    * ---- = padding at the end of the command - this is a legacy feature which used to be used when the commands were a fixed length

Problem
-------

Matrix display code not working very well:
- sometimes commands are skipped (buffer handling)
- code is very messy and difficult to maintain / extend

Task
----

Improve it!
- Handle buffering/validation of commands more reliably
- re-factor code to 
- re-implement / re-factor these modes:
    - Hello
    - Winner
    - Countdown
    - String display
    - Clear
    - Power (in watts)

Stretch Goals
-------------

- Implement legacy display modes
    - Voltage and Current display
    - Max power graph
    - Set / get display ID
- Implement display address handling
    - Handle wildcard address with **

Commands
--------

- ST - string display; parameter is the string to display up to 12 character, e.g. aAASTBananas, displays "Bananas"
- CD - countdown; parameter is a one countdown number, e.g. "aAACD2"
- TI - timer; parameter is a four digit 10ths of seconds number, e.g. "aAATI1234" displays "123.4"
- P - power display in watts; parameter is power in tenths of a watt up to 99999, e.g. "31416", displays "3141.6 W"
- CL - clears the display - does not require a paremeter
- WN - winner; paremeter is the winning team / player number.  If 0, it's a tie. e.g. "aAAWN0" displays "WINNER: TIE!"
- MP - max power graph; see below
- V - voltage and current; parameter: tenths of volt as three digit value, then "I", then four digits of hundreths of an amp, e.g. "aAAV123I4321", displays "12.3V 43.21A"
- ID - set the display ID; parameter is an upper case two letter code, e.g. "AA". If two asteriks are used for the code, the display will respond to commands intended for any ID;

General Info
------------

- Power cable colour coding:
    - Brown: power +ve 10-30V 
    - Blue: power -ve/GND
    - Green & yellow: serial data

Max Power Graph
---------------

The max power graph has a maximum value and plot a bar chart of recent power values, scrolling accross the back of the display, with the maximum value printed in text in the foreground.
