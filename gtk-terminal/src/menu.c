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

#include <string.h>
#include <gtk/gtk.h>
#include <vte/vte.h>
#include "menu.h"
#include "i18n.h"
#include "callbacks.h"


colorbuttons cbs;
terminal_setting_data tsd;

static void activate_action (GtkAction *action)
{
  	g_message ("Action \"%s\" activated", gtk_action_get_name (action));
}

static GtkActionEntry entries[] = {
  	{ "FileMenu", NULL, N_("_File")},               /* name, stock id, label */
  	{ "EditMenu", NULL, N_("_Edit")  }, 
  	{ "SettingMenu", NULL, N_("_Setting") },
  	{ "BookMarksMenu", NULL, N_("_Bookmarks") }, 
  	{ "HelpMenu", NULL, N_("_Help") },
  	{ "Quit", GTK_STOCK_QUIT,         /* name, stock id */
    	N_("_Quit"), "<control>Q",                     /* label, accelerator */     
    	"Quit",                                    /* tooltip */
    	G_CALLBACK (destroy_and_quit) },
/*
  { "Cut", NULL,                   
    "_Cut", "<control>X",         
    "Cut",                       
    G_CALLBACK (cb_cut) },
*/
  	{ "Copy", GTK_STOCK_COPY,               
    	N_("_Copy"), "<control>C",     
    	"Copy",                   
    	G_CALLBACK (cb_copy) },
  	{ "Paste", GTK_STOCK_PASTE,                
    	N_("_Paste"), "<control>V",      
    	"Paste",                    
    	G_CALLBACK (cb_paste) },
  	{ "Font", GTK_STOCK_SELECT_FONT,              
    	N_("_Font"), NULL,    
    	"Font",                  
    	G_CALLBACK (cb_change_font) },
  	{ "Color", GTK_STOCK_SELECT_COLOR,          
    	N_("_Color"), NULL,
    	"Color",              
    	G_CALLBACK (create_color_dialog) },
  	{ "Terminal", GTK_STOCK_PROPERTIES,     
    	N_("_Terminal"), NULL,    
    	"Terminal",                  
    	G_CALLBACK (create_terminal_dialog) },
  	{ "Save", GTK_STOCK_SAVE,     
    	N_("_Save"), NULL,    
    	"Save",                  
    	G_CALLBACK (cb_save) },
  	{ "Submenu", NULL, N_("_Charset") },   
  	{ "bookmark", NULL,              
    	N_("_Bookmarks"), NULL,    
    	"bookmark",                  
    	G_CALLBACK (activate_action) },
  	{ "Default", NULL,              
    	N_("_Default"), NULL,    
    	"Default",                  
    	G_CALLBACK (cb_defcharset) },
  	{ "GB2312", NULL,              
    	"_GB2312", NULL,    
    	"GB2312",                  
    	G_CALLBACK (cb_gb2312) },
  	{ "utf-8", NULL,            
    	"_utf-8", NULL,  
    	"utf-8",                
    	G_CALLBACK (cb_utf8) },
  	{ "About", GTK_STOCK_ABOUT,         
   	N_("_About"), NULL,
    	"About",              
    	G_CALLBACK (cb_about) },
};

static guint n_entries = G_N_ELEMENTS (entries);

static const gchar *ui_info = 
"<ui>"
"  <menubar name='MenuBar'>"
"    <menu action='FileMenu'>"
"      <menuitem action='Quit'/>"
"    </menu>"
"    <menu action='EditMenu'>"
//"        <menuitem action='Cut'/>"
"        <menuitem action='Copy'/>"
"        <menuitem action='Paste'/>"
"    </menu>"
"    <menu action='SettingMenu'>"
"	<menuitem action='Font'/>"
"	<menuitem action='Color'/>"
"	<menuitem action='Terminal'/>"
"		<menu action='Submenu'>"
"			<menuitem action='Default'/>"
"			<menuitem action='GB2312'/>"
"			<menuitem action='utf-8'/>"
"		</menu>"
"	<menuitem action='Save'/>"
"    </menu>"
"    <menu action='BookMarksMenu'>"
"      <menuitem action='bookmark'/>"
"    </menu>"
"    <menu action='HelpMenu'>"
"      <menuitem action='About'/>"
"    </menu>"
"  </menubar>"
"</ui>";

