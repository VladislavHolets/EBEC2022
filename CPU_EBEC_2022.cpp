// Do not remove the include below
#include "CPU_EBEC_2022.h"

//The setup function is called once at startup of the sketch
void setup() {
	pinMode(reset_sum_button, INPUT_PULLUP);
	pinMode(calibration_button, INPUT_PULLUP);
	pinMode(ir_led_pin, OUTPUT);
	pinMode(ir_pullup_pin, OUTPUT);
	digitalWrite(ir_led_pin, 1);
	digitalWrite(ir_pullup_pin, 1);
	Serial.begin(9600);
	delay(1000);
	idle_signal = analogRead(ir_signal_pin);

	lcd.init();
	lcd.backlight();

	if (!digitalRead(calibration_button)) {
		lcd.clear();
		lcd.setCursor(3, 0);
		lcd.print("Service");
		delay(500);
		reset_timer = millis();
		while (1) {
			if (millis() - reset_timer > 3000) { // если кнопка всё ещё удерживается и прошло 3 секунды
				// очистить количество монет
				for (byte i = 0; i < coin_amount; i++) {
					coin_quantity[i] = 0;
					EEPROM.writeInt(coin_amount * 4 + i * 2, 0);
				}
				lcd.clear();
				lcd.setCursor(0, 0);
				lcd.print("Memory cleared");
				delay(100);
			}
			if (digitalRead(calibration_button)) { // если отпустили кнопку, перейти к калибровке
				lcd.clear();
				lcd.setCursor(0, 0);
				lcd.print("Calibration");
				break;
			}
		}
		//while (1) {

		idle_signal = analogRead(ir_signal_pin);
		for (byte i = 0; i < coin_amount; i++) {
			idle_signal = analogRead(ir_signal_pin);
			lcd.setCursor(0, 1);
			lcd.print("      ");
			lcd.setCursor(0, 1);
			lcd.print(coin_value[i]); // отобразить цену монеты, размер которой калибруется
			lcd.setCursor(13, 1);
			lcd.print(currency);      // отобразить валюту
			last_sens_signal = idle_signal;
			while (1) {
				sens_signal = analogRead(ir_signal_pin);   // считать датчик
				if (sens_signal > last_sens_signal)
					last_sens_signal = sens_signal; // если текущее значение больше предыдущего
				if (sens_signal - idle_signal > idle_delta && last_sens_signal>(sens_signal+idle_delta)) {
					coin_flag = true; // если значение упало почти до "пустого", считать что монета улетела
					delay(50);
				}
				if (coin_flag && (abs(sens_signal - idle_signal)) < 3) { // если монета точно улетела
					coin_signal[i] = last_sens_signal;
					Serial.println(coin_signal[i]);
					// записать максимальное значение в память
					EEPROM.writeInt(i * 2, coin_signal[i]);
					delay(500);
					coin_flag = false;
					break;
				}
			}
		}
//			break;
//		}
	}
	for (byte i = 0; i < coin_amount; i++) {
		coin_signal[i] = EEPROM.readInt(i * 2);
		coin_signal[i] = temp[i];
		Serial.println(coin_signal[i]);
		coin_quantity[i] = EEPROM.readInt(coin_amount * 4 + i * 2);
		summ_money += coin_quantity[i] * coin_value[i];
	}

	delay(500);
	lcd.init();
}

// The loop function is called in an endless loop
void loop() {

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Collected");
	lcd.setCursor(0, 1);
	lcd.print(summ_money);
	lcd.setCursor(13, 1);
	lcd.print(currency);
//	while (1) {
	idle_signal = analogRead(ir_signal_pin);
//	Serial.println(idle_signal);
//	delay(50);
//	}
	last_sens_signal = idle_signal;
	while (1) {
		sens_signal = analogRead(ir_signal_pin); // далее такой же алгоритм, как при калибровке
		if (sens_signal > last_sens_signal)
			last_sens_signal = sens_signal;
		if (sens_signal - idle_signal > 8)
			coin_flag = true;
		if (coin_flag && (abs(sens_signal - idle_signal)) < 2) {
			recogn_flag = false;  // флажок ошибки, пока что не используется
			// в общем нашли максимум для пролетевшей монетки, записали в last_sens_signal
			// далее начинаем сравнивать со значениями для монет, хранящимися в памяти
			for (byte i = 0; i < coin_amount; i++) {
				int delta = abs(last_sens_signal - coin_signal[i]); // вот самое главное! ищем АБСОЛЮТНОЕ (то бишь по модулю)
				// значение разности полученного сигнала с нашими значениями из памяти
				if (delta < calibration_delta) { // и вот тут если эта разность попадает в диапазон, то считаем монетку распознанной
					summ_money += coin_value[i]; // к сумме тупо прибавляем цену монетки (дада, сумма считается двумя разными способами. При старте системы суммой всех монет, а тут прибавление
					lcd.setCursor(0, 1);
					lcd.print(summ_money);
					coin_quantity[i]++; // для распознанного номера монетки прибавляем количество
					EEPROM.writeInt(coin_amount * 4 + i * 2, coin_quantity[i]);
					recogn_flag = true;
					break;
				}
			}
			if (recogn_flag == false) {
				lcd.clear();
				lcd.setCursor(0, 1);
				lcd.print("ERROR:not a coin");
				delay(2000);
			}
			coin_flag = false;
			break;
		}
	}
//Add your repeated code here
}
