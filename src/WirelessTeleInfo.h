#ifndef WirelessTeleInfo_h
#define WirelessTeleInfo_h

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "Main.h"
#include "base\Utils.h"
#include "base\Base.h"

#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>
#include "LibTeleInfo.h"

const char appDataPredefPassword[] PROGMEM = "ewcXoCt4HHjZUvY1";

class WebTeleInfo : public Application
{
private:
#define HA_HTTP_GENERIC 0
#define HA_HTTP_JEEDOM_TELEINFO 1

  typedef struct
  {
    byte type = HA_HTTP_GENERIC;
    byte fingerPrint[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    struct
    {
      char uriPattern[150 + 1] = {0};
    } generic;
    struct
    {
      char apiKey[48 + 1] = {0};
    } jeedom;
  } HTTP;

#define HA_MQTT_GENERIC 0

  typedef struct
  {
    byte type = HA_MQTT_GENERIC;
    uint32_t port = 1883;
    char username[128 + 1] = {0};
    char password[150 + 1] = {0};
    struct
    {
      char baseTopic[64 + 1] = {0};
    } generic;
  } MQTT;

#define HA_PROTO_DISABLED 0
#define HA_PROTO_HTTP 1
#define HA_PROTO_MQTT 2

  typedef struct
  {
    byte protocol = HA_PROTO_DISABLED;
    bool tls = false;
    char hostname[64 + 1] = {0};
    HTTP http;
    MQTT mqtt;
  } HomeAutomation;

  HomeAutomation _ha;

  //for returning Status
  String _httpJeedomRequest;
  int _haSendResult = 0;
  char _ADCO[13] = {0};

  TInfo _tinfo;

  WiFiClient *_wifiClient = NULL;
  WiFiClientSecure *_wifiClientSecure = NULL;
  PubSubClient *_pubSubClient = NULL;

  void tinfoUpdatedFrame(ValueList *me);
  String GetLabel(const String &labelName);
  String GetAllLabel();

  void SetConfigDefaultValues();
  void ParseConfigJSON(JsonObject &root);
  bool ParseConfigWebRequest(AsyncWebServerRequest *request);
  String GenerateConfigJSON(bool forSaveFile);
  String GenerateStatusJSON();
  bool AppInit(bool reInit);
  void AppInitWebServer(AsyncWebServer &server, bool &shouldReboot, bool &pauseApplication);
  void AppRun();

public:
  WebTeleInfo(char appId, String appName);
};

#endif