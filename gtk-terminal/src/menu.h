/*
 */

#ifndef _MENU_H
#define _MENU_H
#include "window.h"

typedef struct{
	MainWin * mw;
	GtkWidget * color_dialog;
	GtkColorButton *fgb;
	GtkColorButton *bgb;
}colorbuttons;
GtkUIManager *create_ui_manager(MainWin *mw);
void terminal_popup_menu(GtkWidget *widget,GdkEventButton * event, gpointer data);
void create_color_dialog(GtkMenuItem     *menuitem, gpointer         user_data);

#endif /* _MENU_H */
