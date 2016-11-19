# $Id: Makefile.am 3 2004-01-25 17:23:20Z lennart $
#
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

sbin_PROGRAMS = keyfuzz

keyfuzz_SOURCES = keyfuzz.c cmdline.c cmdline.h
keyfuzz_CFLAGS = $(AM_CFLAGS) -DPKGSYSCONFDIR="\"${sysconfdir}/@PACKAGE@\""

EXTRA_DIST = keyfuzz.ggo
MAINTAINERCLEANFILES =
BUILT_SOURCES = 
CLEANFILES = 

if USE_GENGETOPT 

CLEANFILES += cmdline.c cmdline.h
BUILT_SOURCES += cmdline.c cmdline.h

cmdline.c cmdline.h: keyfuzz.ggo Makefile
	gengetopt < $<

endif
