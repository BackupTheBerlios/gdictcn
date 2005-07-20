#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <iconv.h>
#include <string.h>
#include <iconv.h>

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

#include "callbacks.h"
#include "interface.h"
#include "dictcn.h"

static gchar * entrytext=NULL;
static gint linkdata=1;
static gboolean hovering_over_link = FALSE;
static GdkCursor *hand_cursor = NULL;
static GdkCursor *regular_cursor = NULL;

static gchar * html_browsers[] =
{
	"firefox",
	"mozilla",
	"konqueror",
	"netscape",
	NULL
};

static gboolean file_in_path(const gchar *name)
{
	gchar *path;
	gchar *namel;
	gint p, l;
	gint ret = FALSE;
	GError *error =NULL;

	if (!name) return FALSE;
	path = g_strdup(getenv("PATH"));
	if (!path) return FALSE;
	namel = g_filename_from_utf8(name,-1,NULL,NULL,&error);
	if(error)
		return FALSE;

	p = 0;
	l = strlen(path);
	while (p < l && !ret)
		{
		gchar *f;
		gint e = p;
		while (path[e] != ':' && path[e] != '\0') e++;
		path[e] = '\0';
		e++;
		f = g_strconcat(path + p, "/", namel, NULL);
		if(g_file_test(f,G_FILE_TEST_EXISTS|G_FILE_TEST_IS_EXECUTABLE))
			ret = TRUE;
		g_free(f);
		p = e;
		}
	g_free(namel);
	g_free(path);

	return ret;
}

gchar * get_browser(void)
{
	gint i=0;
	while(html_browsers[i]){
		if(file_in_path(html_browsers[i]))
			return html_browsers[i];
		i++;
	}
	return NULL;
}

static void run_browser(const gchar * link)
{
	gchar * browser;	
	gchar * result;	

	browser = get_browser();
	if(!browser || !link)
		return;
	result = g_strdup_printf("%s %s &",browser,link);
	system(result);
	g_free(result);	
}

/* Links can be activated by pressing Enter.
 */
gboolean
key_press_event (GtkWidget *text_view, 
		 GdkEventKey *event)
{
  GtkTextIter iter;
  GtkTextBuffer *buffer;
  gchar * findstr;
  char dicturl[256]={0,};
  char strdest[200]={0,};
  char * esc;
  iconv_t it;

  findstr=entrytext;
	if(!findstr)
		return FALSE;
  if(strlen(findstr) >150)
	*(findstr+150)=0;

  it=iconv_open("gb2312","utf-8");
  u2g(findstr,strlen(findstr),strdest,150);
  esc=curl_escape(strdest,strlen(strdest));
  sprintf(dicturl,"%s%s",DICT_URL_WEB,esc);
  iconv_close(it);

  switch (event->keyval)
    {
      case GDK_Return: 
      case GDK_KP_Enter:
        buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));
        gtk_text_buffer_get_iter_at_mark (buffer, &iter, 
                                          gtk_text_buffer_get_insert (buffer));
        run_browser(dicturl);
        break;

      default:
        break;
    }

  return FALSE;
}

/* Links can also be activated by clicking.
 */
