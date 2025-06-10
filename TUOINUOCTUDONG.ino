#define BLYNK_TEMPLATE_ID "TMPL6CybEosPq"
#define BLYNK_TEMPLATE_NAME "he thong tuoi nuoc tu dong"
#define BLYNK_AUTH_TOKEN "Hy-LQdCYrtxZtMd51VbjQjZr1BhSQBVe"

#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>  

char ssid[] = "CUONG-HIEP"; 
char pass[] = "46552003"; 
BlynkTimer timer;

#define ledr D5 
#define ledg D6 
#define ledy D7 
#define analogPin A0 
#define relay D2 

int value1, analogVal, button, Ref1, Ref2; 
int phantram; 

WidgetLED appledr(V10); 
WidgetLED appledg(V11); 
WidgetLED appledy(V12); 

// 🛠 Đồng bộ dữ liệu từ Blynk ngay khi thiết bị kết nối
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V2, V3, V4, V9);
}

// 🛠 Đọc cảm biến và gửi dữ liệu lên Blynk
void sendSensorData() {
  analogVal = analogRead(analogPin);
  phantram = map(analogVal, 0, 1023, 100, 0);
  
  Blynk.virtualWrite(V1, phantram);  

  if (value1 == 1) { 
    if (phantram > Ref1) { 
      appledr.off(); appledg.off(); appledy.on(); 
      digitalWrite(ledr, LOW); 
      digitalWrite(ledg, LOW); 
      digitalWrite(ledy, HIGH); 
      digitalWrite(relay, LOW); 
      Blynk.virtualWrite(V6, "Độ ẩm cao");  
    } 
    else if (phantram < Ref2) { 
      appledr.on(); appledg.off(); appledy.off(); 
      digitalWrite(ledr, HIGH); 
      digitalWrite(ledg, LOW); 
      digitalWrite(ledy, LOW); 
      digitalWrite(relay, HIGH);  
      Blynk.virtualWrite(V6, "Đang tưới nước");  
    } 
    else { 
      appledr.off(); appledg.on(); appledy.off(); 
      digitalWrite(ledr, LOW); 
      digitalWrite(ledg, HIGH); 
      digitalWrite(ledy, LOW); 
      digitalWrite(relay, LOW);  
      Blynk.virtualWrite(V6, "Độ ẩm BT");  
    } 
  } 
  else { 
    if (button == 1) { 
      digitalWrite(relay, HIGH); 
      appledr.on(); appledg.on(); appledy.off(); 
      digitalWrite(ledr, HIGH); 
      digitalWrite(ledg, HIGH); 
      digitalWrite(ledy, LOW); 
    } 
    else { 
      digitalWrite(relay, LOW);  
      appledr.off(); appledg.on(); appledy.on(); 
      digitalWrite(ledr, LOW); 
      digitalWrite(ledg, HIGH); 
      digitalWrite(ledy, HIGH); 
    }  
  } 
}

void setup() { 
  Serial.begin(115200); 
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); 

  pinMode(ledr, OUTPUT); 
  pinMode(ledy, OUTPUT); 
  pinMode(ledg, OUTPUT); 
  pinMode(relay, OUTPUT); 

  timer.setInterval(300L, sendSensorData);  
}

BLYNK_WRITE(V2) { value1 = param.asInt(); } 
BLYNK_WRITE(V3) { Ref1 = param.asInt(); } 
BLYNK_WRITE(V4) { Ref2 = param.asInt(); } 
BLYNK_WRITE(V9) { button = param.asInt(); } 


void loop() { 
  Blynk.run(); 
  timer.run();  
}