/*
    Arquivo: trabalhoLampada.c
    Autores: Leonardo Luis Grando e Teddy Ordoñez Nunñez
    Função do arquivo: Controle geral dem LED inteligente utilizando web server para controlar ON/OFF, Dimmer.
    Podendo usar sensor LDR e PIR. Com implementação de um RTC.
    Criado em 21 de outubro de 2020
    Modificado em 23 de outubro de 2020
*/

// PARA FAZER USO DO NTP, SERVIDOR DE TEMPO E DIA -----> https://randomnerdtutorials.com/esp32-date-time-ntp-client-server-arduino/#:~:text=Getting%20Date%20and%20Time%20from,results%20on%20the%20Serial%20Monitor.
#include <CircusESP32Lib.h>
#define OUTPUTLEDPIN 23                              // Definindo o valor do pino de saida para o LED
#define LDRPIN       32                              // Definindo o pino de entrada para o sensor LDR
#define ACENDELUZLDR 2500                            // Valor maximo do LDR é 4095
#define DESLIGA      0                               // Definindo o valor de 0 para poder desligar o LED utilizando PWM na porta de saida
#define LIGA         255                             // Definindo o valor de 255 para poder ligar o LED utilizando PWM na porta de saida
#define PIRSENSOR    22                              // Definindo o valor do pino para o sensor PIR

int pwmFrequency = 5000;                             // Definindo a frequencia de operacao do PWM
int pwmChannel = 0;                                  // Definindo o canal que sera utilizado para o PWM
int pwmResolution = 8;                               // Resolucao do PWM
int dutyCycle = 127;                                 // Define a largura do ciclo
bool ledOnOff = false;                               // Variavel para conhecer o status do LED
short novoStatusLed;                                 // Variavel para saber se vamos lugar ou desligar o LED dependendo do valor lido do sinal de circus of things (COT)
short statusOption;                                  // Variavel para saber qual opcao de controle foi escolhida no site de COT      
short statusBrightness;                              // Variavel para saber qual o nivel de brilho escolhido do sinal de COT

char ssid[] = "2G_H504";                              // Nome da minha rede
char password[] = "94500519";                         // Senha da rede
char server[] = "www.circusofthings.com";             // Onde esta o servidor
char orderKeyLed[] = "22854";                         // Sinal para desligar ou ligar o LED
char orderKeyOption[] = "21498";                      // Sinal para escolher entre o switch ou brighness
char orderKeyBrightness[] = "23938";                  // Sinal para selecionar o brilho do LED
char token [] = "eyJhbGciOiJIUzI1NiJ9.eyJqdGkiOiIyMTQyIn0.gnLisMgQLv9ZTUff8qvT8ioPq-jwFqFnvmh843NeoYA";    // Token para me identificar no site
CircusESP32Lib circusESP32 (server, ssid, password);

void setup() {
  Serial.begin(115200);                                        // Inicializando o monitor serial
  pinMode(PIRSENSOR, INPUT);                                   // Definindo o pino onde esta o PIR como entrada
  ledcSetup(pwmChannel, pwmFrequency, pwmResolution);          // Configurando o PINO 23 como PWM
  ledcAttachPin (OUTPUTLEDPIN, pwmChannel);                    // Colocando o canal 0 de  PWM no pino 23
  circusESP32.begin();                                         // Circus object fazendo a sua conexao
}

void loop() {
  novoStatusLed = circusESP32.read(orderKeyLed, token);                   // Realizando a leitura do sinal do SWITCH no site de COT
  statusOption = circusESP32.read(orderKeyOption, token);                 // Realizando a leitura do sinal de OPCAO no site de COT
  statusBrightness = circusESP32.read(orderKeyBrightness, token);         // Realizando a leitura do sinal de DIMMER no site de COT

  Serial.println("Passei no inicio");

  // Sera que teriamos que fazer uma verificacao de se tem o LEDD ligado ou nao?
  // O problea de fazer isto eh que o canal do LED esta em pwm, entao acho que tentarei usar uma variavel para saber o status dela.
  // Olha a linha 54 para a solucao pensada.

  if (digitalRead(PIRSENSOR == 1))                          // Se detectar movimento
  {
    if (ACENDELUZLDR < analogRead(LDRPIN))                  // E luminosidade baixa
    {
      // if (ledOnOff == false) Ligamos o LED, caso contrario faz nada!!!!
      ledcWrite(pwmChannel, LIGA);                          // Ligamos o LED
    }
  }

  if (statusOption == 0)                                    // Se o valor é 0, iremos ligar ou desligar o LED
  {
    if (novoStatusLed == DESLIGA)                           // Quando o valor recibido for 0 tem que desligar o LED, caso contrario liga
    {
      ledcWrite(pwmChannel, novoStatusLed);                // Utilizando PWM para controlar o OFF
      ledOnOff = false;                                    // Nao verifiquei o seu funcionamento ainda
      Serial.println("Desliguei o LED");
      Serial.println(ledOnOff);                            // Tambem nao verifiquei seu funcionamento
    }
    else if (novoStatusLed == LIGA)
    {
      ledcWrite(pwmChannel, novoStatusLed);                // Utilizando PWM para controlar o ON
      ledOnOff = true;                                     // Nao verifiquei seu funcionamento ainda
      Serial.println("Liguei o LED");
      Serial.println(ledOnOff);                            // Tambem nao verifiquei seu funcionamento
    }
  }

  else if (statusOption == 1)                              // Se o valor é 1, iremos controlar o brilho do LED com PWM
  {
    ledcWrite(pwmChannel, statusBrightness);
    if (statusBrightness > 0)                             // nao verifiquei o funcionamento ainda
    {
      ledOnOff = true;
    }
    else if (statusBrightness == 0)                       // Tambem nao verifiuei o seu funcionamento ainda
    {
      ledOnOff = false;
    }
    Serial.print("Valor do PWM: ");                        // Escrevendo no monitor serial o valor recebido para PWM do servidor
    Serial.println(statusBrightness);
  }

  //
  //  else if (statusOption == 2)                             // Se o valor é 1, iremos controlar o LED
  //  {
  //    if (ACENDELUZLDR > analogRead(LDRPIN))               // Caso o valor da leitura seja menor que o valor pre definido para a luminosidade, iremos desligar o LED
  //    {
  //      ledcWrite(pwmChannel, DESLIGA);                    // Utilizando PWM para desligar o LED, ja que o pino foi configurado como uma saida PWM
  //      Serial.print("Valor LDR NO IF: ");                // Escrevendo o valor lido do sensor LDR no monitor serial
  //      Serial.println(analogRead(LDRPIN));               // Lendo o valor entregue pelo sensor LDR
  //      //delay(1000);
  //    }
  //    else                                                // Caso a luminosidade seja baixa, ou seja o valor entregue pelo sensor seja alto, iremos ligar o LED
  //    {
  //      ledcWrite(pwmChannel, LIGA);                      // Utilizando PWM para ligar o LED pois o pino esta configurado para trabalhar como PWM
  //      Serial.print("Valor LDR NO ELSEIF: ");            // Escrevendo o valor lido do sensor LDR no monitor serial
  //      Serial.println(analogRead(LDRPIN));               // Lendo o valor entregue pelo sensor LDR
  //      //delay(1000);
  //    }
  //  }
}
