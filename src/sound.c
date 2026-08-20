/*  Very simple sample loader and mixer.
    Copyright (C) 2006  Morten Hustveit <morten@rashbox.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <SDL3/SDL.h>

#include "destroy_group.h"
#include "launch.h"
#include "rebound.h"
#include "stick.h"

#include "sound.h"

typedef struct sound_s
{
  SDL_AudioSpec spec;
  Uint8* buf;
  Uint32 len;

  Uint32 pos;
} sound;
sound sounds[4];

bool sound_enabled = true, sound_working = true;
SDL_AudioSpec sdl_audio;
SDL_AudioStream *stream;

static void SDLCALL audio_callback(void */*userdata*/, SDL_AudioStream *stream,
  int additional_amount, int /*total_amount*/)
{
  if (additional_amount > 0)
  {
    Uint8 *data = SDL_stack_alloc(Uint8, additional_amount);
    if (data)
    {
      signed short* sample = (signed short*) data;
      int len = additional_amount / 2;

      for(int i = 0; i < len; i++, sample++)
      {
        int val = 0;

        for(int j = 0; j < SFX_MAX; j++)
        {
          if(sounds[j].pos < sounds[j].len)
          {
            val += *((signed short*) &sounds[j].buf[sounds[j].pos]);
            sounds[j].pos += 2;
          }
        }

        // clipping
        if(val < -32768)
          *sample = -32768;
        else if(val > 32767)
          *sample = 32767;
        else
          *sample = val;
      }

      SDL_PutAudioStreamData(stream, data, additional_amount);
      SDL_stack_free(data);
    }
  }
}

static void load_sounds()
{
  const unsigned char *datas[] = { destroy_group, launch, rebound, stick };
  size_t sizes[] = { size_destroy_group, size_launch, size_rebound, size_stick };

  for(int i = 0; i < SFX_MAX; i++)
  {
    if(!SDL_LoadWAV_IO(SDL_IOFromConstMem(datas[i], sizes[i]), true,
      &sounds[i].spec, &sounds[i].buf, &sounds[i].len))
    {
      sound_working = false;
      return;
    }
  }

  for(int i = 0; i < SFX_MAX; i++)
  {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    swab(sounds[i].buf, sounds[i].buf, sounds[i].len);
#endif
    sounds[i].pos = sounds[i].len;
  }
}

void init_sound() {
  sdl_audio.freq = 44100;
  sdl_audio.format = SDL_AUDIO_S16;
  sdl_audio.channels = 1;

  stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
    &sdl_audio, audio_callback, NULL);
  if (!stream) {
      sound_working = false;
      return;
  }

  load_sounds();

  if(!sound_working) {
    return;
  }
  SDL_ResumeAudioStreamDevice(stream);
}

void deinit_sound() {
  SDL_DestroyAudioStream(stream);

  for(int i = 0; i < SFX_MAX; i++)
  {
    SDL_free(sounds[i].buf);
  }
}

void sound_play(sfx snd) {
  if(!sound_enabled) return;
  if(snd < 0 || snd >= SFX_MAX) return;

  sounds[snd].pos = 0;
}
