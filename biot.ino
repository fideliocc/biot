#include <FileIO.h>
#include <UbidotsYUN.h>
#include <LiquidCrystal.h>
#include <Process.h>
#include "DHT.h"

//Token único de Ubidots
#define TOKEN "YOUR_TOKEN_HERE"

//Definiendo pin y tipo de sensor DHTxx
#define DHTPIN 2
#define DHTTYPE DHT22

#define LDR_PIN                   A0           //LDR PIN
#define MAX_ADC_READING           5023         //Valor máximo de la conversión ADC
#define ADC_REF_VOLTAGE           5.0          //Voltaje referencial Vcc del circuito
#define REF_RESISTANCE            600        //Resistencia referencial del circuito
#define LUX_CALC_SCALAR           12518931     //Escalar
#define LUX_CALC_EXPONENT         -1.405       //Exponente

#define MOISTURE_PIN              A1

// Inicia la librería con los números de pines de la interfaz
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
DHT dht(DHTPIN, DHTTYPE);

byte termo[8] = //icon for termometer
{
    B00100,
    B01010,
    B01010,
    B01110,
    B01110,
    B11111,
    B11111,
    B01110,
};

byte drop[8] = //icon for water droplet 
{
    B00100,
    B00100,
    B01010,
    B01010,
    B10001,
    B10001,
    B10001,
    B01110,
};

byte light[8] = //icon for water droplet 
{
    0b00000,
    0b10101,
    0b01110,
    0b11111,
    0b11111,
    0b01110,
    0b10101,
    0b00000,
};

byte celsius[8] = {
  0b11000,
  0b11000,
  0b00111,
  0b01000,
  0b01000,
  0b01000,
  0b01000,
  0b00111,
};

byte cloud[8] = {
  0b00000,
  0b00000,
  0b01000,
  0b11110,
  0b11111,
  0b00111,
  0b01000,
  0b00101,
};

byte wifiSearch[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00100,
  0b00000,
};

byte wifiOk[8] = {
  0b00000,
  0b01110,
  0b10001,
  0b00100,
  0b01010,
  0b00000,
  0b00100,
  0b00000,
};

Ubidots client(TOKEN);
int moistureIndicator;
int LED = 3; 

void setup() {
  client.init(); //?
  Bridge.begin(); //?
  Serial.begin(9600);
  FileSystem.begin();
  lcd.begin(16, 2);
  dht.begin();
  
  // while (!SerialUSB); // wait for Serial port to connect.
  SerialUSB.println("BIoT v1.0 - github.com/fideliocc/biot\n");
  
  pinMode(moistureIndicator,INPUT);
  pinMode(LED,OUTPUT);

  // set up the LCD's number of columns and rows: 
  lcd.createChar(0, termo);
  lcd.createChar(1, drop);
  lcd.createChar(2, light);
  lcd.createChar(3, celsius);
  lcd.createChar(4, cloud);
  lcd.createChar(5, wifiSearch);
  lcd.createChar(6, wifiOk);

  lcd.setCursor(0,0);
  lcd.write(byte(0));
  lcd.setCursor(5,1);
  lcd.write(byte(1));
  lcd.setCursor(5,0);
  lcd.write(byte(2));
  lcd.setCursor(3,0);
  lcd.write(byte(3));
  lcd.setCursor(0,1);
  lcd.write(byte(4));
  
  // Print a message to the LCD.
  lcd.setCursor(3, 1);
  lcd.print("%");
  delay(2000);
}

void loop() {
  int   ldrRawData;
  int   sensor3int = dht.readTemperature();
  int   sensor4int = dht.readHumidity();
  float moistureValue;
  float resistorVoltage, ldrVoltage;
  float ldrResistance;
  float ldrLux;
  float sensor3;
  float sensor4;
  float sensor5;
  
   Process wifiCheck;  // inicia nuevo Process
   wifiCheck.runShellCommand("/usr/bin/pretty-wifi-info.lua");  // command you want to run

  // while there's any characters coming back from the
  // process, print them to the serial monitor:
    if (wifiCheck.available() > 0) {
      lcd.setCursor(15,1);
      lcd.write(byte(6));
    } else { if (wifiCheck.available() == 0) {
        lcd.write(byte(5));
      }
    }

   //LDR a Lux
  ldrRawData = analogRead(LDR_PIN);
  resistorVoltage = (float)ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;
  ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;
  ldrResistance = ldrVoltage/resistorVoltage * REF_RESISTANCE;
  ldrLux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);

  //Humedad del suelo y alerta por sequía
  moistureValue = analogRead(MOISTURE_PIN);
  if (moistureValue <= 300) 
    {
      digitalWrite(LED, HIGH);
    } else { digitalWrite(LED, LOW); };
  
  //Enviando datalog a microSD
  String dataString;
  dataString += getTimeStamp();
  dataString += " ";

  // Sensores de luz y humedad del suelo
  for (int analogPin = 0; analogPin < 1; analogPin++) {
    dataString += "Nivel de luz = ";
    dataString += String(ldrLux);
    dataString += " lux";
    
    if (analogPin < 2) {
      dataString += " | ";  // separa los valores
    }

    dataString += "Humedad del suelo = ";
    dataString += String(moistureValue);

      if (analogPin < 2) {
      dataString += " | ";  
    }
  }

  //Sensores de temperatura, humedad e indice de calor
  for (int digitalPin = 1; digitalPin < 2; digitalPin++) {
    
    sensor3 = dht.readTemperature();
    dataString += "Temperatura = ";
    dataString += String(sensor3);
    dataString += "ºC ";

 if (digitalPin < 2) {
      dataString += " | ";  
    }
        sensor4 = dht.readHumidity();
        dataString += "Humedad = ";
        dataString += String(sensor4);
        dataString += " % ";

 if (digitalPin < 2) {
      dataString += " | ";  
    }

        sensor5 = dht.computeHeatIndex(sensor3, sensor4, false);
        dataString += "Índice de calor = ";
        dataString += String(sensor5);
        dataString += "ºC";
}

  //Enviando datos a Ubidots
  client.add("Temperatura", dht.readTemperature());
  client.add("Humedad", dht.readHumidity());
  client.add("Luz" , ldrLux);
  client.add("Humedad del suelo" , moistureValue);
  client.add("Indice de calor" , dht.computeHeatIndex(sensor3, sensor4, false));
  client.sendAll(); 
  
  lcd.setCursor(1,0);
  lcd.print(sensor3int);
  lcd.setCursor(1,1);
  lcd.print(sensor4int); 
  lcd.setCursor(6,1);
  lcd.print(moistureValue); 
  lcd.setCursor(6,0);
  lcd.print(ldrLux);
  
  File dataFile = FileSystem.open("/mnt/sd/datalog.txt", FILE_APPEND);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    SerialUSB.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    SerialUSB.println("Eror al abrir datalog.txt");
  };
  

  delay (2000);
}

// This function return a string with the time stamp
String getTimeStamp() {
  String result;
  Process time;
  // date is a command line utility to get the date and the time
  // in different formats depending on the additional parameter
  time.begin("date");
  time.addParameter("+%D-%T");  // parameters: D for the complete date mm/dd/yy
  //             T for the time hh:mm:ss
  time.run();  // run the command

  // read the output of the command
  while (time.available() > 0) {
    char c = time.read();
    if (c != '\n') {
      result += c;
    }
  }

  return result;

}
