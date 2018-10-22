#include <SoftwareSerial.h>
#include <SD.h>

#define CHIP_SELECT 10      // For an Uno it's 10.
#define PIR_MOTION_SENSOR 2 // Use pin 2 to receive the signal from the module
#define TRIG_PIN 9          // Distance sensor trigger pin
#define ECHO_PIN 8          // Distance sensor echo pin
#define LOUDNESS 0
#define VIBRATION 3

SoftwareSerial SoftSerial(8, 7);
File distanceFile;
File pirFile;
File loudnessFile;
File vibrationFile;

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

    Serial.println("Setting up file on SD card");
    setupFile(distanceFile, "DIS00.TXT");
    setupFile(pirFile, "PIR00.TXT");
    setupFile(loudnessFile, "LOU00.TXT");
    setupFile(vibrationFile, "VIB00.TXT");

    SoftSerial.begin(9600); // the SoftSerial baud rate
}

void setupFile(File file, String fileName)
{
    pinMode(10, OUTPUT);

    // see if the card is present and can be initialized:
    if (!SD.begin(CHIP_SELECT))
    {
        Serial.println("Card init. failed!");
    }

    char distanceFileName[15];
    strcpy(distanceFileName, fileName);
    for (uint8_t i = 0; i < 100; i++)
    {
        distanceFileName[6] = '0' + i / 10;
        distanceFileName[7] = '0' + i % 10;

        // create if does not exist, do not open existing, write, sync after write
        if (!SD.exists(distanceFileName))
        {
            break;
        }
    }
    file = SD.open(distanceFileName, FILE_WRITE);
    if (!file)
    {
        Serial.print("FAILED TO CREATE ");
    }
    else
    {
        Serial.print("Writing to ");
    }

    Serial.println(distanceFileName);
}

void loop()
{
    loudness = analogRead(LOUDNESS);

    vibrationData = digitalRead(VIBRATION);
    Serial.println(vibrationData);

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

    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);

    // NEED TO BE CONSIDERED
    if (digitalRead(PIR_MOTION_SENSOR))
        pirData = "Hi,people is coming";
    //if it detects the moving people?
    else
        pirData = "Watching";
    Serial.println(pirData);

    delay(200);

    // Write the data to the SD card
    outData(distanceFile, distance);
    outData(pirFile, pirData);
    outData(loudnessFile, loudness);
    outData(vibrationFile, vibrationData)

    // delay(500);
}

//
// outData
//
void outData(File file, String data)
{
    // if the file is available, write to it:
    if (file)
    {
        file.println(data);

        // print to the serial port too:
        Serial.println(data);
    }

    file.flush();
}