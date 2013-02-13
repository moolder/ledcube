// this version (_05) tries to create better performance by losing sqrt calculations for the distance calc


// STCP:
#define STORAGE_REG_CLOCK 13

// SHCP:
#define SHIFT_REG_CLOCK 12


#define CUBEWIDTH 8
#define CUBEHEIGHT 3
#define LAYERNUM 8

int bitmap[CUBEWIDTH][CUBEHEIGHT][LAYERNUM];
int shiftDataPin[4];


#define LED_PIN 2

#define BUTTON_1_PIN 9
#define BUTTON_2_PIN 10

int sqrtval[31];

void setup() {
  sqrtval[0] = 0;
  sqrtval[1] = 3;
  sqrtval[2] = 4;
  sqrtval[3] = 5;
  sqrtval[4] = 6;
  sqrtval[5] = 7;
  sqrtval[6] = 7;
  sqrtval[7] = 8;
  sqrtval[8] = 8;
  sqrtval[9] = 9;
  sqrtval[10] = 10;
  sqrtval[11] = 10;
  sqrtval[12] = 10;
  sqrtval[13] = 11;
  sqrtval[14] = 11;
  sqrtval[15] = 12;
  sqrtval[16] = 12;
  sqrtval[17] = 13;
  sqrtval[18] = 13;
  sqrtval[19] = 13;
  sqrtval[20] = 14;
  sqrtval[21] = 14;
  sqrtval[22] = 14;
  sqrtval[23] = 15;
  sqrtval[24] = 15;
  sqrtval[25] = 15;
  sqrtval[26] = 16;
  sqrtval[27] = 16;
  sqrtval[28] = 16;
  sqrtval[29] = 17;
  sqrtval[30] = 17;

  
  shiftDataPin[0] = 9; // red
  shiftDataPin[1] = 11; // green
  shiftDataPin[2] = 10; // blue
  shiftDataPin[3] = 8; // rowselect

  // set the digital pin as output:
  pinMode(SHIFT_REG_CLOCK, OUTPUT);      
  pinMode(STORAGE_REG_CLOCK, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
//  pinMode(BUTTON_1_PIN, INPUT);
//  pinMode(BUTTON_2_PIN, INPUT);
  
  for (int p = 0; p < 4; p++){
    pinMode(shiftDataPin[p], OUTPUT);
  }
  digitalWrite(SHIFT_REG_CLOCK, HIGH);      
  digitalWrite(STORAGE_REG_CLOCK, HIGH);
  digitalWrite(LED_PIN, HIGH);
  for (int p = 0; p < 4; p++){
    digitalWrite(shiftDataPin[p], LOW);
  }
  initBitmap();
  randomSeed(1337); //TODO
  //Serial.begin(19200);
}


boolean ledpinValue = HIGH;
boolean button1Pushed = LOW;
boolean button2Pushed = LOW;
boolean oldButton1Pushed = LOW;
boolean oldButton2Pushed = LOW;


boolean ledPinState = HIGH;

long prevLoopMillis = -1;
long thisLoopMillis = -1;
void loop(){
  prevLoopMillis = thisLoopMillis;
  thisLoopMillis = millis();
  
  if (thisLoopMillis % 500 < prevLoopMillis % 500){
    ledPinState = !ledPinState;
    digitalWrite(LED_PIN, ledPinState);
  }

//  oldButton1Pushed = button1Pushed;
//  oldButton2Pushed = button2Pushed;
  
//  button1Pushed = digitalRead(BUTTON_1_PIN);
//  button2Pushed = digitalRead(BUTTON_2_PIN);

  /*
  if (button1Pushed){
    Serial.print("BUTTON1 ");
  } else {
    Serial.print("button1 ");
  }

  if (button2Pushed){
    Serial.println("BUTTON2 ");
  } else {
    Serial.println("button2 ");
  }
  */
  
/*  if (oldButton1Pushed == LOW && button1Pushed){
    selectedDemo++;
    if (selectedDemo > 9) selectedDemo = -1;
  }
*/  

  for (int brightness = 64; brightness < 256; brightness += 64){
    for (int z = 0; z < LAYERNUM; z++){
      for (int x = 0; x < CUBEWIDTH; x++){
        pushValues(!(bitmap[x][0][z] > brightness),
          !(bitmap[7-x][1][z] > brightness),
          !(bitmap[x][2][z] > brightness),
          !(z == x));
      }
      
      showValues();
      //delayMicroseconds(2000/12);
      //delay(150);
    }
    
    
    for (int x = 0; x < CUBEWIDTH; x++){
      pushValues(HIGH, HIGH, HIGH, HIGH);
    }
    
    showValues();
    
    
    updateBitmap(thisLoopMillis);
  }  
  
  
}



