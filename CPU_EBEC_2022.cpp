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
			if (millis() - reset_timer > 3000) { // ���� ������ �� ��� ������������ � ������ 3 �������
				// �������� ���������� �����
				for (byte i = 0; i < coin_amount; i++) {
					coin_quantity[i] = 0;
					EEPROM.writeInt(coin_amount * 4 + i * 2, 0);
				}
				lcd.clear();
				lcd.setCursor(0, 0);
				lcd.print("Memory cleared");
				delay(100);
			}
			if (digitalRead(calibration_button)) { // ���� ��������� ������, ������� � ����������
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
			lcd.print(coin_value[i]); // ���������� ���� ������, ������ ������� �����������
			lcd.setCursor(13, 1);
			lcd.print(currency);      // ���������� ������
			last_sens_signal = idle_signal;
			while (1) {
				sens_signal = analogRead(ir_signal_pin);   // ������� ������
				if (sens_signal > last_sens_signal)
					last_sens_signal = sens_signal; // ���� ������� �������� ������ �����������
				if (sens_signal - idle_signal > idle_delta && last_sens_signal>(sens_signal+idle_delta)) {
					coin_flag = true; // ���� �������� ����� ����� �� "�������", ������� ��� ������ �������
					delay(50);
				}
				if (coin_flag && (abs(sens_signal - idle_signal)) < 3) { // ���� ������ ����� �������
					coin_signal[i] = last_sens_signal;
					Serial.println(coin_signal[i]);
					// �������� ������������ �������� � ������
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
		sens_signal = analogRead(ir_signal_pin); // ����� ����� �� ��������, ��� ��� ����������
		if (sens_signal > last_sens_signal)
			last_sens_signal = sens_signal;
		if (sens_signal - idle_signal > 8)
			coin_flag = true;
		if (coin_flag && (abs(sens_signal - idle_signal)) < 2) {
			recogn_flag = false;  // ������ ������, ���� ��� �� ������������
			// � ����� ����� �������� ��� ����������� �������, �������� � last_sens_signal
			// ����� �������� ���������� �� ���������� ��� �����, ����������� � ������
			for (byte i = 0; i < coin_amount; i++) {
				int delta = abs(last_sens_signal - coin_signal[i]); // ��� ����� �������! ���� ���������� (�� ���� �� ������)
				// �������� �������� ����������� ������� � ������ ���������� �� ������
				if (delta < calibration_delta) { // � ��� ��� ���� ��� �������� �������� � ��������, �� ������� ������� ������������
					summ_money += coin_value[i]; // � ����� ���� ���������� ���� ������� (����, ����� ��������� ����� ������� ���������. ��� ������ ������� ������ ���� �����, � ��� �����������
					lcd.setCursor(0, 1);
					lcd.print(summ_money);
					coin_quantity[i]++; // ��� ������������� ������ ������� ���������� ����������
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
