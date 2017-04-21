/*
 *      scanner.cc
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

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <assert.h>
#include <cctype>
#include "scanner.hh"
#include "token.hh"



/*
 *
 * name: Scanner()
 * @param file_name the name of the file to parse
 */

Scanner::Scanner(std::string file_name)
{
        this -> file_name = file_name;
        init();
}


void Scanner::init()
{
        input_file.open(file_name.c_str());

        if ( ! ( input_file.is_open() ) )
        {
                std::cout<<"Error: File "<<file_name<<" not found "<<std::endl;
                exit(1);
        }

        get_next_token(); //get a token and put it inside the buffer
}

void Scanner::print_file()
{
        assert(input_file.is_open());

        char ch;

        while( input_file.get(ch) )
        {
                std::cout<<ch;
        }
}

void Scanner::get_next_token()
{
        char token;
        assert(input_file.is_open());
        std::string token_type;
        int state  = START; //the state of the FSM
        while(true)
        {
                switch(state)
                {
                        case START:
                        {
                                current_token = "";
                                token = input_file.peek();

                                if (input_file.eof() )
                                {
                                        state = ENDOFFILE;
                                }
                                else if (is_alphabet(token))
                                {
                                        state = IDENTIFIER;
                                }
                                else if ( is_number(token) )
                                {
                                        state = INTEGER;
                                }
                                else if ( is_operator(token) )
                                {
                                        state = OPERATOR;
                                }
                                else if ( is_string(token) )
                                {
                                        state = STRING;
                                }
                                else if ( ' ' == token || '\n' == token || '\r' == token || '\t' == token )
                                {
                                        state = SPACES;
                                }
                                else if ( is_punction( token ) )
                                {
                                        state = PUNCTION;
                                }
                             /*else if ( token == '\n' )
                                {
                                        state = ENDOFFILE;
                                }
                          /*      else if ( '\\' == token )
                                {
                                        state = COMMENT;
                                }taken care of by the operator case*/
                                break;
                        }

                        case IDENTIFIER:
                        {
                                input_file.get(token);
                                current_token += token;
                                token = input_file.peek(); //should add some error checking code
                                if(is_alphabet(token) || is_number(token) || is_underscore(token) )
                                {
                                        state = IDENTIFIER;
                                }
                                else
                                {
                                        if ( is_keyword(current_token) )
                                        {
                                                Buffer = Token(current_token, "Keyword");
                                                state = END;
                                        }
                                        else
                                        {
                                                Buffer = Token(current_token,"Identifier");
                                                state = END;
                                        }
                                }
                                break;
                        }//if current token identifier, extend the current token while the next
                        // character is a number or an alphabet or an underscore

                        case INTEGER:
                        {
                                input_file.get(token);
                                current_token += token;
                                token = input_file.peek();

                                if (is_number(token))
                                {
                                        state = INTEGER;
                                }
                                else
                                {
                                        Buffer = Token(current_token, "Integer");
                                        state = END;
                                }
                                break;
                        }

                        case OPERATOR:
                        {
                                input_file.get(token);

                                if ( token == '/' && input_file.peek() == '/' )
                                {
                                        state = COMMENT;
                                        break;
                                }/*check if backslash followed by backslash
                                   in this case, we have a comment*/

                                current_token += token;
                                token = input_file.peek();

                                if (is_operator(token) )
                                {
                                        state = OPERATOR;
                                }
                                else
                                {
                                        Buffer = Token(current_token, "Operator");
                                        state = END;
                                }
                                break;
                        }

                        case STRING:
                        {
                                input_file.get(token); //consume current token

                                if ( ! is_string(token) ) //add current token to string
                                {                         //if not terminating sequence
                                        current_token += token;
                                }
                                token = input_file.peek(); //look at next token


                                if(is_string(token))
                                {
                                        input_file.get(token);
                                        Buffer = Token(current_token, "String");
                                        state = END;
                                } //if ending sequence  then we have our string
                                else if (is_string_symbol(token))
                                {
                                        state = STRING;
                                }//continue parsing string
                                else if ( '\\' == token )
                                {
                                        state = BACKSLASH;
                                }//check if valid backslash following character
                                else
                                {
                                        Token err("Expect \' at the end of a string","Error");
                                        Buffer = err;
                                        state = END;
                                }
                                break;
                        }

                        case BACKSLASH:
                        {
                                input_file.get(token);
                                current_token += token;
                                token = input_file.peek();

                                if(valid_following_backslash(token) )
                                {
                                        state = STRING;
                                }
                                else
                                {
                                        Token err("Invalid string token","Error");
                                        Buffer = err;
                                        state = END;
                                }
                                break;
                        }

                        case SPACES:
                        {
                                input_file.get(token);
                                token = input_file.peek();

                                if ( ' ' == token || '\t' == token || '\n' == token || '\r' == token)
                                {
                                        state = SPACES;
                                }
                                else
                                {
                                        state  = START;
                                }
                                break;
                        }

                        case ENDOFFILE:
                        {
                                Buffer = Token("EOF","EOF");
                                state = END;
                                break;
                        }

                        case PUNCTION:
                        {
                                input_file.get(token);
                                current_token = token;
                                Buffer = Token(current_token, "Punction");
                                state = END;
                                break;
                        }


                        case COMMENT:
                        {
                                input_file.get(token);
                                token = input_file.peek();

                                if ( valid_following_comment( token ) )
                                {
                                        state = COMMENT;
                                }
                                else if ( token == '\n' )
                                {
                                        input_file.get(token);
                                        state = START;
                                }
                                else
                                {
                                        Token err("Invalid comment termination","Error");
                                        Buffer = err;
                                        state = END;

                                }
                                break;

                        }

                        case END:
                        {
                                return;
                                assert(false);
                                break;
                        }

                        default:
                        {
                                Token err("Unspecified error","Error");
                                Buffer = err;
                                break;
                        }
        } //switch cases for the finite state machine
    }
        Token err("Unspecified error","Error");
        Buffer = err;
}


