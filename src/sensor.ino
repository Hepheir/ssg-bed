class Motion {
private:
  unsigned long now;
  bool is_motion;
  bool is_insleep;
  unsigned long motion_started_at;
  unsigned long motion_continued_at;

  unsigned char sensor_pin;
  unsigned long sensor_duration;

  bool isMotionContinuable() {
    return (motion_continued_at + sensor_duration) >= now;
  }

  void handleMotionStart() {
    if (is_motion && !isMotionContinuable()) {
      motion_started_at = now;
      motion_continued_at = now;
      is_insleep = false;
    }
  }

  void handleMotionContinue() {
    if (is_motion && isMotionContinuable()) {
      motion_continued_at = now;
      is_insleep = false;
    }
  }

  void handleMotionEnd() {
    if (!is_motion && !isMotionContinuable()) {
      is_insleep = true;
    }
  }

public:
  Motion(unsigned long pin, unsigned long duration) {
    sensor_pin = pin;
    sensor_duration = duration;
    is_insleep = false;
    pinMode(pin, INPUT);
  }

  void update() {
    now = millis();
    is_motion = digitalRead(sensor_pin);

    handleMotionStart();
    handleMotionContinue();
    handleMotionEnd();
  }

  bool isMotion() {
    return is_motion;
  }

  bool isInSleep() {
    return is_insleep;
  }
};

Motion motion = Motion(7, 10000);

void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop() {
  motion.update();

  digitalWrite(3, !motion.isInSleep());
  digitalWrite(8, motion.isMotion());
}