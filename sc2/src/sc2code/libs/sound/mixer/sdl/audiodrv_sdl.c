//Copyright Paul Reiche, Fred Ford. 1992-2002

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/* SDL audio driver
 */

#include "audiodrv_sdl.h"
#include "libs/tasklib.h"
#include <stdlib.h>


static Task StreamDecoderTask;

static const audio_Driver mixSDL_Driver =
{
	mixSDL_Uninit,
	mixSDL_GetError,
	audio_DRIVER_MIXSDL,
	{
		/* Errors */
		MIX_NO_ERROR,
		MIX_INVALID_NAME,
		MIX_INVALID_ENUM,
		MIX_INVALID_VALUE,
		MIX_INVALID_OPERATION,
		MIX_OUT_OF_MEMORY,
		MIX_DRIVER_FAILURE,

		/* Source properties */
		MIX_POSITION,
		MIX_LOOPING,
		MIX_BUFFER,
		MIX_GAIN,
		MIX_SOURCE_STATE,
		MIX_BUFFERS_QUEUED,
		MIX_BUFFERS_PROCESSED,

		/* Source state information */
		MIX_INITIAL,
		MIX_STOPPED,
		MIX_PLAYING,
		MIX_PAUSED,

		/* Sound buffer properties */ 
		MIX_FREQUENCY,
		MIX_BITS,
		MIX_CHANNELS,
		MIX_SIZE,
		MIX_DATA,
		MIX_FORMAT_MONO16,
		MIX_FORMAT_STEREO16,
		MIX_FORMAT_MONO8,
		MIX_FORMAT_STEREO8
	},

	/* Sources */
	mixSDL_GenSources,
	mixSDL_DeleteSources,
	mixSDL_IsSource,
	mixSDL_Sourcei,
	mixSDL_Sourcef,
	mixSDL_Sourcefv,
	mixSDL_GetSourcei,
	mixSDL_GetSourcef,
	mixSDL_SourceRewind,
	mixSDL_SourcePlay,
	mixSDL_SourcePause,
	mixSDL_SourceStop,
	mixSDL_SourceQueueBuffers,
	mixSDL_SourceUnqueueBuffers,

	/* Buffers */
	mixSDL_GenBuffers,
	mixSDL_DeleteBuffers,
	mixSDL_IsBuffer,
	mixSDL_GetBufferi,
	mixSDL_BufferData
};


/*
 * Initialization
 */

