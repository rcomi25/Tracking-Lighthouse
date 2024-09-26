/*
  MADE BY RICCARDO COMI
  YOU CAN CHANGE ANYTHING, JUST BE SURE WHAT YOU'RE DOING :D
*/


//Icluding the libraries
#include <Servo.h>                                                                    //Servo library
#include <LiquidCrystal_I2C.h>                                                        //I2C display library

Servo servo;                                                                          //Naming my servo 
LiquidCrystal_I2C lcd(0x27,20,4);                                                     //Defining the I2C address and the number of colunns and rows

long durationLX, durationRX;                                                          //Defining the values for the distance of the UltraSonic sensor
int distanceLX, distanceRX;
const int trigPinLX = 5, trigPinRX = 6, echoPinLX = 8, echoPinRX = 9;                 //Dfining pins for the UltraSonic sensor
const int fanPin = 2;                                                                 //Defining fan pin
const int buttonPinRX = 48, buttonPinLX = 49, buttonPinUP = 51, buttonPinDOWN = 50;   //Defining button pins
const int tempPin = A0;                                                                //Defining temperature sensor pin
int selection = -1;                                                                   //Defining the value of the selection in the display menu
int cursor = 0;                                                                       //Defining the position of the cursor in the menu
int keepTempVal = 40;                                                                 //Defining the temperature value to hold
const float hysteresis = 1.0;                                                         //Defining the hysteresis for the temperature holding
int valTempSensor;                                                                    //Defining the analog value from the temperature sensor.
int temperature;                                                                      //Defining the temperature value in Celsius
int angle = 0;                                                                        //Defining the servo angle
int sweepTime = 15;                                                                   
//Defining the sweep Time in ms for the servomotor 
int diffVal = 10;                                                                     //Defining the max difference value from LX and RX distance sensor
int fanSpeed = 0;

bool servoPos = true;
bool servoNeg = false;

//Defining the custom characters for the display

//CHECKMARK ( ✔ )
byte Check[8] =
{
0b00000,
0b00001,
0b00011,
0b10110,
0b11100,
0b01000,
0b00000,
0b00000
};

//UP ARROW ( ↑ )
byte UpArrow[8] =
{
0b00100,
0b01110,
0b10101,
0b00100,
0b00100,
0b00100,
0b00100,
0b00100
};

//DOWN ARROW ( ↓ )
byte DownArrow[8] =
{
0b00100,
0b00100,
0b00100,
0b00100,
0b00100,
0b10101,
0b01110,
0b00100
};

//LINE ( | )
byte Line[8] =
{
0b00100,
0b00100,
0b00100,
0b00100,
0b00100,
0b00100,
0b00100,
0b00100
};

//BACK ARROW ( < )
byte Back[8] =
{
0b00000,
0b00000,
0b00000,
0b00100,
0b01000,
0b10000,
0b01000,
0b00100
};

//Everything in here will run only on start
void setup()
{
  servo.attach(3);                                                                  //Attaching servo to pin 3
  lcd.init();                                                                       //Initializing LCD display
  lcd.clear();                                                                      //Clearing the display
  lcd.backlight();                                                                  //Turning the backlight of the LCD display

//Defining all of the inputs and outputs
  pinMode(trigPinLX, OUTPUT);
  pinMode(trigPinRX, OUTPUT);
  pinMode(echoPinLX, INPUT); 
  pinMode(echoPinRX, INPUT); 
  pinMode(fanPin, OUTPUT);
  pinMode(buttonPinRX, INPUT);
  pinMode(buttonPinLX, INPUT);
  pinMode(buttonPinUP, INPUT);
  pinMode(buttonPinDOWN, INPUT);
  pinMode(tempPin, INPUT);

//Creating the custom characters
  lcd.createChar(0, Check);
  lcd.createChar(1, UpArrow);
  lcd.createChar(2, DownArrow);
  lcd.createChar(3, Line);
  lcd.createChar(4, Back);

  servo.write(angle);                                                                 //Moving servo to angle 0

  welcome();                                                                          //Starting welcome function
}

//This code will run every cicle
void loop()
{
  selections();
  keepTemp();                                                                         //Satarting the keepTemp function
  delay(200);                                                                         //Stability 100 ms delay
}

