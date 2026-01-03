#include <Arduino.h>
#include <Preferences.h>
#include <FastLED.h>
#include <font2812.h>

#define NET
#define GRAPH
#define U8G

#if defined (NET)
#include "WiFi.h"
#endif


#if defined (GRAPH)
#if defined (U8G)
#include <U8g2lib.h>
#include <Wire.h>
#else // U8G
#include <Arduino_GFX_Library.h>
#endif // U8G
#endif // GRAPH



//#define S3MINIPRO
//#define S2MINI
//#define S3ZERO
#define C3OLED

#define LOCAL_IP (132)
#define DEF_BRIGHTNESS  (32)
#define DEF_XRES (32)
#define DEF_YRES (8)

#define MAX_XRES (256)
#define MAX_YRES (32)
#define MAX_NUM_LEDS    (1024)
#define NUM_LEDS_DEF (32)
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB // GRB for 2812 strip, RGB for christmas lights
#define DEB_THRESH (10)

// 6 scroll
#define BASE_SCROLL (1)
// 16 pong
#define BASE_PONG (7)
// 14 solid
#define BASE_SOLID (23)
#define NUM_SCROLL (BASE_PONG-BASE_SCROLL)
#define NUM_PONG (BASE_SOLID-BASE_PONG)
#define NUM_SOLID (NUM_ANI - BASE_SOLID + 1)
#define NUM_ANI (36)

#define DEF_CLRFREQ (3)
#define DEF_DEL (15)
#define DEF_CYC (10000)

//#define EN_PIN      (12) // certain boards need enable pin held high
//#define DEB_PIN     (42)
#if defined (S3ZERO)
#define LED_PIN     (3)  // 8 for ESP32-S3ZERO (21 builtin), 14 for s3mini-pro, 16 for s2 mini
#define GND_PIN (2)     // some boards need a pin grounded
#define LED_PIN_AUX     (21)  // 8 for ESP32-S3ZERO (21 builtin), 14 for s3mini-pro. C3 flaky, C5 nothing
#define NUM_LEDS_AUX    (1)
#define LED_TYPE_AUX    WS2812
#define COLOR_ORDER_AUX RGB
#define BUT_PIN     (0)   //0 for ESP32-S3ZERO, 47 on S3MINPO, 9 for SEED C3, 0 for s2-mini
#endif // S3ZERO

#if defined (C3OLED)
#define LED_PIN     (2) 
#define BUT_PIN     (9)   //0 for ESP32-S3ZERO, 47 on S3MINPO, 9 for SEED C3, 0 for s2-mini
#endif // 


// net

#if defined(NET)
//#define NET_SCAN
//#define LOOPBACK
#define DEBUG_PRINT
#define WIFI_CONNECT_TIMEOUT_MS (7000)
#define NET_STAT_NONE (0)
#define NET_STAT_CONN (1)
#define NET_STAT_CLIENT (2)
String loc_ipaddr_str = "";
String rem_ipaddr_str = "";
String ssid = "";
typedef struct
{
  int server_hasclient;
  int client_avail;
  int client_conn;
  int stat;
} t_net_status;
char infostr[1024];
char hdr[32];
uint8_t rxbuf[1024];
int button_down = 0;
int net_start;
//uint8_t ds8b_address[8];
t_net_status net_status = { 0, 0, 0, 0 };
#endif // NET


#ifdef GRAPH
#ifdef U8G
char printbuf[128];
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 6, 5);
int width = 72;
int height = 40;
int xOffset = 30; // = (132-w)/2
int yOffset = 12; // = (64-h)/2
#else // U8G
char printbuf[128];
// tft
Arduino_DataBus *bus = create_default_Arduino_DataBus();
Arduino_GFX *gfx = new Arduino_GC9107(bus, DF_GFX_RST, 1 /* rotation */, true /* IPS */);
#define GFX_BL DF_GFX_BL  // default backlight pin, you may replace DF_GFX_BL to actual backlight pin
#endif // U8G
#endif // GRAPH


