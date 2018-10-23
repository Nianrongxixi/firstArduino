// link between the computer and the SoftSerial Shield
// at 9600 bps 8-N-1
// Computer is connected to Hardware UART
// SoftSerial Shield is connected to the Software UART:D2&D3

#include <SoftwareSerial.h>
#include <SD.h>

#define CHIP_SELECT 10                         // For an Uno it's 10

SoftwareSerial SoftSerial(8, 7);
File logfile;
File pirfile;
File distancefile;


//
// Setup
//
void setup()
{
  Serial.begin(9600);                      // the Serial port of Arduino baud rate.

  Serial.println("Setting up file on SD card");
  // setupSDFile();
  setupFile(logfile, "LOG000.TXT");
  setupFile(pirfile, "LOG000.TXT");
  setupFile(distancefile, "LOG000.TXT");
  
  SoftSerial.begin(9600);                  // the SoftSerial baud rate
}

//
// setupSDFile - Create and open for writing the file on the SD card
//
void setupSDFile()
{
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(CHIP_SELECT)) 
  {
    Serial.println("Card init. failed!");
  }

  char filename[15];
  strcpy(filename, "LOGFIL00.TXT");
  for (uint8_t i = 0; i < 100; i++) 
  {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    
    // create if does not exist, do not open existing, write, sync after write
    if (!SD.exists(filename)) 
    {
        break;
    }
  }

  logfile = SD.open(filename, FILE_WRITE);
  if ( !logfile ) 
  {
      Serial.print("FAILED TO CREATE "); 
  }
  else
  {
      Serial.print("Writing to "); 
  }

  Serial.println(filename);
}

void setupFile(File file, const char *fileName)
{
    pinMode(10, OUTPUT);

    // see if the card is present and can be initialized:
    if (!SD.begin(CHIP_SELECT))
    {
        Serial.println("Card init. failed!");
    }

    char tempFileName[15];
    strcpy(tempFileName, fileName);

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
    file = SD.open(tempFileName, FILE_WRITE);
    if (!file)
    {
        Serial.print("FAILED TO CREATE ");
    }
    else
    {
        Serial.print("Writing to ");
    }

    Serial.println(tempFileName);
}

//
// Loop
//
void loop()
{
    // Write the data to the SD card
    // outputData();
    outData(logfile, "1");
    outData(pirfile, "2");
    outData(distancefile, "3");
        
    delay(500);
}

//
// outputData
//
void outputData()
{
  int sensor = analogRead(A0);
  String dataString = "";

  dataString += String(sensor);

  // if the file is available, write to it:
  if (logfile) 
  {
    logfile.println(dataString);

    // print to the serial port too:
    Serial.println(dataString);
  }

  logfile.flush();
}

void outData(File file, String data)
{
    // if the file is available, write to it:
    if (file)
    {
        file.println(data);

        // print to the serial port too:
        Serial.println(data);
    }
    Serial.println(!file);
    file.flush();
}

