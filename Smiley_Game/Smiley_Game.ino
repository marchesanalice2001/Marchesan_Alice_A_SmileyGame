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
int posizione = 0;
int punteggio = 0; // punteggio fatto dal giocatore
int controllo = 0;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
byte Smile[8] = {B00000,B01010,B01010,B00000,B10001,B11011,B01110,B00000};
byte Cuore[8] = {B00000,B01010,B11111,B11111,B11111,B01110,B00100,B00000};
byte Cattivo[8] = {B00000,B01010,B01010,B00000,B01110,B11011,B10001,B00000};

void setup() {
Serial.begin(9600);
randomSeed(millis());
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
  pinMode(A0, OUTPUT);
  lcd.clear();
  lcd.createChar(0, Smile);
  lcd.createChar(1, Cuore);
  lcd.createChar(2, Cattivo);
  lcd.begin(16, 2);
}

void play()//metodo iniziale che fa partire il gioco appena premo un bottone 
{ 
  scrivi2("  SMILEY GAME    "," PREMI E GIOCA ");
  while(vai == 0)
  {
    if(digitalRead (B1) == LOW || digitalRead (B2) == LOW || digitalRead (B3) == LOW)
    {
       vai = 1;
       tempo = millis();
    }
  }
}
void scrivi2 ( String n, String m) // metodo che scrive sul'lcd
{
      lcd.begin(16, 2);
      lcd.print(n);
      lcd.setCursor(0, 1);
      lcd.print(m);
}
void ritorno()//metodo finale che mi restituisce il tempo in cui ho giocato e il punteggio
{
  scrivi2("      GAME","      OVER");delay(2000);
  scrivi2("SCORE "+ String(punteggio), " TIME "+String(tempo/1000));
  delay(4000);
  Reset_AVR(); 
}
void scrivi ( String n, int v, int p,int b) // metodo che scrive sul'lcd
{
      lcd.begin(16, 2);
      lcd.print(n);
      mettivite(v);
      lcd.setCursor(0, 1);
      if(b==0)
      {
        mettifaccine(p,0);
      }
      if(b==1)
      {
        mettifaccine(p,2);
      }
}
void mettifaccine(int p, int n)
{
  if(p==0)
      { 
        lcd.setCursor(3,1);
        lcd.write(byte(n));
      }
      if(p==1)
      {
        lcd.setCursor(7,1);
        lcd.write(byte(n)); 
      }
      if(p==2)
      {
        lcd.setCursor(11,1);
        lcd.write(byte(n));
      }
}
void mettivite(int v)
{
   for(int i= 0;i<v;i++)
      {
        lcd.write(byte(1));
      }
}

void cattivo()
{ int o = (analogRead(A1) %3);
  scrivi("SCORE:"+String(punteggio)+"   ",vite,o,0); 
  int controllo = 0;
  controlla(o);
  bravoocattivo(0);
}
void buono()
{ int o = (analogRead(A1) %3);
  scrivi("SCORE:"+String(punteggio)+"   ",vite,o,1);
  controlla(o);
  bravoocattivo(1);
}
void controlla(int posto)
{
  int secondi = millis();
  while((millis() - secondi)<=4000 || (digitalRead (B1) == LOW) ||(digitalRead (B2) == LOW) || (digitalRead (B3) == LOW))
  {
    if(digitalRead (B1) == LOW)
    {
      posizione = 0;
    }
    if(digitalRead (B2) == LOW)
    {
      posizione = 1;
    }
    if(digitalRead (B3) == LOW)
    {
      posizione = 2;
    }
  }
    if(posto == posizione)
    {
      controllo = 1;
    }
    else
    {
      controllo = 0;
    }
}
void bravoocattivo(int n)
{
  bool finito = false;
  while(!finito)
  {
  if(controllo == 0 && n ==1 || controllo == 1 && n == 0)
  {
    tolgo(); finito = true;
  }
  if(controllo == 1 && n == 1 || controllo == 0 && n == 0)
  {
    aggiungo(); finito = true;
  }
}
controllo = 0;posizione = 0;
}
void tolgo()
{
  vite = vite--;
  if(punteggio >10)
  {
    punteggio = punteggio-10;
  }
}
void aggiungo()
{
  if(vite < 3)
  {
    vite = vite++;
  }
  punteggio = punteggio +10;
}
void loop() 
{
  digitalWrite(A0, HIGH);
  play();
  while(vite > 0)
  {
    int r = (analogRead(A1) %2);
    if(r == 1)
    {
      cattivo();delay(1000);
    }
    else
    {
      buono();delay(1000);
    }
  }
  ritorno();
}