#define MAKPIX_SCL(r,g,b,scl) ( ( (unsigned)(r*scl) << 16 ) | ( (unsigned)(g*scl) << 8 ) | ( (unsigned)(b*scl) ) )
#define MAKPIX(r,g,b) ( ( r << 16 ) | ( g << 8 ) | ( b ) )

#define BACK_BUFFER_LEN (8*5*256)


CRGB leds[MAX_NUM_LEDS];
CRGB map_back[BACK_BUFFER_LEN]; // 256 character background map to store bitmap

#ifdef S3ZERO
CRGB leds_aux[NUM_LEDS_AUX];
#endif

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

// wifi
#if defined(NET)
WiFiClient wifi_client;
WiFiServer wifi_server(80);
#endif

// persistence
Preferences prefs;  // global instance

// config
unsigned xres = DEF_XRES;
unsigned yres = DEF_YRES;
int brightness = DEF_BRIGHTNESS;
uint32_t mode = 0;
unsigned ver = 0;
unsigned storecount = 0;

// global
unsigned num_leds;
unsigned timer_ct=0;

// proto
void draw_text(String txt1, String txt2, String txt3);
void gfx_init(void);


void shift_color(CHSV& incolor, int color_freq);

int network_sm();
void net_connect(String ssid);
void disp_net_status(void);
void wifi_server_process();
int set_if_changed(int *val_to_set, int val);
void check_client_connection();
void init_leds();
void readfs();
void putfs();



void setup() 
{
  Serial.begin(115200);
  delay( 300 ); // power-up safety delay
  Serial.println("Starting..");

  // 1) Open a “namespace” in NVS
  //    Second arg: false = read/write, true = read-only
  if (!prefs.begin("marque", false)) {
    Serial.println("Failed to init NVS");
    while (true) {}
  }  

  //readfs();
  
#if defined(GRAPH)
  gfx_init();
  draw_text("Starting..", "", "");
#endif



  // initialize wifi
#if defined(NET)

// *******************************************************************************    
// SETUP FOR FIXED IP, UNTESTED
  // Set your Static IP address
  IPAddress local_IP(192, 168, 1, LOCAL_IP);
  // Set your Gateway IP address
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 0, 0);
  IPAddress primaryDNS(8, 8, 8, 8);   //optional
  IPAddress secondaryDNS(8, 8, 4, 4); //optional
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) 
  {
    Serial.println("STA Failed to configure");
  }
// *******************************************************************************    
    
  // Set WiFi to station mode and disconnect from an AP if it was previously connected.
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);  // ?????


  font_init();
 
  //font_draw("Hello and Merry Christmas and a Happy New Year!!!", map_back, 0, yres);
  font_draw("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!*? Merry Christmas and a Happy New Year to *All*!!!", map_back, 0, yres);
  font_xfer(map_back, leds, xres, yres, 0, BACK_BUFFER_LEN);
#endif


#if defined EN_PIN
    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, 1);
#endif
    pinMode(LED_PIN, OUTPUT);
#if defined (GND_PIN)
    pinMode(GND_PIN, OUTPUT);
    digitalWrite(GND_PIN, 0); 
#endif

#if defined BUT_PIN
    pinMode(BUT_PIN, INPUT_PULLUP);
#endif

  init_leds();
    
}

void init_leds()
{
    num_leds = xres * yres;
    // validate num_leds

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, num_leds).setCorrection( TypicalLEDStrip );

#if defined(S3ZERO)
    FastLED.addLeds<LED_TYPE_AUX, LED_PIN_AUX, COLOR_ORDER_AUX>(leds_aux, NUM_LEDS_AUX).setCorrection( TypicalLEDStrip );
#endif    
    FastLED.setBrightness(  brightness );

}

void putfs()
{
  ++storecount;
  prefs.putUInt("storecount", storecount);
  prefs.putUInt("ver", ver);
  prefs.putUInt("xres", xres);
  prefs.putInt("yres", yres);
  prefs.putInt("brightness", brightness);
  prefs.putUInt("mode", mode);
  

  Serial.printf("putfs: storect=%u, storedver=%u, xres=%u, yres=%d, brigt=%d\n", 
                        storecount, ver, xres, yres, brightness, mode);
}

