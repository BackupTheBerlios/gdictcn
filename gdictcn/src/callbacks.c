#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdlib.h>
#include <gtk/gtk.h>
#include <iconv.h>

#include "callbacks.h"
#include "interface.h"
#include "dictcn.h"


void
on_dictcn_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_quit_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}

static void activate_email(GtkAboutDialog *about,const char * link, gpointer data){
 	g_print("send mail to %s\n",link);
}

static void activate_url(GtkAboutDialog *about,const char * link, gpointer data){
 	g_print("show url %s\n",link);
}

void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
 GdkPixbuf *pixbuf, *transparent;
  gchar *filename;

  const gchar *authors[] = {
    "Dave Young",
    NULL
  };

  const gchar *documentors[] = {
    "Dave Young",
    NULL
  };

  const gchar *license =
    "This program is free software; you can redistribute it and/or\n"
    "modify it under the terms of the GNU Library General Public License as\n"
    "published by the Free Software Foundation; either version 2 of the\n"
    "License, or (at your option) any later version.\n"
    "\n"
    "This program is distributed in the hope that it will be useful,\n"
    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n"
    "Library General Public License for more details.\n"
    "\n"
    "You should have received a copy of the GNU Library General Public\n"
    "License along with the Gnome Library; see the file COPYING.LIB.  If not,\n"
    "write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,\n"
    "Boston, MA 02111-1307, USA.\n";

  pixbuf = NULL;
  transparent = NULL;
  filename = g_build_filename(PIXMAPS_DIR,"dictcn_logo.png",NULL);

  if (filename)
    {
      pixbuf = gdk_pixbuf_new_from_file (filename, NULL);
      g_free (filename);
      transparent = gdk_pixbuf_add_alpha (pixbuf, TRUE, 0xff, 0xff, 0xff);
      g_object_unref (pixbuf);
    }

  gtk_about_dialog_set_email_hook (activate_email, NULL, NULL);
  gtk_about_dialog_set_url_hook (activate_url, NULL, NULL);
  gtk_show_about_dialog (NULL,//GTK_WINDOW (window),
			 "name", "gdictcn",
			 "version", "0.1",
			 "copyright", "2005-2006 Dave Young",
			 "license", license,
			 "website", "http://gdictcn.berlios.de",
			 "comments", "chinese/english dictionary online",
			 "authors", authors,
			 "documenters", documentors,
			 "logo", transparent,
			 NULL);

  g_object_unref (transparent);
}

void
find_cb (GtkButton       *button,gpointer user_data)
{
	dict_data * ddata;
	GtkTextBuffer * text_buffer;
	gchar * result;
	gchar * findstr;

	ddata = (dict_data *)user_data;
	gtk_statusbar_push(GTK_STATUSBAR(ddata->statusbar),0,"searching dict.cn ...");
	gdk_window_process_all_updates();

	text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ddata->text));
	findstr=(gchar *)gtk_entry_get_text(GTK_ENTRY(ddata->entry));	
	result=(gchar *)dict_find((char *)findstr);
	gtk_text_buffer_set_text(text_buffer,result,-1);
	if(result)
		free(result);
	gtk_statusbar_pop(GTK_STATUSBAR(ddata->statusbar),0);
}
