//  Luogu contest  P6041 「ACOI2020」布丁暗杀计划
//  Created at 10.01.2026 09:37:22 in file P6041_ACOI2020.cpp
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
const double eps = 1e-7;
int dx[] =  {0,0,1,-1};
int dy[] = {1,-1,0,0};
const int MOD = 1e9 + 7;
const int N = 1e5 + 5;
const int M = 1e5 + 7;
class TreeAncestor {
public:
    int n;
    int max_log;
    vector<int> depth;
    vector<vector<int>> up;

    TreeAncestor(int n, const vector<vector<int>>& adj, int root = 0) : n(n) {
        max_log = std::log2(n) + 1;
        depth.assign(n, 0);
        up.assign(n, std::vector<int>(max_log, -1));

        // 预处理：DFS 计算深度和第一级祖先
        dfs(root, -1, 0, adj);

        // 动态规划：预处理倍增表 up[i][j] 表示 i 的第 2^j 个祖先
        for (int j = 1; j < max_log; ++j) {
            for (int i = 0; i < n; ++i) {
                if (up[i][j - 1] != -1) {
                    up[i][j] = up[up[i][j - 1]][j - 1];
                }
            }
        }
    }

    // 向上跳 k 步 (0-indexed: k=0 返回自己)
    int get_kth_ancestor(int u, int k) const {
        if (k > depth[u]) return -1;
        for (int j = 0; j < max_log; ++j) {
            if ((k >> j) & 1) {
                u = up[u][j];
                if (u == -1) break;
            }
        }
        return u;
    }

    // 求最近公共祖先
    int get_lca(int u, int v) const {
        if (depth[u] < depth[v]) std::swap(u, v);

        // 1. 先跳到同一深度
        u = get_kth_ancestor(u, depth[u] - depth[v]);
        if (u == v) return u;

        // 2. 二进制尝试向上跳
        for (int j = max_log - 1; j >= 0; --j) {
            if (up[u][j] != up[v][j]) {
                u = up[u][j];
                v = up[v][j];
            }
        }
        return up[u][0];
    }

private:
    void dfs(int u, int p, int d, const std::vector<std::vector<int>>& adj) {
        depth[u] = d;
        up[u][0] = p;
        for (int v : adj[u]) {
            if (v != p) {
                dfs(v, u, d + 1, adj);
            }
        }
    }
};
template <typename Graph>
class DSUOnTree {
public:
    int n;
    const Graph& g;
    std::vector<int> siz, son,depth;


    DSUOnTree(int n, const Graph& g) : n(n), g(g), siz(n), son(n, -1),depth(n) {}

    // 第一遍 DFS：预处理 siz 和 son
    void prepare(int u, int fa) {
        siz[u] = 1;
        son[u] = -1;
        if (fa != -1) depth[u] = depth[fa] + 1;
        for (auto v : g[u]) {
            if (v == fa) continue;
            prepare(v, u);
            siz[u] += siz[v];
            if (son[u] == -1 || siz[v] > siz[son[u]]) son[u] = v;
        }
    }

    // 核心 DFS 逻辑：利用模板参数 Add, Del, Ans 实现零开销回调
    template <typename Add, typename Del, typename Ans>
    void solve(int u, int fa, bool keep, Add&& add, Del&& del, Ans&& ans) {
        // 1. 递归处理轻儿子
        for (auto v : g[u]) {
            if (v == fa || v == son[u]) continue;
            solve(v, u, false, add, del, ans);
        }

        // 2. 递归处理重儿子
        if (son[u] != -1) {
            solve(son[u], u, true, add, del, ans);
        }

        // 3. 暴力加入轻儿子及其子树
        for (auto v : g[u]) {
            if (v == fa || v == son[u]) continue;
            update_recursive(v, u, add);
        }
        add(u); // 加入当前节点

        // 4. 记录答案
        ans(u);

        // 5. 是否清除
        if (!keep) {
            update_recursive(u, fa, del);
        }
    }

private:
    // 辅助递归函数
    template <typename Updater>
    void update_recursive(int u, int fa, Updater&& updater) {
        updater(u);
        for (auto v : g[u]) {
            if (v == fa) continue;
            update_recursive(v, u, updater);
        }
    }
};
/*
    一个问题（u,k）,如果有k级祖先，问题挂在k级祖先上。
    需要维护一个（深度，颜色）为key，查询个数的结构。
*/
void solve(){
    int n,q; cin >> n >> q;
    vector<int> colors(n),ds(n);
    for (int i = 0; i < n; i++) cin >> colors[i];
    for (int i = 0; i < n; i++) cin >> ds[i];
    vector<vector<int>> g(n);
    for (int i = 1; i <= n - 1; i++) {
        int fa; cin >> fa;fa--;
        g[fa].push_back(i);
    }
    TreeAncestor treeAncestor(n,g);
    DSUOnTree dsu(n,g);dsu.prepare(0,-1);
    //询问挂图
    vector<int> res(q);
    vector<vector<pii>> qs(n);
    for (int i = 0; i < q; i++) {
        int u,k; cin >> u >> k;
        u--;
        int a = treeAncestor.get_kth_ancestor(u,k);
        if (a != -1) qs[a].push_back({k,i});
    }
    map<pii,int> mp;//(层高，颜色)
    auto add = [&](int u){
        mp[{dsu.depth[u],colors[u]}] += ds[u];
    };
    auto del = [&](int u){
        mp.clear();
    };
    auto getAns = [&](int u){
        int dep = dsu.depth[u];
        for (auto [k,i] : qs[u]) {
            res[i] = mp[{dep + k,colors[u]}];
        }
    };
    dsu.solve(0,-1,false,add,del,getAns);
    for (int i = 0; i < q; i++) {
        cout << res[i] << endl;
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