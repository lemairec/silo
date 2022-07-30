#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//#define TEST

const char *ssid = "silo_dizy";
const char *pass = "lejard02";

//const char *ssid = "wifi_lemaire";
//const char *pass = "lejard54";

String company = "zdizy";
String balise = "silo_m";

#define ONE_WIRE_BUS_1 4 // DS18B20 on arduino pin2 corresponds to D4 on physical board
#define ONE_WIRE_BUS_2 0 // DS18B20 on arduino pin2 corresponds to D4 on physical board
#define ONE_WIRE_BUS_3 2 // DS18B20 on arduino pin2 corresponds to D4 on physical board

const char * host = "maplaine.fr";
const uint16_t port = 443;


#ifdef ONE_WIRE_BUS_1
OneWire one_wire_1(ONE_WIRE_BUS_1);
DallasTemperature DS18B20_1(&one_wire_1);
#endif

#ifdef ONE_WIRE_BUS_2
OneWire one_wire_2(ONE_WIRE_BUS_2);
DallasTemperature DS18B20_2(&one_wire_2);
#endif

#ifdef ONE_WIRE_BUS_3
OneWire one_wire_3(ONE_WIRE_BUS_3);
DallasTemperature DS18B20_3(&one_wire_3);

void setup() {
    Serial.begin(115200);
}
#endif

void loop() {
#ifdef TEST
    Serial.println("*** test ");
#endif
#if defined(ONE_WIRE_BUS_1) && defined(TEST)
    DS18B20_1.requestTemperatures(); 
    float t1 = DS18B20_1.getTempCByIndex(0);
    Serial.print("t1 : ");
    Serial.println(t1);
#endif
#if defined(ONE_WIRE_BUS_2) && defined(TEST)
    DS18B20_2.requestTemperatures(); 
    float t2 = DS18B20_2.getTempCByIndex(0);
    Serial.print("t2 : ");
    Serial.println(t2);
#endif
#if defined(ONE_WIRE_BUS_3) && defined(TEST)
    DS18B20_3.requestTemperatures(); 
    float t3 = DS18B20_3.getTempCByIndex(0);
    Serial.print("t3 : ");
    Serial.println(t3);
#endif

    Serial.println("connecting to network..");
    WiFi.begin(ssid, pass);
    bool connected = false;
    for(int i =0; i < 100; ++i){
        if(WiFi.status() == WL_CONNECTED){
            connected = true;
            break;
        }
        delay(1000);
        Serial.print("#");
    }
    Serial.println("");
    if (!connected) {
        Serial.println("fail connecting, retry");
        return;
    }
    Serial.println();
    Serial.println("Connected, IP address: ");
    Serial.println(WiFi.localIP());

    String path2 = "/silo/api_sonde?company=" + company + "&balise=" + balise;
    
#if defined(ONE_WIRE_BUS_1)
    DS18B20_1.requestTemperatures(); 
    float temp1 = DS18B20_1.getTempCByIndex(0);
    path2 = path2 + "&t1="+temp1;
#endif
#if defined(ONE_WIRE_BUS_2)
    DS18B20_2.requestTemperatures(); 
    float temp2 = DS18B20_2.getTempCByIndex(0);
    path2 = path2 + "&t2="+temp2;
#endif
#if defined(ONE_WIRE_BUS_3)
    DS18B20_3.requestTemperatures(); 
    float temp3 = DS18B20_3.getTempCByIndex(0);
    path2 = path2 + "&t3="+temp3;
#endif  

    const char * path = path2.c_str();  

    BearSSL::WiFiClientSecure client;
    client.setInsecure();
    HTTPClient https;

    

    Serial.println("connecting to server..");
    Serial.println(path2);

    if (https.begin(client, host, port, path)) {
        int httpsCode = https.GET();
        if (httpsCode > 0) {
            Serial.println(httpsCode);
            if (httpsCode == HTTP_CODE_OK) {
                Serial.println(https.getString());
            }
        } else {
            Serial.print("failed to GET");
        }
    } else {
        Serial.print("failed to connect to server");
    }
    Serial.println("wait");
    for(int i = 0; i < 5*60; ++i){
        Serial.print(".");
        delay(1000);
    }
    Serial.println("");
}

