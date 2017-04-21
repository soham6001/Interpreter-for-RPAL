#include "tree.hh"
#include "StandardizeAST.hh"
#include<cstdlib>

void StandardizeAST::Print()
{
                                PrintTree(&treeNode);
}
int d = 0; //depth
void StandardizeAST::PrintTree(node * root)
{
        // Prints nodes in Preorder traversal
        if(root == NULL)
                return;

        for(int i=0; i< d; i++)
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
                d++;
                PrintTree(tmp);
                tmp = tmp->sibling;
                d--;
        }
}
int StandardizeAST::getRuleId(string szRule)
{
    int ruleId = R_unknown;

    if (szRule == "let")
        ruleId = R_let;
    else if (szRule == "where")
        ruleId = R_where;
    else if (szRule == "within")
        ruleId = R_within;
    else if (szRule == "gamma")
        ruleId = R_gamma;
    else if (szRule == "lambda")
        ruleId = R_lambda;
    else if ( szRule == "+" || szRule == "-" || szRule == "*" || szRule == "**" || szRule == "/" ||
              szRule == "or" || szRule == "eq" || szRule == "&" || szRule == "aug" ||
              szRule == "gr" || szRule == "ge" || szRule == "ls" || szRule == "le" || szRule == "ne" )
        ruleId = R_binaryop;
    else if (szRule == "neg" || szRule == "not" )
        ruleId = R_unaryop;
    else if (szRule == "=")
        ruleId = R_assignment;
    else if (szRule == "and")
        ruleId = R_and;
    else if (szRule == "tau")
        ruleId = R_tau;
    else if (szRule == "fcn_form")
        ruleId = R_functionform;
    else if (szRule == "rec")
        ruleId = R_rec;
    else if (szRule == ",")
        ruleId = R_comma;
    else if (szRule == "@")
        ruleId = R_at;
    else if (szRule == "->")
        ruleId = R_cond;

    return ruleId;
}

node& StandardizeAST::standardize()
{
    CallAptFunc(treeNode);
    treeNode.stdDone = true;
    return treeNode;
}

void StandardizeAST::CallAptFunc(node& tree_node)
{
    if (tree_node.stdDone)
        return;

    switch (getRuleId(tree_node.data->get_token()))
    {
    case R_let:
        mLetConversion(tree_node);
        break;
    case R_where:
        mWhereConversion(tree_node);
        break;
    case R_within:
        mWithinConversion(tree_node);
        break;
    case R_tau:
        mTauConversion(tree_node);
        break;
    case R_comma:
        mCommaConversion(tree_node);
        break;
    case R_cond:
        mCondConversion(tree_node);
        break;
    case R_binaryop:
        mBinaryOperatorConversion(tree_node);
        break;
    case R_unaryop:
        mUniaryOperatorConversion(tree_node);
        break;
    case R_and:
        mAndConversion(tree_node);
        break;
    case R_rec:
        mRecConversion(tree_node);
        break;
    case R_functionform:
        mFunctionFormConversion(tree_node);
        break;
    case R_lambda:
        mLambdaConversion(tree_node);
        break;
    case R_at:
        mAtConversion(tree_node);
        break;
    case R_assignment:
        mAssignmentConversion(tree_node);
        break;
    default:
        {
            if (tree_node.child)
            {
                CallAptFunc(*(tree_node.child));
                tree_node.child->stdDone = true;
            }
            if (tree_node.sibling)
            {
                CallAptFunc(*(tree_node.sibling));
                tree_node.sibling->stdDone = true;
            }
        };
    }
}

void StandardizeAST::mLetConversion(node& tree_node)
{
        if (getRuleId(tree_node.data->get_token()) == R_let)
    {
        if ((tree_node.child) && (tree_node.child->sibling))
        {
            CallAptFunc(*(tree_node.child));
            tree_node.child->stdDone = true;
            CallAptFunc(*(tree_node.child->sibling));
            tree_node.child->sibling->stdDone = true;
                        //TO DO IF FIRST CHILD NOT = ERROR
                        tree_node.data->token = "gamma";
                        tree_node.child->data->token = "lambda";
                        node * tmpNode = tree_node.child->sibling;
                        tree_node.child->sibling = tree_node.child->child->sibling;
                        tree_node.child->child->sibling = tmpNode;

                        tree_node.stdDone = true;
        }
    }
}

