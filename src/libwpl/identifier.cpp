/*

-------------------------------------------------------------

Copyright (c) MMIII Atle Solbakken
atle@goliathdns.no

-------------------------------------------------------------

This file is part of P* (P-star).

P* is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

P* is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with P*.  If not, see <http://www.gnu.org/licenses/>.

-------------------------------------------------------------

*/

#include "identifier.h"

#include <cstdio>
#include <cstring>

void wpl_identifier::set_name (const char *name) {
	int chars = snprintf (this->name, WPL_VARNAME_SIZE, "%s", name);
	if (chars > WPL_VARNAME_SIZE) {
		throw runtime_error("Overflow in identifier name");
	}
}

void wpl_identifier::set_name(const char *name, int len) {
	if (len > WPL_VARNAME_SIZE) {
		throw runtime_error("Identifier name too long");
	}
	strncpy (this->name, name, len);
	this->name[len] = '\0';
}

bool wpl_identifier::is_name (const char *name) const {
	return (strcmp(this->name, name) == 0);
}
