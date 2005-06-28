/*
 * gtk-terminal 
 * Author: Dave Young <lonelypenguin@gmail.com> 
 *
 * This software is released under the GNU General Public License (GNU GPL).
 * Please read the included file COPYING for more information.
 * This software comes with no warranty of any kind, use at your own risk!
 */

#ifdef HAVE_CONFIG_H
#  	include <config.h>
#endif

#include <gtk/gtk.h>
#include <vte/vte.h>
#include "gtk-terminal.h"

void destroy_and_quit(GtkWidget *widget, gpointer data)
{
	GtkWidget * window;

	window=((MainWin *)data)->window;
	gtk_widget_destroy(GTK_WIDGET(window));
	gtk_main_quit();
}

void destroy_and_quit_eof(GtkWidget *widget, gpointer data)
{
	destroy_and_quit(widget, data);
}

void destroy_and_quit_exited(GtkWidget *widget, gpointer data)
{
	destroy_and_quit(widget, data);
}

static void activate_email(GtkAboutDialog *about,const char * link, gpointer data){
 	g_print("send mail to %s\n",link);
}

static void activate_url(GtkAboutDialog *about,const char * link, gpointer data){
 	g_print("show url %s\n",link);
}


void cb_about (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	MainWin * mw;
 	GdkPixbuf *pixbuf, *transparent;
  	gchar *filename;

  	const gchar *authors[] = {
    		"Dave Young <hidave@mail.berlios.de>",
    		NULL
  	};

  	const gchar *documentors[] = {
    		"Dave Young <hidave@mail.berlios.de>",
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
			 "name", _("gtk-terminal"),
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
	confinfo * cf;
	VteTerminal * vt;
	gchar *currentname,* fontname;
	gint columns,rows,owidth,oheight;

	mw=(MainWin *)user_data;
	vt=(VteTerminal *)mw->vte_terminal;	
	cf=mw->cf;
	
	columns = vt->column_count;
	rows = vt->row_count;
	gtk_window_get_size(GTK_WINDOW(mw->window),&owidth,&oheight);
	owidth -= vt->char_width * columns;
	oheight -= vt->char_height * rows;
	currentname =(gchar *)pango_font_description_to_string(vte_terminal_get_font(VTE_TERMINAL(vt)));
	fontname =get_font_name_by_selector(mw->window,currentname);
	if(fontname){
		memset(cf->fontname,0,sizeof(cf->fontname));
		strcpy(cf->fontname,fontname);
		vte_terminal_set_font_from_string_full(VTE_TERMINAL(vt),fontname,TRUE);
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
	memset(cf->charset,0,sizeof(cf->charset));
	strncpy(cf->charset,cf->defcharset,strlen(cf->defcharset));
	vte_terminal_set_encoding(VTE_TERMINAL(mw->vte_terminal),cf->charset);
}

void cb_gb2312(GtkMenuItem     *menuitem, gpointer         user_data)
{
	MainWin * mw;
	confinfo * cf;

	mw=(MainWin *)user_data;
	cf=mw->cf;
	memset(cf->charset,0,sizeof(cf->charset));
	sprintf(cf->charset,"GB2312");
	vte_terminal_set_encoding(VTE_TERMINAL(mw->vte_terminal),"gb2312");
}
void cb_utf8(GtkMenuItem     *menuitem, gpointer         user_data)
{
	MainWin * mw;
	confinfo * cf;

	mw=(MainWin *)user_data;
	cf=mw->cf;
	memset(cf->charset,0,sizeof(cf->charset));
	sprintf(cf->charset,"utf-8");
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

void cb_save(GtkWidget      *menuitem, gpointer user_data)
{
 	MainWin * mw;
	confinfo * cf;

	mw=(MainWin *)user_data;
	cf=mw->cf;
	save_options(cf);
}

void change_terminal_color(GtkWidget * button,gpointer user_data)
{
	colorbuttons * colorbs;
	VteTerminal * vt;
	
	colorbs=(colorbuttons *)user_data;
	vt=(VteTerminal *)(colorbs->mw->vte_terminal);
	gtk_color_button_get_color(colorbs->fgb,&colorbs->fg_color);
	gtk_color_button_get_color(colorbs->bgb,&colorbs->bg_color);
	vte_terminal_set_color_foreground(vt,&colorbs->fg_color);
	vte_terminal_set_color_background(vt,&colorbs->bg_color);
	colorbs->mw->cf->fg_red=colorbs->fg_color.red;
	colorbs->mw->cf->fg_green=colorbs->fg_color.green;
	colorbs->mw->cf->fg_blue=colorbs->fg_color.blue;

	colorbs->mw->cf->bg_red=colorbs->bg_color.red;
	colorbs->mw->cf->bg_green=colorbs->bg_color.green;
	colorbs->mw->cf->bg_blue=colorbs->bg_color.blue;
}

 
void char_size_changed(GtkWidget *widget, guint width, guint height, gpointer data)
{
	VteTerminal *terminal;
	GtkWidget* window;
	GdkGeometry geometry;
	int xpad, ypad;

	terminal = VTE_TERMINAL(((MainWin *)data)->vte_terminal);
	window=((MainWin *)data)->window;
	g_return_if_fail(GTK_IS_WINDOW(window));
	g_return_if_fail(VTE_IS_TERMINAL(terminal));

	vte_terminal_get_padding(terminal, &xpad, &ypad);

	geometry.width_inc = terminal->char_width;
	geometry.height_inc = terminal->char_height;
	geometry.base_width = xpad;
	geometry.base_height = ypad;
	geometry.min_width = xpad + terminal->char_width * 2;
	geometry.min_height = ypad + terminal->char_height * 2;
	gtk_window_set_geometry_hints(GTK_WINDOW(window), window, &geometry,
				      GDK_HINT_RESIZE_INC |
				      GDK_HINT_BASE_SIZE |
				      GDK_HINT_MIN_SIZE);

}

void status_line_changed(GtkWidget *widget, gpointer data)
{
	VteTerminal *terminal;

	terminal = VTE_TERMINAL(((MainWin *)data)->vte_terminal);
	g_print("Status = `%s'.\n",
		vte_terminal_get_status_line(terminal));
}

 
int button_pressed(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	VteTerminal *terminal;
	char *match;
	int tag;
	gint xpad, ypad;

	terminal = VTE_TERMINAL(((MainWin *)data)->vte_terminal);
	switch (event->button) {
	case 3:
		terminal = VTE_TERMINAL(widget);
		vte_terminal_get_padding(terminal, &xpad, &ypad);
		match = vte_terminal_match_check(terminal,
						 (event->x - ypad) /
						 terminal->char_width,
						 (event->y - ypad) /
						 terminal->char_height,
						 &tag);
		if (match != NULL) {
			g_print("Matched `%s' (%d).\n", match, tag);
			g_free(match);
			if (GPOINTER_TO_INT(data) != 0) {
				vte_terminal_match_remove(terminal, tag);
			}
		}
		break;
	case 1:
	case 2:
	default:
		break;
	}
	return FALSE;
}

 
void iconify_window(GtkWidget *widget, gpointer data)
{
	GtkWidget* window;

	window=((MainWin *)data)->window;
	if (GTK_IS_WIDGET(window)) {
		if ((GTK_WIDGET(window))->window) {
			gdk_window_iconify((GTK_WIDGET(window))->window);
		}
	}
}

 
void deiconify_window(GtkWidget *widget, gpointer data)
{
	GtkWidget* window;

	window=((MainWin *)data)->window;
	if (GTK_IS_WIDGET(window)) {
		if ((GTK_WIDGET(window))->window) {
			gdk_window_deiconify((GTK_WIDGET(window))->window);
		}
	}
}

void raise_window(GtkWidget *widget, gpointer data)
{
	GtkWidget* window;

	window=((MainWin *)data)->window;
	if (GTK_IS_WIDGET(window)) {
		if ((GTK_WIDGET(window))->window) {
			gdk_window_raise((GTK_WIDGET(window))->window);
		}
	}
}

void lower_window(GtkWidget *widget, gpointer data)
{
	GtkWidget* window;

	window=((MainWin *)data)->window;
	if (GTK_IS_WIDGET(window)) {
		if ((GTK_WIDGET(window))->window) {
			gdk_window_lower((GTK_WIDGET(window))->window);
		}
	}
}

void maximize_window(GtkWidget *widget, gpointer data)
{
	GtkWidget* window;

	window=((MainWin *)data)->window;
	if (GTK_IS_WIDGET(window)) {
		if ((GTK_WIDGET(window))->window) {
			gdk_window_maximize((GTK_WIDGET(window))->window);
		}
	}
}

void restore_window(GtkWidget *widget, gpointer data)
{
	GtkWidget* window;

	window=((MainWin *)data)->window;
	if (GTK_IS_WIDGET(window)) {
		if ((GTK_WIDGET(window))->window) {
			gdk_window_unmaximize((GTK_WIDGET(window))->window);
		}
	}
}

void refresh_window(GtkWidget *widget, gpointer data)
{
	GtkWidget* window;
	GdkRectangle rect;

	window=((MainWin *)data)->window;
	if (GTK_IS_WIDGET(window)) {
		if ((GTK_WIDGET(window))->window) {
			rect.x = rect.y = 0;
			rect.width = window->allocation.width;
			rect.height = window->allocation.height;
			gdk_window_invalidate_rect((GTK_WIDGET(window))->window,
						   &rect, TRUE);
		}
	}
}

void resize_window(GtkWidget *widget, guint width, guint height, gpointer data)
{
	GtkWidget* window;
	VteTerminal *terminal;
	gint owidth, oheight, xpad, ypad;

	window=((MainWin *)data)->window;
	terminal = VTE_TERMINAL(((MainWin *)data)->vte_terminal);
	if ((GTK_IS_WINDOW(window)) && (width >= 2) && (height >= 2)) {
		gtk_window_get_size(GTK_WINDOW(window), &owidth, &oheight);
		owidth -= terminal->char_width * terminal->column_count;
		oheight -= terminal->char_height * terminal->row_count;
		vte_terminal_get_padding(VTE_TERMINAL(terminal), &xpad, &ypad);
		owidth -= xpad;
		oheight -= ypad;
		gtk_window_resize(GTK_WINDOW(window),
				  width + owidth, height + oheight);
	}
}

void move_window(GtkWidget *widget, guint x, guint y, gpointer data)
{
	GtkWidget* window;

	window=((MainWin *)data)->window;
	if (GTK_IS_WIDGET(window)) {
		if ((GTK_WIDGET(window))->window) {
			gdk_window_move((GTK_WIDGET(window))->window, x, y);
		}
	}
}

void adjust_font_size(GtkWidget *widget, gpointer data, gint howmuch)
{
	VteTerminal *terminal;
	GtkWidget* window;
	PangoFontDescription *desired;
	gint newsize;
	gint columns, rows, owidth, oheight;

	window=((MainWin *)data)->window;
	terminal = VTE_TERMINAL(((MainWin *)data)->vte_terminal);
	columns = terminal->column_count;
	rows = terminal->row_count;

	gtk_window_get_size(GTK_WINDOW(window), &owidth, &oheight);
	owidth -= terminal->char_width * terminal->column_count;
	oheight -= terminal->char_height * terminal->row_count;

	desired = pango_font_description_copy(vte_terminal_get_font(terminal));
	newsize = pango_font_description_get_size(desired) / PANGO_SCALE;
	newsize += howmuch;
	pango_font_description_set_size(desired,
					CLAMP(newsize, 4, 144) * PANGO_SCALE);

	vte_terminal_set_font(terminal, desired);
	gtk_window_resize(GTK_WINDOW(window),
			  columns * terminal->char_width + owidth,
			  rows * terminal->char_height + oheight);

	pango_font_description_free(desired);
}

void increase_font_size(GtkWidget *widget, gpointer data)
{
	adjust_font_size(widget, data, 1);
}

void decrease_font_size(GtkWidget *widget, gpointer data)
{
	adjust_font_size(widget, data, -1);
}

void window_title_changed(GtkWidget *widget, gpointer win)
{
	GtkWindow *window;

	g_return_if_fail(VTE_TERMINAL(widget));
	g_return_if_fail(GTK_IS_WINDOW(win));
	g_return_if_fail(VTE_TERMINAL(widget)->window_title != NULL);
	window = GTK_WINDOW(win);

	gtk_window_set_title(window, VTE_TERMINAL(widget)->window_title);
}

