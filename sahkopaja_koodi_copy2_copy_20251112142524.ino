#include <Wire.h>
const int forwardSpeed = 80;  // Speed for forward movement
const int turnspeed = 100;
const int turningFast = 60;  // Speed for turning movements
const int turningSlow = 0;
const int turnDelay = 25;      // Delay for completing a turn
const int uTurnDelay = 50; 
const int uTurnSpeed = 100;
const int detectionThreshold = 400;
const int ENB = 3;   // Moottori A = oikea nopeus (PWM)
const int INB1 = 4;   // Moottori A suunta 1
const int INB2 = 5;   // Moottori A suunta 2
const int ENA = 6;  // Moottori B = vasen nopeus (PWM)
const int INA1 = 9;   // Moottori B suunta 1
const int INA2 = 7;   // Moottori B suunta 2
int lastDirection = 3;

void setup() {
  Wire.begin();          // Alusta I2C
  Serial.begin(9600); 
  
  pinMode(ENA, OUTPUT);
  pinMode(INA1, OUTPUT);
  pinMode(INA2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(INB1, OUTPUT);
  pinMode(INB2, OUTPUT);   // Alusta sarjaportti debuggausta varten
}
// tämä funktio selvittää parametrikseen saamiensa arvojen avulla tulisko kulkea eteenpäin, pysähtyä vai kääntyä ja millä nopeudella 
//ja palauttaa kokonaisluvun 0-5 sen mukaisesti. 0 = pysähtyy, 1 = vasen nopea, 2 = vasen hidas, 3 = eteen, 4 = oikea hidas, 5 = oikea nopea 
int whereToGo(int leftValue, int frontValue, int rightValue){
  if (frontValue < detectionThreshold && rightValue > detectionThreshold && leftValue > detectionThreshold){
    return 3;
  }
  else if (leftValue < detectionThreshold && frontValue > detectionThreshold && rightValue > detectionThreshold){
    return 2;
  }

  else if (leftValue < detectionThreshold && frontValue < detectionThreshold && rightValue > detectionThreshold){
    return 3;
  }
  else if (leftValue > detectionThreshold && frontValue < detectionThreshold && rightValue < detectionThreshold){
    return 4;
  }
  else if (leftValue > detectionThreshold && frontValue > detectionThreshold && rightValue < detectionThreshold){
    return 4;
  }
else if (leftValue < detectionThreshold && frontValue < detectionThreshold && rightValue < detectionThreshold){
    return 4;
  }
  else if (leftValue < detectionThreshold && frontValue > detectionThreshold && rightValue < detectionThreshold){
    return 6;
  }
  else{
    return 0;
  }

  }


// funktiot robotin ajamiseen

void moveForward() {
      Serial.println("Move Forward");
      analogWrite(ENA, forwardSpeed);
      digitalWrite(INA1, 255);
      digitalWrite(INA2, 0);
      analogWrite(ENB, forwardSpeed);
      digitalWrite(INB1, 255);
      digitalWrite(INB2, 0);
      
}
void moveBack() {
      Serial.println("Move back");
      analogWrite(ENA, forwardSpeed);
      digitalWrite(INA1, 0);
      digitalWrite(INA2, 255);
      analogWrite(ENB, forwardSpeed);
      digitalWrite(INB1, 0);
      digitalWrite(INB2, 255);
      
}

void moveBackLeft() {
      Serial.println("Move back left");
      analogWrite(ENA, forwardSpeed - 120);
      digitalWrite(INA1, 0);
      digitalWrite(INA2, 255);
      analogWrite(ENB, forwardSpeed);
      digitalWrite(INB1, 0);
      digitalWrite(INB2, 255);
      delay(50);
      
}
void stop(){
      analogWrite(ENA, 0);
      digitalWrite(INA1, 0);
      digitalWrite(INA2, 0);
      analogWrite(ENB, 0);
      digitalWrite(INB1, 0);
      digitalWrite(INB2, 0);
}


void uTurn(){
  Serial.println("U-turn");
  moveForward();
  delay(1400);
  analogWrite(ENA, uTurnSpeed);
  digitalWrite(INA1, 255);
  digitalWrite(INA2, 0);
  analogWrite(ENB, uTurnSpeed);
  digitalWrite(INB1, 0);
  digitalWrite(INB2, 255);
  delay(2600);
  Wire.requestFrom(0x11, 10);  

  int analog[5];
   
  
  for (int i = 0; i < 5; i++) {
      int high = Wire.read() << 8;  
      int low = Wire.read();        
      analog[i] = high + low;       
    }
  int leftValue = analog[0];
  int frontValue = analog[2];
  int rightValue = analog[4];
  Serial.println((leftValue > detectionThreshold && frontValue > detectionThreshold && rightValue > detectionThreshold));
  Serial.print("Anturi 1: ");
    Serial.print(leftValue);      // Indeksi 0 = anturi 1
    Serial.print(" | Anturi 3: ");
    Serial.print(frontValue);      // Indeksi 2 = anturi 3
    Serial.print(" | Anturi 5: ");
    Serial.println(rightValue);    // Indeksi 4 = anturi 5
  
  while (leftValue > detectionThreshold && frontValue > detectionThreshold && rightValue > detectionThreshold){
    Serial.print("Anturi 1: ");
    Serial.print(leftValue);      // Indeksi 0 = anturi 1
    Serial.print(" | Anturi 3: ");
    Serial.print(frontValue);      // Indeksi 2 = anturi 3
    Serial.print(" | Anturi 5: ");
    Serial.println(rightValue);    // Indeksi 4 = anturi 5
  Wire.requestFrom(0x11, 10);  

  int analog[5];
  for (int i = 0; i < 5; i++) {
      int high = Wire.read() << 8;  
      int low = Wire.read();        
      analog[i] = high + low;       
    }
   
  leftValue = analog[0];
  frontValue = analog[2];
  rightValue = analog[4];
  moveForward();
  delay(50);

  }
}
void UTurn(){
  Serial.println("U-turn");
  moveForward();
  delay(500);
  analogWrite(ENA, 180);
  digitalWrite(INA1, 255);
  digitalWrite(INA2, 0);
  analogWrite(ENB, 180);
  digitalWrite(INB1, 0);
  digitalWrite(INB2, 255);
  Wire.requestFrom(0x11, 10);  

  int analog[5];
   
  
  for (int i = 0; i < 5; i++) {
      int high = Wire.read() << 8;  
      int low = Wire.read();        
      analog[i] = high + low;       
    }
  int leftValue = analog[0];
  int frontValue = analog[2];
  int rightValue = analog[4];
  Serial.println((leftValue > detectionThreshold && frontValue > detectionThreshold && rightValue > detectionThreshold));
  Serial.print("Anturi 1: ");
    Serial.print(leftValue);      // Indeksi 0 = anturi 1
    Serial.print(" | Anturi 3: ");
    Serial.print(frontValue);      // Indeksi 2 = anturi 3
    Serial.print(" | Anturi 5: ");
    Serial.println(rightValue);    // Indeksi 4 = anturi 5
  
  while (leftValue > detectionThreshold && frontValue > detectionThreshold && rightValue > detectionThreshold){
    analogWrite(ENA, 180);
    digitalWrite(INA1, 255);
    digitalWrite(INA2, 0);
    analogWrite(ENB, 180);
    digitalWrite(INB1, 0);
    digitalWrite(INB2, 255);
    Wire.requestFrom(0x11, 10);  

  int analog[5];
  for (int i = 0; i < 5; i++) {
      int high = Wire.read() << 8;  
      int low = Wire.read();        
      analog[i] = high + low;       
    }
   
  leftValue = analog[0];
  frontValue = analog[2];
  rightValue = analog[4];
  
  delay(50);

  }
}

void turnLeft(){
      Serial.println("Turn left");
      analogWrite(ENA, turnspeed);
      digitalWrite(INA1, 255);
      digitalWrite(INA2, 0);
      analogWrite(ENB, turningFast);
      digitalWrite(INB1, 0);
      digitalWrite(INB2, 255);

}



void turnRight(){
      Serial.println("Turn right");
      analogWrite(ENA, turningFast);
      digitalWrite(INA1, 0);
      digitalWrite(INA2, 255);
      analogWrite(ENB, turnspeed);
      digitalWrite(INB1, 255);
      digitalWrite(INB2, 0);

}


void loop() {
  Serial.println(lastDirection);
  Wire.requestFrom(0x11, 10);  

  if (Wire.available() == 10) {  
    int analog[5];               

    
    for (int i = 0; i < 5; i++) {
      int high = Wire.read() << 8;  
      int low = Wire.read();        
      analog[i] = high + low;       
    }

    
    int leftValue = analog[0];
    int frontValue = analog[2];
    int rightValue = analog[4];
    Serial.print("Anturi 1: ");
    Serial.print(leftValue);      // Indeksi 0 = anturi 1
    Serial.print(" | Anturi 3: ");
    Serial.print(frontValue);      // Indeksi 2 = anturi 3
    Serial.print(" | Anturi 5: ");
    Serial.println(rightValue);    // Indeksi 4 = anturi 5

    const int dir = whereToGo(leftValue, frontValue, rightValue);
    if (dir == 3){
      lastDirection = 3;
      moveForward();
    } 
    else if (dir == 2){
      lastDirection = 2;
      turnLeft();
    }
    else if (dir == 4){
      lastDirection = 4;
      turnRight();
    }
    
    else if (dir == 6){
      lastDirection = 6;
      turnRight();
    }
    else{
      if (lastDirection == 3){
      
      UTurn();
      lastDirection = 2;
      }
      else {
        moveBack();
      }
    }

    }
    
    delay(150);
    

  }







    // Decision-making based on sensor states
  
