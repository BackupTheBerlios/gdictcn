#ifndef CALLBACKS_H
#define CALLBACKS_H
void destroy_and_quit(GtkWidget *widget, gpointer data);
void destroy_and_quit_eof(GtkWidget *widget, gpointer data);
void destroy_and_quit_exited(GtkWidget *widget, gpointer data);
void cb_about (GtkMenuItem     *menuitem, gpointer         user_data);
void cb_change_font (GtkMenuItem     *menuitem, gpointer         user_data);
void cb_defcharset(GtkMenuItem     *menuitem, gpointer         user_data);
void cb_gb2312(GtkMenuItem     *menuitem, gpointer         user_data);
void cb_utf8(GtkMenuItem     *menuitem, gpointer         user_data);
void cb_cut(GtkWidget      *menuitem, gpointer user_data);
void cb_copy(GtkWidget      *menuitem, gpointer user_data);
void cb_paste (GtkWidget      *menuitem, gpointer user_data);
#endif
