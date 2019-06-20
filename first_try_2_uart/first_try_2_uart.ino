#include "BenchmarkConfig.h"
#include "BenchmarkTestCase.h"
#include "BenchmarkSender.h"
#include "BenchmarkReceiver.h"

#include "PacketSerial.h"
int pushButton = 31;
unsigned long lastButtonPressTime = 0;
unsigned long BUTTON_DEBOUNCE_TIME = 200;
int buttonState = 1;
int lastButtonState = 1;

unsigned char sendBuffer[1024];

PacketSerial myPacketSerial;
/*
 * Define the test cases
 */
#define TEST_CASE_COUNT 3
BenchmarkTestCase testCases[TEST_CASE_COUNT] =
{
  // Name, packet size, packet count, interval, buffer
  BenchmarkTestCase("My Test case 1", 1, 20, 100, sendBuffer),
  BenchmarkTestCase("My Test case 2", 100, 200, 10, sendBuffer),
  BenchmarkTestCase("My Test case 3", 1024, 200, 10, sendBuffer)
};

/*
 * Wrapped callbacks
 */

BenchmarkSendVerdict_t mySendFunction(unsigned char* pBuffer, unsigned int size)
{
    myPacketSerial.send(pBuffer, size);
    return BENCHMARK_SEND_PASS;
}

/*
 * Sender and Receiver
 */
BenchmarkReceiver myReceiver(testCases, TEST_CASE_COUNT);
BenchmarkSender mySender(testCases, TEST_CASE_COUNT, &(mySendFunction), &(delay), &(millis), 2000);


void setup() {
  Serial.begin(115200); //virtual com
  Serial2.begin(9600); //Rx = PD6; Tx = PD7
  myPacketSerial.setStream(&Serial2);
  myPacketSerial.setPacketHandler(&onPacketReceived);
  populateBuffer(sendBuffer, 500);
  lastButtonPressTime = millis();
  Serial2.setReceiveCallback(&serial2Interrupt);
  pinMode(pushButton, INPUT_PULLUP);
}

int receiverCounter = 0;

void loop() {
  buttonState = digitalRead(pushButton);
  if ((buttonState == 0) && (lastButtonState == 1) 
        && (millis() - lastButtonPressTime > BUTTON_DEBOUNCE_TIME))
  {
    mySender.runSend();
  }
  myPacketSerial.update();
}

void serial2Interrupt()
{
  myPacketSerial.update();
}

void onPacketReceived(const uint8_t* buffer, size_t size)
{
  myReceiver.receive((unsigned char *) buffer, size);
}

void populateBuffer(unsigned char * buffer, unsigned int size)
{
  for (unsigned int i = 0; i < size; ++i)
  {
    buffer[i] = (unsigned char) (i & 0xFF);
  }
}
