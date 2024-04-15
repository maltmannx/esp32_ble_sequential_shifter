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

        int currentButtonState[] = {digitalRead(BUTTONPIN1), digitalRead(BUTTONPIN2)};
        int previousButtonState[] = {previousButton1State, previousButton2State};
        int BUTTON[] = {BUTTON_1, BUTTON_2};
        int size = sizeof(currentButtonState) / sizeof(currentButtonState[0]);
        for (int i = 0; i < size; i++)
        {
            if (currentButtonState[i] != previousButtonState[i])
            {
                if (currentButtonState[i] == LOW)
                {
                    bleGamepad.press(BUTTON[i]);
                }
                else
                {
                    bleGamepad.release(BUTTON[i]);
                }
            }
        }
        
        previousButtonState[0] = currentButtonState[0];
        previousButtonState[1] = currentButtonState[1];
        bleGamepad.sendReport();
    }
}