/*
 *      parser.cc
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
#include "parser.hh"
#include <iostream>
#include <string>
#include <cstdlib>
#define DEBUG 0

using namespace std;

Parser::Parser(std::string file_name):scanner(file_name),tree_stack(&scanner)
{
        this->file_name = file_name;
}

/*void Parser::set_next_token(Token token)
{
        current_token = token;
}*/

void Parser::build_ast()
{
        current_token = scanner.get_token();

        if ( current_token.get_token() == "EOF" )
        {
                exit(0);
        }//check if the file is empty
        else
        {
                parse_e();
        }
}

void Parser::parse_e()
{
        if ( current_token.get_token() == "let" )
        {
                if(DEBUG)
                        std::cout<<endl<<"E-> let D in E";
                tree_stack.read( current_token, "let");

                current_token = scanner.get_token();
                tree_stack.pop();

                parse_d();
                tree_stack.read( current_token, "in");
                current_token = scanner.get_token();

                tree_stack.pop();
                //std::cout<<endl<<scanner.peek_token().get_token();
                parse_e();

                tree_stack.build( "let", "Keyword", 2);
                /*tree_stack.read( current_token, "EOF");
                current_token = scanner.get_token();
                tree_stack.pop();*/
        }
        else if ( current_token.get_token() == "fn" )
        {
                if(DEBUG)
                        std::cout<<endl<<"E -> fn VB+ '.' E";
                //read("fn")
                tree_stack.read( current_token, "fn");
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_vb();

                int count = 1;
                while( current_token.get_token() != "." && current_token.get_token() != "EOF" )
                {
                        parse_vb();
                        count ++;
                }

                //read(".");
                tree_stack.read( current_token, ".");
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_e();
                //build("let",2)
                tree_stack.build( "lambda" , "Operator", count + 1);
                //read("EOF")
                /*
                tree_stack.read( current_token, "EOF");
                current_token = scanner.get_token();
                tree_stack.pop();*/
        }
        else
        {
                if(DEBUG)
                        std::cout<<endl<<"E -> Ew";
                parse_ew();
        }

}

void Parser::parse_ew()
{
        if(DEBUG)
                std::cout<<endl<<"E -> T";
        parse_t();

        if ( current_token.get_token() == "where" )
        {

                if(DEBUG)
                        std::cout<<endl<<"E -> T where Dr";
                //read(where)
                tree_stack.read( current_token, "where");
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_dr();
                //build(where,2)
                tree_stack.build( "where" , "Keyword", 2);
        }
}

void Parser::parse_t()
{
        parse_ta();
        if(DEBUG)
                std::cout<<endl<<"E -> T where Dr";

                int count = 1;
                while(current_token.get_token() == ",")
                {
                        //read(",")
                        tree_stack.read( current_token, ",");
                        current_token = scanner.get_token();
                        tree_stack.pop();
                        parse_ta();
                        count++;
                }

                if(count > 1)
                        tree_stack.build( "tau" , "Operator", count );
                //build(tau,count)
}

void Parser::parse_ta()
{
        parse_tc();
        if(DEBUG)
                std::cout<<endl<<"Ta - > Tc";


        //if (current_token.get_token() == "aug")
        //{
                //int count = 1;
        while(current_token.get_token() == "aug")
        {
                //read("aug")
                tree_stack.read( current_token, "aug");
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_tc();
                //count++;
                tree_stack.build( "aug" , "Keyword", 2 );
        }
                //build("aug",count)
                //tree_stack.build( "aug" , "Keyword", count );

        //}

}

void Parser::parse_tc()
{
        parse_b();
        if(DEBUG)
                std::cout<<endl<<"Tc -> B";

        while (current_token.get_token() == "->")
        {
                //read("->")
                tree_stack.read( current_token, "->");
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_tc();
                //read("|")
                tree_stack.read( current_token, "|");
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_tc();
                tree_stack.build( "->" , "Operator", 3 );


        }

}

void Parser::parse_b()
{
        parse_bt();
        if(DEBUG)
                std::cout<<endl<<"B -> bt";

        while(current_token.get_token() == "or")
        {
                //read("or")
                tree_stack.read( current_token, "or");
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_bt();
                tree_stack.build( "or" , "Keyword", 2 );
                //build("or",count)

        }

}

void Parser::parse_bt()
{
        parse_bs();
        if(DEBUG)
                std::cout<<endl<<"Bt -> bs";

        while(current_token.get_token() == "&")
        {
                //read("&")
                //parse_bs()
                tree_stack.read( current_token, "&");
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_bs();
                tree_stack.build( "&" , "Keyword", 2 );
                //build("&",count)
        }
}

