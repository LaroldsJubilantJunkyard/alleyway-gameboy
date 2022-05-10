#include <gb/gb.h>
#include "common.h"
#include "graphics/ball.h"
#include "graphics/paddle.h"

void ResetBall(){
        
    ballX=BALLSTART_X;
    ballY=BALLSTART_Y;
    ballSpeed=2;
    ballVelocityX=0;
    ballVelocityY=0;
}
uint8_t UpdateBall(uint8_t lastSprite){


    ballX+=ballVelocityX>>4;
    ballY+=ballVelocityY>>4;

    if((ballY>>4)>144){

        // Reset the gameplay
        return 0;
    }

    if(blocksLeft==0){

        /// Reset to gameplay
        ///goto GameplayStart;
    }


    lastSprite = move_metasprite(ball_metasprites[0],paddle_TILE_COUNT,lastSprite,(ballX>>4)+7,(ballY>>4)+15);

    return 1;
}


void SpeedUpBall(){

    // Increase Speed slightly with each bounce
    ballVelocityY=(ballVelocityY/100)*101;
    ballVelocityX=(ballVelocityX/100)*101;
}
