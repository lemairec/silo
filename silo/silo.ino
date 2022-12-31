#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define TEST

String version = "22.12.02";

String company = "zdizy";
String balise = "test";
int nb_minutes = 1;

const char *ssid = "silo_dizy";
const char *pass = "lejard02";

//const char *ssid = "wifi_lemaire";
//const char *pass = "lejard54";



#define ONE_WIRE_BUS_1 5 // DS18B20 on arduino pin2 corresponds to D1 on physical board
#define ONE_WIRE_BUS_2 4 // DS18B20 on arduino pin2 corresponds to D2 on physical board
#define ONE_WIRE_BUS_3 0 // DS18B20 on arduino pin2 corresponds to D3 on physical board
#define ONE_WIRE_BUS_4 2 // DS18B20 on arduino pin2 corresponds to D4 on physical board

#define ONE_WIRE_BUS_E 14 // DS18B20 on arduino pin2 corresponds to D5 on physical board


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
#endif

#ifdef ONE_WIRE_BUS_4
OneWire one_wire_4(ONE_WIRE_BUS_4);
DallasTemperature DS18B20_4(&one_wire_4);
#endif

#ifdef ONE_WIRE_BUS_E
OneWire one_wire_E(ONE_WIRE_BUS_E);
DallasTemperature DS18B20_E(&one_wire_E);
#endif

void setup() {
    Serial.begin(115200);
}

void loop() {
#ifdef TEST
    Serial.println();
    Serial.print("Version : ");
    Serial.println(version);
    Serial.println();
    Serial.print("company : ");
    Serial.println(company);
    Serial.print("balise : ");
    Serial.println(balise);
    Serial.print("nb minutes : ");
    Serial.println(nb_minutes);
    Serial.println();
    Serial.print("wifi : ");
    Serial.println(ssid);
    Serial.print("pass : ");
    Serial.println(pass);

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
#if defined(ONE_WIRE_BUS_4) && defined(TEST)
    DS18B20_4.requestTemperatures();
    float t4 = DS18B20_4.getTempCByIndex(0);
    Serial.print("t4 : ");
    Serial.println(t4);
#endif
#if defined(ONE_WIRE_BUS_E) && defined(TEST)
    DS18B20_E.requestTemperatures();
    float te = DS18B20_E.getTempCByIndex(0);
    Serial.print("te : ");
    Serial.println(te);
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
#if defined(ONE_WIRE_BUS_4)
    DS18B20_4.requestTemperatures();
    float temp4 = DS18B20_4.getTempCByIndex(0);
    path2 = path2 + "&t4="+temp4;
#endif
#if defined(ONE_WIRE_BUS_E)
    DS18B20_E.requestTemperatures();
    float tempe = DS18B20_E.getTempCByIndex(0);
    path2 = path2 + "&te="+tempe;
#endif

    const char * path = path2.c_str();

    BearSSL::WiFiClientSecure client;
    client.setInsecure();
    HTTPClient https;



    Serial.println("connecting to server..");
    Serial.println(path2);

    if (https.begin(client, host, port, path)) {
        int httpsCode = https.GET();
        Serial.println(httpsCode);
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
    for(int j = 0; j < nb_minutes; ++j){
        Serial.print(j);
        for(int i = 0; i < nb_minutes*60; ++i){
            Serial.print(".");
            delay(1000);
        }
    }
    Serial.println("");
}


/*#define HTTPC_ERROR_CONNECTION_REFUSED (-1)
#define HTTPC_ERROR_SEND_HEADER_FAILED (-2)
#define HTTPC_ERROR_SEND_PAYLOAD_FAILED (-3)
#define HTTPC_ERROR_NOT_CONNECTED (-4)
#define HTTPC_ERROR_CONNECTION_LOST (-5)
#define HTTPC_ERROR_NO_STREAM (-6)
#define HTTPC_ERROR_NO_HTTP_SERVER (-7)
#define HTTPC_ERROR_TOO_LESS_RAM (-8)
#define HTTPC_ERROR_ENCODING (-9)
#define HTTPC_ERROR_STREAM_WRITE (-10)
#define HTTPC_ERROR_READ_TIMEOUT (-11)*/