void readfs()
{
  storecount = prefs.getUInt("storecount", 0);  
  uint32_t storedver = prefs.getUInt("ver", 0);  
  xres = prefs.getUInt("xres", DEF_XRES);  
  yres = prefs.getInt("yres", DEF_YRES);  
  brightness = prefs.getInt("brightness", DEF_BRIGHTNESS);
  mode = prefs.getUInt("mode", 0);

  Serial.printf("readfs: storect=%u, storedver=%u, xres=%u, yres=%d, bright=%d, mode=%d\n", 
    storecount, storedver, xres, yres, brightness, mode);
}


char b1[16], b2[16], b3[16];

void loop()
{
  static int but_deb = 0;
  static int offs = 0;
  int but_trig = 0;
  


  #ifdef BUT_PIN
    if (digitalRead(BUT_PIN) == 0 && but_deb >=0)
    {
      but_deb = but_deb + 1;
      if (but_deb > DEB_THRESH)
      {
        but_trig = 1;
        Serial.printf("button pressed\n");
        //but_deb = -DEB_THRESH;
        but_deb = 0;
      }
    }
#if 0
    else if (but_deb < 0)
    {
      if (digitalRead(BUT_PIN) != 0)
      {
        // wait for button clear
        ++but_deb;
      }
    }
#endif    
    else
    {
      but_deb = 0;
    }
  #endif

#if defined (NET)
  network_sm();
#endif

    //if (timer_ct == 100)
    //if (but_trig)
    {
      memset(leds, 0, sizeof(CRGB)*MAX_NUM_LEDS);
      font_xfer(map_back, leds, xres, yres, offs, 87*6*8);
      offs += 1* yres;
    }



  ++timer_ct;
  

  FastLED.delay(100);
}





#if defined (NET)

int network_sm() {
  int test_x;
  int ch;

#if defined(NET_SCAN)
  // scan network
  if (net_status.stat == 0) {
    ssid = net_scan();
    delay(250);
  }
#else   // NET_SCAN
  ssid = "netgear777";
  button_down = 1;
#endif  // NET_SCAN

  // log onto network
  if (net_status.stat == 0 && button_down) {
    net_connect(ssid);
    delay(1000);
    if (net_status.stat == NET_STAT_CONN) {
      // begin tcp server
      wifi_server.begin();
      disp_net_status();
    }
    button_down = 0;
  }

  // check for client connections
  if (net_status.stat & NET_STAT_CONN) {
    // if connected to network, check client connected or dropped
    check_client_connection();
  }

  wifi_server_process();

  ch = 0;
  ch += set_if_changed(&net_status.server_hasclient, wifi_server.hasClient());
  ch += set_if_changed(&net_status.client_avail, wifi_client.available());
  ch += set_if_changed(&net_status.client_conn, wifi_client.connected());

  if (ch > 0) disp_net_status();
  return ch;
}

int set_if_changed(int *val_to_set, int val) {
  int ch;
  ch = ~(*val_to_set == val);
  //Serial.printf("%d,%d,%d\n", *val_to_set, val, ch);
  *val_to_set = val;
  return (ch);
}


