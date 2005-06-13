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
/*
	GThread *loadingthread;
GError           *err1 = NULL ;
*/
#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  gtk_set_locale ();
  gtk_init (&argc, &argv);

  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
  window1 = create_window1 ();
  gtk_widget_show (window1);
/*
	if( !g_thread_supported() )
  {
     g_thread_init(NULL);
     gdk_threads_init();                   // Called to initialize internal mutex "gdk_threads_mutex".
     printf("g_thread supported\n");
  }
  else
  {
     printf("g_thread NOT supported\n");
  }

if( (loadingthread = g_thread_create((GThreadFunc)display_loading, (void *)&dictdata, TRUE, &err1)) == NULL)
  {
     printf("Thread create failed: %s!!\n", err1->message );
     g_error_free ( err1 ) ;
  }

g_thread_join(loadingthread);
*/
  gtk_main ();
  return 0;
}

