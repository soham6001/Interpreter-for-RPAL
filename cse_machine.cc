#include <iostream>
#include <assert.h>
#include <cstdlib>
#include <string>
#include "cse_machine.hh"
#include "cse_nodes.hh"
#include "convert.hh"
//#include "environment.hh"

//todo: change the order of the code
//rename variables
int cse_machine::flatten_tree()//flatten the tree return 1 if successful
{
        while(!queue_nodes.empty())
        {
                delta* temp_delta = new delta(delta_no);
                deltas.push_back(*temp_delta);
                node* temp = queue_nodes.front();
                queue_nodes.pop();
                flatten(delta_no, temp, false);
                delta_no++;
        }
        return 1;
}

void cse_machine::flatten(int n, node * start, bool skip_right = false) // internal worker fn for flatten_tree
{
        bool skip_left = false; //whether a lamda was found
        if( !start )
        {
                return;
        }//check if the root is 0
        else if ( start->data->get_token() == "lambda" )
        {
                lambda * lambda_node = new lambda("lambda","lambda");
                lambda_node->k = current_index; //annotate the lamda node
                deltas[n].current_delta.push_front(lambda_node);
                current_index ++;
                if(start->child->data->get_token() == ",")//add all the children of the comma as parameters
                {
                        lambda_node->add_comma(start->child->child); //functions adds all the children of the comma
                }
                else
                {
                        lambda_node->add_param(start->child->data->get_token());
                }//lambda has only one child
                skip_left = true; //skip all the children of the lambda
                queue_nodes.push(start->child->sibling);//the child will be
                                                                                                                                                                                //processed in the
                                                                                                                                                                                //next iteration
        }
        else if ( start->data->get_token() == "->" )
        {
                beta * temp_beta = new beta();
                deltas[n].current_delta.push_front(temp_beta);
                flatten(n, start->child,true);
                node* temp = start->child->sibling->sibling;
                start->child->sibling->sibling = NULL;
                temp_beta->if_delta_num = queue_nodes.size() + 1;
                temp_beta->else_delta_num = queue_nodes.size() + 2;
                queue_nodes.push(start->child->sibling);
                queue_nodes.push(temp);
                skip_left = true;
        }
        else if (start->data->get_token() == "tau")
        {
                tau * tau_node = new tau("tau","tau");//add a tau node
                tau_node->set_num_elements(start->child);//set the number of children
                tau_node->tree_reference = start;
                deltas[n].current_delta.push_front(tau_node);
        }
        else
        {
                deltas[n].current_delta.push_front(start);
        }

        if(!skip_left)
        {
                flatten(n, start->child); //left child
        }
        if (!skip_right)
        {
        flatten(n, start->sibling); //right child
        }

}



void cse_machine::print()
{
        vector<delta>::iterator i;
        int k = 0;
        for ( i = deltas.begin(); i != deltas.end(); i++ )
        {
//              if ( k == 11 )
//              {
//                      std::cout<<k;
//              }
                std::cout<<" Delta "<<k<<" => "<<std::endl;
                list<node*>::reverse_iterator l;
                for ( l = (*i).current_delta.rbegin(); l != (*i).current_delta.rend(); l ++)
                {
                        if( (*l)->data->get_token() == "lambda" )
                        {
                                std::cout<<"<lambda ";
                                ((lambda*)(*l))->print();
                                std::cout<<" >";
                        }
                        else if( (*l)->data->get_token_type() == "Identifier" )
                        {
                                std::cout<<"<ID "<<(*l)->data->get_token()<<" >";
                        }
                        else if( (*l)->data->get_token_type() == "Integer" )
                        {
                                std::cout<<"<Integer "<<(*l)->data->get_token()<<" >";
                        }
                        else if( (*l)->data->get_token_type() == "String" )
                        {
                                std::cout<<"<String '"<<(*l)->data->get_token()<<"'>";
                        }
                        else
                        {
                        std::cout<<(*l)->data->get_token()<<" ";
                        }
                }
                std::cout<<std::endl;
                k ++ ;
        }
}



cse_machine::cse_machine(node* tree)
{
        currentEnvNo = 0;
        envCount = 1;
        this->root = tree;
        current_index = 1;
        delta_no = 0;
        //create e0
        environment* e0 = new environment(0,-1);
        env_map.insert(make_pair(0, e0)) ;
        //push e0 on the stack and on the control
        cse_control.push(e0);
        cse_stack.push(e0);
        //push an initially empty delta 0 on the stack
        cse_control.push(new delta_node(0));

        //for flattening the control expression
        queue_nodes.push(root);
        flatten_tree();
}