gboolean event_after (GtkWidget *text_view,GdkEvent  *ev)
{
  GtkTextIter start, end, iter;
  GtkTextBuffer *buffer;
  GdkEventButton *event;
  gint x, y;
	gchar * findstr;
  char dicturl[256]={0,};
  char strdest[200]={0,};
  char * esc;
  iconv_t it;

  findstr=entrytext;
	if(!findstr)
		return FALSE;
  if(strlen(findstr) >150)
	*(findstr+150)=0;

  it=iconv_open("gb2312","utf-8");
  u2g(findstr,strlen(findstr),strdest,150);
  esc=curl_escape(strdest,strlen(strdest));
  sprintf(dicturl,"%s%s",DICT_URL_WEB,esc);
  iconv_close(it);

  if (ev->type != GDK_BUTTON_RELEASE)
    return FALSE;

  event = (GdkEventButton *)ev;

  if (event->button != 1)
    return FALSE;

  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));

  /* we shouldn't follow a link if the user has selected something */
  gtk_text_buffer_get_selection_bounds (buffer, &start, &end);
  if (gtk_text_iter_get_offset (&start) != gtk_text_iter_get_offset (&end))
    return FALSE;

  gtk_text_view_window_to_buffer_coords (GTK_TEXT_VIEW (text_view), 
                                         GTK_TEXT_WINDOW_WIDGET,
                                         event->x, event->y, &x, &y);

  gtk_text_view_get_iter_at_location (GTK_TEXT_VIEW (text_view), &iter, x, y);

  run_browser(dicturl);

  return FALSE;
}


/* Looks at all tags covering the position (x, y) in the text view, 
 * and if one of them is a link, change the cursor to the "hands" cursor
 * typically used by web browsers.
 */
static void
set_cursor_if_appropriate (GtkTextView    *text_view,
                           gint            x,
                           gint            y)
{
  GSList *tags = NULL, *tagp = NULL;
  GtkTextBuffer *buffer;
  GtkTextIter iter;
  gboolean hovering = FALSE;

  buffer = gtk_text_view_get_buffer (text_view);

  gtk_text_view_get_iter_at_location (text_view, &iter, x, y);
  
  tags = gtk_text_iter_get_tags (&iter);
  for (tagp = tags;  tagp != NULL;  tagp = tagp->next)
    {
      GtkTextTag *tag = tagp->data;
      gint link = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (tag), "linkdata"));

      if (link != 0) 
        {
          hovering = TRUE;
          break;
        }
    }

  if (hovering != hovering_over_link)
    {
      hovering_over_link = hovering;
      if (hovering_over_link)
        gdk_window_set_cursor (gtk_text_view_get_window (text_view, GTK_TEXT_WINDOW_TEXT), hand_cursor);
      else
        gdk_window_set_cursor (gtk_text_view_get_window (text_view, GTK_TEXT_WINDOW_TEXT), regular_cursor);
    }

  if (tags) 
    g_slist_free (tags);
}

/* Update the cursor image if the pointer moved. 
 */

gboolean motion_notify_event (GtkWidget      *text_view, GdkEventMotion *event)
{
  gint x, y;

  gtk_text_view_window_to_buffer_coords (GTK_TEXT_VIEW (text_view), 
                                         GTK_TEXT_WINDOW_WIDGET,
                                         event->x, event->y, &x, &y);

  set_cursor_if_appropriate (GTK_TEXT_VIEW (text_view), x, y);

  gdk_window_get_pointer (text_view->window, NULL, NULL, NULL);
  return FALSE;
}

/* Also update the cursor image if the window becomes visible
 * (e.g. when a window covering it got iconified).
 */

gboolean visibility_notify_event (GtkWidget          *text_view,GdkEventVisibility *event)
{
  gint wx, wy, bx, by;
  
  gdk_window_get_pointer (text_view->window, &wx, &wy, NULL);
  
  gtk_text_view_window_to_buffer_coords (GTK_TEXT_VIEW (text_view), 
                                         GTK_TEXT_WINDOW_WIDGET,
                                         wx, wy, &bx, &by);

  set_cursor_if_appropriate (GTK_TEXT_VIEW (text_view), bx, by);

  return FALSE;
}

void
on_alwaystop_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	dict_data * dt;
	GtkWidget * window;
	gboolean top;

	dt=(dict_data *)user_data;
	window = dt->window;
	top = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menuitem));
	dt->always_on_top = top;

	if(top)
		gtk_window_set_keep_above(GTK_WINDOW(window),TRUE);	
}


void
on_quit_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}

static void activate_email(GtkAboutDialog *about,const char * link, gpointer data){
 	g_print("send mail to %s\n",link);
}

