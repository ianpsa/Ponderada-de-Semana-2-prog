#include <ESP32Servo.h>

const int ledVerde = 25;
const int ledAmarelo = 26;
const int ledVermelho = 27;
const int PINO_TRIG = 16;
const int PINO_ECHO = 17;
const int servoPin = 19;
unsigned long timer = 0;

Servo meuServo;

int delayzinho(int tempo) {
  if (millis() - timer >= tempo) {
    timer = millis();
    return 1;
  }
  return 0;
}

void configurarPinos() {
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(PINO_TRIG, OUTPUT);
  pinMode(PINO_ECHO, INPUT);
  meuServo.attach(servoPin);
  meuServo.write(0);
}

void enviarPulsoTrig() {
  digitalWrite(PINO_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PINO_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);
}

float medirDistancia() {
  long duracao = pulseIn(PINO_ECHO, HIGH);
  return (duracao * 0.0343) / 2;
}

void catraca(int graus, float distancia) {
  if (distancia < 10) {
    meuServo.write(graus);
  } else {
    meuServo.write(0);
  }
}

void sinalVermelho() {
  digitalWrite(ledVermelho, HIGH);
  digitalWrite(ledAmarelo, LOW);
  digitalWrite(ledVerde, LOW);
  while (!delayzinho(6000));
}

void sinalAmarelo() {
  digitalWrite(ledVermelho, LOW);
  digitalWrite(ledAmarelo, HIGH);
  digitalWrite(ledVerde, LOW);
  while (!delayzinho(4000));
}

void sinalVerde() {
  digitalWrite(ledVermelho, LOW);
  digitalWrite(ledAmarelo, LOW);
  digitalWrite(ledVerde, HIGH);
  while (!delayzinho(2000));
}

void setup() {
  Serial.begin(9600);
  configurarPinos();
}

void loop() {
  enviarPulsoTrig();
  float distancia = medirDistancia();
  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  sinalVermelho();

  while (distancia > 10) {
    enviarPulsoTrig();
    distancia = medirDistancia();
    Serial.println("Aguardando aproximação para liberar o sinal...");
    sinalVermelho();
  }

  sinalAmarelo();
  catraca(100, distancia);
  sinalVerde();
  sinalAmarelo();
  catraca(0, distancia);
}
