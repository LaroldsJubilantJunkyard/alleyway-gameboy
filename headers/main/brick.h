#ifndef BRICK_HEADER 
#define BRICK_HEADER set
uint8_t CheckTopOrBottomBrick(int16_t x, int16_t  y);
void UpdateBrick(uint8_t side, uint8_t x, uint8_t  y);
void DrawLevelBricks(uint8_t *level);

#endif