static void activate_url(GtkAboutDialog *about,const char * link, gpointer data){
	run_browser(link);
 	g_print("show url %s\n",link);
}

void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
 GdkPixbuf *pixbuf, *transparent;
  gchar *filename;

  const gchar *authors[] = {
    "Dave Young",
    NULL
  };

  const gchar *documentors[] = {
    "Dave Young",
    NULL
  };

  const gchar *license =
    "This program is free software; you can redistribute it and/or\n"
    "modify it under the terms of the GNU Library General Public License as\n"
    "published by the Free Software Foundation; either version 2 of the\n"
    "License, or (at your option) any later version.\n"
    "\n"
    "This program is distributed in the hope that it will be useful,\n"
    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n"
    "Library General Public License for more details.\n"
    "\n"
    "You should have received a copy of the GNU Library General Public\n"
    "License along with the Gnome Library; see the file COPYING.LIB.  If not,\n"
    "write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,\n"
    "Boston, MA 02111-1307, USA.\n";

  pixbuf = NULL;
  transparent = NULL;
  filename = g_build_filename(PIXMAPS_DIR,"dictcn_logo.png",NULL);

  if (filename)
    {
      pixbuf = gdk_pixbuf_new_from_file (filename, NULL);
      g_free (filename);
      transparent = gdk_pixbuf_add_alpha (pixbuf, TRUE, 0xff, 0xff, 0xff);
      g_object_unref (pixbuf);
    }

  gtk_about_dialog_set_email_hook (activate_email, NULL, NULL);
  gtk_about_dialog_set_url_hook (activate_url, NULL, NULL);
  gtk_show_about_dialog (NULL,//GTK_WINDOW (window),
			 "name", "gdictcn",
			 "version", "0.1",
			 "copyright", "2005-2006 Dave Young",
			 "license", license,
			 "website", "http://gdictcn.berlios.de",
			 "comments", "chinese/english dictionary online",
			 "authors", authors,
			 "documenters", documentors,
			 "logo", transparent,
			 NULL);

  g_object_unref (transparent);
}

void
find_cb (GtkButton       *button,gpointer user_data)
{
	dict_data * ddata;
	GtkTextBuffer * text_buffer;
	gchar * result;
	gchar * findstr;
	gchar * extstr="-------------------------------\n search in ";
	gchar * extstr1="http://dict.cn";
	GtkTextTag *tag;
	GtkTextIter iter;

	ddata = (dict_data *)user_data;
	hand_cursor = gdk_cursor_new (GDK_HAND2);
      	regular_cursor = gdk_cursor_new (GDK_XTERM);
	gtk_statusbar_push(GTK_STATUSBAR(ddata->statusbar),0,"searching dict.cn ...");
	gdk_window_process_all_updates();

	text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ddata->text));
	findstr=(gchar *)gtk_entry_get_text(GTK_ENTRY(ddata->entry));	
	if(entrytext)
		g_free(entrytext);
	entrytext=findstr;
	result=(gchar *)dict_find((char *)findstr);
	if(!findstr || !result)
		gtk_text_buffer_set_text(text_buffer," ",-1);
	else
		gtk_text_buffer_set_text(text_buffer,result,-1);
  	gtk_text_buffer_insert_at_cursor(text_buffer, extstr, strlen(extstr));
  
	gtk_text_buffer_get_end_iter(text_buffer,&iter);
  	tag = gtk_text_buffer_create_tag (text_buffer, NULL, 
				    "foreground", "blue", 
				    "underline", PANGO_UNDERLINE_SINGLE, 
				    NULL);
	g_object_set_data(G_OBJECT(tag),"linkdata",GINT_TO_POINTER(linkdata));
  	gtk_text_buffer_insert_with_tags (text_buffer, &iter, extstr1, -1, tag, NULL);
	if(result)
		free(result);
	gtk_statusbar_pop(GTK_STATUSBAR(ddata->statusbar),0);
}
