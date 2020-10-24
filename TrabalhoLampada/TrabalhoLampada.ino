/*
    Arquivo: trabalhoLampada.c
    Autores: Leonardo Luis Grando e Teddy Ordoñez Nunñez
    Função do arquivo: Controle geral dem LED inteligente utilizando web server para controlar ON/OFF, Dimmer.
    Podendo usar sensor LDR e PIR. Com implementação de um RTC.
    Criado em 21 de outubro de 2020
    Modificado em 23 de outubro de 2020
*/
#include <CircusESP32Lib.h>
#define OUTPUTLEDPIN 23                              // Definindo o valor do pino de saida para o LED
#define LDRPIN       32                              // Definindo o pino de entrada para o sensor LDR
#define ACENDELUZLDR 2500                            // Valor maximo do LDR é 4095
#define DESLIGA      0                               // Definindo o valor de 0 para poder desligar o LED utilizando PWM na porta de saida
#define LIGA         100                             // Definindo o valor de 100 para poder ligar o LED utilizando PWM na porta de saida

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

void setup() {
  Serial.begin(115200);
  ledcSetup(pwmChannel, pwmFrequency, pwmResolution);          // Configurando o PINO 23 como PWM
  ledcAttachPin (OUTPUTLEDPIN, pwmChannel);                    // Colocando o canal 0 de  PWM no pino 23
  circusESP32.begin();                                         // Circus object fazendo a sua conexao
}
void loop() {
  double novoStatusLed = circusESP32.read(orderKeyLed, token);
  double statusOption = circusESP32.read(orderKeyOption, token);
  int statusBrightness = circusESP32.read(orderKeyBrightness, token);

  if (statusOption == 0)                                    // Se o valor é 0, iremos ligar ou desligar o LED
  {
    if (novoStatusLed == 0)                                // Quando o valor recibido for 0 tem que desligar o LED, caso contrario liga
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

  else if (statusOption == 1)                              // Se o valor é 1, iremos controlar o brilho do LED com PWM
  {
    ledcWrite(pwmChannel, statusBrightness);
    Serial.print("Valor do PWM: ");                        // Escrevendo no monitor serial o valor recebido para PWM do servidor
    Serial.println(statusBrightness);
  }

  else if (statusOption == 2)                             // Se o valor é 1, iremos controlar o LED
  {
    if (ACENDELUZLDR > analogRead(LDRPIN))               // Caso o valor da leitura seja menor que o valor pre definido para a luminosidade, iremos desligar o LED
    {
      ledcWrite(pwmChannel, DESLIGA);                    // Utilizando PWM para desligar o LED, ja que o pino foi configurado como uma saida PWM
      Serial.print("Valor LDR NO IF: ");                // Escrevendo o valor lido do sensor LDR no monitor serial
      Serial.println(analogRead(LDRPIN));               // Lendo o valor entregue pelo sensor LDR
      //delay(1000);
    }
    else                                                // Caso a luminosidade seja baixa, ou seja o valor entregue pelo sensor seja alto, iremos ligar o LED
    {
      ledcWrite(pwmChannel, LIGA);                      // Utilizando PWM para ligar o LED pois o pino esta configurado para trabalhar como PWM
      Serial.print("Valor LDR NO ELSEIF: ");            // Escrevendo o valor lido do sensor LDR no monitor serial
      Serial.println(analogRead(LDRPIN));               // Lendo o valor entregue pelo sensor LDR
      //delay(1000);
    }
  }
}
