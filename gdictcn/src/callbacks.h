#ifndef CALLBACKS_H
#define CALLBACKS_H
#include <gtk/gtk.h>


void
on_alwaystop_activate                     (GtkMenuItem     *menuitem,
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

gboolean key_press_event (GtkWidget *text_view,GdkEventKey *event);

gboolean event_after (GtkWidget *text_view,GdkEvent  *ev);

gboolean motion_notify_event (GtkWidget      *text_view, GdkEventMotion *event);
gboolean visibility_notify_event (GtkWidget          *text_view,GdkEventVisibility *event);
		 
#endif
