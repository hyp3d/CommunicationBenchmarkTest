#include <PacketSerial.h>

int pushButton = 31;
unsigned long lastButtonPressTime = 0;
unsigned long BUTTON_DEBOUNCE_TIME = 200;
int buttonState = 1;
int lastButtonState = 1;

PacketSerial myPacketSerial;
unsigned char sendBuffer[50];

void setup()
{
  Serial.begin(115200);
  pinMode(pushButton, INPUT_PULLUP);
  Serial2.begin(9600);
  myPacketSerial.setStream(&Serial2);
  myPacketSerial.setPacketHandler(&onPacketReceived);
  populateBuffer(sendBuffer, 50);
  lastButtonPressTime = millis();
}


void loop()
{
  buttonState = digitalRead(pushButton);
  if ((buttonState == 0) && (lastButtonState == 1) 
        && (millis() - lastButtonPressTime > BUTTON_DEBOUNCE_TIME))
  {
    myPacketSerial.send(sendBuffer, 10);
  }
  myPacketSerial.update();
}

void onPacketReceived(const uint8_t* buffer, size_t size)
{
  Serial.print("Receive ");
  Serial.print((unsigned int) size);
  Serial.print(" byte(s)");
  if (memcmp(buffer, sendBuffer, size) == 0)
    Serial.println("PASSED");
  else
    Serial.println("FAILED");
}

void populateBuffer(unsigned char * buffer, unsigned int size)
{
  for (unsigned int i = 0; i < size; ++i)
  {
    buffer[i] = (unsigned char) (i & 0xFF);
  }
}
