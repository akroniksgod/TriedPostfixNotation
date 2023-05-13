#include <iostream>
#include <string>
#include <stack>
#include <algorithm>

namespace Parser
{
    using namespace std;

    enum Precedence {
        LOW = 1,
        MIDDLE,
        HIGH
    };

    //the function is needed for checking if an input string contains proper symbols such as {0...9} or {+, -, *, /, (, )}
    bool isCorrectStr(string);

    bool isOperand(char);
    bool isOperator(char);    

    //returning precedence of each operation with enumrate
    int getPriority(char);

    //converting infix to postfix notation using stack
    string getPostfixString(string);
    void printExpression(int, int, char);
    void calculateParsedExpression(string);
    string getString();
    void algo();

    void algo()
    {
        string s = getString();// = "(1+2)*4+3"; 

        s = getPostfixString(s); // converting to posfix notation
        calculateParsedExpression(s); // solving the expression
    }

    string getString()
    {
        string s;
        cout << "Input a simple math expression (without using spaces): ";
        bool isParsed = false;
        while (!isParsed)
        {
            cin >> s;
            isParsed = isCorrectStr(s);
            if (!isParsed) cout << "\nCould not parse expression\n" << "Input again: ";
        }
        return s;
    }    

    //converting infix to postfix notation using stack
    string getPostfixString(string s)
    {
        stack <char> stk;
        string result;// = "12,+,4,*,3,+";        
        //int k = 0; // iterator in "for each" loop
        bool flag = true;
        for (int k = 0; k < s.length(); ++k)
        {
            char currentSymbol = s[k];
            if (isdigit(currentSymbol)) 
            {
                result += currentSymbol;              //
                if (k < s.size() - 1 && flag)   // checking if a number is more than 9
                {
                    if (!isdigit(s[k + 1])) 
                    {   //
                        flag = false;           //
                        result += ',';          //
                    }
                    flag = true;            //
                }
                    
                if (k == s.size() - 1) // in case if the last symbol in the string is a digit
                    result += ',';
            }
            else 
            {
                if (isOperator(currentSymbol)) 
                {
                    int currPriority = getPriority(currentSymbol), stackPriority = 0;
                    if (!stk.empty())
                    {
                        stackPriority = getPriority(stk.top());
                    }
                        
                    if (!stk.empty() && (stackPriority < currPriority))
                    {
                        stk.push(currentSymbol);
                    }   
                    else 
                    {
                        while (!stk.empty() && (stackPriority >= currPriority)) 
                        {
                            result += stk.top();
                            result += ',';  //
                            stk.pop();
                            if (!stk.empty())
                                stackPriority = getPriority(stk.top());
                        }
                        stk.push(currentSymbol);
                    }
                }
                
                if (currentSymbol == '(')
                {
                    stk.push(currentSymbol);
                }                    
                
                if (currentSymbol == ')') 
                {
                    while (stk.top() != '(') 
                    {
                        result += stk.top();
                        result += ',';  //
                        stk.pop();
                    }
                    stk.pop();
                }
                flag = true;
            }
        }                                                                              
        // infix expr    postfix expr
        // if stack contains some operators then they should be put in the result string: (1+2)*4+3 --> 12+4*3+
        while (!stk.empty()) 
        {
            result += stk.top();
            result += ',';  //
            stk.pop();
        }

        //cut last symbol
        return result.substr(0, result.length() - 1);
    }    

    void calculateParsedExpression(string s)
    {
        stack <int> storage;
        bool state = 1;
        cout << "===History===";
        for (int i = 0; i < s.size() && state; i++)
        {
            if (isdigit(s[i])) 
            {
                int n = s[i] - 48; // ascii shit                
                while (s[++i] != ',') 
                {
                    if (i < s.size()) 
                    {
                        n *= 10;
                        n += (s[i] - 48);
                    }
                }
                --i;
                storage.push(n);
            }
            
            bool isPlus = s[i] == '+', isMinus = s[i] == '-', isDiv = s[i] == '/', isMult = s[i] == '*';
            if (isPlus || isMinus || isDiv || isMult)
            {
                int op1 = storage.top(); storage.pop();
                int op2 = storage.top(); storage.pop();

                if (isMult)
                {
                    printExpression(op1, op2, '*');
                    op1 *= op2;
                }
                
                if (isDiv)
                {
                    if (op1) 
                    {
                        printExpression(op1, op2, '/');
                        op1 = op2 / op1;
                    }
                    else 
                    {
                        cout << "\nCannot divide by zero" << endl;
                        state = false;
                    }
                }
                
                if (isPlus)
                {
                    printExpression(op1, op2, '+');
                    op1 += op2;
                }
                
                if (isMinus)
                {
                    printExpression(op1, op2, '-');
                    op1 = op2 - op1;
                }

                storage.push(op1);
            }
        }

        if (state) cout << "\n===Result===\n" << storage.top() << endl;
    }

    void printExpression(int op1, int op2, char oper)
    {
        cout << endl << op2 << oper << op1 << endl;
    }

    //returning precedence of each operation with enumrate
    int getPriority(char symbol)
    {
        switch (symbol)
        {
            case '(':
                //case ')':
                return LOW;
            case '+':
            case '-':
                return MIDDLE;
            case '*':
            case '/':
                return HIGH;            
            default:
                return -1;
        }
    }

    bool isOperand(char symbol)
    {
        switch (symbol)
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                return true;
            default: 
                return false;
        }
    }

    bool isOperator(char symbol)
    {
        switch (symbol)
        {
            case '*':
            case '/':
            case '+':
            case '-':
                return true;
            default:
                return false;
        }
    }

    //the function is needed for checking if an input string contains proper symbols such as {0...9} or {+, -, *, /, (, )}
    bool isCorrectStr(string expression)
    {
        auto isBracket = [](char c) { return c == '(' || c == ')'; };
        auto isValidCharacter = [&](char c) {
            return isOperand(c) || isOperator(c) || isBracket(c);
        };        

        auto containsValidChars = all_of(
            expression.begin(), expression.end(),
            [&](char c) { return isValidCharacter(c); }
        );

        auto containsOperators = any_of(
            expression.begin(), expression.end(),
            [&](char c) { return isOperator(c); }
        );

        char lastChar = expression[expression.length() - 1];

        return containsValidChars && containsOperators && (isOperand(lastChar) || isBracket(lastChar));
    }
}

int main()
{
    system("chcp 1251>nul");
    Parser::algo();
    return 0;
}