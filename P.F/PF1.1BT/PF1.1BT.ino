/*
   Fundacion Kinal
   Centro educativo tecnico laboral Kinal
   Electronica
   Grado: Quinto
   Sección: A
   Curso: Taller de electronica digital y reparación de computadoras I
   Nombre: Guillermo Antonio Franco Longo
   Profe. Alejandro Navas
   Carnet: 2022357
   Proyecto: Proyecto final
*/


//Variables Voltaje
const int sensor= A0; // Sensor FZ0430 conectado al ping A0
int sensorvalue;      // Variable que almacena el valor de 0 a 1023
float V;              // Almacena el voltaje de 0.0 a 25.0


//Variables Amperaje
//Sensibilidad del sensor
float SEN = 0.185; // Para el sensor de 5 A
int SAMPLESNUMBER = 100;

//Variables Resistencia 

int sensorPin = A3;    
int valorsensor;
float voltaje, r1, r2;

// Variables para Bluetooth
#include <SoftwareSerial.h>
const int bluetoothTx = 10;  // Pin TX del módulo Bluetooth conectado al pin 2 del Arduino
const int bluetoothRx = 11;  // Pin RX del módulo Bluetooth conectado al pin 3 del Arduino
SoftwareSerial BT(bluetoothTx, bluetoothRx);


void setup() {
 Serial.begin(9600);
 BT.begin(9600); // Inicializamos el puerto serie BT
 //Resistencia 
 r2 = 10000; // R2 = 10KΩ
}

void loop() {

  // sensor de voltaje
  sensorvalue= analogRead(sensor); //Realiza la lectura
  V= fmap(sensorvalue,0,1023,0.0,25.0); // cambia a 0.0 - 25
  Serial.print("Voltaje = ");
  Serial.println(V); // imprime la variable 
  

  // fin sensor de voltaje
  // Inicio sensor de amperaje

  float current = getCorriente(SAMPLESNUMBER);
  float currentRMS = 0.707 * current; 
  float power = 230.0 * currentRMS;

   printm("intensidad", current, "A ,");
   printm("Irms", currentRMS, "A . ");
   printm("Potencia", power, "W");
   

   //Fin Sensor de amperaje
  // Inicio Resistencia 

  float R2 = 10000;
  float Vout = analogRead(A2);
  float Vout2= map(Vout,0,1023,0,500000);
  float Vout3= Vout2/100000;
  float V1 = (R2*5) / Vout3;
  float R1 = V1 - R2;
  R1=R1+9;
  Serial.println();
  Serial.print("Valor aproximado de R1 = ");
  Serial.println(R1);

  
  // Envía la información por Bluetooth al teléfono
  // Voltaje
  BT.print(voltaje); // Envia el voltaje en voltios
  
  // Amperaje
  BT.print("Intensidad: ");
  BT.print(current);
  BT.print(" A, ");
  BT.print("Irms: ");
  BT.print(currentRMS);
  BT.print(" A, ");
  BT.print("Potencia: ");
  BT.print(power);
  BT.println(" W");
  
  // Resistencia
  BT.print(R1);
  BT.println("Ω");
  delay(3000);
}

//Sensor de voltaje 
// Cambio de escala para dar lectura mas precisa 
float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
 
}
// fin sensor de voltaje 

// Sensor de amperaje
void printm( String prefix, float value1, String postfix)
{
  Serial.print(prefix);
  Serial.print(value1, 3);
  Serial.print(postfix);
  
}

float getCorriente(int SN){
  float Volt;
  float corriente = 0;

  for(int i=0; i < SN; i++)
  {
    Volt = analogRead(A1) * 5.0 / 1023.0;
    corriente += (Volt -2.5) / SEN;
  }
  return(corriente / SN);
}
