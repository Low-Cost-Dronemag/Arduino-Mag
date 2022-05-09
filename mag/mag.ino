/*
Code for the Arduino-based magnetometer.
Logs magnetic data and GPS data.

Created by:
Brett Bernstein
*/

#include <TinyGPS++.h>
#include <SD.h>

static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;
const int dataPin = A0, chipSelect = 4;
float V_out = 0, R1 = 200, B = 0;
float latt = 0, lon = 0; 
int mm = 0, dd = 0, yy = 0, h = 0, m = 0, s = 0, cs = 0, ii;

// Define the sampling period (ms)
int T = 50;


// The TinyGPS++ object
TinyGPSPlus gps;

void setup()
{
  // Initialize serial monitor
  Serial.begin(9600); 
  
  // Initialize serial conneciton to GPS (using Serial1 ports on Arduino)
  Serial1.begin(9600);

  // Initialize SD card
  Serial.println("Initializing SD card...");
  pinMode(chipSelect, OUTPUT);
  // check that the memory card exists and is usable
  if (!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or not present");
    // stop sketch
    return;
  }
  Serial.println("memory card is ready");
    // If the file exits, keep the current record and include an indication that a new survey started
    if (SD.exists("gps.TXT")) {
      File dataFile = SD.open("gps.TXT", FILE_WRITE);
      if (dataFile) {
        dataFile.println( "New Survey" );
        dataFile.close();
      }
  }
  
}

void loop()
{
  // If information is coming through the serial port, 
  while (Serial1.available() > 0) {
    if (gps.encode(Serial1.read())) {
      collectData();
    }
    delay(T);
  }
  

}

float getMag()
{
  V_out = analogRead(dataPin);   // read the input pin
  B = V_out / (4*12.2*R1); // [mT]
  B = B*pow(10,6); // Convert mT to nT
  return B;
}

void collectData()
{
  // Check if there is a valid location reading
  if (gps.location.isValid())
  {
    latt = gps.location.lat(); // [decimal degrees]
    lon = gps.location.lng(); // [decimal degrees]
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  // Check if there is a valid date reading
  if (gps.date.isValid())
  {
    mm = gps.date.month(); 
    dd = gps.date.day();
    yy = gps.date.year();
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  // Check if there is a valid time reading
  if (gps.time.isValid())
  {
    h = gps.time.hour();
    m = gps.time.minute();
    s = gps.time.second();
    cs = gps.time.centisecond();
  }
    // Get mag reading
    B = getMag();    
    File dataFile = SD.open("gps.TXT", FILE_WRITE);
    if (dataFile) 
      {
    dataFile.print( B , 8);
    dataFile.print( F(", " ));
    dataFile.print( latt , 10);
    dataFile.print(F( ", " ));
    dataFile.print( lon , 10);
    dataFile.print( F(", " ));
    dataFile.print( mm );
    dataFile.print( F(", " ));
    dataFile.print( dd );
    dataFile.print( F(", " ));
    dataFile.print( yy );
    dataFile.print( F(", " ));
    dataFile.print( h );
    dataFile.print( F(", " ));
    dataFile.print( m );
    dataFile.print( F(", " ));
    dataFile.print( s );
    dataFile.print( F(", " ));
    dataFile.println( cs );
    dataFile.close();
        
      }
  else
  {
    Serial.print(F("INVALID"));
  }
}
