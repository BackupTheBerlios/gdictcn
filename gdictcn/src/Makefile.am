bin_PROGRAMS = gdictcn 

gdictcn_SOURCES = main.c interface.c callbacks.c dictcn.c parsexml.c 
gdictcn_LDADD = $(DEPS_LIBS) $(CURL_LIBS) $(XML2_LIBS)

noinst_HEADERS= interface.h callbacks.h dictcn.h i18n.h

AM_CPPFLAGS = $(DEPS_CFLAGS) \
		$(CURL_CFLAGS)\
		$(XML2_CFLAGS)\
		-DLOCALE_DIR=\""$(datadir)/locale"\" \
	      -DPIXMAPS_DIR=\""$(datadir)/pixmaps"\" 
                                                                                                    
AM_CFLAGS = -Wall $(DEPS_CFLAGS) \
		$(CURL_CFLAGS)\
		$(XML2_CFLAGS)\
		-DLOCALE_DIR=\""$(datadir)/locale"\" \
	      -DPIXMAPS_DIR=\""$(datadir)/pixmaps"\" 
