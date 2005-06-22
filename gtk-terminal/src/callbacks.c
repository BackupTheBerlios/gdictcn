#include <gtk/gtk.h>
#include <vte/vte.h>
#include "window.h"
#include "font.h"
#include "menu.h"

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

void change_terminal_color(GtkWidget * button,gpointer user_data)
{
	colorbuttons * colorbs;
	VteTerminal * vt;
	GdkColor fg,bg;
	
	colorbs=(colorbuttons *)user_data;
	vt=(VteTerminal *)(colorbs->mw->vte_terminal);
	gtk_color_button_get_color(colorbs->fgb,&fg);
	gtk_color_button_get_color(colorbs->bgb,&bg);
	vte_terminal_set_color_foreground(vt,&fg);
	vte_terminal_set_color_background(vt,&bg);
}

 
void char_size_changed(GtkWidget *widget, guint width, guint height, gpointer data)
{
	VteTerminal *terminal;
	GtkWindow *window;
	GdkGeometry geometry;
	int xpad, ypad;

	g_return_if_fail(GTK_IS_WINDOW(data));
	g_return_if_fail(VTE_IS_TERMINAL(widget));

	terminal = VTE_TERMINAL(widget);
	window = GTK_WINDOW(data);

	vte_terminal_get_padding(terminal, &xpad, &ypad);

	geometry.width_inc = terminal->char_width;
	geometry.height_inc = terminal->char_height;
	geometry.base_width = xpad;
	geometry.base_height = ypad;
	geometry.min_width = xpad + terminal->char_width * 2;
	geometry.min_height = ypad + terminal->char_height * 2;

	gtk_window_set_geometry_hints(window, widget, &geometry,
				      GDK_HINT_RESIZE_INC |
				      GDK_HINT_BASE_SIZE |
				      GDK_HINT_MIN_SIZE);
}

void status_line_changed(GtkWidget *widget, gpointer data)
{
	g_print("Status = `%s'.\n",
		vte_terminal_get_status_line(VTE_TERMINAL(widget)));
}

 
int button_pressed(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	VteTerminal *terminal;
	char *match;
	int tag;
	gint xpad, ypad;
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
	if (GTK_IS_WIDGET(data)) {
		if ((GTK_WIDGET(data))->window) {
			gdk_window_iconify((GTK_WIDGET(data))->window);
		}
	}
}

 
void deiconify_window(GtkWidget *widget, gpointer data)
{
	if (GTK_IS_WIDGET(data)) {
		if ((GTK_WIDGET(data))->window) {
			gdk_window_deiconify((GTK_WIDGET(data))->window);
		}
	}
}

void raise_window(GtkWidget *widget, gpointer data)
{
	if (GTK_IS_WIDGET(data)) {
		if ((GTK_WIDGET(data))->window) {
			gdk_window_raise((GTK_WIDGET(data))->window);
		}
	}
}

void lower_window(GtkWidget *widget, gpointer data)
{
	if (GTK_IS_WIDGET(data)) {
		if ((GTK_WIDGET(data))->window) {
			gdk_window_lower((GTK_WIDGET(data))->window);
		}
	}
}

void maximize_window(GtkWidget *widget, gpointer data)
{
	if (GTK_IS_WIDGET(data)) {
		if ((GTK_WIDGET(data))->window) {
			gdk_window_maximize((GTK_WIDGET(data))->window);
		}
	}
}

void restore_window(GtkWidget *widget, gpointer data)
{
	if (GTK_IS_WIDGET(data)) {
		if ((GTK_WIDGET(data))->window) {
			gdk_window_unmaximize((GTK_WIDGET(data))->window);
		}
	}
}

void refresh_window(GtkWidget *widget, gpointer data)
{
	GdkRectangle rect;
	if (GTK_IS_WIDGET(data)) {
		if ((GTK_WIDGET(data))->window) {
			rect.x = rect.y = 0;
			rect.width = (GTK_WIDGET(data))->allocation.width;
			rect.height = (GTK_WIDGET(data))->allocation.height;
			gdk_window_invalidate_rect((GTK_WIDGET(data))->window,
						   &rect, TRUE);
		}
	}
}

void resize_window(GtkWidget *widget, guint width, guint height, gpointer data)
{
	VteTerminal *terminal;
	gint owidth, oheight, xpad, ypad;
	if ((GTK_IS_WINDOW(data)) && (width >= 2) && (height >= 2)) {
		terminal = VTE_TERMINAL(widget);
		gtk_window_get_size(GTK_WINDOW(data), &owidth, &oheight);
		owidth -= terminal->char_width * terminal->column_count;
		oheight -= terminal->char_height * terminal->row_count;
		vte_terminal_get_padding(VTE_TERMINAL(widget), &xpad, &ypad);
		owidth -= xpad;
		oheight -= ypad;
		gtk_window_resize(GTK_WINDOW(data),
				  width + owidth, height + oheight);
	}
}

void move_window(GtkWidget *widget, guint x, guint y, gpointer data)
{
	if (GTK_IS_WIDGET(data)) {
		if ((GTK_WIDGET(data))->window) {
			gdk_window_move((GTK_WIDGET(data))->window, x, y);
		}
	}
}

void adjust_font_size(GtkWidget *widget, gpointer data, gint howmuch)
{
	VteTerminal *terminal;
	PangoFontDescription *desired;
	gint newsize;
	gint columns, rows, owidth, oheight;

	terminal = VTE_TERMINAL(widget);
	columns = terminal->column_count;
	rows = terminal->row_count;

	gtk_window_get_size(GTK_WINDOW(data), &owidth, &oheight);
	owidth -= terminal->char_width * terminal->column_count;
	oheight -= terminal->char_height * terminal->row_count;

	desired = pango_font_description_copy(vte_terminal_get_font(terminal));
	newsize = pango_font_description_get_size(desired) / PANGO_SCALE;
	newsize += howmuch;
	pango_font_description_set_size(desired,
					CLAMP(newsize, 4, 144) * PANGO_SCALE);

	vte_terminal_set_font(terminal, desired);
	gtk_window_resize(GTK_WINDOW(data),
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

gboolean read_and_feed(GIOChannel *source, GIOCondition condition, gpointer data)
{
	char buf[2048];
	gsize size;
	GIOStatus status;
	g_return_val_if_fail(VTE_IS_TERMINAL(data), FALSE);
	status = g_io_channel_read_chars(source, buf, sizeof(buf),
					 &size, NULL);
	if ((status == G_IO_STATUS_NORMAL) && (size > 0)) {
		vte_terminal_feed(VTE_TERMINAL(data), buf, size);
		return TRUE;
	}
	return FALSE;
}
