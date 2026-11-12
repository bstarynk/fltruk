/****************************************************************
 * file fltruk/generated/_pseudo_.c
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Description:
 *      This handwritten file generated/_pseudo_.c fakes to be generated
 *      but is handwritten by
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
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/// every plugin needs one
bool
fltruk_init (const char *arg)
{
  printf ("pseudo-fltruk_init (%s:%d) with %s\n", __FILE__, __LINE__, arg);
  fflush (NULL);
}				/* end fltruk_init */

/********
 ** for GNU emacs...
 ** Local-Variables: ;;
 ** compile-command: "make -C.. -j3" ;;
 ** End: ;;
 *********/
// end of file fltruk/generated/_pseudo_.c
