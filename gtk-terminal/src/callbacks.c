#include <gtk/gtk.h>
#include <vte/vte.h>
#include "window.h"
#include "font.h"
void
destroy_and_quit(GtkWidget *widget, gpointer data)
{
	GtkWidget * window;

	window=((MainWin *)data)->window;
	gtk_widget_destroy(GTK_WIDGET(window));
	gtk_main_quit();
}
void
destroy_and_quit_eof(GtkWidget *widget, gpointer data)
{
	g_print("Detected EOF.\n");
	destroy_and_quit(widget, data);
}
void
destroy_and_quit_exited(GtkWidget *widget, gpointer data)
{
	destroy_and_quit(widget, data);
}

static void activate_email(GtkAboutDialog *about,const char * link, gpointer data){
 	g_print("send mail to %s\n",link);
}

static void activate_url(GtkAboutDialog *about,const char * link, gpointer data){
 	g_print("show url %s\n",link);
}

void
cb_about (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	MainWin * mw;
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

 mw=(MainWin * )user_data;
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
  filename = g_build_filename(PIXMAPS_DIR,"gtk-terminal.png",NULL);

  if (filename)
    {
      pixbuf = gdk_pixbuf_new_from_file (filename, NULL);
      g_free (filename);
      transparent = gdk_pixbuf_add_alpha (pixbuf, TRUE, 0xff, 0xff, 0xff);
      g_object_unref (pixbuf);
    }

  gtk_about_dialog_set_email_hook (activate_email, NULL, NULL);
  gtk_about_dialog_set_url_hook (activate_url, NULL, NULL);
  gtk_show_about_dialog (GTK_WINDOW(mw->window),
			 "name", "gtk-terminal",
			 "version", "0.1",
			 "copyright", "2005-2006 Dave Young",
			 "license", license,
			 "website", "http://gtk-terminal.berlios.de",
			 "comments", "a gtk+ terminal",
			 "authors", authors,
			 "documenters", documentors,
			 "logo", transparent,
			 NULL);

  g_object_unref (transparent);
}
void cb_change_font (GtkMenuItem     *menuitem, gpointer         user_data)
{
	MainWin * mw;
	VteTerminal * vt;
	gchar *currentname,* fontname;
	gint columns,rows,owidth,oheight;

	mw=(MainWin *)user_data;
	vt=(VteTerminal *)mw->vte_terminal;	
	
	columns = vt->column_count;
	rows = vt->row_count;
	gtk_window_get_size(GTK_WINDOW(mw->window),&owidth,&oheight);
	owidth -= vt->char_width * columns;
	oheight -= vt->char_height * rows;
	currentname =(gchar *)pango_font_description_to_string(vte_terminal_get_font(VTE_TERMINAL(vt)));
	fontname =get_font_name_by_selector(mw->window,currentname);
	if(fontname){
		vte_terminal_set_font_from_string(VTE_TERMINAL(vt),fontname);
		gtk_window_resize(GTK_WINDOW(mw->window),
				columns * vt->char_width + owidth,
				rows * vt->char_height + oheight);
		g_free(fontname);
	}
	g_free(currentname);
}

void cb_defcharset(GtkMenuItem     *menuitem, gpointer         user_data)
{
	MainWin * mw;
	confinfo * cf;

	mw=(MainWin *)user_data;
	cf=mw->cf;
	cf->charset=cf->defcharset;
	vte_terminal_set_encoding(VTE_TERMINAL(mw->vte_terminal),cf->charset);
}

void cb_gb2312(GtkMenuItem     *menuitem, gpointer         user_data)
{
	MainWin * mw;
	confinfo * cf;

	mw=(MainWin *)user_data;
	cf=mw->cf;
	cf->charset="GB2312";
	vte_terminal_set_encoding(VTE_TERMINAL(mw->vte_terminal),"gb2312");
}
void cb_utf8(GtkMenuItem     *menuitem, gpointer         user_data)
{
	MainWin * mw;
	confinfo * cf;

	mw=(MainWin *)user_data;
	cf=mw->cf;
	cf->charset="utf-8";
	vte_terminal_set_encoding(VTE_TERMINAL(mw->vte_terminal),"utf-8");
}

void cb_cut(GtkWidget      *menuitem, gpointer user_data)
{
 	MainWin * mw;

	mw=(MainWin *)user_data;
	g_signal_emit_by_name(G_OBJECT(mw->vte_terminal),"cut-clipboard");
}

void cb_copy (GtkWidget      *menuitem, gpointer user_data)
{
 	MainWin * mw;

	mw=(MainWin *)user_data;
      vte_terminal_copy_clipboard (VTE_TERMINAL(mw->vte_terminal));
}

void cb_paste (GtkWidget      *menuitem, gpointer user_data)
{
 	MainWin * mw;

	mw=(MainWin *)user_data;
      vte_terminal_paste_clipboard (VTE_TERMINAL(mw->vte_terminal));
}
