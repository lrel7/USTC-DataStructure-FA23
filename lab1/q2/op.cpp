#include "iostream"
#include "stack"
#include "string"
#include "fstream"
#include "map"

bool is_operator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || ch == '#';
}

int compare_priority(char theta1, char theta2) {
    /*
     * @return 0: theta1 < theta2, 1: theta1 > theta2, 2: theta1 = theta2
    */

    // 先判断含括号的情况
    if (theta1 == '(' && theta2 == ')' || theta1 == '#' && theta2 == '#') {
        return 2;
    } else if (theta1 == '(' || theta2 == '(') {
        return 0;
    } else if (theta1 == ')' || theta2 == ')') {
        return 1;
    }

    // 剩下是判断加减乘除
    std::map<char, int> priority;
    priority['#'] = 0;
    priority['+'] = priority['-'] = 1;
    priority['*'] = priority['/'] = 2;
    if (priority[theta1] < priority[theta2]) {
        return 0;
    } else {
        return 1;
    }
}

int do_operation(int a, int b, char op) {
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        default:
            return a / b;
    }
}

void evaluate_expression(const std::string input_filename, const std::string output_filename) {
    std::ifstream input_file(input_filename);
    if (!input_file) {
        std::cerr << "Error: Unable to open the input file." << std::endl;
        exit(1);
    }

    std::stack<char> opr;
    std::stack<int> num;
    char ch;
    input_file >> ch;
    opr.push('#');   // 将表达式起始符压入栈中
    input_file >> ch;
    while (ch != '#' || opr.top() != '#') {
        // 若不是运算符, 直接压入num栈
        if (!is_operator(ch)) {
            num.push(ch - '0');
            input_file >> ch;
        } else
            switch (compare_priority(opr.top(), ch)) {
            case 0:
                // opr栈顶元素优先级小于ch
                opr.push(ch);
                input_file >> ch;
                break;
            case 1: {
                // opr栈顶元素优先级大于ch
                char theta = opr.top();
                opr.pop();
                int b = num.top();
                num.pop();
                int a = num.top();
                num.pop();
                num.push(do_operation(a, b, theta));
                break;
            }
            case 2:
                // 两者优先级相等, 说明是一对括号
                opr.pop();
                input_file >> ch;
                break;
            }
    }
    input_file.close();

    std::ofstream output_file(output_filename);
    if (!output_file) {
        std::cerr << "Error: Unable to open the output file." << std::endl;
        exit(1);
    }
    output_file << num.top();
    output_file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }
    std::string input_file = argv[1];
    std::string output_file = argv[2];
    evaluate_expression(input_file, output_file);
    return 0;
}