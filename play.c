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
/*
	Sint16 stream[2][MUSBUFFER*2*2];
	#define MUSBUFFLEN = MUSBUFFER*2*2
	int which=0;
*/

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

/*void CleanUp(int exitcode)
{
        if( Mix_PlayingMusic() ) {
                Mix_FadeOutMusic(1500);
                SDL_Delay(1500);
        }
        if ( music ) {
                Mix_FreeMusic(music);
                music = NULL;
        }
        Mix_CloseAudio();
        SDL_Quit();
        exit(exitcode);
}
*/

void startMusic(char* song)
{
	printf("startMusic\n");
	Mix_Music *music = NULL;

	// aud = audio 
	int aud_rate;
	Uint16 aud_format;
	int aud_channels;
	int aud_volume = MIX_MAX_VOLUME;

	// Initialize 
	// Hard coded to stereo settings 
	aud_rate = 22050;
	aud_format = AUDIO_S16;
	aud_channels = 2;

	// Initialize SDL library 
	if ( SDL_Init(SDL_INIT_AUDIO < 0 )) 
	{
		fprintf(stderr, "Couldn't initialize the SDL, EC: %s\n",SDL_GetError());
	//	return(255);
	}

	// Initialize audio device 
	if (Mix_OpenAudio(aud_rate, aud_format, aud_channels, MUSBUFFER) < 0)
	{
		fprintf(stderr, "Couldn't open audio, EC: %s\n", SDL_GetError());
	//	return (2);
	}
	// set up the post mix processor 
	Mix_SetPostMix(postmix,song);

	// Set volume for music 
	Mix_VolumeMusic(aud_volume);

	// Load music 
	music = Mix_LoadMUS(song);

	// Play and exit 
	Mix_FadeInMusic(music, -1, 1000);
	int tmp;
	while ( Mix_PlayingMusic() || Mix_PausedMusic() ) 
	{
		tmp = getheight();
		SDL_Delay(100);
	}

//	CleanUp(0);

}

int main(int argc, char *argv[])
{
	startMusic(argv[1]);
	fflush(stdout);
	fflush(stdout);
	int tmp = getheight();
	return 0;
}
/*

int main(int argc, char *argv[])
{
	Mix_Music *music = NULL;
	int aud_rate;
	Uint16 aud_format;
	int aud_channels;
	int aud_buffers;
	int aud_volume = MIX_MAX_VOLUME;

	aud_rate = 22050;
	aud_format = AUDIO_S16;
	aud_channels = 2;

	if ( SDL_Init(SDL_INIT_AUDIO < 0 )) 
	{
		fprintf(stderr, "Couldn't initialize the SDL, EC: %s\n",SDL_GetError());
		return(255);
	}

	if (Mix_OpenAudio(aud_rate, aud_format, aud_channels, MUSBUFFER) < 0)
	{
		fprintf(stderr, "Couldn't open audio, EC: %s\n", SDL_GetError());
		return (2);
	}
	Mix_SetPostMix(postmix,argv[1]);

	Mix_VolumeMusic(aud_volume);

	music = Mix_LoadMUS(argv[1]);

	Mix_FadeInMusic(music, -1, 1000);
	int tmp;
	printf("got out of Mix_PlayMusic\n");

	while ( Mix_PlayingMusic() || Mix_PausedMusic()) 
	{
		SDL_Delay(100);
		fflush(stdout);
		fflush(stdout);
		tmp = getheight();
		SDL_Delay(100);
		printf("Height : %d\n",tmp);
	}

	//CleanUp(0);
	
	return 0;
}
*/