sint32 
mixSDL_Init (audio_Driver *driver, sint32 flags)
{
	int i;
	char devicename[256];
	SDL_AudioSpec desired, obtained;
	mixer_Quality quality;
	TFB_DecoderFormats formats =
	{
		MIX_IS_BIG_ENDIAN, MIX_WANT_BIG_ENDIAN,
		audio_FORMAT_MONO8, audio_FORMAT_STEREO8,
		audio_FORMAT_MONO16, audio_FORMAT_STEREO16
	};

	fprintf (stderr, "Initializing SDL audio subsystem.\n");
	if ((SDL_InitSubSystem(SDL_INIT_AUDIO)) == -1)
	{
		fprintf (stderr, "Couldn't initialize audio subsystem: %s\n", SDL_GetError());
		return -1;
	}
	fprintf (stderr, "SDL audio subsystem initialized.\n");
	
	if (flags & audio_QUALITY_HIGH)
	{
		quality = MIX_QUALITY_HIGH;
		desired.freq = 44100;
		desired.samples = 4096;
	}
	else if (flags & audio_QUALITY_LOW)
	{
		quality = MIX_QUALITY_LOW;
		desired.freq = 22050;
		desired.samples = 2048;
	}
	else
	{
		quality = MIX_QUALITY_DEFAULT;
		desired.freq = 44100;
		desired.samples = 4096;
	}

	desired.format = AUDIO_S16SYS;
	desired.channels = 2;
	desired.callback = mixer_MixChannels;
	
	fprintf (stderr, "Opening SDL audio device.\n");
	if (SDL_OpenAudio (&desired, &obtained) < 0)
	{
		fprintf (stderr, "Unable to open audio device: %s\n", SDL_GetError ());
		SDL_QuitSubSystem (SDL_INIT_AUDIO);
		return -1;
	}
	if (obtained.format != desired.format ||
		(obtained.channels != 1 && obtained.channels != 2))
	{
		fprintf (stderr, "Unable to obtain desired audio format.\n");
		SDL_CloseAudio ();
		SDL_QuitSubSystem (SDL_INIT_AUDIO);
		return -1;
	}

	SDL_AudioDriverName (devicename, sizeof (devicename));
	fprintf (stderr, "    using %s at %d Hz 16 bit %s, %d samples audio buffer\n",
			devicename, obtained.freq, obtained.channels > 1 ? "stereo" : "mono",
			obtained.samples);

	fprintf (stderr, "Initializing mixer.\n");
	if (!mixer_Init (obtained.freq, MIX_FORMAT_MAKE (2, obtained.channels),
			quality, 0))
	{
		fprintf (stderr, "Mixer initialization failed: %x\n",
				mixer_GetError ());
		SDL_CloseAudio ();
		SDL_QuitSubSystem (SDL_INIT_AUDIO);
		return -1;
	}
	fprintf (stderr, "Mixer initialized.\n");

	fprintf (stderr, "Initializing sound decoders.\n");
	if (SoundDecoder_Init (flags, &formats))
	{
		fprintf (stderr, "Sound decoders initialization failed.\n");
		SDL_CloseAudio ();
		mixer_Uninit ();
		SDL_QuitSubSystem (SDL_INIT_AUDIO);
		return -1;
	}
	fprintf (stderr, "Sound decoders initialized.\n");

	*driver = mixSDL_Driver;
	for (i = 0; i < NUM_SOUNDSOURCES; ++i)
	{
		audio_GenSources (1, &soundSource[i].handle);		
		soundSource[i].sample = NULL;
		soundSource[i].stream_should_be_playing = FALSE;
		soundSource[i].stream_mutex = CreateMutex ("MixSDL stream mutex", SYNC_CLASS_AUDIO);
		soundSource[i].sbuffer = NULL;
		soundSource[i].sbuf_start = 0;
		soundSource[i].sbuf_size = 0;
		soundSource[i].sbuf_offset = 0;
	}

	atexit (unInitAudio);

	SetSFXVolume (sfxVolumeScale);
	SetSpeechVolume (speechVolumeScale);
	SetMusicVolume ((COUNT)musicVolume);
		
	StreamDecoderTask = AssignTask (StreamDecoderTaskFunc, 1024, 
		"audio stream decoder");

	SDL_PauseAudio (0);
	
	return 0;
}

void
mixSDL_Uninit (void)
{
	int i;

	if (StreamDecoderTask)
	{
		ConcludeTask (StreamDecoderTask);
		StreamDecoderTask = NULL;
	}

	for (i = 0; i < NUM_SOUNDSOURCES; ++i)
	{
		if (soundSource[i].sample && soundSource[i].sample->decoder)
		{
			StopStream (i);
		}
		if (soundSource[i].sbuffer)
		{
			void *sbuffer = soundSource[i].sbuffer;
			soundSource[i].sbuffer = NULL;
			HFree (sbuffer);
		}
		DestroyMutex (soundSource[i].stream_mutex);

		mixSDL_DeleteSources (1, &soundSource[i].handle);
	}

	SDL_CloseAudio ();
	mixer_Uninit ();
	SoundDecoder_Uninit ();
	SDL_QuitSubSystem (SDL_INIT_AUDIO);
}


/*
 * General
 */

sint32
mixSDL_GetError (void)
{
	sint32 value = mixer_GetError ();
	switch (value)
	{
		case MIX_NO_ERROR:
			return audio_NO_ERROR;
		case MIX_INVALID_NAME:
			return audio_INVALID_NAME;
		case MIX_INVALID_ENUM:
			return audio_INVALID_ENUM;
		case MIX_INVALID_VALUE:
			return audio_INVALID_VALUE;
		case MIX_INVALID_OPERATION:
			return audio_INVALID_OPERATION;
		case MIX_OUT_OF_MEMORY:
			return audio_OUT_OF_MEMORY;
		default:
			fprintf (stderr, "mixSDL_GetError: unknown value %x\n", value);
			return audio_DRIVER_FAILURE;
			break;
	}
}


/*
 * Sources
 */

