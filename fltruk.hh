#ifndef FLTRUK_INCLUDED
/****************************************************************
 * file fltruk/fltruk.hh
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Description:
 *      This file is the included header file of fltruk
 *      © Copyright (C) 2026, Basile Starynkevitch, France
 *       <basile@starynkevitch.net>
 * It is GPLv3+ licensed (or CeCILL in France)
 *
 * License: GPLv3+ (file COPYING-GPLv3)
 *    This software is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#define FLTRUK_INCLUDED 1

#ifndef FLTRUK_SHORTGIT
#error FLTRUK_SHORTGIT should be defined in compilation command
#endif


#include <dlfcn.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>


#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <ostream>
#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>


/// HJSON from https://github.com/hjson/hjson-cpp
#include "hjson/hjson.h"

extern "C" void* fltruk_proghdl; // for dlopen
extern "C" int fltruk_prog_arg_cnt(void);

extern "C" const char* fltruk_program_name (void);
extern "C" int fltruk_program_arg_count (void);
extern "C" const char*fltruk_fetch_prog_arg(int rk);

typedef void fltruk_source_file_handler_t (const char*, int, void*);
extern "C" void fltruk_do_every_source_file(fltruk_source_file_handler_t*p, void*data);
extern "C" void fltruk_compile_plugins(void);
extern "C" volatile bool verbose_fltruk;


extern "C" void warned_at_fltruk(const char*fil, int lin, const char*fun); // for GDB
#define FLTRUK_WARNING_AT2(Fil,Lin,Func,Fmt,...) do {           \
char*buf##FLTRUK_BASE##Lin = nullptr;                           \
if (asprintf(&buf##FLTRUK_BASE##Lin,Fmt,##__VA_ARGS__)<0)       \
  abort();                                                      \
 assert(buf##FLTRUK_BASE##Lin);                                 \
 Fl::warning("Fltruk WARNING (%s:%d <%s>) %s",                  \
             (Fil), (Lin), (Func),                              \
             buf##FLTRUK_BASE##Lin);                            \
 warned_at_fltruk(Fil,Lin,Func);                                \
 free(buf##FLTRUK_BASE##Lin);                                   \
} while(0)

#define FLTRUK_WARNING_AT(Fil,Lin,Func,Fmt,...) \
  FLTRUK_WARNING_AT2(Fil,Lin,Func,Fmt,##__VA_ARGS__)

#define FLTRUK_WARNING(Fmt,...) \
  FLTRUK_WARNING_AT(__FILE__,__LINE__,__func__,##__VA_ARGS__)



extern "C" void verbal_at_fltruk(const char*fil, int lin, const char*fun); // for GDB
#define FLTRUK_VERBOSE_AT2(Fil,Lin,Func,Fmt,...) do {           \
  if (verbose_fltruk) {                                         \
    char*verbuf##FLTRUK_BASE##Lin = nullptr;                    \
    if (asprintf(&verbuf##FLTRUK_BASE##Lin,                     \
                 (Fmt), ##__VA_ARGS__)<0)                       \
      abort();                                                  \
    std::clog << "Fltruk° "                                     \
              << (Fil) << ":" << (Lin) << " ∈(" << Func << ") " \
              << verbuf##FLTRUK_BASE##Lin << std::endl;         \
 verbal_at_fltruk(Fil,Lin,Func);                                \
 fflush (nullptr);                                              \
 free(verbuf##FLTRUK_BASE##Lin);                                \
} } while(0)

#define FLTRUK_VERBOSE_AT(Fil,Lin,Func,Fmt,...) \
  FLTRUK_VERBOSE_AT2(Fil,Lin,Func,Fmt,##__VA_ARGS__)

#define FLTRUK_VERBOSE(Fmt,...) \
  FLTRUK_VERBOSE_AT(__FILE__,__LINE__,__func__,Fmt,##__VA_ARGS__)


class value_Fltruk {
};
#endif /*FLTRUK_INCLUDED*/
