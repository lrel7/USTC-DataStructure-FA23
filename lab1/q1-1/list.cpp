#include "iostream"
#include "fstream"
#include "vector"
#include "unordered_set"

struct Node {
    int val;
    Node *prev, *next;

    Node(int v) : val(v), prev(nullptr), next(nullptr) {}
};

class LinkedList {
private:
    Node *head;
    Node *tail;

public:
    LinkedList() : head(nullptr), tail(nullptr) {}

    Node* begin() {
        return head;
    }

    // 添加结点到链表末尾
    void push_back(int v) {
        Node *new_node = new Node(v);
        if (!head) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            new_node->prev = tail;
            tail = new_node;
        }
    }

    // 删除指定结点, 并返回下一个结点
    Node* remove(Node *pos) {
        auto prev_node = pos->prev;
        auto next_node = pos->next;
        if (prev_node) {
            prev_node->next = next_node;
        } else {
            head = head->next;
        }
        if (next_node) {
            next_node->prev = prev_node;
        } else {
            tail = tail->prev;
        }
        auto ret = pos->next;
        delete pos;
        return ret;
    }

    // 自定义析构函数
    ~LinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

void create_list(LinkedList &l, const std::string &filename, int n) {
    /*
     * @param l 双向链表
     * @param filename 包含数据集的输入文件
     * @param n 要求选择的数据个数
    */

    // 从输入文件中获取数据集
    std::ifstream input_file(filename);
    if (!input_file) {
        std::cerr << "Error: Unable to open the input file." << std::endl;
        exit(1);
    }
    std::vector<int> dataset;
    int data;
    while (input_file >> data) {
        dataset.push_back(data);
    }
    input_file.close();

    // 从数据集中随机选取n个数
    for (int i = 0; i < n; i++) {
        int random_index = rand() % (dataset.size());
        l.push_back(dataset[random_index]);
    }
}

void delete_duplicates(LinkedList &l) {
    std::unordered_set<int> dataset;  // 存储已出现过的元素
    auto pos = l.begin();
    while (pos) {
        if (dataset.find(pos->val) != dataset.end()) {  // 当前元素是重复的
            pos = l.remove(pos);
        } else {
            dataset.insert(pos->val);
            pos = pos->next;
        }
    }
}

void print_list(LinkedList &l, const std::string filename, bool duplicate) {
    std::ofstream output_file;
    if (!duplicate) {
        output_file.open(filename, std::ios::app);
    } else {
        output_file.open(filename);
    }
    if (!output_file) {
        std::cerr << "Error: Unable to open the output file." << std::endl;
        exit(1);
    }

    if (duplicate) {
        output_file << "删除重复元素前的链表: \n";
    } else {
        output_file << "删除重复元素后的链表: \n";
    }

    for (auto pos = l.begin(); pos; pos = pos->next) {
        output_file << pos->val << " ";
    }

    output_file << "\n";
    output_file.close();
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <n> <output_file>" << std::endl;
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[3];
    int n = std::atoi(argv[2]);

    LinkedList l;
    create_list(l, input_file, n);
    print_list(l, output_file, true);
    delete_duplicates(l);
    print_list(l, output_file, false);

    return 0;
}