void
mixSDL_GenSources (uint32 n, audio_Object *psrcobj)
{
	mixer_GenSources (n, (mixer_Object *) psrcobj);
}

void
mixSDL_DeleteSources (uint32 n, audio_Object *psrcobj)
{
	mixer_DeleteSources (n, (mixer_Object *) psrcobj);
}

bool
mixSDL_IsSource (audio_Object srcobj)
{
	return mixer_IsSource ((mixer_Object) srcobj);
}

void
mixSDL_Sourcei (audio_Object srcobj, audio_SourceProp pname,
		audio_IntVal value)

{
	mixer_Sourcei ((mixer_Object) srcobj, (mixer_SourceProp) pname,
			(mixer_IntVal) value);
}

void
mixSDL_Sourcef (audio_Object srcobj, audio_SourceProp pname,
		float value)
{
	mixer_Sourcef ((mixer_Object) srcobj, (mixer_SourceProp) pname, value);
}

void
mixSDL_Sourcefv (audio_Object srcobj, audio_SourceProp pname,
		float *value)
{
	mixer_Sourcefv ((mixer_Object) srcobj, (mixer_SourceProp) pname, value);
}

void
mixSDL_GetSourcei (audio_Object srcobj, audio_SourceProp pname,
		audio_IntVal *value)
{
	mixer_GetSourcei ((mixer_Object) srcobj, (mixer_SourceProp) pname,
			(mixer_IntVal *) value);
	if (pname == MIX_SOURCE_STATE)
	{
		switch (*value)
		{
			case MIX_INITIAL:
				*value = audio_INITIAL;
				break;
			case MIX_STOPPED:
				*value = audio_STOPPED;
				break;
			case MIX_PLAYING:
				*value = audio_PLAYING;
				break;
			case MIX_PAUSED:
				*value = audio_PAUSED;
				break;
			default:
				*value = audio_DRIVER_FAILURE;
		}
	}
}

void
mixSDL_GetSourcef (audio_Object srcobj, audio_SourceProp pname,
		float *value)
{
	mixer_GetSourcef ((mixer_Object) srcobj, (mixer_SourceProp) pname, value);
}

void
mixSDL_SourceRewind (audio_Object srcobj)
{
	mixer_SourceRewind ((mixer_Object) srcobj);
}

void
mixSDL_SourcePlay (audio_Object srcobj)
{
	mixer_SourcePlay ((mixer_Object) srcobj);
}

void
mixSDL_SourcePause (audio_Object srcobj)
{
	mixer_SourcePause ((mixer_Object) srcobj);
}

void
mixSDL_SourceStop (audio_Object srcobj)
{
	mixer_SourceStop ((mixer_Object) srcobj);
}

void
mixSDL_SourceQueueBuffers (audio_Object srcobj, uint32 n,
		audio_Object* pbufobj)
{
	mixer_SourceQueueBuffers ((mixer_Object) srcobj, n,
			(mixer_Object *) pbufobj);
}

void
mixSDL_SourceUnqueueBuffers (audio_Object srcobj, uint32 n,
		audio_Object* pbufobj)
{
	mixer_SourceUnqueueBuffers ((mixer_Object) srcobj, n,
			(mixer_Object *) pbufobj);
}


/*
 * Buffers
 */

void
mixSDL_GenBuffers (uint32 n, audio_Object *pbufobj)
{
	mixer_GenBuffers (n, (mixer_Object *) pbufobj);
}

void
mixSDL_DeleteBuffers (uint32 n, audio_Object *pbufobj)
{
	mixer_DeleteBuffers (n, (mixer_Object *) pbufobj);
}

bool
mixSDL_IsBuffer (audio_Object bufobj)
{
	return mixer_IsBuffer ((mixer_Object) bufobj);
}

void
mixSDL_GetBufferi (audio_Object bufobj, audio_BufferProp pname,
		audio_IntVal *value)
{
	mixer_GetBufferi ((mixer_Object) bufobj, (mixer_BufferProp) pname,
			(mixer_IntVal *) value);
}

void
mixSDL_BufferData (audio_Object bufobj, uint32 format, void* data,
		uint32 size, uint32 freq)
{
	mixer_BufferData ((mixer_Object) bufobj, format, data, size, freq);
}