void Parser::parse_bs()
{
        if ( current_token.get_token() == "not" )
        {
                if(DEBUG)
                        std::cout<<endl<<"Bs -> not Bp";
                tree_stack.read( current_token, "not");
                current_token = scanner.get_token();
                tree_stack.pop();
                //read("not")
                parse_bp();
                //build("not",1)
                tree_stack.build( "not" , "Keyword", 1 );
        }
        else
        {
                parse_bp();
        }
}

void Parser::parse_bp()
{
        if(DEBUG)
                std::cout<<endl<<"Bp -> A";
        parse_a();

        if ( current_token.get_token() == "gr" || current_token.get_token() == ">" )
        {
                if(DEBUG)
                        std::cout<<endl<<"Bp -> A 'gr'A";
                //read(current_token.get_token())  - token could be either gr or >
                tree_stack.read( current_token, current_token.get_token() );
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_a();
                //build("gr",2)
                tree_stack.build( "gr" , "Keyword", 2 );
        }
        else if ( current_token.get_token() == "ge" || current_token.get_token() == ">=" )
        {
                if(DEBUG)
                        std::cout<<endl<<"Bp -> A 'ge'A";
                //read(current_token.get_token())  - token could be either gr or >
                tree_stack.read( current_token, current_token.get_token() );
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_a();
                //build("gr",2)
                tree_stack.build( "ge" , "Keyword", 2 );
        }
        else if ( current_token.get_token() == "ls" || current_token.get_token() == "<" )
        {
                if(DEBUG)
                        std::cout<<endl<<"Bp -> A 'ls'A";
                //read(current_token.get_token())  - token could be either gr or >
                tree_stack.read( current_token, current_token.get_token() );
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_a();
                //build("gr",2)
                tree_stack.build( "ls" , "Keyword", 2 );
        }
        else if ( current_token.get_token() == "le" || current_token.get_token() == "<=" )
        {
                if(DEBUG)
                        std::cout<<endl<<"Bp -> A 'le'A";
                //read(current_token.get_token())  - token could be either gr or >
                tree_stack.read( current_token, current_token.get_token() );
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_a();
                //build("gr",2)
                tree_stack.build( "le" , "Keyword", 2 );
        }

        else if ( current_token.get_token() == "eq" )
        {
                if(DEBUG)
                        std::cout<<endl<<"Bp -> A 'eq'A";
                tree_stack.read( current_token,  "eq" );
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_a();
                //read(current_token.get_token())
                //parse_a()
                //build("eq",3)
                tree_stack.build( "eq" , "Keyword", 2 );
        }
        else if ( current_token.get_token() == "ne" )
        {
                if(DEBUG)
                        std::cout<<endl<<"Bp -> A 'ne'A";
                tree_stack.read( current_token,  "ne" );
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_a();
                //read(current_token.get_token())
                //parse_a()
                //build("eq",3)
                tree_stack.build( "ne" , "Keyword", 2 );
        }

}


void Parser::parse_a()
{
        if ( current_token.get_token() == "+" )
        {
                if(DEBUG)
                        std::cout<<endl<<"A ->  + At";
                //read(current_token.get_token())
                tree_stack.read( current_token,  "+" );
                current_token = scanner.get_token();
                tree_stack.pop();
                //parse_a()
                //read +
                parse_at();
        }
        else if ( current_token.get_token() == "-" )
        {
                if(DEBUG)
                        std::cout<<endl<<"A -> -At";
                //read -
                tree_stack.read( current_token,  "-" );
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_at();
                //build (neg, 1)
                tree_stack.build( "neg" , "Keyword", 1 );
        }
        else
        {
                parse_at();
        }

        if(DEBUG)
                std::cout<<endl<<"A -> At";

        while ( current_token.get_token() == "+" || current_token.get_token() == "-"  )
        {
                if(DEBUG)
                        std::cout<<endl<<"A -> A + At || A -> A - At";
                //read current_token.get_token()
                std::string temp = current_token.get_token();
                tree_stack.read( current_token,  current_token.get_token() );
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_at();
                //build current_token.get_token() 2
                tree_stack.build( temp, "Operator", 2 );
        }

}

void Parser::parse_at()
{
        if(DEBUG)
                std::cout<<endl<<"At -> Af";
        parse_af();
        while ( current_token.get_token() == "*" || current_token.get_token() == "/"  )
        {
                if(DEBUG)
                        std::cout<<endl<<"At -> At * Af | At -> At / Af";
                //read current_token.get_token()
                //parse_af
                //build current_token.get_token() 2
                std::string temp = current_token.get_token();
                tree_stack.read( current_token,  current_token.get_token() );
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_af();
                //build current_token.get_token() 2
                tree_stack.build( temp, "Operator", 2 );


        }

}

void Parser::print_ast()
{
        tree_stack.Print();
}

