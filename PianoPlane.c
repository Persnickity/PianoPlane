#include <SDL.h>
#include <stdlib.h>
#include <SDL_mixer.h>

#define MOVE_SPEED 200.0f
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define ARRAY_SIZE 100
#define BOX_WIDTH 30
#define BOX_HEIGHT 30
#define MUSBUFFER 1024

Sint16 stream[2][MUSBUFFER*2*2];
int len=MUSBUFFER*2*2, which=0;

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
	object.rect.x = (int)x;
	object.rect.y = (int)y;
	object.rect.w = width;
	object.rect.h = height;
	object.red = red;
	object.green = green;
	object.blue = blue;
	return object;
}

// Collision Detection
int collisionDetection(gameRect player, gameRect* array)
{
	
	int c;
	int pl = (int)player.x,
	    pr = (int)player.x + player.rect.w,
	    pt = (int)player.y,
	    pb = (int)player.y + player.rect.h;
	int bl,br,bt,bb;
	int ver,hor;
	
	for(c=0;c<ARRAY_SIZE;c++)
	{
		ver=0;
		hor=0;
		if(array[c].exist==1)
		{
			bl = (int)array[c].x;
			br = (int)array[c].x + array[c].rect.w;
			bt = (int)array[c].y;
			bb = (int)array[c].y + array[c].rect.h;
			if( (pb < bt) && (pt > bb) && (pr < pl ) && (pl > br) )
				return 1;
			if( ( (pl < bl) && (pr > bl) ) || ( (pl < br) && (pr > br) ) )
			{	
				if( ( (pt < bb) && (pb > bb) ) || ( (pt < bt) && (pb > bt) ) || ((pt > bt) && (pb < bb) ) )
				{
					return 1;
				}
			}
		
		}	
	}
	return 0;
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
int createNewBar(gameRect *array, int y, int value )
{
	int i, j = 0;
	
	if( y > 400 )
		y = 400;
	if( y < 150 )
		y = 150; 
	for( i = 0; i < ARRAY_SIZE; i++ )
	{
		if(array[i].exist == 0)
		{
			if( j == 0)
			{
				array[i] = createGameRect( 630.0f, (float)y + value, 10, SCREEN_HEIGHT - (y + value), rand() % 255, rand() % 255, rand() % 255);
				j++;
			} else {
				array[i] = createGameRect( 630.0f, 0.0f, 10, y + value - 100, rand() % 255, rand() % 255, rand() % 255);
				return y + value;
			}
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
	return 1;
}

// Init Array
void initArrayOfBars(gameRect *array)
{
	int i;
	for( i = 0; i < ARRAY_SIZE; i++ )
	{
		array[i].exist = 0;
	}
}

// Postmix updates audio stream buffer
void postmix(void *udata, Uint8 *_stream, int _len)
{
	len=_len;
        memcpy(stream[(which+1)%2],_stream,len);
	which = (which + 1) % 2;
}

// getHeight returns music data
int getheight() 
{
	Sint16 *buf;
	buf = stream[which];
	int height = 0;
	int x;//position
	int c;//count
	for(x=0;x<MUSBUFFER;x++)
	{
		if (buf[x] > 0)
		{
			height += buf[x];
			c++;
		}
	}
	height = (height/c);
	return height;	
}


// Play Music file
void startMusic(char* song)
{
	// aud = audio 
	int aud_rate;
	Uint16 aud_format;
	int aud_channels;
	int aud_volume = MIX_MAX_VOLUME;
	Mix_Music *music = NULL;

	// Initialize 
	// Hard coded to stereo settings 
	aud_rate = 22050;
	aud_format = AUDIO_S16;
	aud_channels = 2;

	// Initialize SDL library 
	if ( SDL_Init(SDL_INIT_AUDIO < 0 )) 
	{
		fprintf(stderr, "Couldn't initialize the SDL, EC: %s\n",SDL_GetError());
	}

	// Initialize audio device 
	if (Mix_OpenAudio(aud_rate, aud_format, aud_channels, MUSBUFFER) < 0)
	{
		fprintf(stderr, "Couldn't open audio, EC: %s\n", SDL_GetError());
	}
	// set up the post mix processor 
	Mix_SetPostMix(postmix,song);

	// Set volume for music 
	Mix_VolumeMusic(aud_volume);

	// Load music 
	music = Mix_LoadMUS(song);

	// Play  
	Mix_FadeInMusic(music, -1, 1000);
	//while ( Mix_PlayingMusic() || Mix_PausedMusic() ) 
	//{
	//	SDL_Delay(100);
	//}

}

// Main
int main(int argc, char* argv[])
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
	
	// Initialize rects
	gameRect rects[ARRAY_SIZE];
	initArrayOfBars(rects);

	float counter = 0;
	int yPrev = 240;

	// Start The Song
	startMusic(argv[1]);

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

		// Create new bar
		if(counter > 15.0f)
		{
			if( ( yPrev = createNewBar(rects, yPrev, 5) ) == 0)
			{
				fprintf(stderr, "Create New Bars Error\n");
				break;
			}
			counter = 0;
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

		// Update Array
		if ( updateArrayOfBars(rects, ftime) == 0 )
		{
			fprintf(stderr, "Update Array of Bars Failed \n");
			break;
		}

		// Collision Detection
		if (collisionDetection(player, rects) == 1) break;
		
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

		// Draw Array
		if(drawGameRectArray(display, rects) == 0) break;
	
		// Update Display
		SDL_Flip(display);

		// Increment counter
		counter+= MOVE_SPEED * ftime / 2;

	}

	// Tell the SDL to clean up and shut down
	SDL_Quit();
    
	return 0;    
}
