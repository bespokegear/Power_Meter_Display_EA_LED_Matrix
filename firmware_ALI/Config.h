#pragma once

#include <Arduino.h>

#define HEARTBEAT_PIN           13
#define SW_SERIAL_RX            A5
#define SW_SERIAL_TX            A4

// Button feel settings
#define DEBOUNCED_BUTTON_THRESHOLD      5
#define DEBOUNCED_BUTTON_DELAY          5
#define DEBOUNCED_BUTTON_HELD_MS        300
#define DEBOUNCED_BUTTON_RPT_INITIAL_MS 500
#define DEBOUNCED_BUTTON_RPT_MS         300

// Button parameters
#define RESET_BUTTON_PIN                2

#define CMD_TIMEOUT_MS          5000
#define CMD_BUF_LEN             5

#define GRAPH_DATA_ITEMS        64
#define GRAPH_DATA_ITEMS_SM     32
#define GRAPH_PLOT_MS           1000
#define VALUE_PLOT_MS           2000
#define GRAPH_MAX_WATT          400

#define MATRIX_SIZE_X           64
#define MATRIX_SIZE_Y           16
#define LED_PANELS              2

#define SERIAL_BAUD             115200
#define DEBUG                   true


//#define SERIAL_BAUD             9600
//#define DEBUG                   true