static GtkActionEntry popentries[] = {
  	{ "PopupMenu", NULL, "_File" },               /* name, stock id, label */
  	{ "Pop_copy", NULL,               
    	"_Copy", "<control>C",     
    	"Copy",                   
    	G_CALLBACK (cb_copy) },
  	{ "Pop_paste", NULL,                
    	"_Paste", "<control>V",      
    	"Paste",                    
    	G_CALLBACK (cb_paste) },
};
static guint n_popentries = G_N_ELEMENTS (popentries);
static const gchar *popui = 
"<ui>"
"<popup name='PopupMenu'>"
"<menuitem action = 'Pop_copy'/>"
"<menuitem action = 'Pop_paste'/>"
"</popup>"
"</ui>";

GtkUIManager * create_ui_manager(MainWin * mw)
{
      	GtkUIManager *ui;
      	GtkActionGroup *actions;
      	GError *error = NULL;

     
      	actions = gtk_action_group_new ("Actions");
	gtk_action_group_set_translation_domain (actions, GETTEXT_PACKAGE);
      	gtk_action_group_add_actions (actions, entries, n_entries, mw);
      	ui = gtk_ui_manager_new ();
      	gtk_ui_manager_insert_action_group (ui, actions, 0);
      	gtk_window_add_accel_group (GTK_WINDOW (mw->window), 
				  gtk_ui_manager_get_accel_group (ui));
      
      	if (!gtk_ui_manager_add_ui_from_string (ui, ui_info, -1, &error))
	{
	  	g_message ("building menus failed: %s", error->message);
	  	g_error_free (error);
	}

  	return ui; 
}

void terminal_popup_menu(GtkWidget *widget,GdkEventButton * event, gpointer data)
{
	MainWin * mw;
      	GtkUIManager *ui;
      	GtkActionGroup *actions;
      	GError *error = NULL;
	GtkWidget * menu;

	mw=(MainWin *)data;     
	if(event->type !=GDK_BUTTON_PRESS || event->button != 3){
		g_print("left button\n");
		return;	
	}
      	actions = gtk_action_group_new ("PopActions");
      	gtk_action_group_add_actions (actions, popentries, n_popentries, mw);

      	ui = gtk_ui_manager_new ();
      	gtk_ui_manager_insert_action_group (ui, actions, 0);
      	gtk_window_add_accel_group (GTK_WINDOW (mw->window), 
				  gtk_ui_manager_get_accel_group (ui));
      
      	if (!gtk_ui_manager_add_ui_from_string (ui, popui, -1, &error))
	{
	  	g_message ("building menus failed: %s", error->message);
	  	g_error_free (error);
	}


	menu  = gtk_ui_manager_get_widget (ui, "/ui/PopupMenu");
	g_signal_connect (menu, "deactivate", G_CALLBACK (gtk_widget_destroy),NULL);
	gtk_menu_popup (GTK_MENU(menu), NULL, NULL, NULL, NULL, 3, 
		gtk_get_current_event_time ());
}

