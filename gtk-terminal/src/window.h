/*
 * gtk-terminal 
 * Author: Dave Young <lonelypenguin@gmail.com> 
 *
 * This software is released under the GNU General Public License (GNU GPL).
 * Please read the included file COPYING for more information.
 * This software comes with no warranty of any kind, use at your own risk!
 */

#ifndef _WINDOW_H
#define _WINDOW_H

typedef struct {
	gchar fontname[256];
	gchar charset[256];
	gchar defcharset[256];
	gushort fg_red;
	gushort fg_green;
	gushort fg_blue;
	gushort bg_red;
	gushort bg_green;
	gushort bg_blue;
}confinfo;
typedef struct {
	GtkWidget *window;
	GtkWidget *menubar;
	GtkWidget *vte_terminal;
	confinfo * cf;
} MainWin;

MainWin *create_main_window(void);

#endif /* _WINDOW_H */
