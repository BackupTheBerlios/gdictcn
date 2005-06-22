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
void change_terminal_color(GtkWidget * button,gpointer user_data); 
void iconify_window(GtkWidget *widget, gpointer data);
void deiconify_window(GtkWidget *widget, gpointer data);
void raise_window(GtkWidget *widget, gpointer data);
void lower_window(GtkWidget *widget, gpointer data);
void maximize_window(GtkWidget *widget, gpointer data);
void restore_window(GtkWidget *widget, gpointer data);
void refresh_window(GtkWidget *widget, gpointer data);
void resize_window(GtkWidget *widget, guint width, guint height, gpointer data);
void move_window(GtkWidget *widget, guint x, guint y, gpointer data);
void adjust_font_size(GtkWidget *widget, gpointer data, gint howmuch);
void increase_font_size(GtkWidget *widget, gpointer data);
void decrease_font_size(GtkWidget *widget, gpointer data);
void char_size_changed(GtkWidget *widget, guint width, guint height, gpointer data);
#endif
