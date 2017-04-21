/*
 *      main.cc
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
#include<iostream>
#include<fstream>
#include "scanner.hh"
#include "token.hh"
#include "parser.hh"
#include <fstream>
#include <cstdlib>
#include "StandardizeAST.hh"
#include "cse_machine.hh"


int main(int argc, char* argv[])
{
        int print_flag = 0;
        int ast_flag = 0;
        std::string file_name;

        if ( argc <= 1)
        {
                std::cout<<"Please specify a file name"<<std::endl;
                exit(1);
        }
        for (int i = 1; i < argc; i++)
        {
                std::string args = argv[i];
                if ( args == "-l" )
                {
                        print_flag = 1;
                }
                else if (args == "-ast")
                {
                        ast_flag = 1;
                }
                else
                {
                        file_name = args;
                }
        }
        if(print_flag)
        {
                std::ifstream input_file;
                input_file.open( file_name.c_str() );

                if ( !( input_file.is_open() ) )
                {
                        std::cout<<"Could not open file"<<std::endl;
                        exit(1);
                }

                std::string line;
                while (! input_file.eof() )
                {
                        getline( input_file, line );
                        std::cout<<line<<std::endl      ;
                }
                exit(0);
        }

        Parser p(file_name);
        p.build_ast();

        if(ast_flag)
        {
                p.print_ast();
        }

        //testing
        StandardizeAST std_ast(*(p.tree_stack.Root));
        std_ast.standardize();
        //std_ast.Print();
       cse_machine new_machine(&std_ast.treeNode);
       //new_machine.print();
	   new_machine.run_machine();
}

