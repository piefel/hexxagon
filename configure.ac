##################################################################
# Process this file with autoconf to produce a configure script. #
##################################################################

AC_INIT([Hexxagon], [1.0], [hexxagon@nesqi.se])
AM_INIT_AUTOMAKE([Hexxagon], [1.0])

AC_CONFIG_SRCDIR([src/libhexx/bitboard64.cpp])

AM_CONFIG_HEADER(src/config.h)

AC_LANG(C++)

IMAGE_INSTALL_DIR=${datadir}/hexxagon/images/
AC_SUBST(IMAGE_INSTALL_DIR)

###########################
# Adjust compiler options #
###########################

[
if [ -z "$CXXFLAGS" ]
then
    CXXFLAGS="-O2"
fi
]

########################
# Checks for programs. #
########################

AC_PROG_CC
AC_PROG_AWK
AC_PROG_LN_S
AC_PROG_RANLIB
AC_PROG_CXX

#########################
# Checks for libraries. #
#########################

PKG_CHECK_MODULES(GTKMM, gtkmm-2.4)


############################
# Checks for header files. #
############################

AC_CHECK_HEADERS([netinet/in.h stdlib.h string.h strings.h sys/timeb.h])

##################################################################
# Checks for typedefs, structures, and compiler characteristics. #
##################################################################

AC_C_CONST

#################################
# Checks for library functions. #
#################################

AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_HEADER_STDC
AC_CHECK_FUNCS([ftime memset strcasecmp strncasecmp])

AC_CONFIG_FILES(Makefile 
                images/Makefile
                src/Makefile 
                src/libhexx/Makefile
                src/gui/Makefile)

AC_OUTPUT

