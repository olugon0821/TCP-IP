#include "DHT.h"
#include "HCSR04.h"
#include <Stepper.h>
#include <MFRC522Debug.h> 
#include <MFRC522DriverPinSimple.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522v2.h>

enum CONTROL_PINS{
  TEMPER_HUMID = A0,
  TRIG = 12U,
  ECHO = 11U,
  RGB_RED = 5U,
  RGB_GREEN = 6U,
  RGB_BLUE = 7U
};

const uint16_t STEP_REVOLUTION {2048U};

class DHT dht(TEMPER_HUMID, 11); // dht 객체 생성
class Stepper stepping(STEP_REVOLUTION,42,43,44,45);
class UltraSonicDistanceSensor ul_sn(TRIG, ECHO);
class MFRC522DriverPinSimple sda_pin(53);
class MFRC522DriverSPI driver {sda_pin};
class MFRC522 mfrc522{driver};
const String MASTER_CARD {String("C2A72B1B")};

bool function_state = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200UL);
  dht.begin(); // dht 초기화
  stepping.setSpeed(14);
  mfrc522.PCD_Init();
  Serial.begin(115200UL);
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
  pinMode(TEMPER_HUMID, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  function_state = false;

  Distance();
  delay(100UL);

  if(Serial.available()){
      const String in_comming_data {Serial.readStringUntil('\n')}; // 입력한 문자열을 \n 까지 받음
      if(in_comming_data.equals("DOOR_OPEN")){
        stepping.step(STEP_REVOLUTION / 4);
      }else if(in_comming_data.equals("DOOR_CLOSE")){
        stepping.step(-(STEP_REVOLUTION / 4));
      }else{}
    }

  if(!mfrc522.PICC_IsNewCardPresent()) return;
  if(!mfrc522.PICC_ReadCardSerial()) return;
  String tagID = ""; // 빈문자열
  for(uint8_t i {0U}; i <4; ++i){
    tagID += String(mfrc522.uid.uidByte[i],HEX);
  }
  tagID.toUpperCase(); // 소문자를 대문자로 변환
  mfrc522.PICC_HaltA(); // UID 이외에는 멈춤
  if(tagID == MASTER_CARD){
    Serial.println("ID ALLOWED");
  }else{
    Serial.println("ID IS NOT ALLOWED");
  }

  
}

void Distance() {
  if(dht.read()){
    const float temperature {dht.readTemperature()};
    const float humidity {dht.readHumidity()};
    float distance = ul_sn.measureDistanceCm(temperature);
    const String sending_data {String(temperature) + "," + String(humidity) + "," + String(distance)};

    if (!function_state) {
      if (distance > 0 and distance <= 20) {
        digitalWrite(RGB_BLUE, LOW);
        digitalWrite(RGB_GREEN, LOW);
        digitalWrite(RGB_RED, 1);
      } else if (distance > 20 and distance <= 50) {
        digitalWrite(RGB_BLUE, LOW);
        digitalWrite(RGB_GREEN, 1);
        digitalWrite(RGB_RED, LOW);
      } else if (distance > 50) {
        digitalWrite(RGB_BLUE, 1);
        digitalWrite(RGB_GREEN, LOW);
        digitalWrite(RGB_RED, LOW);
      }
      // 시리얼 출력 추가
      Serial.println(sending_data);
    }
  }
}
