#include <Arduino.h>
#include <BleGamepad.h> // https://github.com/lemmingDev/ESP32-BLE-Gamepad

#define BUTTONPIN1 5
#define BUTTONPIN2 21
 // Pin button is attached to


BleGamepad bleGamepad;

int previousButton1State = HIGH;
int previousButton2State = HIGH;

void setup()
{
    pinMode(BUTTONPIN1, INPUT_PULLUP);
    pinMode(BUTTONPIN2, INPUT_PULLUP);
    bleGamepad.begin();
}

void loop()
{
    if (bleGamepad.isConnected())
    {

        int currentButton1State = digitalRead(BUTTONPIN1);
        int currentButton2State = digitalRead(BUTTONPIN2);

        if (currentButton1State != previousButton1State)
        {
            if (currentButton1State == LOW)
            {
                bleGamepad.press(BUTTON_1);
            }
            else
            {
                bleGamepad.release(BUTTON_1);
            }
        }
                if (currentButton2State != previousButton2State)
        {
            if (currentButton2State == LOW)
            {
                bleGamepad.press(BUTTON_2);
            }
            else
            {
                bleGamepad.release(BUTTON_2);
            }
        }
        previousButton1State = currentButton1State;
        previousButton2State = currentButton2State;
        bleGamepad.sendReport();
    }
}