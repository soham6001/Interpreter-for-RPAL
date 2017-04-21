/*
 * StandardizeAST.hh
 *
 *  Created on: Apr 18, 2009
 *      Author: Saurabh
 */

#ifndef STANDARDIZEAST_HH_
#define STANDARDIZEAST_HH_


#include "tree.hh"


class StandardizeAST
{
private:
        enum eRuleNumm   { R_unknown = -1, R_let = 0, R_where, R_within, R_and, R_tau,
                               R_functionform, R_lambda, R_binaryop, R_unaryop,
                               R_rec, R_comma, R_at, R_assignment, R_gamma, R_cond };

    //StandardizeAST() { }
    void mLetConversion(node& tree_node);
    void mWhereConversion(node& tree_node);
    void mTauConversion(node& tree_node);
    void mCondConversion(node& tree_node);
    void mUniaryOperatorConversion(node& tree_node);
    void mWithinConversion(node& tree_node);
    void mRecConversion(node& tree_node);
    void mFunctionFormConversion(node& tree_node);
    void mLambdaConversion(node& tree_node);
    void mCommaConversion(node& tree_node);
    void mBinaryOperatorConversion(node& tree_node);
    void mAtConversion(node& tree_node);
    void mAndConversion(node& tree_node);
    void mAssignmentConversion(node& tree_node);
    void CallAptFunc(node& tree_node);
                void PrintTree(node *);
public:
    node treeNode;
    StandardizeAST(const node& tree_node) { treeNode = tree_node; }
    node& standardize();
    const node& mGetSTTree() { return treeNode; }
    static int getRuleId(string szRule);
                void Print();
};

#endif /* STANDARDIZEAST_HH_ */

