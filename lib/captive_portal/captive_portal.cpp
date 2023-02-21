/*******************************************************************************
 * Title                 :   Captive Portal
 * Filename              :   captive_portal.cpp
 * Author                :   Simone Gasparella
 * Origin Date           :   22/12/2021
 * Compiler              :   xtensa32 2.50200.97
 * Target                :   ESP32WROOMSE
 * Notes                 :   None
 *
 * THIS SOFTWARE IS PROVIDED BY SYSTEL ELETTRONICA "AS IS" AND ANY EXPRESSED
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL SYSTEL ELETTRONICA OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************/
/****************************************************************************
 * Doxygen C Template
 * Copyright (c) 2021 - Systel Elettronica - All Rights Reserved
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Template.
 *
 *****************************************************************************/
/*************** SOURCE REVISION LOG *****************************************
 *
 *    Date    Version   Author         Description
 *  22/12/21   1.0.0   Simone Gasparella   Initial Release.
 *
 *******************************************************************************/
/** @file : captive_portal.cpp
 *  @brief This module is created for the creation of the captive portal
 */

#include "WString.h"
#include <mqtt_mng.h>
#include <DNSServer.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "captive_portal.h"
#include <Preferences.h>

Preferences preferences;
DNSServer dnsServer;
AsyncWebServer server(ASYNC_WEBSERVER_PORT);
String dropdown_option[MAX_NUM_WIFI_NETWORKS];

static String ssid = "";
static String password = "";
static String no_wifi_network = "NO_WIFI_NETWORK_FOUND";

static int modbus_address = 0;
static int baudrate = 0;
static int data_bit = 0;
static int stop_bit = 0;
static int parity_bit = 0;
static int number_of_wifi_networks_discovered = 0;

static bool is_setup_done = false;
static bool valid_ssid_received = false;
static bool valid_password_received = false;
static bool wifi_timeout = false;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Captive Portal IOT Configurator</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h1>Captive Portal IOT Configurator</h1>
  <br><br>
  <h3>Connectivity with the Cloud setting</h3>
  <br>
  <h3>Optional if you use an Ethernet cable connection (just press on Submit values)</h3>
  <br>
   <form action="/get">
    <br>
    Wifi Name: 
    <select name = "ssid">
      <option value=%0%>%0%</option>
      <option value=%1%>%1%</option>
      <option value=%2%>%2%</option>
      <option value=%3%>%3%</option>
      <option value=%4%>%4%</option>
      <option value=%5%>%5%</option>
      <option value=%6%>%6%</option>
      <option value=%7%>%7%</option>
      <option value=%8%>%8%</option>
      <option value=%9%>%9%</option>
      <option value=%10%>%10%</option>
      <option value=%11%>%11%</option>
      <option value=%12%>%12%</option>
      <option value=%13%>%13%</option>
      <option value=%14%>%14%</option>
      <option value=%15%>%15%</option>
      <option value=%16%>%16%</option>
      <option value=%17%>%17%</option>
      <option value=%18%>%18%</option>
      <option value=%19%>%19%</option>
    </select>
    <br>
    Password: <input type="text" name="password">
    <br>
    <br>
    <h3>Optional (to use only if you don't want DHCP)</h3>
    <br>
    <input type="checkbox" id="Ethernet" name="Ethernet" value="Eth">
    <label for="vehicle1">I want to use a Static IP connection</label>
    <br>
    Static local IP: <input type="number" name="local_ip">
    <br>
    Gateway      IP: <input type="number" name="gateway_ip">
    <br>
    Subnet       IP: <input type="number" name="subnet_ip">
    <br>
    DNS            : <input type="number" name="dns">
    <br>
    <br>
    <input type="submit" value="Submit Values">
    </form>
</body></html>)rawliteral";

/*! \class CaptiveRequestHandler
 *  \brief This is a class for handling the request from captive portal
 *
 * Some details about  class.
 */

class CaptiveRequestHandler : public AsyncWebHandler
{
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request)
  {
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request)
  {
    request->send_P(200, "text/html", index_html, processor);
  }
};

/**
 * @brief  Function to convert the data from the form to a string to show in the html fiie
 * @param  var input string
 * @retval dropdown_option output string
 */
String processor(const String &var)
{
  for (int i = 0; i < MAX_NUM_WIFI_NETWORKS; ++i)
  {
    if (var.toInt() == i)
    {
      return dropdown_option[i];
    }
  }
}
/**
 * @brief  Function to setup the webserver and getting the response from client
 * @param  None
 * @retval None
 */