//Just a quick welcome function
void welcome()
{ 
  lcd.clear();                                                                       
  lcd.setCursor(0,0);                                                                  //Setting the cursor
  lcd.print("Ciao User");                                                              //Print on the LCD
  lcd.setCursor(0,1);                                                                
  lcd.print("Benvenuto nel nostro");                                                   
  lcd.setCursor(0,2);                                                                  
  lcd.print("progetto");
  delay(3000);                                                                         //Delay 2 sec
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sto preparando");
  lcd.setCursor(0,1);
  lcd.print("Il sistema");
  lcd.setCursor(0,2);
  lcd.print("Prepara i");
  lcd.setCursor(0,3);
  lcd.print("Pop Corn :D");
  delay(3000);                                                                          //Delay 2 sec
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Scegli un'opzione");
  delay(2000);

  selections();                                                                         //Start selection function
}

//Selections function, this will run untill you take a decision
void selections()
{
  
  if (selection == -1)
  {
    if (digitalRead(buttonPinUP) == HIGH)                                               //If button UP is pressed...
    {
      cursor -= 1;                                                                      //Add 1 to var cursor
    }
    else if (digitalRead(buttonPinDOWN) == HIGH)                                        //Else if button DOWN is pressed...
    {
      cursor += 1;                                                                      //Subtract 1 from var cursor
    }

    if (cursor == -1)
    {
      cursor = 3; //CHANGE **ONLY** IF YOU ADD SELECTIONS
    }
    else if (cursor == 4)  //CHANGE **ONLY** IF YOU ADD SELECTIONS
    {
      cursor = 0; 
    }
    else if (cursor == 0)   //CHANGE TEMP
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cambia la temp.");
      lcd.setCursor(15, 0);
      lcd.write(4);
      lcd.setCursor(19, 0);
      lcd.write(1);
      lcd.setCursor(0, 1);
      lcd.print("Cambia ang. Servo");
      lcd.setCursor(19, 1);
      lcd.write(3);
      lcd.setCursor(0, 2);
      lcd.print("Servo SWEEP");
      lcd.setCursor(19, 2);
      lcd.write(3);
      lcd.setCursor(0, 3);
      lcd.print("Puntatore");
      lcd.setCursor(19, 3);
      lcd.write(2);

      if (digitalRead(buttonPinRX) == HIGH)
      {
        selection = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Cambia la temp.");
        lcd.setCursor(16, 0);
        lcd.write(0);
        delay(2000);
        menu();
      }
    }
    else if (cursor == 1)    //MOVE SERVO AS YOU LIKE
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cambia ang. Servo");
      lcd.setCursor(17, 0);
      lcd.write(4);
      lcd.setCursor(19, 0);
      lcd.write(1);
      lcd.setCursor(0, 1);
      lcd.print("Servo SWEEP");
      lcd.setCursor(19, 1);
      lcd.write(3);
      lcd.setCursor(0, 2);
      lcd.print("Puntatore");
      lcd.setCursor(19, 2);
      lcd.write(3);
      lcd.setCursor(0, 3);
      lcd.print("Cambia la temp.");
      lcd.setCursor(19, 3);
      lcd.write(2);

      if (digitalRead(buttonPinRX) == HIGH)
      {
        selection = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Cambia ang. Servo");
        lcd.setCursor(17, 0);
        lcd.write(0);
        delay(2000);
        menu();
      }
    }
    else if (cursor == 2)    //SERVO SWEEP
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Servo SWEEP");
      lcd.setCursor(11, 0);
      lcd.write(4);
      lcd.setCursor(19, 0);
      lcd.write(1);
      lcd.setCursor(0, 1);
      lcd.print("Puntatore");
      lcd.setCursor(19, 1);
      lcd.write(3);
      lcd.setCursor(0, 2);
      lcd.print("Cambia la temp.");
      lcd.setCursor(19, 2);
      lcd.write(3);      
      lcd.setCursor(0, 3);
      lcd.print("Cambia ang. Servo");
      lcd.setCursor(19, 3);
      lcd.write(2);

      if (digitalRead(buttonPinRX) == HIGH)
      {
        selection = 2;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Servo SWEEP");
        lcd.setCursor(11, 0);
        lcd.write(0);
        delay(2000);
        menu();
      }
    }
    else if (cursor == 3)     //OBJECT POINTER
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Puntatore");
      lcd.setCursor(9, 0);
      lcd.write(4);
      lcd.setCursor(19, 0);
      lcd.write(1);
      lcd.setCursor(0, 1);
      lcd.print("Cambia la temp.");  
      lcd.setCursor(19, 1);
      lcd.write(3);
      lcd.setCursor(0, 2);
      lcd.print("Cambia ang. Servo");  
      lcd.setCursor(19, 2);
      lcd.write(3);  
      lcd.setCursor(0, 3);
      lcd.print("Servo SWEEP");
      lcd.setCursor(19, 3);
      lcd.write(2);

      if (digitalRead(buttonPinRX) == HIGH)
      {
        selection = 3;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Puntatore");
        lcd.setCursor(9, 0);
        lcd.write(0);
        delay(2000);
        menu();
      }
    }
  }

  else
  {
    menu();
  }
}

