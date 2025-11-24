#include <Arduino.h>

#define LED_PIN 5 // D1 = GPIO5
#define BLINK_DELAY_MS 1000

void setup()
{
    Serial.begin(115200);
    while(!Serial);  // Wait for Serial
    Serial.println("setup() entered");
    pinMode(LED_PIN, OUTPUT);
    Serial.printf("LED_PIN = %d\n", LED_PIN);
}

void loop()
{
    Serial.println("LOOP: HIGH");
    digitalWrite(LED_PIN, HIGH); // LED on
    delay(BLINK_DELAY_MS);
    Serial.println("LOOP: LOW");
    digitalWrite(LED_PIN, LOW); // LED off
    delay(BLINK_DELAY_MS);
}