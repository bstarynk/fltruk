/****************************************************************
 * file fltruk/main-fltruk.cc
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Description:
 *      This file is the main file of fltruk
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
#include "fltruk.hh"

extern "C" const char main_fltruk_git[] = FLTRUK_SHORTGIT;
extern "C" const char main_fltruk_base[] = FLTRUK_BASE;

void* fltruk_proghdl;
volatile bool verbose_fltruk;

static int fltruk_prog_argc;
static const char**fltruk_prog_argv;

static std::string fltruk_curarg;
static void
usage_fltruk(void)
{
    std::clog << fltruk_prog_argv[0] << " usage:" << std::endl
              << "\t --help | -h                       # give this help"
              << std::endl
              << "\t --version | -V                    # give version"
              << std::endl
              << "\t --verbose | -v                    # verbose messages (FLTRUK_VERBAL)"
              << std::endl
              << "\t --arg | -A <ARGSTR>               # set argument for next plugin"
              << std::endl
              << "\t --plugin | -P <PLUGIN>            # dlopen this plugin"
              << std::endl
              << "\t                                   # call its fltruk_init function"
              << std::endl
              << "\t                                   # with the const char* <ARGSTR>"
              << std::endl
              << "\t# cf github.com/bstarynk/fltruk" << std::endl;
} // end usage_fltruk

static void
version_fltruk(void)
{
    std::clog << fltruk_prog_argv[0] << " version:" << std::endl
              << "\t git " << FLTRUK_SHORTGIT << std::endl
              << "\t FLTK ABI: " << Fl::abi_version() << std::endl
              << "\t FLTK API: " << Fl::api_version() << std::endl;
    std::clog << "\t\t cf github.com/bstarynk/fltruk"
              << " & starynkevitch.net/Basile" << std::endl;
} // end version_fltruk

/*** our arghandle_fltruk function should
 * Returns 2 if argv[i] matches with required parameter in argv[i+1],
 * returns 1 if argv[i] matches on its own,
 * returns 0 if argv[i] does not match.
 ***/
static int
arghandle_fltruk(int argc, char**argv, int &i)
{
    assert (argc>0);
    assert (argv);
    assert (i>=0);
    asm volatile ("nop");
    if (strcmp("-h", argv[i]) == 0
            || strcmp("--help", argv[i]) == 0) {
        usage_fltruk();
        i++;
        return 1;
    }
    else if (strcmp("-V", argv[i]) == 0
             || strcmp("--version", argv[i]) == 0) {
        version_fltruk();
        i++;
        return 1;
    }
    else if (strcmp("-v", argv[i]) == 0
             || strcmp("--verbose", argv[i]) == 0) {
        verbose_fltruk = true;
        i++;
        return 1;
    }
    else if (i+1<argc && (strcmp("-A", argv[i]) == 0
                          || strcmp("--arg", argv[i]) == 0)) {
        fltruk_curarg = std::string(argv[i+1]);
        FLTRUK_VERBOSE("program argument #%d set argument to %s",
                       i, fltruk_curarg.c_str());
        i += 2;
        return 2;
    }
    else if (i+1<argc && (strcmp("-P", argv[i]) == 0
                          || strcmp("--plugin", argv[i]) == 0)) {
        const char*plugin_name = (argv[i+1]);
        FLTRUK_VERBOSE("%s loading plugin %s",
                       fltruk_program_name(), plugin_name);
        void* plughdl = dlopen(plugin_name, RTLD_NOW | RTLD_GLOBAL);
        if (!plughdl) {
            FLTRUK_WARNING("%s failed to dlopen %s: %s",
                           fltruk_program_name (), plugin_name,
                           dlerror());
            exit (EXIT_FAILURE);
        };
        FLTRUK_VERBOSE("%s dlopened plugin %s",
                       fltruk_program_name(), plugin_name);
        void*initad = dlsym(plughdl, "fltruk_init");
        if (!initad) {
            FLTRUK_WARNING("%s failed to dlsym fltruk_init in %s: %s",
                           fltruk_program_name (), plugin_name,
                           dlerror());
            exit (EXIT_FAILURE);
        };
        FLTRUK_VERBOSE("%s running in plugin %s"
                       " its fltruk_init (@%p) with '%s'",
                       fltruk_program_name(),  plugin_name, initad,
                       fltruk_curarg.c_str());
        fflush (nullptr);
        bool ok = ((fltruk_init_rout_t*)initad) (fltruk_curarg.c_str());
        if (!ok) {
            FLTRUK_WARNING("%s failed to run fltruk_init (%s) in plugin %s",
                           fltruk_program_name (), fltruk_curarg.c_str(),
                           plugin_name);
            exit(EXIT_FAILURE);
        }
        FLTRUK_VERBOSE("%s did run fltruk_init (%s) in plugin %s",
                       fltruk_program_name (), fltruk_curarg.c_str(),
                       plugin_name);
        i += 2;
        return 2;
    }
    return 0;
} // end arghandle_fltruk