int cse_machine::run_machine()
{

        while (! cse_control.empty() )
        {
                node * current_node = cse_control.top(); //get the topmost control node
                cse_control.pop();

                if (current_node->data->get_token() == "Delta")
                {
                        //lookup the delta and push all the control structures on to the control
                        delta_node * temp = (delta_node *) current_node;
                        delta  temp_delta =     deltas[temp->delta_no];
                        list<node*>::reverse_iterator l;

                        for ( l = temp_delta.current_delta.rbegin(); l != temp_delta.current_delta.rend(); l ++)
                        {
                            cse_control.push(*l);
                        }//todo need to check if delta not found

                }
                else if (current_node->data->get_token() == "gamma" )
                {
                        node * current_stack = cse_stack.top();
                        cse_stack.pop();
                        //check if lambda on the stack
                        if ( current_stack->data->get_token() == "lambda" )
                        {
							     lambda * lclosure = (lambda *) current_stack;
                                 //create a new environment
                                 environment * new_env = new environment( envCount, lclosure->lambda_env_num );
                                 new_env->calling_env = currentEnvNo;
                                 list<std::string>::iterator i;
                                 for ( i = lclosure->params.begin(); i != lclosure->params.end(); i++)
                                 {
                                        std::string temp_string = *i;
                                        node * next_stack = cse_stack.top();
                                        cse_stack.pop();
                                        new_env->bindings.insert( make_pair( temp_string, next_stack ) );
                                 }
                                 env_map.insert( make_pair( envCount, new_env ) );
                                 cse_stack.push( new_env );
                                 cse_control.push( new_env );
                                 //push a delta_node on the stack so that it is fetched in
                                 // the next iteration
                                 delta_node * temp_delta = new delta_node( lclosure -> k );
                                 cse_control.push(temp_delta);
                                 ++ envCount;
                                 currentEnvNo = new_env->current_env;
                        }
						else if (current_stack->data->get_token() == "Y*" )
						{
							node * current_stack = cse_stack.top();
							cse_stack.pop();
							if ( current_stack->data->get_token() == "lambda" )
							{
								eta * temp_eta = new eta( (lambda *) current_stack ) ;
								cse_stack.push(temp_eta);
							}
							else 
							{
							std::cout<<"Expected lambda closure";
							exit(1);
							}
						}
						else if (current_stack->data->get_token() == "eta")
						{
							lambda * temp_closure = ((eta *) current_stack)->lambda_closure;
							cse_stack.push(temp_closure);
							cse_stack.push(new node("gamma","gamma"));
							cse_stack.push(new node("gamma","gamma"));
						}
                        else //the object is a simple node
                        {
                                if ( current_stack -> data -> get_token_type() == "Identifier" )
                                {
                                        if ( current_stack -> data -> get_token() == "Print" )
                                        {
                                                node * next_stack = cse_stack.top();
                                                cse_stack.pop(); //pop the item to be printed
                                                if ( next_stack -> data -> get_token_type() == "Integer" )
                                                {
                                                        std::cout<< next_stack->data->get_token() ;
                                                }
                                                else if ( next_stack->data->get_token_type() == "String" )
                                                {
                                                        print_string(next_stack -> data -> get_token() );
                                                }
                                                else if ( next_stack->data->get_token_type() == "Keyword" )
                                                {
                                                        std::cout<<next_stack->data->get_token();
                                                }
                                                else if ( next_stack->data->get_token_type() == "tuple" )
                                                {
                                                        tuple * temp_tau = (tuple *) next_stack;
                                                        temp_tau->print();
                                                }
												else if ( next_stack->data->get_token_type() == "tau" )
												{
													tau * t = (tau *) next_stack;
													t->print_tau();
												}
                                                else
                                                {//if it is a lambda, then print it, else print the node
                                                        lambda * lclosure = (lambda *) next_stack;
                                                        if (next_stack->data->get_token_type() == "lambda" )
                                                        {
                                                                std::cout<<"[lambda closure :";
                                                                lclosure->print();
                                                                std::cout<<"]";
                                                        }//to do format nicely
                                                        else
                                                        {
                                                                std::cout<< next_stack->data->get_token_type();
                                                        }
                                                }
                                                //push dummy onto the stack
                                                cse_stack.push ( (new node("dummy","dummy" )));
                                        }
                                        else if( current_stack -> data -> get_token() == "Order" )
                                        {
                                                node * next_stack = cse_stack.top();
                                                cse_stack.pop(); //pop the tuple
                                                if ( next_stack->data->get_token() == "nil" )
                                                {
                                                        node * temp = new node("Integer", "0");
                                                        cse_stack.push(temp);
                                                }//we have an empty tuple
                                                else if ( next_stack->data->get_token() == "tuple" )
                                                {
                                                        tuple * temp_tau = (tuple *) next_stack;
                                                        int tuple_size = temp_tau->elements.size();
                                                        //push an integer node containing the number of elements
                                                        //in the tuple
                                                        node * temp = new node("Integer", toString( tuple_size));
                                                        cse_stack.push(temp);
                                                }
                                                else
                                                {
                                                        std::cout<<"Illegal function: Order application";
                                                }
                                        }
                                        else if( current_stack -> data -> get_token() == "Stem" )
                                        {
                                                node * next_stack = cse_stack.top();
                                                cse_stack.pop(); //pop the string
                                                std::string temp_string = next_stack -> data -> get_token();
                                                node* temp_head = new node( "String", temp_string.substr(0, 2) );
                                                cse_stack.push(temp_head);
                                        }
                                        else if( current_stack -> data -> get_token() == "Stern" )
                                        {
                                                node * next_stack = cse_stack.top();
                                                cse_stack.pop(); //pop the string
                                                std::string temp_string = next_stack -> data -> get_token();
                                                node* temp_head = new node( "String", temp_string.substr(2, temp_string.length() - 2) );
                                                cse_stack.push(temp_head);
                                        }
                                        else if( current_stack -> data -> get_token() == "Conc" )
                                        {
                                                //needs two strings and two gammas
                                                if ( cse_control.top() -> data -> get_token_type() == "gamma" )
                                                {
                                                        node * first_string = cse_stack.top();
                                                    cse_stack.pop();
                                                    node * second_string = cse_stack.top();
                                                    cse_stack.pop();
                                                    //discard the gamma
                                                    cse_control.pop();
                                                    if ( first_string -> data -> get_token_type() == "String" &&
                                                        second_string -> data -> get_token_type() == "String" )
                                                    {
                                                        std::string temp_string = first_string->data->get_token();
                                                        temp_string = temp_string + second_string->data->get_token();
                                                        node * temp = new node("String",temp_string);
                                                        cse_stack.push(temp);
                                                    }
                                                    else
                                                    {
                                                        std::cout<<"Invalid input types for the conc operator";
                                                        exit(1);
                                                    }
                                                }
                                                else
                                                {
                                                        cse_stack.push(current_stack);
                                                }
                                        }
                                        else if( current_stack -> data -> get_token() == "Isinteger" )
                                        {
                                                node * next_stack = cse_stack.top();
                                                cse_stack.pop(); //pop the node
                                                if (next_stack -> data -> get_token_type() == "Integer")
                                                {
                                                        node * temp = new node("Identifier","true");
                                                        cse_stack.push(temp);
                                                }
                                                else
                                                {
                                                        node * temp = new node("Identifier","false");
                                                        cse_stack.push(temp);
                                                }
                                        }
                                        else if( current_stack -> data -> get_token() == "Istruthvalue" )
                                        {
                                                node * next_stack = cse_stack.top();
                                                cse_stack.pop(); //pop the node
                                                if (next_stack -> data -> get_token() == "true" || next_stack -> data -> get_token() == "false" )
                                                {
                                                        node * temp = new node("Identifier","true");
                                                        cse_stack.push(temp);
                                                }
                                                else
                                                {
                                                        node * temp = new node("Identifier","false");
                                                        cse_stack.push(temp);
                                                }
                                        }
                                        else if( current_stack -> data -> get_token() == "Isstring" )
                                        {
                                                node * next_stack = cse_stack.top();
                                                cse_stack.pop(); //pop the node
                                                if (next_stack -> data -> get_token_type() == "String")
                                                {
                                                        node * temp = new node("Identifier","true");
                                                        cse_stack.push(temp);
                                                }
                                                else
                                                {
                                                        node * temp = new node("Identifier","false");
                                                        cse_stack.push(temp);
                                                }
                                        }
                                        else if( current_stack -> data -> get_token() == "Istuple" )
                                        {
                                                node * next_stack = cse_stack.top();
                                                cse_stack.pop(); //pop the node
                                                if (next_stack -> data -> get_token_type() == "tuple")
                                                {
                                                        node * temp = new node("Identifier","true");
                                                        cse_stack.push(temp);
                                                }
                                                else
                                                {
                                                        node * temp = new node("Identifier","false");
                                                        cse_stack.push(temp);
                                                }
                                        }
                                        else if( current_stack -> data -> get_token() == "Isfunction" )
                                        {
                                                node * next_stack = cse_stack.top();
                                                cse_stack.pop(); //pop the node
                                                if (next_stack -> data -> get_token_type() == "lambda")
                                                {
                                                        node * temp = new node("Identifier","true");
                                                        cse_stack.push(temp);
                                                }
                                                else
                                                {
                                                        node * temp = new node("Identifier","false");
                                                        cse_stack.push(temp);
                                                }
                                        }
                                        else if( current_stack -> data -> get_token() == "Isdummy" )
                                        {
                                                node * next_stack = cse_stack.top();
                                                cse_stack.pop(); //pop the node
                                                if (next_stack -> data -> get_token() == "dummy")
                                                {
                                                        node * temp = new node("Identifier","true");
                                                        cse_stack.push(temp);
                                                }
                                                else

                                                {
                                                        node * temp = new node("Identifier","false");
                                                        cse_stack.push(temp);
                                                }
                                        }
                                        else if( current_stack -> data -> get_token() == "ItoS" )
                                        {
                                                node * next_stack = cse_stack.top();
                                                cse_stack.pop(); //pop the node
                                                node * int_node = new node( "Integer", next_stack->data->get_token() );
                                                cse_stack.push(int_node);
                                        }
                                        else if( current_stack -> data -> get_token_type() == "Keyword" )
                                        {
                                                std::cout<<"Invalid application of Keyword";
                                                exit(1);
                                        }
                                        else if ( current_stack -> data -> get_token_type() == "tuple" )
                                        {
                                                tuple * temp_tau = (tuple*) current_stack;
                                                node * next_stack = cse_stack.top();
                                                cse_stack.pop(); //pop the node
                                                int temp_element = toInt( next_stack -> data -> get_token() );
                                                node* temp_node = temp_tau -> get_element(temp_element);
                                                cse_stack.push(temp_node);
                                        }


                                }

                        }

                }
                else if (current_node->data->get_token() == "Environment" )
                {
                        if ( !cse_stack.empty() )
						{
						node * current_stack = cse_stack.top();
                        cse_stack.pop();
                        stack<node *> temp_stack;

                        while (!cse_stack.empty() && current_stack ->data -> get_token_type() != "Environment" )
                        {
                                cse_stack.pop();
                                temp_stack.push(current_stack);
                                if (!cse_stack.empty())
								current_stack = cse_stack.top();
                        }
						if(!cse_stack.empty())
					    cse_stack.pop();

                        while ( !temp_stack.empty() )
                        {
                                cse_stack.push( temp_stack.top() );
                                temp_stack.pop();
                        }
                        environment * temp_env = (environment *) current_node;
                        map<int, environment *>::iterator i = env_map.find(temp_env -> current_env );
                        currentEnvNo = i->second->calling_env;
					}
				}
                else if (current_node->data->get_token() == "lambda" )
                {//make the lambda a lambda closure and push it on the control
				        lambda * temp_lambda = (lambda *) current_node;
                        temp_lambda -> lambda_env_num = currentEnvNo;
						cse_stack.push(current_node);
                }
			
                else if (current_node->data->get_token() == "beta" )
                {
                        node * current_stack = cse_stack.top();
                        cse_stack.pop();

                        if ( current_stack -> data -> get_token() == "true" )
                        {
                                beta * temp_beta = (beta *) current_node;
                                delta_node * temp_delta = new delta_node(temp_beta->if_delta_num);
                                //temp_delta->delta_no = current_node->if_delta_num;
                        }
                        else if ( current_stack -> data -> get_token() == "false" )
                        {
                                beta * temp_beta = (beta *) current_node;
                                delta_node * temp_delta = new delta_node(temp_beta->else_delta_num);
                                //temp_delta->delta_no = current_node->else_delta_num;
                        }
                }
				else if ( current_node->data->get_token() == "eta" )
				{
					cse_stack.push(current_node);
				}
                else // Process element as node
                {
                        switch (getNodeTypeId( (current_node->data->get_token_type()) ))
                        {
                        case ID:
                        {
                                // Find ID value in current and parent environments
                                bool valFound = false;
                                map<int, environment *>::iterator ite_map = env_map.find(currentEnvNo);

                                while (ite_map != env_map.end() && !valFound)
                                {
                                        map <string, node *>::iterator i;
                                        for(i = ite_map->second->bindings.begin(); i != ite_map->second->bindings.end(); i++)
                                        {
                                                if(i->first == current_node->data->get_token())
                                                {
                                                        valFound = true;
                                                        cse_stack.push(i->second);
                                                }
                                        }
                                        if(!valFound)
                                        {
                                                ite_map = env_map.find(ite_map->second->parent_env);
                                        }
                                }
                        }
                        break;

                        case INT:
                        {
                                cse_stack.push(current_node);
                                                 }
                        break;
                        case STR:
                        {
                                cse_stack.push(current_node);
                        }
                        break;
                        case OPERATOR:
                        {
                                // Do the operations

                                switch (getRuleTypeId(current_node->data->get_token()))
                                {
                                case e_unary:
                                {
                                        if ("neg" == current_node->data->get_token())
                                        {
                                                node *stackElem = cse_stack.top();
                                                cse_stack.pop();
                                                int value = toInt(stackElem->data->get_token());
                                                stackElem->data->token = toString(- value);
                                                cse_stack.push(stackElem);
                                        }
                                        else if (current_node->data->get_token() == "not")
                                        {
                                                node *stackElem = cse_stack.top();
                                                cse_stack.pop();
                                                // Negate logical value held by node data
                                                if(!stackElem->data->get_token().compare("true"))
                                                        stackElem->data->token = "false";
                                                stackElem->data->token = "true";
                                                cse_stack.push(stackElem);
                                        }
                                }
                                break;
                                case e_binary:
                                                {
                                                        string argA = cse_stack.top()->data->get_token();
                                                        cse_stack.pop();
                                                        string argB = cse_stack.top()->data->get_token();
                                                        cse_stack.pop();
                                                        string result = executeBinaryOp(current_node->data->get_token(), argA, argB);

                                                        //clsControlElement objStackElem;
                                                        node* stackElem = new node();
                                                        stackElem->data->token = result;
                                                        cse_stack.push(stackElem);
                                                }
                                                break;
                                case e_tau:
                                {
                                        list <node *> tupleElems;
                                        //string retTup = "(";
                                        tau *tmp = (tau *)current_node;
                                        for (int i = 0; i < tmp->num_elements; i++)
                                        {
                                                node *stackElem = cse_stack.top();
                                                cse_stack.pop();
                                                //if (objStackElem.mControlType == clsControlElement::C_NODE)
                                                //string val = stackElem.data->get_token();
                                                tupleElems.push_back(stackElem);


                                                //if (i != (tmp.num_elements - 1))
                                        //              retTup += ",";
                                        }
                                        //retTup += ")";
                                        tuple * newStackElem = new tuple(tupleElems);
                                        //newStackElem.data->Token(retTup, "");
                                        cse_stack.push(newStackElem);
                                }
                                break;
                                default:
                                {
                                        cout << "Exiting wrong operator " << endl;
                                        exit(1);
                                }
                                }
                        }
                        break;

                        case KEYWORD:
                        {
                                cse_stack.push(current_node);
                        }
                        break;

                        default:
                                cse_stack.push(current_node);
                                break;
                        }
                }
        }
		std::cout<<std::endl;
        return 0;
}



