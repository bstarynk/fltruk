#ifndef FLTRUK_INCLUDED
#define FLTRUK_INCLUDED 1
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
#include <mutex>
#include <variant>
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
extern "C" void* fltruk_dlopen(const char*plugin, const char*arg);
extern "C" void* fltruk_dlsym(const char*symname);

// every dlopened plugin should have a fltruk_init function of signature
typedef bool fltruk_init_rout_t (const char*);

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

enum class fltruk_Vtype: uint8_t {
    _Notype = 0,
    IntTy,
    DoubleTy,
    StringTy,
    NodeTy,
    FltkTy,
    ObjectTy,
    _LastTy
};

struct fltruk_IntV;
struct fltruk_DoubleV;
struct fltruk_StringV;
struct fltruk_NodeV;
struct fltruk_FltkV;
struct fltruk_ObjectV;
struct fltruk_Value;


enum class fltruk_Selector: uint16_t {
    _NoSel = 0,
    DestroySel,
    OutSel,
    InputSel,
    StringSel,
    ConstValSel,
    HjsonSel,
    _LastSel
};

typedef std::variant<void*,
        std::uintptr_t,
        std::ostream*,
        std::istream*,
        std::string,
        const fltruk_Value*,
        Hjson::Value*
        > fltruk_Variant;

typedef void fltruk_methsig_t (uint16_t /*selector*/,
                               struct fltruk_Value*,
                               struct fltruk_Value*,
                               struct fltruk_Value*,
                               fltruk_Variant);

struct fltruk_MethodDict {
    mutable std::recursive_mutex _mtx;
    std::map<uint16_t, fltruk_methsig_t*> _dictmeth;
    fltruk_methsig_t* find_meth(uint16_t selnum) const {
        if (selnum == 0)
            return nullptr;
        std::lock_guard<std::recursive_mutex> gu(_mtx);
        auto m = _dictmeth.find(selnum);
        if (m != _dictmeth.end())
            return nullptr; //m.second;
        return nullptr;
    };                            // end find_meth
};                              // end fltruk_MethodDict

struct fltruk_Value {
    const fltruk_Vtype _vtype;
    mutable uint8_t _vgc;
    const uint16_t _vnum;
    const uint32_t _vhash;
    const std::shared_ptr<fltruk_MethodDict> _vmethdict;
protected:
    fltruk_Value(fltruk_Vtype ty, uint16_t num,
                 uint32_t hash,fltruk_MethodDict&mdict)
        : _vtype(ty), _vgc(0), _vnum(num), _vhash(hash),
          _vmethdict(&mdict) {
        assert (ty > fltruk_Vtype::_Notype && ty < fltruk_Vtype::_LastTy);
        assert (num > 0);
        assert (hash > 0);
    };
    ~fltruk_Value() {
#warning incomplete fltruk_Value destructor
    };
};



/********
 ** for GNU emacs...
 ** Local-Variables: ;;
 ** compile-command: "make -j3" ;;
 ** End: ;;
 *********/
// end of file fltruk/fltruk.hh
#endif /*FLTRUK_INCLUDED*/