void StandardizeAST::mWhereConversion(node& tree_node)
{
        if (getRuleId(tree_node.data->get_token()) == R_where)
    {
        if ((tree_node.child) && (tree_node.child->sibling))
        {
            //convert to Let
                                        node * tmp_nodeeq = tree_node.child->sibling;
                                        node * tmp_nodeP = tree_node.child;
                                        tree_node.child = tmp_nodeeq;
                                        tree_node.child->sibling = tmp_nodeP;
                                        tmp_nodeP->sibling = NULL;

                                        CallAptFunc(*(tree_node.child));
          tree_node.child->stdDone = true;
          CallAptFunc(*(tree_node.child->sibling));
          tree_node.child->sibling->stdDone = true;
                                        //TO DO IF FIRST CHILD NOT = ERROR
                                        tree_node.data->token = "gamma";
                                        tree_node.child->data->token = "lambda";
                                        node * tmpNode = tree_node.child->sibling;
                                        tree_node.child->sibling = tree_node.child->child->sibling;
                                        tree_node.child->child->sibling = tmpNode;

                                        tree_node.stdDone = true;
        }


    }
}

void StandardizeAST::mTauConversion(node& tree_node)
{
        if (getRuleId(tree_node.data->get_token()) == R_tau)
    {
        if (tree_node.child)
        {
            CallAptFunc(*(tree_node.child));
            tree_node.child->stdDone = true;
        }
                node * ptempTreeNode = tree_node.child;
        while ((ptempTreeNode = ptempTreeNode->sibling))
        {
            CallAptFunc(*ptempTreeNode);
            ptempTreeNode->stdDone = true;

        }
    }
        tree_node.stdDone = true;
        return;
}

void StandardizeAST::mCondConversion(node& tree_node)
{
    if (getRuleId(tree_node.data->get_token()) == R_cond)
    {
        if (tree_node.child)
        {
            CallAptFunc(*(tree_node.child));
            tree_node.child->stdDone = true;
        }

        if (tree_node.child->sibling)
        {
            CallAptFunc(*(tree_node.child->sibling));
            tree_node.child->sibling->stdDone = true;
        }

        if (tree_node.child->sibling->sibling)
        {
            CallAptFunc(*(tree_node.child->sibling->sibling));
            tree_node.child->sibling->sibling->stdDone = true;
        }
    }
        tree_node.stdDone = true;
        return;
}

void StandardizeAST::mUniaryOperatorConversion(node& tree_node)
{
    if (getRuleId(tree_node.data->get_token()) == R_unaryop)
    {
        if (tree_node.child)
        {
            CallAptFunc(*(tree_node.child));
            tree_node.child->stdDone = true;
        }
        tree_node.stdDone = true;
    }
}

void StandardizeAST::mWithinConversion(node& tree_node)
{
    if (getRuleId(tree_node.data->get_token()) == R_within)
    {
        if (tree_node.child)
        {
            CallAptFunc(*(tree_node.child));
            tree_node.child->stdDone = true;
        }
                node * ptempTreeNode = tree_node.child;
        while ((ptempTreeNode = ptempTreeNode->sibling))
        {
            CallAptFunc(*ptempTreeNode);
            ptempTreeNode->stdDone = true;

        }

                node * X1,*X2,*E1,*E2,*eq1,*eq2;
                eq1 = tree_node.child;
                eq2 = eq1->sibling;
                X2 = eq2->child;
                E2 = X2->sibling;
                X1 = eq1->child;
                E1 = X1->sibling;

        tree_node.data->token = "=";
                eq1->data->token = "lambda";
                X1->sibling = E2;
                tree_node.child = X2;
                X2->sibling = eq2;
                eq2->data->token = "gamma";
                //eq2->sibling = eq1;
                eq2->child = eq1;
                eq1->sibling = E1;
                tree_node.stdDone = true;
    }
}

