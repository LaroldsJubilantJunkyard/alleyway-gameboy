#include <gb/gb.h>
#include <gb/metasprites.h>
#include <gb/cgb.h>
#include <stdio.h>
#include "PlaceholderBlock.h"
#include "graphics/ball.h"
#include "graphics/bricks.h"
#include "graphics/paddle.h"
#include "graphics/font.h"
#include "graphics/UserInterface.h"

#define AREA_WIDTH 14
#define AREA_HEIGHT 17

#define EMPTY 0
#define TOP 0x02
#define BOTTOM 0x01
#define FULL TOP & BOTTOM

#define LGHT__BRICK 15
#define MED___BRICK 10
#define DRK___BRICK 5
#define NO____BRICK 0

#define BRICKS_TILES_START UserInterface_TILE_COUNT
#define USERINTERFACE_TILES_START UserInterface_TILE_COUNT+Bricks_TILE_COUNT

const uint8_t level[AREA_HEIGHT][AREA_WIDTH]={
    {NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK},
    {NO____BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,NO____BRICK},
    {NO____BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,NO____BRICK},
    {NO____BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,NO____BRICK},
    {NO____BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,NO____BRICK},
    {NO____BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,LGHT__BRICK,NO____BRICK},
    {NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK},
    {NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK},
    {NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK},
    {NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK},
    {NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK},
    {NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK},
    {NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK},
    {NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK},
    {NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK},
    {NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK},
    {NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK,NO____BRICK}
};

uint16_t paddleX=0;
uint16_t paddleY=0;
int16_t paddleXSpeed=0;

uint16_t blocksLeft=0;
uint16_t ballX=0;
uint16_t ballY=0;
uint8_t ballSpeed=1;
int8_t ballVelocityX=0;
int8_t ballVelocityY=0;

uint8_t paddleSize=16;

const UWORD BlackAndWhitePalette[] ={32767,22197,5285,0};
const unsigned char singleBlock[]={0x01};
const unsigned char singleBlockPalette[]={0x00};

unsigned char singleTileArray[] = {0x00};

uint8_t RandomNumber(uint8_t min, uint8_t max)
{
    const unsigned char *ptr_div_reg = 0xFF04;

    // get value at memory address
    return min + (*(ptr_div_reg) % (max - min)); 
}

uint8_t checkTopOrBottomCollision(uint8_t x, uint8_t  y, uint8_t *tile){

    // Get the background tile at the given location
    get_bkg_tiles(x/8,y/8,1,1,singleTileArray);

    // Pass what tile was
    *tile=singleTileArray[0];

    // If this tile is zero, both the top and bottom tile have been destroyed
    if(singleTileArray[0]==0) return 0;

    // Get the vertical middle of the tile
    uint8_t mid = (y/8)*8+4;

    // Return if it's the top or bottom
    if(y>=mid && (singleTileArray[0] == 3||singleTileArray[0]==1))return BOTTOM;
    if(y<mid && (singleTileArray[0] == 2||singleTileArray[0]==1))return TOP;

    // 
    return 0;

}

void UpdateBackgroundPartial(uint8_t column, uint8_t row){

    uint8_t brick = level[row][column];

    uint8_t tile=0;

    if(brick==11||brick==6||brick==0)tile=BRICKS_TILES_START+12;
    else if(brick>11)tile=BRICKS_TILES_START+(LGHT__BRICK-brick);
    else if(brick>6)tile=BRICKS_TILES_START+(MED___BRICK-brick);
    else tile=BRICKS_TILES_START+(DRK___BRICK-brick);

    set_bkg_tile_xy(column+1,row+1,tile);
}

void UpdateBackgroundFull(){

    for(uint8_t i=0;i<AREA_HEIGHT;i++){
        for(uint8_t j=0;j<AREA_WIDTH;j++){
            UpdateBackgroundPartial(j,i);
        }
    }

}

