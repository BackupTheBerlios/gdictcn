/*
 * gtk-terminal 
 * Author: Dave Young <lonelypenguin@gmail.com> 
 *
 * This software is released under the GNU General Public License (GNU GPL).
 * Please read the included file COPYING for more information.
 * This software comes with no warranty of any kind, use at your own risk!
 */

#ifndef _MENU_H
#define _MENU_H
#include "window.h"

typedef struct{
	MainWin * mw;
	GtkWidget * color_dialog;
	GtkColorButton *fgb;
	GtkColorButton *bgb;
	GdkColor bg_color;
	GdkColor fg_color;
}colorbuttons;
GtkUIManager *create_ui_manager(MainWin *mw);
void terminal_popup_menu(GtkWidget *widget,GdkEventButton * event, gpointer data);
void create_color_dialog(GtkMenuItem     *menuitem, gpointer         user_data);

#endif /* _MENU_H */
