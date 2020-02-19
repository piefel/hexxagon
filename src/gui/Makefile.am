LIBS = @GTKMM_LIBS@
AM_CPPFLAGS = @GTKMM_CFLAGS@ -DIMAGE_INSTALL_DIR="\"$(IMAGE_INSTALL_DIR)\""

bin_PROGRAMS = hexxagon
hexxagon_SOURCES = gtkeditwindow.cpp gtkhexxagonboardedit.cpp gtkaboutwindow.cpp \
				   gamemodel.cpp gtkhexxagonboard.cpp gtkhexxagonboardplay.cpp \
				   gtkmainmenu.cpp gtkmainwindow.cpp gtknavbar.cpp hexxagon.cpp \
				   gtkboardlist.cpp gtkscore.cpp gtkscaleimage.cpp \
				   gtkgameoverdialog.cpp helper_functions.cpp

LDADD = ../libhexx/libhexx.a

