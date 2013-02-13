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

// push low or high to first bit of storage register, moving previous values onwards
void pushValues(boolean value0, boolean value1, boolean value2, boolean value3){
  digitalWrite(shiftDataPin[0], value0);
  digitalWrite(shiftDataPin[1], value1);
  digitalWrite(shiftDataPin[2], value2);
  digitalWrite(shiftDataPin[3], value3);
  digitalWrite(STORAGE_REG_CLOCK, LOW);
  digitalWrite(STORAGE_REG_CLOCK, HIGH);
}

// show current values in storage register on parallel output lines
void showValues(void){
  digitalWrite(SHIFT_REG_CLOCK, LOW);
  digitalWrite(SHIFT_REG_CLOCK, HIGH);
}


