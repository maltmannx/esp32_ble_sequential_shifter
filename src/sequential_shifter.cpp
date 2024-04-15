#include <Arduino.h>
#include <BleGamepad.h> // https://github.com/lemmingDev/ESP32-BLE-Gamepad

#define BUTTONPIN1 5
#define BUTTONPIN2 21
#define LED 2
// Pin button is attached to

BleGamepad bleGamepad;

int previousButton1State = HIGH;
int previousButton2State = HIGH;
bool previousConnectionState = false;

unsigned long buttonPressStartTime = 0;
bool longPressTriggered = false;

void blink(int time, int on_time, int off_time)
{
    for (size_t i = 0; i < time; i++)
    {
        digitalWrite(LED, HIGH);
        delay(on_time);
        digitalWrite(LED, LOW);
        delay(off_time);
    }
}

void setup()
{
    pinMode(BUTTONPIN1, INPUT_PULLUP);
    pinMode(BUTTONPIN2, INPUT_PULLUP);
    bleGamepad.begin();
}

void loop()
{

    if (previousConnectionState != bleGamepad.isConnected())
    {
        blink(5, 50, 50);
    }
    else
    {
        blink(1, 500, 0);
    }

    if (bleGamepad.isConnected())
    {

        int currentButtonState[] = {digitalRead(BUTTONPIN1), digitalRead(BUTTONPIN2)};
        int previousButtonState[] = {previousButton1State, previousButton2State};

        // add on-press buttons here
        int button[] = {BUTTON_1, BUTTON_2};
        // add on-longpress buttons here
        int longPressButton[] = {BUTTON_3, BUTTON_4};

        int size = sizeof(currentButtonState) / sizeof(currentButtonState[0]);
        for (int i = 0; i < size; i++)
        {
            if (currentButtonState[i] != previousButtonState[i])
            {
                if (currentButtonState[i] == LOW)
                {
                    buttonPressStartTime = millis();
                }
                else
                {
                    unsigned long buttonPressDuration = millis() - buttonPressStartTime;
                    if (buttonPressDuration < 500 && !longPressTriggered)
                    {
                        bleGamepad.press(button[i]);
                    }
                    else if (buttonPressDuration > 500)
                    {
                        bleGamepad.press(longPressButton[i]);
                        longPressTriggered = true;
                    }
                    buttonPressStartTime = 0;
                }
            }
        }

        previousButtonState[0] = currentButtonState[0];
        previousButtonState[1] = currentButtonState[1];
        bleGamepad.sendReport();
        delay(20);
    }
}