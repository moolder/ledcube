// RGB LED Matrix - branch of the ledcube4x4x4
// this version (_05) tries to create better performance by losing sqrt calculations for the distance calc
// license: Creative Commons Non-Commercial Attribution Share-Alike


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

//bitmaps 
long pacman[4][8];


#define X_AXIS_PIN 2 // ANALOG
#define Y_AXIS_PIN 1
#define Z_AXIS_PIN 0
#define MESSANZAHL 1
int  value_x, value_y, value_z;

void setup() {
  Serial.begin(57600);
  setSqrtVal();
  
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
  initBitmaps(); //like pacman
  randomSeed(1337); //TODO
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
      for (int x = CUBEWIDTH - 1; x >= 0; x--){
        pushValues(!(bitmap[x][0][z] > brightness),
          !(bitmap[7-x][1][z] > brightness),
          !(bitmap[x][2][z] > brightness),
          !(z == (7-x)));
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



