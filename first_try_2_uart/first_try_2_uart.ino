
int pushButton = 31;
int stringNumber = 0;
String inputString = ""; // a string to hold incoming data

void setup() {
  Serial.begin(115200); //virtual com
  Serial2.begin(9600); //Rx = PD6; Tx = PD7
  Serial2.setReceiveCallback(&serial2Interrupt);
  pinMode(pushButton, INPUT_PULLUP);
}

int receiverCounter = 0;

void loop() {
  int buttonState = digitalRead(pushButton);
  if (buttonState == 0) {
    receiverCounter = 0;
    for (int i = 0; i < 100; ++i) {
      Serial2.print("H");
      //Serial.println(i);
    }
  }

}

void serial2Interrupt()
{
  while (Serial2.available())
  {
    char thisChar = Serial2.read();
    Serial.print(++receiverCounter);
    Serial.println(thisChar);
  }
}
