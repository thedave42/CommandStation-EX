# 1 "C:\\Users\\dave\\AppData\\Local\\Temp\\tmpdvd9vz_1"
#include <Arduino.h>
# 1 "C:/tools/git-repos/DCC-EX/CommandStation-EX/CommandStation-EX.ino"
# 19 "C:/tools/git-repos/DCC-EX/CommandStation-EX/CommandStation-EX.ino"
#if __has_include ( "config.h")
  #include "config.h"
  #ifndef MOTOR_SHIELD_TYPE
  #error Your config.h must include a MOTOR_SHIELD_TYPE definition. If you see this warning in spite not having a config.h, you have a buggy preprocessor and must copy config.example.h to config.h
  #endif
#else
  #warning config.h not found. Using defaults from config.example.h
  #include "config.example.h"
#endif
# 52 "C:/tools/git-repos/DCC-EX/CommandStation-EX/CommandStation-EX.ino"
#include "DCCEX.h"
#include "Display_Implementation.h"
#ifdef ARDUINO_ARCH_ESP32
#include "Sniffer.h"
#include "DCCDecoder.h"
Sniffer *dccSniffer = NULL;
bool DCCDecoder::active = false;
#endif

#ifdef CPU_TYPE_ERROR
#error CANNOT COMPILE - DCC++ EX ONLY WORKS WITH THE ARCHITECTURES LISTED IN defines.h
#endif

#ifdef WIFI_WARNING
#warning You have defined that you want WiFi but your hardware has not enough memory to do that, so WiFi DISABLED
#endif
#ifdef ETHERNET_WARNING
#warning You have defined that you want Ethernet but your hardware has not enough memory to do that, so Ethernet DISABLED
#endif
#ifdef EXRAIL_WARNING
#warning You have myAutomation.h but your hardware has not enough memory to do that, so EX-RAIL DISABLED
#endif


#define PASSWDCHECK(S) static_assert(sizeof(S) == 1 || sizeof(S) > 8, "Password shorter than 8 chars")
void setup();
void loop();
#line 78 "C:/tools/git-repos/DCC-EX/CommandStation-EX/CommandStation-EX.ino"
void setup()
{




  SerialManager::init();

  DIAG(F("License GPLv3 fsf.org (c) dcc-ex.com"));


#if defined(STARTUP_DELAY)
  DIAG(F("Delaying startup for %dms"), STARTUP_DELAY);
  delay(STARTUP_DELAY);
#endif


  IODevice::begin();



  ADCee::begin();

  TrackManager::Setup(MOTOR_SHIELD_TYPE);

  DISPLAY_START (

    LCD(0,F("DCC-EX v" VERSION));
    LCD(1,F("Lic GPLv3"));
  );




#ifndef ARDUINO_ARCH_ESP32
#if WIFI_ON
  PASSWDCHECK(WIFI_PASSWORD);
  WifiInterface::setup(WIFI_SERIAL_LINK_SPEED, F(WIFI_SSID), F(WIFI_PASSWORD), F(WIFI_HOSTNAME), IP_PORT, WIFI_CHANNEL, WIFI_FORCE_AP);
#endif
#else

  PASSWDCHECK(WIFI_PASSWORD);
  WifiESP::setup(WIFI_SSID, WIFI_PASSWORD, WIFI_HOSTNAME, IP_PORT, WIFI_CHANNEL, WIFI_FORCE_AP);
#endif

#if ETHERNET_ON
  EthernetInterface::setup();
#endif


  DCC::begin();


  RMFT::begin();

#ifdef ARDUINO_ARCH_ESP32
#ifdef BOOSTER_INPUT
  dccSniffer = new Sniffer(BOOSTER_INPUT);
#endif
#endif



  #if __has_include ( "mySetup.h")
    #define SETUP(cmd) DCCEXParser::parse(F(cmd))
    #include "mySetup.h"
    #undef SETUP
  #endif

  #if defined(LCN_SERIAL)
  LCN_SERIAL.begin(115200);
  LCN::init(LCN_SERIAL);
  #endif
  LCD(3, F("Ready"));
  CommandDistributor::broadcastPower();
}

void loop()
{
#ifdef ARDUINO_ARCH_ESP32
#ifdef BOOSTER_INPUT
  static bool oldactive = false;
  if (dccSniffer) {
    bool newactive = dccSniffer->inputActive();
    if (oldactive != newactive) {
      RMFT2::railsyncEvent(newactive);
      oldactive = newactive;
    }
    DCCPacket p = dccSniffer->fetchPacket();
    if (p.len() != 0) {
      if (DCCDecoder::parse(p)) {
 if (Diag::SNIFFER)
   p.print();
      }
    }
  }
#endif
#endif





  DCC::loop();


  SerialManager::loop();


#ifndef ARDUINO_ARCH_ESP32
#if WIFI_ON
  WifiInterface::loop();

#endif
#else
#ifndef WIFI_TASK_ON_CORE0
  WifiESP::loop();
#endif
#endif
#if ETHERNET_ON
  EthernetInterface::loop();
#endif

  RMFT::loop();

  #if defined(LCN_SERIAL)
  LCN::loop();
  #endif


  DisplayInterface::loop();


  IODevice::loop();

  Sensor::checkAll();


  static int ramLowWatermark = __INT_MAX__;

  int freeNow = DCCTimer::getMinimumFreeMemory();
  if (freeNow < ramLowWatermark) {
    ramLowWatermark = freeNow;
    LCD(3,F("Free RAM=%5db"), ramLowWatermark);
  }
}