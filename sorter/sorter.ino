#include <Servo.h>

#define TRIG 6
#define ECHO 7
#define EN 9
#define IN1 10
#define IN2 11

const float BASE_CM = 100.0;
const float MIN_LEN = 2.0; 
const float SHORT_MAX = 10.0;
const float MED_MAX = 20.0;

Servo diverter;
int shortCount = 0, medCount = 0, longCount = 0;

float getDistance() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long d = pulseIn(ECHO, HIGH, 30000);
  if (d == 0) return -1;
  return d * 0.0343 / 2.0;
}

float getLength() {
  float sum = 0; int n = 0;
  for (int i = 0; i < 5; i++) {
    float d = getDistance();
    if (d > 0) { sum += d; n++; }
    delay(30);
  }
  if (n == 0) return -1;
  return sum / n;
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);
  pinMode(EN, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  diverter.attach(3);
  diverter.write(90);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  analogWrite(EN, 150);
  Serial.println("Sorter ready");
}

void loop() {
  float len = getLength();
  Serial.print("DEBUG len = "); Serial.println(len);
  if (len < MIN_LEN) { delay(200); return; } 
  if (len <= SHORT_MAX) {
    diverter.write(30); shortCount++;
    Serial.print("SHORT");
  } else if (len <= MED_MAX) {
    diverter.write(90); medCount++;
    Serial.print("MEDIUM");
  } else {
    diverter.write(150); longCount++;
    Serial.print("LONG");
  }
  Serial.print(" ("); Serial.print(len); Serial.println(" cm)");

  Serial.print("Short: "); Serial.print(shortCount);
  Serial.print(" | Medium: "); Serial.print(medCount);
  Serial.print(" | Long: "); Serial.print(longCount);
  Serial.print(" | Total: "); Serial.println(shortCount + medCount + longCount);

  delay(1500); 
  diverter.write(90);
  delay(1000);
}
