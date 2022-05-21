#ifndef USERINTERFACE_HEADER
#define USERINTERFACE_HEADER set
void IncreaseScore(uint16_t amount);
void UpdateScore();
void ResetUserInterface();
void DrawNumber(uint8_t x,uint8_t y, uint16_t number,uint8_t digits);

#endif