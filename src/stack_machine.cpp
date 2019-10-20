////////////////////////////////////////////////////////////////////////////////
// Module Name:  stack_machine.h/cpp
// Authors:      Sergey Shershakov
// Version:      0.2.0
// Date:         23.01.2017
//
// This is a part of the course "Algorithms and Data Structures" 
// provided by  the School of Software Engineering of the Faculty 
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////

#include "stack_machine.h"

#include <vector>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string>

namespace xi {

//==============================================================================
// Free functions -- helpers
//==============================================================================

std::vector<std::string> split(const std::string &expr, char splitSymbol){
    std::vector<std::string> tokens;
    std::string buf = "";
    for (int i = 0; i < expr.length(); ++i)
    {
        if(expr[i] == splitSymbol)
        {
            tokens.push_back(buf);
            buf = "";
        }
        else
            buf += expr[i];
    }
    tokens.push_back(buf);
    return tokens;
}

bool isInt(std::string token){
    for (char symbol : token)
    {
        if(symbol < '0' || symbol > '9')
            return false;
    }
    return true;
}

//==============================================================================
// class PlusOp
//==============================================================================


int PlusOp::operation(char op, int a, int b, int /*c*/) // < just commented unused argument. This does not affect the code anyhow.
{
    if(op != '+')
        throw std::logic_error("Operation other than Plus (+) are not supported");

    // here we just ignore unused operands
    return a + b;
}

IOperation::Arity PlusOp::getArity() const
{
    return arDue;
}


//==============================================================================
// class DivOp
//==============================================================================


int DivOp::operation(char op, int a, int b, int /*c*/)
{
    if(op != '/')
        throw std::logic_error("Operation other than Plus (/) are not supported");
    if(b == 0 && a < 0)
        return -INT_MAX;
    if(b == 0 && a >=0)
        return INT_MAX;
    return a/b;
}

IOperation::Arity DivOp::getArity() const
{
    return arDue;
}


//==============================================================================
// class AssignOp
//==============================================================================


int AssignOp::operation(char op, int a, int /*b*/, int /*c*/)
{
    if(op != '=')
        throw std::logic_error("Operation other than Plus (=) are not supported");
    return a;
}

IOperation::Arity AssignOp::getArity() const
{
    return arUno;
}


//==============================================================================
// class InverOp
//==============================================================================


int InverOp::operation(char op, int a, int /*b*/, int /*c*/)
{
    if(op != '~')
        throw std::logic_error("Operation other than Plus (~) are not supported");
    return ~a;
}

IOperation::Arity InverOp::getArity() const
{
    return arUno;
}

//==============================================================================
// class StackMachine
//==============================================================================


void StackMachine::registerOperation(char symb, xi::IOperation *oper)
{
    SymbolToOperMapConstIter it = _opers.find(symb);
    if(it != _opers.end())
        throw std::logic_error("An operation ... is alr. reg...");
    _opers[symb] = oper;
}

IOperation* StackMachine::getOperation(char symb)
{
    SymbolToOperMapConstIter it = _opers.find(symb);
    if(it == _opers.end())
        throw std::logic_error("An operation wasn't found");
    return it -> second;
}

int StackMachine::calculate(const std::string &expr, bool clearStack)
{
    if(clearStack)
        _s.clear();

    std::vector<std::string> tokens = split(expr, ' ');

    for(std::string token : tokens){
        if(isInt(token))
            _s.push(int(std::stoi(token)));
        else
        {
            IOperation* operation = getOperation(token[0]);
            IntStack arguments(3);
            for (int i = 0; i < operation->getArity() + 1; ++i)
            {
                arguments.push(_s.pop());
            }
            switch (operation->getArity())
            {
                case IOperation::Arity::arUno:
                    _s.push(operation->operation(token[0], arguments.pop()));
                    break;
                case IOperation::Arity::arDue:
                    _s.push(operation->operation(token[0], arguments.pop(), arguments.pop()));
                    break;
                case IOperation::Arity::arTre:
                    _s.push(operation->operation(token[0], arguments.pop(), arguments.pop(), arguments.pop()));
                    break;
            }
        }
    }
    return _s.top();
}

} // namespace xi
