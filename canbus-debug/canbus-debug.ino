int canH = 10;
int canL = 9;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(125000);
  pinMode(canH, INPUT);
  pinMode(canL, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int canHData = digitalRead(canH);
  int canLData = digitalRead(canL);
  Serial.printf("CanH: %d -- canL %d \n", canHData, canLData);
  delay(1);
}
