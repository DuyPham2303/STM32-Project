#define BLYNK_TEMPLATE_ID "TMPL6moGh9nGI"
#define BLYNK_TEMPLATE_NAME "TestLed"
#define BLYNK_AUTH_TOKEN "n7sXq2FbpYEog4wtDEvjFb5jfPHaG5ad"
#define BLYNK_PRINT Serial
#include<BlynkSimpleEsp32.h>
#include<WiFi.h>
#include<WiFiClient.h>
#include<SoftwareSerial.h>
#include<string.h>
#include<cstring>
#define RX 16 
#define TX 17 
SoftwareSerial espserial(RX,TX);
#define  LED1_PIN 12
#define  LED2_PIN 13
#define  LED1_STATUS V0
#define  LED2_STATUS V1
WidgetLED VIRTUAL_LED1(V0);
WidgetLED VIRTUAL_LED2(V1);
int status_led1 = 0,status_led2 = 0;
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Duy Khoa";
char pass[] = "gacon123";
int flag = 0;
void setup(){
    Serial.begin(115200);
    espserial.begin(38400);
    espserial.listen();
    Blynk.begin(auth,ssid,pass);
    pinMode(LED1_PIN,OUTPUT);
    pinMode(LED2_PIN,OUTPUT);
   Serial.println("Blynk connected !");
}
BLYNK_WRITE(LED1_STATUS){
    status_led1 = param.asInt();
    if(status_led1 == 1){
        VIRTUAL_LED1.on();
        //digitalWrite(LED1_PIN,HIGH);
    } 
    else{
        VIRTUAL_LED1.off();
        //digitalWrite(LED1_PIN,LOW);
    }    
    flag = 1;
}
BLYNK_WRITE(LED2_STATUS){
    status_led2 = param.asInt();
    if(status_led2 == 1){
        VIRTUAL_LED2.on();
        //digitalWrite(LED2_PIN,HIGH);
    } 
    else{
        VIRTUAL_LED2.off();
        //digitalWrite(LED2_PIN,LOW);

    }    
    flag = 1;
}
void send_stm32(){
    if(flag == 1){
        String buff_tx = "led 1 status:" + String(status_led1) + '^' + "led 2 status:" + String(status_led2) + '*';
        espserial.print(buff_tx);
        Serial.println(buff_tx);
        flag = 0;
    }
}
void loop(){
  Blynk.run();
  send_stm32();
}
