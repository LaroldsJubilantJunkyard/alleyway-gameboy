#include <gb/gb.h>
#include "common.h"
#include "ball.h"
#include "brick.h"
#include "userinterface.h"

void BounceAgainstTheWalls(){
    if((ballY>>4)<=8+BALL_RADIUS){


        ballY=(8+BALL_RADIUS+1)<<4;

        ballVelocityY=ABS(ballVelocityY);

        // Increase Speed slightly with each bounce
        SpeedUpBall();
        
    }
    if((ballX>>4)<=8+BALL_RADIUS){


        ballX=(8+BALL_RADIUS+1)<<4;
        ballVelocityX=ABS(ballVelocityX);

        // Increase Speed slightly with each bounce
        SpeedUpBall();
    }
    
    if((ballX>>4)>=120-BALL_RADIUS){
        
        ballX=(120-BALL_RADIUS-1)<<4;
        ballVelocityX=-ABS(ballVelocityX);

        // Increase Speed slightly with each bounce
        SpeedUpBall();
    }

}


void CollidePaddleAgainstBall(){
    int16_t xd = (int16_t)(ballX>>4)-(int16_t)(paddleX>>4);

    xd = ABS(xd);

    if(xd<=paddleSize/2+BALL_RADIUS){

        int16_t yd =(int16_t)(ballY>>4)-(int16_t)(paddleY>>4);

        yd = ABS(yd);
        
        if(yd<=(PADDLE_HALF_THICKNESS+BALL_RADIUS)){

            // Flip the y velocity
            ballVelocityY=-ballVelocityY;

            if(xd>paddleSize/2-PADDLE_HALF_THICKNESS){
                if(paddleX<ballX){
                    // Use the negative absolute value so it goes right
                    ballVelocityX=ABS(ballVelocityX);
                    ballX=MAX(ballX,paddleX+1+paddleSize/2+((PADDLE_HALF_THICKNESS-BALL_RADIUS)<<4));
                }else{
                    // Use the negative absolute value so it goes left
                    ballVelocityX=-ABS(ballVelocityX);
                    ballX=MIN(ballX,paddleX-1-paddleSize/2-((PADDLE_HALF_THICKNESS-BALL_RADIUS)<<4));
                }
            }
            ballY=MIN(ballY,paddleY-((1+PADDLE_HALF_THICKNESS+BALL_RADIUS)<<4));

            // Let the xspeed of the paddle lightly affect things
            ballVelocityX+=paddleXSpeed/20;

            // Increase Speed slightly with each bounce
            SpeedUpBall();

        }


    }
}

void CollideBricksAgainstBall(){

    int16_t verticalSide=SIGN(ballVelocityY);
    int16_t horizontalSide=SIGN(ballVelocityX);

    int16_t checkHorizontal = (ballX>>4)+horizontalSide*BALL_RADIUS;
    int16_t checkVertical = (ballY>>4)+verticalSide*BALL_RADIUS;

    uint8_t check = checkTopOrBottomCollision(ballX>>4,checkVertical);

    if(check!=0){

        ballVelocityY=-verticalSide*ABS(ballVelocityY);
        ballVelocityY+=SIGN(ballVelocityY)*BALL_SPEEDUP;
        blocksLeft--;

        IncreaseScore(5);

        UpdateTile(check,ballX>>4,checkVertical);
    }
    

    check = checkTopOrBottomCollision(checkHorizontal,ballY>>4);

    

    if(check!=0){

        ballVelocityX=-horizontalSide*ABS(ballVelocityX);
        ballVelocityX+=SIGN(ballVelocityX)*BALL_SPEEDUP;
        blocksLeft--;

        IncreaseScore(5);

        
        UpdateTile(check,checkHorizontal,ballY>>4);
    }
}
