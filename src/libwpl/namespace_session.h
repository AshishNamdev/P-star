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

#pragma once

#include "namespace.h"
#include "variable.h"

#include <vector>
#include <list>
#include <memory>

class wpl_expression_state;

/**
 * @brief This class is passed into all runables which are called with the run() function. The runables copy their identifiers into this temporary namespace. The namespace session creates a child of itself when a runable calls another runable.
 */
class wpl_namespace_session {
	private:
	wpl_namespace_session *parent;
	wpl_namespace_session *sibling;

	bool do_sibling_lookup;
	const wpl_namespace *template_namespace;

	vector<unique_ptr<wpl_variable>> variables_ptr;

	public:
	wpl_namespace_session(const wpl_namespace_session &copy);
	wpl_namespace_session &operator= (const wpl_namespace_session &rhs);
	wpl_namespace_session(
			wpl_namespace_session *parent
	);
	wpl_namespace_session(
			wpl_namespace_session *parent,
			const wpl_namespace *template_namespace
	);
	wpl_namespace_session(
			wpl_namespace_session *parent,
			wpl_namespace_session *sibling,
			const wpl_namespace *template_namespace
	);
	wpl_namespace_session(const wpl_namespace *template_namespace);
	wpl_namespace_session();
	~wpl_namespace_session();

	void use_sibling_lookup() {
		do_sibling_lookup = true;
	}

	bool set_variables_from_expression (wpl_expression_state *exp_state);
	void replace_variables (wpl_namespace_session *source);

	void push(wpl_variable *variable);

	wpl_variable *find_variable(const wpl_value *return_value);
	wpl_variable *find_variable(const char *name);
	wpl_function *find_function(const char *name);

	wpl_variable *get_variable(int i);
	int variable_index(const char *name);

	void set_namespace(wpl_namespace *template_namespace) {
		this->template_namespace = template_namespace;
	}

	void set_parent(wpl_namespace_session *parent) {
		this->parent = parent;
	}

	void set_sibling(wpl_namespace_session *sibling) {
		this->sibling = sibling;
	}

	void variable_list(list<wpl_variable*> &target) {
		for (unique_ptr<wpl_variable> &variable : variables_ptr) {
			target.push_back(variable.get());
		}
	}
};
