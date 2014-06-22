## Process this file with automake to produce Makefile.in

## Created by Anjuta

AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(IRTSP_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bindir = $(prefix)/irtsp
bin_PROGRAMS = irtsp

irtsp_SOURCES = \
	live/H264LiveStreamSource.cpp \
	live/H264LiveStreamServerMediaSubsession.cpp \
	main.c

irtsp_LDFLAGS = 

irtsp_LDADD = $(IRTSP_LIBS) -lBasicUsageEnvironment -lUsageEnvironment -lgroupsock -lliveMedia

