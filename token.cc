/*
 *      token.cc
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
#include "token.hh"

std::string Token::get_token()
{
        return token;
}

Token::Token(std::string token,std::string token_type)
{
        this -> token = token;
        this -> token_type = token_type;
}

Token::Token()
{
        this -> token = "Error";
        this -> token_type = "Error";
}

std::string Token::get_token_type()
{
        return token_type;
}
