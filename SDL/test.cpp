#include<iostream>
#include<SDL2/SDL.h>

const int WIDTH = 800;
const int HEIGHT = 600;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Hello, World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if(NULL == window) {
        std::cout <<"could not create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Event windowEvent;
    while (true)    
    {
        if(SDL_PollEvent(&windowEvent)) 
        {
            if(windowEvent.type == SDL_QUIT) 
            {
                break;
            }
        }
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}