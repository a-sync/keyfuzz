# $Id: Makefile.am 9 2006-02-14 03:04:23Z lennart $
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

pkgsysconfdir=$(sysconfdir)/@PACKAGE@
sysinitdir=@SYSINITDIR@

pkgsysconf_DATA = default.keyfuzz medion_9580f.keyfuzz msi_s270.keyfuzz
noinst_DATA = keyfuzz.init

EXTRA_DIST = $(pkgsysconf_DATA) keyfuzz.init.in
CLEANFILES = keyfuzz.init

if !NOINITSCRIPT 

install-data-local: keyfuzz.init
	$(INSTALL) -b -D -m755 keyfuzz.init ${DESTDIR}${sysinitdir}/keyfuzz

uninstall-local:
	rm -f ${DESTDIR}${sysconfdir}/init.d/keyfuzz

endif

keyfuzz.init: keyfuzz.init.in Makefile
	sed -e 's,@pkgsysconfdir\@,$(sysconfdir)/@PACKAGE@,g' -e 's,@sbindir\@,$(sbindir),g' $< > $@
	chmod 755 $@
