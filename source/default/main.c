#include <gb/gb.h>
#include <gb/metasprites.h>
#include <gb/cgb.h>
#include <stdio.h>
#include "graphics/ball.h"
#include "graphics/bricks.h"
#include "graphics/paddle.h"
#include "graphics/font.h"
#include "graphics/UserInterface.h"
#include "common.h"
#include "collisions.h"
#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "userinterface.h"

const uint8_t level[AREA_TOTAL]={
    NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,
    NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,
    NO____BRICK,NO____BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,NO____BRICK,NO____BRICK,
    NO____BRICK,NO____BRICK,MED___BRICK,MED___BRICK,MED___BRICK,MED___BRICK,MED___BRICK,MED___BRICK,MED___BRICK,MED___BRICK,MED___BRICK,MED___BRICK,NO____BRICK,NO____BRICK,
    NO____BRICK,NO____BRICK,MED___BRICK,MED___BRICK,MED___BRICK,MED___BRICK,MED___BRICK,MED___BRICK,MED___BRICK,MED___BRICK,MED___BRICK,MED___BRICK,NO____BRICK,NO____BRICK,
    NO____BRICK,NO____BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,NO____BRICK,NO____BRICK,
    NO____BRICK,NO____BRICK,DRK___BRICK,DRK___BRICK,DRK___BRICK,DRK___BRICK,DRK___BRICK,DRK___BRICK,DRK___BRICK,DRK___BRICK,DRK___BRICK,DRK___BRICK,NO____BRICK,NO____BRICK,
    NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,
    NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,
    NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,
    NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,
    NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,
    NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,
    NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,
    NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,
    NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,
    NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK
};

void ResetToReady(){
    ballState = BALLSTATE_READY;
    score=0;

    ResetUserInterface();

    DrawLevelBricks(level);
    ResetPaddle();
    ResetBall();
}


void main(void){

    DISPLAY_ON;
    SHOW_SPRITES;
    SHOW_BKG;
    SPRITES_8x8;

    // Turn on sound
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x77;

    set_sprite_palette(0,1,BlackAndWhitePalette);
    set_bkg_palette(0,1,BlackAndWhitePalette);

    set_bkg_data(0,Bricks_TILE_COUNT,Bricks_tiles);
    set_bkg_data(USERINTERFACE_TILES_START,UserInterface_TILE_COUNT,UserInterface_tiles);
    set_bkg_data(FONT_TILES_START,Font_TILE_COUNT,Font_tiles);

    set_sprite_data(0,paddle_TILE_COUNT,paddle_tiles);
    set_sprite_data(paddle_TILE_COUNT,ball_TILE_COUNT,ball_tiles);


    topScore=0;
    stage=0;


    ResetToReady();

    while(1){

        joypadPrevious=joypadCurrent;
        joypadCurrent=joypad();

        // Update the paddle
        uint8_t lastSprite = UpdatePaddle();

        // If the ball is ready
        if(ballState==BALLSTATE_READY){

            // Reset it's position
            ballX=BALLSTART_X;
            ballY=BALLSTART_Y;
            
            lastSprite = move_metasprite(ball_metasprites[0],paddle_TILE_COUNT,lastSprite,(ballX>>4)+7,(ballY>>4)+15);

            // If the A button was JUST pressed
            if((joypadCurrent & J_A) && !(joypadPrevious & J_A)){

                // The ball is now active
                ballState=BALLSTATE_ACTIVE;

                // Randomly choose left or right
                if(RandomNumber(0,100)<50)ballVelocityX=200;
                else ballVelocityX=-200;

                ballVelocityY=-RandomNumber(180,220);
            }
        }else{


            // Update the ball
            ballState=UpdateBall(lastSprite);

            CollideBricksAgainstBall();
            CollidePaddleAgainstBall();

            if(blocksLeft==0){

                stage++;

                ResetToReady();

            }
        }


        // Slow down the frame rate some
        wait_vbl_done();
    }
}