void wifi_server_process() 
{
#if defined(LOOPBACK)
  // loopback
  while (wifi_client.connected() && wifi_client.available()) 
  {
    int rxd = wifi_client.read(rxbuf, sizeof(rxbuf));
    wifi_client.write(rxbuf, rxd);
  }
#else
  int sumrxd = 0;
  while (wifi_client.connected() && wifi_client.available()) 
  {
    int rxd = wifi_client.read(rxbuf, sizeof(rxbuf));
    sumrxd += rxd;
#if 0
    if (rxd > 0)
    {
      Serial.println(rxd);
      for (int i=0; i<rxd; ++i) infostr[i] = rxbuf[i];
      infostr[rxd] = 0;
      Serial.println(infostr);
      for (int i=0; i<rxd; ++i) 
        Serial.printf("%.2x-", infostr[i]);
      Serial.printf("\n");

      delay(250);
      disp_net_status();

    }
#endif

/*
  ?      : get status
  b{val} : set brightness
  x{val} : set xres
  y{val} : set yres
  m{val} : set mode
  w      : write settings to persistent memeory   

*/

#if 1

    for (int i=0; i<rxd; ++i) infostr[i] = rxbuf[i];
    infostr[rxd] = 0;
    Serial.printf("str=%s len=%d\n",infostr, rxd);

    if (rxbuf[0] == '?') 
    {
      // remote status request
      sprintf(infostr, "bright=%d, xres=%d, yres=%d, mode=%u ver=%d \n [ ? | w | {x} | b{x} | n{x} | m{x} | f{x} | d{x} | c{x} ]\n", 
                        brightness,xres,    yres,    mode,   ver);
      wifi_client.write(infostr, strlen(infostr));
      Serial.println(infostr);
    } 
    else if (rxbuf[0] == 'X' || rxbuf[0] == 'x' )
    {
        int test_xres = atoi(infostr+1);
        if (test_xres > 0 && test_xres <= MAX_XRES)
        {
          xres = test_xres;
          sprintf(infostr, "OK xres=%d\n", test_xres);
          wifi_client.write(infostr, strlen(infostr));
          init_leds();
        }
    }
    else if (rxbuf[0] == 'Y' || rxbuf[0] == 'y' )
    {
        int test_yres = atoi(infostr+1);
        if (test_yres > 0 && test_yres <= MAX_YRES)
        {
          yres = test_yres;
          sprintf(infostr, "OK yres=%d\n", test_yres);
          wifi_client.write(infostr, strlen(infostr));
          init_leds();
        }
    }    
    else if (rxbuf[0] == 'B' || rxbuf[0] == 'b' )
    {
        int test_bright = atoi(infostr+1);
        if (test_bright >= 0 && test_bright <= 255)
        {
          brightness = test_bright;
          sprintf(infostr, "OK bright=%d\n", brightness);
          wifi_client.write(infostr, strlen(infostr));
          init_leds();
        }
    }
    else if (rxbuf[0] == 'W' or rxbuf[0] == 'w')
    {
      putfs();
      sprintf(infostr, "OK wrote config\n");
      wifi_client.write(infostr, strlen(infostr));
    }
    else if (rxbuf[0] == 'M' or rxbuf[0] == 'm')
    {
      mode = atoi(infostr+1);
      sprintf(infostr, "OK mode=%u\n", mode);
      wifi_client.write(infostr, strlen(infostr));
    }
#endif  // 1
  }// while
#endif  // LOOPBACK
  if (sumrxd)
  {
  }
}

void disp_net_status(void) {

  sprintf(infostr, "timer_ct=%d, net_stat=%d, loc_ip=%s, cli_ip=%s  srv_hascli=%d, cli_av=%d cli_con=%d \n",
          timer_ct, net_status.stat, loc_ipaddr_str.c_str(), rem_ipaddr_str.c_str(), net_status.server_hasclient, net_status.client_avail, net_status.client_conn);
  Serial.printf(infostr);

//  sprintf(infostr, "ssid=%s\nnet_stat=%d\nloc_ip=%s\ncli_ip=%s\nsrv_hascli=%d\ncli_av=%d\ncli_con=%d \n",
//          "netgear777", net_status.stat, loc_ipaddr_str.c_str(), rem_ipaddr_str.c_str(), net_status.server_hasclient, net_status.client_avail, net_status.client_conn);
//#ifdef GRAPH
//  sprintf(infostr, "%s\stat=%d\n%s, %s\nst=%d,%d,%d\n",
//          "netgear777", net_status.stat, loc_ipaddr_str.c_str(), rem_ipaddr_str.c_str(), net_status.server_hasclient, net_status.client_avail, net_status.client_conn);
//  draw_text("stat      ", infostr);
//#endif

}

