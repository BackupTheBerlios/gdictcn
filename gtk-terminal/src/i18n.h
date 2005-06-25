/*
 * gtk-terminal 
 * Author: Dave Young <lonelypenguin@gmail.com> 
 *
 * This software is released under the GNU General Public License (GNU GPL).
 * Please read the included file COPYING for more information.
 * This software comes with no warranty of any kind, use at your own risk!
 */

#ifndef I18N_H 
#define I18N_H
#include <libintl.h>
#include <locale.h>
#define _(text) gettext(text)
#define N_(text) (text)
#endif
