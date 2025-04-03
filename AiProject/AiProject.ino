#include "NeuralNetworkAI.h"
#include <MemoryFree.h>  // Include memory library for memory check

// Declare inputBuffer as extern in the header file
String inputBuffer;


void printLoadingBar(int progress) {
  int barWidth = 50;
  Serial.print("[");
  int pos = (progress * barWidth) / 100;
  for (int i = 0; i < barWidth; ++i) {
    if (i < pos) {
      Serial.print("=");
    } else if (i == pos) {
      Serial.print(">");
    } else {
      Serial.print(" ");
    }
  }
  Serial.print("] ");
  Serial.print(progress);
  Serial.println("%");
}

void printMemoryUsage() {
  int freeMem = freeMemory();
  int totalMem = 2048;
  Serial.print("Memory Usage: ");
  Serial.print(totalMem - freeMem);
  Serial.print(" / ");
  Serial.print(totalMem);
  Serial.println(" bytes used.");
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Display ASCII Art & Loading
     Serial.println(F("========================================"));
  Serial.println(F("|      ██   ██    ██    ███    ███      |"));
  Serial.println(F("|      ██   ██    ██    ████  ████      |"));
  Serial.println(F("|      ███████    ██    ██ ████ ██      |"));
  Serial.println(F("|      ██   ██    ██    ██  ██  ██      |"));
  Serial.println(F("|      ██   ██    ██    ██      ██ v2    |"));
  Serial.println(F("|--------------------------------------|"));
  Serial.println(F("|    Welcome to KOMVORM AI! ™          |"));
  Serial.println(F("|   high Intelligent learning machine |"));
  Serial.println(F("|    © 2025 PalorderSoftworks      |"));
  Serial.println(F("|--------------------------------------|"));
  Serial.println(F("========================================"));

  delay(5000);
  
  Serial.println("Booting KOMVORM! AI...");
  delay(1000);
  
  // Display Memory Check and Loading Bar
  printMemoryUsage();
  printLoadingBar(10);  // Initial loading status
  delay(500);

  Serial.println("Checking memory...");
  delay(1000);
  printMemoryUsage();
  printLoadingBar(30);
  delay(500);

  Serial.println("Initializing logic core...");
  delay(1000);
  printMemoryUsage();
  printLoadingBar(50);
  delay(500);

  Serial.println("Initiating neural pathways...");
  delay(1000);
  printMemoryUsage();
  printLoadingBar(70);
  delay(500);

  Serial.println("Finalizing setup...");
  delay(1000);
  printMemoryUsage();
  printLoadingBar(90);
  delay(500);

  Serial.println("System Ready.");
  delay(1000);
  printMemoryUsage();
  printLoadingBar(100);
  delay(5000);
  Serial.println("Initiating Auto feed");
  delay(1000);
  autoFeed();
  delay(10000);
  Serial.println("If your wondering why there will be no loading bar nor messages its because this is getting stored in flash memory so i guess you can test now");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (inputBuffer.length() > 0) {
        handleInput(inputBuffer);
        inputBuffer = "";
      }
    } else {
      inputBuffer += c;
    }
  }
}
