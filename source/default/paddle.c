#include <gb/gb.h>
#include "common.h"
#include "graphics/paddle.h"

void ResetPaddle(){
    
    
    paddleX=80<<4;
    paddleY=130<<4;
    paddleXSpeed=0;

}
uint8_t UpdatePaddle(){

    if(joypadCurrent & J_LEFT){
        paddleXSpeed=-600;

    }else if(joypadCurrent & J_RIGHT){
        paddleXSpeed=600;

    }else{

        // Slow down
        if(paddleXSpeed>30)paddleXSpeed-=30;
        else if(paddleXSpeed<-30)paddleXSpeed+=30;
        else paddleXSpeed=0;
    }

    paddleX=CLAMP(paddleX+(paddleXSpeed>>4),LEFT_PADDLE_LIMIT,RIGHT_PADDLE_LIMIT);

    
    /// Check if we are at the right limit
    if(paddleX>=RIGHT_PADDLE_LIMIT){
        paddleX=RIGHT_PADDLE_LIMIT;
        paddleXSpeed=-ABS((paddleXSpeed)/4)*2;
    } 
    // Check if we are at the left limit
    else if(paddleX<=LEFT_PADDLE_LIMIT){
        paddleX=LEFT_PADDLE_LIMIT;
        paddleXSpeed=ABS((paddleXSpeed)/4)*2;
    }
    return move_metasprite(paddle_metasprites[0],0,0,(paddleX>>4)+8,(paddleY>>4)+16);
}
