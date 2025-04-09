#ifndef AI_CORE_H
#define AI_CORE_H

#include <Arduino.h>
#include <EEPROM.h>
#include "games.h"
const int EEPROM_START = 100, PAIR_SIZE = 64, MAX_PAIRS = (EEPROM.length() - EEPROM_START) / PAIR_SIZE;
const char* vocabulary[] = {"hello", "creator", "neural", "network", "hi", "hey", "how", "are", "you", "good", "bad", "yes", "no", "what", "is", "up", "doing", "today", "there", "i", "dont", "know", "fine", "well", "why", "your", "name", "who", "help", "smart", "response", "reset"};
const int vocabSize = sizeof(vocabulary) / sizeof(vocabulary[0]);
bool trainingDone = false, inCooldown = false, inKernelTerminal = false;
unsigned long cooldownEnd = 0;
extern String inputBuffer;
String response = "";
const String illegalKeywords[] = {"real", "fake", "exist", "simulation", "not real", "are you real", "aren't real", "fuck you"};

const String kernelPassword = "dev_access"; // Kernel password for terminal access
String enteredPassword = ""; // User input for the password
unsigned long lastMillis = 0;  // Used for simple clock

bool isIllegalInput(const String& input) {
  for (const auto& keyword : illegalKeywords) if (input.indexOf(keyword) >= 0) return true;
  return false;
}

void resetLearnedData() {
  for (int i = EEPROM_START; i < EEPROM.length(); i++) EEPROM.write(i, 0);
}

void storePair(const String& input, const String& output) {
  char inBuf[32], outBuf[32];
  input.toCharArray(inBuf, sizeof(inBuf));
  output.toCharArray(outBuf, sizeof(outBuf));
  for (int i = EEPROM_START; i < EEPROM.length(); i += PAIR_SIZE) {
    char storedInput[32];
    EEPROM.get(i, storedInput);
    if (String(storedInput).length() == 0) {
      EEPROM.put(i, inBuf);
      EEPROM.put(i + 32, outBuf);
      break;
    }
  }
}

String getLearnedResponse(const String& input) {
  String inputStr = input;
  inputStr.trim();
  inputStr.toLowerCase();
  for (int i = EEPROM_START; i < EEPROM.length(); i += PAIR_SIZE) {
    char storedInput[32], storedOutput[32];
    EEPROM.get(i, storedInput);
    EEPROM.get(i + 32, storedOutput);
    String storedInputStr = String(storedInput);
    storedInputStr.trim();
    storedInputStr.toLowerCase();
    if (storedInputStr == inputStr) return String(storedOutput);
  }
  return "";
}

void autoFeed() {
  const String trainData[][2] = {{"hello", "hello, Sir/Ma'am"},{"how are you", "I am doing well, Sir/Ma'am"}};
  int total = sizeof(trainData) / sizeof(trainData[0]);
  for (int i = 0; i < total; i++) {
    storePair(trainData[i][0], trainData[i][1]);
    delay(50);
  }
  trainingDone = true;
}

void generateResponse(String input) {
  input.toLowerCase();
  input.trim();
  response = "";
  if (isIllegalInput(input)) {
    inCooldown = true;
    cooldownEnd = millis() + 20000;
    response = "{ERROR: Message Contains Illegal Information. The AI is cooling down for 20 seconds}";
    return;
  }
  if (input == "reset.") {
    resetLearnedData();
    response = "All learned memory has been reset.";
    return;
  }
  if (input == "give me a IT Joke") {
    Serial.println("POV your a IT worker and you forgot to save your month work: PANIK, autosave was on: calm, but the autosave failed: panik.");
  }
  int commaIndex = input.indexOf(',');
  if (commaIndex >= 1 && commaIndex < input.length() - 1) {
    storePair(input.substring(0, commaIndex), input.substring(commaIndex + 1));
    response = "Training pair stored.";
    return;
  }
  response = getLearnedResponse(input);
  if (response.length() == 0) response = "I don't understand yet.";
}

void handleKernelTerminal() {
  if (enteredPassword == kernelPassword) {
    inKernelTerminal = true;
    Serial.println("Access granted. Welcome to the Kernel Terminal.");
  } else {
    Serial.println("Incorrect password. Access denied.");
  }
  enteredPassword = "";
}

void printClock() {
  unsigned long currentMillis = millis();
  unsigned long seconds = currentMillis / 1000;
  unsigned long minutes = seconds / 60;
  seconds = seconds % 60;
  Serial.print("Current Time: ");
  Serial.print(minutes);
  Serial.print(":");
  if (seconds < 10) Serial.print("0");
  Serial.println(seconds);
}

void processKernelCommand(String cmd) {
  if (cmd == "echo") {
    Serial.println("Echo: Kernel Terminal Active.");
  } else if (cmd == "print") {
    Serial.println("Debugging Enabled.");
  } else if (cmd == "clock") {
    printClock();
  } else if (cmd == "exit") {
    inKernelTerminal = false;
    Serial.println("Exited Kernel Terminal.");
  } else if (cmd == "play _ship lv") {
  playShipGame(1);
  } else if (cmd == "play _ship lv2") {
  playShipGame(2);
  } else if (cmd == "play _ship lv3") {
  playShipGame(3);
} else if (cmd == "play _ship lv4") {
  playShipGame(4);
} else if (cmd == "play _ship lv5") {
  playShipGame(5);
} else if (cmd == "play _ship lv6") {
  playShipGame(6);
  } else if (cmd == "play _ship lv7") {
  playShipGame(7);
  } else if (cmd == "play _ship lv8") {
  playShipGame(8);
} else if (cmd == "play _ship lv9") {
  playShipGame(9);
} else if (cmd == "play _ship lv10") {
  playShipGame(10);
}
  else {
 Serial.println("Unknown command in Kernel Terminal.");
  }
}

void handleInput(String input) {
  input.trim();
  if (inCooldown) {
    if (millis() < cooldownEnd) {
      Serial.println("{ERROR: AI is in cooldown mode, please wait...}");
      return;
    }
    inCooldown = false;
  }

  if (input.startsWith(":s enter_kernelterminal")) {
    Serial.println("Enter Kernel Password: ");
    enteredPassword = "";
    while (!Serial.available()); // Wait for user input
    enteredPassword = Serial.readStringUntil('\n');
    handleKernelTerminal();
    return;
  }

  if (inKernelTerminal) {
    processKernelCommand(input);
    return;
  }
  generateResponse(input);
  Serial.println(response);
}

#endif
