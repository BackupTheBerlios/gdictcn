#include <gtk/gtk.h>
#include "menu.h"
#include "i18n.h"
#include "callbacks.h"

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

static void
activate_action (GtkAction *action)
{
  g_message ("Action \"%s\" activated", gtk_action_get_name (action));
}

static GtkActionEntry entries[] = {
  { "FileMenu", NULL, "File"},               /* name, stock id, label */
  { "EditMenu", NULL, "Edit"  }, 
  { "PreferencesMenu", NULL, "Preferences" },
  { "BookMarksMenu", NULL, "Bookmarks" }, 
  { "HelpMenu", NULL, "Help" },
  { "Quit", GTK_STOCK_QUIT,         /* name, stock id */
    "_Quit", "<control>Q",                     /* label, accelerator */     
    "Quit",                                    /* tooltip */
    G_CALLBACK (destroy_and_quit) },
/*
  { "Cut", NULL,                   
    "_Cut", "<control>X",         
    "Cut",                       
    G_CALLBACK (cb_cut) },
*/
  { "Copy", NULL,               
    "_Copy", "<control>C",     
    "Copy",                   
    G_CALLBACK (cb_copy) },
  { "Paste", NULL,                
    "_Paste", "<control>V",      
    "Paste",                    
    G_CALLBACK (cb_paste) },
  { "Font", NULL,              
    "_Font", NULL,    
    "Font",                  
    G_CALLBACK (cb_change_font) },
  { "Color", NULL,          
    "_Color", NULL,
    "Color",              
    G_CALLBACK (activate_action) },
  { "Terminal", NULL,     
    "_Terminal", NULL,    
    "Terminal",                  
    G_CALLBACK (activate_action) },
  { "Submenu", NULL, "_Charset" },   
  { "Default", NULL,              
    "_Default", NULL,    
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
  { "About", NULL,         
    "_About", "<control>A",
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

