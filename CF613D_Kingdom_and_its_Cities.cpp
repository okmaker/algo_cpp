//  Luogu contest  CF613D Kingdom and its Cities
//  Created at 07.02.2026 17:35:32 in file CF613D_Kingdom_and_its_Cities.cpp
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
#include <bits/stdc++.h>
using namespace std;

// 封装 LCA 和 树的基础信息
class TreeBase {
public:
    int n;
    int timer;
    vector<vector<int>> adj;
    vector<vector<int>> up; // 倍增数组
    vector<int> dfn;
    vector<int> depth;
    int LOG;

    TreeBase(int n) : n(n), timer(0), adj(n) {
        LOG = 32 - __builtin_clz(n) + 1; // 自动计算 log2(n)
        up.assign(n, vector<int>(LOG, 0));
        dfn.resize(n);
        depth.assign(n, 0);
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void build(int root = 0) {
        dfs_build(root, root);
    }

    int get_lca(int u, int v) const {
        if (depth[u] < depth[v]) swap(u, v);
        for (int i = LOG - 1; i >= 0; --i) {
            if (depth[u] - (1 << i) >= depth[v]) {
                u = up[u][i];
            }
        }
        if (u == v) return u;
        for (int i = LOG - 1; i >= 0; --i) {
            if (up[u][i] != up[v][i]) {
                u = up[u][i];
                v = up[v][i];
            }
        }
        return up[u][0];
    }

    // 获取 u 的父节点（用于特殊判断）
    int get_parent(int u) const {
        return u == up[u][0] ? -1 : up[u][0]; // 根节点的父节点设为 -1
    }

private:
    void dfs_build(int u, int p) {
        dfn[u] = ++timer;
        depth[u] = depth[p] + 1;
        up[u][0] = p;
        for (int i = 1; i < LOG; ++i) {
            up[u][i] = up[up[u][i - 1]][i - 1];
        }
        for (int v : adj[u]) {
            if (v != p) dfs_build(v, u);
        }
    }
};

// 封装虚树构建器
class VirtualTreeSolver {
    const TreeBase& tree; // 引用原树信息
    vector<vector<int>> vt_adj; // 虚树邻接表
    vector<int> used_nodes; // 记录当前虚树涉及的节点

public:
    VirtualTreeSolver(const TreeBase& t) : tree(t), vt_adj(t.n) {}

    // 清理上一轮虚树
    void clear() {
        for (int u : used_nodes) {
            vt_adj[u].clear();
        }
        used_nodes.clear();
    }

    // 构建虚树，返回根节点
    // 传入的 nodes 会被修改（排序/去重）
    int build(vector<int>& nodes) {
        // 1. 按 DFN 排序
        sort(nodes.begin(), nodes.end(), [&](int a, int b) {
            return tree.dfn[a] < tree.dfn[b];
        });

        // 2. 加入 LCA
        int m = nodes.size();
        for (int i = 0; i < m - 1; ++i) {
            nodes.push_back(tree.get_lca(nodes[i], nodes[i+1]));
        }

        // 3. 再次排序去重
        sort(nodes.begin(), nodes.end(), [&](int a, int b) {
            return tree.dfn[a] < tree.dfn[b];
        });
        nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());

        // 4. 连边
        for (int i = 0; i < (int)nodes.size() - 1; ++i) {
            int u = tree.get_lca(nodes[i], nodes[i+1]);
            int v = nodes[i+1];
            vt_adj[u].push_back(v);
        }

        // 记录用到的点，方便下一次 O(K) 清空
        used_nodes = nodes;

        return nodes[0];
    }

    // 暴露虚树邻接表给外部 DP 使用
    const vector<vector<int>>& get_graph() const {
        return vt_adj;
    }
};

// --- 解题逻辑 ---
void solve() {
    int n; cin >> n;
    TreeBase tree(n); // 实例化原树
    for (int i = 0; i < n - 1; ++i) {
        int u, v; cin >> u >> v;
        tree.add_edge(u - 1, v - 1);
    }
    tree.build(0); // 预处理 LCA

    VirtualTreeSolver vt_solver(tree); // 实例化虚树求解器
    vector<bool> is_key(n, false);

    int q; cin >> q;
    while (q--) {
        int k; cin >> k;
        vector<int> h(k);
        for (int& x : h) {
            cin >> x; x--;
            is_key[x] = true;
        }

        // 预判 -1 的情况
        bool possible = true;
        for (int u : h) {
            int p = tree.get_parent(u);
            if (p != -1 && is_key[p]) {
                possible = false;
                break;
            }
        }

        if (!possible) {
            cout << -1 << "\n";
            for (int x : h) is_key[x] = false;
            continue;
        }

        // 构建虚树
        vt_solver.clear();
        int root = vt_solver.build(h);
        const auto& vg = vt_solver.get_graph();

        int ans = 0;

        auto dfs = [&](auto&& self, int u) -> int {
            int cnt = 0;
            for (int v : vg[u]) {
                cnt += self(self, v);
            }

            if (is_key[u]) {
                ans += cnt;
                return 1;
            } else {
                if (cnt > 1) {
                    ans++;
                    return 0;
                }
                return cnt;
            }
        };

        dfs(dfs, root);
        cout << ans << "\n";

        for(int x : h) is_key[x] = false;
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