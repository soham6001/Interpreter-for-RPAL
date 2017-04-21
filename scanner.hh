/*
 *      scanner.hh
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

#ifndef COP5555_SCANNER
#define COP5555_SCANNER
#include "token.hh"
#include <fstream>

class Scanner
{
        public:
                Scanner(std::string);
                void print_file();
                Token get_token(); //get the next token
                Token peek_token(); //look at the next token
                                   //without getting it

        private:
                void init();
                void get_next_token();
                bool is_alphabet(char);
                bool is_underscore(char);
                bool is_number(char);
                bool is_keyword( std::string );
                bool is_operator( char );
                bool is_string( char );
                bool is_string_symbol( char );
                bool valid_following_backslash( char );
                bool valid_following_comment( char );
                bool is_punction( char );

                //member declarations
                std::string current_token;
                std::string file_name;
                std::ifstream input_file;
                enum states { START,
                IDENTIFIER, INTEGER, OPERATOR, STRING, DELETE,
                BACKSLASH, END, COMMENT, PUNCTION, ENDOFFILE, SPACES};
                Token Buffer; //Buffers the next token
};

#endif

