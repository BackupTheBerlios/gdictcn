/*
 * gtk-terminal 
 * Author: Dave Young <lonelypenguin@gmail.com> 
 *
 * This software is released under the GNU General Public License (GNU GPL).
 * Please read the included file COPYING for more information.
 * This software comes with no warranty of any kind, use at your own risk!
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include "gtk-terminal.h"
#include "util.h"

confinfo cf;
MainWin mainwin; 

MainWin *create_main_window(void)
{
	GtkWidget *window;
	GtkWidget *vbox;
 	GtkWidget *menubar;
      	GtkUIManager *ui;
 	GtkWidget *vt;
 	GdkPixbuf *icon=NULL,*transparent ;
	MainWin *mw = &mainwin; 
	GdkColor bg={0,0xffff,0xffff,0xffff};
	GdkColor fg={0,0x0000,0x0000,0x0000};
	GdkColor highlight={0,0xc000,0xc000,0xc000};
	//GdkColor cursor={0,0xffff,0x8000,0x8000};
	gchar * enc;
	gchar * shell;
	gchar * home_dir;
	gchar * filename;
	
	memset(&cf,0,sizeof(cf));
	sprintf(cf.fontname,"Monospace 14");
	cf.fg_red=0x0000;
	cf.fg_green=0x0000;
	cf.fg_blue=0x0000;

	cf.bg_red=0xffff;
	cf.bg_green=0xffff;
	cf.bg_blue=0xffff;
	cf.allow_bold=TRUE;
	cf.bell=TRUE;
	cf.cursor_blinks=TRUE;
	cf.scroll_output=FALSE;
	cf.scroll_key=TRUE;
	cf.scrollback_lines=80;
	memset(cf.word_chars,0,sizeof(cf.word_chars));
	strcpy(cf.word_chars,"-A-Za-z0-9,/?%&#:_");
	cf.backspace_style=VTE_ERASE_ASCII_DELETE;
	cf.delete_style=VTE_ERASE_DELETE_SEQUENCE;
	
	load_options(&cf);	
	fg.red=cf.fg_red;
	fg.green=cf.fg_green;
	fg.blue=cf.fg_blue;
	bg.red=cf.bg_red;
	bg.green=cf.bg_green;
	bg.blue=cf.bg_blue;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), _("gtk-terminal"));
  	filename = g_build_filename(PIXMAPS_DIR,"gtk-terminal.png",NULL);
  	if (filename)
    	{
      		icon = gdk_pixbuf_new_from_file (filename, NULL);
      		g_free (filename);
      		transparent = gdk_pixbuf_add_alpha (icon, TRUE, 0xff, 0xff, 0xff);
    	}
	gtk_window_set_icon(GTK_WINDOW(window), icon);
	if (icon)
		g_object_unref(icon);
	
	vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	
	mw->window = window;
	ui = create_ui_manager(mw);
	menubar =gtk_ui_manager_get_widget (ui, "/MenuBar");
	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
	
	vt = vte_terminal_new();
	gtk_widget_set_double_buffered(vt, TRUE);
	mw->menubar = menubar;
	mw->vte_terminal = vt;
	mw->cf = &cf;

	enc=(gchar *)vte_terminal_get_encoding(VTE_TERMINAL(vt));
	vte_terminal_set_font_from_string_full(VTE_TERMINAL(vt),cf.fontname,TRUE);
	sprintf(cf.defcharset,enc);
	sprintf(cf.charset,cf.defcharset);
	g_free(enc);

	char_size_changed(vt, 0, 0, mw);
	g_signal_connect(G_OBJECT(vt), "char-size-changed",
				 G_CALLBACK(char_size_changed), mw);
	vte_terminal_set_allow_bold(VTE_TERMINAL(vt), cf.allow_bold);
	vte_terminal_set_audible_bell(VTE_TERMINAL(vt), cf.bell);
	vte_terminal_set_visible_bell(VTE_TERMINAL(vt), FALSE);
	vte_terminal_set_cursor_blinks(VTE_TERMINAL(vt),cf.cursor_blinks);
	vte_terminal_set_scroll_background(VTE_TERMINAL(vt), TRUE);
	vte_terminal_set_scroll_on_output(VTE_TERMINAL(vt), cf.scroll_output);
	vte_terminal_set_scroll_on_keystroke(VTE_TERMINAL(vt), cf.scroll_key);
	vte_terminal_set_scrollback_lines(VTE_TERMINAL(vt), cf.scrollback_lines);
	vte_terminal_set_word_chars(VTE_TERMINAL(vt), cf.word_chars);
	vte_terminal_set_mouse_autohide(VTE_TERMINAL(vt), TRUE);
	vte_terminal_set_backspace_binding(VTE_TERMINAL(vt), cf.backspace_style);
	vte_terminal_set_delete_binding(VTE_TERMINAL(vt), cf.delete_style);

	vte_terminal_set_background_tint_color(VTE_TERMINAL(vt), &bg);
	vte_terminal_set_colors(VTE_TERMINAL(vt), &fg, &bg, NULL, 0);
	vte_terminal_set_color_highlight(VTE_TERMINAL(vt),&highlight);
						 
	//vte_terminal_set_color_cursor(VTE_TERMINAL(vt), &cursor);
	vte_terminal_set_emulation(VTE_TERMINAL(vt), "xterm");

	shell = (gchar *)get_user_shell();
	home_dir = (gchar *)get_home_dir();
	if(!shell)
		vte_terminal_fork_command(VTE_TERMINAL(vt),
					  "bash", NULL, NULL,
					  home_dir,
					  TRUE, TRUE, TRUE);
	else {
		vte_terminal_fork_command(VTE_TERMINAL(vt),
					  shell, NULL, NULL,
					  home_dir,
					  TRUE, TRUE, TRUE);
		g_free(shell);
	}
	g_free(home_dir);
	gtk_box_pack_start(GTK_BOX(vbox), vt, TRUE, TRUE, 0);

	g_signal_connect(G_OBJECT(vt), "window-title-changed",
			 G_CALLBACK(window_title_changed), window);
	g_signal_connect(G_OBJECT(vt), "iconify-window",
			 G_CALLBACK(iconify_window), mw);
	g_signal_connect(G_OBJECT(vt), "deiconify-window",
			 G_CALLBACK(deiconify_window),mw);
	g_signal_connect(G_OBJECT(vt), "raise-window",
			 G_CALLBACK(raise_window), mw);
	g_signal_connect(G_OBJECT(vt), "lower-window",
			 G_CALLBACK(lower_window), mw);
	g_signal_connect(G_OBJECT(vt), "maximize-window",
			 G_CALLBACK(maximize_window), mw);
	g_signal_connect(G_OBJECT(vt), "restore-window",
			 G_CALLBACK(restore_window), mw);
	g_signal_connect(G_OBJECT(vt), "move-window",
			 G_CALLBACK(move_window), mw);
	g_signal_connect(G_OBJECT(vt), "refresh-window",
			 G_CALLBACK(refresh_window), mw);
	g_signal_connect(G_OBJECT(vt), "resize-window",
			 G_CALLBACK(resize_window), mw);
	g_signal_connect(G_OBJECT(vt), "increase-font-size",
			 G_CALLBACK(increase_font_size), mw);
	g_signal_connect(G_OBJECT(vt), "decrease-font-size",
			 G_CALLBACK(decrease_font_size), mw);

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
