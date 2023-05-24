#include <SoftwareSerial.h>
#include <DHT.h>

#define DHTTYPE DHT11

SoftwareSerial rylr(13, 15);

DHT dht(4, DHTTYPE);

void setup(){

  Serial.begin(115200);
  dht.begin();
  rylr.begin(115200);
}

void loop(){
  delay(2000);
  for(int i = 0; i < 2; i++){
    float h = dht.readHumidity();
    float t = dht.readTemperature();
  
    if (isnan(h) || isnan(t) ) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.println(t);
  
    String umidade = (String)h;
    String temperatura = (String)t;
  
    //int compresultadoint = umidade.length() + temperatura.length() + 35;
    int compresultadoint = umidade.length() + temperatura.length() + 11;
      
    String compresultado = (String)compresultadoint;
  
    Serial.println(umidade);
    Serial.println(temperatura);
    Serial.println(compresultado);
    
    //rylr.println("AT+SEND=5,5,HELLO");
    //delay(5000);
    //rylr.println("AT+SEND=5," + compresultado + ",{\"dht11\": {\"temp\": \"" +  temperatura.c_str() + "\", \"umid\": \"" + umidade.c_str() + "\"}}");
    rylr.println("AT+SEND=5," + compresultado + ",TEMP:" + temperatura.c_str() + ";UMID:" + umidade.c_str());
    
    delay(5000);
  }  
  //ESP.deepSleep(1800e6);
}
