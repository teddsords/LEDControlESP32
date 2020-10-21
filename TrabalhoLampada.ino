#include <CircusESP32Lib.h>

char ssid[] = "2G_H504";                              // Nome da minha rede
char password[] = "94500519";                         // Senha da rede
char server[] = "www.circusofthings.com";             // Onde esta o servidor
char orderKeyLed[] = "22854";                         // Sinal para desligar ou ligar o LED
char orderKeyOption[] = "21498";                      // Sinal para escolher entre o switch ou brighness
char orderKeyBrightness[] = "23938";                  // Sinal para selecionar o brilho do LED
char token [] = "eyJhbGciOiJIUzI1NiJ9.eyJqdGkiOiIyMTQyIn0.gnLisMgQLv9ZTUff8qvT8ioPq-jwFqFnvmh843NeoYA";    // Token para me identificar no site
CircusESP32Lib circusESP32 (server, ssid, password);

int PWM_Frequency = 1000;                             // Definindo a frequencia de operacao do PWM
int PWM_Channel = 0;                                  // Definindo o canal que sera utilizado para o PWM
int PWM_Resolution = 8;                               // Resolucao do PWM
int dutyCycle = 127;                                  // Define a largura do ciclo

#define OUTPUTLEDPIN 23
void setup() {
  Serial.begin(115200);
//  ledcSetup(PWM_Channel, PWM_Frequency, PWM_Resolution);
//  ledcAttachPin (OUTPUTLEDPIN, PWM_Channel);
  pinMode(OUTPUTLEDPIN, OUTPUT);                      // Definindo o pin como saida
  circusESP32.begin();                                // Circus object fazendo a sua conexao

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
      digitalWrite(OUTPUTLEDPIN, LOW);
      Serial.print("Desliguei o LED\n");
    }
    else if (novoStatusLed == 1)
    {
      digitalWrite(OUTPUTLEDPIN, HIGH);
      Serial.print("Liguei o LED\n");
    }
  }
  else if (statusOption == 1)                   // Se o valor é 1, iremos controlar o brilho do LED com PWM
  {
    ledcSetup(PWM_Channel, PWM_Frequency, PWM_Resolution);
    ledcAttachPin (OUTPUTLEDPIN, PWM_Channel);
    ledcWrite(PWM_Channel, statusBrightness);
    Serial.print("Valor do PWM: ");
    Serial.print(statusBrightness);
    Serial.print("\n\n");
  }
}
