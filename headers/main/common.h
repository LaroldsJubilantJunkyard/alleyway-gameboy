#ifndef COMMON_HEADER
#define COMMON_HEADER set

#include <gb/gb.h>

#define BIT(n,k) (n & (1 << (k-1)))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define ABS(x) ((x < 0) ? -x : x)
#define SIGN(x) (((x) < (0)) ? (-1) : (1))
#define CLAMP(x,y,z) MIN(MAX(x,y),z)

#define BALLSTART_X 64<<4
#define BALLSTART_Y 104<<4

#define BALLSTATE_READY 0
#define BALLSTATE_ACTIVE 1

#define PADDLE_SIZE 32

#define LEFT_PADDLE_LIMIT ((8+PADDLE_SIZE/2)<<4)
#define RIGHT_PADDLE_LIMIT ((120-PADDLE_SIZE/2)<<4)

#define BALL_SPEEDUP 3
#define PADDLE_HALF_THICKNESS 2
#define BALL_RADIUS 3
#define AREA_WIDTH 14
#define AREA_HEIGHT 17
#define AREA_TOTAL AREA_WIDTH*AREA_HEIGHT

#define EMPTY 0
#define TOP 0x02
#define BOTTOM 0x01
#define WALL 0x03
#define FULL TOP & BOTTOM

#define LGHT__BRICK 1
#define MED___BRICK 6
#define DRK___BRICK 11
#define NO____BRICK 255

#define USERINTERFACE_TILES_START Bricks_TILE_COUNT
#define FONT_TILES_START UserInterface_TILE_COUNT+USERINTERFACE_TILES_START

extern uint16_t paddleX;
extern uint16_t paddleY;
extern int16_t paddleXSpeed;

extern uint16_t blocksLeft;
extern uint16_t ballX;
extern uint16_t ballY;
extern uint16_t ballAngle;
extern int16_t ballVelocityX;
extern int16_t ballVelocityY;

extern uint8_t stage;
extern uint16_t score,topScore;

extern uint8_t ballState;


extern uint8_t joypadPrevious;
extern uint8_t joypadCurrent;

extern const UWORD BlackAndWhitePalette[];

uint8_t RandomNumber(uint8_t min, uint8_t max);

#endif