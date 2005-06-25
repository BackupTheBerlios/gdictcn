/*
 * gtk-terminal 
 * Author: Dave Young <lonelypenguin@gmail.com> 
 *
 * This software is released under the GNU General Public License (GNU GPL).
 * Please read the included file COPYING for more information.
 * This software comes with no warranty of any kind, use at your own risk!
 */

#include <gtk/gtk.h>

gchar *get_font_name_by_selector(GtkWidget *window, gchar *current_fontname)
{
	GtkWidget *dialog;
	gchar *fontname;
	
	dialog = gtk_font_selection_dialog_new("Font");
	gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));
	gtk_font_selection_dialog_set_font_name(GTK_FONT_SELECTION_DIALOG(dialog), current_fontname);
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK)
		fontname = gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(dialog));
	else
		fontname = NULL;
	gtk_widget_destroy(dialog);
		
	return fontname;
}

