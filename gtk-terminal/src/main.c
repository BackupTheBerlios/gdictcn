/*
 * gtk-terminal 
 * Author: Dave Young <lonelypenguin@gmail.com> 
 *
 * This software is released under the GNU General Public License (GNU GPL).
 * Please read the included file COPYING for more information.
 * This software comes with no warranty of any kind, use at your own risk!
 */

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif
#include "gtk-terminal.h"

int main (int argc, char *argv[])
{
  	GtkWidget *window1;
  	MainWin * mw;

#ifdef ENABLE_NLS
  	bindtextdomain (GETTEXT_PACKAGE, LOCALE_DIR);
  	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  	textdomain (GETTEXT_PACKAGE);
#endif
  	gtk_set_locale ();
  	gtk_init (&argc, &argv);

  	mw=create_main_window();
  	window1 = mw->window; 
  	gtk_widget_show_all (window1);
  	gtk_main ();
  	return 0;
}