/// GDB can put a breakpoint inside
void
warned_at_fltruk(const char*fil, int lin, const char*fun)
{
    assert (fil);
    assert (lin);
    assert (fun);
    asm volatile ("nop; nop; nop; nop");
} // end warned_at_fltruk



/// GDB can put a breakpoint inside
void
verbal_at_fltruk(const char*fil, int lin, const char*fun)
{
    assert (fil);
    assert (lin);
    assert (fun);
    asm volatile ("nop; nop; nop; nop");
} // end verbal_at_fltruk


extern "C" const char*source_files_fltruk[]; // in generated _listfiles_.cc
void
fltruk_do_every_source_file (fltruk_source_file_handler_t*p, void*data)
{
    if (!p)
        return;
    for (int ix=0; source_files_fltruk[ix] != nullptr; ix++)
        (*p) (source_files_fltruk[ix], ix, data);
} // end fltruk_do_every_source_file


const char*
fltruk_program_name(void)
{
    return fltruk_prog_argv[0];
} // end fltruk_program_name

int
fltruk_program_arg_count(void)
{
    return
        fltruk_prog_argc;
} // end fltruk_program_arg_count

const char*
fltruk_fetch_prog_arg(int rk)
{
    if (rk<0)
        rk += fltruk_prog_argc;
    if (rk>=0 && rk<fltruk_prog_argc)
        return fltruk_prog_argv[rk];
    return nullptr;
} // end fltruk_fetch_prog_arg


void*
fltruk_dlsym(const char*name)
{
    void* ad = nullptr;
    if (name || !isalpha(name[0])) {
        FLTRUK_WARNING("%s calling fltruk_dlsym with bad name",
                       fltruk_prog_argv[0]);
        return nullptr;
    }
    assert (fltruk_proghdl != nullptr);
    ad = dlsym(fltruk_proghdl, name);
    if (!ad) {
        FLTRUK_WARNING("%s fltruk_dlsym fails with name %s",
                       fltruk_prog_argv[0], name);
        return nullptr;
    }
    FLTRUK_VERBOSE("%s fltruk_dlsym %s -> %p",
                   fltruk_prog_argv[0], name, ad);
    return ad;
}				 // end fltruk_dlsym


extern "C" const char*source_directory_fltruk[]; // in generated _listfiles_.cc
int
main(int argc, char**argv)
{
    int iarg= 0;
    fltruk_prog_argc = argc;
    fltruk_prog_argv = (const char**)argv;
    fltruk_proghdl = dlopen (NULL, RTLD_NOW);
    assert (fltruk_proghdl);
    assert (main_fltruk_base);
    assert (main_fltruk_git);
    if (argc>1) {
        if (!strcmp(argv[1], "--help")) {
            usage_fltruk();
            return 0;
        } else if (!strcmp(argv[1], "--version")) {
            version_fltruk();
            return 0;
        };
    };
    int r = Fl::args(argc, argv, iarg, arghandle_fltruk);
    assert (r>=0);
    fltruk_compile_plugins ();
    return 0;
} // end main

/********
 ** for GNU emacs...
 ** Local-Variables: ;;
 ** compile-command: "make -j3" ;;
 ** End: ;;
 *********/
// end of file fltruk/main-fltruk.cc
