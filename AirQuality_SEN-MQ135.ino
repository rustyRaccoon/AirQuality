#include <SPI.h>
#include <SD.h>

int sensor_value;
uint32_t oldMillis;
const int sensor_pin = A0;
const int SD_pin = 10;
//const int MOSI = 11;
//const int MISO = 12;
//const int SCK = 13;

void setup() {
  // pin modes
  pinMode(sensor_pin, INPUT);                         // connect to analog output of MQ-135

  // initial conditions
  oldMillis = 0;

  // SD card setup
  Serial.begin(9600);
  
  oldMillis = millis();
  while(!Serial){                                     // wait for serial communication (timeout after 3 seconds)
    if((millis()-oldMillis)>3000){
      break;
    }
  }
  
  Serial.print("Initializing SD card...");
  oldMillis = millis();
  if (!SD.begin(SD_pin)) {                            // see if the card is present and can be initialized (timeout after 5 seconds)
    while((millis()-oldMillis)<5000);
  }

  writeToFile("");                                    // mark that a new log has started in the file
  writeToFile("NEW LOG");                             
  Serial.println("card initialized.");                // card found
}

void writeToFile(String dataToWrite){
  File logFile = SD.open("dataLog.txt", FILE_WRITE);  //Open file to log data in
    if (logFile) {
      logFile.println(dataToWrite);
      logFile.close();
      Serial.println(" (logged)");
    }
    else {
      Serial.println(" (Error opening logfile!)");
    }
}

void loop() {
  if ((millis() - oldMillis) > 5000) {
    oldMillis = millis();                         // reset timer
    
    String data = "";                             // create string to store data temporarily
    sensor_value = analogRead(sensor_pin);        // read in value
    data += String(sensor_value, DEC);            // append sensor value to string

    Serial.print("Value read: ");
    Serial.print(sensor_value, DEC);              // write value to serial
    writeToFile(data);
  }
}
