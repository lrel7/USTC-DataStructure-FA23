#include "iostream"
#include "fstream"
#include "stack"

int n;
int is_recursion;
int a[20];  // a[i]表示第i个皇后在第i行, 第a[i]列
int num_sol = 0;
std::string filename;  // 输出文件名

// 判断第k个皇后与前面k个是否冲突
bool is_valid(int k) {
    for (int i = 0; i < k; i++) {
        if (a[i] == a[k] || abs(a[k] - a[i]) == k - i) {  // 是否在同一列或同一对角线
            return false;
        }
    }
    return true;
}

void print() {
    std::ofstream output_file;
    if (num_sol == 0) {
        output_file.open(filename);
    } else {
        output_file.open(filename, std::ios::app);
    }
    if (!output_file) {
        std::cerr << "Error: Unable to open the output file." << std::endl;
        exit(1);
    }

    output_file << "解法" << num_sol << ": \n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < a[i]; j++) {
            output_file << "0 ";
        }
        output_file << "1 ";
        for (int j = a[i] + 1; j < n; j++) {
            output_file << "0 ";
        }
        output_file << "\n";
    }
    output_file << "\n";
    output_file.close();
}

void nqueens_recursion(int k) {
    if (k == n) {
        // 找到了一种新的解法
        print();
        num_sol++;
        return;
    }
    for (int i = 0; i < n; i++) {  // 遍历列
        a[k] = i;  // 第`k`个皇后放在第`i`列
        if (is_valid(k)) {
            // 若成功放置, 继续放置下一个
            nqueens_recursion(k + 1);
        }
    }
}

void nqueens_non_recursion() {
    int row = 0, col = 0;  // row: 当前正在放置的行, col: 将当前行皇后放置的列
    while (row < n) {
        while (col < n) {
            a[row] = col;
            if (is_valid(row)) {
                // 成功放置
                break;
            }
            a[row] = -1;  // 表示尚未放置成功
            col++;
        }
        // 当前皇后放置失败, 需要回溯前一行
        if (a[row] == -1) {
            // 已经回溯完第0行了, 说明全部解已经找到, 退出
            if (row == 0) {
                break;
            }
            row--;
            col = a[row] + 1;  // 继续向右尝试
            a[row] = -1;
            continue;
        }
        // 找到一种新的解法
        if (row == n - 1) {
            print();
            num_sol++;
            col = a[row] + 1;  // 在当前行继续向右尝试, 寻求更多解法
            a[row] = -1;
            continue;
        }
        // 当前皇后放置成功, 但尚未完成, 进入下一行
        row++;
        col = 0;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        // std::cerr << "Usage: " << argv[0] << " <n> <is_recursion>" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <input_file> <is_recursion>" << std::endl;
        return 1;
    }
    // n = std::atoi(argv[1]);

    std::ifstream input_file(argv[1]);
    if (!input_file) {
        std::cerr << "Error: Unable to open the input file." << std::endl;
        return 1;
    }
    input_file >> n;
    input_file.close();

    is_recursion = std::atoi(argv[2]);

    if (is_recursion) {
        filename = "recursion_" + std::to_string(n) + ".txt";
        nqueens_recursion(0);
    } else {
        filename = "non-recursion_" + std::to_string(n) + ".txt";
        nqueens_non_recursion();
    }
    return 0;
}