#include<ESP8266WiFi.h>
#include<espnow.h>
const int button1 = 5;
const int button2 = 4;
const int button3 = 0;

#define MY_NAME         "CONTROLLER_NODE"
#define MY_ROLE         ESP_NOW_ROLE_CONTROLLER         // set the role of this device: CONTROLLER, SLAVE, COMBO
#define RECEIVER_ROLE   ESP_NOW_ROLE_SLAVE              // set the role of the receiver
#define WIFI_CHANNEL    1

uint8_t receiverAddress1[] = {0x00,0X00,0x00,0x00,0x00,0x00};   // please update this with the MAC address of the receiver
uint8_t receiverAddress2[] = {0x00,0x00,0x00,0x00,0x00,0x00};   // please update this with the MAC address of the receiver
uint8_t receiverAddress3[] = {0x00,0x00,0x00,0x00,0x00,0x00};   // please update this with the MAC address of the receiver


struct __attribute__((packed)) dataPacket {
  int state ;


};

void transmissionComplete(uint8_t *receiver_mac, uint8_t transmissionStatus) {
  if(transmissionStatus == 0) {
    Serial.println("Data sent successfully");
  } else {
    Serial.print("Error code: ");
    Serial.println(transmissionStatus);
  }
}

void setup() {
  pinMode(button1,INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP); 
  pinMode(button3,INPUT_PULLUP);
  Serial.begin(115200);     // initialize serial port
  
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.print("Initializing...");
  Serial.println(MY_NAME);
  Serial.print("My MAC address is: ");
  Serial.println(WiFi.macAddress());
 
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();        // we do not want to connect to a WiFi network

  if(esp_now_init() != 0) {
    Serial.println("ESP-NOW initialization failed");
    return;
  }

  esp_now_set_self_role(MY_ROLE);   
  esp_now_register_send_cb(transmissionComplete);   // this function will get called once all data is sent
  esp_now_add_peer(receiverAddress1, RECEIVER_ROLE, WIFI_CHANNEL, NULL, 0);
  esp_now_add_peer(receiverAddress2, RECEIVER_ROLE, WIFI_CHANNEL, NULL, 0);
  esp_now_add_peer(receiverAddress3, RECEIVER_ROLE, WIFI_CHANNEL, NULL, 0);

  Serial.println("Initialized.");
}

void loop() {
 
  dataPacket packet;

  packet.state = digitalRead(button1);
  esp_now_send(receiverAddress1, (uint8_t *) &packet, sizeof(packet));

  packet.state = digitalRead(button2);
  esp_now_send(receiverAddress2, (uint8_t *) &packet, sizeof(packet));

  packet.state = digitalRead(button3);
  esp_now_send(receiverAddress3, (uint8_t *) &packet, sizeof(packet));


  delay(30);
}