void check_client_connection() {
  if (wifi_server.hasClient()) {
    // If we are already connected to another computer,
    // then reject the new connection. Otherwise accept
    // the connection.
    if (wifi_client.connected()) {
      Serial.println("Connection rejected");
      wifi_server.available().stop();
    } else {
      wifi_client = wifi_server.available();
      rem_ipaddr_str = wifi_client.remoteIP().toString();
      Serial.printf("Connection accepted from %s\n", rem_ipaddr_str.c_str());
      net_status.stat = NET_STAT_CLIENT;
    }
    disp_net_status();
  }
}

void net_connect(String ssid) {
  int try_ct = 0;

  Serial.printf("Connecting to WiFi ssid=%s", ssid.c_str());
  sprintf(infostr, "%s", ssid.c_str());
#ifdef GRAPH
  char b1[8];
  sprintf(b1, "ip=%d", LOCAL_IP);
  draw_text("connect", infostr, b1);
#endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, "princess777");

  while (WiFi.status() != WL_CONNECTED && ++try_ct < WIFI_CONNECT_TIMEOUT_MS * 0.01) 
  {
    Serial.print('.');
    delay(50);
  }
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED) 
  {
    loc_ipaddr_str = WiFi.localIP().toString();
    sprintf(infostr, "ssid=%s\nip=%s", ssid.c_str(), loc_ipaddr_str.c_str());
    Serial.println(infostr);
#ifdef GRAPH
    char b1[8];
    sprintf(b1, "ip=%d", LOCAL_IP);
    draw_text("connect", "success", b1);
#endif    
    net_status.stat = NET_STAT_CONN;
  } 
  else 
  {
    sprintf(infostr, "st=%d", WiFi.status());
    Serial.println(String("Error") + infostr);
#ifdef GRAPH
    draw_text("connect", "error", infostr);
#endif    
  }
}


String net_scan() {
  String s_infostr;
  String retval = "";

  Serial.println("Scan start");

  // WiFi.scanNetworks will return the number of networks found.
  int n = WiFi.scanNetworks();
  int n1 = n;
  if (n1 > 7) n1 = 7;
  Serial.println("Scan done");
  s_infostr = "";
  if (n1 == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
    for (int i = 0; i < n1; ++i) {
      // Print SSID and RSSI for each network found
      Serial.printf("%2d", i + 1);
      //sprintf(infostr,"%2d",i + 1); s_infostr += infostr;
      Serial.print("|");
      //sprintf(infostr,"|");  s_infostr += infostr;
      Serial.printf("%s", WiFi.SSID(i).c_str());
      sprintf(infostr, "%-10.10s", WiFi.SSID(i).c_str());
      s_infostr += infostr;
      Serial.print("|");
      sprintf(infostr, "|");
      s_infostr += infostr;
      Serial.printf("%d", WiFi.RSSI(i));
      sprintf(infostr, "%d", WiFi.RSSI(i));
      s_infostr += infostr;
      Serial.print("|");
      sprintf(infostr, "|");
      s_infostr += infostr;
      Serial.printf("%d", WiFi.channel(i));
      sprintf(infostr, "%d", WiFi.channel(i));
      s_infostr += infostr;
      Serial.print("|");
      sprintf(infostr, "|");
      s_infostr += infostr;
      switch (WiFi.encryptionType(i)) {
        case WIFI_AUTH_OPEN:
          Serial.print("open");
          sprintf(infostr, "open");
          s_infostr += infostr;
          break;
        case WIFI_AUTH_WEP:
          Serial.print("WEP");
          sprintf(infostr, "wep ");
          s_infostr += infostr;
          break;
        case WIFI_AUTH_WPA_PSK:
          Serial.print("WPA");
          sprintf(infostr, "wpa ");
          s_infostr += infostr;
          break;
        case WIFI_AUTH_WPA2_PSK:
          Serial.print("WPA2");
          sprintf(infostr, "wpa2");
          s_infostr += infostr;
          break;
        case WIFI_AUTH_WPA_WPA2_PSK:
          Serial.print("WPA+WPA2");
          sprintf(infostr, "wwp2");
          s_infostr += infostr;
          break;
        case WIFI_AUTH_WPA2_ENTERPRISE:
          Serial.print("WPA2-EAP");
          sprintf(infostr, "wp2e");
          s_infostr += infostr;
          break;
        case WIFI_AUTH_WPA3_PSK:
          Serial.print("WPA3");
          sprintf(infostr, "wpa3");
          s_infostr += infostr;
          break;
        case WIFI_AUTH_WPA2_WPA3_PSK:
          Serial.print("WPA2+WPA3");
          sprintf(infostr, "wp23");
          s_infostr += infostr;
          break;
        case WIFI_AUTH_WAPI_PSK:
          Serial.print("WAPI");
          sprintf(infostr, "wapi");
          s_infostr += infostr;
          break;
        default:
          Serial.print("unknown");
          sprintf(infostr, "????");
          s_infostr += infostr;
      }
      Serial.println();
      sprintf(infostr, "\n");
      s_infostr += infostr;
      delay(10);
    }
  }
  Serial.println("");
  sprintf(hdr, "%d found:", n);
//#ifdef GRAPH
//  draw_text(hdr, s_infostr);
//#endif
  if (n > 0) retval = WiFi.SSID(0);

  // Delete the scan result to free memory for code below.
  WiFi.scanDelete();

  return retval;
}

