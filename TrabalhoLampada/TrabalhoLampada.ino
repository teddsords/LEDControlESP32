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

#define OUTPUTLEDPIN 23
void setup() {
  Serial.begin(115200);
  ledcSetup(pwmChannel, pwmFrequency, pwmResolution);        // Configurando o PINO 23 como PWM
  ledcAttachPin (OUTPUTLEDPIN, pwmChannel);                    // Colocando o canal 0 de  PWM no pino 23
  circusESP32.begin();                                          // Circus object fazendo a sua conexao

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
     // pinMode(OUTPUTLEDPIN, OUTPUT);                      // Definindo o pin como saida
      //digitalWrite(OUTPUTLEDPIN, LOW);
      ledcWrite(pwmChannel, novoStatusLed);                // Utilizando PWM para controlar o OFF
      Serial.print("Desliguei o LED\n");
    }
    else if (novoStatusLed == 100)
    {
      //pinMode(OUTPUTLEDPIN, OUTPUT);                      // Definindo o pin como saida
      //digitalWrite(OUTPUTLEDPIN, HIGH);
      ledcWrite(pwmChannel, novoStatusLed);                // Utilizando PWM para controlar o ON
      Serial.print("Liguei o LED\n");
    }
  }
  else if (statusOption == 1)                   // Se o valor é 1, iremos controlar o brilho do LED com PWM
  {
//    ledcSetup(PWM_Channel, PWM_Frequency, PWM_Resolution);        // pode ser apagado
//    ledcAttachPin (OUTPUTLEDPIN, PWM_Channel);
    ledcWrite(pwmChannel, statusBrightness);
    Serial.print("Valor do PWM: ");
    Serial.print(statusBrightness);
    Serial.print("\n\n");
  }
}
