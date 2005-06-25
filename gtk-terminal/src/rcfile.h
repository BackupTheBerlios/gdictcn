/*
 * gtk-terminal 
 * Author: Dave Young 
 *
 * This software is released under the GNU General Public License (GNU GPL).
 * Please read the included file COPYING for more information.
 * This software comes with no warranty of any kind, use at your own risk!
 */


#ifndef RCFILE_H
#define RCFILE_H

#include "window.h"

#define RCFILE ".gtk-terminalrc"
gchar *quoted_value(const gchar *text);

void save_options(confinfo * cf);
void load_options(confinfo * cf);


#endif

