/*
 */

#ifndef _WINDOW_H
#define _WINDOW_H

typedef struct {
	gchar * fontname;
	gchar * fontsize;
	gchar * charset;
	gchar * defcharset;
}confinfo;
typedef struct {
	GtkWidget *window;
	GtkWidget *menubar;
	GtkWidget *vte_terminal;
	confinfo * cf;
} MainWin;

MainWin *create_main_window(void);

#endif /* _WINDOW_H */
