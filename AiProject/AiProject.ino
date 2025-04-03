#include "NeuralNetworkAI.h"
#include <MemoryFree.h>  

String inputBuffer;

void printLoadingBar(int progress) {
  int pos = (progress * 50) / 100;
  Serial.print('[');
  for (int i = 0; i < 50; ++i) Serial.print(i < pos ? '=' : (i == pos ? '>' : ' '));
  Serial.print("] "); Serial.print(progress); Serial.println('%');
}

void printMemoryUsage() {
  Serial.print(F("Memory Usage: "));
  Serial.print(3072 - freeMemory());
  Serial.println(F(" / 3072 bytes used."));
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println(F("========================================\n"
                   "|      ██   ██    ██    ███    ███      |\n"
                   "|      ██   ██    ██    ████  ████      |\n"
                   "|      ███████    ██    ██ ████ ██      |\n"
                   "|      ██   ██    ██    ██  ██  ██      |\n"
                   "|      ██   ██    ██    ██      ██ v2   |\n"
                   "|--------------------------------------|\n"
                   "|    Welcome to KOMVORM AI! ™          |\n"
                   "|   High Intelligent Learning Machine  |\n"
                   "|    © 2025 PalorderSoftworks          |\n"
                   "|--------------------------------------|\n"
                   "========================================\n"));

  delay(5000);

  const char* messages[] = {
    "Booting KOMVORM! AI...", "Checking memory...", 
    "Initializing logic core...", "Initiating neural pathways...", 
    "Finalizing setup...", "System Ready.", "Initiating Auto feed"
  };

  for (int i = 0; i < 6; ++i) {
    Serial.println(messages[i]);
    delay(1000);
    printMemoryUsage();
    printLoadingBar((i + 1) * 15);
    delay(500);
  }

  delay(5000);
  Serial.println(messages[6]);
  delay(1000);
  autoFeed();
  delay(10000);

  Serial.println(F("If your wondering why there will be no loading bar nor messages its because this is getting stored in flash memory so i guess you can test now"));
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (inputBuffer.length() > 0) {
        handleInput(inputBuffer);
        inputBuffer = "";
      }
    } else inputBuffer += c;
  }
}
