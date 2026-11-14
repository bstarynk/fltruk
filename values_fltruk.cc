/****************************************************************
 * file fltruk/values_fltruk.cc
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Description:
 *      This file is the values file of fltruk
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

extern "C" const char values_fltruk_git[] = FLTRUK_SHORTGIT;
extern "C" const char values_fltruk_base[] = FLTRUK_BASE;


fltruk_MethodDict fltruk_IntV::meth_int_dict;

uint16_t
fltruk_IntV::register_int_val(struct fltruk_IntV*p, intptr_t i)
{
#warning unimplemented fltruk_IntV::register_int_val
    FLTRUK_VERBOSE("fail to €register_int_val p@%p i=%ld",
                   (void*)p, (long)i);
    asm volatile("nop; nop; nop; nop");
    fflush(nullptr);
    throw std::runtime_error("register_int_val");
} // end fltruk_IntV::register_int_val




fltruk_MethodDict fltruk_DoubleV::meth_double_dict;

uint16_t
fltruk_DoubleV::register_double_val(struct fltruk_DoubleV*pd, double x)
{
#warning unimplemented fltruk_DoubleV::register_double_val
    FLTRUK_VERBOSE("fail to register_double_val pd@%p x=%g",
                   (void*)pd, x);
    asm volatile("nop; nop; nop; nop");
    fflush(nullptr);
    throw std::runtime_error("register_double_val");
} // end fltruk_IntV::register_double_val




/********
 ** for GNU emacs...
 ** Local-Variables: ;;
 ** compile-command: "make -j3" ;;
 ** End: ;;
 *********/
// end of file fltruk/values_fltruk.cc
