#include <SDL.h>
#include <stdlib.h>

#define MOVE_SPEED 200.0f
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BOX_WIDTH 30
#define BOX_HEIGHT 30

typedef struct
{
	float x, y;
	int red, green, blue;
	SDL_Rect rect;
}gameRect;


// Draw Game Rectangle
// Takes SDL_Surface* and struct gameRect
int drawGameRect(SDL_Surface* display, gameRect object)
{
	// Update Player.rect
	object.rect.x = (int)object.x;
	object.rect.y = (int)object.y;

	// Draw Player
	if (SDL_FillRect(display,
			 &(object.rect),
			 SDL_MapRGB(display->format, object.red, object.green, object.blue))
			 != 0)
	{
		fprintf(stderr, "SDL_FillRect() Failed: %s\n", SDL_GetError());
		return 0;
	}
	return 1;
}

// Main
int main(int argc, char* args[])
{
	// Initialize the SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "SDL_Init() Failed: %s\n", SDL_GetError());
		exit(1);
	}

	// Set the video mode
	SDL_Surface* display;
	display = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (display == NULL)
	{
		fprintf(stderr, "SDL_SetVideoMode() Failed: %s\n", SDL_GetError());
		exit(1);
	}

	// Set the title bar
	SDL_WM_SetCaption("Piano Plane", "Piano Plane");

	// Gain access to keystate array
	Uint8 *keys = SDL_GetKeyState(NULL);

	// Timing variables
	Uint32 old_time, current_time;
	float ftime;

	// Need to initialize this here for event loop to work
	current_time = SDL_GetTicks();
	SDL_Event event;

	// Initialize Player
	gameRect player;
	player.x = 35.0f;
	player.y = 35.0f;
	player.red = 0;
	player.green = 0;
	player.blue = 255;
	player.rect.h = BOX_HEIGHT;
	player.rect.w = BOX_WIDTH;

	// Game loop
	while(1)
	{
		// Update the timing information
		old_time = current_time;
		current_time = SDL_GetTicks();
		ftime = (current_time - old_time) / 1000.0f;
	
		// Check for messages
		if (SDL_PollEvent(&event))
		{
			// Check for the quit message
			if (event.type == SDL_QUIT)
			{
				// Quit the program
				break;
			}
		}


		// Handle input
		if ( keys[SDLK_LEFT] && (player.x > 5.0f) )
			player.x -= MOVE_SPEED * ftime;
		if (keys[SDLK_RIGHT] && (player.x + BOX_WIDTH < SCREEN_WIDTH - 5.0f) )
			player.x += MOVE_SPEED * ftime;
		if (keys[SDLK_DOWN] && (player.y + BOX_HEIGHT < SCREEN_HEIGHT - 5.0f) )
			player.y += MOVE_SPEED * ftime;
		if (keys[SDLK_UP] && (player.y > 5.0f) )
			player.y -= MOVE_SPEED * ftime;
	
		// Gravity Impact
		if ( player.y + BOX_HEIGHT < SCREEN_HEIGHT - 5.0f )
			player.y += MOVE_SPEED * ftime / 2;
	
		// Clear the screen
		if (SDL_FillRect(display, 
				 NULL,
				 SDL_MapRGB( display->format, 0,0,0))
				 != 0)
		{
			fprintf(stderr, "SDL_FillRect() Failed: %s\n", SDL_GetError());
			break;
		}

		// Draw Player
		if(drawGameRect(display, player) == 0)
		{
			break;
		}

		// Update Display
		SDL_Flip(display);

	}

	// Tell the SDL to clean up and shut down
	SDL_Quit();
    
	return 0;    
}
