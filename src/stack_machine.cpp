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

namespace xi {

//==============================================================================
// Free functions -- helpers
//==============================================================================

// TODO: if you need any free functions, add their definitions here.

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
    switch(symb){
        case '+':
            return new PlusOp();
        case '/':
            return new DivOp();
        case '=':
            return new AssignOp;
        case '~':
            return new InverOp();
        default:
            return nullptr;
    }
}

} // namespace xi
