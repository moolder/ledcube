/* --- bitmap calculations --- */

int selectedDemo = -1;

void initBitmap(){
  for (int z = 0; z < LAYERNUM; z++){
    for (int y = 0; y < CUBEHEIGHT; y++){
      for (int x = 0; x < CUBEWIDTH; x++){
        bitmap[x][y][z] = 0;
      }
    }
  }
}

int ledPos = 0;
void updateBitmap(long treal){

  long demoSwitch = (treal / 30000) % 6;
  
  showDemoSwitch(demoSwitch);
  
  for (int ledCount = 0; ledCount < 4; ledCount++){
  
    if (selectedDemo == -1){
      if (demoSwitch == 0) updateBitmapPacMan(treal);
      if (demoSwitch == 1) updateBitmapRandom2(treal); // pfeile
      if (demoSwitch == 2) updateBitmapPlane1(treal, ledPos); // schon sehr cool
      if (demoSwitch == 3) updateBitmapRandom1(treal); // blink blink
      if (demoSwitch == 4) updateBitmapPlane3(treal, ledPos, true, true); // rotierende und verschiebende Ebene
      if (demoSwitch == 5) updateBitmapPlane2(treal, ledPos);
      //if (demoSwitch == 7) updateBitmapTestButtons(treal, ledPos);
      if (demoSwitch == 6) updateBitmapPlane3(treal, ledPos, false, true); // rotierende Ebene um Mitte
      if (demoSwitch == 7) updateBitmapTestPattern(treal); // halt ein testmuster
      if (demoSwitch == 8) updateBitmapAllOn(treal); // alle leds an. immer.
      if (demoSwitch == 9) updateBitmapPlane3(treal, ledPos, true, false); // verschiebende Ebene
    } else {
      if (selectedDemo == 0) updateBitmapPacMan(treal);
      if (selectedDemo == 1) updateBitmapRandom2(treal); // pfeile
      if (selectedDemo == 2) updateBitmapPlane1(treal, ledPos); // schon sehr cool
      if (selectedDemo == 3) updateBitmapRandom1(treal); // blink blink
      if (selectedDemo == 4) updateBitmapPlane3(treal, ledPos, true, true); // rotierende und verschiebende Ebene
      if (selectedDemo == 5) updateBitmapPlane2(treal, ledPos);
      //if (selectedDemo == 7) updateBitmapTestButtons(treal, ledPos);
      if (selectedDemo == 6) updateBitmapPlane3(treal, ledPos, false, true); // rotierende Ebene um Mitte
      if (selectedDemo == 7) updateBitmapTestPattern(treal); // halt ein testmuster
      if (selectedDemo == 8) updateBitmapAllOn(treal); // alle leds an. immer.
      if (selectedDemo == 9) updateBitmapPlane3(treal, ledPos, true, false); // verschiebende Ebene
    }
  
  
    // updateBitmapSinus1(treal); // noch hässlich
  
    ledPos++;
    if (ledPos >= CUBEWIDTH * LAYERNUM){
      ledPos = 0;
    }
  }
}

long trealModOld = 99999;

int deltaX = 0;
int deltaY = 0;
void updateBitmapPacMan(long treal){
  int deltaX = int((treal % 8000) / 500 - 8);
  int deltaY = 0; //deltaY = int((treal % 16000) / 1000 - 8);
  int animNr = int((treal % (3*8000)) / 8000);
  animateBitmap(treal, animNr, deltaX, deltaY, true);
}

void animateBitmap(long treal, int animNr, int deltaX, int deltaY, boolean drawBlack){
  int r = 0;
  int g = 0;
  int b = 0;
  int deltaFrame = 0;
  if (animNr == 0){
    r = 255;
    g = 255;
    b = 0;
    deltaFrame = 0;
  } else if (animNr == 1){
    r = 0;
    g = 0;
    b = 255;
    deltaFrame = 2;
  } else if (animNr == 2){
    r = 255;
    g = 0;
    b = 255;
    deltaFrame = 2;
  }
  int frame = 0;
  if ((treal % 500) > 250) frame = 1;
  for (int z = 0; z < LAYERNUM; z++) {
    for (int x = 0; x < CUBEWIDTH; x++) {
      long shiftLeftValue = ((7-x)+deltaX);
      if ((shiftLeftValue >= 0) && (z+deltaY > -1) && (z+deltaY < 8) && (pacman[frame+deltaFrame][z+deltaY] & 1<<shiftLeftValue) > 0){
        bitmap[x][0][z] = r;
        bitmap[x][1][z] = g;
        bitmap[x][2][z] = b;
      } else if (drawBlack) {
        bitmap[x][0][z] = 0;
        bitmap[x][1][z] = 0;
        bitmap[x][2][z] = 0;
      }
    }

  }
}  

void updateBitmapAllOn(long treal){
  for (int z = 0; z < LAYERNUM; z++) {
    for (int y = 0; y < CUBEHEIGHT; y++) {
      for (int x = 0; x < CUBEWIDTH; x++) {
        bitmap[x][y][z] = 255;
      }
    }
  }
}  
/*
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
  */

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
    int rX = (int)random(CUBEWIDTH);
    int rY = (int)random(CUBEHEIGHT);
    int rZ = (int)random(LAYERNUM);
    int rWay = (int)random(3);
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
    bitmap[(int)random(CUBEWIDTH)][(int)random(CUBEHEIGHT)][(int)random(LAYERNUM)] = 255;
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
  long tMod = t % 84;
  
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