int cse_machine::getRuleTypeId(string arg)
{
    int val = e_unknown;

    if (arg == "let")
        val = e_let;
    else if (arg == "where")
        val = e_where;
    else if (arg == "within")
        val = e_within;
    else if (arg == "gamma")
        val = e_gamma;
    else if (arg == "lambda")
        val = e_lambda;
    else if ( arg == "+" || arg == "-" || arg == "*" || arg == "**" || arg == "/" ||
              arg == "or" || arg == "eq" || arg == "&" || arg == "aug" ||
              arg == "gr" || arg == "ge" || arg == "ls" || arg == "le" || arg == "ne" )
        val = e_binary;
    else if (arg == "neg" || arg == "not" )
        val = e_unary;
    else if (arg == "=")
        val = e_assignment;
    else if (arg == "and")
        val = e_and;
    else if (arg == "tau")
        val = e_tau;
    else if (arg == "function_form")
        val = e_functionform;
    else if (arg == "rec")
        val = e_rec;
    else if (arg == ",")
        val = e_comma;
    else if (arg == "@")
        val = e_at;
    else if (arg == "->")
        val = e_cond;

    return val;
}


int cse_machine::getNodeTypeId(string arg)
{
    if (arg == "Identifier")
        return ID;
    else if (arg == "Integer")
        return INT;
    else if (arg == "String")
        return STR;
    else if (arg == "tuple")
        return TUPLE;
    else if (arg == "Operator")
        return OPERATOR;
    else if (arg == "Integer")
        return KEYWORD;
    else
        return UNKNOWN;
}

