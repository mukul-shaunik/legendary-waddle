#include <SoftwareSerial.h>
#include <dht.h>

dht DHT;

#define DHT11_PIN 7


SoftwareSerial SIM900(9, 10); //TX|RX of sim900 a to connected to 9|10

void setup()
{
  SIM900.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
}

void delayFun(){
  delay(3000);
  }
void delaySlowerFun(){
  delay(7000);
  }

void send() {
  int chk = DHT.read11(DHT11_PIN);
  double temp,humid;
  Serial.print("Temperature = ");
  temp = DHT.temperature;
  humid = DHT.humidity; 
  Serial.println(temp);
  Serial.print("Humidity = ");
  Serial.println(humid);
  Serial.println("HTTP get method :");
  Serial.println("AT\\r\\n");
  SIM900.println("AT");  /* Check Communication */
  delayFun();
  ShowSerialData(); /* Print response on the serial monitor */
  /* Configure bearer profile 1 */
  Serial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\\r\\n");    
  SIM900.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");  /* Connection type GPRS */
  delayFun();
  ShowSerialData();
  Serial.println("AT+SAPBR=3,1,\"APN\",\"bsnlnet\"\\r\\n");  
  SIM900.println("AT+SAPBR=3,1,\"APN\",\"bsnlnet\"");  /* APN of the provider */
  delayFun();
  ShowSerialData();
  Serial.println("AT+SAPBR=1,1\\r\\n");
  SIM900.println("AT+SAPBR=1,1"); /* Open GPRS context */
  delayFun();
  ShowSerialData();
  Serial.println("AT+SAPBR=2,1\\r\\n");
  SIM900.println("AT+SAPBR=2,1"); /* Query the GPRS context */
  delayFun();
  ShowSerialData();
  Serial.println("AT+HTTPINIT\\r\\n");
  SIM900.println("AT+HTTPINIT"); /* Initialize HTTP service */
  delayFun(); 
  ShowSerialData();
  Serial.println("AT+HTTPPARA=\"CID\",1\\r\\n");
  SIM900.println("AT+HTTPPARA=\"CID\",1");  /* Set parameters for HTTP session */
  delayFun();
  ShowSerialData();
  delayFun();
  Serial.println("AT+HTTPPARA=\"URL\",\"0637-103-40-196-20.ngrok.io\"\\r\\n");
  SIM900.println("AT+HTTPPARA=\"URL\",\"0637-103-40-196-20.ngrok.io/save?id=1&temp=" + String(temp,2) + "&humid="+ String(humid,2) +"\"");  /* Set parameters for HTTP session */
  delayFun();
  ShowSerialData();
  Serial.println("AT+HTTPACTION=0\\r\\n");
  SIM900.println("AT+HTTPACTION=0");  /* Start GET session */
  delaySlowerFun();
  ShowSerialData();
  Serial.println("AT+HTTPREAD\\r\\n");
  SIM900.println("AT+HTTPREAD");  /* Read data from HTTP server */
  delaySlowerFun();
  ShowSerialData();
  Serial.println("AT+HTTPTERM\\r\\n");  
  SIM900.println("AT+HTTPTERM");  /* Terminate HTTP service */
  delayFun();
  ShowSerialData();
  Serial.println("AT+SAPBR=0,1\\r\\n");
  SIM900.println("AT+SAPBR=0,1"); /* Close GPRS context */
  delayFun();
  ShowSerialData();
}

void ShowSerialData()
{
  while(SIM900.available()!=0)  /* If data is available on serial port */
  Serial.write(char (SIM900.read())); /* Print character received on to the serial monitor */
}

void ShowSerialData1()
{
  wait("OK,ERROR",10000);
  Serial.println();
  delay(3000);
}

int16_t wait(char* Values, uint16_t timeout) {

  uint16_t Length = strlen(Values);

  char InputBuffer[Length + 1];
  strcpy(InputBuffer, Values);
  char CompareBuffer[Length + 1];
  memset(CompareBuffer,0,sizeof(CompareBuffer));

  uint16_t Quantity = 1;
  
  for (int16_t n = 0; n < Length; n++) {
    if (InputBuffer[n] == ',')
      Quantity++;
  }

  char* InputTokens[Quantity];
  memset(InputTokens, 0, sizeof(InputTokens));
  char* CompareTokens[Quantity];
  memset(CompareTokens, 0, sizeof(CompareTokens));

  InputTokens[0] = InputBuffer;
  CompareTokens[0] = CompareBuffer;

  uint16_t TokenPosition = 1;
  for (int16_t n = 0; n < Length; n++) {
    if (InputBuffer[n] == ',') {
      InputBuffer[n] = 0;
      InputTokens[TokenPosition] = &InputBuffer[n + 1];
      CompareTokens[TokenPosition] = &CompareBuffer[n + 1];
      TokenPosition++;
    }
  }

  uint64_t timer = millis();
  char c;

  while (millis() - timer < timeout) {
    while (SIM900.available()) {
      c = SIM900.read();
      Serial.print(c);

      for (int16_t n = 0; n < Quantity; n++) {
        Length = strlen(CompareTokens[n]);
        if (c == InputTokens[n][Length])
          CompareTokens[n][Length] = c;
        else
          memset(CompareTokens[n], 0, Length);

        if (!strcmp(InputTokens[n],CompareTokens[n]))
          return n;
      }
    }
  }
  return -1;
}

void loop()
{
  send();
}
