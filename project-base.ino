#include <Wire.h>
#include <Arduino.h>
#include "TaskScheduler.h"
#include "ssd1306_i2c.h"
#include "adxl345_i2c.h"
#include "ConsoleController.h"

SSD1306 mDisplay(&Wire);
ADXL345 mAccel(&Wire);
ConsoleView mConsoleView(&mDisplay);
ConsoleController mConsole(&mConsoleView);

Scheduler mScheduler;
Task taskConsole(300, -1, &taskConsoleCallback);
Task t2(10, -1, &t2Callback);

void setup() {
  Serial.begin(9600);
  Wire.begin(2, 0);
  Wire.setClock(400000); 
  mDisplay.init();
  mDisplay.clear();
  mDisplay.display();
  mDisplay.flipScreenVertically();
  mDisplay.setFontScale2x2(false);
  mAccel.init();

  mScheduler.init();
  mScheduler.addTask(taskConsole);
  mScheduler.addTask(t2);
  taskConsole.enable(); 
  t2.enable();

  mConsole.print(KEY_WAKE_UP);
  mConsole.print(KEY_MATRIX_HAS);
  mConsole.print(KEY_FOLLOW);
  mConsole.print("192.168.250.142");
  mConsole.print(KEY_WHITE_RABBIT);
  mConsole.print("OLOLOLS");

}

void loop() {
  mScheduler.execute();
}

void taskConsoleCallback() {
  mConsole.execute();
}
void t2Callback() {
  //mConsole.print(String(mAccel.getAngels().pitch).c_str());

  Serial.print(mAccel.getAngels().pitch*1000);
  Serial.print(":");
  Serial.println(mAccel.getAngels().roll*1000);
}