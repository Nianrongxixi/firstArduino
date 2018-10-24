#include <Time.h>
#include <TimeLib.h>

#include <SoftwareSerial.h>
#include <SD.h>

#define CHIP_SELECT 10      // For an Uno it's 10.
#define PIR_MOTION_SENSOR 2 // Use pin 2 to receive the signal from the module
#define TRIG_PIN 9          // Distance sensor trigger pin
#define ECHO_PIN 8          // Distance sensor echo pin
#define LOUDNESS 0
#define VIBRATION 3

SoftwareSerial SoftSerial(8, 7);
File logFile;

// defines variables
long duration;
int distance;
String pirData;
int loudness;
int vibrationData;

void setup()
{
    pinMode(TRIG_PIN, OUTPUT); // Sets the TRIG_PIN as an Output
    pinMode(ECHO_PIN, INPUT);  // Sets the ECHO_PIN as an Input
    pinMode(PIR_MOTION_SENSOR, INPUT);
    Serial.begin(9600); // Starts the serial communication

    // Serial.println("Setting up logFile on SD card");
    setupFile();

    SoftSerial.begin(9600); // the SoftSerial baud rate
}

void setupFile()
{
    pinMode(10, OUTPUT);

    // see if the card is present and can be initialized:
    if (!SD.begin(CHIP_SELECT))
    {
        Serial.println("Card init. failed!");
    }

    char tempFileName[15];
    strcpy(tempFileName, "LOG000.TXT");

    for (uint8_t i = 0; i < 100; i++)
    {
        tempFileName[3] = '0' + i / 100;
        tempFileName[4] = '0' + (i % 10) / 10;
        tempFileName[5] = '0' + (i % 100) % 10;

        // create if does not exist, do not open existing, write, sync after write
        if (!SD.exists(tempFileName))
        {
            break;
        }
    }
    logFile = SD.open(tempFileName, FILE_WRITE);
    if (!logFile)
    {
        Serial.print("FAILED TO CREATE ");
    }
    else
    {
        Serial.print("Writing to ");
        logFile.println("distance,pir,loudness,vibration,timestamp");
    }

    Serial.println(tempFileName);
}

void loop()
{
    loudness = analogRead(LOUDNESS);

    vibrationData = digitalRead(VIBRATION);
    // Serial.println(vibrationData);

    // Clears the TRIG_PIN
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    // Sets the TRIG_PIN on HIGH state for 10 micro seconds
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Reads the ECHO_PIN, returns the sound wave travel time in microseconds
    duration = pulseIn(ECHO_PIN, HIGH);

    // Calculating the distance
    distance = duration * 0.034 / 2;

    // NEED TO BE CONSIDERED
    if (digitalRead(PIR_MOTION_SENSOR))
        pirData = "People is coming";
    //if it detects the moving people?
    else
        pirData = "Watching";
    // Serial.println(pirData);

    // Write the data to the SD card
    outData();
}

//
// outData
//
void outData()
{
    // if the logFile is available, write to it:
    if (logFile)
    {
        logFile.println(String(distance) + "," + pirData + "," + String(loudness) + "," + String(vibrationData) + "," + String(hour()) + ":" + String(minute()) + ":" + String(second()) + ":" + String(millis()));

        // print to the serial port too:
        Serial.println(String(distance) + "," + pirData + "," + String(loudness) + "," + String(vibrationData) + "," + String(hour()) + ":" + String(minute()) + ":" + String(second()) + ":" + String(millis()));
    }
    Serial.println(!logFile);
    logFile.flush();
}
