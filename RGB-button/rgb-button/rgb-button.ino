#define RED 8
#define GREEN 9
#define BLUE 10
#define BUTTON 3
int buttonPressed = 0;
void testColors(int transitionDelay = 500) {
  Serial.println("red start");
  digitalWrite(RED, HIGH);
  delay(transitionDelay);
  digitalWrite(RED, LOW);
  Serial.println("red end");
  delay(transitionDelay);
  Serial.println("GREEN start");
  digitalWrite(GREEN, HIGH);
  delay(transitionDelay);
  digitalWrite(GREEN, LOW);
  Serial.println("GREEN end");
  delay(transitionDelay);
  Serial.println("BLUE start");
  digitalWrite(BLUE, HIGH);
  delay(transitionDelay);
  digitalWrite(BLUE, LOW);
  Serial.println("BLUE end");
  delay(transitionDelay); 
}

void setup() {
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  testColors(500);
//  buttonPressed = digitalRead(BUTTON);
//  Serial.printf("button state: %d \n", buttonPressed);
//  if (buttonPressed == LOW) {
//    digitalWrite(GREEN, HIGH);
//    digitalWrite(RED, LOW);
//  } else {
//    digitalWrite(GREEN, LOW);
//    digitalWrite(RED, HIGH);
//  }
//  delay(60);
}
