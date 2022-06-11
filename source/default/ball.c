#include <gb/gb.h>
#include "common.h"
#include "graphics/ball.h"
#include "graphics/paddle.h"

void ResetBall(){
        
    ballX=BALLSTART_X;
    ballY=BALLSTART_Y;
    ballVelocityX=0;
    ballVelocityY=0;
}

uint8_t UpdateBall(uint8_t lastSprite){

    // Move according to our velocity
    ballX+=ballVelocityX>>4;
    ballY+=ballVelocityY>>4;

    // if the ball is at the bottom of the screen
    if((ballY>>4)>144){

        // Reset the gameplay
        return 0;
    }

    lastSprite = move_metasprite(ball_metasprites[0],paddle_TILE_COUNT,lastSprite,(ballX>>4)+7,(ballY>>4)+15);

    return 1;
}