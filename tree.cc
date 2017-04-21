/*
 * tree.cc
 *
 *  Created on: Mar 14, 2009
 *      Author: Saurabh
 */


#include "tree.hh"
#include <iostream>
#include "token.hh"
#include <stack>
#include <cstdlib>

static std::stack <node *>treeStack;
static int stackCount;

node::node()
{
                                //data = node_data;
                                data = new Token("", "");
                                child = NULL;
                                sibling = NULL;
}

node::node(string tok_data, string tok_type)
{
        //data = node_data;
        data = new Token(tok_data, tok_type);
        child = NULL;
        sibling = NULL;
}

tree::tree(Scanner* current_scanner) {
        this->scanner = current_scanner;
        Root = NULL;
}

tree::~tree() {
        // TODO Auto-generated destructor stub
}


void tree::build(string token_data, string token_type, int iChildren)
{
        // Pops iChildren number of trees from stack and merges them as siblings, with sData node as root
        // Then pushes this new tree onto stack
        //std::cout<<endl<<"Building tree with root"<<token_data;

    if(stackCount < iChildren)
    	return;

	node * tmpRoot = new node(token_data, token_type);
        for(int i=0; i<iChildren; i++)
        {
               // if(treeStack.empty())
               // {
                        //std::cout<<"stack still empty for "<<iChildren<<std::endl;
                        //exit(0);
                //return;
                //}
                node * t1 = treeStack.top();
                treeStack.pop();
                stackCount--;

                node * tmp_child = tmpRoot->child;
                tmpRoot->child = t1;
                t1->sibling = tmp_child;
        }
        treeStack.push(tmpRoot);
        stackCount++;
        Root = tmpRoot;
}


void tree::Push(Token tok)
{
        // Pushes a new node onto tree stack
        node * newChild = new node(tok.get_token(), tok.get_token_type());
        if(Root == NULL)
                Root = newChild;
        treeStack.push(newChild);
        stackCount++;
}

void tree::read(Token token, string expected_data)
{
        if (expected_data == "Identifier" || expected_data == "String" || expected_data == "Integer" )
        {
                if(expected_data == token.get_token_type())
                {
                        tree::Push(token);
                        //scanner->get_token();
                }
                else
                {
                std::cout<<std::endl<< "Erroneous token found at: "+ token.get_token();
                std::cout<<std::endl<<"Expected "<<expected_data;
                exit(0);
                }
        }

        else if(expected_data == token.get_token())
        {
                tree::Push(token);
                //scanner->get_token();
        }
        else
        {
                std::cout<<"Erroneous token found at: "+ token.get_token();
                std::cout<<std::endl<<"Expected "<<expected_data;
                exit(0);
        }
}

void tree::Print()
{
        //std::cout<<"FINAL OUTPUT"<<endl;
        PrintTree(Root);
}
int depth =0;
void tree::PrintTree(node * root)
{
        // Prints nodes in Preorder traversal
        if(root == NULL)
                return;

        for(int i=0; i< depth; i++)
                cout<<".";
        if(root->data->get_token_type()== "Identifier")
                cout<<"<ID:"<<root->data->get_token()<<"> \n";
        else if(root->data->get_token_type()== "Integer")
                cout<<"<INT:"<<root->data->get_token()<<"> \n";
        else if(root->data->get_token_type()== "String")
                cout<<"<STR:'"<<root->data->get_token()<<"'> \n";
        else if(root->data->get_token()== "nil")
                cout<<"<nil> \n";
        else if(root->data->get_token()== "fcn_form")
                cout<<"function_form \n";
        else if(root->data->get_token()== "nil" || root->data->get_token()== "dummy" || root->data->get_token()== "true" || root->data->get_token()== "false")
                cout<<"<"<<root->data->get_token()<<"> \n";

        else cout<<root->data->get_token()<<" \n";

                //root->data->get_token()<<">"<<endl;
        node* tmp = root->child;
        while(tmp)
        {
                depth++;
                PrintTree(tmp);
                tmp = tmp->sibling;
                depth--;
        }
}

node* tree::pop()
{
        node* tmp = treeStack.top();
        treeStack.pop();
        stackCount--;
        return tmp;
}
