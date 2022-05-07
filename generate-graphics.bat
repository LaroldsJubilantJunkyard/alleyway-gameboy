
rmdir /s/q  "source\gen"
rmdir /s/q  "headers\gen"

mkdir "source\gen\graphics"
mkdir "headers\gen\graphics"


SET GBDK_HOME=C:/gbdk
SET PNG2ASSET=%GBDK_HOME%/bin/png2asset.exe

:: Tetrominos
%PNG2ASSET% graphics\paddle.png -c source\gen\graphics\paddle.c -px 16 -py 0 -sx 0 -sy 0 -sw 32 -sh 8 -spr8x8 -keep_palette_order -noflip
%PNG2ASSET% graphics\ball.png -c source\gen\graphics\ball.c -px 4 -py 4 -sx 0 -sy 0 -sw 8 -sh 8 -spr8x8 -keep_palette_order -noflip

:: Backgrounds & Maps
%PNG2ASSET% graphics\UserInterface.png -c source\gen\graphics\UserInterface.c -map -use_map_attributes  -keep_palette_order -noflip
%PNG2ASSET% graphics\Bricks.png -c source\gen\graphics\Bricks.c -map -use_map_attributes  -keep_palette_order -noflip
%PNG2ASSET% graphics\Font.png -c source\gen\graphics\Font.c -map -use_map_attributes  -keep_palette_order -noflip


:: move .h files to their proper location
FOR /R "source/gen/graphics/" %%X IN (*.h) DO (
    move "%%X" "%cd%/headers/gen/graphics/"
)