void Parser::parse_af()
{
        if(DEBUG)
                std::cout<<endl<<"Af -> Ap";
        parse_ap();
        if ( current_token.get_token() == "**"  )
        {
                if(DEBUG)
                        std::cout<<endl<<"Af -> Ap ** Af";
                //read **
                //read_af
                //build ** 2
                std::string temp = current_token.get_token();
                tree_stack.read( current_token,  "**" );
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_af();
                //build current_token.get_token() 2
                tree_stack.build( temp, "Operator", 2 );

        }
}


void Parser::parse_ap()
{
        if(DEBUG)
                std::cout<<endl<<"Ap -> r";
        parse_r();
        while ( current_token.get_token() == "@"  )
        {
                if(DEBUG)
                        std::cout<<endl<<"Ap -> Ap @ <ID>  r";
                //read identifier
                //parse_r
                //build @ 3
                std::string temp = current_token.get_token();
                tree_stack.read( current_token,  "@" );
                current_token = scanner.get_token();
                tree_stack.pop();
                tree_stack.read( current_token,  "Identifier" );
                ////std::cout<<endl<<"Pushed onto stack"<<current_token.get_token();
                current_token = scanner.get_token();
                parse_r();
                //build current_token.get_token() 2
                tree_stack.build( "@", "Operator", 3 );

        }

}


void Parser::parse_vb()
{
        if ( current_token.get_token_type() == "Identifier" )
        {
                if(DEBUG)
                        std::cout<<endl<<"Vb -> '<IDENTIFIER>'";
                tree_stack.read(current_token, "Identifier");
                //std::cout<<endl<<"Pushed onto stack"<<current_token.get_token();
                current_token = scanner.get_token();
        }
        else if ( current_token.get_token() == "(" )
        {
                tree_stack.read(current_token, "(");
                current_token = scanner.get_token();
                tree_stack.pop();
                if( ")" == current_token.get_token() )
                {
                        //if(DEBUG)
                        //      std::cout<<endl<<"Vb -> '(' V1 ')'";
                        //tree_stack.pop();
                        tree_stack.read(current_token, ")");
                        current_token = scanner.get_token();
                        tree_stack.pop();
                        tree_stack.build("()", "Operator", 0);
                }

                else
                {
                        //if(DEBUG)
                        //      std::cout<<endl<<"Vb -> '(' ')'";
                        parse_vl();
                        //tree_stack.pop();
                        tree_stack.read(current_token, ")");
                        current_token = scanner.get_token();
                        tree_stack.pop();
                        //tree_stack.build("()", "Operator", 0);
                }
        }
}

void Parser::parse_vl()
{
        tree_stack.read(current_token, "Identifier");
        current_token = scanner.get_token();
        int id_count = 1;
        while( "," == current_token.get_token() )
        {
                tree_stack.read(current_token, ",");
                current_token = scanner.get_token();
                tree_stack.pop();
                tree_stack.read(current_token, "Identifier");
                //std::cout<<endl<<"Pushed onto stack"<<current_token.get_token();
                current_token = scanner.get_token();
                id_count++;
        }
        if(id_count >1)
                tree_stack.build(",", "Operator", id_count);
}

void Parser::parse_r()
{
        parse_rn();
        while(( current_token.get_token() == "true" )||( current_token.get_token() == "false" )||( current_token.get_token() == "(" )||( current_token.get_token() == "nil" )||( current_token.get_token() == "dummy" )||( current_token.get_token_type() == "Identifier" )||( current_token.get_token_type() == "Integer" )||( current_token.get_token_type() == "String" ))
        {
                parse_rn();
                tree_stack.build("gamma", "operator", 2);
        }
}

void Parser::parse_rn()
{
        if (( current_token.get_token_type() == "Identifier" )||( current_token.get_token_type() == "Integer" )||( current_token.get_token_type() == "String" ))
                {
                        if(DEBUG)
                                std::cout<<endl<<"R -> Rn";
                        tree_stack.read(current_token, current_token.get_token_type());
                        //std::cout<<endl<<"Pushed onto stack"<<current_token.get_token();
                        current_token = scanner.get_token();
                }
        else if("true" == current_token.get_token())
        {
                if(DEBUG)
                                        std::cout<<endl<<"Rn -> 'true'";
                tree_stack.read(current_token, "true");
                //std::cout<<endl<<"Pushed onto stack"<<current_token.get_token();
                current_token = scanner.get_token();
        }
        else if("false" == current_token.get_token())
        {
                if(DEBUG)
                                        std::cout<<endl<<"Rn -> 'false'";
                tree_stack.read(current_token, "false");
                //std::cout<<endl<<"Pushed onto stack"<<current_token.get_token();
                current_token = scanner.get_token();
        }
        else if("nil" == current_token.get_token())
        {
                if(DEBUG)
                                        std::cout<<endl<<"Rn -> 'nil'";
                tree_stack.read(current_token, "nil");
                //std::cout<<endl<<"Pushed onto stack"<<current_token.get_token();
                current_token = scanner.get_token();
        }
        else if("dummy" == current_token.get_token())
        {
                if(DEBUG)
                                        std::cout<<endl<<"Vb -> 'dummy'";
                tree_stack.read(current_token, "dummy");
                //std::cout<<endl<<"Pushed onto stack"<<current_token.get_token();
                current_token = scanner.get_token();
        }
        else if("(" == current_token.get_token())
        {
                if(DEBUG)
                                        std::cout<<endl<<"Rn -> '(' E ')'";
                //read(current_token, "(");
                tree_stack.read(current_token, "(");
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_e();
                //read(current_token, ")");
                tree_stack.read(current_token, ")");
                current_token = scanner.get_token();
                tree_stack.pop();
        }
}