string cse_machine::executeBinaryOp(string Op, string argA, string argB)
{
    if (Op == "+")
    {
        int i_argA = toInt(argA);
        int i_argB = toInt(argB);
        return toString(i_argA + i_argB);
    }
    else if (Op == "-")
    {
        int i_argA = toInt(argA);
        int i_argB = toInt(argB);
        return toString(i_argA - i_argB);
    }
    else if(Op == "*")
    {
        int i_argA = toInt(argA);
        int i_argB = toInt(argB);
        return toString(i_argA * i_argB);
    }
    else if (Op == "**")
    {
        int i_argA = toInt(argA);
        int i_argB = toInt(argB);
        int val = 1;
        for (int i = 0; i < i_argB; i++)
            val = val * i_argA;
        return toString(val);
    }
    else if (Op == "/")
    {
        int i_argA = toInt(argA);
        int i_argB = toInt(argB);
        return toString(i_argA / i_argB);
    }
    else if (Op == "or")
    {
        bool b_argA = false, b_argB = false;
        if (argA == "<true>")
            b_argA = true;
        else
            b_argA = false;
        if (argB == "<true>")
            b_argB = true;
        else
            b_argB = false;
        if (b_argA || b_argB)
            return "<true>";
        else
            return "<false>";
    }
    else if (Op == "eq")
    {
        if (argA == argB)
            return "<true>";
        else
            return "<false>";
    }
    else if (Op == "&")
    {
        bool b_argA = false, b_argB = false;
        if (argA == "<true>")
            b_argA = true;
        else
            b_argA = false;
        if (argB == "<true>")
            b_argB = true;
        else
            b_argB = false;
        if (b_argA && b_argB)
            return "<true>";
        else
            return "<false>";
    }
    else if (Op == "aug")
    {
        string retTup = "";
        if (argA == "<nil>")
                retTup = "(" + argB + ")";
        else if (argA[0] == '(')
                retTup = argA.substr(0, argA.length() - 1) + "," + argB + ")";
        else
        {
            cout << "Error in aug input " << endl;
            exit(1);
        }
        return retTup;
    }
    else if (Op == "gr")
    {
        int i_argA = toInt(argA);
        int i_argB = toInt(argB);
        if (i_argA > i_argB)
            return "<true>";
        else
            return "<false>";
    }
    else if (Op == "ge")
    {
        int i_argA = toInt(argA);
        int i_argB = toInt(argB);
        if (i_argA >= i_argB)
            return "<true>";
        else
            return "<false>";
    }
    else if (Op == "ls")
    {
        int i_argA = toInt(argA);
        int i_argB = toInt(argB);
        if (i_argA < i_argB)
            return "<true>";
        else
            return "<false>";
    }
    else if (Op == "le")
    {
        int i_argA = toInt(argA);
        int i_argB = toInt(argB);
        if (i_argA <= i_argB)
            return "<true>";
        else
            return "<false>";
    }
    else if (Op == "ne" )
    {
        if (argA != argB)
            return "<true>";
        else
            return "<false>";
    }
    else
    {
        cout << " Wrong binary operation " << Op << " exiting ..." << endl;
        exit(1);
    }




        return 0;
}

void cse_machine::print_string(std::string s) //print out a string
{
        int i = 0;
        while ( i < s.size() )
        {
                if ( s[i] == '\\' )
                {
                        if ( s[i+1] == 'n' )
                        {
                                std::cout<<"\n";
                                i += 2;
                        }
                        else if ( s[i+1] == 't' )
                        {
                                std::cout<<"\t";
                                i += 2;
                        }
                        else
                        {
                                std::cout<<s[i];
                                i++;
                        }
                }
                else
                {
                        std::cout<<s[i];
                        i++;
                }

        }
}
