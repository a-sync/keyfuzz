#!/bin/bash
# $Id$

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

run_versioned() {
    local P
    type -p "$1-$2" &> /dev/null && P="$1-$2" || local P="$1"

    shift 2
    "$P" "$@"
}

if [ "x$1" = "xam" ] ; then
    set -ex
    run_versioned automake 1.7 -a -c
    ./config.status
else 
    set -ex
    rm -rf autom4te.cache
    rm -f config.cache

    run_versioned aclocal 1.7
    autoheader
    run_versioned automake 1.7 -a -c
    autoconf -Wall

    ./configure --sysconfdir=/etc "$@"

    make clean
fi
