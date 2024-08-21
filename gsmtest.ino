#include <SoftwareSerial.h>

// Choose two Arduino pins to use for software serial
int RXPin = 14;
int TXPin = 15;

SoftwareSerial mySerial(RXPin, TXPin);

void setup() {
  // Start the serial communication with the computer
  Serial.begin(9600);

  // Start the serial communication with the SIM800L
  mySerial.begin(9600);

  // Give the module some time to initialize
  delay(1000);

  // Set the SIM800L to SMS text mode
  mySerial.println("AT+CMGF=1");
  delay(1000);
  if (mySerial.find("OK")) {
    Serial.println("Text mode set successfully.");
  } else {
    Serial.println("Error: Failed to set text mode.");
  }

  // Set the recipient phone number and send the message
  mySerial.print("AT+CMGS=\"+639566047496\"\r");
  delay(1000);
  mySerial.print("Hello from SIM800L on Arduino Mega!");
  mySerial.write(26); // Ctrl+Z to send the message
  Serial.println("Message sent.");
}

void loop() {
  // Loop is empty since the message is sent once in setup()
}
