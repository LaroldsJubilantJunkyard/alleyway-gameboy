#include <gb/gb.h>
#include "common.h"
#include "graphics/Bricks.h"
#include "graphics/Paddle.h"


uint8_t checkTopOrBottomCollision(uint8_t x, uint8_t  y){

    if(x<8) return 0;
    if(y<8) return 0;
    if(x>=(AREA_WIDTH+1)*8) return 0;

    // Get the background tile at the given location
    uint8_t brick =get_bkg_tile_xy(x/8,y/8);


    // If this tile is zero, both the top and bottom tile have been destroyed
    if(brick==0) return 0;

    // Get the vertical middle of the tile
    uint8_t mid = (y/8)*8+4;

    uint8_t bottomMissing = (brick-1)%7==3;
    uint8_t topMissing = (brick-1)>=12&&(brick-1)<15; 

    // Return if it's the top or bottom
    if((y/4)%2!=0 && !bottomMissing)return BOTTOM;
    if((y/4)%2==0  && !topMissing)return TOP;

    // 
    return 0;

}


void UpdateBrick(uint8_t side, uint8_t x, uint8_t  y){

    uint8_t column=x/8;
    uint8_t row=y/8;

    // Get the background tile at the given location
    uint8_t brick =get_bkg_tile_xy(column,row);

    // If this tile is zero, both the top and bottom tile have been destroyed
    if(brick==0) return;
    if(brick>=USERINTERFACE_TILES_START)return;

    uint8_t brickRow= 0;

    side = (y/4)%2==0?TOP:BOTTOM;

    // Clearing the brick
    // If we're top only or bottom only
    if((brick==4||brick==8||brick==12)&&side==TOP)set_bkg_tile_xy(column,row,0);
    else if(brick>=13&&side==BOTTOM)set_bkg_tile_xy(column,row,0);
    else{

        uint8_t nextTile=brick;
        if(side==BOTTOM){
            if(brick>=1&&brick<=3)nextTile=4;
            else if(brick>=5&&brick<=7)nextTile=8;
            else if(brick>=9&&brick<=11)nextTile=12;
        }
        else if(side==TOP){
            if(brick>=1&&brick<=3)nextTile=13;
            else if(brick>=5&&brick<=7)nextTile=14;
            else if(brick>=9&&brick<=11)nextTile=15;
        }

        set_bkg_tile_xy(column,row,nextTile);
    }


}


void DrawLevelBricks(uint8_t *level){

    blocksLeft=0;

    for(uint8_t i=0;i<AREA_HEIGHT;i++){
        for(uint8_t j=0;j<AREA_WIDTH;j++){

             uint8_t brick = level[i*AREA_WIDTH+j];

            uint8_t tile=0;

            if(brick==NO____BRICK){
                set_bkg_tile_xy(j+1,i+1,0);
                
            }
            else {
                set_bkg_tile_xy(j+1,i+1,brick);
                blocksLeft+= (brick==LGHT__BRICK||brick==MED___BRICK||brick==DRK___BRICK)? 2:1;
            }
        }
    }

}
