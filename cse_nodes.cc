#include "cse_nodes.hh"
#include <cstdlib>

void lambda::add_param(std::string p)
{
        params.push_front(p);
}

/**
 *Essentially performs a preorder traversal of comma child
 *and adds everything to the list
 **/
void lambda::add_comma(node* comma_child)
{
        if(! comma_child)
        {
                return;
        }
        else
        {
                params.push_front(comma_child->data->get_token());
                //std::cout<<"Got string "<< comma_child->data->get_token();
                add_comma(comma_child->child);
                add_comma(comma_child->sibling);
        }

}
void lambda::print()
{
                list<std::string>::iterator l;
                std::cout<<"k = "<<k<<" ";
                for ( l = params.begin(); l != params.end(); l ++)
                {
                                                std::cout<<(*l)<<" ";
                }
}

void tau::set_num_elements(node* tau_child)
{
                if(! tau_child)
                {
                        return;
                }
                else
                {
                        num_elements ++;
                        set_num_elements(tau_child->child);
                        set_num_elements(tau_child->sibling);
                }
}

node* tau::get_element(int element_no )
{
        int count = 1;
        node * ptr = tree_reference->child;
        while (ptr && count != element_no)
        {
                if( ptr->sibling)
                {
                        ptr = ptr->sibling;
                }
                else
                {
                        cout<<"Element not found";
                        exit(0);
                }
        }
        return ptr;
}

void tau::print_tau()
{
        if (num_elements == 0)
        {
                std::cout<<"(nil)";
        }
        else
        {
                std::cout<<"(";
                node * ptr = tree_reference->child;
                while(ptr)
                {
                        std::cout<<ptr->data->get_token();
                        if (ptr->sibling)
                        {
                                ptr = ptr->sibling;
                        }
                }
                std::cout<<")";
        }
}

void tuple::print()
{
        std::cout<<"Todo: tuple print called";
}


node * tuple::get_element(int element_no)
{
        list<node *>::iterator i;
        int j = 1;
        for ( i = elements.begin(); i != elements.end(); i++)
        {
                if (j == element_no)
                {
                        return (*i);
                }
                else
                {
                        j++;
                }
        }
}

