#include "vector"
#include "list"
#include "utility"
#include "iostream"
#include "queue"
#include "cstring"
#include "stack"
#include "fstream"
#include "string"

using namespace std;

class Graph {
public:
    Graph(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "Unable to open the file." << endl;
            exit(1);
        }
        file >> num_vertices;  // 第一行输入顶点数与边数
        file >> num_edges;
        mat.resize(num_vertices, vector<int>(num_vertices, 0));
        adj = new list <pair<int, int>>[num_vertices];
        int u, v, w;
        for (int i = 0; i < num_edges; i++) {  // 剩下E行输入u, v, w (边的两个顶点与边权值)
            file >> u >> v >> w;
            add_edge(u, v, w);
        }
        file >> dijkstra_start;  // 最后一行输入dijkstra算法的源顶点
        file.close();
    }

    // 在uv之间连一条权为weight的边
    void add_edge(int u, int v, int weight = 1) {
        mat[u][v] = mat[v][u] = weight;
        adj[u].push_back({v, weight});
        adj[v].push_back({u, weight});
    }

    // 打印邻接矩阵
    void print_mat() {
        cout << "邻接矩阵为:" << endl;
        for (auto row: mat) {
            for (auto element: row) {
                cout << element << " ";
            }
            cout << endl;
        }
    }

    // 打印邻接表
    void print_adj() {
        cout << "邻接表为:" << endl;
        for (int u = 0; u < num_vertices; u++) {
            cout << u << ": ";
            for (auto [v, w]: adj[u]) {
                cout << "(" << v  << "," << w << ")" << " ";
            }
            cout << endl;
        }
    }

    // 非递归深度优先搜索
    void dfs(int v) {
        cout << "图的深度优先搜索序列为: ";
        int visited[num_vertices];  // 标记顶点是否被访问
        memset(visited, 0, sizeof(visited));
        stack<int> s;
        s.push(v);  // 将起点v入栈
        while (!s.empty()) {
            auto curr = s.top();  // 弹出栈顶顶点
            s.pop();
            if (!visited[curr]) {  // 当前顶点未被访问过
                cout << curr << " ";  // 访问当前顶点
                visited[curr] = 1;
                for (auto [neighbor, w]: adj[curr]) {
                    if (!visited[neighbor]) {
                        s.push(neighbor);  // 将当前顶点的所有未被访问的邻接顶点入栈
                    }
                }
            }
        }
        cout << endl;
    }

    // 广度优先搜索
    void bfs(int v) {
        cout << "图的广度优先搜索序列为: ";
        int visited[num_vertices];  // 标记顶点是否被访问
        memset(visited, 0, sizeof(visited));
        queue<int> q;
        visited[v] = 1;
        q.push(v);  // 将当前顶点入队
        while (!q.empty()) {
            auto curr = q.front();  // 弹出队首顶点
            q.pop();
            cout << curr << " ";  // 访问当前顶点
            for (auto [neighbor, w]: adj[curr]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = 1;
                    q.push(neighbor);  // 将未访问过的邻顶入队
                }
            }
        }
        cout << endl;
    }

    // prim算法求最小生成树
    void prim() {
        if (num_vertices == 0) {
            return;
        }

        struct EdgeEntry {
            int u;  // 在U中的端点
            int v;  // 在V中的端点
            int weight;  // 边权
            bool operator<(const EdgeEntry &other) const {
                return weight > other.weight;
            }
        };

        vector<pair<int, int>> tree;
        priority_queue <EdgeEntry> q;
        int min_edge_weight[num_vertices];  // 每个顶点到已选择顶点子集U中的边的最小边权
        int selected[num_vertices];  // 每个顶点是否已被选入MST
        int mst_weight = 0;  // 最小生成树的总权值
        int count = 0;  // 已加入最小生成树的顶点数
        memset(min_edge_weight, 0x3f, sizeof(min_edge_weight));
        memset(selected, 0, sizeof(selected));

        selected[0] = 1;  // 将顶点1加入U
        for (const auto &[v, weight]: adj[0]) {  // 将顶点0的邻顶入队
            q.push({0, v, weight});
        }

        while (!q.empty()) {
            auto [u, v, weight] = q.top();  // 在(U,V)中选择一条权最小的边
            q.pop();
            if (selected[v]) {
                continue;
            }
            selected[v] = 1;
            tree.push_back({u, v});
            count++;
            mst_weight += weight;

            for (const auto &[neighbor, w]: adj[v]) {
                if (w < min_edge_weight[neighbor]) {
                    min_edge_weight[neighbor] = w;
                    q.push({v, neighbor, w});
                }
            }
        }

        if (count < num_vertices - 1) {
            cout << "此图不连通, 不存在生成树." << endl;
        } else {
            cout << "最小生成树由这些边组成: ";
            for (auto [u, v]: tree) {
                cout << u << "-" << v << " ";
            }
            cout << "总边权为: " << mst_weight << endl;
        }
    }

    // 单源最短路径
    void dijkstra() {
        int dist[num_vertices];
        int marked[num_vertices];
        int prev[num_vertices];
        priority_queue < pair < int, int >, vector < pair < int, int >>, greater < pair < int, int>>> q;
        memset(dist, 0x3f, sizeof(dist));  // 距离初始化为无穷大
        memset(marked, 0, sizeof(marked));
        memset(prev, -1, sizeof(prev));  // 前驱顶点初始化为-1
        dist[dijkstra_start] = 0;  // v到自身距离为0
        q.push({0, dijkstra_start});

        while (!q.empty()) {
            auto [d, curr] = q.top();  // 取dist最小且未被mark的顶点
            q.pop();
            if (marked[curr]) {
                continue;
            }
            marked[curr] = 1;
            for (const auto &[neighbor, weight]: adj[curr]) {  // 更新curr的所有未被mark的邻顶的距离
                if (!marked[neighbor] && dist[curr] + weight < dist[neighbor]) {
                    dist[neighbor] = dist[curr] + weight;
                    q.push({dist[neighbor], neighbor});
                    prev[neighbor] = curr;  // 更新前驱顶点
                }
            }
        }

        // 打印最短路径
        for (int u = 0; u < num_vertices; u++) {
            bool reachable = true;
            if (u == dijkstra_start) {
                continue;
            }
            cout << dijkstra_start << "到" << u << "的最短路径为: ";
            stack<int> path;
            path.push(u);
            while (path.top() != dijkstra_start) {
                if (prev[path.top()] == -1) {  // 前驱顶点为-1, 说明不可达
                    reachable = false;
                    break;
                }
                path.push(prev[path.top()]);
            }
            if (!reachable) {
                cout << "不可达" << endl;
                continue;
            }
            while (!path.empty()) {  // 打印路径
                cout << path.top() << " ";
                path.pop();
            }
            cout << ", 长度为" << dist[u] << endl;
        }
    }

private:
    vector <vector<int>> mat;  // 邻接矩阵
    list <pair<int, int>> *adj;  // 邻接表
    int num_vertices;  // 顶点数
    int num_edges;  // 边数
    int dijkstra_start;  // dijkstra算法的源顶点

};

int main() {
    Graph g("in.txt");
    g.print_mat();
    g.print_adj();
    g.dfs(0);
    g.bfs(0);
    g.dijkstra();
    g.prim();
    return 0;
}