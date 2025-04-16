#ifndef STARTUP_EXTRA_H
#define STARTUP_EXTRA_H
#include <MemoryFree.h>  
#include <avr/pgmspace.h>
void printLoadingBar(int progress) {
  int pos = (progress * 50) / 100;
  Serial.print('[');
  for (int i = 0; i < 50; ++i) Serial.print(i < pos ? '=' : (i == pos ? '>' : ' '));
  Serial.print("] "); Serial.print(progress); Serial.println('%');
}

const char stringInFlash[] PROGMEM = "";

void printMemoryUsage() {
  unsigned int usedFlash = 9178, totalFlash = 32256, freeFlash = totalFlash - usedFlash;
  unsigned int usedSRAM = 1304, totalSRAM = 2048, freeSRAM = totalSRAM - usedSRAM;

  Serial.print(F("Flash Memory Usage: ")); Serial.print(usedFlash); Serial.print(F(" / ")); Serial.println(totalFlash);
  Serial.print(F("SRAM Memory Usage: ")); Serial.print(usedSRAM); Serial.print(F(" / ")); Serial.println(totalSRAM);
  Serial.print(F("Free SRAM: ")); Serial.println(freeMemory());
}
#endif
// put yo code here