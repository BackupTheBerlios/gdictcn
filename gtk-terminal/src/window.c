/*
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
	GtkWidget *hbox;
	GtkWidget *scrollbar;
 	GtkWidget *menubar;
      	GtkUIManager *ui;
 	GtkWidget *vt;
 	GdkPixbuf *icon;
	gchar * fontstr;
	GdkColor bg={0,0xffff,0xffff,0xffff};
	GdkColor fg={0,0x0000,0x0000,0x0000};
	GdkColor tint={0,0x0000,0x0000,0x0000};
	GdkColor highlight={0,0xc000,0xc000,0xc000};
	GdkColor cursor={0,0xffff,0x8000,0x8000};
	
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
	
	//gtk_container_set_resize_mode(GTK_CONTAINER(window),
	//			      GTK_RESIZE_IMMEDIATE);
	vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	
	mw->window = window;
	ui = create_ui_manager(mw);
	menubar =gtk_ui_manager_get_widget (ui, "/MenuBar");
	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
	
	
	hbox = gtk_hbox_new(0, FALSE);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
	vt = vte_terminal_new();
	gtk_widget_set_double_buffered(vt, TRUE);
	//char_size_changed(vt, 0, 0, window);
	g_signal_connect(G_OBJECT(vt), "char-size-changed",
				 G_CALLBACK(char_size_changed), window);
	vte_terminal_set_font_from_string_full(VTE_TERMINAL(vt),fontstr,FALSE);
	cf.defcharset=(gchar *)vte_terminal_get_encoding(VTE_TERMINAL(vt));
	g_free(fontstr);

	vte_terminal_set_audible_bell(VTE_TERMINAL(vt), TRUE);
	vte_terminal_set_visible_bell(VTE_TERMINAL(vt), FALSE);
	vte_terminal_set_cursor_blinks(VTE_TERMINAL(vt), FALSE);
	vte_terminal_set_scroll_background(VTE_TERMINAL(vt), TRUE);
	vte_terminal_set_scroll_on_output(VTE_TERMINAL(vt), FALSE);
	vte_terminal_set_scroll_on_keystroke(VTE_TERMINAL(vt), TRUE);
	vte_terminal_set_scrollback_lines(VTE_TERMINAL(vt), 80);
	vte_terminal_set_mouse_autohide(VTE_TERMINAL(vt), TRUE);

	vte_terminal_set_background_tint_color(VTE_TERMINAL(vt), &tint);
	vte_terminal_set_colors(VTE_TERMINAL(vt), &fg, &bg, NULL, 0);
	vte_terminal_set_color_highlight(VTE_TERMINAL(vt),&highlight);
						 
	vte_terminal_set_color_cursor(VTE_TERMINAL(vt), &cursor);
	//vte_terminal_set_emulation(VTE_TERMINAL(vt), terminal);

	vte_terminal_fork_command(VTE_TERMINAL(vt),
					  "bash", NULL, NULL,
					  "~",
					  TRUE, TRUE, TRUE);
	gtk_box_pack_start(GTK_BOX(hbox), vt, FALSE, FALSE, 0);

	scrollbar = gtk_vscrollbar_new((VTE_TERMINAL(vt))->adjustment);
	gtk_box_pack_start(GTK_BOX(hbox), scrollbar, FALSE, FALSE, 0);

	mw->menubar = menubar;
	mw->vte_terminal = vt;
	mw->cf = &cf;

	g_signal_connect(G_OBJECT(vt), "iconify-window",
			 G_CALLBACK(iconify_window), window);
	g_signal_connect(G_OBJECT(vt), "deiconify-window",
			 G_CALLBACK(deiconify_window), window);
	g_signal_connect(G_OBJECT(vt), "raise-window",
			 G_CALLBACK(raise_window), window);
	g_signal_connect(G_OBJECT(vt), "lower-window",
			 G_CALLBACK(lower_window), window);
	g_signal_connect(G_OBJECT(vt), "maximize-window",
			 G_CALLBACK(maximize_window), window);
	g_signal_connect(G_OBJECT(vt), "restore-window",
			 G_CALLBACK(restore_window), window);
	g_signal_connect(G_OBJECT(vt), "move-window",
			 G_CALLBACK(move_window), window);
	g_signal_connect(G_OBJECT(vt), "refresh-window",
			 G_CALLBACK(refresh_window), window);
	g_signal_connect(G_OBJECT(vt), "resize-window",
			 G_CALLBACK(resize_window), window);

	g_signal_connect(G_OBJECT(vt), "increase-font-size",
			 G_CALLBACK(increase_font_size), window);
	g_signal_connect(G_OBJECT(vt), "decrease-font-size",
			 G_CALLBACK(decrease_font_size), window);

  	g_signal_connect_after((gpointer) window, "destroy",
                          G_CALLBACK (destroy_and_quit_exited),
                          mw);

	g_signal_connect(G_OBJECT(vt), "eof",
			 G_CALLBACK(destroy_and_quit_eof), mw);
	g_signal_connect(G_OBJECT(vt), "child-exited",
			 G_CALLBACK(destroy_and_quit_exited), mw);

/*
	g_signal_connect(G_OBJECT(vt), "button_press_event",
			 G_CALLBACK(terminal_popup_menu), mw);
*/	
	return mw;
}
