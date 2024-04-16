#include <Arduino.h>
#include <BleGamepad.h> // https://github.com/lemmingDev/ESP32-BLE-Gamepad

#define BUTTONPIN1 5
#define BUTTONPIN2 21
// Pin button is attached to

#define LONGPRESS 500
// any pressing longer than 500ms will be triggered as long press

#define LED 2
// on-board LED(optional)

// add on-press buttons here
int button[] = {BUTTON_1, BUTTON_2};
// add on-longpress buttons here
int longPressButton[] = {BUTTON_3, BUTTON_4};

// bluetooth ssid can be modified, defalut:
// BleGamepad(std::string deviceName = "ESP32 BLE Gamepad", std::string deviceManufacturer = "Espressif", uint8_t batteryLevel = 100);
BleGamepad bleGamepad;

int previousButton1State = HIGH;
int previousButton2State = HIGH;
bool previousConnectionState = false;

unsigned long buttonPressStartTime = 0;
bool longPressTriggered = false;

// use on-board LED as connection indicator
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
    // connection indicate
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
                    if (buttonPressDuration < LONGPRESS && !longPressTriggered)
                    {
                        bleGamepad.press(button[i]);
                    }
                    else if (buttonPressDuration > LONGPRESS)
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