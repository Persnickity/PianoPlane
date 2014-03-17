/*
	1st run
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_mixer.h>

#define BUFFER 1024

static int audio_open = 0;
static Mix_Music *music = NULL;
Sint16 stream[2][BUFFER*2*2];
int len = BUFFER*2*2;
Sint16 *buf;

void postmix(void *udata, Uint8 *_stream, int _len)
{
        memcpy(buf,_stream,len);
}
int barheight() 
{	
	int height = 0;
	int x;//position
	int c;//count
	for(x=0;x<len;x++)
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

void CleanUp(int exitcode)
{
        if( Mix_PlayingMusic() ) {
                Mix_FadeOutMusic(1500);
                SDL_Delay(1500);
        }
        if ( music ) {
                Mix_FreeMusic(music);
                music = NULL;
        }
        if ( audio_open ) {
                Mix_CloseAudio();
                audio_open = 0;
        }
        SDL_Quit();
        exit(exitcode);
}

int main(int argc, char *argv[])
{
	/* aud = audio */
	int aud_rate;
	Uint16 aud_format;
	int aud_channels;
	int aud_buffers;
	int aud_volume = MIX_MAX_VOLUME;

	/* Initialize */
	/* Hard coded to stereo settings */
	aud_rate = 22050;
	aud_format = AUDIO_S16;
	aud_channels = 2;

	/* Initialize SDL library */
	if ( SDL_Init(SDL_INIT_AUDIO < 0 )) 
	{
		fprintf(stderr, "Couldn't initialize the SDL, EC: %s\n",SDL_GetError());
		return(255);
	}

	/* Initialize audio device */
	if (Mix_OpenAudio(aud_rate, aud_format, aud_channels, BUFFER) < 0)
	{
		fprintf(stderr, "Couldn't open audio, EC: %s\n", SDL_GetError());
		return (2);
	} else {
		Mix_QuerySpec(&aud_rate, &aud_format, &aud_channels);
		printf("Opened audio successfully\n");
	}
	audio_open = 1;
	/* set up the post mix processor */
	Mix_SetPostMix(postmix,argv[1]);

	/* Set volume for music */
	Mix_VolumeMusic(aud_volume);

	/* Load music */
	music = Mix_LoadMUS(argv[1]);

	while (music)
	{
		/* Play and exit */
		Mix_PlayMusic(music, -1);
		while ( Mix_PlayingMusic() || Mix_PausedMusic()) 
		{
			SDL_Delay(100);
		}
	
		Mix_FreeMusic(music);
		music = NULL;
		SDL_Delay(500);
	}
	CleanUp(0);
	
	return 0;
}
