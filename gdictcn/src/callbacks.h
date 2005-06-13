#ifndef CALLBACKS_H
#define CALLBACKS_H
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
#endif
