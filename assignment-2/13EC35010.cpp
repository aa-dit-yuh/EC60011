#include <iostream>
#include <stack>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Token
{
  public:
    virtual int value() = 0;
    virtual string postFixTraversal() = 0;
};

class NumericToken : public Token
{
  private:
    int val;

  public:
    NumericToken(int val) : val(val) {}
    virtual int value()
    {
        return val;
    }

    virtual string postFixTraversal()
    {
        return to_string(val);
    }
};

class BinaryOperatorToken : public Token
{
  private:
    char op;
    Token *left, *right;

  public:
    BinaryOperatorToken(const char op, Token *left, Token *right) : op(op), left(left), right(right) {}

    virtual int value()
    {
        int left_val = left->value();
        int right_val = right->value();

        switch (op)
        {
        case '+':
            return left_val + right_val;
        case '-':
            return left_val - right_val;
        case '*':
            return left_val * right_val;
        case '/':
            return left_val / right_val;
        case '%':
            return left_val % right_val;
        default:
            return 0;
        }
    }

    virtual string postFixTraversal()
    {
        stringstream postfix;
        if (left != NULL)
            postfix << left->postFixTraversal();
        if (right != NULL)
            postfix << " " << right->postFixTraversal();
        postfix << " " << op;
        string s = postfix.str();
        return postfix.str();
    }
};

class ExpressionBuilder
{
  private:
    stack<char> operator_stack;
    stack<Token *> token_stack;

    vector<string> tokenize(const string &input)
    {
        vector<string> tokens;
        string token;
        for (int i = 0; i < input.size(); ++i)
        {
            char c = input[i];
            if (isalnum(c))
            {
                token += c;
            }
            else if (!isspace(c))
            {
                if (!token.empty())
                {
                    tokens.push_back(token);
                    token.clear();
                }
                tokens.push_back(string(1, c));
            }
        }
        if (!token.empty())
        {
            tokens.push_back(token);
        }
        return tokens;
    }

    int precedence(char op)
    {
        enum
        {
            lowest,
            mid,
            highest
        };

        switch (op)
        {
        case '/':
        case '*':
            return highest;
        case '+':
        case '-':
            return mid;
        default:
            return lowest;
        }
    }

    void addBinaryOperator()
    {
        Token *right = token_stack.top();
        token_stack.pop();
        Token *left = token_stack.top();
        token_stack.pop();
        BinaryOperatorToken *p = new BinaryOperatorToken(operator_stack.top(), left, right);
        token_stack.push(p);
        operator_stack.pop();
    }

  public:
    BinaryOperatorToken *parseFromInfix(const string infix)
    {
        vector<string> tokens = tokenize(infix);
        for (int i = 0; i < tokens.size(); ++i)
        {
            string token = tokens[i];
            if (token.length() == 1)
            {
                char op = token[0];
                switch (op)
                {
                case '+':
                case '-':
                case '*':
                case '/':
                    // pop operators with higher precedence and create their BinaryOperationNode
                    while ((!operator_stack.empty()) && (precedence(op) <= precedence(operator_stack.top())))
                        addBinaryOperator();
                    // lastly push the operator passed onto the operator_stack
                    operator_stack.push(op);
                    break;
                case '(':
                    operator_stack.push(op);
                    break;
                case ')':
                    while (!operator_stack.empty() && operator_stack.top() != '(')
                        addBinaryOperator();
                    operator_stack.pop(); // remove '('
                    break;
                default:
                    NumericToken *num_token = new NumericToken(stoi(token));
                    token_stack.push(num_token);
                }
            }
            else
            {
                NumericToken *num_token = new NumericToken(stoi(token));
                token_stack.push(num_token);
            }
        }

        while (!operator_stack.empty())
            addBinaryOperator();

        // Invariant: At this point the operandStack should have only one element
        //     operandStack.size() == 1
        // otherwise, the expression is not well formed.
        if (token_stack.size() != 1)
        {
            // throw invalid_argument("Malformed input");
        }
        Token *p = token_stack.top();
        return static_cast<BinaryOperatorToken *>(p);
    }
};

int main()
{
    string infix;
    getline(cin, infix);
    cout << "Infix Expression: " << infix << endl;

    ExpressionBuilder builder;
    BinaryOperatorToken *root = builder.parseFromInfix(infix);

    string postfix = root->postFixTraversal();
    cout << "Postfix Expression: " << postfix << endl;
    cout << "Expression evaluates to " << root->value() << endl;
    return 0;
}
