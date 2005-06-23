#ifdef HAVE_CONFIG_H
  	include <config.h>
#endif

#include <string.h>
#include <gtk/gtk.h>
#include "menu.h"
#include "i18n.h"
#include "callbacks.h"


colorbuttons cbs;

static void activate_action (GtkAction *action)
{
  	g_message ("Action \"%s\" activated", gtk_action_get_name (action));
}

static GtkActionEntry entries[] = {
  	{ "FileMenu", NULL, N_("_File")},               /* name, stock id, label */
  	{ "EditMenu", NULL, N_("_Edit")  }, 
  	{ "PreferencesMenu", NULL, N_("_Preferences") },
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
    	G_CALLBACK (activate_action) },
  	{ "Submenu", NULL, N_("_Charset") },   
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
   	N_("_About"), "<control>A",
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
"    <menu action='PreferencesMenu'>"
"	<menuitem action='Font'/>"
"	<menuitem action='Color'/>"
"	<menuitem action='Terminal'/>"
"		<menu action='Submenu'>"
"			<menuitem action='Default'/>"
"			<menuitem action='GB2312'/>"
"			<menuitem action='utf-8'/>"
"		</menu>"
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
