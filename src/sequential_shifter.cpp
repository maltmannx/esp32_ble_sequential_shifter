#include <Arduino.h>
#include <BleGamepad.h> // https://github.com/lemmingDev/ESP32-BLE-Gamepad

// useage: BleGamepad(std::string deviceName = "ESP32 BLE Gamepad", std::string deviceManufacturer = "Espressif", uint8_t batteryLevel = 100);
BleGamepad bleGamepad("ESP32 BLE Shifter", "Espressif", 100);

#define numOfButtons 2
#define LED_GPIO 2 // change it to your (on-board)LED pin

byte previousButtonStates[numOfButtons];
byte currentButtonStates[numOfButtons];
byte shiftButtonPins[numOfButtons] = {25, 34};
byte shiftPhysicalButtons[numOfButtons] = {1, 2};

void blink_led(int time)
{
    for (int index = 0; index <= time; index++)
    {
        digitalWrite(LED_GPIO, HIGH);
        delay(50);
        digitalWrite(LED_GPIO, LOW);
        delay(50);
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

                    switch (currentIndex)
                    {
                    case 0:
                        blink_led(2);
                        break;

                    default:
                        blink_led(1);
                        break;
                    }
                }
                else
                {
                    bleGamepad.release(shiftPhysicalButtons[currentIndex]);
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