#include <iostream>
#include <string>
#include <stack>

namespace forParsing 
{
    using namespace std;

    enum Precedence {
        low = 1,
        mid,
        high
    };

    //the function is needed for checking if an input string contains proper symbols such as {0...9} or {+, -, *, /, (, )}
    bool check(string result);
    
    bool isOperand(char op);
    bool isOperator(char op);

    void PrintExprInTheConsole(int op1, int op2, char oper) {
        cout << endl << op2 << oper << op1 << endl;
    }

    //returning precedence of each operation with enumrate
    int Priority(char op);    

    //the function is used for removing commas
    string removeLastEl(string result);

    //converting infix to postfix notation using stack
    string toPostfix(string s);
    void Calculate(string s);   

    void algo()
    {
        string s;// = "(1+2)*4+3";
        cout << "Введите простой пример (без пробелов): ";        
        cin >> s;
        bool state;
        check(s) ? state = 1 : state = 0;
        if (state) {
            s = toPostfix(s); // converting to posfix notation
            Calculate(s); // solving the expression
        }
        else
            cout << "\nThere was a problem in input.\n";
    }    

    //converting infix to postfix notation using stack
    string toPostfix(string s)
    {
        stack <char> stk;
        string result;// = "12,+,4,*,3,+";        
        int k = 0; // iterator in "for each" loop
        bool flag = true;
        for (char CurrVar : s) {
            if (isdigit(CurrVar)) {
                result += CurrVar;              //
                if (k < s.size() - 1 && flag)   // checking if a number is more than 9
                    if (!isdigit(s[k + 1])) {   //
                        flag = false;           //
                        result += ',';          //
                    }                           //
                    else                        //
                        flag = true;            //
                if (k == s.size() - 1) // in case if the last symbol in the string is a digit
                    result += ',';
            }
            else {
                if (isOperator(CurrVar)) {
                    int CurrPrior = Priority(CurrVar), StackPrior = 0;
                    if (!stk.empty())
                        StackPrior = Priority(stk.top());
                    if (!stk.empty() && (StackPrior < CurrPrior))
                        stk.push(CurrVar);
                    else {
                        while (!stk.empty() && (StackPrior >= CurrPrior)) {
                            result += stk.top();
                            result += ',';  //
                            stk.pop();
                            if(!stk.empty())
                                StackPrior = Priority(stk.top());
                        }
                        stk.push(CurrVar);
                    }
                }
                if (CurrVar == '(')
                    stk.push(CurrVar);
                if (CurrVar == ')') {
                    while (stk.top() != '(') {
                        result += stk.top();
                        result += ',';  //
                        stk.pop();
                    }
                    stk.pop();
                }
                flag = true;
            }
            ++k;
        }                                                                              // infix expr    postfix expr
        // if stack contains some operators then they should be put in the result string: (1+2)*4+3 --> 12+4*3+
        while (!stk.empty()) { 
            result += stk.top();
            result += ',';  //
            stk.pop();
        }
        result = removeLastEl(result);  //
        return result;
    }

    void Calculate(string s)
    {
        stack <int> storage;
        bool state = 1;
        cout << "===CALC===" << endl;
        for (int i = 0; i < s.size() && state; i++)
        {
            if (isdigit(s[i])) {
                int n = s[i] - 48; // ascii shit                
                while (s[++i] != ',') {
                    if (i < s.size()) {
                        n *= 10;
                        n += (s[i] - 48);
                    }                    
                }                
                --i;            
                storage.push(n);
            }
            else if (s[i] == '+' || s[i] == '-' || s[i] == '/' || s[i] == '*') {
                int op1 = storage.top();
                storage.pop();
                int op2 = storage.top();
                storage.pop();

                if (s[i] == '*') {
                    PrintExprInTheConsole(op1, op2, '*');                    
                    op1 *= op2;
                }
                else if (s[i] == '/') {
                    if (op1) {
                        PrintExprInTheConsole(op1, op2, '/');
                        op1 = op2 / op1;
                    }
                    else {
                        cout << "\nCANNOT DIVIDE BY ZERO" << endl;
                        state = false;
                    }
                }
                if (s[i] == '+') {
                    PrintExprInTheConsole(op1, op2, '+');
                    op1 += op2;
                }
                else if (s[i] == '-') {
                    PrintExprInTheConsole(op1, op2, '-');
                    op1 = op2 - op1;
                    
                }

                storage.push(op1);
            }
        }
        if (state)
            cout << "\nRESULT\n" << storage.top() << endl;
    }

    //returning precedence of each operation with enumrate
    int Priority(char op)
    {
        int res = -1;
        switch (op)
        {
        case '+':
        case '-':
            return mid;

        case '*':
        case '/':
            return high;

        case '(':
            //case ')':
            return low;

        default:            
            return -1;            
        }
    }

    //the function is used for removing commas
    string removeLastEl(string result)
    {
        //const int size = result.size();
        string tmp;
        for (int i = 0; i < result.size() - 1; i++)
            tmp += result[i];        
        return tmp;
    }

    bool isOperand(char op)
    {
        switch (op)
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
        }
        return false;
    }

    bool isOperator(char op)
    {
        switch (op)
        {
        case '*':
        case '/':
        case '+':
        case '-':
            return true;
        default:
            return false;
        }
        return false;
    }

    //the function is needed for checking if an input string contains proper symbols such as {0...9} or {+, -, *, /, (, )}
    bool check(string result)
    {
        bool state = 1;
        for (int i = 0; i < result.size() && state; i++)
        {
            if (!isOperand(result[i]) && !isOperator(result[i]) && result[i] != '(' && result[i] != ')')
                return false;
        }
        return true;
    }
}

int main()
{
    system("chcp 1251>nul");    
    forParsing::algo();
    return 0;
}