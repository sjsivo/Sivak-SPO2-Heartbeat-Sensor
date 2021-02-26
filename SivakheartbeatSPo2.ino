
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_PCD8544_84X48_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 33, /* data=*/ 25, /* cs=*/ 19, /* dc=*/ 12, /* reset=*/ 17);  // Nokia 5110 Display

#include <Wire.h>
#include "algorithm_by_RF.h"
#include "MAX30102.h"
//#include "spo2_algorithm.h"

//MAX30105 particleSensor;

uint32_t elapsedTime,timeStart;

uint32_t aun_ir_buffer[BUFFER_SIZE]; //infrared LED sensor data
uint32_t aun_red_buffer[BUFFER_SIZE];  //red LED sensor data
float old_n_spo2;  // Previous SPO2 value
uint8_t uch_dummy,k;
// Interrupt pin
const byte oxiInt = 16; // pin connected to MAX30102 INT

void setup()
{
  Serial.begin(115200); // initialize serial communication at 115200 bits per second:
  Wire.begin(21,22);
//  pinMode(pulseLED, OUTPUT);
 // pinMode(readLED, OUTPUT);
  u8g2.begin();

  maxim_max30102_reset(); //resets the MAX30102
  delay(1000);

  maxim_max30102_read_reg(REG_INTR_STATUS_1,&uch_dummy);  //Reads/clears the interrupt status register
  maxim_max30102_init();  //initialize the MAX30102
 
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);

  u8g2.clearBuffer();
  u8g2.drawStr( 0, 0, "Sivak Heart.");
  u8g2.sendBuffer();
  // Initialize sensor
  /*
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
     u8g2.clearBuffer();
  u8g2.drawStr( 0, 10, "Sensor Error!!!");
  u8g2.sendBuffer();
    while (1);
  }*/

  Serial.println(F("Attach sensor to finger with rubber band. Press any key to start conversion"));
  //while (Serial.available() == 0) ; //wait until user presses a key
  //Serial.read();

  //Setup to sense up to 18 inches, max LED brightness
  /* from original Maxim file
byte ledBrightness = 15; //Options: 0=Off to 255=50mA
byte sampleAverage = 8; //Options: 1, 2, 4, 8, 16, 32
byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
byte sampleRate = 400; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
int pulseWidth = 411; //Options: 69, 118, 215, 411
int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  particleSensor.enableDIETEMPRDY();
  */
}

void loop()
{
 float n_spo2,ratio,correl;  //SPO2 value
  int8_t ch_spo2_valid;  //indicator to show if the SPO2 calculation is valid
  int32_t n_heart_rate; //heart rate value
  int8_t  ch_hr_valid;  //indicator to show if the heart rate calculation is valid
  int32_t i;
  char hr_str[10];
     
  //buffer length of BUFFER_SIZE stores ST seconds of samples running at FS sps
  //read BUFFER_SIZE samples, and determine the signal range
  for(i=0;i<BUFFER_SIZE;i++)
  {
    while(digitalRead(oxiInt)==1);  //wait until the interrupt pin asserts
    maxim_max30102_read_fifo((aun_red_buffer+i), (aun_ir_buffer+i));  //read from MAX30102 FIFO

   // Serial.print(i, DEC);
  //  Serial.print(F("\t"));
    Serial.print(aun_red_buffer[i], DEC);
    Serial.print(F("\t"));
    Serial.print(aun_ir_buffer[i], DEC);    
    Serial.println("");

  }

  //calculate heart rate and SpO2 after BUFFER_SIZE samples (ST seconds of samples) using Robert's method
  rf_heart_rate_and_oxygen_saturation(aun_ir_buffer, BUFFER_SIZE, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid, &ratio, &correl);      

 // Serial.print(n_spo2);
 // Serial.print("\t");
 // Serial.println(n_heart_rate, DEC);

    u8g2.clearBuffer();
  u8g2.drawStr( 0, 0, "Sivak Heart.");
  u8g2.setCursor(0,10);
  u8g2.print(n_heart_rate,DEC);
  u8g2.print("BPM");
  u8g2.setCursor(0,20);
  u8g2.print(n_spo2,DEC);
  u8g2.print("%");
  u8g2.sendBuffer();
}
