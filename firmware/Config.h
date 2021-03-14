#pragma once

#include <Arduino.h>

#define HEARTBEAT_PIN           13
#define SW_SERIAL_RX            12
#define SW_SERIAL_TX            11

#define CMD_TIMEOUT_MS          10000
#define CMD_BUF_LEN             24
#define GRAPH_DATA_ITEMS        64
#define GRAPH_PLOT_MS           1000
#define VALUE_PLOT_MS           2000
#define GRAPH_MAX_WATT          400

#define MATRIX_SIZE_X           64
#define MATRIX_SIZE_Y           16
#define LED_PANELS              2

#define SERIAL_BAUD             9600
#define DEBUG                   true
