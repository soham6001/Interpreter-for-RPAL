/*
 *      environment.hh
 *
 *      Copyright 2009 M.S. Bhinder <mbhinder@ufl.edu>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

#ifndef COP5555_ENVIRONMENT
#define COP5555_ENVIRONMENT
#include <map>
#include "tree.hh"

class environment {
		
		map<std::string, node> bindings;
public:
		environment * parent;
		int env_no;
		environment * calling_env;
		void add_binding(std::string, node *);
		void get_binding(std::string);
		environment(int env_no);
};

#endif
