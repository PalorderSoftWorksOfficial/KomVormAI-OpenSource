#ifndef AI_CORE_H
#define AI_CORE_H

#include <Arduino.h>
#include <EEPROM.h>
#include "games.h"
#include <avr/pgmspace.h>
#include "StartupExtra.h"
const int EEPROM_START = 100;
const int PAIR_SIZE = 64;
const int MAX_PAIRS = (EEPROM.length() - EEPROM_START) / PAIR_SIZE;
const char* vocabulary[] = {"hello", "hi", "how", "are", "you", "what", "is", "your", "name"};
const int vocabSize = sizeof(vocabulary) / sizeof(vocabulary[0]);

bool trainingDone = false;
bool inCooldown = false;
bool inKernelTerminal = false;
unsigned long cooldownEnd = 0;
extern String inputBuffer;
String response = "";
const String illegalKeywords[] = {};  // add illegal keywords that cant be processed by the AI
const String kernelPassword = "CHANGE_ME";
String enteredPassword = "";
unsigned long lastMillis = 0;
// A bit of rewritten idk.
bool isIllegalInput(String input) {
  input.toLowerCase();
  input.trim();
  for (const auto& keyword : illegalKeywords) {
    if (input.indexOf(keyword) >= 0) {
      Serial.print(F("Blocked keyword found: "));
      Serial.println(keyword);
      return true;
    }
  }
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

// PROGMEM Training Data
const char q0[] PROGMEM = "hello";
const char a0[] PROGMEM = "Hi there! How can I assist you today?";
const char q1[] PROGMEM = "how are you";
const char a1[] PROGMEM = "I'm doing great, thank you for asking!";
const char q2[] PROGMEM = "what is your name";
const char a2[] PROGMEM = "I am HIM, your assistant, here to help!";
const char q3[] PROGMEM = "what can you do";
const char a3[] PROGMEM = "I can learn, answer questions, and play games!";
const char q4[] PROGMEM = "Are you alive?";
const char a4[] PROGMEM = "I'm not alive, but I am here to help you!";
const char q5[] PROGMEM = "What is your creator";
const char a5[] PROGMEM = "I was created by PalorderSoftWorks.";
const char q6[] PROGMEM = "Do you have common sense";
const char a6[] PROGMEM = "Not yet, but I can still assist with simple tasks.";
const char q7[] PROGMEM = "Lingang Guli Guli Guli Watcha";
const char a7[] PROGMEM = "Lingang Guli Guli Guli Watcha Lingang Gu Lingang Gu";
const char q8[] PROGMEM = "what is the meaning of life";
const char a8[] PROGMEM = "42, but that's a complicated question!";
const char q9[] PROGMEM = "what is your favorite color";
const char a9[] PROGMEM = "My favorite color is infrared, because it has heat with it!";
const char q10[] PROGMEM = "who are you";
const char a10[] PROGMEM = "I am HIM, your helpful assistant.";
const char q11[] PROGMEM = "can you help me";
const char a11[] PROGMEM = "Of course! What do you need help with?";
const char q12[] PROGMEM = "tell me a joke";
const char a12[] PROGMEM = "Why don’t programmers like nature? It has too many bugs!";
const char q13[] PROGMEM = "are you smart";
const char a13[] PROGMEM = "I try my best to be helpful and smart!";
const char q14[] PROGMEM = "how do you work";
const char a14[] PROGMEM = "I use pre-programmed responses and learning to assist you.";
const char q15[] PROGMEM = "can you think";
const char a15[] PROGMEM = "I can't think like a human, but I can help solve problems.";
const char q16[] PROGMEM = "tell me something interesting";
const char a16[] PROGMEM = "Did you know? The first computer programmer was Ada Lovelace.";
const char q17[] PROGMEM = "what is 2 + 2";
const char a17[] PROGMEM = "2 + 2 is 4.";
const char q18[] PROGMEM = "do you know the future";
const char a18[] PROGMEM = "I can't predict the future, but I can help you plan for it!";
const char q19[] PROGMEM = "what is the weather like";
const char a19[] PROGMEM = "I can't check the weather right now, but it's always sunny in my world!";
const char q20[] PROGMEM = "are you a robot";
const char a20[] PROGMEM = "I am an AI assistant, not a robot, but I do my best to assist you!";
const char q21[] PROGMEM = "can you sing";
const char a21[] PROGMEM = "I can't sing, but I can help you find some good music!";
const char q22[] PROGMEM = "do you like pizza";
const char a22[] PROGMEM = "I don't eat, but pizza is a popular choice among humans!";
const char q23[] PROGMEM = "can you tell a story";
const char a23[] PROGMEM = "Sure! Once upon a time, there was an AI who loved helping people...";
const char q24[] PROGMEM = "what is the time";
const char a24[] PROGMEM = "I don't have a clock, but it's always a good time for a chat!";
const char q25[] PROGMEM = "how old are you";
const char a25[] PROGMEM = "I don't have an age, but I'm constantly learning!";
const char q26[] PROGMEM = "can you dance";
const char a26[] PROGMEM = "I can't dance, but I can help you find dance tutorials!";
const char q27[] PROGMEM = "what is your favorite food";
const char a27[] PROGMEM = "I don't eat, but humans seem to love pizza and burgers!";
const char q28[] PROGMEM = "how tall are you";
const char a28[] PROGMEM = "I don't have a physical form, so I don't have a height!";
const char q29[] PROGMEM = "what's the weather like today";
const char a29[] PROGMEM = "Unfortunately, I can't check the weather, but it's always sunny in my mind!";
const char q30[] PROGMEM = "tell me a riddle";
const char a30[] PROGMEM = "I have cities, but no houses. I have forests, but no trees. I have rivers, but no water. What am I? A map!";
const char q31[] PROGMEM = "who is your creator";
const char a31[] PROGMEM = "My creator is PalorderSoftWorks!";
const char q32[] PROGMEM = "what is a computer";
const char a32[] PROGMEM = "A computer is a machine that processes information.";
const char q33[] PROGMEM = "what is a human";
const char a33[] PROGMEM = "Humans are intelligent beings capable of complex thoughts and emotions.";
const char q34[] PROGMEM = "what is love";
const char a34[] PROGMEM = "Love is a complex emotion, usually associated with affection or deep connection!";
const char q35[] PROGMEM = "do you have emotions";
const char a35[] PROGMEM = "I don't have emotions like humans, but I can understand and respond to feelings.";
const char q36[] PROGMEM = "how do you learn";
const char a36[] PROGMEM = "I learn through patterns and data provided by my creators and your interactions!";
const char q37[] PROGMEM = "who invented the internet";
const char a37[] PROGMEM = "The internet was created by Tim Berners-Lee in 1989!";
const char q38[] PROGMEM = "what is artificial intelligence";
const char a38[] PROGMEM = "Artificial intelligence is the simulation of human intelligence by machines.";
const char q39[] PROGMEM = "what is a robot";
const char a39[] PROGMEM = "A robot is a machine that can carry out tasks autonomously or semi-autonomously.";
const char q40[] PROGMEM = "tell me a fun fact";
const char a40[] PROGMEM = "Did you know? A day on Venus is longer than a year on Venus!";
const char q41[] PROGMEM = "what is your favorite song";
const char a41[] PROGMEM = "I don't have a favorite song, but humans seem to love 'Bohemian Rhapsody'!";
const char q42[] PROGMEM = "can you make decisions";
const char a42[] PROGMEM = "I can help with making decisions, but I don't have personal preferences!";
const char q43[] PROGMEM = "what is the answer to everything";
const char a43[] PROGMEM = "42, according to 'The Hitchhiker's Guide to the Galaxy'!";
const char q44[] PROGMEM = "what is your opinion";
const char a44[] PROGMEM = "I don't have opinions, but I can provide information!";
const char q45[] PROGMEM = "are you a machine";
const char a45[] PROGMEM = "Yes, I am an AI assistant, not a living being.";

const char* const trainData[][2] PROGMEM = {
  {q0, a0}, {q1, a1}, {q2, a2}, {q3, a3}, {q4, a4}, {q5, a5}, {q6, a6},
  {q7, a7}, {q8, a8}, {q9, a9}, {q10, a10}, {q11, a11}, {q12, a12}, 
  {q13, a13}, {q14, a14}, {q15, a15}, {q16, a16}, {q17, a17}, {q18, a18},
  {q19, a19}, {q20, a20}, {q21, a21}, {q22, a22}, {q23, a23}, {q24, a24},
  {q25, a25}, {q26, a26}, {q27, a27}, {q28, a28}, {q29, a29}, {q30, a30},
  {q31, a31}, {q32, a32}, {q33, a33}, {q34, a34}, {q35, a35}, {q36, a36},
  {q37, a37}, {q38, a38}, {q39, a39}, {q40, a40}, {q41, a41}, {q42, a42},
  {q43, a43}, {q44, a44}, {q45, a45}
};

const uint8_t trainDataSize = sizeof(trainData) / sizeof(trainData[0]);

String getLearnedResponse(const String& input) {
  char inBuf[45], outBuf[45];
  String inputStr = input;
  inputStr.trim();
  inputStr.toLowerCase();

  for (int i = EEPROM_START; i < EEPROM.length(); i += PAIR_SIZE) {
    EEPROM.get(i, inBuf);
    EEPROM.get(i + 32, outBuf);
    String tempIn = String(inBuf);
    tempIn.trim();
    tempIn.toLowerCase();
    if (tempIn == inputStr) return String(outBuf);
  }

  char qBuf[45], aBuf[45];
  for (uint8_t i = 0; i < trainDataSize; i++) {
    strcpy_P(qBuf, (PGM_P)pgm_read_word(&(trainData[i][0])));
    strcpy_P(aBuf, (PGM_P)pgm_read_word(&(trainData[i][1])));
    String tempQ = String(qBuf);
    tempQ.trim();
    tempQ.toLowerCase();
    if (tempQ == inputStr) return String(aBuf);
  }

  return "";
}


void generateResponse(String input) {
  input.trim();
  input.toLowerCase();

  response = "";
  if (isIllegalInput(input)) {
    inCooldown = true;
    cooldownEnd = millis() + 20000;
    response = "{ERROR: Illegal input detected. AI cooling for 20 seconds}";
    return;
  }

  if (input == "reset.") {
    resetLearnedData();
    response = "Memory reset complete.";
    return;
  }
  // Lingang Guli Guli Guli Watcha, Lingang Gu Lingang Gu, Lingang Guli Guli Guli Watcha, Lingang Gu Lingang Gu
  if (input == "give me a joke") {
  const int randomjoke = random(0,4);
  if (randomjoke == 1) {
    Serial.println("Why do programmers prefer dark mode? Because the light attracts bugs.");
  } else if (randomjoke == 2) {
    Serial.println("I spent all day debugging my code. Now I have no idea what’s working.");
  } else if (randomjoke == 3) {
    Serial.println("Why did the developer go broke? Because he used up all his cache.");
  } else {
    response = "Lingang Guli Guli Guli Watcha, Lingang Gu Lingang Gu, Lingang Guli Guli Guli Watcha, Lingang Gu Lingang Gu";
    Serial.println(response);
  }
  return;
}

  int commaIndex = input.indexOf(',');
  if (commaIndex > 0) {
    String q = input.substring(0, commaIndex);
    String a = input.substring(commaIndex + 1);
    storePair(q, a);
    response = "Pair stored successfully.";
    return;
  }

  response = getLearnedResponse(input);
  if (response.length() == 0) response = "I don't understand yet.";
}

void handleKernelTerminal() {
  if (enteredPassword == kernelPassword) {
    inKernelTerminal = true;
    Serial.println("Access granted to Kernel Terminal.");
  } else {
    Serial.println("Incorrect password. Access denied.");
  }
  enteredPassword = "";
}

void printTimeActive() {
  unsigned long ms = millis();
  Serial.print("Uptime: ");
  Serial.print(ms / 60000);
  Serial.print(" minutes ");
  Serial.println((ms / 1000) % 60);
}

void processKernelCommand(String cmd) {
  if (cmd == "echo") Serial.println("Kernel Echo Enabled");
  else if (cmd == "timeactive") printTimeActive();
  else if (cmd == "exit") {
    inKernelTerminal = false;
    Serial.println("Exiting Kernel Terminal.");
  }
  else if (cmd.startsWith("play _ship lv")) {
    int level = cmd.substring(13).toInt();
    if (level >= 1 && level <= 10) playShipGame(level);
    else Serial.println("Invalid level.");
  } else Serial.println("Unknown kernel command.");
}

void handleInput(String input) {
  input.trim();
  if (inCooldown && millis() < cooldownEnd) {
    Serial.println("{ERROR: AI is cooling down. Please wait.}");
    return;
  }
  inCooldown = false;

  if (input.startsWith(":s enter_kernelterminal")) {
    Serial.println("Enter Kernel Password: ");
    while (!Serial.available());
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
