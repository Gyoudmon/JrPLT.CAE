#include <iostream>
#include <SDL2/SDL.h>

int main ( int argc, char** argv )
{
// initialize SDL video
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        std::cout << "Unable to init SDL video: "<< SDL_GetError() << std::endl;
        return 1;
    }
// make sure SDL cleans up before exit
    atexit(SDL_Quit);

// create a new window
    SDL_Window* screen = NULL;
    screen = SDL_CreateWindow("CodeBlocs Logo Window",
                          SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED,
                          150,120,
                          /* SDL_WINDOW_FULLSCREEN | */
                          SDL_WINDOW_OPENGL);
    if ( NULL == screen )
    {
        std::cout << "Unable to create window: "<< SDL_GetError() << std::endl;
        return 1;
    }

// Create a new renderer
    SDL_Renderer* renderer = NULL;
    renderer = SDL_CreateRenderer(screen, -1, 0);

    if ( NULL == renderer )
    {
        std::cout << "Unable to create renderer: "<< SDL_GetError() << std::endl;
        return 1;
    }

// load an image
    SDL_Surface* bitmap = NULL;
    bitmap = SDL_LoadBMP("cb.bmp");

    if ( NULL == bitmap )
    {
        std::cout << "Unable to load bitmap: "<< SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Texture* texture = NULL;
    texture = SDL_CreateTextureFromSurface(renderer, bitmap);
    SDL_FreeSurface(bitmap);

    // program main loop
    bool done = false;
    while (!done)
    {
    // drawing
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
            // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;

            // check for keypresses
            case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    break;
                }
            } // end switch
        } // end of message processing
    } // end main loop

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);

    SDL_Quit();
    return 0;
}
