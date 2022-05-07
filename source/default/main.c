#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdio.h>
#include "PlaceholderBlock.h"

#define EMPTY 0
#define TOP 0x02
#define BOTTOM 0x01
#define FULL TOP & BOTTOM

UINT8 paddleX=0;
INT8 paddleXSpeed=0;
UINT8 paddleY=0;

UINT16 blocksLeft=0;
UINT8 ballX=0;
UINT8 ballY=0;
UINT8 ballSpeed=1;
INT8 ballVelocityX=0;
INT8 ballVelocityY=0;

const UWORD BlackAndWhitePalette[] ={32767,22197,5285,0};
const unsigned char singleBlock[]={0x01};
const unsigned char singleBlockPalette[]={0x00};

unsigned char singleTileArray[] = {0x00};

UINT8 RandomNumber(UINT8 min, UINT8 max)
{
    const unsigned char *ptr_div_reg = 0xFF04;

    // get value at memory address
    return min + (*(ptr_div_reg) % (max - min)); 
}

UINT8 checkTopOrBottomCollision(UINT8 x, UINT8  y, UINT8 *tile){

    // Get the background tile at the given location
    get_bkg_tiles(x/8,y/8,1,1,singleTileArray);

    // Pass what tile was
    *tile=singleTileArray[0];

    // If this tile is zero, both the top and bottom tile have been destroyed
    if(singleTileArray[0]==0) return 0;

    // Get the vertical middle of the tile
    UINT8 mid = (y/8)*8+4;

    // Return if it's the top or bottom
    if(y>=mid && (singleTileArray[0] == 3||singleTileArray[0]==1))return BOTTOM;
    if(y<mid && (singleTileArray[0] == 2||singleTileArray[0]==1))return TOP;

    // 
    return 0;

}

void main(void){

    DISPLAY_ON;
    SHOW_SPRITES;
    SHOW_BKG;
    SPRITES_8x8;

    set_sprite_palette(0,1,BlackAndWhitePalette);
    set_bkg_palette(0,1,BlackAndWhitePalette);

    set_bkg_data(0,7,PlaceholderBlock);
    set_sprite_data(0,7,PlaceholderBlock);

    set_sprite_tile(0,5);
    set_sprite_tile(1,6);
    set_sprite_tile(2,4);

    LevelStart:

    for(UINT8 i=1;i<19;i++){
        for(UINT8 j=1;j<8;j++){
            VBK_REG=1;
            set_bkg_tiles(i,j,1,1,singleBlockPalette);
            VBK_REG=0;
            blocksLeft++;
            set_bkg_tiles(i,j,1,1,singleTileArray);

            
            singleTileArray[0]=1;
        }
    }

    // Reset the position and velocity of the ball and paddle
    ResetBallPaddlePosition:

    
    paddleX=80;
    paddleY=136;
    paddleXSpeed=0;
    ballX=80;
    ballY=104;
    ballSpeed=2;
    ballVelocityX=0;
    ballVelocityY=0;
    move_sprite(0,paddleX,paddleY+12);
    move_sprite(1,paddleX+8,paddleY+12);
    move_sprite(2,ballX+8,ballY+12);

    waitpad(J_A);

    // Randomly choose left or right
    if(RandomNumber(0,100)<50)ballVelocityX=1;
    else ballVelocityX=-1;

    ballVelocityY=-1;

    while(1){

        if(joypad() & J_LEFT){
            paddleXSpeed=-4;

            // Check if we are at the left limit
            if(paddleX<4||paddleX>250){
                paddleX=4;
                paddleXSpeed=0;
            }
        }else if(joypad() & J_RIGHT){
            paddleXSpeed=4;

            /// Check if we are at the right limit
            if(paddleX>156){
                paddleX=156;
                paddleXSpeed=0;
            }
        }else{

            // Slow down
            if(paddleXSpeed>0)paddleXSpeed--;
            else if(paddleXSpeed<0)paddleXSpeed++;
        }

        paddleX+=paddleXSpeed;

        for(UINT8 i=0;i<ballSpeed;i++){

            ballX+=ballVelocityX;
            ballY+=ballVelocityY;

            INT8 verticalSide=0;
            INT8 horizontalSide=0;

            if(ballVelocityX<0)horizontalSide=1;
            else if(ballVelocityX>0)horizontalSide=-1;

            if(ballVelocityY<0)verticalSide=1;
            else if(ballVelocityY>0)verticalSide=-1;

            UINT8 checkHorizontal = ballX+horizontalSide*-3;
            UINT8 checkVertical = ballY+verticalSide*-3;

            UINT8 checkTile=0;
            UINT8 check = checkTopOrBottomCollision(ballX,checkVertical,&checkTile);

            

            if(check!=0){

                ballVelocityY=verticalSide;
                blocksLeft--;

                if(check==TOP){
                    if(checkTile==1)singleTileArray[0]=3;
                    else singleTileArray[0]=0;
                }else if(check==BOTTOM){
                    if(checkTile==1)singleTileArray[0]=2;
                    else singleTileArray[0]=0;
                }
                
                set_bkg_tiles(ballX/8,checkVertical/8,1,1,singleTileArray);
            }else if(ballY<=2){

                ballVelocityY*=-1;
                
            }

            checkTile=0;
            check = checkTopOrBottomCollision(checkHorizontal,ballY,&checkTile);

            

            if(check!=0){

                ballVelocityX=horizontalSide;
                blocksLeft--;

                if(check==TOP){
                    if(checkTile==1)singleTileArray[0]=3;
                    else singleTileArray[0]=0;
                }else if(check==BOTTOM){
                    if(checkTile==1)singleTileArray[0]=2;
                    else singleTileArray[0]=0;
                }
                
                set_bkg_tiles(checkHorizontal/8,ballY/8,1,1,singleTileArray);
            }else if(ballX<=2){
                ballVelocityX=1;
            }else if(ballX>=158){
                ballVelocityX=-1;
            }

            INT16 xd = (INT16)ballX-(INT16)paddleX;
            INT16 yd = (INT16)ballY-(INT16)paddleY;

            if(xd<0)xd=-xd;
            if(yd<0)yd=-yd;

            if(xd<=10&&yd<=4){

                if(xd<8){
                    ballVelocityY=-ballVelocityY;
                }else if((ballVelocityX<0&&paddleX<ballX)||(ballVelocityX>0&&paddleX>ballX)){
                    ballVelocityX=-ballVelocityX;
                    ballVelocityY=-ballVelocityY;
                }


            }

            if(ballY>144){

                // Reset the gameplay
                goto Reset the ball and paddle position;
            }

            if(blocksLeft==0){

                /// Reset to gameplay
                goto GameplayStart;
            }

        }

        // Update our ball and paddle sprites
        move_sprite(0,paddleX,paddleY+12);
        move_sprite(1,paddleX+8,paddleY+12);
        move_sprite(2,ballX+4,ballY+12);

        // Slow down the frame rate some
        wait_vbl_done();
        wait_vbl_done();
        wait_vbl_done();
    }
}