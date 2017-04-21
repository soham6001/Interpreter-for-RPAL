/*
 *      parser.hh
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
#ifndef COP5555_PARSER
#define COP5555_PARSER
#include "token.hh"
#include "scanner.hh"
#include "tree.hh"

class Parser
{
        public:
                void build_ast();
                Parser(std::string file_name);
                void print_ast();
                //void set_next_token(Token);
                //made the tree public in
                //order to pass it to the
                //standardizer
                Scanner scanner;
                tree tree_stack;

        private:
                std::string file_name;
                Token current_token;

                //private functions
                void parse_e();
                void parse_ew();
                void parse_t();
                void parse_ta();
                void parse_tc();
                void parse_b();
                void parse_bt();
                void parse_bs();
                void parse_bp();
                void parse_a();
                void parse_at();
                void parse_af();
                void parse_ap();

                void parse_r();
                void parse_rn();
                void parse_d();
                void parse_da();
                void parse_dr();
                void parse_db();
                void parse_vb();
                void parse_vl();


};

#endif
