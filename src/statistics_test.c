/* 
Messed around with SDL, to see if its usable for the project.
The Glow Effect and hovering was done with the help of copilot, since math is hard and I'm
not an expert with SDL.
Code based of Tutorials from Programming Rainbow and Christopher Medina.

To run this version of gcc is needed: i686-w64-mingw32 and this command
gcc statistics_test.c -o test.exe -I "../external/SDL/i686-w64-mingw32/include" -I "../external/SDL3_ttf/i686-w64-mingw32/include" -L "../external/SDL/i686-w64-mingw32/lib"  -L "../external/SDL3_ttf/i686-w64-mingw32/lib"  -lSDL3 -lSDL3_ttf

*/

#include "../include/statistics_output_lib.h"
#include "../external/SDL/include/SDL3/SDL.h"
#include <time.h>

// ================== Data ==================



int main() {
    
    open_gui("data/", 5);

    return 0;
}