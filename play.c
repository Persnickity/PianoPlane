/*
	1st run
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_mixer.h>

#define MUSBUFFER 1024

Sint16 stream[2][MUSBUFFER*2*2];
int len=MUSBUFFER*2*2, which=0;

void postmix(void *udata, Uint8 *_stream, int _len)
{
	len=_len;
        memcpy(stream[(which+1)%2],_stream,len);
	which = (which + 1) % 2;
}

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

	// Play and exit 
	Mix_FadeInMusic(music, -1, 1000);
	while ( Mix_PlayingMusic() || Mix_PausedMusic() ) 
	{
		SDL_Delay(100);
	}

}

int main(int argc, char *argv[])
{
	startMusic(argv[1]);
	return 0;
}
