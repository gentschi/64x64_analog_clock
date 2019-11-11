
#define double_buffer

#include <ESP8266WiFi.h>
#include <Ticker.h>

// PxMatrix - Library by 2dom
#include <PxMatrix.h>
// NTPtimeESP - Library by Andreas Spiess
#include <NTPtimeESP.h>

Ticker display_ticker;

// Pins for LED MATRIX
#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_OE 2
#define P_D 12
#define P_E 0

PxMATRIX display(64, 64, P_LAT, P_OE, P_A, P_B, P_C, P_D, P_E);

NTPtime NTP("192.168.0.1");   //ntp-server, pool or fritz.box   

// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);

uint16 myCOLORS[8] = {myRED, myGREEN, myBLUE, myWHITE, myYELLOW, myCYAN, myMAGENTA, myBLACK};

strDateTime dateTime;

void setup() {
  display.begin(32);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booted");
  Serial.println("Connecting to WiFi");

  WiFi.begin("ssid","ssid-pw");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected!");
}

float deg2rad(float deg) {
  float rad = (deg * 3.1415 / 180);
  return rad;
}

void display_updater() {
  display.display(25);   // when value > 25 Sketch crashes! means dont work properly! 
}

void loop() {
  //Serial.println("in the loop....");
  dateTime = NTP.getNTPtime(1,1);  // timezone, dst
  if (dateTime.valid) { 
    //NTP.printDateTime(dateTime);
    display_ticker.attach(0.002, display_updater);  
  
    int cx = 32;
    int cy = 32;
    int h  = 14;
  
    int r_sec = 28;  // Length of second-hand
    int r_min = 26;  // Length of minute-hand
    int r_hr  = 20;  // Length of hour-hand
  
    byte seconds = dateTime.second; 
    byte minutes = dateTime.minute + seconds/60; 
    byte hours   = dateTime.hour + minutes/60; 
  
    int x_sec = r_sec * cos(deg2rad((seconds-15) * (360/60))) + cx;
    int y_sec = r_sec * sin(deg2rad((seconds-15) * (360/60))) + cy;
  
    int x_min = r_min * cos(deg2rad((minutes-15) * (360/60))) + cx;
    int y_min = r_min * sin(deg2rad((minutes-15) * (360/60))) + cy;
  
    int x_hr = r_hr * cos(deg2rad((hours-3) * (360/12))) + cx;
    int y_hr = r_hr * sin(deg2rad((hours-3) * (360/12))) + cy;
  
    display.clearDisplay();
    //display.fillScreen(myBLACK);  flickers too much....
    display.drawCircle(cx,cy,31,myCYAN);
  
    display.setTextColor(myBLUE);
  
    display.setCursor(27,4);
    display.print("12");
  
    display.setCursor(4,29);
    display.print("9");
  
    display.setCursor(56,29);
    display.print("3");
  
    display.setCursor(30,54);
    display.print("6");
  
    display.drawLine(cx,cy,x_sec,y_sec,myRED);
    display.drawLine(cx,cy,x_min,y_min,myWHITE);
    display.drawLine(cx,cy,x_hr,y_hr,myWHITE);
    display.drawCircle(x_hr,y_hr,2,myMAGENTA);
  
    display.drawCircle(cx,cy,0,myBLACK);
    display.drawCircle(cx,cy,1,myBLACK);
    display.drawCircle(cx,cy,2,myCYAN);
    display.showBuffer();
    delay(1000);
  }
  display_ticker.detach();
}
