#include <esp_now.h>
#include <WiFi.h>
#include "HX711.h"
#include <EEPROM.h>
#include <Wire.h>

//Variables globales
byte DT=12;  //Celda+MX711 PIN Digital DT
byte CLK=13; //Celda+MX711 PIN Digital CLK
byte modo=7;
byte tara = 6;
int peso_conocido[4] = {500,1000,3000,5000};
long escala;
char opcion;

//Crear el objeto de la balanza
HX711 balanza;


// Funcion de Anti-debounce(evita el rebote del pulsador)
void anti_debounce(byte boton){
  delay(100);
  while(digitalRead(boton));  //Antirebote
  delay(100);
}

//Función de calibración y ajuste
void calibration(){
  int i = 0, cal = 1;
  long adc_lecture;

  // Escribimos el Mensaje en el LCD
  Serial.println("Calibracion de Balanza");

  //Inicia el proceso de ajuste y calibración
  while (cal == 1) {
    Serial.print("Peso conocido:");
    Serial.print(peso_conocido[i]);
    Serial.println(" g");
    Serial.println("Ingrese una opción: (1 para calibrar)");
    //Esperar hasta que el usuario ingrese la opción
    while (Serial.available() == 0 ) {
    delay(100);
    }

    opcion = Serial.read(); // Leer un carácter (byte)

    if (opcion == '1') { // Comparamos con comillas simples para comparar con caracteres
      i = (i > 2) ? 0 : i + 1; //if-else en una línea
    }
    //Selecciona el peso conocido con el boton modo
    else if (opcion == '2') { // Comparamos con comillas simples para comparar con caracteres
      Serial.println("Ponga el Peso y espere ...");
      delay(2000);

      //Lee el valor del HX711
      adc_lecture = balanza.get_value(100);
      //Calcula la escala con el valor leido dividido el peso conocido
      escala = adc_lecture / peso_conocido[i];
      //Guarda la escala en la EEPROM
      EEPROM.put(0, escala);
      delay(100);
      cal = 0; //Cambia la bandera para salir del while
    }
  }
  Serial.println("Salio");
}

void setup() {
  
  // Set up Serial Monitor
  Serial.begin(115200);
  //Configurando la balanza
  balanza.begin(DT,CLK);
  //Lee el valor de la escala en la EEPROM
  EEPROM.get( 0, escala );
  delay(1000);
  calibration();
  balanza.set_scale(escala); // Establecemos la escala
  balanza.tare(20);  //El peso actual es considerado Tara.
  
  
  
}
 
void loop() {
  float peso;
  //Mide el peso de la balanza
  peso = balanza.get_units(10);
  //Muestra el resultado
  Serial.print("Peso: ");
  Serial.print(peso);
  Serial.println(" g        ");  
  delay(1000);
  
}