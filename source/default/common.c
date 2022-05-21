#include <gb/gb.h>
#include "common.h"

uint16_t paddleX=0;
uint16_t paddleY=0;
int16_t paddleXSpeed=0;

uint16_t blocksLeft=0;
uint16_t ballX=0;
uint16_t ballY=0;
uint16_t ballAngle = 45;
uint8_t ballSpeed=1;
int16_t ballVelocityX=0;
int16_t ballVelocityY=0;

uint8_t stage=0;
uint16_t score=0,topScore=0;

uint8_t ballState = BALLSTATE_READY;

uint8_t paddleSize=16;

uint8_t joypadPrevious=0;
uint8_t joypadCurrent=0;

const UWORD BlackAndWhitePalette[] ={32767,22197,5285,0};


uint8_t RandomNumber(uint8_t min, uint8_t max)
{

    // get value at memory address
    return min + (DIV_REG % (max - min)); 
}

uint16_t sqrt(uint16_t x) {
  uint16_t i = 0;
  if (x >= 65536) {
    if ((i + 32768) * (i + 32768) <= x) i += 32768;
    if ((i + 16384) * (i + 16384) <= x) i += 16384;
    if ((i + 8192) * (i + 8192) <= x) i += 8192;
    if ((i + 4096) * (i + 4096) <= x) i += 4096;
    if ((i + 2048) * (i + 2048) <= x) i += 2048;
    if ((i + 1024) * (i + 1024) <= x) i += 1024;
    if ((i + 512) * (i + 512) <= x) i += 512;
    if ((i + 256) * (i + 256) <= x) i += 256;
  }
  if ((i + 128) * (i + 128) <= x) i += 128;
  if ((i + 64) * (i + 64) <= x) i += 64;
  if ((i + 32) * (i + 32) <= x) i += 32;
  if ((i + 16) * (i + 16) <= x) i += 16;
  if ((i + 8) * (i + 8) <= x) i += 8;
  if ((i + 4) * (i + 4) <= x) i += 4;
  if ((i + 2) * (i + 2) <= x) i += 2;
  if ((i + 1) * (i + 1) <= x) i += 1;
  return i;
}