void StandardizeAST::mRecConversion(node& tree_node)
{
        if (getRuleId(tree_node.data->get_token()) == R_rec)
    {
        if (tree_node.child)
        {
            CallAptFunc(*(tree_node.child));
            tree_node.child->stdDone = true;
        }
                node * ptempTreeNode = tree_node.child;
        while ((ptempTreeNode = ptempTreeNode->sibling))
        {
            CallAptFunc(*ptempTreeNode);
            ptempTreeNode->stdDone = true;

        }

                node * newX,*gamma,*Y,*lambda;
                lambda = tree_node.child;
                lambda->data->token = "lambda";

                newX = new node();
                gamma = new node();
                Y = new node();
                if (!(newX->data))
                                                return;
                newX->data->token = lambda->child->data->get_token();
                newX->child = NULL;
                newX ->sibling = gamma;

                gamma->data->token = "gamma";
                gamma->sibling = NULL;
                gamma->child = Y;

                Y->data->token = "Y*";
				Y->data->token_type = "Y*";
                Y->child = NULL;
                Y->sibling = lambda;

                tree_node.data->token = "=";
                tree_node.child = newX;
                tree_node.stdDone = true;
    }
}

void StandardizeAST::mFunctionFormConversion(node& tree_node)
{
        if (getRuleId(tree_node.data->get_token()) == R_functionform)
    {
        if (tree_node.child)
        {
            CallAptFunc(*(tree_node.child));
            tree_node.child->stdDone = true;
        }
                int numV =-1;
                node * ptempTreeNode = tree_node.child;
        while ((ptempTreeNode = ptempTreeNode->sibling))
        {
            CallAptFunc(*ptempTreeNode);
            ptempTreeNode->stdDone = true;
                        numV++;

        }
                if(numV <0){
                        cout<<"Error";
                        exit(1);
                }
                tree_node.data->token = "=";
                ptempTreeNode = tree_node.child;
                node * prev = tree_node.child;
        while ((prev=ptempTreeNode )&& (ptempTreeNode = ptempTreeNode->sibling))
        {
                        if(ptempTreeNode->sibling){

                                node * lambda = new node();
                                lambda->data->token ="lambda";
                                prev->sibling =lambda;
                                lambda->child = ptempTreeNode;
                        }
                        else
                        {
                                break;
                        }

        }

                tree_node.stdDone = true;
    }
}

void StandardizeAST::mLambdaConversion(node& tree_node)
{
        if (getRuleId(tree_node.data->get_token()) == R_lambda)
    {
        if (tree_node.child)
        {
            CallAptFunc(*(tree_node.child));
            tree_node.child->stdDone = true;
        }
                int numV =0;
                node * ptempTreeNode = tree_node.child;
        while ((ptempTreeNode = ptempTreeNode->sibling))
        {
            CallAptFunc(*ptempTreeNode);
            ptempTreeNode->stdDone = true;
                        numV++;


        }
                if(numV <0){
                        tree_node.stdDone = true;
                        return;
                }

                ptempTreeNode = tree_node.child;
                node * prev = tree_node.child;
        while ((prev=ptempTreeNode )&& (ptempTreeNode = ptempTreeNode->sibling))
        {
                        if(ptempTreeNode->sibling){

                                node * lambda = new node();
                                lambda->data->token ="lambda";
                                prev->sibling =lambda;
                                lambda->child = ptempTreeNode;
                        }
                        else
                        {
                                break;
                        }

        }

                tree_node.stdDone = true;
    }
}

