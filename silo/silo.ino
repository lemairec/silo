#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char *ssid = "Nordnet_lemaire";
const char *pass = "coincoin88";
String company = "zdizy";
String balise = "essai_1";

#define ONE_WIRE_BUS 2 // DS18B20 on arduino pin2 corresponds to D4 on physical board

const char * host = "maplaine.fr";
const uint16_t port = 443;


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

void setup() {
    Serial.begin(115200);
}

void loop() {
    Serial.println("connecting to network..");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Connected, IP address: ");
    Serial.println(WiFi.localIP());

    DS18B20.requestTemperatures(); 
    float temp = DS18B20.getTempCByIndex(0);
    Serial.println();
    Serial.print("temp : ");
    String path2 = "/silo/api_sonde?company=" + company + "&balise=" + balise + "&temp="+temp;
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
    for(int i = 0; i < 60; ++i){
        Serial.print(".");
        delay(1000);
    }
    
}

