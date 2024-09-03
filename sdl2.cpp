#include <SDL2/SDL.h>
#include "OLED Library/illustrator.cpp"

using namespace std;

vector<u8> addr1 = {0,1,1,1,1,0,0};
oled oled1(addr1);

const int SCREEN_WIDTH = oled1.WIDTH * 8 + 2 * (oled1.WIDTH - 1) + 2 * 10; 
const int SCREEN_HEIGHT = oled1.HEIGHT * 8 + 2 * (oled1.HEIGHT - 1) + 2 * 10; 
const int PIXEL_SIZE = 8;                                   
const int SPACE_BETWEEN_PIXELS = 2;
const u8 BACKGROUND_COLOR = 0x00;
const u8 BORDER_COLOR = 0x30;
const int BORDER_SPACE = 10;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        return false;
    }

    gWindow = SDL_CreateWindow("OLED Pixel Screen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        cout << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL) {
        cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
        return false;
    }

    SDL_SetRenderDrawColor(gRenderer, BACKGROUND_COLOR, BACKGROUND_COLOR, BACKGROUND_COLOR, SDL_ALPHA_OPAQUE); 
    SDL_RenderClear(gRenderer); 

    return true;
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

void drawPixel(int x, int y, vector<vector<u8>> &GDDR) {
    SDL_Rect pixelRect = {BORDER_SPACE + x * (PIXEL_SIZE + SPACE_BETWEEN_PIXELS), BORDER_SPACE + y * (PIXEL_SIZE + SPACE_BETWEEN_PIXELS), PIXEL_SIZE, PIXEL_SIZE};
    
    SDL_SetRenderDrawColor(gRenderer, BORDER_COLOR, BORDER_COLOR, BORDER_COLOR, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(gRenderer, &pixelRect);
    
    if (GDDR[y][x]) {
        pixelRect.x += 1;
        pixelRect.y += 1;
        pixelRect.w -= 2;
        pixelRect.h -= 2;
        SDL_SetRenderDrawColor(gRenderer, 0, oled1.pixel_color, oled1.pixel_color, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(gRenderer, &pixelRect);
    }
}

// void renderScreen(vector<vector<u8>> &GDDR) {
//     SDL_RenderClear(gRenderer); // Clear the renderer

//     // Drawing all pixels
//     for (int y = 0; y < 32; ++y) {
//         for (int x = 0; x < 128; ++x) {
//             drawPixel(x, y, GDDR);
//         }
//     }

//     SDL_RenderPresent(gRenderer); // Update the screen
// }

void renderScreen(vector<vector<u8>> &GDDR) {
    SDL_SetRenderDrawColor(gRenderer, BACKGROUND_COLOR, BACKGROUND_COLOR, BACKGROUND_COLOR, SDL_ALPHA_OPAQUE); // Clear with background color
    SDL_RenderClear(gRenderer); // Clear the renderer

    // Drawing grid lines
    for (int y = 0; y < 33; ++y) {
        SDL_RenderDrawLine(gRenderer, BORDER_SPACE, BORDER_SPACE + y * (PIXEL_SIZE + SPACE_BETWEEN_PIXELS), SCREEN_WIDTH - BORDER_SPACE, BORDER_SPACE + y * (PIXEL_SIZE + SPACE_BETWEEN_PIXELS));
    }
    for (int x = 0; x < 129; ++x) {
        SDL_RenderDrawLine(gRenderer, BORDER_SPACE + x * (PIXEL_SIZE + SPACE_BETWEEN_PIXELS), BORDER_SPACE, BORDER_SPACE + x * (PIXEL_SIZE + SPACE_BETWEEN_PIXELS), SCREEN_HEIGHT - BORDER_SPACE);
    }

    // Drawing all pixels
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 128; ++x) {
            drawPixel(x, y, GDDR);
        }
    }

    SDL_RenderPresent(gRenderer); // Update the screen
}
