//  Luogu contest  P2812 校园网络【[USACO]Network of Schools加强版】
//  Created at 01.02.2026 20:27:57 in file P2812_USACONetwork_of_Schools.cpp
//  By zfq
#include <bits/stdc++.h>
// #include <bits/extc++.h>
#define LIMIT 22
#define inf 0x3f3f3f3f3f3f
#define endl '\n'

#define int long long
using namespace std;
//using namespace __gnu_pbds;
//using ordered_set = tree<int, null_type, less<>, rb_tree_tag, tree_order_statistics_node_update>;

using pii = pair<int,int>;
using v2d = vector<vector<int>>;
constexpr double eps = 1e-7;
constexpr int dx[] =  {0,0,1,-1};
constexpr int dy[] = {1,-1,0,0};
constexpr int MOD = 1e9 + 7;
constexpr int N = 1e5 + 5;
constexpr int M = 1e5 + 7;

struct SCC {
    int n;                      // 节点数量
    vector<vector<int>> adj;    // 原图邻接表

    // Tarjan 算法相关变量
    vector<int> dfn, low;       // 时间戳
    vector<int> scc_id;         // 每个点所属的 SCC 编号 (0 ~ scc_cnt-1)
    vector<int> scc_size;       // 每个 SCC 的大小 (点数)
    stack<int> stk;
    vector<bool> in_stk;
    int timer;
    int scc_cnt;

    vector<int> scc_in;         // 缩点后，每个 SCC 的入度
    vector<int> scc_out;        // 缩点后，每个 SCC 的出度

    explicit SCC(int _n) : n(_n), adj(_n) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    void dfs(int u) {
        dfn[u] = low[u] = ++timer;
        stk.push(u);
        in_stk[u] = true;

        for (int v : adj[u]) {
            if (!dfn[v]) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (in_stk[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }

        if (low[u] == dfn[u]) {
            int v;
            scc_size.push_back(0);
            do {
                v = stk.top();
                stk.pop();
                in_stk[v] = false;
                scc_id[v] = scc_cnt;
                scc_size[scc_cnt]++;
            } while (u != v);
            scc_cnt++;
        }
    }
    // 执行算法
    void run() {
        dfn.assign(n, 0);
        low.assign(n, 0);
        scc_id.assign(n, -1);
        in_stk.assign(n, false);
        scc_size.clear();
        timer = 0;
        scc_cnt = 0;
        while (!stk.empty()) stk.pop();

        for (int i = 0; i < n; i++) {
            if (!dfn[i]) dfs(i);
        }
    }

    /**
     * @brief 构建缩点后的 DAG 并计算出入度
     * @return 返回新的 DAG 邻接表
     *  由于去重边，复杂度: O(M log M)
     * 同时会自动填充 scc_in 和 scc_out 数组
     */
    vector<vector<int>> condense() {
        vector<vector<int>> dag(scc_cnt);
        scc_in.assign(scc_cnt, 0);
        scc_out.assign(scc_cnt, 0);

        vector<pair<int, int>> edges;
        for (int u = 0; u < n; u++) {
            for (int v : adj[u]) {
                if (scc_id[u] != scc_id[v]) {
                    edges.push_back({scc_id[u], scc_id[v]});
                }
            }
        }

        sort(edges.begin(), edges.end());
        edges.erase(unique(edges.begin(), edges.end()), edges.end());

        for (auto& e : edges) {
            int u = e.first;
            int v = e.second;
            dag[u].push_back(v);

            // 统计出入度
            scc_out[u]++;
            scc_in[v]++;
        }

        return dag;
    }
    /**
     * @brief 构建每一个强联通分量具体有哪些点
     * @return  返回一个 vector<vector<int>> 邻接表，包含 scc_cnt 个节点
     */
    vector<vector<int>> build_scc_graph() const {
        vector<vector<int>> sccG(scc_cnt);
        for (int i = 0; i < n; i++) {
            sccG[scc_id[i]].push_back(i);
        }
        return sccG;
    }

    /**
     *
     * @brief 用于多测。
     */
    void init(int _n) {
        n = _n;
        adj.assign(n, vector<int>());
        dfn.assign(n, 0);
        while (!stk.empty()) stk.pop();
    }
};
void solve(){
    int n; cin >> n;
    SCC scc(n);
    for (int i = 1; i <= n; i++) {
        int x; cin >> x;
        while (x != 0) {
            scc.add_edge(i - 1,x - 1);
            cin >> x;
        }

    }
    scc.run();
    auto dag = scc.condense();
    auto scc_in = scc.scc_in,scc_out = scc.scc_out;
    //选入度为0的
    int in = 0,out = 0;
    for (int i = 0; i < scc_in.size(); i++) {
        if (scc_in[i] == 0) in++;
        if (scc_out[i] == 0) out++;
    }
    cout << in << endl;
    cout << (scc_in.size() == 1 ? 0 : max(in,out)) << endl;
}
signed main() {
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int T = 1;//cin >> T;
    while(T--){
        solve();
    }
    
    return 0;
}