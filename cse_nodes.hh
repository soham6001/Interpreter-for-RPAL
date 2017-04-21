/*
 *      cse_nodes.hh
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

#ifndef COP5555_CSENODES
#define COP5555_CSENODES
#include "tree.hh"
#include <list>
#include <map>

class lambda:public node {
        public:
        list<std::string> params; //a list of the parameters
        int lambda_env_num;
        lambda(std::string token_name, std::string token_type) : node(token_name, token_type){lambda_env_num = -1;}
        void add_param(std::string);
        void add_comma(node*);
        void print();
};

class tau:public node {

public:
        node* tree_reference;
        int num_elements; //holds the number of elements in the tuple
        node * get_element(int);
        tau(std::string token_name, std::string token_type) : node(token_name, token_type){num_elements = 0;};
        void set_num_elements(node *);//this function sets the number of elements in the tau
        void print_tau();
};

class environment: public node {
public:
        int calling_env;
        std::map<string,node*> bindings;
        int parent_env;
        int current_env;
        environment(int current_env, int parent_env) : node("Environment","Environment"){ 
						bindings.insert (make_pair( "Print", new node("Print", "Identifier" ) ) );
						bindings.insert (make_pair( "Conc", new node("Conc", "Identifier" ) ) );
						bindings.insert (make_pair( "Stem", new node("Stem", "Identifier" ) ) );
						bindings.insert (make_pair( "Stern", new node("Stern", "Identifier" ) ) );
						bindings.insert (make_pair( "Order", new node("Order", "Identifier" ) ) );
						bindings.insert (make_pair( "Isstring", new node("Isstring", "Identifier" ) ) );
						bindings.insert (make_pair( "Isinteger", new node("Isinteger", "Identifier" ) ) );
						bindings.insert (make_pair( "Istuple", new node("Istuple", "Identifier" ) ) );
						bindings.insert (make_pair( "Istruthvalue", new node("Istruthvalue", "Identifier" ) ) );
						bindings.insert (make_pair( "Isfunction", new node("Isfunction", "Identifier" ) ) );
						bindings.insert (make_pair( "Isdummy", new node("Isummy", "Identifier" ) ) );
						bindings.insert (make_pair( "ItoS", new node("ItoS", "Identifier" ) ) );
						bindings.insert (make_pair( "dummy", new node("dummy", "Identifier" ) ) );
        this->current_env = current_env;
        this->parent_env = parent_env;}
};


class beta: public node {
        public:
        int if_delta_num, else_delta_num;
        beta():node("beta","beta") {this->if_delta_num = -1; this->else_delta_num = -1; }
};




class delta_node: public node {
public:
    int delta_no;
    delta_node(int delta_no): node( "Delta" , "Delta" ) { this->delta_no  = delta_no;}
};

class tuple: public node {
public:
        list<node *> elements;
        tuple(list<node *> &elements):node( "tuple", "tuple" ) { this->elements = elements; }
        void print();
        node* get_element(int);
};

class eta: public node {
	public :
	lambda * lambda_closure;
	eta(lambda * lambda_closure):node("eta","eta") { this->lambda_closure = lambda_closure;}
};
#endif

