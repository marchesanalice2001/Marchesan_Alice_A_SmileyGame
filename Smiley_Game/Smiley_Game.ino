int vai = 0;// mi dice quando ho premuto qualcosa  // vedo se si puo togliere
int vite = 3;// numero di vite che ho a disposizione
int tempo = 0; // tempo da ritornare alla fine
int posizione = 0;
int punteggio = 0; // punteggio fatto dal giocatore
int controllo = 0;
#define B1 13
#define B2 10
#define B3 8
#include <LiquidCrystal.h>
#include <avr/io.h>
#include <avr/wdt.h>
#define Reset_AVR()   wdt_enable(WDTO_30MS); //reset
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 

void setup() {
Serial.begin(9600);
randomSeed(millis());
  pinMode(B1, INPUT);
  pinMode(B2, INPUT);
  pinMode(B3, INPUT);
  pinMode(A0, OUTPUT);
lcd.clear();
lcd.begin(16, 2);

}
void play()//metodo iniziale che fa partire il gioco appena premo un bottone 
{
  scrivi("   SMILEY GAME    ","PREMI PER GIOCARE");
  while(digitalRead (B1) == LOW ||digitalRead (B2) == LOW || digitalRead (B3) == LOW)
  {
    vai = 1;
    tempo = millis;
  }
}
void ritorno()//metodo finale che mi restituisce il tempo in cui ho giocato e il punteggio
{
  scrivi("GAME OVER","SCORE "+ String(punteggio) + "   TIME "+String(tempo/1000));
  delay(4000);
  Reset_AVR(); 
}
void scrivi ( String n, String m) // metodo che scrive sul'lcd
{
      lcd.begin(16, 2);
      lcd.print(n);
      lcd.setCursor(0, 1);
      lcd.print(m);
}
void cattivo()
{
  int conto = millis;
  int controllo = 0;
  controlla(mostraCattivo());
  bravoocattivo(controllo);
}
void buono()
{
  int conto = millis;
  controlla(mostraBuono());
  bravoocattivo(controllo);
}
int mostraCattivo()
{
    int o = (analogRead(A1) %3);
    if(o == 0 )
    {
      scrivi("SCORE:"+String(punteggio)+"        "+inseriscicuori(vite),"");
    }
    if(o == 1)
    {
      scrivi("SCORE:"+String(punteggio)+"        "+inseriscicuori(vite),"");
    }
    if(o == 2)
    {
      scrivi("SCORE:"+String(punteggio)+"        "+inseriscicuori(vite),"");
    }
    return o;
}
int mostraBuono()
{
    int o = (analogRead(A1) %3);
    if(o == 0 )
    {
      scrivi("SCORE:"+String(punteggio)+"        "+inseriscicuori(vite),"");
    }
    if(o == 1)
    {
      scrivi("SCORE:"+String(punteggio)+"        "+inseriscicuori(vite),"");
    }
    if(o == 2)
    {
      scrivi("SCORE:"+String(punteggio)+"        "+inseriscicuori(vite),"");
    }
    return o;
}

void controlla(int posto)
{
  int secondi = millis;
  while((millis - secondi)<1000 && (digitalRead (B1) == HIGH) ||(digitalRead (B2) == HIGH) || (digitalRead (B3) == HIGH))
  {
    if(digitalRead (B1) == HIGH)
    {
      posizione = 0;
    }
    if(digitalRead (B2) == HIGH)
    {
      posizione = 1;
    }
    if(digitalRead (B3) == HIGH)
    {
      posizione = 2;
    }
    if(posto == posizione)
    {
      controllo = 1;
    }
    else{
      controllo = 0;
    }
  }
}

String inseriscicuori(int v)
{
  String s = "";
  while(v >= 0)
  {
    s = s +"❤";
  }
  return s;
}

void bravoocattivo(int h)
{
  if(h ==0)
  {
    tolgo();
  }
  if(h ==1)
  {
    aggiungo();
  }
}

void tolgo()
{
  vite = vite - 1;
  if(punteggio > 0)
  {
    punteggio = punteggio-10;
  }
}
void aggiungo()
{
  if(vite<3)
  {
    vite = vite+1;
  }
  punteggio = punteggio +10;
}


void loop() 
{
  play();
  while(vai == 1 && vite > 0)
  {
    int r = (analogRead(A1) %2);
    if(r == 1)
    {
      cattivo();
    }
    if(r == 0)
    {
      buono();
    }
  }
  ritorno();
}


