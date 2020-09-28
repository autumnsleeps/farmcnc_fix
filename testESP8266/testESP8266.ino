//#include <FirebaseArduino.h>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <SoftwareSerial.h>

SoftwareSerial MySerial(4, 5); //RX,TX = D1, D2, common GND
ESP8266WiFiMulti wifiMulti;
FirebaseData firebaseData;

#define FIREBASE_HOST "farmbotcnc.firebaseio.com"
#define FIREBASE_AUTH "qMYQ1i0drocYWOxBFXf66d7BHyDwT5FtzRfnWNqR"

bool connectioWasAlive = true;
//String data = "*s;autorace_2G;autoraceonly;abcdefgh;123456789#";
const char *wifi[4];
String dataPic;
String value[5];
String dataFromMega;

void setup() {
  Serial.begin(9600);
  MySerial.begin(19200);

//  MySerial.print("*s#");  
//  while(1){                     //*s;autorace_2G;autoraceonly;abdfhdterf;1235456dff#
//    if(MySerial.available()){
//      dataPic = MySerial.readStringUntil('#');
//      if (dataPic.startsWith("*s")) break;
//      }
//    }
//  dataPic = dataPic.substring(1,dataPic.length()); //Xóa * và # ở đầu và cuối chuỗi
//  dataPic.concat(";");
//  for (int i=0; i<5; i++){
//    value[i] = dataPic.substring(0,dataPic.indexOf(";",0));
//    dataPic.remove(0,dataPic.indexOf(";",0)+1);
//    }
//  wifi[0] = value[1].c_str();         
//  wifi[1] = value[2].c_str();         
//  wifi[2] = value[3].c_str(); 
//  wifi[3] = value[4].c_str();
//  
//  wifiMulti.addAP(wifi[0], wifi[1]);
//  wifiMulti.addAP(wifi[2], wifi[3]);

  wifiMulti.addAP("autorace_2.4G","autoraceonly");
  connectWiFi();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
//  Firebase.stream("/");   
  }

void loop() { 
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
    connectWiFi();
  }
  else {
//      if(MySerial.available()){
//          dataFromMega = MySerial.read();
//          Serial.println(dataFromMega);
//      }
      processingData(); 
//    handleReadingGroundHumiditySensor();
  }
}
  
void processingData(){
    //Get recognition activation data
    Firebase.getInt(firebaseData, "/allWorkingModes/weedDetectionMode/isDetected");
    int isDetected = firebaseData.intData();
     
} 

void connectWiFi(){
  Serial.println("Connecting Wifi...");
  for(int k=0;k<30;k++){
//    if (wifiMulti.run() == WL_CONNECTED){
//      Serial.println("Wifi is connected!!!");
//      break;
//      }
     if (wifiMulti.run() != WL_CONNECTED){
      if (connectioWasAlive == true){
        connectioWasAlive = false;
        Serial.print("Looking for WiFi ");
        }
        Serial.print(".");
        delay(500);
        }   
    else if (connectioWasAlive == false){   
      connectioWasAlive = true;
      Serial.printf(" connected to %s\n", WiFi.SSID().c_str());
      if (WiFi.status() == WL_CONNECTED){ 
        Serial.println("");
        Serial.println("WiFi connected");
        WiFi.softAPdisconnect(true);       
        }
      else {
        Serial.println("lỗi kết nối");
        delay(1000);
        ESP.reset();
        delay(2000);
        }
      } 
    }
  }
