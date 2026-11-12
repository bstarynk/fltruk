/****************************************************************
 * file fltruk/plugins-fltruk.cc
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Description:
 *      This file manage plugins of fltruk
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
#include <elf.h>
#include <stdio.h>

extern "C" const char plugins_fltruk_git[] = FLTRUK_SHORTGIT;
extern "C" const char plugins_fltruk_base[] = FLTRUK_BASE;

extern "C" const char source_directory_fltruk[];

extern "C" const char make_fltruk[];

void
fltruk_compile_plugins(void)
{
    struct dirent* ent = nullptr;
    int compilcnt = 0;
    char genpath[384];
    memset (genpath, 0, sizeof(genpath));
    snprintf(genpath, sizeof(genpath)-1,
             "%s/generated/", source_directory_fltruk);
    DIR* gendir= opendir(genpath);
    if (!gendir) {
        FLTRUK_WARNING("fail to opendir %s", genpath);
        return;
    };
    do {
        int entlen = 0;
        constexpr int srclen=80;
        char srcpath[sizeof(genpath) + srclen + 4];
        memset(srcpath, 0, sizeof(srcpath));
        char pluginpath[sizeof(srcpath)];
        memset(pluginpath, 0, sizeof(pluginpath));
        char cmdbuf[sizeof(genpath) + 2*srclen + 4];
        memset (cmdbuf, 0, sizeof(cmdbuf));
        ent = readdir(gendir);
        if (!ent)
            return;
        if (ent->d_type != DT_REG && ent->d_type != DT_LNK)
            continue;
        entlen = strlen(ent->d_name);
        assert (entlen < ent->d_reclen && entlen > 0);
        if (ent->d_name[0] == '.' || ent->d_name[0] == '!')
            continue;
        bool goodent = isalnum(ent->d_name[0]) || ent->d_name[0] == '_';
        for  (int i=0; i<entlen && goodent; i++)
            goodent =
                isalnum(ent->d_name[i])
                || ent->d_name[i] == '_' || ent->d_name[i] == '.'
                || ent->d_name[i] == '+' || ent->d_name[i] == '-';
        if (!goodent) {
            FLTRUK_WARNING("in %s bad entry %s", genpath, ent->d_name);
            continue;
        }
        if (entlen < 4 || entlen>srclen+(int)sizeof(genpath))
            continue;
        if (ent->d_name[entlen-2] != '.'
                && ent->d_name[entlen-1] != 'c')
            continue;
        int slen = snprintf (srcpath, sizeof(srcpath), "%s/generated/%s",
                             source_directory_fltruk, ent->d_name);
        FLTRUK_VERBOSE("plugin srcpath %s (ent %s)", srcpath, ent->d_name);
        if (slen<0) {
            FLTRUK_WARNING("snprintf failed for %s in source dir %s",
                           ent->d_name, source_directory_fltruk);
            continue;
        }
        int pluglen =
            snprintf (pluginpath, sizeof(pluginpath)-1,
                      "%s/obj/%s", source_directory_fltruk,
                      strrchr(srcpath, '/')+1);
        assert (pluglen>4);
        assert (pluglen<(int)sizeof(pluginpath)-4);
        assert (pluginpath[pluglen-2] == '.'
                && pluginpath[pluglen-1] == 'c');
        pluginpath[pluglen-1] = 's';
        pluginpath[pluglen] = 'o';
        pluginpath[pluglen+1] = (char)0;
        if (2*slen + 32 > (int) sizeof(cmdbuf))
            FLTRUK_WARNING("probably too long source %s and plugin %s files",
                           srcpath, pluginpath);
        FLTRUK_VERBOSE("plugin srcpath= %s  pluginpath= %s",
                       srcpath, pluginpath);
        struct stat srcstat = {};
        struct stat pluginstat = {};
        assert (0 == *(char*)&srcstat);
        assert (0 == *(char*)&pluginstat);
        if (stat (srcpath, &srcstat)) {
            FLTRUK_WARNING("failed to stat source file %s (%s)",
                           srcpath, strerror(errno));
            continue;
        };
        assert (srcstat.st_uid > 0);
        if (stat (pluginpath, &pluginstat)
                || pluginstat.st_mtime <= srcstat.st_mtime) {
            const char *lastslplug = strrchr(pluginpath, '/');
            if (lastslplug)
                lastslplug++;
            else
                lastslplug = pluginpath;
            int cmdlen = snprintf (cmdbuf, sizeof(cmdbuf)-1,
                                   "%s -C %s obj/%s",
                                   make_fltruk, source_directory_fltruk, lastslplug);
            if (cmdlen<0) {
                FLTRUK_WARNING("failed to synthesize command to build %s in %s",
                               pluginpath, source_directory_fltruk);
                continue;
            }
            else {
                assert(cmdlen>0);
                fflush (nullptr);
                FLTRUK_VERBOSE("%s running command %s",
                               fltruk_program_name(),
                               cmdbuf);
                int cmdres = system (cmdbuf);
                if (cmdres != 0)
                    FLTRUK_WARNING("command %s failed", cmdbuf);
                compilcnt++;
            }
        };
    } while (ent);
    closedir(gendir);
    FLTRUK_VERBOSE("compile_plugins_fltruk did %d compilations",
                   compilcnt);
} // end compile_plugins_fltruk

void*
fltruk_dlopen(const char*plugin, const char*arg)
{
    char buf[8];
    memset(buf, 0, sizeof(buf));
    if (!plugin || !plugin[0] || !strstr(plugin, ".so")) {
        FLTRUK_WARNING("invalid plugin (%s) to fltruk_dlopen",
                       plugin?:"<€null>");
        return nullptr;
    }
    if (!arg) {
        FLTRUK_WARNING("missing arg to plugin %s",plugin);
        return nullptr;
    }
    FLTRUK_VERBOSE("%s fltruk_dlopen plugin:%s arg:%s",
                   fltruk_program_name(), plugin, arg);
    FILE* f = fopen(plugin, "rb");
    if (!f) {
        FLTRUK_WARNING("% fltruk_dlopen failed to read plugin %s: %s",
                       fltruk_program_name(), plugin, strerror(errno));
        return nullptr;
    };
    if (fread(buf, sizeof(buf), 1, f) == 0) {
        FLTRUK_WARNING("% fltruk_dlopen failed to read first "
                       " %d bytes in plugin %s: %s",
                       fltruk_program_name(), (int)sizeof(buf),
                       plugin, strerror(errno));
        fclose(f);
        return nullptr;
    };
    if (buf[0] != EI_MAG0 || buf[1] != EI_MAG1
            || buf[2] != EI_MAG2 || buf[3] != EI_MAG3) {
        FLTRUK_WARNING("% fltruk_dlopen bad plugin %s start",
                       fltruk_program_name(), plugin);
        fclose(f);
        return nullptr;
    }
    fclose(f), f= nullptr;
    void* dlh = dlopen(plugin, RTLD_NOW);
    if (!dlh) {
        FLTRUK_WARNING("% fltruk_dlopen failed to dlopen plugin %s: %s",
                       fltruk_program_name(), plugin, dlerror());
        return nullptr;
    };
    void *iniad = dlsym(dlh, "fltruk_init");
    if (!iniad) {
        FLTRUK_WARNING("% fltruk_dlopen failed to dlsym fltruk_init"
                       " in plugin %s: %s",
                       fltruk_program_name(), plugin, dlerror());
        dlclose(dlh);
        return nullptr;
    }
    bool ok =  (*(fltruk_init_rout_t*)iniad) (arg);
    if (!ok) {
        FLTRUK_WARNING("% fltruk_dlopen failed to initialize plugin %s with arg %s",
                       fltruk_program_name(), plugin, arg);
        dlclose(dlh);
        return nullptr;
    };
    FLTRUK_VERBOSE("%s did fltruk_dlopen plugin %s arg %s -> %p",
                   fltruk_program_name(), plugin, arg, dlh);
    return dlh;
} // end fltruk_dlopen

// fltruk_dlsym is in main_fltruk.cc

/********
 ** for GNU emacs...
 ** Local-Variables: ;;
 ** compile-command: "make -j3" ;;
 ** End: ;;
 *********/
// end of file fltruk/plugins_fltruk.cc
