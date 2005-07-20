#ifndef INTERFACE_H
#define INTERFACE_H
typedef struct {
	GtkWidget * window;
	GtkWidget * entry;
	GtkWidget * text;
	GtkWidget * statusbar;	
	gboolean always_on_top;
}dict_data;
GtkWidget* create_window1 (void);
#endif
