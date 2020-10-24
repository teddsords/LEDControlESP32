/*
* Arquivo: trabalhoLampada.c
* Autores: Leonardo Luis Grando e Teddy Ordoñez Nunñez
* Função do arquivo: Controle geral dem LED inteligente utilizando web server para controlar ON/OFF, Dimmer. Podendo usar sensor LDR e PIR. Com implementação de um RTC.
* Criado em 21 de outubro de 2020
* Modificado em 23 de outubro de 2020
*/
#include <CircusESP32Lib.h>

char ssid[] = "2G_H504";                              // Nome da minha rede
char password[] = "94500519";                         // Senha da rede
char server[] = "www.circusofthings.com";             // Onde esta o servidor
char orderKeyLed[] = "22854";                         // Sinal para desligar ou ligar o LED
char orderKeyOption[] = "21498";                      // Sinal para escolher entre o switch ou brighness
char orderKeyBrightness[] = "23938";                  // Sinal para selecionar o brilho do LED
char token [] = "eyJhbGciOiJIUzI1NiJ9.eyJqdGkiOiIyMTQyIn0.gnLisMgQLv9ZTUff8qvT8ioPq-jwFqFnvmh843NeoYA";    // Token para me identificar no site
CircusESP32Lib circusESP32 (server, ssid, password);

int pwmFrequency = 1000;                             // Definindo a frequencia de operacao do PWM
int pwmChannel = 0;                                  // Definindo o canal que sera utilizado para o PWM
int pwmResolution = 8;                               // Resolucao do PWM
int dutyCycle = 127;                                  // Define a largura do ciclo
int leituraLDR = 0;

#define OUTPUTLEDPIN 23
#define LDRPIN       32
#define ACENDELUZLDR 2500                            // Valor maximo do LDR é 4095
#define DESLIGA      0
#define LIGA         100
void setup() {
    Serial.begin(9600);
  Serial.println(analogRead(15));
  Serial.println("FIRST");
  
  ledcSetup(pwmChannel, pwmFrequency, pwmResolution);        // Configurando o PINO 23 como PWM
  ledcAttachPin (OUTPUTLEDPIN, pwmChannel);                    // Colocando o canal 0 de  PWM no pino 23
  //pinMode(LDRPIN, INPUT);
  circusESP32.begin();                                          // Circus object fazendo a sua conexao

//  Serial.begin(9600);
//  Serial.println(analogRead(15));
//  Serial.println("FIRST");
  }
void loop() {
  //delay (1000);
  double novoStatusLed = circusESP32.read(orderKeyLed, token);
  double statusOption = circusESP32.read(orderKeyOption, token);
  int statusBrightness = circusESP32.read(orderKeyBrightness, token);

  if (statusOption == 0)                            // Se o valor é 0, iremos ligar ou desligar o LED
  {

    // Quando o valor recibido for 0 tem que desligar o LED, caso contrario liga
    if (novoStatusLed == 0)
    {
      ledcWrite(pwmChannel, novoStatusLed);                // Utilizando PWM para controlar o OFF
      Serial.print("Desliguei o LED\n");
    }
    else if (novoStatusLed == 100)
    {
      ledcWrite(pwmChannel, novoStatusLed);                // Utilizando PWM para controlar o ON
      Serial.print("Liguei o LED\n");
    }
  }
  
  else if (statusOption == 1)                   // Se o valor é 1, iremos controlar o brilho do LED com PWM
  {
    ledcWrite(pwmChannel, statusBrightness);
    Serial.print("Valor do PWM: ");
    Serial.println(statusBrightness);
  }
  
  else if (statusOption == 2)
  {
      Serial.println(analogRead(15));
      Serial.println("Oi");
    if (analogRead(LDRPIN) < ACENDELUZLDR)
    {
      ledcWrite(pwmChannel, DESLIGA);
      Serial.print("Valor LDR NO IF: ");
      Serial.println(analogRead(LDRPIN));
      delay(1000);
    }
    else
    {
      ledcWrite(pwmChannel, LIGA);
      Serial.print("Valor LDR NO ELSEIF: ");
      Serial.println(analogRead(LDRPIN));
      delay(1000);
    }
  }
}
