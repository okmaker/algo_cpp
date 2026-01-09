//  Luogu contest  CF600E Lomsat gelral
//  Created at 09.01.2026 13:46:24 in file CF600E_Lomsat_gelral.cpp
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
public:
    int n;
    const Graph& g;
    std::vector<int> siz, son;


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
    维护颜色计数信息，最大值个数信息，当一个节点加入的时候，
        如果比最大大，更新最大值，res = 当前编号
        如果一样大，res += 当前编号
    删除的时候，清空计数，res = 0
 */
void solve(){
    int n; cin >> n;
    vector<int> colors(n);
    for (int i = 0; i < n; i++) cin >> colors[i];
    vector<vector<int>> g(n);
    for (int i = 1; i < n; i++) {
        int u,v; cin >> u >> v; u--,v--;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    DSUOnTree dsu(n, g);
    dsu.prepare(0, -1);
    vector<int> res(n),colorCnts(n + 1);
    int maxCnt = 0,tot = 0;
    auto add = [&](int u) {
        int c = colors[u];
        colorCnts[c]++;
        if (colorCnts[c] > maxCnt) {
            maxCnt = colorCnts[c];
            tot = c;
        }else if (colorCnts[c] == maxCnt) {
            tot += c;
        }
    };
    auto del = [&](int u) {
        int c = colors[u];
        colorCnts[c]--;
        tot = 0;
        maxCnt = 0;
     };
    auto ans = [&](int u) {
        res[u] = tot;
    };
    dsu.solve(0,-1,false,add,del,ans);
    for (int i = 0; i < n; i++) {
        cout << res[i] << " ";
    }
    cout << endl;
}
signed main() {
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int T = 1;//cin >> T;
    while(T--){
        solve();
    }
    
    return 0;
}