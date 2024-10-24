#include "iostream"
#include "string"
#include "stack"
#include "cctype"
#include "algorithm"

using namespace std;

enum ExpressionType {
    PREFIX,
    INFIX,
    POSTFIX
};

// 二叉树结点
struct TreeNode {
    char data;
    TreeNode *left;
    TreeNode *right;
    TreeNode *parent;
    bool ltag;  // true-线索, false-左孩子
    bool rtag;  // true-线索, false-右孩子
    explicit TreeNode(char c) : data(c), left(nullptr), right(nullptr), parent(nullptr), ltag(false), rtag(false) {}
};

class BinaryTree {
public:
    BinaryTree() : root(nullptr), prev(nullptr) {}

    BinaryTree(const string &inorder, const string &postorder) : prev(nullptr) {
        /*
         * @param inorder 中序遍历结果序列
         * @param postorder 后续遍历结果序列
         */
        root = build_tree(inorder, postorder, 0, inorder.length() - 1, 0, postorder.length() - 1);
    }

    void preorder_traverse() {
        cout << "前序遍历结果: ";
        preorder_traverse_helper(root);
        cout << endl;
    }

    void inorder_traverse() {
        cout << "中序遍历结果: ";
        inorder_traverse_helper(root);
        cout << endl;
    }

    void postorder_traverse() {
        cout << "后序遍历结果: ";
        postorder_traverse_helper(root);
        cout << endl;
    }

    void postorder_threading() {
        postorder_threading_helper(root);
        if (root && !root->right) {  // 将根结点的后继线索置为nullptr
            root->rtag = true;
        }
    }

    void postorder_threading_traverse() {
        /*
         * 利用后序后继线索遍历
         */
        cout << "后序线索化遍历结果: ";
        // 一直向左, 找后序遍历开始结点
        auto curr = root;
        while (curr && !curr->ltag) {
            curr = curr->left;
        }

        TreeNode *prev_node = nullptr;
        while (curr) {
            if (curr->rtag) {  // right是线索, 说明当前结点无右子树(即左右子树已遍历完), 则打印当前结点, 然后跳转到后继线索
                cout << curr->data << " ";
                prev_node = curr;
                curr = curr->right;
            } else {  // 当前结点有右子树
                if (prev_node == curr->right) {  // 上一个访问的结点是右孩子, 说明左右子树已经遍历完, 可以打印当前结点, 然后回到parent
                    cout << curr->data << " ";
                    prev_node = curr;
                    curr = curr->parent;
                } else {  // 上一次访问的不是右孩子, 说明右子树尚未遍历, 从右子树中的最左侧结点开始遍历
                    curr = curr->right;
                    while (curr && !curr->ltag) {
                        curr = curr->left;
                    }
                }
            }
        }
        cout << endl;
    }

    void postorder_threading_traverse_v1() {
        /*
         * 利用后序前驱线索逆向遍历
         */
        stack<TreeNode *> s;
        auto curr = root;  // 后序遍历的结束结点为根结点
        while (curr) {
            s.push(curr);
            if (curr->ltag) {  // left是线索, 则后序前驱就是后序前驱线索
                curr = curr->left;
            } else if (curr->rtag) {  // right是线索, 说明右孩子为空, 则后序前驱就是左孩子
                curr = curr->left;
            } else {  // 右孩子非空, 则后序前驱就是右孩子
                curr = curr->right;
            }
        }
        cout << "后序线索化遍历结果: ";
        while (!s.empty()) {
            cout << s.top()->data << " ";
            s.pop();
        }
        cout << endl;
    }

protected:
    TreeNode *root;
    TreeNode *prev;  // 线索化用

