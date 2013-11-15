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

#include "state.h"
#include "block_state.h"
#include "exp_deque.h"
#include "shunting_yard_carrier.h"

#include <unordered_map>

using namespace std;

class wpl_namespace_session;
class wpl_runable;

class wpl_expression_state : public wpl_state {
	private:

	wpl_exp_deque<shunting_yard_carrier> run_stack;
	wpl_exp_deque<wpl_value*> wait_stack;
	wpl_exp_deque<wpl_value*> discard_chain;

	unordered_map<int,unique_ptr<wpl_state>> child_states;

	void optimize();

	public:
	wpl_expression_state (
			wpl_namespace_session *nss,
			const wpl_exp_deque<shunting_yard_carrier> &my_list
			) :
		wpl_state(nss),
		run_stack(my_list)
	{
		optimize();
		run_stack.save_pos();
	}

	wpl_namespace_session *get_nss() {
		return nss;
	}

	wpl_variable *find_variable (const char *name) {
		return nss->find_variable(name);
	}

	wpl_function *find_function (const char *name) {
		return nss->find_function(name);
	}

	wpl_state *get_child_state(int index) {
		return child_states[index].get();
	}

	// RUN STACK
	void revert() {
		run_stack.revert_pos();
		wait_stack.clear();
		discard_chain.clear();
	}

	void replace (int index, shunting_yard_carrier &new_carrier) {
		run_stack.replace (index, new_carrier);
	}

	int pos() {
		return run_stack.pos();
	}

	bool empty() {
		return run_stack.empty();
	}

	shunting_yard_carrier &top() {
		return run_stack.top();
	}

	void pop() {
		run_stack.pop();
	}

	wpl_exp_deque<shunting_yard_carrier> &get_stack() {
		return run_stack;
	}

	// WAITING OPERANDS QUEUE
	void push_waiting(wpl_value *value) {
		wait_stack.push(value);
	}

	void pop_waiting() {
		wait_stack.pop();
	}

	wpl_value *top_waiting() {
		return wait_stack.top();
	}

	bool empty_waiting() {
		return wait_stack.empty();
	}

	// DISCARD CHAIN
	wpl_exp_deque<wpl_value*> &get_discard() {
		return discard_chain;
	}

	void push_front_discard(wpl_value *value) {
		discard_chain.push_front(value);
	}

	void push_discard(wpl_value *value) {
		discard_chain.push(value);
	}

	void clear_discard() {
		discard_chain.clear();
	}

	// RUNNING
	int run_child (wpl_runable *runable, int index, wpl_value *final_result);
	int run_function (
			wpl_function *function,
			int index,
			wpl_value *final_result,
			wpl_namespace_session *nss_this,
			wpl_namespace_session *nss_caller
	);
};