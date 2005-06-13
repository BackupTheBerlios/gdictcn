/*
 * dict.cn ui  
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "i18n.h"

dict_data dictdata;

GtkWidget*
create_window1 (void)
{
  GtkWidget *window1;
  GtkWidget *vbox1;
  GtkWidget *menubar1;
  GtkWidget *menuitem1;
  GtkWidget *menuitem1_menu;
  GtkWidget *dictcn;
  GtkWidget *separatormenuitem1;
  GtkWidget *quit;
  GtkWidget *menuitem2;
  GtkWidget *menuitem2_menu;
  GtkWidget *about1;
  GtkWidget *hbox1;
  GtkWidget *word;
  GtkWidget *entry1;
  GtkWidget *button1;
  GtkWidget *frame1;
  GtkWidget *scrolledwindow1;
  GtkWidget *text_view;
  GtkWidget *statusbar1;
  GtkAccelGroup *accel_group;

  accel_group = gtk_accel_group_new ();

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window1), _("dict.cn gtk client"));
  gtk_window_set_default_size (GTK_WINDOW (window1), 400, 320);

  vbox1 = gtk_vbox_new (FALSE, 4);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (window1), vbox1);

  menubar1 = gtk_menu_bar_new ();
  gtk_widget_show (menubar1);
  gtk_box_pack_start (GTK_BOX (vbox1), menubar1, FALSE, FALSE, 0);

  menuitem1 = gtk_menu_item_new_with_mnemonic (_("_Dictionary"));
  gtk_widget_show (menuitem1);
  gtk_container_add (GTK_CONTAINER (menubar1), menuitem1);

  menuitem1_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem1), menuitem1_menu);

  dictcn = gtk_check_menu_item_new_with_mnemonic (_("dict.cn"));
  gtk_widget_show (dictcn);
  gtk_container_add (GTK_CONTAINER (menuitem1_menu), dictcn);
  gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (dictcn), TRUE);

  separatormenuitem1 = gtk_separator_menu_item_new ();
  gtk_widget_show (separatormenuitem1);
  gtk_container_add (GTK_CONTAINER (menuitem1_menu), separatormenuitem1);
  gtk_widget_set_sensitive (separatormenuitem1, FALSE);

  quit= gtk_image_menu_item_new_from_stock ("gtk-quit", accel_group);
  gtk_widget_show (quit);
  gtk_container_add (GTK_CONTAINER (menuitem1_menu),quit);

  menuitem2 = gtk_menu_item_new_with_mnemonic (_("_Help"));
  gtk_widget_show (menuitem2);
  gtk_container_add (GTK_CONTAINER (menubar1), menuitem2);

  menuitem2_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem2), menuitem2_menu);

  about1 = gtk_menu_item_new_with_mnemonic (_("_About"));
  gtk_widget_show (about1);
  gtk_container_add (GTK_CONTAINER (menuitem2_menu), about1);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_set_size_request (hbox1, -1, 30);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 5);

  word = gtk_label_new (_("word : "));
  gtk_widget_set_size_request (word, -1, 30);
  gtk_widget_show (word);
  gtk_box_pack_start (GTK_BOX (hbox1), word, FALSE, FALSE, 5);

  entry1 = gtk_entry_new ();
  gtk_widget_set_size_request (entry1, -1, 30);
  gtk_widget_show (entry1);
  gtk_box_pack_start (GTK_BOX (hbox1), entry1, TRUE, TRUE, 5);

  button1 = gtk_button_new_from_stock(GTK_STOCK_FIND);
  gtk_widget_set_size_request (button1, -1, 30);
  gtk_widget_show (button1);
  gtk_box_pack_start (GTK_BOX (hbox1), button1, FALSE, FALSE, 5);

  frame1 = gtk_frame_new (NULL);
  gtk_widget_show (frame1);
  gtk_box_pack_start (GTK_BOX (vbox1), frame1, TRUE, TRUE, 0);
  gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_IN);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow1);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_SHADOW_IN);
  gtk_container_add(GTK_CONTAINER(frame1),scrolledwindow1);

  text_view = gtk_text_view_new ();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view),FALSE);
  gtk_widget_show (text_view);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), text_view);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text_view), GTK_WRAP_WORD);
  gtk_text_view_set_left_margin (GTK_TEXT_VIEW (text_view), 5);
  gtk_text_view_set_right_margin (GTK_TEXT_VIEW (text_view), 5);

  statusbar1 = gtk_statusbar_new ();
  gtk_widget_show (statusbar1);
  gtk_box_pack_start (GTK_BOX (vbox1), statusbar1, FALSE, FALSE, 0);

/*
  pbar = gtk_progress_bar_new();
  progressdata.pbar=pbar;
  progressdata.statusbar=statusbar1;
  gtk_container_add(GTK_CONTAINER(statusbar1),pbar);
  gtk_widget_show(pbar);
  progressdata.timer=g_timeout_add(100,progress_timeout,&progressdata);
*/

  dictdata.entry=entry1;
  dictdata.text=text_view;

  g_signal_connect_after ((gpointer) window1, "destroy",
                          G_CALLBACK (gtk_main_quit),
                          NULL);

  g_signal_connect (GTK_OBJECT(entry1), "activate",
                    G_CALLBACK (find_cb),
                    &dictdata);
  g_signal_connect ((gpointer) dictcn, "activate",
                    G_CALLBACK (on_dictcn_activate),
                    NULL);
  g_signal_connect ((gpointer) quit, "activate",
                    G_CALLBACK (gtk_main_quit),
                    NULL);
  g_signal_connect ((gpointer) about1, "activate",
                    G_CALLBACK (on_about1_activate),
                    NULL);
  g_signal_connect ((gpointer) button1, "clicked",
                    G_CALLBACK (find_cb),
                    &dictdata);
  gtk_window_add_accel_group (GTK_WINDOW (window1), accel_group);

  return window1;
}

