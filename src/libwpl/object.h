/*

-------------------------------------------------------------

Copyright (c) MMXIII Atle Solbakken
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

#ifndef __WPL_OBJECT_H
  #define __WPL_OBJECT_H

#include <cstdlib>
#include <cstring>

class wpl_object {
	private:
	void *pointer;
	int size;

	public:
	~wpl_object() {
		free(pointer);
	}
	wpl_object (int size);
	void write (void *bytes, int length);
	inline void *get () {
		return this->pointer;
	}
};

#endif // __WPL_OBJECT_H

