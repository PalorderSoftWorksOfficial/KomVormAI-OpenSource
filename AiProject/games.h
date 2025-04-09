#ifndef GAMES_H
#define GAMES_H

#include <Arduino.h>
#include <EEPROM.h>

void playShipGame(int difficulty) {
  const int FW = 10, FH = 10;
  const int MAX_SHIPS = 5;
  const unsigned long MAX_TIME = 5 * 60 * 1000; // 5 minutes
  char field[FW][FH];
  int ammo = 25 + difficulty * 2;
  int cx = 0, cy = 0;
  unsigned long gameStart = millis();

  struct Ship {
    int x, y, len;
    bool vert, sunk;
  } ships[MAX_SHIPS];

  auto splash = []() {
    Serial.println(F("\n  __   __  ___  _   _ "));
    Serial.println(F("  \\ \\ / / / _ \\| | | |"));
    Serial.println(F("   \\ V / | (_) | |_| |"));
    Serial.println(F("    \\_/   \\___/ \\___/ "));
    Serial.println(F("   Arduino Battleship"));
    Serial.println(F("       v1.0 KONSOLE\n"));
    delay(500);
    Serial.print("Initializing");
    for (int i = 0; i < 3; i++) {
      delay(400);
      Serial.print(".");
    }
    Serial.println("\nScanning grid sectors...");
    delay(700);
    Serial.println("Deploying hostile units...");
    delay(800);
    Serial.println("READY.\n");
  };

  auto clearField = [&]() {
    for (int i = 0; i < FW; i++)
      for (int j = 0; j < FH; j++)
        field[i][j] = '.';
  };

  auto placeShips = [&]() {
    for (int i = 0; i < MAX_SHIPS; i++) {
      ships[i].len = random(2, 4);
      ships[i].vert = random(0, 2);
      ships[i].sunk = false;
      bool placed = false;
      while (!placed) {
        ships[i].x = random(FW);
        ships[i].y = random(FH);
        bool fits = true;
        if (ships[i].vert && ships[i].y + ships[i].len <= FH) {
          for (int j = 0; j < ships[i].len; j++)
            if (field[ships[i].x][ships[i].y + j] == 'X') fits = false;
          if (fits) {
            for (int j = 0; j < ships[i].len; j++)
              field[ships[i].x][ships[i].y + j] = 'X';
            placed = true;
          }
        } else if (!ships[i].vert && ships[i].x + ships[i].len <= FW) {
          for (int j = 0; j < ships[i].len; j++)
            if (field[ships[i].x + j][ships[i].y] == 'X') fits = false;
          if (fits) {
            for (int j = 0; j < ships[i].len; j++)
              field[ships[i].x + j][ships[i].y] = 'X';
            placed = true;
          }
        }
      }
    }
  };

  auto shipsRemaining = [&]() {
    int count = 0;
    for (int i = 0; i < MAX_SHIPS; i++) {
      if (!ships[i].sunk) count++;
    }
    return count;
  };

  auto displayField = [&]() {
    Serial.println("Field:");
    for (int j = 0; j < FH; j++) {
      for (int i = 0; i < FW; i++) {
        if (i == cx && j == cy) {
          Serial.print("[");
          Serial.print(field[i][j]);
          Serial.print("]");
        } else {
          Serial.print(" ");
          Serial.print(field[i][j]);
          Serial.print(" ");
        }
      }
      Serial.println();
    }
    unsigned long remaining = (MAX_TIME - (millis() - gameStart)) / 1000;
    int min = remaining / 60;
    int sec = remaining % 60;

    Serial.print("Ammo: ");
    Serial.print(ammo);
    Serial.print(" | Cursor: ");
    Serial.print(cx);
    Serial.print(",");
    Serial.print(cy);
    Serial.print(" | Ships Remaining: ");
    Serial.print(shipsRemaining());
    Serial.print(" | Time Left: ");
    Serial.print(min);
    Serial.print(":");
    if (sec < 10) Serial.print("0");
    Serial.println(sec);
  };

  auto checkWin = [&]() {
    for (int i = 0; i < MAX_SHIPS; i++) {
      int hits = 0;
      for (int j = 0; j < ships[i].len; j++) {
        int xi = ships[i].x + (ships[i].vert ? 0 : j);
        int yi = ships[i].y + (ships[i].vert ? j : 0);
        if (field[xi][yi] == 'H') hits++;
      }
      ships[i].sunk = (hits == ships[i].len);
    }
    for (int i = 0; i < MAX_SHIPS; i++)
      if (!ships[i].sunk) return false;
    return true;
  };

  splash();
  clearField();
  placeShips();
  Serial.println("~~ Use WASD to move, F to fire, Q to quit ~~");

  while (ammo > 0 && millis() - gameStart < MAX_TIME) {
    displayField();
    while (!Serial.available());
    char input = Serial.read();
    if (input == 'q') {
      Serial.println("Game aborted.");
      return;
    } else if (input == 'w' && cy > 0) cy--;
    else if (input == 's' && cy < FH - 1) cy++;
    else if (input == 'a' && cx > 0) cx--;
    else if (input == 'd' && cx < FW - 1) cx++;
    else if (input == 'f') {
      if (field[cx][cy] == 'X') {
        field[cx][cy] = 'H';
        Serial.println(">> Hit!");
        ammo--;
      } else if (field[cx][cy] == '.') {
        field[cx][cy] = 'M';
        Serial.println(">> Miss.");
        ammo--;
      } else {
        Serial.println(">> Already targeted.");
      }
      if (checkWin()) {
        displayField();
        Serial.println(F("\n>> ALL SHIPS SUNK <<"));
        Serial.println("Mission Success!");
        int score = ammo * 5 + ((MAX_TIME - (millis() - gameStart)) / 1000) * difficulty;
        Serial.print("Final Score: ");
        Serial.println(score);
        return;
      }
    }
  }

  displayField();
  Serial.println(F("\n== GAME OVER =="));
  if (ammo <= 0) Serial.println("You ran out of ammo.");
  else Serial.println("Time expired.");
  Serial.println("Mission Failed.");
  int score = ammo * 2; // Consolation score
  Serial.print("Final Score: ");
  Serial.println(score);
}

#endif
