# $Id: Makefile.am 15 2006-02-14 16:30:09Z lennart $

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

EXTRA_DIST=bootstrap.sh README LICENSE
SUBDIRS=src conf man doc

MAINTAINERCLEANFILES = README
noinst_DATA = README

README:
	rm -f README
	$(MAKE) -C doc README
	cd $(srcdir) && ln -s doc/README README

homepage:
	test -d $$HOME/homepage/private
	mkdir -p $$HOME/homepage/private/projects/keyfuzz
	cp keyfuzz-@PACKAGE_VERSION@.tar.gz man/keyfuzz.8.xml man/xmltoman.css man/xmltoman.xsl man/xmltoman.dtd doc/style.css doc/README.html $$HOME/homepage/private/projects/keyfuzz
	ln -sf README.html $$HOME/homepage/private/projects/keyfuzz/index.html

distcleancheck:
	@

.PHONY: homepage distcleancheck
