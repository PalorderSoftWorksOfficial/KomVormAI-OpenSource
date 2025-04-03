#ifndef AI_CORE_H
#define AI_CORE_H

#include <Arduino.h>
#include <EEPROM.h>
// This is written for a ARDUINO UNO R3
// Tested on ARDUINO UNO R3
// Remove Comments For Storage Saving.
const int EEPROM_START = 100, PAIR_SIZE = 64, MAX_PAIRS = (EEPROM.length() - EEPROM_START) / PAIR_SIZE;
const char* vocabulary[] = {"hello", "creator", "neural", "network", "hi", "hey", "how", "are", "you", "good", "bad", "yes", "no", "what", "is", "up", "doing", "today", "there", "i", "dont", "know", "fine", "well", "why", "your", "name", "who", "help", "smart", "response", "reset"};
const int vocabSize = sizeof(vocabulary) / sizeof(vocabulary[0]);
bool trainingDone = false, inCooldown = false;
unsigned long cooldownEnd = 0;
extern String inputBuffer;
String response = "";
const String illegalKeywords[] = {"real", "fake", "exist", "simulation", "not real", "are you real", "aren't real"};

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
  const String trainData[][2] = {{"hello", "hello, Sir/Ma'am"},{"how are you", "I am doing well, Sir/Ma'am"},{"what is up", "Just thinking, Sir/Ma'am"},{"i dont know", "Yes"},{"who are you", "I am your AI creation"},{"what is your name", "My name is KOMVORM AI, how can i assist you?"},{"help me", "I am here, creator"},{"you are smart", "Thank you, creator"},{"hello there", "Hi creator!"},{"your wife left you", "... I dont have a wife :/"},{"what is komvorm", "Komvorm is a place where kids do alot of stuff like with computers and more!, i was created in komvorm"}};
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
  int commaIndex = input.indexOf(',');
  if (commaIndex >= 1 && commaIndex < input.length() - 1) {
    storePair(input.substring(0, commaIndex), input.substring(commaIndex + 1));
    response = "Training pair stored.";
    return;
  }
  response = getLearnedResponse(input);
  if (response.length() == 0) response = "I don't understand yet.";
}

void processCommand(const String& cmd) {
  if (cmd == ":s dump") {
    for (int i = EEPROM_START; i < EEPROM.length(); i += PAIR_SIZE) {
      char storedInput[32], storedOutput[32];
      EEPROM.get(i, storedInput);
      EEPROM.get(i + 32, storedOutput);
      if (String(storedInput).length() > 0) {
        Serial.print("[ ");
        Serial.print(storedInput);
        Serial.print(" ] => [ ");
        Serial.print(storedOutput);
        Serial.println(" ]");
      }
    }
  } else if (cmd == ":s terminate") {
    Serial.flush();
    noInterrupts();
    for (int i = 0; i <= 100; i += 10) {
      Serial.print("Loading: [");
      for (int j = 0; j < 10; j++) Serial.print(j < i / 10 ? "=" : " ");
      Serial.print("] ");
      Serial.print(i);
      Serial.println("%");
      delay(100);
    }
    while (true) {
      Serial.println("INTERNAL ERROR: CANNOT TERMINATE");
      delay(2000);
      Serial.println("CRITICAL ERROR: KERNEL FAILURE");
    }
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
  if (input.startsWith(":s ")) {
    processCommand(input);
    return;
  }
  if (!trainingDone) {
    Serial.println("Still training, please wait...");
    return;
  }
  generateResponse(input);
  Serial.println(response);
}

#endif