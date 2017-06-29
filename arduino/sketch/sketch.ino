// Librerie da usare
#include <LiquidCrystal.h>
#include <_Timer.h>
#include "IRremote.h"

const int ledCampionamento = 13;//posizione led "campionamento"
const int ledReady = 10;// posizione led "ready"
int receiver = 9; //posizione del sensore a infrarossi
//parte del sensore ad ultrasuioni
const int triggerPort = 7;// posizione del emettitore di ultrasuoni
const int echoPort = 8; //posizione del ricevitore ad ultrasuoni

//inizializzazione tempo campioni
const long tempoCampioni=100;
int sled = 0, inizializzato=0; // zona di memorizzazione dello stato del led (1 = acceso; 0 = spento)
long origineTemporale=0, campioneTempo=0, duration=0, distanza=0;

//istanza classe display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//istanza classe timer
_Timer Attesa;


IRrecv irrecv(receiver);    // istanza classe IRecv
decode_results results;     // create instance of decode_results

void setup()
{ 
  pinMode(ledCampionamento,OUTPUT);//configurazione led 
pinMode(ledReady,OUTPUT);

pinMode(triggerPort, OUTPUT );// configurazione sensore ad ultrasuoni 
pinMode(echoPort, INPUT );
//inizializzazione seriale
Serial.begin(115200);
Serial.println("Sensore ultrasuoni: ");

//inizializzazione display
lcd.begin(16, 2); //colonne, righe

//accensione led "Ready"
digitalWrite (ledReady, HIGH);

  irrecv.enableIRIn(); // Start the receiver
  pinMode (13, OUTPUT);
}

void loop()
{
  delay(300);
if (irrecv.decode(&results));// parte che controlla se il telecomando è in funzione

  {
    switch(results.value)

    {

      case 0xFF629D: // vol+
      digitalWrite(ledCampionamento,HIGH);
      sled = 1;
      break;
                      

      case 0xFFA857: // vol-
                   digitalWrite (ledCampionamento, LOW);
      sled = 0; 
                      break; 
                      
    }
    
      irrecv.resume(); // pronto a ricevere un'altro comando
  }  

  if (sled==1)
  {
    if (inizializzato==0)
    {
      //inizializzazione display
      lcd.setCursor(0, 0);
      lcd.print("Distanza :");
      lcd.setCursor(0, 1);
      lcd.print("(cm)");

      //header file campioni
      Serial.println("File di campioni Spazio-Tempo");
      Serial.println("Distanza [cm]  Tempo [millis]");

      //inizializzazione origine temporale
      origineTemporale=millis();
      
      inizializzato=1;
    }

//gestione campione distanza      
    if (Attesa.LeggiAttivo()==0)
    {
      //avvia il timer per il prossimo ciclo
      Attesa.Start(tempoCampioni);

      //campionamento tempo
      campioneTempo=millis()-origineTemporale;
      
      //campionamento distanza
      //porta bassa l'uscita del trigger
      digitalWrite( triggerPort, LOW );
      //invia un impulso di 10microsec su trigger
      digitalWrite( triggerPort, HIGH );
      delayMicroseconds( 10 );
      digitalWrite( triggerPort, LOW );
      duration = pulseIn( echoPort, HIGH );
      distanza = 0.034 * duration / 2;

      //gestione display
      if(distanza > 99)
      {
        lcd.setCursor(13, 0);
        lcd.print(distanza);
      }
      else
      {
        if(distanza > 9)
        {
          lcd.setCursor(13, 0);
          lcd.print(" ");
          lcd.setCursor(14, 0);
          lcd.print(distanza);
        }
        else
        {
          lcd.setCursor(13, 0);
          lcd.print("  ");
          lcd.setCursor(15, 0);
          lcd.print(distanza);
        }
      }  
        
      //gestione porta seriale
      if( duration <= 38000 )
      {
        
        Serial.println(distanza);
        
        
      }
      else
      {
        Serial.println(0);
        
        //Serial.println("Distanza 0  Tempo 0");
      }
    }
  }
  else
  {
    if (inizializzato==1)
    {
      lcd.clear();
      inizializzato=0;
    }
  }


}
