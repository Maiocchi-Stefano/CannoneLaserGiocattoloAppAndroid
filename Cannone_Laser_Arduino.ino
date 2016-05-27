/* Controllo bluetooth via Android di un Cannone Laser
   modulo bluetooth HC-05 configurato a 9600 Baud
   connesso a seriale virtuale pin 2 e 3
   Maiocchi Stefano - Vigevano (PV)
*/   

#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial BT(2, 3);  // pin usati per RX e TX
Servo ServoX;             // servomotore asse X
Servo ServoY;             // servomotore asse Y
int Laser = 7;            // pin usato per Laser
int Buzzer = 8;           // pin usato per buzzer piezo
int tono = 1000;          // frequenza suonata dal buzzer
byte joyX, joyY;          // coordinate X e Y in gradi 0-180 ricevute da bluetooth

void setup()  
{
  // Abilitazione porta seriale per debug con Monitor seriale IDE Arduino
  Serial.begin(9600);
  Serial.println("Enabled!");

  // Configurazione data rate per porta seriale virtuale
  BT.begin(9600);

  ServoX.attach(9);  // servomotore asse X usa pin 9
  ServoY.attach(10); // servomotore asse Y usa pin 10

  pinMode(Laser, OUTPUT);
  pinMode(Buzzer, OUTPUT);
}

void loop()
{
  if (BT.available())               // arrivano dati da bluetooth
  {
      if (BT.read()=='D')           // e' un dato valido
      {
         delay(1);                  // attendi dato successivo
         joyX = BT.read();          // leggi coordinata X da bluetooth
         Serial.print(joyX);        // debug su Serial Monitor
         Serial.print("   ");       // di IDE Arduino
         joyY = BT.read();          // leggi coordinata Y da bluetooth
         Serial.println(joyY);      // debug su Serial Monitor
         ServoX.write(joyX);        // sposta servomotore asse X
         ServoY.write(joyY);        // sposta servomotore asse Y
         delay(700);                // attendi spostamento servomotori
         digitalWrite(Laser,HIGH);  // accendi Laser
         tone(Buzzer,tono);         // suona buzzer
         BT.write("H");             // invia a smartphone conferma inizio sparo
         delay(300);                // durata sparo Laser
         digitalWrite(Laser,LOW);   // spegni Laser
         noTone(Buzzer);            // spegni buzzer
         BT.write("L");             // invia a smartphone conferma fine sparo
      }   
  }  
}
