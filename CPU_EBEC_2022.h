// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _CPU_EBEC_2022_H_
#define _CPU_EBEC_2022_H_
#include "Arduino.h"
//add your includes for the project CPU_EBEC_2022 here

#include <EEPROMex.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//end of add your includes here

//add your function definitions for the project CPU_EBEC_2022 here

//add your constants and globals for the project CPU_EBEC_2022 here
const uint8_t reset_sum_button = 3;
const uint8_t calibration_button = 2;
const uint8_t ir_led_pin = 11;
const uint8_t ir_pullup_pin = A3;
const uint8_t ir_signal_pin = A0;
const String sponsors =
		" BEST . GENESIS . EPAM . HUAWEI . MELEXIS . SOFTSERVE . SOMBRA . SQUAD . AJAX . UI.COM . ITERNAL .";
const String team = "Not a Team";
const String currency = "UAH";
const uint8_t coin_amount = 4;   // число монет, которые нужно распознать
const float coin_value[coin_amount] = { 0.1, 1.0, 0.5, 1.0 };
const int temp[4]= {288,1023,1013,573};
// стоимость монет 10.0,
const unsigned int idle_delta = 3;
const unsigned int calibration_delta = 12;
int coin_signal[coin_amount]; // тут хранится значение сигнала для каждого размера монет
int coin_quantity[coin_amount]; // количество монет
int idle_signal;               // храним уровень пустого сигнала
unsigned long reset_timer;      // таймер
float summ_money = 0;           // сумма монет в копилке
boolean recogn_flag;   // флажки
int sens_signal, last_sens_signal;
boolean coin_flag;
LiquidCrystal_I2C lcd(0x27, 16, 2);
//Do not add code below this line
#endif /* _CPU_EBEC_2022_H_ */
