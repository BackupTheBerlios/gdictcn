#include <gtk/gtk.h>


void
on_dictcn_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_quit_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
find_cb					(GtkButton       *button,
                                        gpointer         user_data);
void
update_statusbar					(GtkButton       *button,
                                        gpointer         user_data);
void
loading_start				(GtkButton       *button,
                                        gpointer         user_data);
void
loading_stop				(GtkButton       *button,
                                        gpointer         user_data);
void display_loading(void *);
