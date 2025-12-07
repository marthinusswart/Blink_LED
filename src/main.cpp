#include <Arduino.h>
#include <DHT.h>
#include <Ticker.h>
#include "logger.h"

#define GREEN_LED_PIN 5   // D1 = GPIO5
#define YELLOW_LED_PIN 14 // D5 = GPIO14
#define RED_LED_PIN 12    // D6 = GPIO12
#define READ_DELAY_SEC 10 // Read every 10 seconds
#define BLINK_DELAY_MS 2000
#define DHT_PIN 4 // D2 = GPIO4
#define DHTTYPE DHT22
#define BUTTON_PIN 2 // D4 = GPIO2
DHT dht(DHT_PIN, DHTTYPE);

void setIndicator(float temperature);
void resetIndicators();
void sensorReadCallback();

Ticker sensorTimer;
int buttonValue;
volatile bool readSensor = false;

void setup()
{
#ifdef DEBUG
    Serial.begin(9600);
    delay(100);
    Serial.println("setup() entered");
#endif
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    dht.begin();
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    DBG("BUTTON_PIN = %d\n", BUTTON_PIN);
    DBG("GREEN_LED_PIN = %d\n", GREEN_LED_PIN);
    DBG("YELLOW_LED_PIN = %d\n", YELLOW_LED_PIN);
    DBG("RED_LED_PIN = %d\n", RED_LED_PIN);

    sensorTimer.attach(READ_DELAY_SEC, sensorReadCallback); // Every 5s
}

void loop()
{
    // Check button for immediate reading
    static unsigned long lastDebounceTime = 0;
    unsigned long now = millis();
    if (digitalRead(BUTTON_PIN) == LOW && (now - lastDebounceTime > 200))
    {
        DBGL("Button: LOW!");
        lastDebounceTime = now;
        readSensor = true;
    }

    // Small delay
    delay(100);

    if (readSensor)
    {
        readSensor = false;
        float h = dht.readHumidity();
        float t = dht.readTemperature();

        if (isnan(h) || isnan(t))
        {
            DBGL("Failed to read from DHT sensor!");
        }
        else
        {
            DBGL("Humidity: %.1f%%  Temperature: %.1f *C ", h, t);
        }

        setIndicator(t);
        delay(BLINK_DELAY_MS);
        resetIndicators();
    }
}

void setIndicator(float temperature)
{
    DBGL("Setting indicators for temperature: %.1f", temperature);
    if (temperature <= 25.0)
    {
        digitalWrite(GREEN_LED_PIN, HIGH); // LED on
        digitalWrite(YELLOW_LED_PIN, LOW); // LED ff
        digitalWrite(RED_LED_PIN, LOW);    // LED off
    }
    else if (temperature > 25.0 && temperature < 30.0)
    {
        digitalWrite(GREEN_LED_PIN, LOW);   // LED off
        digitalWrite(YELLOW_LED_PIN, HIGH); // LED on
        digitalWrite(RED_LED_PIN, LOW);     // LED off
    }
    else
    {
        digitalWrite(GREEN_LED_PIN, LOW);  // LED off
        digitalWrite(YELLOW_LED_PIN, LOW); // LED off
        digitalWrite(RED_LED_PIN, HIGH);   // LED on
    }
}

void resetIndicators()
{
    digitalWrite(GREEN_LED_PIN, LOW);  // LED off
    digitalWrite(YELLOW_LED_PIN, LOW); // LED off
    digitalWrite(RED_LED_PIN, LOW);    // LED off
}

void sensorReadCallback()
{
    readSensor = true;
}