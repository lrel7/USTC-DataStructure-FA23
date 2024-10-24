#include "iostream"

struct Node {
    int val;
    Node* next;
    Node(): val(0), next(nullptr) {}
};

// 创建一个长度为n的单链表
Node* create_list(int n) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int i = 0; i < n; i++) {
        Node* new_node = new Node();
        if (i == 0) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }
    return head;
}

// 创建一个长度为n的环
Node* create_cycle(int n) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int i = 0; i < n; i++) {
        Node* new_node = new Node();
        if (i == 0) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }
    tail->next = head;
    return head;
}

// 将单链表和环连接成一个含环的链表
void concatenate(Node* head, Node* cycle_entry) {
    auto cur = head;
    while (cur && cur->next) {
        cur = cur->next;
    }
    cur->next = cycle_entry;
}

void detect_cycle(Node* head) {
    auto fast = head, slow = head;
    int s = 0;  // 环的入口到头结点的距离
    int l = 0;  // 环的长度
    while (1) {
        slow = slow->next;
        fast = fast->next;
        fast = fast->next;
        if (fast == slow) {
            break;
        }
    }
    // 让fast指针遍历一次环, 计算环的长度
    while (1) {
        fast = fast->next;
        l++;
        if (fast == slow) {
            break;
        }
    }
    std::cout << "环的长度为: " << l << std::endl;
    // 设fast指针和slow指针第一次相遇点距离环的入口长度为x, 此时二者分别绕环n,m圈
    // 则有s + nl + x = 2(s + ml + x) -> s = (n - 2m)l - x
    // 从而, 让fast指针从起点出发, 两者同步移动, 相遇点即为入口
    fast = head;
    while (fast != slow) {
        fast = fast->next;
        slow = slow->next;
        s++;
    }
    std::cout << "环的入口到头结点的距离为: " << s << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <n_list> <n_cycle>" << std::endl;
        return 1;
    }
    int n_list = std::atoi(argv[1]);
    int n_cycle = std::atoi(argv[2]);
    auto head = create_list(n_list);
    auto cycle_entry = create_cycle(n_cycle);
    concatenate(head, cycle_entry);
    detect_cycle(head);
    return 0;
}
