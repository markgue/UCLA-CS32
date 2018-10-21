#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

int precedence(char op);

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
	postfix = "";
	stack<char> opStack;
	char ch;

	// convert infix to postfix
	for (int i = 0; i < infix.length(); i++)
	{
		ch = infix[i];
		switch (ch)
		{
		case '(':
			opStack.push(ch);
			break;

		case ')':
			// while loop ends when it encounters a '(' or the stack empties
			while (!opStack.empty() && opStack.top() != '(')
			{
				postfix += opStack.top();
				opStack.pop();
			}
			// returns 1 if there was no '(' to end the while loop
			if (opStack.empty())
				return 1;
			// pop the '('
			opStack.pop();
			break;

		// Operators
		case '+':
		case '-':
		case '*':
		case '/':
			while (!opStack.empty() && opStack.top() != '('
				&& precedence(ch) <= precedence(opStack.top()))
			{
				postfix += opStack.top();
				opStack.pop();
			}
			opStack.push(ch);
			break;

		// Skip spaces
		case ' ':
			break;

		// Default checks for operands
		default:
			if (ch >= 'a' && ch <= 'z')
				postfix += ch;
			else
				return 1;
		}
	}
	while (!opStack.empty())
	{
		postfix += opStack.top();
		opStack.pop();
	}

	// evaluate postfix
	stack<int> evalStack;
	int value, operand1, operand2;

	for (int i = 0; i < postfix.length(); i++)
	{
		ch = postfix[i];
		if (ch >= 'a' && ch <= 'z')
		{
			// if ch does not contain a matching value return 2 
			if (values.get(ch, value) == 0)
				return 2;
			evalStack.push(value);
		}
		else // it is an operator
		{
			// Check that the 2nd operand exists and retrieve it then pop it
			if (evalStack.empty())
				return 1;
			operand2 = evalStack.top();
			evalStack.pop();
			// Check that the 1st operand exists and retrieve it then pop it
			if (evalStack.empty())
				return 1;
			operand1 = evalStack.top();
			evalStack.pop();
			// Perform the required operation
			switch (ch)
			{
			case '+':
				evalStack.push(operand1 + operand2);
				break;
			case '-':
				evalStack.push(operand1 - operand2);
				break;
			case '*':
				evalStack.push(operand1 * operand2);
				break;
			case '/':
				// Check for division by 0
				if (operand2 == 0)
					return 3;
				evalStack.push(operand1 / operand2);
				break;
			}
		}
	}
	if (evalStack.size() != 1)
		return 1;
	result = evalStack.top();
	return 0;
}

// returns an int based on the precedence of the four operators
int precedence(char op)
{
	switch (op)
	{
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	default:
		return -1;
	}
}

int main()
{
	char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
	int  vals[] = { 3,  -9,   6,   2,   4,   1 };
	Map m;
	for (int k = 0; vars[k] != '#'; k++)
		m.insert(vars[k], vals[k]);
	string pf;
	int answer;
	assert(evaluate("a+ e", m, pf, answer) == 0 &&
		pf == "ae+"  &&  answer == -6);
	answer = 999;
	assert(evaluate("", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a+", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a i", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("ai", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("()", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("y(o+u)", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a+E", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("(a+(i-o)", m, pf, answer) == 1 && answer == 999);
	// unary operators not allowed:
	assert(evaluate("-a", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a*b", m, pf, answer) == 2 &&
		pf == "ab*"  &&  answer == 999);
	assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0 &&
		pf == "yoau-*+"  &&  answer == -1);
	answer = 999;
	assert(evaluate("o/(y-y)", m, pf, answer) == 3 &&
		pf == "oyy-/"  &&  answer == 999);
	assert(evaluate(" a  ", m, pf, answer) == 0 &&
		pf == "a"  &&  answer == 3);
	assert(evaluate("((a))", m, pf, answer) == 0 &&
		pf == "a"  &&  answer == 3);
	cout << "Passed all tests" << endl;
}