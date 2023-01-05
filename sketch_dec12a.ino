
/*********
 By the Autonomous Systems Laboratory 2022-2023, for the Department of Informatics & Telecommunications
 Lesson "Internet of Things" Computer System Courses Flow of the 7th Semester
*********/


#include <ThingerESP32.h> //include the official Thinger.io library
#include <DHT.h> //library version
#include <LiquidCrystal_I2C.h>//library version for the screen

#define USERNAME "nikos_sarris" //Enter your Thinger.io's account Username
#define DEVICE_ID "ESP32AM2051" //Enter your registered device ID
#define DEVICE_CREDENTIAL "!W1%LS%O6OtI36RW" //Enter your registered device credentials

#define SSID "CYTA4830" //Enter your Wi-Fi ID
#define SSID_PASSWORD "ZTEEF4AF5304086" //Enter your Wi-Fi password
#define DHTPIN 4     // connect to the D4
#define DHTTYPE DHT11   // declare the DHT 11 sensor type

// set the LCD number of columns and rows
int lcdColumns = 16; //set the LCD number of columns

int lcdRows = 2; //set the lcd number of rows

const int ledPin_GREEN = 7;//declare the green led on serial port 7

const int ledPin_RED  = 10;//declare the red led on serial port 10

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

DHT dht(DHTPIN, DHTTYPE); //declare the DHT PIN, WITH THE TYPE (INITIALIZE THE DHT11 SENSOR)

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL); //create a connection between the ESP32 & Thinger.io

void setup() {

  pinMode(LED_BUILTIN , OUTPUT);
  pinMode(ledPin_GREEN , OUTPUT);
  pinMode(ledPin_RED , OUTPUT);

  // open serial for debugging
  Serial.begin(115200); //intialize the serial baud rate to 115200 bauds/sec
  lcd.init(); //initialize the LCD display                     
  lcd.backlight();  // turn on LCD backlight 
  dht.begin(); //begin taking temperature measurements
  thing.add_wifi(SSID, SSID_PASSWORD); //establish the Wi-Fi connection

}

void loop() {
  delay(1000);   // Wait a second between measurements.
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) { //if the value reading failed
    Serial.println(F("Failed to read from DHT sensor!")); //then print the right message into the Serial Port 
    lcd.setCursor(0, 0); //also set the cursor to first column, first row 
    lcd.print("FAILURE"); //print the right message into the LCD Display
    return; 
  }
  if(isnan(h)) {
    Serial.println(F("Failed to read from DHT sensor!")); //then print the right message into the Serial Port 
    lcd.setCursor(0, 0); //also set the cursor to first column, first row 
    lcd.print("FAILURE"); //print the right message into the LCD Display
  }

  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  lcd.print(t);   // print the temperature
  lcd.setCursor(0,1); //set the cursor to the first column, second row
  lcd.print("Celsius"); //print the right message into the Serial port
    // clears the display to print new message
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));

  lcd.setCursor(10,0);
  lcd.print(h);
  lcd.setCursor(9,1);
  lcd.print("percent");

  // digitalWrite(LED_BUILTIN , LOW);//Turn off the BUILT_IN LED

  // clears the display to print new message
  delay(10000); //delay 10 seconds before clearing the lcd display (taking another measurement)
  lcd.clear(); //clear the LCD Display

  pson data ;

  data["temperature"] = dht.readTemperature();
  data["humidity"] = dht.readHumidity();
  

  if( t>= 25 && h >= 35){//if temperature and humidity are above those values we are sending an email to the client

  //When sending to email the red led is blinking 

    digitalWrite(ledPin_RED, HIGH);
    delay(500);
    digitalWrite(ledPin_RED , LOW);
    delay(500);

    thing.call_endpoint("Email" , data);

  }

  //Control the green pin througth Dashboard(Button)
  thing["GREEN_LED"] << digitalPin(ledPin_GREEN);
  

  // if( t >= 25 ){
  //   digitalWrite(LED_BUILTIN, HIGH);
  // }else if(t <=10){
  // digitalWrite(LED_BUILTIN, LOW);
  // }else{
  //   digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  //   delay(1000);                      // wait for a second
  //   digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  //   delay(1000);                     // wait for a second
  // }


  /*if(t >= 10){//In that condition all leds are on
    
  digitalWrite(ledPin_RED, HIGH);  
  digitalWrite(ledPin_GREEN , HIGH);
  digitalWrite(LED_BUILTIN , HIGH);
  }*/

  //HTTT requests
  thing.call_endpoint("test_sensor" , data);
  
  thing.handle(); //handle the connection

  //Values for Dashboards
  thing["VALUES EXAMLE"] >> [](pson& out){
    out["TEMPERATURE = "] = dht.readTemperature();
    out["HUMIDITY = "] = dht.readHumidity();
  };
  

}