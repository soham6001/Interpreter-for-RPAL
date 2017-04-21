/*
 * tree.hh
 *
 *  Created on: Mar 14, 2009
 *      Author: Saurabh
 */


#ifndef TREE_H_
#define TREE_H_
#include <iostream>
#include <stack>
#include "token.hh"
#include "scanner.hh"

using namespace std;


class node{
public:
        int k, i;
        Token * data;
        node * child;
        node * sibling;
        node();
        node(string tok_data, string tok_type);
        bool stdDone;
};



class tree {

private:
        Scanner* scanner;
        //static int stackCount;

public:
        node * Root;
        tree(Scanner*);
        virtual ~tree();
        void build(string token_data, string token_type, int count);
        void Push(Token tok);
        void read(Token token, string token_type);
        void Print();
        void PrintTree(node * root);
        node* pop();
};

#endif /* TREE_H_ */
