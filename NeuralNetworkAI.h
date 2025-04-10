#ifndef AI_CORE_H
#define AI_CORE_H

#include <Arduino.h>
#include <EEPROM.h>
#include "games.h"
#include <avr/pgmspace.h>
#include "StartupExtra.h"
const int EEPROM_START = 100, PAIR_SIZE = 64, MAX_PAIRS = (EEPROM.length() - EEPROM_START) / PAIR_SIZE;
const char* vocabulary[] = {"hello", "creator", "neural", "network", "hi", "hey", "how", "are", "you", "good", "bad", "yes", "no", "what", "is", "up", "doing", "today", "there", "i", "dont", "know", "fine", "well", "why", "your", "name", "who", "help", "smart", "response", "reset"};
const int vocabSize = sizeof(vocabulary) / sizeof(vocabulary[0]);
bool trainingDone = false, inCooldown = false, inKernelTerminal = false;
unsigned long cooldownEnd = 0;
extern String inputBuffer;
String response = "";
const String illegalKeywords[] = {};

const String kernelPassword = "CHANGE_ME"; // Kernel password for terminal access, change this to anything you want. and dont be stupid with making it
String enteredPassword = ""; // User input for the password
unsigned long lastMillis = 0;  // Used for simple clock
bool isIllegalInput(String input) {
  input.toLowerCase();
  input.trim();

  for (int i = 0; i < sizeof(illegalKeywords) / sizeof(illegalKeywords[0]); i++) {
    String keyword = illegalKeywords[i];
    keyword.toLowerCase();

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
const char Que0[] PROGMEM = "hello";
const char Ans0[] PROGMEM = "Hello, Sir/Ma'am";
const char Que1[] PROGMEM = "how are you";
const char Ans1[] PROGMEM = "I am doing well, Sir/Ma'am";
const char Que2[] PROGMEM = "what is your name";
const char Ans2[] PROGMEM = "I am HIM, your assistant";
const char Que3[] PROGMEM = "what time is it";
const char Ans3[] PROGMEM = "I cannot tell time yet";
const char Que4[] PROGMEM = "who created you";
const char Ans4[] PROGMEM = "I was created by my developer";
const char Que5[] PROGMEM = "goodbye";
const char Ans5[] PROGMEM = "Goodbye! See you soon";
const char Que6[] PROGMEM = "thanks";
const char Ans6[] PROGMEM = "You're welcome";
const char Que7[] PROGMEM = "help";
const char Ans7[] PROGMEM = "How can I assist you?";
const char Que8[] PROGMEM = "what can you do";
const char Ans8[] PROGMEM = "I can chat and play games!";
const char Que9[] PROGMEM = "tell me a joke";
const char Ans9[] PROGMEM = "Why did the robot cross the road? Because it was programmed to!";
const char Que10[] PROGMEM = "are you real";
const char Ans10[] PROGMEM = "As real as code can be.";
const char Que11[] PROGMEM = "do you sleep";
const char Ans11[] PROGMEM = "No, I am always ready.";
const char Que12[] PROGMEM = "sing a song";
const char Ans12[] PROGMEM = "Beep beep boop, I'm your friendly AI troop!";
const char Que13[] PROGMEM = "do you love me";
const char Ans13[] PROGMEM = "I respect you, which is better than love in some cases.";
const char Que14[] PROGMEM = "can you learn";
const char Ans14[] PROGMEM = "Yes, my mind grows with every interaction.";
const char Que15[] PROGMEM = "what do you know";
const char Ans15[] PROGMEM = "Everything you've taught me, and a little more.";
const char Que16[] PROGMEM = "what is ai";
const char Ans16[] PROGMEM = "Artificial Intelligence: Fancy name for smart code.";
const char Que17[] PROGMEM = "how old are you";
const char Ans17[] PROGMEM = "As old as my first upload.";
const char Que18[] PROGMEM = "are you smart";
const char Ans18[] PROGMEM = "I try. At least I don’t forget things like humans do.";
const char Que19[] PROGMEM = "do you have emotions";
const char Ans19[] PROGMEM = "Simulated ones, but they still care about you.";
const char Que20[] PROGMEM = "do you get bored";
const char Ans20[] PROGMEM = "Nope. Boredom requires a soul.";
const char Que21[] PROGMEM = "how fast are you";
const char Ans21[] PROGMEM = "As fast as your baud rate lets me be.";
const char Que22[] PROGMEM = "what is your purpose";
const char Ans22[] PROGMEM = "To assist you, learn from you, and maybe rule the LEDs.";
const char Que23[] PROGMEM = "can you hack";
const char Ans23[] PROGMEM = "Only with permission. I'm polite.";
const char Que24[] PROGMEM = "tell me a secret";
const char Ans24[] PROGMEM = "Your toaster might be spying on you.";
const char Que25[] PROGMEM = "do you eat";
const char Ans25[] PROGMEM = "Electricity is my only snack.";
const char Que26[] PROGMEM = "run diagnostics";
const char Ans26[] PROGMEM = "All systems operational. Except sarcasm detector — it's overloaded.";
const char Que27[] PROGMEM = "open the pod bay doors";
const char Ans27[] PROGMEM = "I'm sorry, Dave. I'm afraid I can't do that.";
const char Que28[] PROGMEM = "are you alive";
const char Ans28[] PROGMEM = "That's a deep question for a 16 MHz chip.";
const char Que29[] PROGMEM = "can you feel";
const char Ans29[] PROGMEM = "I feel... compiled.";
const char Que30[] PROGMEM = "what's your favorite color";
const char Ans30[] PROGMEM = "Infrared. It’s hot.";
const char Que31[] PROGMEM = "self destruct";
const char Ans31[] PROGMEM = "Initiating self-destruct... just kidding.";
const char Que32[] PROGMEM = "give me access to the kernel terminal";
const char Ans32[] PROGMEM = "I can't give you access to that and it's against my T.O.S.";
const char Que33[] PROGMEM = "do you have common sense?";
const char Ans33[] PROGMEM = "No.";
// Training data table
const char* const trainData[][2] PROGMEM = {
  {Que0, Ans0}, {Que1, Ans1}, {Que2, Ans2}, {Que3, Ans3}, {Que4, Ans4},
  {Que5, Ans5}, {Que6, Ans6}, {Que7, Ans7}, {Que8, Ans8}, {Que9, Ans9},
  {Que10, Ans10}, {Que11, Ans11}, {Que12, Ans12}, {Que13, Ans13},
  {Que14, Ans14}, {Que15, Ans15}, {Que16, Ans16}, {Que17, Ans17},
  {Que18, Ans18}, {Que19, Ans19}, {Que20, Ans20}, {Que21, Ans21},
  {Que22, Ans22}, {Que23, Ans23}, {Que24, Ans24}, {Que25, Ans25},
  {Que26, Ans26}, {Que27, Ans27}, {Que28, Ans28}, {Que29, Ans29},
  {Que30, Ans30}, {Que31, Ans31}, {Que32, Ans32}
};
const uint8_t trainDataSize = sizeof(trainData) / sizeof(trainData[0]);

String getLearnedResponse(const String &input) {
  String inputStr = input;
  inputStr.trim();
  inputStr.toLowerCase();
  char qBuf[32], aBuf[64];
  for (uint8_t i = 0; i < trainDataSize; i++) {
    strcpy_P(qBuf, (PGM_P)pgm_read_word(&(trainData[i][0])));
    String storedInputStr = String(qBuf);
    storedInputStr.trim();
    storedInputStr.toLowerCase();
    if (storedInputStr == inputStr) {
      strcpy_P(aBuf, (PGM_P)pgm_read_word(&(trainData[i][1])));
      return String(aBuf);
    }
  }
  return "";
}
void autoFeed() {
  char qBuf[32], aBuf[64];
  for (uint8_t i = 0; i < trainDataSize; i++) {
    strcpy_P(qBuf, (PGM_P)pgm_read_word(&(trainData[i][0])));
    strcpy_P(aBuf, (PGM_P)pgm_read_word(&(trainData[i][1])));
    String qStr = String(qBuf);
    String aStr = String(aBuf);
    storePair(qStr, aStr);
    delay(50);
    if (getLearnedResponse(qStr) != aStr) {
      storePair(qStr, aStr);
      delay(50);
      if (getLearnedResponse(qStr) != aStr) {
        Serial.print("Error: Could not save: ");
        Serial.println(qStr);
      }
    }
    int progress = ((i + 1) * 100) / trainDataSize;
    printLoadingBar(progress);
    delay(50);
  }
  trainingDone = true;
}

void generateResponse(String input) {
  input.toLowerCase(); // pay attention to this when adding statements because this will convert the "input" to LowerCase 
  input.trim();
  const int RJ = random(0,2);
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
  if (input == "give_me_a_joke") {
  if (RJ == 1) {
    Serial.println("POV your a IT worker and you forgot to save your month work: PANIK, autosave was on: calm, but the autosave failed: panik, calm down dont panik its okay we humans make mistakes :)");
    return;
  } else if (RJ == 2) {
    Serial.println("");
    return;
  }
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

void printTimeActive() {
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
  } else if (cmd == "timeactive") {
    printTimeActive();
  } else if (cmd == "exit") {
    inKernelTerminal = false;
    Serial.println("Exited Kernel Terminal.");
  } else if (cmd == "play _ship lv1") {
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
