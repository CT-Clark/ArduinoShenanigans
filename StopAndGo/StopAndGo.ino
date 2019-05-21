#define potLED 6
#define LRED 9
#define GREEN 10
#define RRED 11
#define BUTTON 12
#define pot A0

int switchstate = 0;
int buttonHeld = 0;
float brightness = 0;
float fade = 15;
float fadeMod = 1;
int sensorValue = 0;
int sensorMin = 1023;
int sensorMax = 0;

void setup() {
  // Setup calibration light
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  pinMode(potLED, OUTPUT);
  pinMode(LRED, OUTPUT);
  pinMode(RRED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  
  pinMode(BUTTON, INPUT);

  // Spend 5 seconds calibrating the pot light
  while (millis() < 3000) {
    sensorValue = analogRead(pot);

    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
    }

    if (sensorValue < sensorMin && sensorValue > 20 ) {
      sensorMin = sensorValue;
    }
  }

  // Opens up communication with the terminal
  Serial.begin(9600);

  // Turn off the calibration light
  digitalWrite(13, LOW);
}

void loop() {
  // Monitors the state of the pot
  int val = analogRead(pot);
  Serial.print("Sensor = ");
  Serial.print(val);
  val = map(val, sensorMin, sensorMax, 0, 255);
  val = constrain(val, 0, 255);
  Serial.print("\t Output = ");
  Serial.println(val);
  analogWrite(potLED, val);

  // Used so that the state is changed every time the button is pressed without noise
  if(digitalRead(BUTTON) == HIGH && buttonHeld == 0) {
    buttonHeld = 1;
    if(switchstate == 0){
      switchstate = 1;
    } else {
      switchstate = 0;
    }
    delay(1);
  } else if (digitalRead(BUTTON) == LOW && buttonHeld == 1) {
    buttonHeld = 0;
  }

  // State of the LEDs
  if(switchstate == 0) {
    digitalWrite(GREEN,HIGH);
    digitalWrite(LRED,LOW);
    digitalWrite(RRED,LOW);
  } else {
    digitalWrite(GREEN,LOW);
    analogWrite(LRED,brightness);
    analogWrite(RRED,255-brightness);
  }

  
  brightness = brightness + fade;

  val = analogRead(pot);
  val = map(val, 0, 1023, 0, 50);
  if (fade > 0) {
    fade = val;
  } else {
    fade = -val;
  }

  if(fade >= 0 && brightness >= 255 - fade) {
    fade = -fade;
    delay(20);
  } else if (fade <= 0 && brightness <= 0 - fade) {
    fade = -fade;
    delay(20);
  }
  
  Serial.println(fade);
  delay(2);
}