    TreeNode *build_tree(const string &inorder, const string &postorder,
                         int in_start, int in_end, int post_start, int post_end) {
        /*
         * @param inorder 中序遍历结果序列
         * @param postorder 后续遍历结果序列
         * @param in_start inorder序列开始索引
         * @param in_end inorder序列结束索引
         * @param post_start postorder序列开始索引
         * @param post_end postorder序列结束索引
         * @return 根结点
         */
        if (in_start > in_end || post_start > post_end) {
            return nullptr;
        }

        char root_data = postorder[post_end];  // postorder最后一个元素即为根结点
        auto curr_root = new TreeNode(root_data);  // 构建根结点

        int root_index = in_start;  // 在inorder序列中寻找root结点的位置
        while (inorder[root_index] != root_data) {
            root_index++;
        }
        auto left_child = build_tree(inorder, postorder, in_start, root_index - 1, post_start,
                                     post_start + root_index - in_start - 1);  // 递归构建左子树
        auto right_child = build_tree(inorder, postorder, root_index + 1, in_end,
                                      post_start + root_index - in_start, post_end - 1);  // 递归构建右子树
        if (left_child) {
            curr_root->left = left_child;
            left_child->parent = curr_root;
        }
        if (right_child) {
            curr_root->right = right_child;
            right_child->parent = curr_root;
        }
        return curr_root;
    }

    void preorder_traverse_helper(TreeNode *curr_root) {
        if (!curr_root) {
            return;
        }
        cout << curr_root->data << " ";
        preorder_traverse_helper(curr_root->left);
        preorder_traverse_helper(curr_root->right);
    }

    void inorder_traverse_helper(TreeNode *curr_root) {
        if (!curr_root) {
            return;
        }
        inorder_traverse_helper(curr_root->left);
        cout << curr_root->data << " ";
        inorder_traverse_helper(curr_root->right);
    }

    void postorder_traverse_helper(TreeNode *curr_root) {
        if (!curr_root) {
            return;
        }
        postorder_traverse_helper(curr_root->left);
        postorder_traverse_helper(curr_root->right);
        cout << curr_root->data << " ";
    }

    void postorder_threading_helper(TreeNode *curr) {
        if (!curr) {
            return;
        }
        postorder_threading_helper(curr->left);  // 线索化左子树
        postorder_threading_helper(curr->right);  // 线索化右子树
        if (!curr->left) {  // 将当前结点的前驱线索指向后序前驱节点
            curr->left = prev;
            curr->ltag = true;
        }
        if (prev && !prev->right) {  // 将后序前驱结点的后继线索指向当前结点
            prev->right = curr;
            prev->rtag = true;
        }
        prev = curr;
    }
};

class ExpressionTree : public BinaryTree {
public:
    explicit ExpressionTree(const string &expr, ExpressionType expr_type) {
        if (expr.empty()) {  // 表达式为空
            root = nullptr;
            return;
        }
        switch (expr_type) {
            case PREFIX: {
                build_prefix(expr);
                break;
            }
            case INFIX: {
                build_infix(expr);
                break;
            }
            case POSTFIX: {
                build_postfix(expr);
                break;
            }
            default: {
                cerr << "Invalid expression type." << endl;
                exit(1);
            }
        }
    }

    void print_prefix_expression() {
        cout << "前缀表达式为: ";
        preorder_traverse_helper(root);
        cout << endl;
    }

    void print_infix_expression() {
        cout << "中缀表达式为: ";
        inorder_traverse_helper(root);
        cout << endl;
    }

    void print_postfix_expression() {
        cout << "后缀表达式为: ";
        postorder_traverse_helper(root);
        cout << endl;
    }

    void evaluate_expression() {
        cout << "表达式求值结果为: " << evaluate_helper(root) << endl;
    }

private:
    bool lower_priority(char op1, char op2) {
        /*
         * @return op1的优先级是否低于op2
         */
        return (op2 == '*' || op2 == '/') && (op1 == '+' || op1 == '-');
    }

    void build_prefix(const string &expr) {
        auto expr_rev = expr;
        reverse(expr_rev.begin(), expr_rev.end());
        stack<TreeNode *> s;
        for (auto c: expr_rev) {
            auto node = new TreeNode(c);
            if (isdigit(c)) {  // 操作数
                s.push(node);
            } else {
                node->left = s.top();  // 第1个操作数
                s.pop();
                node->right = s.top();  // 第2个操作数
                s.pop();
                s.push(node);
            }
        }
        root = s.top();
    }

