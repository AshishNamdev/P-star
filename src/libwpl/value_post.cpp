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

#include "value_post.h"
#include "operator.h"
#include "value_string.h"
#include "value_array.h"
#include "matcher.h"

#include <stdlib.h>
#include <memory>
#include <cstring>
#include <map>

using namespace mimetic;

const wpl_type_array wpl_value_post::type_array("GET array");
const wpl_type_bool wpl_value_post::type_bool("GET bool");
const wpl_type_array_instance wpl_value_post::type_complete_array(&type_array, &type_bool);
/*
void printMimeStructure(MimeEntity* pMe, int tabcount = 0)
{
	Header& h = pMe->header();                   // get header object
	for(int c = tabcount; c > 0; --c)            // indent nested entities
		cerr << "    ";                      //
	cerr << h.contentType() << endl;             // prints Content-Type
	cerr << pMe->body().preamble() << endl;
	cerr << pMe->body().epilogue() << endl;
	cerr << h.contentDisposition().param(string("name")) << endl;             // prints Content-Type
	MimeEntityList& parts = pMe->body().parts(); // list of sub entities obj
	// cycle on sub entities list and print info of every item
	MimeEntityList::iterator mbit = parts.begin(), meit = parts.end();
	for(; mbit != meit; ++mbit)
		printMimeStructure(*mbit, 1 + tabcount);
}
*/
void wpl_value_post::parse_entity (MimeEntity *me) {
	Header &header = me->header();

	string name = header.contentDisposition().param(string("name"));

	if (!name.empty()) {
		string out;

		/* Credits: https://github.com/SteveJones/mailview/blob/master/mailview.cpp */
		if (me->hasField("Content-Transfer-Encoding")) {
			std::string content_transfer_encoding
				= header.contentTransferEncoding().mechanism();
			if (content_transfer_encoding == "quoted-printable") {
				QP::Decoder qp;
				decode(me->body().begin(), me->body().end(), qp, out.begin());
				goto copy_done;
			}
			if (content_transfer_encoding == "base64") {
				Base64::Decoder b64;
				decode(me->body().begin(), me->body().end(), b64, out.begin());
				goto copy_done;
			}
		}

		out = me->body();

		copy_done:
		wpl_value_array *storage = (wpl_value_array*) hash.get(name);
		if (!storage) {
			storage = (wpl_value_array*) hash.define(name);
		}
		storage->push(new wpl_value_string(out));
	}

	/* Credits: http://www.codesink.org/mimetic_mime_library.html */
	MimeEntityList &parts = me->body().parts();
	MimeEntityList::iterator mbit = parts.begin(), meit = parts.end();
	for(; mbit != meit; ++mbit) {
		parse_entity(*mbit);
	}
}

void wpl_value_post::parse() {
	const char *content_type = getenv("CONTENT_TYPE");
	const char *content_length = getenv("CONTENT_LENGTH");
	const char *request_method = getenv("REQUEST_METHOD");

	if (!request_method || strcmp(request_method, "POST") != 0 || !content_type) {
		return;
	}

	if (!content_length) {
		throw runtime_error("Could not find CONTENT_LENGTH environment variable while parsing POST-query");
	}

	long int length = strtol(content_length, NULL, 10);

	if (length <= 0) {
		cerr << "Invalid value for CONTENT_LENGTH '" << content_length << "'\n";
		throw runtime_error("Error while parsing CONTENT_LENGTH environment variable in POST object");
	}

	unique_ptr<char[]> buf_ptr(new char[500+1]);
	char *buf = buf_ptr.get();

	cin.read(buf, length);
	buf[length] = '\0';

	if (strcmp(content_type, "application/x-www-form-urlencoded") == 0) {
		cerr << "Found application/x-www-form-urlencoded\n";

		value_get.parse(buf);

		use_get = true;
		return;
	}

	const char name[] = "multipart/form-data";
	if (strncmp(content_type, name, sizeof(name)-1) == 0) {
		const char *ct = "Content-Type: ";
		stringstream ss;
		ss << ct << content_type << "\r\n\r\n" << buf;

		MimeEntity me(ss);

		parse_entity(&me);

		return;
	}
}

int wpl_value_post::do_operator (
		wpl_expression_state *exp_state,
		wpl_value *final_result,
		const wpl_operator_struct *op,
		wpl_value *lhs,
		wpl_value *rhs
) {
	if (!did_parse) {
		parse();
		did_parse = true;
	}

	if (use_get) {
		return value_get.do_operator(
			exp_state,
			final_result,
			op,
			(lhs == this ? &value_get : lhs),
			(rhs == this ? &value_get : rhs)
		);
	}

	return hash.do_operator (
		exp_state,
		final_result,
		op,
		(lhs == this ? &hash : lhs),
		(rhs == this ? &hash : rhs)
	);
}
