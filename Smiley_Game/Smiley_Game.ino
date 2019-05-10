#define B1 7
#define B2 8
#define B3 13
#include <LiquidCrystal.h>
#include <avr/io.h>
#include <avr/wdt.h>
#define Reset_AVR()   wdt_enable(WDTO_30MS); //reset
int vai = 0;// mi dice quando ho premuto qualcosa
int vite = 3;// numero di vite che ho a disposizione
float tempo = 0; // tempo da ritornare alla fine
int posizione = 3;
int punteggio = 0; // punteggio fatto dal giocatore
int controllo = 0;
int timer = 4000;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
byte Cattivo[8] = {B00000,B01010,B01010,B00000,B01110,B11011,B10001,B00000};
byte Smile[8] = {B00000,B01010,B01010,B00000,B10001,B11011,B01110,B00000};
byte Cuore[8] = {B00000,B01010,B11111,B11111,B11111,B01110,B00100,B00000};
byte CuoreRotto[8] = { B00000,B01010,B10101, B10001,B10001,B01010,B00100,B00000 };
void setup() {
Serial.begin(9600);
randomSeed(millis());
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
  pinMode(A0, OUTPUT);
  lcd.clear();
  lcd.createChar(1, Smile);
  lcd.createChar(2, Cattivo);
  lcd.createChar(7, Cuore);
  lcd.createChar(8, CuoreRotto);
}
void play()//metodo iniziale che fa partire il gioco appena premo un bottone 
{ scrivi2("  SMILEY GAME    "," PREMI E GIOCA ");
  while(vai == 0)
  { if(digitalRead (B1) == LOW || digitalRead (B2) == LOW || digitalRead (B3) == LOW)
    {   vai = 1;
       tempo = millis();
    }
  }
}
void scrivi2 ( String n, String m) // metodo che scrive sul'lcd
{     lcd.begin(16, 2);
      lcd.print(n);
      lcd.setCursor(0, 1);
      lcd.print(m);
}
void ritorno()//metodo finale che mi restituisce il tempo in cui ho giocato e il punteggio
{ scrivi2("      GAME","      OVER");delay(3000);
  scrivi2(" SCORE "+ String(punteggio), " TIME "+String((millis()-tempo)/1000)+"sec");
  delay(4000);
  Reset_AVR(); 
}
void scrivi ( String n, int v, int p,int b) // metodo che scrive sul'lcd
{     lcd.begin(16, 2);
      lcd.print(n);
      mettivite(v);lcd.setCursor(0,1);
      if(b==0)
      { mettifaccine(p,1);  }
      if(b==1)
      { mettifaccine(p,2);  }
}
void mettifaccine(int p, int n)
{     if(p==0)
      { lcd.setCursor(3,1);
        lcd.write((uint8_t)n);
      }
      if(p==1)
      { lcd.setCursor(7,1);
        lcd.write((uint8_t)n); 
      }
      if(p==2)
      { lcd.setCursor(11,1);
        lcd.write((uint8_t)n);
      }
}
void mettivite(int vita)
{     lcd.setCursor(12,0);
      for(int i= 0;i<vita;i++)
      { lcd.write((uint8_t)7);}
      for(int i= 0;i<(3-vita);i++)
      { lcd.write((uint8_t)8);}
}
void cattivo()
{ int o = (analogRead(A1) %3);
  scrivi("SCORE:"+String(punteggio)+"   ",vite,o,1); 
  controlla(o, 0);
  bravoocattivo(0);
}
void buono()
{ int o = (analogRead(A1) %3);
  scrivi("SCORE:"+String(punteggio)+"   ",vite,o,0);
  controlla(o, 1);
  bravoocattivo(1);
}
void controlla(int posto, int b)
{ bool finito = false; float sec = millis();
  while(!finito)
  {  if((millis() - sec) >= timer)
    { posizione = 3; finito = true; contatimer(); }
    if(digitalRead (B1) == LOW)
    { posizione = 0; finito = true; contatimer(); }
    if(digitalRead (B2) == LOW)
    { posizione = 1; finito = true; contatimer(); }
    if(digitalRead (B3) == LOW)
    { posizione = 2; finito = true; contatimer(); }
  }   
    scrivi2("SCORE:"+String(punteggio),"    "); delay(1000);
    uguale(posto, b);
}
void uguale(int pos, int boc)
{   if((boc == 0 && posizione != 3 )|| pos == posizione)
    { controllo = 1;  }
    if((boc == 0 && posizione == 3) || (pos != posizione && boc == 1))
    { controllo = 0;  }
}
void contatimer()
{ if(timer <= 1000)
  { timer = 4000; }
  else
  { timer = timer - 200; }
}
void bravoocattivo(int n)
{ if(controllo == 0 && n ==1 || controllo == 1 && n == 0)
  { tolgo();  }
  if(controllo == 1 && n == 1 || controllo == 0 && n == 0)
  { aggiungo(); }
controllo = 0; posizione = 3;
}
void tolgo()
{ vite = vite-1;
  if(punteggio >= 10)
  { punteggio = punteggio-10; }
}
void aggiungo()
{ if(vite < 3)
  { vite = vite+1; }
  punteggio = punteggio +10;
}
void loop() 
{ digitalWrite(A0, HIGH);
  play();delay(1000);
  while(vite > 0)
  { int r = (analogRead(A1) %2);
    if(r == 1)
    { cattivo();}
    else
    { buono();}
  }
  ritorno();
}
