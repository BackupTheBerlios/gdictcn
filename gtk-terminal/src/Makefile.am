bin_PROGRAMS = gtk-terminal 

gtk_terminal_SOURCES = main.c window.c callbacks.c menu.c font.c 
gtk_terminal_LDADD = $(DEPS_LIBS)

noinst_HEADERS= gtk-terminal.h window.h callbacks.h menu.h font.h i18n.h

AM_CPPFLAGS = $(DEPS_CFLAGS) \
		-DLOCALE_DIR=\""$(datadir)/locale"\" \
	      -DPIXMAPS_DIR=\""$(datadir)/pixmaps"\" 
                                                                                                    
AM_CFLAGS = -Wall $(DEPS_CFLAGS) \
		-DLOCALE_DIR=\""$(datadir)/locale"\" \
	      -DPIXMAPS_DIR=\""$(datadir)/pixmaps"\" 
