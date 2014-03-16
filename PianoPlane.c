#include <SDL.h>
#include <stdlib.h>

#define MOVE_SPEED 200.0f
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BOX_WIDTH 30
#define BOX_HEIGHT 30

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

   // Box
   float x = 35.0f, y = 35.0f;
   SDL_Rect rect;
   rect.h = BOX_HEIGHT;
   rect.w = BOX_WIDTH;

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
       if ( keys[SDLK_LEFT] && (x > 5.0f) )
         x -= MOVE_SPEED * ftime;
       if (keys[SDLK_RIGHT] && (x + BOX_WIDTH < SCREEN_WIDTH - 5.0f) )
         x += MOVE_SPEED * ftime;
       if (keys[SDLK_DOWN] && (y + BOX_HEIGHT < SCREEN_HEIGHT - 5.0f) )
         y += MOVE_SPEED * ftime;
       if (keys[SDLK_UP] && (y > 5.0f) )
         y -= MOVE_SPEED * ftime;

      // Gravity Impact
      if ( y + BOX_HEIGHT < SCREEN_HEIGHT - 5.0f )
         y += MOVE_SPEED * ftime / 2;

      // Clear the screen
      if (SDL_FillRect(display, 
                       NULL,
                       SDL_MapRGB( display->format, 0,0,0))
                       != 0)
      {
	 fprintf(stderr, "SDL_FillRect() Failed: %s\n", SDL_GetError());
         break;
      }

      // Draw box
      rect.x = (int)x;
      rect.y = (int)y;
      if (SDL_FillRect(display, 
                       &rect,
                       SDL_MapRGB(display->format, 255,255,255))
                       != 0)
      {
	 fprintf(stderr, "SDL_FillRect() Failed: %s\n", SDL_GetError());
         break;
      }
        //Update the display
      SDL_Flip(display);

   }

   // Tell the SDL to clean up and shut down
   SDL_Quit();
    
   return 0;    
}
