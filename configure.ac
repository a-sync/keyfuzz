# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# $Id: configure.ac 12 2006-02-14 16:20:46Z lennart $

# This file is part of keyfuzz.
#
# keyfuzz is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# keyfuzz is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with keyfuzz; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.

AC_PREREQ(2.59)
AC_INIT([keyfuzz],[0.2],[mzxrlshmm (at) 0pointer (dot) de])
AC_CONFIG_SRCDIR([src/keyfuzz.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign 1.9 -Wall])

AC_SUBST(PACKAGE_URL, [http://0pointer.de/lennart/projects/keyfuzz/])

if type -p stow > /dev/null && test -d /usr/local/stow ; then
   AC_MSG_NOTICE([*** Found /usr/local/stow: default install prefix set to /usr/local/stow/${PACKAGE_NAME}-${PACKAGE_VERSION} ***])
   ac_default_prefix="/usr/local/stow/${PACKAGE_NAME}-${PACKAGE_VERSION}"
fi

# Checks for programs.
AC_PROG_CC
AC_PROG_LN_S
AC_PROG_MAKE_SET

# If using GCC specifiy some additional parameters
if test "x$GCC" = "xyes" ; then
   CFLAGS="$CFLAGS -pipe -Wall -W -pedantic"
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h limits.h stdlib.h string.h sys/ioctl.h unistd.h linux/input.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strchr strerror strspn])

# Init script location
AC_ARG_WITH(initdir, AS_HELP_STRING(--with-initdir=DIR,Install init script in DIR (system dependent default)))

if test "x$with_initdir" = xno ; then
	SYSINITDIR=no
	AC_MSG_NOTICE([*** No init script will be installed ***])
else
	test "x$with_initdir" = xyes && with_initdir=

	if test "x$with_initdir" = x ; then
	    if test -d /etc/init.d ; then
        	SYSINITDIR=/etc/init.d
	    else
    		if test -d /etc/rc.d/init.d ; then
	        	SYSINITDIR=/etc/rc.d/init.d
		    else
	        	AC_MSG_ERROR([missing --with-initdir=DIR])
	    	fi
	    fi
	else
	   SYSINITDIR="$with_initdir"
	fi
	AC_MSG_NOTICE([*** Init script will be installed in $SYSINITDIR ***])
fi
AC_SUBST(SYSINITDIR)
AM_CONDITIONAL(NOINITSCRIPT, test x$SYSINITDIR = xno)

# LYNX documentation generation
AC_ARG_ENABLE(lynx,
        AS_HELP_STRING(--disable-lynx,Turn off lynx usage for documentation generation),
[case "${enableval}" in
  yes) lynx=yes ;;
  no)  lynx=no ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --disable-lynx) ;;
esac],[lynx=yes])

if test x$lynx = xyes ; then
   AC_CHECK_PROG(have_lynx, lynx, yes, no)

   if test x$have_lynx = xno ; then
     AC_MSG_ERROR([*** Sorry, you have to install lynx or use --disable-lynx ***])
   fi
fi

AM_CONDITIONAL([USE_LYNX], [test "x$lynx" = xyes])

# XMLTOMAN manpage generation
AC_ARG_ENABLE(xmltoman,
        AS_HELP_STRING(--disable-xmltoman,Disable rebuilding of man pages with xmltoman),
[case "${enableval}" in
  yes) xmltoman=yes ;;
  no)  xmltoman=no ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --disable-xmltoman]) ;;
esac],[xmltoman=yes])

if test x$xmltoman = xyes ; then
   AC_CHECK_PROG(have_xmltoman, xmltoman, yes, no)

   if test x$have_xmltoman = xno ; then
     AC_MSG_WARN([*** Not rebuilding man pages as xmltoman is not found ***])
     xmltoman=no
   fi
fi

AM_CONDITIONAL([USE_XMLTOMAN], [test "x$xmltoman" = xyes])

# gengetopt command line parser generation
AC_ARG_ENABLE(gengetopt,
        AS_HELP_STRING(--disable-gengetopt,Disable rebuilding of command line parser with gengetopt),
[case "${enableval}" in
  yes) gengetopt=yes ;;
  no)  gengetopt=no ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --disable-gengetopt]) ;;
esac],[gengetopt=yes])

if test x$gengetopt = xyes ; then
   AC_CHECK_PROG(have_gengetopt, gengetopt, yes, no)

   if test x$have_gengetopt = xno ; then
     AC_MSG_WARN([*** Not rebuilding command line parser as gengetopt is not found ***])
     gengetopt=no
   fi
fi

AM_CONDITIONAL([USE_GENGETOPT], [test "x$gengetopt" = xyes])

AC_CONFIG_FILES([src/Makefile Makefile conf/Makefile man/Makefile doc/Makefile doc/README.html])
AC_OUTPUT