bool Scanner::valid_following_comment( char token )
{
        return (
                        /*token == '\''                 ||
                        token == '|'            ||
                        token == ';'            ||
                        token == ','            ||
                        token == '\\'           ||
                        token == '\t'           ||
                        token == ':'            ||
                        token == ' '            ||
                        is_alphabet(token)      ||
                        is_number(token)        ||
                        is_operator(token)*/
                        ! ( token == '\n' )
               );
}


bool Scanner::valid_following_backslash( char token )
{
        return (
                        token == 't'  ||
                        token == 'n'  ||
                        token == '\\' || //need to fix
                        token == '\''
               );
}

bool Scanner::is_alphabet(char token)
{
    return ( isalpha(token) );
}

bool Scanner::is_number(char token)
{
    return ( isdigit(token) );
}

/*
 *Checks if the parameter is a keyword
 * @return true if the token is a keyword
 * @param the token to check
 */
bool Scanner::is_keyword(std::string token)
{
        return (
                        token == "let" ||
                        token == "in"  ||
                        token == "fn"  ||
                        token == "where" ||
                        token == "aug" ||
                        token == "or" ||
                        token == "not" ||
                        token == "gr" ||
                        token == "ge" ||
                        token == "ls" ||
                        token == "le" ||
                        token == "eq" ||
                        token == "ne" ||
                        token == "true" ||
                        token == "false" ||
                        token == "nil" ||
                        token == "dummy" ||
                        token == "within" ||
                        token == "and" ||
                        token == "rec"
               );

}

bool Scanner::is_underscore(char token)
{
                if (token == '_')
                {
                        return true;
                }
                else
                {
                        return false;
                }
}


bool Scanner::is_punction( char token)
{
        return (
                        token == '(' ||
                        token == ')' ||
                        token == ';' ||
                        token == ','
               );
}
bool Scanner::is_string_symbol(char token)
{
        return (
                        token == '('||
                        token == ')'||
                        token == ';'||
                        token == ','||
                        token == ' '||
                        is_alphabet(token) ||
                        is_number(token) ||
                        is_operator(token)
               );
}

bool Scanner::is_operator(char token)
{
        return (
                        token == '+' ||
                        token == '-' ||
                        token == '*' ||
                        token == '<' ||
                        token == '>' ||
                        token == '&' ||
                        token == '.' ||
                        token == '@' ||
                        token == '/' ||
                        token == ':' ||
                        token == '=' ||
                        token == '~' ||
                        token == '|' ||
                        token == '$' ||
                        token == '!' ||
                        token == '#' ||
                        token == '%' ||
                        token == '^' ||
                        token == '_' ||
                        token == '[' ||
                        token == ']' ||
                        token == '{' ||
                        token == '}' ||
                        token == '?' ||
                        token == '\"' ||
                        token == '`'
                );
}


bool Scanner::is_string(char token)
{
        if (token == '\'')
        {
                return true;
        }
        else
        {
                return false;
        }
}

/**
 *This function returns the current token
 * and fetches a new token
 * @return The current token
 */
Token Scanner::get_token()
{
        Token temp(Buffer.get_token(), Buffer.get_token_type());
        get_next_token();
        return temp;
}

Token Scanner::peek_token()
{
        return Buffer;
}

