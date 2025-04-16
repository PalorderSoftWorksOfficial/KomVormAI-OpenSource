#include "NeuralNetworkAI.h"
#include <MemoryFree.h>  
#include <avr/pgmspace.h>
#include <avr/boot.h>
#include "StartupExtra.h"
// remember
String inputBuffer;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println(F("========================================\n"
                   "|      ██   ██    ██    ███    ███     |\n"
                   "|      ██   ██    ██    ████  ████     |\n"
                   "|      ███████    ██    ██ ████ ██     |\n"
                   "|      ██   ██    ██    ██  ██  ██     |\n"
                   "|      ██   ██    ██    ██      ██ v1  |\n"
                   "|--------------------------------------|\n"
                   "|  Welcome to KOMVORM AI! ™ REWRITTEN  |\n"
                   "|   High Intelligent Learning Machine  |\n"
                   "| Fun-fact its dumb cuz almost no data |\n"
                   "|    © 2025 PalorderSoftworks          |\n"
                   "|--------------------------------------|\n"
                   "========================================\n"));

  delay(5000);

  const char* messages[] = {
    "Booting KOMVORM! AI...", "Checking memory...", 
    "Initializing logic core...", "Initiating neural pathways...", 
    "Finalizing setup...", "System Ready.", "You can chat now."
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
  Serial.println(F("If your wondering why there will be no loading bar nor messages its because this is getting stored in flash memory so i guess you can test now"));
}

void loop() {
  // Process serial input without blocking
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
  
  // Avoid blocking with delays
  // Check other system tasks here if needed (like sensors or other processes)
  // This loop is always running and non-blocking
  
  // Example of a background task without using delay
  // If you need to perform periodic tasks, use millis() to avoid blocking
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis(); 
  if (currentMillis - previousMillis >= 10) {  // Run every 10Ms
    
    previousMillis = currentMillis;
  }
}
