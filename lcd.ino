// Bibliotecas------------------------------------------------------------------
#include <Wire.h>              // Permite comunicação com I2C
#include <LiquidCrystal_I2C.h> // Controla o LCD
#include <RTClib.h>            // Controla o sensor de Tempo Real
#include <DHT.h>               // Controla sensor de temperatura e humidade
#include <Keypad.h>            // Controla o teclado
//------------------------------------------------------------------------------

// Pinos------------------------------------------------------------------------
#define DHTPIN 2
#define DHTTYPE DHT11
#define button 3
#define vib 4
#define buz 5
//------------------------------------------------------------------------------

// Setup de variáveis para o objeto teclado-------------------------------------
char mapaTeclas[4][3] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'*', '0', '#'}};
byte pino_linha[4] = {6, 7, 8, 9};
byte pino_coluna[3] = {10, 11, 12};
Keypad teclado = Keypad(makeKeymap(mapaTeclas), pino_linha, pino_coluna, 4, 3);
//------------------------------------------------------------------------------

// Setup de variáveis para data e botão-----------------------------------------
char daysOfTheWeek[7][12] = {"DOM", "SEG", "TER", "QUA", "QUI", "SEX", "SAB"};
int buttonState = 0;
//------------------------------------------------------------------------------

// Setup de variáveis para controle do despertador------------------------------
//bool inicio = false; // Variáveis de controle
//bool final = false;
//------------------------------------------------------------------------------

// Objetos----------------------------------------------------------------------
LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS1307 rtc;
DHT dht(DHTPIN, DHTTYPE);
//------------------------------------------------------------------------------

// Setup------------------------------------------------------------------------
void setup()
{
  // Inicialização do sistema --------------------------------------------------
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (!rtc.isrunning())
  {
    Serial.println("RTC is NOT running, let's set the time!");
  }
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();

  // Declaração de Pinos--------------------------------------------------------
  pinMode(vib, OUTPUT);
  pinMode(button, INPUT);
  pinMode(buz, OUTPUT);

  // Mensagem inicial ----------------------------------------------------------
  /*digitalWrite(buz, HIGH);
  delay(100);
  digitalWrite(buz, LOW);*/
  bip();

  lcd.setCursor(5, 1);
  lcd.print(F("- Relogio -"));
  lcd.setCursor(3, 2);
  lcd.print(F("- Despertador -"));
  delay(3000);
  lcd.clear();

  for (int i = 0; i < 10; i++)
  {
    relogio();
    delay(1000);
  }
}

// Loop-----------------------------------------------------------------------
void loop()
{
  hibernar();
  digito();
  botao();
}

// Funções----------------------------------------------------------------------
void relogio()
{
  // Config sensor de tempo
  DateTime now = rtc.now();
  //lcd.setBacklight(HIGH);

  // Config sensor de humidade e temperatura
  float h = dht.readHumidity();
  // Temperature em Celsius (default)
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    Serial.println(F("Falha de leitura do sensor DHT!"));
    return;
  }
  float hic = dht.computeHeatIndex(t, h, false);

  // Impressão
  lcd.setCursor(0, 0);
  lcd.print("Data: ");
  lcd.print(now.day(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.year(), DEC);
  lcd.print(' ');
  lcd.setCursor(17, 0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  lcd.setCursor(0, 1);
  lcd.print("Hora: ");
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);

  lcd.setCursor(0, 2);
  lcd.print(F("Humidade: "));
  lcd.setCursor(10, 2);
  lcd.print(round(h));
  lcd.setCursor(12, 2);
  lcd.print(F(" %"));

  lcd.setCursor(0, 3);
  lcd.print(F("Temperatura: "));
  lcd.setCursor(13, 3);
  lcd.print(round(t));
  lcd.setCursor(15, 3);
  lcd.write(223); // Caracter °
  lcd.print(F("C"));
  //lcd.write(32);  // Caracter espaço
}

void botao()
{
  buttonState = digitalRead(button);
  if (buttonState == HIGH)
  {
    digitalWrite(vib, HIGH);
  }
  else
    digitalWrite(vib, LOW);
}

void bip()
{
  digitalWrite(buz, HIGH);
  delay(100);
  digitalWrite(buz, LOW);
  delay(230);

  digitalWrite(buz, HIGH);
  delay(100);
  digitalWrite(buz, LOW);
  delay(50);

  digitalWrite(buz, HIGH);
  delay(100);
  digitalWrite(buz, LOW);
  delay(50);

  digitalWrite(buz, HIGH);
  delay(100);
  digitalWrite(buz, LOW);
  delay(250);

  digitalWrite(buz, HIGH);
  delay(100);
  digitalWrite(buz, LOW);
  delay(550);

  digitalWrite(buz, HIGH);
  delay(100);
  digitalWrite(buz, LOW);
  delay(220);

  digitalWrite(buz, HIGH);
  delay(100);
  digitalWrite(buz, LOW);
}

void hibernar()
{
  lcd.noDisplay();
  lcd.noBacklight();

  char tecla = teclado.getKey();

  if (tecla != NO_KEY)
  {
    for (int i = 0; i < 20; i++)
    {
      relogio();
      lcd.display();
      lcd.backlight();
      delay(1000);
      if (tecla != NO_KEY)
      {
        continue;
      }
    }
  }
  else
  {
    lcd.noBacklight();
    lcd.noDisplay();
  }
}

void digito()
{
  char tecla = teclado.getKey();
  if (tecla != NO_KEY)
  {
    Serial.print(tecla);
  }
}
