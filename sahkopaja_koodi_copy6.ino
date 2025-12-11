#include <Wire.h>
const int forwardSpeed = 120; // nopeus etenemiseen
const int turnSpeed = 100;   // käännöksen sisäpuolen telan nopeus, pyörii taaksepäin käännöksessä
const int uTurnSpeed = 80;  // nopeus u-käännöksiin
const int detectionThreshold = 400;   // Raja millä arvolla sensori tunnistaa teipin. Luetut arvot välillä 0-1024.
const int ENA = 3;   // Moottori A  = oikea nopeus (PWM)
const int INA1 = 4;   // Moottori A suunta 1
const int INA2 = 5;   // Moottori A suunta 2
const int ENB = 6;  // Moottori B = Oikea nopeus (PWM)
const int INB1 = 9;   // Moottori B suunta 1
const int INB2 = 7;   // Moottori B suunta 2
const int front = 0;  // etusensori, joka tunnistaa maalin
const int refreshRate = 100;  // päivitystaajuus millisekunteina
int lastDirection = 3;  // viimeinen suunta, alustetaan eteenpäin

void setup() {
  Wire.begin();          // Alusta I2C
  Serial.begin(9600); 
  
  pinMode(ENB, OUTPUT);
  pinMode(INB1, OUTPUT);
  pinMode(INB2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(INA1, OUTPUT);
  pinMode(INA2, OUTPUT);   
}
// tämä funktio selvittää parametrikseen saamiensa arvojen avulla tulisko kulkea eteenpäin, pysähtyä vai kääntyä ja millä nopeudella 
//ja palauttaa kokonaisluvun [0, 2-4] sen mukaisesti. 0 = pysähtyy, 2 = vasen , 3 = eteen, 4 = oikea 
int whereToGo(int leftValue, int frontValue, int rightValue){
  if (frontValue < detectionThreshold && rightValue > detectionThreshold && leftValue > detectionThreshold){   // tunnistaa teipin keskellä, menee eteenpäin
    return 3;
  }
  else if (leftValue < detectionThreshold && frontValue > detectionThreshold && rightValue > detectionThreshold){  //tunnistaa teipin vasemmalla, kääntyy vasemmalle
    return 2;
  }

  else if (leftValue < detectionThreshold && frontValue < detectionThreshold && rightValue > detectionThreshold){ //tunnistaa teipin keskellä ja vasemmalla, jatkaa eteenpäin
    return 3;
  }
  else if (leftValue > detectionThreshold && frontValue < detectionThreshold && rightValue < detectionThreshold){  //tunnistaa teipin keskellä ja oikealla. kääntyy oikealle
    return 4;
  }
  else if (leftValue > detectionThreshold && frontValue > detectionThreshold && rightValue < detectionThreshold){  //tunnistaa teipin oikealla, kääntyy oikealle
    return 4;
  }
else if (leftValue < detectionThreshold && frontValue < detectionThreshold && rightValue < detectionThreshold){  // tunnistaa teipin kaikilla sensoreilla, kääntyy oikealle
    return 4;
  }
  else if (leftValue < detectionThreshold && frontValue > detectionThreshold && rightValue < detectionThreshold){ // tunnistaa teipin vasemmalla ja oikealla, kääntyy oikealle
    return 6;
  }
  else{
    return 0;
  }

  }


// funktiot robotin ajamiseen

void moveForward() {
      Serial.println("Move Forward");
      analogWrite(ENB, forwardSpeed);
      digitalWrite(INB1, 255);
      digitalWrite(INB2, 0);
      analogWrite(ENA, forwardSpeed);
      digitalWrite(INA1, 255);
      digitalWrite(INA2, 0);
      
}
void moveBack() {
      Serial.println("Move back");
      analogWrite(ENB, forwardSpeed);
      digitalWrite(INB1, 0);
      digitalWrite(INB2, 255);
      analogWrite(ENA, forwardSpeed);
      digitalWrite(INA1, 0);
      digitalWrite(INA2, 255);
      
}


void stop(){
      analogWrite(ENB, 0);
      digitalWrite(INB1, 0);
      digitalWrite(INB2, 0);
      analogWrite(ENA, 0);
      digitalWrite(INA1, 0);
      digitalWrite(INA2, 0);
}


void UTurn(){
  Serial.println("U-turn");
  moveForward();                  // ajaa eteenpäin puoli sekuntia
  delay(500);
  analogWrite(ENB, uTurnSpeed);  // alkaa pyörittämään moottoreita vastakkaisiin suuntiin ja kääntyy paikallaan vasemmalle
  digitalWrite(INB1, 255);
  digitalWrite(INB2, 0);
  analogWrite(ENA, uTurnSpeed);
  digitalWrite(INA1, 0);
  digitalWrite(INA2, 255);
  Wire.requestFrom(0x11, 10);  //pyytää sensoreilta dataa 

  int analog[5];
   
  
  for (int i = 0; i < 5; i++) {
      int high = Wire.read() << 8;  
      int low = Wire.read();        
      analog[i] = high + low;       
    }
  int leftValue = analog[0];
  int frontValue = analog[2];
  int rightValue = analog[4];
  Serial.print("Anturi 1: ");      // printtaukset anturien debuggaukseen.
    Serial.print(leftValue);      // Indeksi 0 = anturi 1
    Serial.print(" | Anturi 3: ");
    Serial.print(frontValue);      // Indeksi 2 = anturi 3
    Serial.print(" | Anturi 5: ");
    Serial.println(rightValue);    // Indeksi 4 = anturi 5
  
  while (leftValue > detectionThreshold && frontValue > detectionThreshold && rightValue > detectionThreshold){        // kääntyy niin kauan kunnes tunnistaa taas teipin ja jatkaa siitä normaalisti
    analogWrite(ENB, 180);
    digitalWrite(INB1, 255);
    digitalWrite(INB2, 0);
    analogWrite(ENA, 180);
    digitalWrite(INA1, 0);
    digitalWrite(INA2, 255);
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
  
  delay(50); // päivitystaajuus

  }
}

void turnLeft(){
      Serial.println("Turn left");
      analogWrite(ENB, forwardSpeed);
      digitalWrite(INB1, 255);
      digitalWrite(INB2, 0);
      analogWrite(ENA, turnSpeed);
      digitalWrite(INA1, 0);
      digitalWrite(INA2, 255);

}



void turnRight(){
      Serial.println("Turn right");
      analogWrite(ENB, turnSpeed);
      digitalWrite(INB1, 0);
      digitalWrite(INB2, 255);
      analogWrite(ENA, forwardSpeed);
      digitalWrite(INA1, 255);
      digitalWrite(INA2, 0);

}


void loop() {             // funktio joka pyörii kokoajan ja ajaa muita funktioita
  

  while (analogRead(0) > 300) {           // tarkistaa, tunnistaako etusensori maalia ja pysäyttää jos tunnistaa.
    Serial.println(analogRead(0));

    
    Serial.println("Stop");
    stop();
    delay(200);
  }
  Serial.println(lastDirection);
  Wire.requestFrom(0x11, 10);    // pyydetään sensorilta dataa 5 kokonaislukua eli 10 tavua

  if (Wire.available() == 10) {   // varmistetaan että saatiin oikea määrä dataa virheiden välttämiseksi
    int analog[5];                 

    
    for (int i = 0; i < 5; i++) {
      int high = Wire.read() << 8;  
      int low = Wire.read();        
      analog[i] = high + low;       
    }

    
    int leftValue = analog[0];    
    int frontValue = analog[2];
    int rightValue = analog[4];
    Serial.print("Anturi 1: ");    //
    Serial.print(leftValue);      // Indeksi 0 = anturi 1
    Serial.print(" | Anturi 3: ");
    Serial.print(frontValue);      // Indeksi 2 = anturi 3
    Serial.print(" | Anturi 5: ");
    Serial.println(rightValue);    // Indeksi 4 = anturi 5

    const int dir = whereToGo(leftValue, frontValue, rightValue);   // funktio joka ajaa robottia sensorien arvojrn mukaisesti
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
    
    delay(refreshRate);  // päivitystaajuus
    

  }








