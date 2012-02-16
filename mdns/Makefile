# Copyright (C) 2007-2011 cozybit Inc.
#
# This file is part of libmdns.
#
# libmdns is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as
# published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# libmdns is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public
# License along with libmdns. If not, see <http://www.gnu.org/licenses/>.

CFLAGS += -I$(TOP)/include
OBJS = mdns.o mdns_respond.o mdns_query.o debug.o dname.o

all: libmdns.a

libmdns.a: $(OBJS)
	$(AR) rcs $@ $^

clean:
	rm -f *.o libmdns.a
