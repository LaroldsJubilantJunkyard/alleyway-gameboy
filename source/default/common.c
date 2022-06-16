#include <gb/gb.h>
#include "common.h"

uint16_t paddleX=0;
uint16_t paddleY=0;
int16_t paddleXSpeed=0;

uint16_t blocksLeft=0;
uint16_t ballX=0;
uint16_t ballY=0;
uint16_t ballAngle = 45;
int16_t ballVelocityX=0;
int16_t ballVelocityY=0;

uint8_t stage=0;
uint16_t score=0,topScore=0;

uint8_t ballState = BALLSTATE_READY;


uint8_t joypadPrevious=0;
uint8_t joypadCurrent=0;

const UWORD BlackAndWhitePalette[] ={32767,22197,5285,0};


uint8_t RandomNumber(uint8_t min, uint8_t max)
{

    // get value at memory address
    return min + (DIV_REG % (max - min)); 
}
