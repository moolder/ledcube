// ledcube: arduino code for a led cube (optimised for 4x4x4) using 2 shift registers (74595) for layer data control and 4 pins for layer switching
// this version (_05) tries to create better performance by losing sqrt calculations for the distance calc
// license: Creative Commons Non-Commercial Attribution Share-Alike

// pin definitions:

// DS:
#define SHIFT_DATA 2

// STCP:
#define STORAGE_REG_CLOCK 4

// SHCP:
#define SHIFT_REG_CLOCK 3

// cube geometry
#define CUBEWIDTH 4
#define CUBEHEIGHT 4
#define LAYERNUM 4

int bitmap[CUBEWIDTH][CUBEHEIGHT][LAYERNUM];
int layerPin[LAYERNUM];


#define LED_PIN 13

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

  
  layerPin[0] = 5;
  layerPin[1] = 6;
  layerPin[2] = 7;
  layerPin[3] = 8;

  // set the digital pin as output:
  pinMode(SHIFT_DATA, OUTPUT);      
  pinMode(SHIFT_REG_CLOCK, OUTPUT);      
  pinMode(STORAGE_REG_CLOCK, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_1_PIN, INPUT);
  pinMode(BUTTON_2_PIN, INPUT);
  
  for (int z = 0; z < LAYERNUM; z++){
    pinMode(layerPin[z], OUTPUT);
  }
  digitalWrite(SHIFT_DATA, LOW);      
  digitalWrite(SHIFT_REG_CLOCK, HIGH);      
  digitalWrite(STORAGE_REG_CLOCK, HIGH);
  digitalWrite(LED_PIN, HIGH);
  for (int z = 0; z < LAYERNUM; z++){
    digitalWrite(layerPin[z], LOW);
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

int selectedDemo = -1;

void loop(){
  
  oldButton1Pushed = button1Pushed;
  oldButton2Pushed = button2Pushed;
  
  button1Pushed = digitalRead(BUTTON_1_PIN);
  button2Pushed = digitalRead(BUTTON_2_PIN);

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
  
  if (oldButton1Pushed == LOW && button1Pushed){
    selectedDemo++;
    if (selectedDemo > 9) selectedDemo = -1;
  }
  
  for (int z = 0; z < LAYERNUM; z++){
    setLayer(-1);
    for (int brightness = 32; brightness < 256; brightness += 32){
      for (int y = 0; y < CUBEHEIGHT; y++){
        for (int x = 0; x < CUBEWIDTH; x++){
          if (bitmap[x][y][z] < brightness){
            pushValue(LOW);
          } else {
            pushValue(HIGH);
            }
        }
      }
      showValues();
      setLayer(z);
      delayMicroseconds(2000/12);
    }
    
    ledpinValue = !ledpinValue;
    digitalWrite(LED_PIN, ledpinValue);

    updateBitmap(millis());
  }  
}


/* --- bitmap calculations --- */

void initBitmap(){
  for (int z = 0; z < LAYERNUM; z++){
    for (int y = 0; y < CUBEHEIGHT; y++){
      for (int x = 0; x < CUBEWIDTH; x++){
        bitmap[x][y][z] = 255;
      }
    }
  }
}

int ledPos = 0;
void updateBitmap(long treal){

  int demoSwitch = (treal / 30000) % 7;
  
  //demoSwitch = 7; // TODO 
  
  if (selectedDemo == -1){
    if (demoSwitch == 0) updateBitmapPlane3(treal, ledPos, false, true); // rotierende Ebene um Mitte
    if (demoSwitch == 1) updateBitmapRandom2(treal); // pfeile
    if (demoSwitch == 2) updateBitmapPlane3(treal, ledPos, true, false); // verschiebende Ebene
    if (demoSwitch == 3) updateBitmapPlane1(treal, ledPos); // schon sehr cool
    if (demoSwitch == 4) updateBitmapRandom1(treal); // blink blink
    if (demoSwitch == 5) updateBitmapPlane3(treal, ledPos, true, true); // rotierende und verschiebende Ebene
    if (demoSwitch == 6) updateBitmapPlane2(treal, ledPos);
    if (demoSwitch == 7) updateBitmapTestButtons(treal, ledPos);
    if (demoSwitch == 8) updateBitmapTestPattern(treal); // halt ein testmuster
    if (demoSwitch == 9) updateBitmapAllOn(treal); // alle leds an. immer.
  } else {
    if (selectedDemo == 0) updateBitmapPlane3(treal, ledPos, false, true); // rotierende Ebene um Mitte
    if (selectedDemo == 1) updateBitmapRandom2(treal); // pfeile
    if (selectedDemo == 2) updateBitmapPlane3(treal, ledPos, true, false); // verschiebende Ebene
    if (selectedDemo == 3) updateBitmapPlane1(treal, ledPos); // schon sehr cool
    if (selectedDemo == 4) updateBitmapRandom1(treal); // blink blink
    if (selectedDemo == 5) updateBitmapPlane3(treal, ledPos, true, true); // rotierende und verschiebende Ebene
    if (selectedDemo == 6) updateBitmapPlane2(treal, ledPos);
    if (selectedDemo == 7) updateBitmapTestButtons(treal, ledPos);
    if (selectedDemo == 8) updateBitmapTestPattern(treal); // halt ein testmuster
    if (selectedDemo == 9) updateBitmapAllOn(treal); // alle leds an. immer.
  }


  // updateBitmapSinus1(treal); // noch hässlich

  ledPos++;
  if (ledPos >= CUBEWIDTH * LAYERNUM){
    ledPos = 0;
  }
  
}

long trealModOld = 99999;

void updateBitmapAllOn(long treal){
  for (int z = 0; z < LAYERNUM; z++) {
    for (int y = 0; y < CUBEHEIGHT; y++) {
      for (int x = 0; x < CUBEWIDTH; x++) {
        bitmap[x][y][z] = 255;
      }
    }
  }
}  

void updateBitmapTestButtons(long treal, int ledPos){
  boolean button1Pushed = digitalRead(BUTTON_1_PIN);
  boolean button2Pushed = digitalRead(BUTTON_2_PIN);
  for (int z = 0; z < LAYERNUM; z++) {
    for (int y = 0; y < CUBEHEIGHT; y++) {
      for (int x = 0; x < CUBEWIDTH; x++) {
        if (button1Pushed && x < CUBEWIDTH / 2){
          bitmap[x][y][z] = 255;
        } else {
          bitmap[x][y][z] = 0;
        }
        if (button2Pushed && x >= CUBEWIDTH / 2){
          bitmap[x][y][z] = 255;
        } else {
          bitmap[x][y][z] = 0;
        }
      }
    }
  }
}
  

void updateBitmapRandom2(long treal){
  for (int z = 0; z < LAYERNUM; z++) {
    for (int y = 0; y < CUBEHEIGHT; y++) {
      for (int x = 0; x < CUBEWIDTH; x++) {
        if (bitmap[x][y][z] > 0){
          bitmap[x][y][z]--;
        }
      }
    }
  }
  if (trealModOld > treal % 250){
    int rX = random(CUBEWIDTH);
    int rY = random(CUBEHEIGHT);
    int rZ = random(LAYERNUM);
    int rWay = random(3);
    int runvar;
    if (rWay == 0){
      for (runvar = 0; runvar < LAYERNUM; runvar++){
        bitmap[rX][rY][runvar] = 255;
      }
    } else if (rWay == 1){
      for (runvar = 0; runvar < CUBEHEIGHT; runvar++){
        bitmap[rX][runvar][rZ] = 255;
      }
    } else {
      for (runvar = 0; runvar < CUBEWIDTH; runvar++){
        bitmap[runvar][rY][rZ] = 255;
      }
    }
  }
  trealModOld = treal % 250;
}
  

void updateBitmapRandom1(long treal){
  for (int z = 0; z < LAYERNUM; z++) {
    for (int y = 0; y < CUBEHEIGHT; y++) {
      for (int x = 0; x < CUBEWIDTH; x++) {
        if (bitmap[x][y][z] > 0){
          bitmap[x][y][z]--;
        }
        //if (bitmap[x][y][z] > 255){
        //  bitmap[x][y][z] = 0;
        //}
      }
    }
  }
  if (trealModOld > treal % 50){
    bitmap[random(CUBEWIDTH)][random(CUBEHEIGHT)][random(LAYERNUM)] = 255;
  }
  trealModOld = treal % 50;
}
  

void updateBitmapSinus1(long treal){
  for (int z = 0; z < LAYERNUM; z++) {
    for (int y = 0; y < CUBEHEIGHT; y++) {
      for (int x = 0; x < CUBEWIDTH; x++) {
        bitmap[x][y][z] = int(min(255, max(0, sin(1.0F * (treal + z * 1000) / 3140)*255)));
      }
    }
  }
}

// equation from http://easycalculation.com/analytical/shortest-distance-between-point-plane.php
// plus cross product calc for vector calc

float a = -1;
float b = 0;
float c = 0;
float d = 0;

void updateBitmapTestPattern(long treal){
  long t = treal / 1000;
  int tMod = t % 84;
  
  for (int z = 0; z < LAYERNUM; z++) {
    for (int y = 0; y < CUBEHEIGHT; y++) {
      for (int x = 0; x < CUBEWIDTH; x++) {
        if (tMod < 64){
          if (x + CUBEWIDTH * y + CUBEWIDTH * CUBEHEIGHT * z == tMod){
            bitmap[x][y][z] = 255;
          } else {
            bitmap[x][y][z] = 0;
          }
        } else if(tMod >= 64 && tMod < 68) {
          if (x == tMod - 64){
            bitmap[x][y][z] = 255;
          } else {
            bitmap[x][y][z] = 0;
          }
        } else if(tMod >= 68 && tMod < 72) {
          if (y == tMod - 68){
            bitmap[x][y][z] = 255;
          } else {
            bitmap[x][y][z] = 0;
          }
        } else if(tMod >= 72 && tMod < 76) {
          if (z == tMod - 72){
            bitmap[x][y][z] = 255;
          } else {
            bitmap[x][y][z] = 0;
          }
        } else if(tMod >= 76 && tMod < 80) {
          bitmap[x][y][z] = 255;
        } else if(tMod >= 80 && tMod < 84) {
          bitmap[x][y][z] = 0;
        }

      }
    }
  }
}

void updateBitmapPlane3(long treal, int ledPos, boolean doMove, boolean doRotate){
  long t = treal / 1000;
  //int v = 0;

  float v0 = 1.5; // center
  if (doMove) { // do move
    //v0 = 0.0005 * (treal % 24000) - 1;
    v0 = 0.0002 * (treal % 60000) - 1;
    if (v0 > 5) v0 = 10 - v0;
  }
  
  float v1x = 0;
  float v1y = 1;  
  float v1z = 0;  

  if (doRotate) { // do rotate
    v1x = sin(0.0005*treal);
    v1y = cos(0.0005*treal);
  }
  
  
  float v2x = 0;
  float v2y = 0;
  float v2z = 1;
  
  // normal vector calculation
  float v3x = crossProductX(v1x,v1y,v1z,v2x,v2y,v2z);
  float v3y = crossProductY(v1x,v1y,v1z,v2x,v2y,v2z);
  float v3z = crossProductZ(v1x,v1y,v1z,v2x,v2y,v2z);

  a = v3x;
  b = v3y;
  c = v3z;
  d = (v3x*v0 + v3y * v0 + v3z * v0) * -1;


  //float sharpness = 0.6; // 0.5 for sharpest, 1 for smoother. 0.6 seems nice
  for (int z = 0; z < LAYERNUM; z++) {
    for (int y = 0; y < CUBEHEIGHT; y++) {
      for (int x = 0; x < CUBEWIDTH; x++) {
        if (x + CUBEWIDTH * z == ledPos){
          float distance = sqrttable(distanceSquared(a, b, c, d, x, y, z));
          bitmap[x][y][z] = int(min(255, max(0, 0.65 - distance)*255 / 0.65));
        }
      }
    }
  }
}


// equation from http://easycalculation.com/analytical/shortest-distance-between-point-plane.php
void updateBitmapPlane2(long treal, int ledPos){
  long t = treal / 1000;
  int v = 0;
  
  float b = cos(1.0F * treal / 3140);
  float a = 0;
  float c = sin(1.0F * treal / 3140);
  float d = 0;

  for (int z = 0; z < LAYERNUM; z++) {
    for (int y = 0; y < CUBEHEIGHT; y++) {
      for (int x = 0; x < CUBEWIDTH; x++) {
        if (x + CUBEWIDTH * z == ledPos){
          float distance = sqrttable(distanceSquared(a, b, c, d, x, y, z));
          bitmap[x][y][z] = int(min(255, max(0, 2 - distance)*128));
        }
      }
    }
  }
}

void updateBitmapPlane1(long treal, int ledPos){
  long t = treal / 1000;
  int v = 0;
  
  if (trealModOld > treal % 8000){
    a = random(3)-1;
    b = random(3)-1;
    c = random(3)-1;
  }
  trealModOld = treal % 8000;
  
  float d = (3 - 1.0F * (treal % 8000)/500);

  for (int z = 0; z < LAYERNUM; z++) {
    for (int y = 0; y < CUBEHEIGHT; y++) {
      for (int x = 0; x < CUBEWIDTH; x++) {
        if (x + CUBEWIDTH * z == ledPos){
          float distance = sqrttable(distanceSquared(a, b, c, d, x, y, z));
          bitmap[x][y][z] = int(min(255, max(0, 1 - distance)*255));
          //bitmap[x][y][z] = bitmap[x][y][z]  * bitmap[x][y][z] / 255;
        }
      }
    }
  }
}

/* --- matrix helper functions --- */

// old distance calc: float distance = abs(a*x + b*y + c*z + d) / sqrt(a*a + b*b + c*c);
// this returns the squared value of that
float distanceSquared(float a, float b, float c, float d, int x, int y ,int z){
  float divisor = a*x + b*y + c*z + d;
  return (divisor * divisor) / (a*a + b*b + c*c);
}

float sqrttable(float invalueF){
  int invalue = int(100 * invalueF);
  if (invalue <= 0) return 0.0;
  if (invalue <= 5) return 0.2236068;
  if (invalue <= 10) return 0.31622776;
  if (invalue <= 15) return 0.38729832;
  if (invalue <= 20) return 0.4472136;
  if (invalue <= 25) return 0.5;
  if (invalue <= 30) return 0.5477225;
  if (invalue <= 35) return 0.591608;
  if (invalue <= 40) return 0.6324555;
  if (invalue <= 45) return 0.67082036;
  if (invalue <= 50) return 0.70710677;
  if (invalue <= 55) return 0.7416199;
  if (invalue <= 60) return 0.77459663;
  if (invalue <= 65) return 0.8062258;
  if (invalue <= 70) return 0.83666;
  if (invalue <= 75) return 0.8660254;
  if (invalue <= 80) return 0.8944272;
  if (invalue <= 85) return 0.92195445;
  if (invalue <= 90) return 0.94868326;
  if (invalue <= 95) return 0.9746794;
  if (invalue <= 100) return 1.0;
  if (invalue <= 110) return 1.0488088;
  if (invalue <= 120) return 1.095445;
  if (invalue <= 130) return 1.1401755;
  if (invalue <= 140) return 1.183216;
  if (invalue <= 150) return 1.2247449;
  if (invalue <= 160) return 1.264911;
  if (invalue <= 170) return 1.3038404;
  if (invalue <= 180) return 1.3416407;
  if (invalue <= 190) return 1.3784049;
  if (invalue <= 200) return 1.4142135;
  if (invalue <= 210) return 1.4491377;
  if (invalue <= 220) return 1.4832398;
  if (invalue <= 230) return 1.5165751;
  if (invalue <= 240) return 1.5491933;
  if (invalue <= 250) return 1.5811388;
  if (invalue <= 260) return 1.6124516;
  if (invalue <= 270) return 1.6431677;
  if (invalue <= 280) return 1.67332;
  if (invalue <= 290) return 1.7029386;
  return 1.7320508;
}
    

float crossProductX(float x1, float y1, float z1, float x2, float y2, float z2){
  return y1*z2 - z1*y2;
}

float crossProductY(float x1, float y1, float z1, float x2, float y2, float z2){
  return z1*x2 - x1*z2;
}

float crossProductZ(float x1, float y1, float z1, float x2, float y2, float z2){
  return x1*y2 - y1*x2;
}


/* --- low level functions --- */

void setLayer(int layer){
  for (int z = 0; z < LAYERNUM; z++){
    if (z == layer)
      digitalWrite(layerPin[z], HIGH);
    else
      digitalWrite(layerPin[z], LOW);
  }
}

// push low or high to first bit of storage register, moving previous values onwards
void pushValue(boolean value){
  digitalWrite(SHIFT_DATA, value);
  digitalWrite(STORAGE_REG_CLOCK, LOW);
  digitalWrite(STORAGE_REG_CLOCK, HIGH);
}

// show current values in storage register on parallel output lines
void showValues(void){
  digitalWrite(SHIFT_REG_CLOCK, LOW);
  digitalWrite(SHIFT_REG_CLOCK, HIGH);
}


