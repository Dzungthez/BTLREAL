// SDL2Game.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"

BaseObject g_background;
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             SCREEN_WIDTH, SCREEN_HEIGHT,
                             SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gScreen = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gScreen == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gScreen,
                            RENDER_DRAW_COLOR,
                            RENDER_DRAW_COLOR,
                            RENDER_DRAW_COLOR,
                            RENDER_DRAW_COLOR);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool LoadBackground()
{
    bool ret = g_background.LoadImg("img/5.png",gScreen);
    if (ret == false)
    {
		printf("could not load bgr");
        return false;
    }
    return true;

}

void close ()
{
    g_background.Free();
    SDL_DestroyRenderer(gScreen);
    gScreen = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
}
int main( int argc, char* argv[] )
{
    if (!init())
    {
		printf("could not init %s\n", SDL_GetError());
        return -1;
    }
	if (!LoadBackground())
	{
		printf("could not load background %s\n", SDL_GetError());
		return -1;
	}
        
	GameMap game_map;
	game_map.LoadMap("map/map.dat");
	game_map.LoadTiles(gScreen);
	
	bool quit = false;
	while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        SDL_SetRenderDrawColor(gScreen,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR);

        SDL_RenderClear(gScreen);

        g_background.Render(gScreen, NULL);
		
		game_map.DrawMap(gScreen);
        SDL_RenderPresent(gScreen);

    }

	close();

	return 0;
}



