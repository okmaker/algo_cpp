//  Luogu contest  B3609 [图论与代数结构 701] 强连通分量
//  Created at 31.01.2026 11:53:52 in file B3609__701_.cpp
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
/*
    
*/
struct SCC {
    int n;                          // 节点数量
    vector<vector<int>> adj; // 原图邻接表

    // Tarjan 算法相关变量
    vector<int> dfn, low;      // 时间戳，能追溯到的最早时间戳
    vector<int> scc_id;        // 每个点所属的 SCC 编号 (0 ~ scc_cnt-1)
    vector<int> scc_size;      // 每个 SCC 的大小 (点数)
    stack<int> stk;            // 栈
    vector<bool> in_stk;       // 是否在栈中
    int timer;                      // 时间戳计数器
    int scc_cnt;                    // 强连通分量计数器

    // 构造函数：初始化
    explicit SCC(int _n) : n(_n), adj(_n) {}

    // 加边 (u -> v)
    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    // 核心 Tarjan DFS
    void dfs(int u) {
        dfn[u] = low[u] = ++timer;
        stk.push(u);
        in_stk[u] = true;

        for (int v : adj[u]) {
            if (!dfn[v]) {
                dfs(v);
                low[u] = std::min(low[u], low[v]);
            } else if (in_stk[v]) {
                low[u] = std::min(low[u], dfn[v]);
            }
        }

        // 找到一个 SCC
        if (low[u] == dfn[u]) {
            int v;
            scc_size.push_back(0); // 新增一个分量大小计数
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
     * @brief 构建缩点后的 DAG (有向无环图)
     * @return 返回一个新的  vector<vector<int>> 邻接表，包含 scc_cnt 个节点
     * 注意：新图中的节点 u 对应原图中的 SCC 编号 u
     * 并且会自动去重边 (例如原图 u->v, x->y 若 u,x 同属 SCC A, v,y 同属 SCC B，则 A->B 只加一次)
     */
    vector<vector<int>> condense() const {
        vector<vector<int>> dag(scc_cnt);
        std::vector<std::map<int, bool>> seen_edges(scc_cnt);

        for (int u = 0; u < n; u++) {
            for (int v : adj[u]) {
                int u_scc = scc_id[u];
                int v_scc = scc_id[v];

                if (u_scc != v_scc) {
                    // 避免重复加边
                    if (!seen_edges[u_scc].contains(v_scc)) {
                        dag[u_scc].push_back(v_scc);
                        seen_edges[u_scc][v_scc] = true;
                    }
                }
            }
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
};
void solve(){
    int n,m; cin >> n >> m;
    SCC scc(n);
    for (int i = 0; i < m; i++) {
        int u,v; cin >> u >> v;u--,v--;
        scc.add_edge(u,v);
    }
    scc.run();
    cout << scc.scc_cnt << endl;
    vector<vector<int>> dag = scc.build_scc_graph();
    vector<int> vis(scc.scc_cnt,false);
    for (int i = 0; i < n; i++) {
        int id = scc.scc_id[i];
        if (!vis[id]) {
            vis[id] = true;
            sort(dag[id].begin(),dag[id].end());
            for (int v : dag[id]) cout << v + 1  << " ";
            cout << endl;
        }
    }
}
signed main() {
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int T = 1;//cin >> T;
    while(T--){
        solve();
    }
    
    return 0;
}