#include <iostream>
#include <map>
#include <stack>
#include <tuple>
#include <utility>

using namespace std;

/**
 *  Returns the result of the ackermann's function for a pair of integers.
 */
int ack(int M, int N)
{
    stack<pair<int, int>> invocation_stack; // Stack: records the invocations of the ackermann function
    map<pair<int, int>, int> ackermann;     // Memoization: stores the intermediate ackermann function output
    invocation_stack.push(make_pair(M, N)); // Insert input to stack

    while (!invocation_stack.empty())
    {
        int m, n;
        tie(m, n) = invocation_stack.top();
        if (m == 0) // ack(m, n) = n + 1    , when m == 0
        {
            ackermann[invocation_stack.top()] = n + 1;
            invocation_stack.pop();
        }
        else if (n == 0) // ack(m, n) = ack(m-1, 1)  , when n == 0
        {
            try
            {
                // check if ackermann already calculated for (m-1, 1)
                ackermann[invocation_stack.top()] = ackermann.at(make_pair(m - 1, 1));
                // already calculated for (m-1, 1); pop stack
                invocation_stack.pop();
            }
            catch (out_of_range)
            {
                // not calculated yet for (m-1, 1); push to stack
                invocation_stack.push(make_pair(m - 1, 1));
            }
        }
        else
        {
            try
            {
                // check if ackermann already calculated for (m, n-1)
                auto arg = ackermann.at(make_pair(m, n - 1));
                try
                {
                    // check if ackermann already calculated for (m-1, ack(m, n-1))
                    ackermann[invocation_stack.top()] = ackermann.at(make_pair(m - 1, arg));
                    // already calculated for (m-1, ack(n-1)); pop stack
                    invocation_stack.pop();
                }
                catch (out_of_range)
                {
                    // not calculated yet for (m-1, ack(m, n-1))
                    invocation_stack.push(make_pair(m - 1, arg));
                }
            }
            catch (out_of_range)
            {
                // not calculated yet for (m, n-1)
                invocation_stack.push(make_pair(m, n - 1));
            }
        }
    }
    return ackermann.at(make_pair(M, N)); // Return memoized output
}

int main()
{
    int m, n;
    cout << "Enter input m and n:" << endl;
    cin >> m >> n;
    if (m < 0 || n < 0)
    {
        cout << "Error: negative input" << endl;
        return -1;
    }
    cout << "Ackermann(" << m << ", " << n << ") = " << ack(m, n) << endl;
    return 0;
}
