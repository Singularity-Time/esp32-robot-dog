void initHAL()
{
  Serial.print("HAL ");
  initServo();
  initLegs();
  Serial.println();
}

void initLegs() {

}

void updateHAL() {
  if (disableHAL) return;

  updateLegs();
}

void doHAL() {
  servoSet();
}

void readLegsSensors() {
  // TODO enable if sensors available
  // TODO don't set to legs, this should be part of gait
  for (int i = 0; i < LEG_NUM; i++) {
    //legs[i].sensor.onGround = analogRead(legs[i].sensor.pin) > legs[i].sensor.threshold;
  }
}

void updateLegs() {
  readLegsSensors();
  // TODO check `isSolved`
  #ifdef DEBUG_HAL_LEG
    Serial.println("LEGLF");
  #endif
  legs[LEGLF].angle = ikLegLF.solve().angle;
  
  #ifdef DEBUG_HAL_LEG
    Serial.println("LEGRF");
  #endif
  legs[LEGRF].angle = ikLegRF.solve().angle;
  
  #ifdef DEBUG_HAL_LEG
    Serial.println("LEGLH");
  #endif
  legs[LEGLH].angle = ikLegLH.solve().angle;

  #ifdef DEBUG_HAL_LEG
    Serial.println("LEGRH");
  #endif
  legs[LEGRH].angle = ikLegRH.solve().angle;
}

double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t angleToPulse(double angleRad) {
  // TODO, angle expected to be between 0 and 180 deg (0 and PI)
  
  double pulse = mapf(angleRad, 0, M_PI, 600, 2400);
  // from Adafruit PWM lib
  double pulselength;
  pulselength = 1000000; // 1,000,000 us per second

  // Read prescale
  uint16_t prescale = pwm.readPrescale();

  // Calculate the pulse for PWM based on Equation 1 from the datasheet section
  // 7.3.5
  prescale += 1;
  pulselength *= prescale;
  pulselength /= pwm.getOscillatorFrequency();
  pulse /= pulselength;

  return pulse;
}

void setLegPWM(leg &_leg)
{
  
  // TODO limits and angle normalize
  pwm.setPWM(_leg.hal.pin.alpha,  0,  angleToPulse(getHALAngle(_leg.angle.alpha, _leg.hal.trim.alpha, _leg.hal.ratio.alpha, _leg.inverse.alpha)));
  pwm.setPWM(_leg.hal.pin.beta,   0,  angleToPulse(getHALAngle(_leg.angle.beta,  _leg.hal.trim.beta,  _leg.hal.ratio.beta,  _leg.inverse.beta)));
  pwm.setPWM(_leg.hal.pin.gamma,  0,  angleToPulse(getHALAngle(_leg.angle.gamma, _leg.hal.trim.gamma, _leg.hal.ratio.gamma, _leg.inverse.gamma)));
}

double getHALAngle(double angle, double trimAngle, double gearRatio, bool inverse) {
  angle = angle + trimAngle;
  if (gearRatio != 1) {
    angle = (angle - M_PI_2) * gearRatio + M_PI_2;  // map around middle of servo (90 deg, PI/2)
  }
  if (inverse) angle = M_PI - angle;
  return angle;
}
