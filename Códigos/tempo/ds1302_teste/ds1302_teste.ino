
// --- Bibliotecas Auxiliares ---
#include <virtuabotixRTC.h>                    //biblioteca para o RTC DS1302      

// --- Mapeamento de Hardware ---
#define   clk   7
#define   dat   6
#define   rst   5


// --- Constantes Auxiliares ---
#define   segL       15
#define   minL       32
#define   horL       11
#define   d_semL      4
#define   d_mesL     18
#define   mesL        4
#define   anoL     2025


// --- Declaração de Objetos ---
virtuabotixRTC   myRTC(clk, dat, rst);         //declara objeto para o RTC


// --- Protótipo das Funções ---
void DS1302();
void week(int dayW);


void setup()  
{        
  Serial.begin(9600);
  //Após, comente a linha abaixo e faça upload novamente. 
  myRTC.setDS1302Time(segL, minL, horL, d_semL, d_mesL, mesL, anoL);
  
} 

void loop()  
{
  DS1302();
  
}


// --- Desenvolvimento das Funções ---
void DS1302()
{
  myRTC.updateTime();
  
  Serial.print(" -> ");
  week(myRTC.dayofweek);
  Serial.print(" | ");
  Serial.print(myRTC.dayofmonth);
  Serial.print("/");
  Serial.print(myRTC.month);
  Serial.print("/");
  Serial.print(myRTC.year);
  Serial.print(" | ");
  if(myRTC.hours < 10) Serial.print("0");
  Serial.print(myRTC.hours);
  Serial.print(":");
  if(myRTC.minutes < 10) Serial.print("0");
  Serial.print(myRTC.minutes);
  Serial.print(":");
  if(myRTC.seconds < 10) Serial.print("0");
  Serial.println(myRTC.seconds);
  delay(1000);
  
}

 
void week(int dayW)
{
  
  switch(dayW)
  {
    case 1: Serial.print("Dom"); break;
    case 2: Serial.print("Seg"); break;
    case 3: Serial.print("Ter"); break;
    case 4: Serial.print("Qua"); break;
    case 5: Serial.print("Qui"); break;
    case 6: Serial.print("Sex"); break;
    case 7: Serial.print("Sab"); break;
   
  }
  
}