void StandardizeAST::mCommaConversion(node& tree_node)
{
        if (getRuleId(tree_node.data->get_token()) == R_comma)
    {
        if (tree_node.child)
        {
            CallAptFunc(*(tree_node.child));
            tree_node.child->stdDone = true;
        }
                int numV =1;
                node * ptempTreeNode = tree_node.child;
        while ( ptempTreeNode && (ptempTreeNode = ptempTreeNode->sibling))
        {
            CallAptFunc(*ptempTreeNode);
            ptempTreeNode->stdDone = true;
                        numV++;

        }

                tree_node.stdDone = true;
    }
}

void StandardizeAST::mBinaryOperatorConversion(node& tree_node)
{
    if (getRuleId(tree_node.data->get_token()) == R_binaryop)
    {
        if (tree_node.child)
        {
            CallAptFunc(*(tree_node.child));
            tree_node.child->stdDone = true;
        }

        if (tree_node.child && tree_node.child->sibling)
        {
            CallAptFunc(*(tree_node.child->sibling));
            tree_node.child->sibling->stdDone = true;
        }

        tree_node.stdDone = true;
    }
}

void StandardizeAST::mAtConversion(node& tree_node)
{
        if (getRuleId(tree_node.data->get_token()) == R_at)
    {
        if (tree_node.child)
        {
            CallAptFunc(*(tree_node.child));
            tree_node.child->stdDone = true;
        }
                int numV =1;
                node * ptempTreeNode = tree_node.child;
        while ((ptempTreeNode = ptempTreeNode->sibling))
        {
            CallAptFunc(*ptempTreeNode);
            ptempTreeNode->stdDone = true;
                        numV++;

        }
                if(numV !=3){
                        tree_node.stdDone = true;
                        cout<< "@ Error";
                        return;
                }

                node * gamma = new node[1];
                gamma->data->token ="gamma";
                node * E1,*N,*E2;

                E1 = tree_node.child;
                N = E1->sibling;
                E2 = N->sibling;
                tree_node.data->token = "gamma";
                tree_node.child = gamma;
                gamma->sibling = E2;
                gamma->child = N;
                N->sibling =E1;
                E1->sibling = NULL;

                tree_node.stdDone = true;
    }
}

void StandardizeAST::mAndConversion(node& tree_node)
{
        if (getRuleId(tree_node.data->get_token()) == R_and)
    {
        if (tree_node.child)
        {
            CallAptFunc(*(tree_node.child));
            tree_node.child->stdDone = true;
        }
                int numV =1;
                node * ptempTreeNode = tree_node.child;
        while ((ptempTreeNode = ptempTreeNode->sibling))
        {
            CallAptFunc(*ptempTreeNode);
            ptempTreeNode->stdDone = true;
                        numV++;

        }
                if(numV <0){
                        tree_node.stdDone = true;
                        return;
                }

                node * comma = new node[1];
                comma->data->token =",";
                node * tau = new node[1];
                tau->data->token ="tau";
                comma->child = tree_node.child->child;
                tau->child = comma->child->sibling;
                comma->child->sibling = NULL;
                node * curcomma = comma->child;
                node * curtau = tau->child;

                ptempTreeNode = tree_node.child;
                node * prev = tree_node.child;
        while ((prev=ptempTreeNode )&& (ptempTreeNode = ptempTreeNode->sibling))
        {
                        delete[] prev;
                        curcomma->sibling = ptempTreeNode->child;
                        curtau->sibling = curcomma->sibling->sibling;
                        curcomma->sibling->sibling =NULL;
                        curcomma = curcomma->sibling;
                        curtau = curtau->sibling;

        }
                tree_node.data->token = "=";
                tree_node.child = comma;
                comma->sibling = tau;
                tau->sibling = NULL;
                tree_node.stdDone = true;
    }
}

void StandardizeAST::mAssignmentConversion(node& tree_node)
{
    if (tree_node.data->get_token() == "=")
    {
        if (tree_node.child)
        {
            CallAptFunc(*(tree_node.child));
            tree_node.child->stdDone = true;
        }
        if (tree_node.sibling)
        {
            CallAptFunc(*(tree_node.sibling));
            tree_node.child->stdDone = true;
        }
    }
        tree_node.stdDone = true;
}
