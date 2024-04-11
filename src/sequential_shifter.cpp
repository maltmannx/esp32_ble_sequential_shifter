#include <Arduino.h>
#include <BleGamepad.h> // https://github.com/lemmingDev/ESP32-BLE-Gamepad

// useage: BleGamepad(std::string deviceName = "ESP32 BLE Gamepad", std::string deviceManufacturer = "Espressif", uint8_t batteryLevel = 100);
BleGamepad bleGamepad("ESP32 BLE Shifter", "Espressif", 100);

#define numOfButtons 2
#define numOfLongPressButtons 2
#define LED_GPIO 2 // change it to your (on-board)LED pin

byte previousButtonStates[numOfButtons];
byte currentButtonStates[numOfButtons];

byte shiftButtonPins[numOfButtons] = {25, 34};
byte shiftPhysicalButtons[numOfButtons] = {1, 2};

byte longPressPhysicalButtons[numOfLongPressButtons] = {3, 4};
byte buttonPressStartTime[numOfButtons];

void blink_led(int time, int lit, int unlit)
{
    for (int index = 0; index <= time; index++)
    {
        digitalWrite(LED_GPIO, HIGH);
        delay(lit);
        digitalWrite(LED_GPIO, LOW);
        delay(unlit);
    }
}

void setup()
{

    pinMode(LED_GPIO, OUTPUT);

    for (byte currentPinIndex = 0; currentPinIndex < numOfButtons; currentPinIndex++)
    {
        // shifter buttons
        pinMode(shiftButtonPins[currentPinIndex], INPUT_PULLUP);
        previousButtonStates[currentPinIndex] = HIGH;
        currentButtonStates[currentPinIndex] = HIGH;
    }

    BleGamepadConfiguration bleGamepadConfig;
    bleGamepadConfig.setAutoReport(false);
    bleGamepadConfig.setButtonCount(numOfButtons);
    bleGamepad.begin(&bleGamepadConfig);
    // changing bleGamepadConfig after the begin function has no effect, unless you call the begin function again

    if (bleGamepad.isConnected())
    {
        blink_led(5, 50, 50);
    }
    
}

void loop()
{

    if (bleGamepad.isConnected())
    {
        for (byte currentIndex = 0; currentIndex < numOfButtons; currentIndex++)
        {
            currentButtonStates[currentIndex] = digitalRead(shiftButtonPins[currentIndex]);

            if (currentButtonStates[currentIndex] != previousButtonStates[currentIndex])
            {
                if (currentButtonStates[currentIndex] == LOW)
                {
                    bleGamepad.press(shiftPhysicalButtons[currentIndex]);
                    buttonPressStartTime[currentIndex] = millis();

                    switch (currentIndex)
                    {
                    case 0:
                        blink_led(2, 50, 50);
                        break;

                    case 1:
                        blink_led(1, 50, 50);

                    default:
                        break;
                    }
                }
                else
                {
                    bleGamepad.release(shiftPhysicalButtons[currentIndex]);

                    // long press logic
                    unsigned long buttonPressDuration = millis() - buttonPressStartTime[currentIndex];

                    if (buttonPressDuration >= 800)
                    {
                        switch (currentIndex)
                        {
                        case 0:
                            bleGamepad.press(longPressPhysicalButtons[currentIndex]);
                            blink_led(1, 500, 0);
                            break;
                        case 1:
                            bleGamepad.press(longPressPhysicalButtons[currentIndex]);
                            blink_led(3, 50, 50);
                        default:
                            break;
                        }
                    }
                    bleGamepad.release(longPressPhysicalButtons[currentIndex]);
                }
            }
        }

        if (currentButtonStates != previousButtonStates)
        {
            for (byte currentIndex = 0; currentIndex < numOfButtons; currentIndex++)
            {
                previousButtonStates[currentIndex] = currentButtonStates[currentIndex];
            }

            bleGamepad.sendReport();
        }

        delay(20);
    }
}