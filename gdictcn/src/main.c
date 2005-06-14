/*
 * Initial main.c file
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "interface.h"
#include "i18n.h"
int
main (int argc, char *argv[])
{
  GtkWidget *window1;
#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  gtk_set_locale ();
  gtk_init (&argc, &argv);

  window1 = create_window1 ();
  gtk_widget_show (window1);
  gtk_main ();
  return 0;
}

