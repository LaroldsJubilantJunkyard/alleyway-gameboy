#include <gb/gb.h>
#include "common.h"
#include "ball.h"
#include "brick.h"
#include "paddle.h"
#include "userinterface.h"


void CollidePaddleAgainstBall(){
    int16_t xd = (int16_t)(ballX>>4)-(int16_t)(paddleX>>4);

    xd = ABS(xd);

    if(xd<=PADDLE_SIZE/2+BALL_RADIUS){

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

            if(xd>PADDLE_SIZE/2-PADDLE_HALF_THICKNESS){
                if(paddleX<ballX){
                    // Use the negative absolute value so it goes right
                    ballVelocityX=ABS(ballVelocityX);
                    ballX=MAX(ballX,paddleX+1+PADDLE_SIZE/2+((PADDLE_HALF_THICKNESS-BALL_RADIUS)<<4));
                }else{
                    // Use the negative absolute value so it goes left
                    ballVelocityX=-ABS(ballVelocityX);
                    ballX=MIN(ballX,paddleX-1-PADDLE_SIZE/2-((PADDLE_HALF_THICKNESS-BALL_RADIUS)<<4));
                }
            }
            ballY=MIN(ballY,paddleY-((1+PADDLE_HALF_THICKNESS+BALL_RADIUS)<<4));

        }


    }
}

void CollideBricksAgainstBall(){

    int16_t verticalSide=SIGN(ballVelocityY);
    int16_t horizontalSide=SIGN(ballVelocityX);

    int16_t checkHorizontal = (ballX>>4)+horizontalSide*BALL_RADIUS;
    int16_t checkVertical = (ballY>>4)+verticalSide*BALL_RADIUS;

    uint8_t topBottomOrWall = CheckTopOrBottomBrick(ballX>>4,checkVertical);

    // If the ball hit something
    if(topBottomOrWall!=0){

        ballVelocityY=-verticalSide*ABS(ballVelocityY);

        // Increase Speed slightly with each bounce
        ballVelocityY+=SIGN(ballVelocityY)*BALL_SPEEDUP;

        // If we didn't hit a wall
        if(topBottomOrWall!=WALL){
            
            blocksLeft--;

            // Play a sound
            NR10_REG=0X00;
            NR11_REG=0X81;
            NR12_REG=0X43;
            NR13_REG=0X73;
            NR14_REG=0X86;

            IncreaseScore(5);

            UpdateBrick(topBottomOrWall,ballX>>4,checkVertical);
        }else{

            // Play a sound
            NR10_REG=0X4A;
            NR11_REG=0X81;
            NR12_REG=0X43;
            NR13_REG=0X56;
            NR14_REG=0X86;
        }
    }
    

    topBottomOrWall = CheckTopOrBottomBrick(checkHorizontal,ballY>>4);


    // If the ball hit something
    if(topBottomOrWall!=0){

        // Reflect 
        ballVelocityX=-horizontalSide*ABS(ballVelocityX);

        // Increase Speed slightly with each bounce
        ballVelocityX+=SIGN(ballVelocityX)*BALL_SPEEDUP;

        // If we didn't hit a wall
        if(topBottomOrWall!=WALL){

            // Decrease how many bullets we have
            blocksLeft--;


            // Play a sound
            NR10_REG=0X00;
            NR11_REG=0X81;
            NR12_REG=0X43;
            NR13_REG=0X73;
            NR14_REG=0X86;


            IncreaseScore(5);

            // Update the brick at the location
            UpdateBrick(topBottomOrWall,checkHorizontal,ballY>>4);
        }else{
            

            // Play a sound
            NR10_REG=0X4A;
            NR11_REG=0X81;
            NR12_REG=0X43;
            NR13_REG=0X56;
            NR14_REG=0X86;
        }
    }
}