void Parser::parse_d()
{
        parse_da();
        if ( current_token.get_token() == "within" )
        {
                if(DEBUG)
                        std::cout<<endl<<"D -> Da 'within' D";
                tree_stack.read(current_token, "within");
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_d();
                tree_stack.build("within", "Keyword", 2);
        }
}


void Parser::parse_da()
{
        if(DEBUG)
                std::cout<<endl<<"Da ->  Dr ( 'and' Dr)+";
        parse_dr();
        int count = 0;
        while("and" == current_token.get_token())
        {
        //      tree_stack.read();
                tree_stack.read(current_token, "and");
                current_token = scanner.get_token();
                tree_stack.pop();
        //      tree_stack.pop();
                parse_dr();
                count++;
        }
        if(count > 0)
                tree_stack.build("and", "Keyword", count + 1);
}

void Parser::parse_dr()
{
        if("rec" == current_token.get_token())
        {
                if(DEBUG)
                        std::cout<<endl<<"Dr -> 'rec' Db";
                tree_stack.read(current_token, "rec");
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_db();
                tree_stack.build("rec", "Keyword", 1);
        }
        else
        {
                if(DEBUG)
                        std::cout<<endl<<"Dr -> Db";
                parse_db();
        }
}

void Parser::parse_db()
{
        if("(" == current_token.get_token())
        {
                if(DEBUG)
                                        std::cout<<endl<<"Db -> '(' D ')'";
                tree_stack.read(current_token, "(");
                current_token = scanner.get_token();
                tree_stack.pop();
                parse_d();
                tree_stack.read(current_token, ")");
                current_token = scanner.get_token();
                tree_stack.pop();
        }
        else
        {
                parse_vl();
                int count = 1;
                if ( current_token.get_token() == "=" )
                {
                        tree_stack.read(current_token, "=");
                        current_token = scanner.get_token();
                        tree_stack.pop();
                        parse_e();
                        tree_stack.build("=", "Operator", 2);
                }
                else
                {
                        parse_vb();
                        count ++;
                        while ( current_token.get_token() != "="  && current_token.get_token() != "EOF")
                        {
                                parse_vb();
                                count ++;
                        }
                        tree_stack.read(current_token, "=");
                        current_token = scanner.get_token();
                        tree_stack.pop();
                        parse_e();
                        count ++;
                        tree_stack.build("fcn_form", "Operator", count );
                }
        }
        /*else if("Identifier" == current_token.get_token_type())
        {
                if(("," == scanner.peek_token().get_token())||("=" == scanner.peek_token().get_token()))
                {
                        if(DEBUG)
                                std::cout<<endl<<"Db -> Vl '=' E";
                        parse_vl();
                        tree_stack.read(current_token, "=");
                        current_token = scanner.get_token();
                        tree_stack.pop();
                        parse_e();
                        tree_stack.build("=", "Operator", 2);
                }
                else if("Identifier" == scanner.peek_token().get_token_type()||("(" ==current_token.get_token()) )
                {
                        if(DEBUG)
                                std::cout<<endl<<"Db -> '<IDENTIFIER>' Vb+ '=' E";
                        tree_stack.read(current_token, "Identifier");
                        //std::cout<<endl<<"Pushed onto stack"<<current_token.get_token();
                        current_token = scanner.get_token();
                        int vb_count = 0;
                        while(("Identifier" ==current_token.get_token_type())||("(" ==current_token.get_token()))
                        {
                                parse_vb();
                                vb_count++;
                        }
                        tree_stack.read(current_token, "=");
                        current_token = scanner.get_token();
                        tree_stack.pop();
                        parse_e();
                        tree_stack.build("fcn_form", "Operator", vb_count + 2);
                }
        }*/

}
