#ifndef BRICK_HEADER 
#define BRICK_HEADER set
uint8_t checkTopOrBottomCollision(uint8_t x, uint8_t  y);
void UpdateTile(uint8_t side, uint8_t x, uint8_t  y);
void UpdateBackgroundFull(uint8_t *level);

#endif