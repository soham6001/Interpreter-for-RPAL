/*
 *      cse_machine.hh
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

#ifndef COP5555_CSEMACHINE
#define COP5555_CSEMACHINE
#include "tree.hh"
#include <queue>
#include <vector>
#include "delta.hh"
#include <stack>
#include <map>
#include "cse_nodes.hh"


class cse_machine {
        int currentEnvNo;
        int envCount;
        node* root; // the root of the (standardized tree)
        int flatten_tree();//flatten the tree return 1 if successful
        void flatten(int, node*,bool); // internal worker fn for flatten_tree
        queue<node *> queue_nodes;
        vector<delta> deltas; //vector for holding the deltas
        int current_index; //the index to use for next lambda
        int delta_no; //the delta which is being created
        stack<node *> cse_control;
        stack<node *> cse_stack;
        map<int, environment*> env_map;
        enum nodeType { UNKNOWN = -1, ID, INT, STR, OPERATOR, KEYWORD, TUPLE };
enum RuleNum   { e_unknown = -1, e_let = 0, e_where, e_within, e_and, e_tau,
                       e_functionform, e_lambda, e_binary, e_unary,
                       e_rec, e_comma, e_at, e_assignment, e_gamma, e_cond };
        int getRuleTypeId(string arg);
        int getNodeTypeId(string arg);
        string executeBinaryOp(string Op, string argA, string argB);
        public:
                cse_machine(node*);
                void print(); //print the deltas
                int run_machine(); //evaluate the machine
                void print_string(std::string);//print out a string
};
#endif
