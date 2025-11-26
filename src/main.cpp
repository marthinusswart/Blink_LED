#include <Arduino.h>
#include <DHT.h>

#define GREEN_LED_PIN 5   // D1 = GPIO5
#define YELLOW_LED_PIN 14 // D5 = GPI14
#define RED_LED_PIN 12    // D6 = GPI12
#define READ_DELAY_MS 5000
#define BLINK_DELAY_MS 2000
#define DHT_PIN 4 // D2 = GPIO4
#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);

#ifdef DEBUG
#define DBG(fmt, ...) Serial.printf(fmt, ##__VA_ARGS__)       /* Debug with no line feed */
#define DBGL(fmt, ...) Serial.printf(fmt "\n", ##__VA_ARGS__) /* Debug Line */
#else
#define DBG(fmt, ...)
#define DBGL(fmt, ...)
#endif

void setIndicator(float temperature);
void resetIndicators();

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
    DBG("GREEN_LED_PIN = %d\n", GREEN_LED_PIN);
    DBG("YELLOW_LED_PIN = %d\n", YELLOW_LED_PIN);
    DBG("RED_LED_PIN = %d\n", RED_LED_PIN);
}

void loop()
{
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
    delay(READ_DELAY_MS);
}

void setIndicator(float temperature)
{
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