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

#ifndef __WPL_PARSER_H
  #define __WPL_PARSER_H

#include "scene.h"
#include "matcher.h"
#include "namespace.h"

#include <iostream>
#include <list>
#include <memory>

using namespace std;

class wpl_namespace;

class wpl_parser : public wpl_matcher {
	private:

	static int files_count;
	list<unique_ptr<wpl_parser>> includes;

	char varname_tmp[WPL_VARNAME_SIZE+1];

	void throw_parser_exception(const char *msg, const struct wpl_matcher_position *where);
	void syntax_error();

	void parse_include(wpl_namespace *parent_namespace);
	void parse_scene(wpl_namespace *parent_namespace);
	void parse_template(wpl_namespace *parent_namespace);
	void first_level(wpl_namespace *parent_namespace);

	char *file_content;

	public:

	wpl_parser ();
	~wpl_parser ();
	void parse_file(wpl_namespace *parent_namespace, const char *filename);
};

#endif // __WPL_PARSER_H