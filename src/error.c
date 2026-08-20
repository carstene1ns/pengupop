/*  Error handling routines.
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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"

void fatal_error(const char* format, ...)
{
  va_list args;
  char buf[512];

  va_start(args, format);
  vsnprintf(buf, sizeof(buf), format, args);
  buf[sizeof(buf) - 1] = 0;

  if(!SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Pengupop", buf, NULL /*window*/)) {
    fwrite(buf, sizeof(buf), 1, stderr);
    fwrite("\n", 1, 1, stderr);
  }

  exit(EXIT_FAILURE);
}
