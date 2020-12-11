//-- Libraries Included --------------------------------------------------------------
  #include <ESP8266WiFi.h>    // The Basic Function Of The ESP NOD MCU

//------------------------------------------------------------------------------------
  // Define I/O Pins
  #define       LED0      2       // WIFI Module LED
  #define       button    D2

//------------------------------------------------------------------------------------
  // Authentication Variables
  char*         ssid = "DataTransfer";            // Wifi Name
  char*         password = "test1234567890";        // Wifi Password
  String buttonstate = "0";
//------------------------------------------------------------------------------------
  // WIFI Module Role & Port
  IPAddress     TCP_Server(192, 168, 4, 1);
  IPAddress     TCP_Gateway(192, 168, 4, 1);
  IPAddress     TCP_Subnet(255, 255, 255, 0);

  unsigned int  TCPPort = 2390;
  WiFiClient    TCP_Client;

//------------------------------------------------------------------------------------
  // Some Variables
  unsigned char buffer[80];
  char result[10];

//====================================================================================

void setup(){
  // setting the serial port ----------------------------------------------
  Serial.begin(115200);  
  
  // setting the mode of pins ---------------------------------------------
  pinMode(LED0, OUTPUT);                          // WIFI OnBoard LED Light
  digitalWrite(LED0, !LOW);                       // Turn WiFi LED Off
  pinMode(button, INPUT);                          // Button Pin
  
  // WiFi Connect ----------------------------------------------------
  Check_WiFi_and_Connect_or_Reconnect();          // Checking For Connection
}

//====================================================================================
 
void loop(){

  if(digitalRead(button))
  {
    delay(200);
    if(buttonstate == "0") 
    {
      buttonstate = "1";
    }
    else if(buttonstate == "1")
    {
      buttonstate = "0";
    }
    Send_Request_To_Server();
    
  }

  if(WiFi.status() != WL_CONNECTED){
    ESP.restart();
  }

}

//====================================================================================

void Send_Request_To_Server() {
  unsigned long tNow;
  TCP_Client.println(buttonstate);                             // Send Data                    

  TCP_Client.flush();                                     // Empty Bufffer 
  Check_WiFi_and_Connect_or_Reconnect();
}

//====================================================================================

void Check_WiFi_and_Connect_or_Reconnect(){
  
  if (WiFi.status() != WL_CONNECTED){
 
    TCP_Client.stop(); //Make Sure Everything Is Reset
    WiFi.disconnect();
    Serial.println("Not Connected...trying to connect...");
    delay(50);
    WiFi.mode(WIFI_STA);                                // station (Client) Only - to avoid broadcasting an SSID ??
    WiFi.begin("DataTransfer");                         // the SSID that we want to connect to
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED){
      for(int i=0; i < 10; i++){
        digitalWrite(LED0, !HIGH);
        delay(250);
        digitalWrite(LED0, !LOW);
        delay(250);
        Serial.print(".");
      }
      Serial.println("");
    }
  // stop blinking to indicate if connected -------------------------------
    digitalWrite(LED0, !HIGH);
    Serial.println("!-- Client Device Connected --!");

  
  // Printing IP Address --------------------------------------------------
    Serial.println("Connected To      : " + String(WiFi.SSID()));
    Serial.println("Signal Strenght   : " + String(WiFi.RSSI()) + " dBm");
    Serial.print  ("Server IP Address : ");
    Serial.println(TCP_Server);
    Serial.print  ("Device IP Address : ");
    Serial.println(WiFi.localIP());
    TCP_Client.connect(TCP_Server, TCPPort);
    TCP_Client.setNoDelay(1);
  }
}