    void build_infix(const string &expr) {
        stack<char> optr;  // 运算符栈
        stack<TreeNode *> s;  // 结点栈
        for (auto c: expr) {
            if (isdigit(c)) {  // 为操作数
                auto node = new TreeNode(c);
                s.push(node);
            } else {  // 运算符
                if (c == '(') {  // 左括号
                    optr.push(c);
                } else if (c == ')') {  // 右括号
                    while (optr.top() != '(') {  // 不断弹出optr栈中的运算符, 为它们建立结点, 直至遇到左括号
                        auto node = new TreeNode(optr.top());
                        optr.pop();
                        node->right = s.top();  // 操作数2
                        s.pop();
                        node->left = s.top();  // 操作数1
                        s.pop();
                        s.push(node);
                    }
                    optr.pop();  // 弹出左括号
                } else {  // 不是括号
                    while (!optr.empty() && optr.top() != '(' &&
                           !lower_priority(optr.top(), c)) {  // 不断弹出optr栈中的运算符, 为它们建立结点, 直至栈顶运算符的优先级比当前低或栈空了
                        auto node = new TreeNode(optr.top());
                        optr.pop();
                        node->right = s.top();  // 操作数2
                        s.pop();
                        node->left = s.top();  // 操作数1
                        s.pop();
                        s.push(node);
                    }
                    optr.push(c);  // 将当前符号压入optr栈
                }
            }
        }
        while (!optr.empty()) {  // 处理剩余optr栈中的运算符, 依次为它们建立结点
            auto node = new TreeNode(optr.top());
            optr.pop();
            node->right = s.top();  // 操作数2
            s.pop();
            node->left = s.top();  // 操作数1
            s.pop();
            s.push(node);
        }
        root = s.top();
    }

    void build_postfix(const string &expr) {
        stack<TreeNode *> s;
        for (auto c: expr) {
            auto node = new TreeNode(c);
            if (isdigit(c)) {  // 操作数
                s.push(node);
            } else {  // 运算符
                node->right = s.top();  // 第2个操作数
                s.pop();
                node->left = s.top();  // 第1个操作数
                s.pop();
                s.push(node);
            }
        }
        root = s.top();  // 最后栈中只剩一个元素, 即为根结点
    }

    void inorder_traverse_helper(TreeNode* curr_root) {
        if (!curr_root) {
            return;
        }
        if (curr_root->left) {
            cout << "( ";
        }
        inorder_traverse_helper(curr_root->left);
        cout << curr_root->data << " ";
        inorder_traverse_helper(curr_root->right);
        if (curr_root->right) {
            cout << ") ";
        }
    }

    int evaluate_helper(TreeNode *curr_root) {
        if (!curr_root) {
            return 0;
        }
        if (!curr_root->left && !curr_root->right) {  // 叶子结点存储的是数字
            return curr_root->data - '0';
        }
        int left_res = evaluate_helper(curr_root->left);  // 左子树运算结果
        int right_res = evaluate_helper(curr_root->right);  // 右子树运算结果
        switch (curr_root->data) {
            case '+':
                return left_res + right_res;
            case '-':
                return left_res - right_res;
            case '*':
                return left_res * right_res;
            case '/':
                return left_res / right_res;
            default:
                cerr << "Invalid operator" << endl;
                exit(1);
        }
    }

};

int main() {
    string inorder, postorder;
    cout << "请输入中序遍历结果: ";
    getline(cin, inorder);
    cout << "请输入后序遍历结果: ";
    getline(cin, postorder);
    BinaryTree btree(inorder, postorder);
    btree.preorder_traverse();
    btree.inorder_traverse();
    btree.postorder_traverse();
    btree.postorder_threading();
    btree.postorder_threading_traverse();

    string expr;
    int expr_type;
    cout << "请输入表达式: ";
    getline(cin, expr);
    cout << "请输入表达式类型: 0-前缀, 1-中缀, 2-后缀: ";
    cin >> expr_type;
    ExpressionTree expr_tree(expr, static_cast<ExpressionType>(expr_type));
    expr_tree.print_prefix_expression();
    expr_tree.print_infix_expression();
    expr_tree.print_postfix_expression();
    expr_tree.evaluate_expression();
    return 0;
}