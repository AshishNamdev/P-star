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

#include "function.h"
#include "expression_state.h"
#include "types.h"

#include <cstring>

using namespace std;

void wpl_function::generate_signature() {
	ostringstream name_builder;
	name_builder << return_type->get_name() << " " << get_name() << "(";
	generate_typename_list(name_builder);
	name_builder << ")";
	strncpy (function_name, name_builder.str().c_str(), WPL_VARNAME_SIZE);
}

wpl_state *wpl_function::new_state(wpl_namespace_session *nss_this, wpl_namespace_session *nss_caller) {
	return new wpl_function_state (nss_this, nss_caller, this, return_type);
}