#include <gb/gb.h>
#include "common.h"
#include <stdlib.h>
#include <string.h>
#include "graphics/Font.h"
#include "graphics/Bricks.h"
#include "graphics/UserInterface.h"

void DrawNumber(uint8_t x,uint8_t y, uint16_t number,uint8_t digits){
	
    unsigned char buffer[]="00000000";

    // Convert the number to a decimal string (stored in the buffer char array)
    uitoa(number, buffer, 10);

    // Get the length of the number so we can add leading zeroes
    uint8_t len =strlen(buffer);

    // Add some leading zeroes
    // uitoa will not do this for us
    // Increase the VRAM address each iteration to move to the next tile
    for(uint8_t i=0;i<digits-len;i++){
        VBK_REG=1; set_bkg_tile_xy(x,y,0);
        VBK_REG=0;set_bkg_tile_xy(x++,y,USERINTERFACE_TILES_START+42);
    }
        
    // Draw our number
    // Increase the VRAM address each iteration to move to the next tile
    for(uint8_t i=0;i<len;i++){
        
        VBK_REG=1;set_bkg_tile_xy(x,y,0);
        VBK_REG=0;set_bkg_tile_xy(x++,y,(buffer[i]-'0')+USERINTERFACE_TILES_START+42);
    }
}


void UpdateScore(){

    DrawNumber(16,2,topScore,4);
    DrawNumber(16,5,score,4);
    DrawNumber(17,10,stage+1,2);
}

void ResetUserInterface(){
    
    // Redraw the entire user-interface which will clear all existing bricks
    set_bkg_based_tiles(0,0,20,18,UserInterface_map,USERINTERFACE_TILES_START);

    UpdateScore();
}

void IncreaseScore(uint16_t amount){
    score+=amount;
    topScore =MAX(topScore,score);
    UpdateScore();
}