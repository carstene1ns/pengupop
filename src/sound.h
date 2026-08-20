/*
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

#ifndef SOUND_H
#define SOUND_H

typedef enum {
    SFX_DESTROY = 0,
    SFX_LAUNCH,
    SFX_REBOUND,
    SFX_STICK,
    SFX_MAX
} sfx;

extern bool sound_enabled;

extern void init_sound();
extern void deinit_sound();

extern void sound_play(sfx snd);

#endif