void setupServer(void)
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html, processor); });

  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String inputMessage;
              String inputParam;

              /*Wifi Configuration*/
              if (request->hasParam("ssid"))
              {
                inputMessage = request->getParam("ssid")->value();
                inputParam = "ssid";
                ssid = inputMessage;
                valid_ssid_received = true;
              }
              if (request->hasParam("password"))
              {
                inputMessage = request->getParam("password")->value();
                inputParam = "password";
                password = inputMessage;
                valid_password_received = true;
              }
              /*Serial Configuration*/
              if (request->hasParam("modbus_address"))
              {
                inputMessage = request->getParam("modbus_address")->value();
                inputParam = "modbus_address";
                modbus_address = inputMessage.toInt();
              }
              if (request->hasParam("baudrate"))
              {
                inputMessage = request->getParam("baudrate")->value();
                inputParam = "baudrate";
                baudrate = inputMessage.toInt();
              }
              if (request->hasParam("data_bit"))
              {
                inputMessage = request->getParam("data_bit")->value();
                inputParam = "data_bit";
                data_bit = inputMessage.toInt();
              }
              if (request->hasParam("stop_bit"))
              {
                inputMessage = request->getParam("stop_bit")->value();
                inputParam = "stop_bit";
                stop_bit = inputMessage.toInt();
              }
              if (request->hasParam("parity_bit"))
              {
                inputMessage = request->getParam("parity_bit")->value();
                inputParam = "parity_bit";
                parity_bit = inputMessage.toInt();
              }

              if (number_of_wifi_networks_discovered != 0)
              {
                request->send(200, "text/html", "The values entered by you have been successfully sent to the device. It will now attempt WiFi connection");
              }
              else
              {
                request->send(200, "text/html", "NO WIFI NETWORK DISCOVERED. PLEASE ENABLE/ACTIVATE AT LEAST ONE VALID WIFI NETWORK!!!");
              }

              /* Saving the information in the memory of ESP32*/
              if (valid_ssid_received && valid_password_received)
              {
                preferences.putBool("is_setup_done", true);
                preferences.putString("rec_ssid", ssid);
                preferences.putString("rec_password", password);
              } });
}
/**
 * @brief  Function to call for the setup of the Access Point
 * @param  None
 * @retval None
 */
void WiFiSoftAPSetup(void)
{
  /*Setting up the wifi as access point*/
  WiFi.mode(WIFI_AP);
  WiFi.softAP("IOT CAPTIVE PORTAL");
}
/**
 * @brief  Function to start the captive portal setting the acces point and the web server
 * @param  None
 * @retval None
 */
void StartCaptivePortal(void)
{
  WiFiSoftAPSetup();
  setupServer();
  dnsServer.start(DNS_PORT_NUMBER, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); // only when requested from AP
  server.begin();
  dnsServer.processNextRequest();
}

/**
 * @brief  Function to run the captive portal if needed
 * @param  None
 * @retval 1 if crdentials were inserted
 */
int setup_captive_and_wifi(void)
{

  /*Test for solution for the problem of the displau*/
  Serial2.begin(38400, SERIAL_8N1, GPIO_NUM_21, GPIO_NUM_19);

  /*Init of the variables*/
  preferences.begin("my-pref", false);
  is_setup_done = preferences.getBool("is_setup_done", false);
  ssid = preferences.getString("rec_ssid", "Sample_SSID");
  password = preferences.getString("rec_password", "abcdefgh");

  /*If the credentials are not saved i start the scan and run the captive portal*/
  if (!is_setup_done)
  {
    /*list of wifi networks*/
    number_of_wifi_networks_discovered = Wifi_list_search();
    /*start of the captive portal*/
    StartCaptivePortal();
  }
  else
  {
    /* ok credentials are valid I can run mqtt process*/
    return 1;
  }

  /*Running the webserver until the credentials are valid*/
  while (!is_setup_done)
  {
    dnsServer.processNextRequest();
    delay(10);

    /*if the credentials are valid i try to connect to the wifi network*/
    if (valid_ssid_received && valid_password_received)
    {
      /* ok credentials are valid*/
      return 1;
    }
  }
}
/**
 * @brief  Function to search for the Wifi networks and save the result in one array
 * @param  None
 * @retval n the number of the wifi networks gound
 */
int Wifi_list_search(void)
{
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();

  Serial.println("scan done");

  /*Checking the number of wifi networks discovered*/
  if (n == 0)
  {
    Serial.println("no networks found");
    for (int i = 0; i < MAX_NUM_WIFI_NETWORKS; ++i)
    {
      dropdown_option[i] = no_wifi_network;
    }
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      dropdown_option[i] = WiFi.SSID(i);
      delay(10);
    }
  }
  return n;
}
/**
 * @brief  Function called from the button to delete information inserted in captive portal
 * @param  val
 * @retval bool the result of the delete
 */
int delete_preferences(uint16_t val)
{
  if (val == 1)
  {
    preferences.clear();
  }
}
