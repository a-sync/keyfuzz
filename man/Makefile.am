# $Id: Makefile.am 3 2004-01-25 17:23:20Z lennart $

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

man_MANS = keyfuzz.8

noinst_DATA = keyfuzz.8.xml 

EXTRA_DIST = $(man_MANS) keyfuzz.8.xml.in xmltoman.css xmltoman.xsl xmltoman.dtd

CLEANFILES = keyfuzz.8.xml

keyfuzz.8.xml: keyfuzz.8.xml.in Makefile
	sed -e 's,@sysconfdir\@,$(sysconfdir),g' -e 's,@sbindir\@,$(sbindir),g' -e 's,@PACKAGE_BUGREPORT\@,$(PACKAGE_BUGREPORT),g' -e 's,@PACKAGE_URL\@,$(PACKAGE_URL),g' $< > $@

if USE_XMLTOMAN

CLEANFILES += $(man_MANS)

keyfuzz.8: keyfuzz.8.xml Makefile
	xmltoman $< > $@

endif
