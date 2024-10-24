#include "iostream"
#include "fstream"
#include "stack"
#include "string"
#include "cstring"

int T;
int n;
int w[100];
int flag[100];
int num_sol = 0;
std::string input_filename;
std::string output_filename;

void print() {
    std::ofstream output_file;
    if (num_sol == 0) {
        output_file.open(output_filename);
    } else {
        output_file.open(output_filename, std::ios::app);
    }
    if (!output_file) {
        std::cerr << "Error: Unable to open the output file." << std::endl;
        exit(1);
    }

    output_file << "解法" << num_sol << ": \n";
    for (int i = 0; i < n; i++) {
        if (flag[i]) {
            output_file << w[i] << " ";
        }
    }
    output_file << "\n\n";
    output_file.close();
}

void knapsack(int rest, int index) {
    /*
     * @param rest 背包剩余容量
     * @param index 当前尝试放入的物品编号
    */
    // 背包已装满, 找到一组解
    if (rest == 0) {
        print();
        num_sol++;
        return;
    }
    // 背包装不下了, 或物品已遍历完
    if (rest < 0 || index >= n) {
        return;
    }
    // 背包可以装得下当前物品
    if (w[index] <= rest) {
        flag[index] = 1;
        knapsack(rest - w[index], index + 1);  // 放入当前物品
        flag[index] = 0;  // 取出当前物品(回溯)
    }
    knapsack(rest, index + 1);  // 不放入当前物品
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }
    input_filename = argv[1];
    output_filename = argv[2];
    std::ifstream input_file(input_filename);
    if (!input_file) {
        std::cerr << "Error: Unable to open the input file." << std::endl;
        exit(1);
    }
    input_file >> T;
    input_file >> n;
    for (int i = 0; i < n; i++) {
        input_file >> w[i];
    }
    input_file.close();
    memset(flag, 0, sizeof(int) * n);
    knapsack(T, 0);
    return 0;
}