//  Luogu contest  U41492 树上数颜色
//  Created at 09.01.2026 11:54:51 in file U41492_.cpp
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
/*
    
*/
template <typename Graph>
class DSUOnTree {
    int n;
    const Graph& g;
    std::vector<int> siz, son;

public:
    DSUOnTree(int n, const Graph& g) : n(n), g(g), siz(n), son(n, -1) {}

    // 第一遍 DFS：预处理 siz 和 son
    void prepare(int u, int fa) {
        siz[u] = 1;
        son[u] = -1;
        for (auto v : g[u]) {
            if (v == fa) continue;
            prepare(v, u);
            siz[u] += siz[v];
            if (son[u] == -1 || siz[v] > siz[son[u]]) son[u] = v;
        }
    }

    // 递归更新子树贡献 (通用模板实现)
    // template <typename Updater>
    // void update_subtree(int u, int fa, int skip_son, Updater&& updater) {
    //     updater(u);
    //     for (auto v : g[u]) {
    //         if (v == fa || v == skip_son) continue;
    //         update_subtree(v, fa, skip_son, updater); // 注意这里传 fa 还是 u 取决于是否有向图
    //         // 修正：对于通用的树遍历，应当传当前节点 u 作为下一个 v 的 fa
    //         update_subtree(v, u, skip_son, updater);
    //     }
    // }

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
void solve(){
    int n; cin >> n;
    vector<vector<int>> g(n);
    for (int i = 1; i < n; i++) {
        int u,v; cin >> u >> v; u--,v--;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    vector<int> colors(n);
    for (int i = 0; i < n; i++) cin >> colors[i];
    DSUOnTree dsu(n, g);
    dsu.prepare(0, -1);
    vector<int> cnt(n + 1, 0), res(n);
    int diff = 0;

    // 直接传入 Lambda，编译器会生成特定版本的 solve 函数
    dsu.solve(
        0, -1, true,
        [&](int u) { if (++cnt[colors[u]] == 1) diff++; }, // Add
        [&](int u) { if (--cnt[colors[u]] == 0) diff--; }, // Del
        [&](int u) { res[u] = diff; }                      // Ans
    );
    //回答询问
    int m; cin >> m;
    for (int i = 0,x; i < m; i++) {
        cin >> x; x--;
        cout << res[x] << endl;
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