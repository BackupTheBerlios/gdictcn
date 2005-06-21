/*
 *  Leafpad - GTK+ based simple text editor
 *  Copyright (C) 2004-2005 Tarot Osuji
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gtk-terminal.h"

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

confinfo cf;
MainWin mainwin; 

MainWin *create_main_window(void)
{
	GtkWidget *window;
	GtkWidget *vbox;
 	GtkWidget *menubar;
      	GtkUIManager *ui;
 	GtkWidget *event_box;
 	GtkWidget *vt;
 	GdkPixbuf *icon;
	gchar * fontstr;
	
	cf.fontname="Monospace";
	cf.fontsize="16";
	fontstr=g_strjoin(" ",cf.fontname,cf.fontsize,NULL);
	MainWin *mw = &mainwin; 
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), _("gtk-terminal"));
/*	icon = gdk_pixbuf_new_from_file(
		ICONDIR G_DIR_SEPARATOR_S PACKAGE ".png", NULL);
	gtk_window_set_icon(GTK_WINDOW(window), icon);
	if (icon)
		g_object_unref(icon);
	g_signal_connect(G_OBJECT(window), "delete-event",
		G_CALLBACK(on_file_quit), NULL);
	g_signal_connect_after(G_OBJECT(window), "delete-event",
		G_CALLBACK(gtk_widget_hide_on_delete), NULL);
*/
	
	vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	
	mw->window = window;
	ui = create_ui_manager(mw);
	menubar =gtk_ui_manager_get_widget (ui, "/MenuBar");
	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
	
	event_box = gtk_event_box_new();
	gtk_box_pack_start(GTK_BOX(vbox), event_box, TRUE, TRUE, 0);
	
	vt = vte_terminal_new();
	vte_terminal_set_font_from_string(VTE_TERMINAL(vt),fontstr);
	cf.defcharset=(gchar *)vte_terminal_get_encoding(VTE_TERMINAL(vt));
	g_print("default charset is %s\n",cf.defcharset);
	g_free(fontstr);
	vte_terminal_fork_command(VTE_TERMINAL(vt),
					  "bash", NULL, NULL,
					  "~",
					  TRUE, TRUE, TRUE);
	gtk_container_add(GTK_CONTAINER(event_box), vt);
	mw->menubar = menubar;
	mw->vte_terminal = vt;
	mw->cf = &cf;
	gtk_widget_add_events(event_box,GDK_BUTTON_PRESS_MASK|GDK_BUTTON_RELEASE_MASK);
	gtk_event_box_set_above_child(event_box,TRUE);
  	g_signal_connect_after ((gpointer) window, "destroy",
                          G_CALLBACK (gtk_main_quit),
                          NULL);
	g_signal_connect(G_OBJECT(vt), "eof",
			 G_CALLBACK(destroy_and_quit_eof), mw);
	g_signal_connect(G_OBJECT(vt), "child-exited",
			 G_CALLBACK(destroy_and_quit_exited), mw);
/*
	g_signal_connect(G_OBJECT(event_box), "button_press_event",
			 G_CALLBACK(terminal_popup_menu), mw);
*/	
	return mw;
}
