//
//  main.c
//  covid_screener
//
//
//

#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
// TEMP SENSOR AND LCD
#define I2C_ADDR 0x27 //I2C adress
#define En_pin 10
#define Rs_pin 11
#define D4_pin 6
#define D5_pin 7
#define D6_pin 8
#define D7_pin 9
// ULTRASONIC SENSOR
#define echoPin 3 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 4 //attach pin D3 Arduino to pin Trig of HC-SR04
//Covid-19 screener


long duration, temp = 0, avgtemp = 0; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
//PASSIVE BUZZER
#define buzzer 2
//IR SENSOR
int IRSensor = 5;
//Relay module
int Relaydoor = 12;
int Relaylights = 13;
//MISC
unsigned int headcount = 0;
unsigned int count = 0, ircount = 0, i = 0;
LiquidCrystal lcd(Rs_pin, En_pin, D4_pin, D5_pin, D6_pin, D7_pin);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
void setup()
{
//LCD with TEMP
mlx.begin();
lcd.begin (16, 2);
lcd.home ();
//ULTRASONIC
pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
Serial.println("with Arduino UNO R3");
//IR
pinMode (IRSensor, INPUT); // sensor pin INPUT
//Relay
pinMode (Relaydoor, OUTPUT);
pinMode (Relaylights, OUTPUT);
}
void loop() {
if (headcount == 0)
digitalWrite(Relaylights, LOW);
else if (headcount > 0)
digitalWrite(Relaylights, LOW);
lcd.autoscroll();
// LCD



lcd.setCursor(0, 0);
lcd.print("Head Count ");
lcd.print(headcount);
lcd.setCursor(0, 1);
lcd.print("Please stand in front of the scanner");
lcd.scrollDisplayLeft();
delay(150);
lcd.print(mlx.readObjectTempC());
lcd.print(" C");
//US Sensor
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin HIGH (ACTIVE) for 10 microseconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
// Displays the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");

if (distance <= 12)
{
count++;
delay(5);
if (count >= 100)
{
headcount++;
count = 0;
delay(100);
}
}
else count = 0;
//IR sensor
if (digitalRead(IRSensor))
{
ircount++;
delay(5);
if (ircount >= 100)
{

Page 29 of 33

// PERSON DETECTED IN RANGE
tone(buzzer, 1000);
lcd.setCursor(0, 0);
lcd.print("Please stand still");
lcd.setCursor(0, 1);
lcd.print("Your Temperature is being measured");
lcd.scrollDisplayLeft();
lcd.print(mlx.readObjectTempC());
lcd.print(" C");
delay(1000);
noTone(buzzer);
// TEMPERATURE MEASURE
for (i = 1; i <= 100; i++)
{
temp = mlx.readObjectTempC() + temp;
delay(1);
}
avgtemp = temp / 100;
temp = 0;
delay(1000);
noTone(buzzer);
//TEMP NOTED
lcd.setCursor(0, 0);
lcd.print("Temperature : ");
lcd.print(avgtemp);
lcd.print(" C");
if (avgtemp > 37.5)
{
lcd.setCursor(0, 1);
lcd.print("Entry not allowed.");
lcd.scrollDisplayLeft();
delay(5000);
}
else if (avgtemp < 36.1)
{
lcd.setCursor(0, 1);
lcd.print("Please test again");
lcd.scrollDisplayLeft();
delay(5000);
}
else
{
//ACCESS GRANTED
lcd.setCursor(0, 1);



lcd.print("Entry allowed");
lcd.scrollDisplayLeft();
digitalWrite(Relaydoor, HIGH);
delay(7000);
}
avgtemp = 0;
}
}
else ircount = 0;
}
