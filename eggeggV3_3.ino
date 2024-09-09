#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define TRIG_PIN1 3
#define ECHO_PIN1 4
#define TRIG_PIN2 5
#define ECHO_PIN2 6
#define TRIG_PIN3 7
#define ECHO_PIN3 8
#define MIN_DISTANCE_CHANGE 8  // Minimum distance change to consider as movement

#define DHTPIN 2           // what pin we're connected to
#define DHTTYPE DHT22      // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor for normal 16mhz Arduino

#include <SoftwareSerial.h>
SoftwareSerial sim800lSerial(14, 15);  //rx tx

long previousDistance1 = 0;
long previousDistance2 = 0;
long previousDistance3 = 0;

String MsDeliver = " ";

double minTemp = 36.00;
double maxTemp = 38.30;
double minHum = 55.00;
double maxHum = 63.00;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, 16 column and 2 rows

bool mdetected = false;                                           //movement detected,
char *indicatorTemp, indicatorHum, prevTin = " ", prevHin = " ";  //indicators

const char* phoneNumber = "+639993487532";

void setup() {
  Serial.begin(9600);

  dht.begin();
  lcd.init();  // initialize the lcd
  Wire.begin();
  lcd.backlight();

  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
  pinMode(TRIG_PIN3, OUTPUT);
  pinMode(ECHO_PIN3, INPUT);

  sim800lSerial.begin(19200);  // GSM Module Baud rate - communication speed
  // Send AT command to check if SIM800L is responding
  sim800lSerial.println("AT");
  delay(1000);

  // Check SIM800L response
  if (sim800lSerial.find("OK")) {
    Serial.println("SIM800L is ready.");
  } else {
    Serial.println("SIM800L not responding. Check connections.");
  }

  Serial.println("On na sya");

  delay(100);
}

void loop() {
  sim800lSerial.println("AT");
  delay(500);

  // Check SIM800L response
  if (sim800lSerial.find("OK")) {
    Serial.println("SIM800L is ready.");
  } else {
    Serial.println("SIM800L not responding. Check connections.");
  }

  // Serial.println("On na sya");

  delay(100);

  disphumtemp();

  condition1(ultrasonic1());
  condition1(ultrasonic2());
  condition1(ultrasonic3());
  condition2();


  MsDeliver = " ";
  delay(1000);
}

void disphumtemp() {
  delay(2000);
  //with display
  lcd.clear();          // clear display
  lcd.setCursor(0, 0);  // move cursor to   (0, 0)
  lcd.print("Humidity: ");
  lcd.print(dht.readHumidity());
  lcd.print("%");       // print message at (0, 0)
  lcd.setCursor(1, 1);  // move cursor to   (2, 1)
  lcd.print("Temp: ");
  lcd.print(dht.readTemperature());
  lcd.print(" C");  // print message at (2, 1)

}

bool ultrasonic1() {
  long duration, distance;
  digitalWrite(TRIG_PIN1, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN1, LOW);
  duration = pulseIn(ECHO_PIN1, HIGH);
  distance = (duration / 2) / 29.1;

  if (abs(distance - previousDistance1) > MIN_DISTANCE_CHANGE) {
    Serial.println("Movement detected1");
    Serial.println("ult1");
    Serial.print("curr ");
    Serial.println(distance);
    Serial.print("prev ");
    Serial.println(previousDistance1);
    Serial.print("abs ");
    Serial.println(abs(distance - previousDistance1));
    // mdetected = true;
    previousDistance1 = distance;
    return true;
  } else {
    Serial.println("No Movement :)1");
    mdetected = false;
    previousDistance1 = distance;
    return false;
  }

  previousDistance1 = distance;
  delay(1000);
}
bool ultrasonic2() {
  long duration, distance;
  digitalWrite(TRIG_PIN2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN2, LOW);
  duration = pulseIn(ECHO_PIN2, HIGH);
  distance = (duration / 2) / 29.1;

  if (abs(distance - previousDistance2) > MIN_DISTANCE_CHANGE) {
    Serial.println("Movement detected2");
    Serial.println("ult2");
    Serial.print("curr ");
    Serial.println(distance);
    Serial.print("prev ");
    Serial.println(previousDistance2);
    Serial.print("abs ");
    Serial.println(abs(distance - previousDistance2));
    mdetected = true;
    previousDistance2 = distance;
    return true;
  } else {
    Serial.println("No Movement :)2");
    mdetected = false;
    previousDistance2 = distance;
    return false;
  }

  previousDistance2 = distance;
  delay(1000);
}
bool ultrasonic3() {
  long duration, distance;
  digitalWrite(TRIG_PIN3, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN3, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN3, LOW);
  duration = pulseIn(ECHO_PIN3, HIGH);
  distance = (duration / 2) / 29.1;

  if (abs(distance - previousDistance3) > MIN_DISTANCE_CHANGE) {
    Serial.println("Movement detected3");
    Serial.println("ult3");
    Serial.print("curr ");
    Serial.println(distance);
    Serial.print("prev ");
    Serial.println(previousDistance3);
    Serial.print("abs ");
    Serial.println(abs(distance - previousDistance3));
    mdetected = true;
    previousDistance3 = distance;
    return true;
  } else {
    Serial.println("No Movement :)3");
    mdetected = false;
    previousDistance3 = distance;
    return false;
  }

  previousDistance3 = distance;
  delay(1000);
}

void Message(bool msgver) {
   String message = "Movement Detected! Egg has hatched. Check your incubator. This is for educational purposes no spamming intended.";
  String message2 = "Humidity:"+String(dht.readHumidity())+"% Temp:"+String(dht.readTemperature())+" C";

  if(!msgver){
    message = message2;
    Serial.println("Sending Message2 please wait....");
  }else{
    message = message;
    Serial.println("Sending Message1 please wait....");
  }

  sim800lSerial.println("AT+CMGF=1");
  delay(1000);

  // Set recipient phone number
  sim800lSerial.print("AT+CMGS=\"");
  sim800lSerial.print(phoneNumber);
  sim800lSerial.println("\"");
  delay(1000);

  // Send the SMS message
  sim800lSerial.print(message);
  sim800lSerial.write(26);  // End of message character

  Serial.println("Message Sent");

  delay(1000);
}
//ultrasonic detected
void condition1(bool cond1) {
  if (cond1) {
    Message(1);
    Serial.println("sending message ultr");
    mdetected = false;
  }
}
//temp and hum
void condition2(){
  float readingtemp = dht.readTemperature();
  float readinghum = dht.readHumidity();
  if(readingtemp > maxTemp || readinghum > maxHum){
    indicatorTemp = "H";
    indicatorHum = "H";
  } else if(readingtemp < minTemp || readinghum < minHum){
    indicatorTemp = "L";
    indicatorHum = "L";
  }

  if(indicatorTemp != prevTin){
    if(indicatorHum != prevHin){
      Message(false);
      prevHin = indicatorHum;
    }
  prevTin = indicatorTemp;
  }
}