#endif // NET



#if defined(GRAPH)

void gfx_init(void) 
{

#ifdef U8G
  delay(1000);
  u8g2.begin();
  u8g2.setContrast(255); // set contrast to maximum 
  u8g2.setBusClock(400000); //400kHz I2C 
  // tf: full font / all glyphs includedtr: Transparent background
  // ncen = new centry schoolbook, helv, courier, etc
  
  
  u8g2.setFont(u8g2_font_helvB14_tf);
  //u8g2.setFont(u8g2_font_courB12_tf);
  //u8g2.setFont(u8g2_font_ncenB12_tr);
  //u8g2.setFont(u8g2_font_ncenB10_tr);
  //u8g2.setFont(u8g2_font_ncenB08_tf);
#else
  #ifdef GFX_EXTRA_PRE_INIT
    GFX_EXTRA_PRE_INIT();
  #endif
    // Init Display
    if (!gfx->begin()) {
      Serial.println("gfx->begin() failed!");
    }
    gfx->fillScreen(BLACK);
    // backlight
  #ifdef GFX_BL
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);
  #endif
#endif
}



void draw_text(String txt1, String txt2, String txt3) 
{

#ifdef U8G
  u8g2.clearBuffer(); // clear the internal memory


  //u8g2.drawFrame(xOffset+0, yOffset+0, width, height); //draw a frame around the border
  u8g2.setCursor(xOffset+0, yOffset+24);
  u8g2.printf(txt1.c_str());
  u8g2.setCursor(xOffset+0, yOffset+38);
  u8g2.printf(txt2.c_str());
  u8g2.setCursor(xOffset+0, yOffset+52);
  u8g2.printf(txt3.c_str());
  u8g2.sendBuffer(); // transfer internal memory to the display     
#else // U8G

#define SPC (8)
#define TXT_SZ (1)
#define SPC2 (16)
#define TXT_SZ_2 (2)
  //gfx->setFont("u8g2_font_cubic11_h_cjk");
  //  gfx->setTextWrap(false);

//  gfx->fillRect(0, 0, 127, SPC, WHITE);
  gfx->setTextSize(TXT_SZ);
  gfx->setCursor(0, 0);
  gfx->setTextColor(BLACK, WHITE);
  gfx->println(txt1);

  gfx->fillRect(0, SPC
   + 1, 127, 127, BLACK);
  gfx->setTextSize(TXT_SZ_2);
  gfx->setCursor(0, SPC + 1);
  gfx->setTextColor(WHITE, BLACK);
  gfx->println(txt2);
#endif // U8G  
}
#endif // GRAPH