void main(void){

    DISPLAY_ON;
    SHOW_SPRITES;
    SHOW_BKG;
    SPRITES_8x8;

    set_sprite_palette(0,1,BlackAndWhitePalette);
    set_bkg_palette(0,1,BlackAndWhitePalette);

    set_bkg_data(0,UserInterface_TILE_COUNT,UserInterface_tiles);
    set_bkg_data(BRICKS_TILES_START,Bricks_TILE_COUNT,Bricks_tiles);
    set_bkg_data(USERINTERFACE_TILES_START,Font_TILE_COUNT,Font_tiles);

    set_sprite_data(0,paddle_TILE_COUNT,paddle_tiles);
    set_sprite_data(paddle_TILE_COUNT,ball_TILE_COUNT,ball_tiles);

    GameplayStart:

    set_bkg_tiles(0,0,20,18,UserInterface_map);


    set_sprite_tile(0,5);
    set_sprite_tile(1,6);
    set_sprite_tile(2,4);

    LevelStart:

    UpdateBackgroundFull();

    for(uint8_t i=1;i<AREA_WIDTH;i++){
        for(uint8_t j=1;j<AREA_HEIGHT;j++){
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

    
    paddleX=80<<4;
    paddleY=136;
    paddleXSpeed=0;
    ballX=64<<4;
    ballY=104<<4;
    ballSpeed=2;
    ballVelocityX=0;
    ballVelocityY=0;


    // Randomly choose left or right
    if(RandomNumber(0,100)<50)ballVelocityX=1;
    else ballVelocityX=-1;

    ballVelocityY=-1;

    uint8_t joypadPrevious=0;
    uint8_t joypadCurrent=joypad();

    while(1){

        joypadPrevious=joypadCurrent;
        joypadCurrent=joypad();

        if(joypadCurrent & J_LEFT){
            paddleXSpeed=-198;

            // Check if we are at the left limit
            if((paddleX>>4)<4||(paddleX>>4)>250){
                paddleX=4<<4;
                paddleXSpeed=0;
            }
        }else if(joypadCurrent & J_RIGHT){
            paddleXSpeed=198;

            /// Check if we are at the right limit
            if((paddleX>>4)>156){
                paddleX=156<<4;
                paddleXSpeed=0;
            }
        }else{

            // Slow down
            if(paddleXSpeed>0)paddleXSpeed-=5;
            else if(paddleXSpeed<0)paddleXSpeed+=5;
        }

        paddleX+=paddleXSpeed>>4;

        for(uint8_t i=0;i<ballSpeed;i++){

            ballX+=ballVelocityX;
            ballY+=ballVelocityY;

            int8_t verticalSide=0;
            int8_t horizontalSide=0;

            if(ballVelocityX<0)horizontalSide=1;
            else if(ballVelocityX>0)horizontalSide=-1;

            if(ballVelocityY<0)verticalSide=1;
            else if(ballVelocityY>0)verticalSide=-1;

            uint8_t checkHorizontal = (ballX>>4)+horizontalSide*-3;
            uint8_t checkVertical = (ballY>>4)+verticalSide*-3;

            uint8_t checkTile=0;
            uint8_t check = checkTopOrBottomCollision(ballX>>4,checkVertical,&checkTile);

            

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
            check = checkTopOrBottomCollision(checkHorizontal,ballY>>4,&checkTile);

            

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
                
                set_bkg_tiles(checkHorizontal/8,(ballY>>4)/8,1,1,singleTileArray);
            }else if((ballX>>4)<=2){
                ballVelocityX=1;
            }else if((ballX>>4)>=158){
                ballVelocityX=-1;
            }

            int16_t xd = (int16_t)(ballX>>4)-(int16_t)(paddleX>>4);
            int16_t yd = (int16_t)(ballY>>4)-(int16_t)(paddleY>>4);

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

            if((ballY>>4)>144){

                // Reset the gameplay
                goto GameplayStart;
            }

            if(blocksLeft==0){

                /// Reset to gameplay
                goto GameplayStart;
            }

        }

        uint8_t lastSprite = move_metasprite(paddle_metasprites[paddleSize-16],0,0,paddleX>>4,paddleY);
        lastSprite = move_metasprite(ball_metasprites[0],paddle_TILE_COUNT,lastSprite,ballX>>4,ballY>>4);


        // Update our ball and paddle sprites
        ///move_sprite(0,paddleX,paddleY+12);
        //move_sprite(1,paddleX+8,paddleY+12);
       // move_sprite(2,ballX+4,ballY+12);

        // Slow down the frame rate some
        wait_vbl_done();
    }
}