//-- Libraries Included --------------------------------------------------------------
  #include <ESP8266WiFi.h>

//------------------------------------------------------------------------------------
  // Define I/O Pins
  #define     LED0        2         // WIFI Module LED
  #define     LED1        D2         // WIFI Module LED
//------------------------------------------------------------------------------------
  // WiFi settings
  #define     MAXSC     1           // MAXIMUM NUMBER OF CLIENTS
  IPAddress APlocal_IP(192, 168, 4, 1);
  IPAddress APgateway(192, 168, 4, 1);
  IPAddress APsubnet(255, 255, 255, 0);

  unsigned int TCPPort = 2390;
  
  WiFiServer  TCP_SERVER(TCPPort);      // THE SERVER AND THE PORT NUMBER
//------------------------------------------------------------------------------------
  // Some Variables
  char result[10];
  bool heartBeat = true;
  unsigned long previousMillis = 0;
  const long interval = 3000;
  int count = 0;

  //char *data = "Hello my name is parvaiz ahmad, I am sending this huge amount of data just to confirm that either this amount of data transfers successfully or not.";
void setup(){
  
  // Setting the serial port
  Serial.begin(115200);           // Computer Communication
  // Setting the mode of the pins
  pinMode(LED0, OUTPUT);          // WIFI OnBoard LED Light
  pinMode(LED1, OUTPUT);          // WIFI OnBoard LED Light
    
  // setting up a Wifi AccessPoint
  char apname[] = "DataTransfer";
  char appass[] = "test1234567890";
  SetWifi(apname,appass);
  
}

//====================================================================================
 
void loop(){
  WiFiClient TCP_Client = TCP_SERVER.available();
  if(TCP_Client)  // TCP_SERVER.hasClient()
  {
    TCP_Client.setNoDelay(1);
    bool conn = TCP_Client.connected();
    while(conn)
    {
      if(TCP_Client.available())
      {
        String Message = TCP_Client.readStringUntil('\n');
        Serial.println(Message);
        if(Message.toInt() == 0)
        {
          Serial.println("Turining off led");
          digitalWrite(LED1, LOW);
        }
    
        if(Message.toInt() == 1)
        {
          Serial.println("Turining on led");
          digitalWrite(LED1, HIGH);
        }

        if(Message.toInt() == 2)
        {
          heartBeat = true;
          count = 0;
          Serial.println("Hearbeat Received.");
        }
      }
      conn = TCP_Client.connected();
      count++;
      if(count > 10000) 
      {
        heartBeat = false;
      }
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if(heartBeat == false) 
        {
          Serial.print("Heart beat not received, Restarting server.");
          ESP.restart();
        }
      }
      delay(1);
    }  
  }
  else
  {
    
    // the LED blinks if no clients are available
    digitalWrite(LED0, HIGH);
    delay(150);
    digitalWrite(LED0, LOW);
    delay(150); 
  } 
}  

//====================================================================================
 
void SetWifi(char* Name, char* Password){
  // Stop any previous WIFI
  WiFi.disconnect();

  // Setting The Wifi Mode
  WiFi.mode(WIFI_AP);
  Serial.println("WIFI Mode : AccessPoint");
   
  // Starting the access point
  WiFi.softAPConfig(APlocal_IP, APgateway, APsubnet);                 // softAPConfig (local_ip, gateway, subnet)
  WiFi.softAP(Name, Password, 1, 0, MAXSC);                           // WiFi.softAP(ssid, password, channel, hidden, max_connection)     
   
  // wait a bit
  delay(50);
  TCP_SERVER.begin();                                                 // which means basically WiFiServer(TCPPort);
  
  Serial.println("Server Started");
}