void create_color_dialog(GtkMenuItem     *menuitem, gpointer         user_data)
{
	GtkWidget * cw;
	GtkWidget * top_win;
	GtkWidget * vbox;
	GtkWidget * hbox1;
	GtkWidget * label_fg;
	GtkWidget * picker_fg;
	GtkWidget * hbox2;
	GtkWidget * label_bg;
	GtkWidget * picker_bg;
	GtkWidget * button_ok;	
	GdkColor fg={0,0x0000,0x0000,0x0000};
	GdkColor bg={0,0xffff,0xffff,0xffff};

	if(!cbs.mw){
		cbs.mw=(MainWin *)user_data;
		fg.red = cbs.mw->cf->fg_red;
		fg.green= cbs.mw->cf->fg_green;
		fg.blue= cbs.mw->cf->fg_blue;
		bg.red = cbs.mw->cf->bg_red;
		bg.green= cbs.mw->cf->bg_green;
		bg.blue= cbs.mw->cf->bg_blue;
		memcpy(&cbs.fg_color,&fg,sizeof(fg));
		memcpy(&cbs.bg_color,&bg,sizeof(bg));
	}
	top_win = cbs.mw->window;
      	cw = gtk_window_new (GTK_WINDOW_TOPLEVEL);
			     
      	gtk_window_set_title (GTK_WINDOW (cw), _("select color"));
	gtk_window_set_resizable(GTK_WINDOW(cw),FALSE);
	gtk_window_set_transient_for(GTK_WINDOW(cw),GTK_WINDOW(top_win));
	gtk_window_set_destroy_with_parent(GTK_WINDOW(cw),TRUE);
      	gtk_container_set_border_width (GTK_CONTAINER (cw), 0);

      	vbox = gtk_vbox_new (FALSE, 5);
      	gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
      	gtk_container_add (GTK_CONTAINER (cw), vbox);

      	hbox1 = gtk_hbox_new (FALSE, 8);
      	gtk_container_set_border_width (GTK_CONTAINER (hbox1), 8);
      	gtk_box_pack_start(GTK_BOX(vbox), hbox1,FALSE,FALSE,5);
      
      	label_fg = gtk_label_new (_("foreground"));
  	gtk_widget_set_size_request (label_fg, 120,20);
      	gtk_box_pack_start(GTK_BOX(hbox1), label_fg,FALSE,FALSE,5);

      	picker_fg = gtk_color_button_new_with_color (&cbs.fg_color);
      	gtk_color_button_set_use_alpha (GTK_COLOR_BUTTON (picker_fg), TRUE);
      	gtk_box_pack_start(GTK_BOX(hbox1), picker_fg,FALSE,FALSE,5);

      	hbox2 = gtk_hbox_new (FALSE, 8);
      	gtk_container_set_border_width (GTK_CONTAINER (hbox2), 8);
      	gtk_box_pack_start(GTK_BOX(vbox), hbox2,FALSE,FALSE,5);
      
      	label_bg = gtk_label_new (_("background"));
  	gtk_widget_set_size_request (label_bg, 120,20);
      	gtk_box_pack_start(GTK_BOX(hbox2), label_bg,FALSE,FALSE,5);

      	picker_bg = gtk_color_button_new_with_color (&cbs.bg_color);
      	gtk_color_button_set_use_alpha (GTK_COLOR_BUTTON (picker_bg), TRUE);
      	gtk_box_pack_start(GTK_BOX(hbox2), picker_bg,FALSE,FALSE,5);

	button_ok=gtk_button_new_from_stock(GTK_STOCK_OK);
      	gtk_box_pack_start(GTK_BOX(vbox), button_ok,FALSE,FALSE,5);
	
	cbs.fgb=(GtkColorButton *)picker_fg;
	cbs.bgb=(GtkColorButton *)picker_bg;
	cbs.color_dialog=cw;

	gtk_widget_show_all(GTK_WIDGET(cw));

	g_signal_connect(G_OBJECT(cw), "delete-event",
		G_CALLBACK(gtk_widget_destroy), NULL);
      	g_signal_connect(button_ok, "clicked",
			G_CALLBACK (change_terminal_color),
                        &cbs);

      	g_signal_connect_swapped(button_ok, "clicked",
			G_CALLBACK (gtk_widget_destroy),
                        cw);
}

static gint get_combo_index(VteTerminalEraseBinding bind)
{
	switch(bind){
		case VTE_ERASE_ASCII_DELETE:
			return 0;
		case VTE_ERASE_DELETE_SEQUENCE:
			return 1;
		case VTE_ERASE_ASCII_BACKSPACE:
			return 2;
		default:
			return 0;
	}
	return 0;
}

