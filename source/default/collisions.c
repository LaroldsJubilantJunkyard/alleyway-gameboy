#include <gb/gb.h>
#include "common.h"
#include "ball.h"
#include "brick.h"
#include "userinterface.h"


void CollidePaddleAgainstBall(){
    int16_t xd = (int16_t)(ballX>>4)-(int16_t)(paddleX>>4);

    xd = ABS(xd);

    if(xd<=paddleSize/2+BALL_RADIUS){

        int16_t yd =(int16_t)(ballY>>4)-(int16_t)(paddleY>>4);

        yd = ABS(yd);
        
        if(yd<=(PADDLE_HALF_THICKNESS+BALL_RADIUS)){

            // Flip the y velocity
            ballVelocityY=-ballVelocityY;
            
            NR10_REG=0X2B;
            NR11_REG=0X81;
            NR12_REG=0X41;
            NR13_REG=0X5C;
            NR14_REG=0X86;

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

    uint8_t check = CheckTopOrBottomBrick(ballX>>4,checkVertical);

    if(check!=0){

        ballVelocityY=-verticalSide*ABS(ballVelocityY);
        ballVelocityY+=SIGN(ballVelocityY)*BALL_SPEEDUP;

        // If we didn't hit a wall
        if(check!=WALL){
            
            blocksLeft--;

            NR10_REG=0X00;
            NR11_REG=0X81;
            NR12_REG=0X43;
            NR13_REG=0X73;
            NR14_REG=0X86;

            IncreaseScore(5);

            UpdateBrick(check,ballX>>4,checkVertical);
        }else{
            NR10_REG=0X4A;
            NR11_REG=0X81;
            NR12_REG=0X43;
            NR13_REG=0X56;
            NR14_REG=0X86;
        }

        // Increase Speed slightly with each bounce
        SpeedUpBall();
    }
    

    check = CheckTopOrBottomBrick(checkHorizontal,ballY>>4);

    if(check!=0){

        ballVelocityX=-horizontalSide*ABS(ballVelocityX);
        ballVelocityX+=SIGN(ballVelocityX)*BALL_SPEEDUP;

        // If we didn't hit a wall
        if(check!=WALL){
            blocksLeft--;

            NR10_REG=0X00;
            NR11_REG=0X81;
            NR12_REG=0X43;
            NR13_REG=0X73;
            NR14_REG=0X86;


            IncreaseScore(5);

            
            UpdateBrick(check,checkHorizontal,ballY>>4);
        }else{
            
            NR10_REG=0X4A;
            NR11_REG=0X81;
            NR12_REG=0X43;
            NR13_REG=0X56;
            NR14_REG=0X86;
        }

        // Increase Speed slightly with each bounce
        SpeedUpBall();
    }
}
