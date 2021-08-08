#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char *ssid = "wifi_lemaire";
const char *pass = "lejard54";
String company = "zdizy";
String balise = "essai_2";

#define ONE_WIRE_BUS_1 4 // DS18B20 on arduino pin2 corresponds to D4 on physical board
#define ONE_WIRE_BUS_2 3 // DS18B20 on arduino pin2 corresponds to D4 on physical board
#define ONE_WIRE_BUS_3 4 // DS18B20 on arduino pin2 corresponds to D4 on physical board

const char * host = "maplaine.fr";
const uint16_t port = 443;


OneWire one_wire_1(ONE_WIRE_BUS_1);
DallasTemperature DS18B20_1(&one_wire_1);


OneWire one_wire_2(ONE_WIRE_BUS_2);
DallasTemperature DS18B20_2(&one_wire_2);

OneWire one_wire_3(ONE_WIRE_BUS_3);
DallasTemperature DS18B20_3(&one_wire_3);

void setup() {
    Serial.begin(115200);
}

void loop() {
    Serial.println("connecting to network..");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print("#");
    }
    Serial.println();
    Serial.println("Connected, IP address: ");
    Serial.println(WiFi.localIP());

    DS18B20_1.requestTemperatures(); 
    float temp = DS18B20_1.getTempCByIndex(0);
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
    for(int i = 0; i < 20; ++i){
        Serial.print(".");
        Serial.print(i);
        delay(1000);
    }
    
}

