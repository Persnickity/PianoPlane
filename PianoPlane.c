#include <SDL.h>
#include <stdlib.h>

#define MOVE_SPEED 200.0f
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define ARRAY_SIZE 80
#define BOX_WIDTH 30
#define BOX_HEIGHT 30

typedef struct
{
	int exist;
	float x, y;
	int red, green, blue;
	SDL_Rect rect;
}gameRect;


// Constructor for gameRect
gameRect createGameRect(float x, float y, int width, int height, int red, int green, int blue)
{
	gameRect object;
	object.exist = 1;
	object.x = x;
	object.y = y;
	object.rect.w = width;
	object.rect.h = height;
	object.red = red;
	object.green = green;
	object.blue = blue;
	return object;
}


// Draw Game Rectangle
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

// Draw array of bars
int drawGameRectArray(SDL_Surface* display, gameRect *array)
{
	int i;
	for( i = 0; i < ARRAY_SIZE; i++ )
	{
		if(array[i].exist == 1)
		{
			if(drawGameRect(display, array[i]) == 0)
				return 0;
		}
	}
	return 1;
} 

// Create new bar
int createNewBar(gameRect *array, int value )
{
	int i;
	for( i = 0; i < ARRAY_SIZE; i++ )
	{
		if(array[i].exist != 1)
		{
			float y= 240.0f;
			array[i] = createGameRect( 631.0f, y, 7, SCREEN_HEIGHT - (int)y, 255, 255, 255);
			return 1;
		}	
	}
	return 0;
}


// Update Array of bars
int updateArrayOfBars(gameRect *array, float ftime)
{
	int i;
	for( i = 0; i < ARRAY_SIZE; i++ )
	{
		if(array[i].exist == 1)
		{
			array[i].x -= MOVE_SPEED * ftime / 2;
			if ( (array[i].x + array[i].rect.w) < 0 )
			{
				array[i].exist = 0;
			}
		}
	}
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
	gameRect player = createGameRect(35.0f, 35.0f, BOX_WIDTH, BOX_HEIGHT, 0, 0, 255);
	
	gameRect rects[80];

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
		if (keys[SDLK_RIGHT] && (player.x + player.rect.w < SCREEN_WIDTH - 5.0f) )
			player.x += MOVE_SPEED * ftime;
		if (keys[SDLK_DOWN] && (player.y + player.rect.h < SCREEN_HEIGHT - 5.0f) )
			player.y += MOVE_SPEED * ftime;
		if (keys[SDLK_UP] && (player.y > 5.0f) )
			player.y -= MOVE_SPEED * ftime;
	
		// Gravity Impact
		if ( player.y + player.rect.h < SCREEN_HEIGHT - 5.0f )
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
		if(drawGameRect(display, player) == 0) break;
		// Draw bar1
	
		// Update Display
		SDL_Flip(display);

	}

	// Tell the SDL to clean up and shut down
	SDL_Quit();
    
	return 0;    
}
