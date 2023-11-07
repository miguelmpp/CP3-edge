#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(13, 12, 11, 7, 8, 9, 10);

// Sensor de Luminosidade
int ledverde = 4;    // Led Verde
int ledamarelo = 5;  // Led Amarelo
int ledvermelho = 6; // Led Vermelho
int LDR = A0;        // Fotorresistor
int valorLDR = 0;    // Vai ler as informações do LDR (LDR = 0)
int buzzer = 3;      // Variável do piezo

// Sensores de Temperatura e Umidade
const int analogIn = A1; // Sensor de Temperatura
int umidade = 0;        // Sensor de Umidade
int ValorBruto = 0;     // Valor Bruto
double Voltagem = 0;    // Voltagem
double tempC = 0;       // Temperatura em °C

int ledvermelhoT = 10;  // Led de Temperatura
int ledamareloT = 11;   // Led de Temperatura

// HC-SR04 Sensor connection
const int trigPin = 2;
const int echoPin = A3;
long duration;

void setup() {
  pinMode(ledverde, OUTPUT);
  pinMode(ledamarelo, OUTPUT);
  pinMode(ledvermelho, OUTPUT);
  pinMode(LDR, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(ledamareloT, OUTPUT);
  pinMode(ledvermelhoT, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  lcd.begin(16, 2); // Inicializa o LCD
  lcd.print("Inicializando...");

  delay(2000); // Aguarde 2 segundos para o LCD

  lcd.clear();
}

void displaySensorInfo(int distancia, int luminosidade, double temperatura, int umidade) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("uni livres:");
  lcd.print(distancia);
  lcd.print("");
  lcd.setCursor(0, 1);
  lcd.print("Lum: ");
  lcd.print(luminosidade);
  lcd.print("  Temp: ");
  lcd.print(temperatura);
  lcd.print("C  Umid: ");
  lcd.print(umidade);
  lcd.print("%");
}

void loop() {
  valorLDR = analogRead(LDR);

  // Luminosidade adequada (LED VERDE)
  if (valorLDR <= 100) {
    digitalWrite(ledvermelho, LOW);
    digitalWrite(ledamarelo, LOW);
    digitalWrite(ledverde, HIGH);
    noTone(buzzer);
  } else if (valorLDR > 100 && valorLDR < 150) {
    // Luminosidade em alerta (LED AMARELO)
    digitalWrite(ledvermelho, LOW);
    digitalWrite(ledamarelo, HIGH);
    digitalWrite(ledverde, LOW);
    noTone(buzzer);
  } else if (valorLDR >= 150) {
    // Problema de luminosidade (LED VERMELHO)
    digitalWrite(ledvermelho, HIGH);
    digitalWrite(ledamarelo, LOW);
    digitalWrite(ledverde, LOW);
    tone(buzzer, 1000);
  }

  // Sensor de Temperatura
  ValorBruto = analogRead(analogIn);
  Voltagem = (ValorBruto / 1023.0) * 5000;
  tempC = (Voltagem - 500) * 0.1;

  if (tempC >= 10 && tempC <= 15) {
    digitalWrite(ledvermelhoT, LOW);
    digitalWrite(ledamareloT, LOW);
    noTone(buzzer);
  } else if (tempC > 16) {
    digitalWrite(ledvermelhoT, HIGH);
    digitalWrite(ledamareloT, LOW);
    tone(buzzer, 2000);
  } else if (tempC < 9) {
    digitalWrite(ledamareloT, HIGH);
    digitalWrite(ledvermelhoT, LOW);
  }

  // Sensor de Umidade (Potenciômetro)
  umidade = analogRead(A2);
  int umidade_P = map(umidade, 0, 1023, 0, 100);

  displaySensorInfo(distanceCM(), valorLDR, tempC, umidade_P); // Mostra todas as leituras no LCD
  delay(300);  // Mantém as leituras no LCD por 3 segundos
}

int distanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  int Y;
  Y = duration * 0.034 / 2;
  return Y;
}