//Everything from here, will determine what will happen on your selection
void menu()
{
  lcd.clear();

  if (selection == 0)
  {
    if (digitalRead(buttonPinLX) == HIGH)
    {
      selection = -1;
    }
    
    if (digitalRead(buttonPinUP) == HIGH)
    {
      keepTempVal += 10;
      checkTempVal();
    }
    else if (digitalRead(buttonPinDOWN) == HIGH)
    {
      keepTempVal -= 10;
      checkTempVal();
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(4);
    lcd.setCursor(1, 0);
    lcd.print("Torna indietro");
    lcd.setCursor(0, 1);
    lcd.print("Temperatura = ");
    lcd.setCursor(15, 1);
    lcd.print(String(temperature));
    lcd.setCursor(0, 2);
    lcd.print("Mantenere = ");
    lcd.setCursor(13, 2);
    lcd.print(String(keepTempVal));
    lcd.setCursor(0, 3);
    lcd.print("Vel. ventola = ");
    lcd.setCursor(16, 3);
    lcd.print(String(fanSpeed));

    delay(800);
  }
  else if (selection == 1)
  {
    if (digitalRead(buttonPinLX) == HIGH)
    {
      selection = -1;
    }

    if (digitalRead(buttonPinUP) == HIGH)
    {
      angle += 10;
      checkAngle();
      servo.write(angle);
    }
    else if (digitalRead(buttonPinDOWN) == HIGH)
    {
      angle -= 10;
      checkAngle();
      servo.write(angle);
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(4);
    lcd.setCursor(1, 0);
    lcd.print("Torna indietro");
    lcd.setCursor(0, 1);
    lcd.print("Angolo att. = ");
    lcd.setCursor(15, 1);
    lcd.print(String(angle));
    lcd.setCursor(0, 3);
    lcd.print("Temperatura = ");
    lcd.setCursor(15, 3);
    lcd.print(String(temperature));

    delay(400);
  }
  else if (selection == 2)
  {
    if (digitalRead(buttonPinLX) == HIGH)
    {
      selection = -1;
    }

    if (digitalRead(buttonPinUP) == HIGH)
    {
      sweepTime += 15;
      checkSwpeepSpeed();
    }
    else if (digitalRead(buttonPinDOWN) == HIGH)
    {
      sweepTime -= 15;
      checkSwpeepSpeed();
    }

    

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(4);
    lcd.setCursor(1, 0);
    lcd.print("Torna indietro");
    lcd.setCursor(0, 1);
    lcd.print("Velocita = ");
    lcd.setCursor(12, 1);
    lcd.print(String(sweepTime));
    lcd.setCursor(0, 3);
    lcd.print("Temperatura = ");
    lcd.setCursor(15, 3);
    lcd.print(String(temperature));

    servoSweep();
  }
  else if (selection == 3)
  {
    if (digitalRead(buttonPinLX) == HIGH)
    {
      selection = -1;
    }

    objectPointer();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(4);
    lcd.setCursor(1, 0);
    lcd.print("Torna indietro");
    lcd.setCursor(0, 1);
    lcd.print("Puntamento attivato");
    lcd.setCursor(0, 3);
    lcd.print("Temperatura = ");
    lcd.setCursor(15, 3);
    lcd.print(String(temperature));

    delay(300);
  }
}

//This function keeps the temperature of the lifght at the temperature you input
void keepTemp()
{
  getTemperature();                                                                         //Reads current temperature
  if ((keepTempVal - temperature) > hysteresis)                                             //If the temperature is too high
  {
    analogWrite(fanPin, 0);                                                                  //Turn off the fan
    fanSpeed = 0;
  }
  else if ((keepTempVal - temperature) < hysteresis)                                         //If the temperature is too high
  {
    analogWrite(fanPin, 255);                                                                //Turn on the fan
    fanSpeed = 100;
  }
  else
  {
    analogWrite(fanPin, map((keepTempVal - temperature), -hysteresis, hysteresis, 255, 0));   //If temperature is within range, adjust fan speed proportionally
    fanSpeed = (map((keepTempVal - temperature), -hysteresis, hysteresis, 255, 0) / 255) * 100;
  }
}

//This function will check if the temperature value, selected in the menu, is in the delta
void checkTempVal()
{
  if (keepTempVal > 80)
  {
    keepTempVal = 80;
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("IMPOSTA UNA TEMP < DI");
    lcd.setCursor(0,2);
    lcd.print("80 GRADI CELSIUS");
    delay(2000);
  }
  else if (keepTempVal < 40)
  {
    keepTempVal = 30;
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("IMPOSTA UNA TEMP > DI");
    lcd.setCursor(0,2);
    lcd.print("30 GRADI CELSIUS");
    delay(2000);
  }
}

//This function, when called, will point an object that is closer than 1.8 meters
void objectPointer()
{
  getDistance();

  if ((distanceLX < 180) && (distanceRX < 180))
  {
    if (abs(distanceLX - distanceRX) <= 10) 
    {

    }
    else if (distanceLX > distanceRX)
    {
      angle = max(0, angle - 10);
    }
    else if (distanceRX > distanceLX)
    {
      angle = min(120, angle + 10);
    }
  }
  else
  {
    if (servoPos == true)
    {
      if (angle > 90)
      {
        servoNeg = true;
        servoPos = false;
      }
      else
      {
        angle = min(120, angle + 30);
      }
    }
    else if (servoNeg == true)
    {
      if (angle <= 0)
      {
        servoNeg = false;
        servoPos = true;
      }
      else
      {
        angle = max(0, angle - 30);
      }
    }
  }

  servo.write(angle);
}

//This function will check the temperature value, reading directly from your sensor and converting it to Celsius
void getTemperature()
{
  valTempSensor = analogRead(tempPin);                                                     //Read the raw value from the temperature sensor
  temperature = ((valTempSensor * 0.00488) - 0.5) / 0.01;                                  //Convert the raw value, to celsius
}

//This function will make your servo sweep from 0-150 degrees
void servoSweep()
{
  //Move the servo from 0 to 150 degrees
  for (angle = 0; angle <= 150; angle += 1) 
    { 
      servo.write(angle);            
      delay(sweepTime);                                                                     //Pause for 'sweepTime' milliseconds between each degree                 
    }
  //Move the servo back from 150 to 0 degrees
  for (angle = 150; angle >= 0; angle -= 1) 
    {
      servo.write(angle);          
      delay(sweepTime);                                                                     //Pause for 'sweepTime' milliseconds between each degree                 
    }
}

//This function will give you a distance of both sensors
void getDistance()
{
  digitalWrite(trigPinLX, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinLX, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinLX, LOW);
  
  durationLX = pulseIn(echoPinLX, HIGH);
  distanceLX = durationLX * 0.034 / 2;
  
  digitalWrite(trigPinRX, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinRX, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinRX, LOW);
  
  durationRX = pulseIn(echoPinRX, HIGH);
  distanceRX = durationRX * 0.034 / 2;
}

//This function will check if the angle, selected in the menu, is in the delta
void checkAngle()
{
  if (angle > 150) //DO NOT CHANGE THIS VALUE
  {
    angle = 150;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ATTENZIONE");
    lcd.setCursor(0, 1);
    lcd.print("NON OLTREPASSARE I");
    lcd.setCursor(0, 2);
    lcd.print("150 gradi");
  }
  else if (angle < 0) //DO NOT CHANGE THIS VALUE
  {
    angle = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ATTENZIONE");
    lcd.setCursor(0, 1);
    lcd.print("NON ANDARE SOTTO");
    lcd.setCursor(0, 2);
    lcd.print("gli 0 gradi");
  }
}

//This function will check if the sweep time, selected in the menu, is in the delta
void checkSwpeepSpeed()
{
  if (sweepTime > 200) //DO NOT CHANGE THIS VALUE
  {
    sweepTime = 200;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ATTENZIONE");
    lcd.setCursor(0, 1);
    lcd.print("NON OLTREPASSARE I");
    lcd.setCursor(0, 2);
    lcd.print("200 ms");
  }
  else if (sweepTime < 15) //DO NOT CHANGE THIS VALUE
  {
    sweepTime = 15;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ATTENZIONE");
    lcd.setCursor(0, 1);
    lcd.print("NON ANDARE SOTTO I");
    lcd.setCursor(0, 2);
    lcd.print("15 ms");
  }
}