void create_terminal_dialog(GtkMenuItem     *menuitem, gpointer         user_data)
{
	MainWin *mw=(MainWin *)user_data;
	GtkWidget *dialog;	
	GtkWidget * vbox;
	GtkWidget *hbox1,*hbox2,*hbox3,*hbox4;	
	GtkWidget *cb_bold,*cb_blinks,*cb_bell,*cb_scroll_output,*cb_scroll_key;
	GtkWidget *label_scroll,*spin_lines,*label_lines;//,*spin_kilo;
	GtkAdjustment *lines_adj;//,*kilo_adj;
	GtkWidget *label_words,*entry_words;
  	GtkListStore *store;
  	GtkTreeIter iter;
  	GtkCellRenderer *cell = gtk_cell_renderer_text_new ();
	GtkWidget *label_backspace,*combo_backspace;
	GtkWidget *label_delete,*combo_delete;
	GtkWidget *button_ok;	

	dialog = gtk_dialog_new();
	vbox = GTK_DIALOG(dialog)->vbox;

	cb_bold = gtk_check_button_new_with_label(_("Allow bold text"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_bold),mw->cf->allow_bold);
	gtk_box_pack_start(GTK_BOX(vbox),cb_bold,FALSE,FALSE,5);

	cb_blinks = gtk_check_button_new_with_label(_("Cursor blinks"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_blinks),mw->cf->cursor_blinks);
	gtk_box_pack_start(GTK_BOX(vbox),cb_blinks,FALSE,FALSE,5);

	cb_bell = gtk_check_button_new_with_label(_("Terminal bell"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_bell),mw->cf->bell);
	gtk_box_pack_start(GTK_BOX(vbox),cb_bell,FALSE,FALSE,5);

	cb_scroll_output = gtk_check_button_new_with_label(_("Scroll on output"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_scroll_output),mw->cf->scroll_output);
	gtk_box_pack_start(GTK_BOX(vbox),cb_scroll_output,FALSE,FALSE,5);

	cb_scroll_key= gtk_check_button_new_with_label(_("Scroll on keystroke"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_scroll_key),mw->cf->scroll_key);
	gtk_box_pack_start(GTK_BOX(vbox),cb_scroll_key,FALSE,FALSE,5);

	hbox1= gtk_hbox_new(FALSE,5);
	label_scroll = gtk_label_new(_("scrollback"));

   	lines_adj= (GtkAdjustment *) gtk_adjustment_new (mw->cf->scrollback_lines, 1, 100000, 1, 5,0);
   	//kilo_adj= (GtkAdjustment *) gtk_adjustment_new (1, 1, 100000, 1, 5,0);
   
	spin_lines = gtk_spin_button_new (lines_adj, 1.0, 0);
	label_lines = gtk_label_new(_("lines"));
	//spin_kilo = gtk_spin_button_new (kilo_adj, 1.0, 0);
	gtk_box_pack_start(GTK_BOX(hbox1),label_scroll,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(hbox1),spin_lines,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(hbox1),label_lines,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(vbox),hbox1,FALSE,FALSE,5);


	hbox2= gtk_hbox_new(FALSE,5);
	label_words = gtk_label_new(_("select-by-word characters"));
	entry_words = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry_words),mw->cf->word_chars);
	gtk_box_pack_start(GTK_BOX(hbox2),label_words,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(hbox2),entry_words,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(vbox),hbox2,FALSE,FALSE,5);

 	store = gtk_list_store_new (2,G_TYPE_STRING,G_TYPE_INT);
        gtk_list_store_append (store, &iter);
        gtk_list_store_set (store, &iter,
                            	0,_("ASCII DEL"),
				1,VTE_ERASE_ASCII_DELETE,
                            	-1);
        gtk_list_store_append (store, &iter);
        gtk_list_store_set (store, &iter,
                            	0,_("Escape sequence"),
				1,VTE_ERASE_DELETE_SEQUENCE,
                            	-1);
        gtk_list_store_append (store, &iter);
        gtk_list_store_set (store, &iter,
                            	0,_("Control-H"),
				1,VTE_ERASE_ASCII_BACKSPACE,
                            	-1);

	hbox3= gtk_hbox_new(FALSE,5);
	label_backspace = gtk_label_new(_("backspace"));
	combo_backspace = gtk_combo_box_new_with_model (GTK_TREE_MODEL(store));
  	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT(combo_backspace),cell,FALSE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_backspace),cell,"text",0,NULL);
        gtk_combo_box_set_active (GTK_COMBO_BOX (combo_backspace), get_combo_index(mw->cf->backspace_style));

	gtk_box_pack_start(GTK_BOX(hbox3),label_backspace,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(hbox3),combo_backspace,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(vbox),hbox3,FALSE,FALSE,5);

	hbox4= gtk_hbox_new(FALSE,5);
	label_delete= gtk_label_new(_("delete"));
	combo_delete = gtk_combo_box_new_with_model (GTK_TREE_MODEL(store));
  	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT(combo_delete),cell,FALSE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_delete),cell,"text",0,NULL);
        gtk_combo_box_set_active (GTK_COMBO_BOX (combo_delete), get_combo_index(mw->cf->delete_style));
        g_object_unref (store);
	gtk_box_pack_start(GTK_BOX(hbox4),label_delete,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(hbox4),combo_delete,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(vbox),hbox4,FALSE,FALSE,5);

	button_ok = gtk_dialog_add_button(GTK_DIALOG(dialog),GTK_STOCK_OK,GTK_RESPONSE_ACCEPT);
	gtk_widget_show_all(GTK_WIDGET(dialog));
}
