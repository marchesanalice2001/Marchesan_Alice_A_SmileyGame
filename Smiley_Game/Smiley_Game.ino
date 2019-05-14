#define B1 7
#define B2 8
#define B3 13
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <avr/io.h>
#include <avr/wdt.h>
#define Reset_AVR()   wdt_enable(WDTO_30MS); //reset
int vai = 0;// mi dice quando ho premuto qualcosa
int vite = 3;// numero di vite che ho a disposizione
float tempo = 0; // tempo da ritornare alla fine
int posizione = 3;//posizione che sceglie il giocatore
int punteggio = 0; // punteggio fatto dal giocatore
int controllo = 0;//mi controlla se ho abagliato o no
int timer = 4000;// soglia di tempo che hai per premere 
int record = 0;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
byte Cattivo[8] = {B00000, B01010, B01010, B00000, B01110, B11011, B10001, B00000};
byte Smile[8] = {B00000, B01010, B01010, B00000, B10001, B11011, B01110, B00000};
byte Cuore[8] = {B00000, B01010, B11111, B11111, B11111, B01110, B00100, B00000};
byte CuoreRotto[8] = { B00000, B01010, B10101, B10001, B10001, B01010, B00100, B00000 };
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
void mettirecord()
{ if(record < punteggio)
  {scrivi2("NUOVO RECORD:",String(punteggio));delay(2000);  EEPROM.write(0, punteggio);}
  else{ scrivi2("RECORD NON","BATTUTO:  "+String(record));delay(2000);}
}
void play()//metodo iniziale che fa partire il gioco appena premo un bottone 
{ scrivi2("  SMILEY GAME    "," PREMI E GIOCA "); record = EEPROM.read(0); 
  while(vai == 0)
  { if(digitalRead (B1) == LOW || digitalRead (B2) == LOW || digitalRead (B3) == LOW)
    {   vai = 1;  tempo = millis(); scrivi2("RECORD DA","BATTERE: " + String(record)); delay(2000);}
  }
}
void scrivi2 ( String n, String m) // metodo che scrive sul'lcd solo due stringhe
{     lcd.begin(16, 2);
      lcd.print(n);
      lcd.setCursor(0, 1);
      lcd.print(m);
}
void ritorno()//metodo finale che mi restituisce il tempo in cui ho giocato e il punteggio
{ scrivi("SCORE:"+String(punteggio)+"   ",vite,7,7); delay(1000); mettirecord(); delay(1000);
  scrivi2("      GAME","      OVER");delay(3000);
  scrivi2("SCORE "+ String(punteggio), "TIME "+String((millis()-tempo)/1000)+"sec");
  delay(4000);
  Reset_AVR(); 
}
void scrivi ( String n, int v, int p,int b) // metodo che scrive sul'lcd e decide in che posto mettere la faccina
{     lcd.begin(16, 2);
      lcd.print(n);
      mettivite(v);lcd.setCursor(0,1);
      if(b==0)
      { mettifaccine(p,1);  }
      if(b==1)
      { mettifaccine(p,2);  }
}
void mettifaccine(int p, int n)// metodo che a seconda della p decide dove mettere la faccina
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
void mettivite(int vita)// scrive le vite che hai a disposizione e quelle che hai perso
{     lcd.setCursor(12,0);
      for(int i= 0;i<vita;i++)
      { lcd.write((uint8_t)7);}
      for(int i= 0;i<(3-vita);i++)
      { lcd.write((uint8_t)8);}
}
void cattivo()// fa in modo che compare solo la faccina cattiva ed esegue il controllo
{ int o = (analogRead(A1) %3);
  scrivi("SCORE:"+String(punteggio)+"   ",vite,o,1); 
  controlla(o, 0);
  bravoocattivo(0);
}
void buono()// fa in modo che compare solo la faccina buona ed esegue il controllo
{ int o = (analogRead(A1) %3);
  scrivi("SCORE:"+String(punteggio)+"   ",vite,o,0);
  controlla(o, 1);
  bravoocattivo(1);
}
void controlla(int posto, int b)// da il tempo che il giocatore ha a disposizione e legge cosa ha scelto
{ bool finito = false; float sec = millis();
  while(!finito)
  { if((millis() - sec) >= timer)
    { posizione = 3;  finito = true;   contatimer();   }
    if(digitalRead (B1) == LOW)
    { posizione = 0;  finito = true;   contatimer();   }
    if(digitalRead (B2) == LOW)
    { posizione = 1;  finito = true;   contatimer();   }
    if(digitalRead (B3) == LOW)
    { posizione = 2;  finito = true;   contatimer();   }
  }   
    scrivi2("SCORE:"+String(punteggio),"    "); delay(500);
    uguale(posto, b);
}
void uguale(int pos, int boc)// compara quello che hai scelto con la posizione della faccina e il tipo di faccina
{   if((boc == 0 && posizione != 3 )|| pos == posizione)
    { controllo = 1;  }
    if((boc == 0 && posizione == 3) || (pos != posizione && boc == 1))
    { controllo = 0;  }
}
void contatimer()// modifica il tempo che hai a disposizione per premere
{ if(timer <= 600)
  { timer = 4000; }
  else
  { timer = timer - 200; }
}
void bravoocattivo(int n)// controlla se deve togliere o aggiungere punti e vite
{ if(controllo == 0 && n ==1 || controllo == 1 && n == 0)
  { tolgo();  }
  if(controllo == 1 && n == 1 || controllo == 0 && n == 0)
  { aggiungo(); }
controllo = 0; posizione = 3;
}
void tolgo()// toglie una vita e un tot di punteggio
{ vite = vite-1;
  if(punteggio >= 10)
  { punteggio = punteggio-10; }
}
void aggiungo()// aggiunge una vita se ne avevi gia persa una e aggiunge punti
{ if(vite < 3)
  { vite = vite+1; }
  punteggio = punteggio +10;
}
void loop() //loop gioco
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
