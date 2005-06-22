#include "gtk-terminal.h"

#ifdef HAVE_CONFIG_H
#  	include <config.h>
#endif

int main (int argc, char *argv[])
{
  	GtkWidget *window1;
  	MainWin * mw;

#ifdef ENABLE_NLS
  	bindtextdomain (GETTEXT_PACKAGE, LOCALE_DIR);
  	bind_textdomain_codeset (GETTEXT_PACKAGE, NULL);
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
