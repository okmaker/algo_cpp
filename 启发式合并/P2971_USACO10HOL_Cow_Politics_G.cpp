//  Luogu contest  P2971 [USACO10HOL] Cow Politics G
//  Created at 09.01.2026 23:41:03 in file P2971_USACO10HOL_Cow_Politics_G.cpp
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
template <typename Graph>
class DSUOnTree {
public:
    int n;
    const Graph& g;
    std::vector<int> siz, son, depth;
    std::vector<long long> dist; // 额外增加：存储到根的路径边权和

    DSUOnTree(int n, const Graph& g) : n(n), g(g), siz(n), son(n, -1), depth(n), dist(n) {}

    void prepare(int u, int fa, long long d, int dep) {
        siz[u] = 1;
        son[u] = -1;
        dist[u] = d;
        depth[u] = dep;
        for (auto& v : g[u]) {
            int w = 1;
            if (v == fa) continue;
            prepare(v, u, d + w, dep + 1);
            siz[u] += siz[v];
            if (son[u] == -1 || siz[v] > siz[son[u]]) son[u] = v;
        }
    }


    template <typename Add, typename Del, typename Calc>
    void solve(int u, int fa, bool keep, Add&& add, Del&& del, Calc&& calc) {

        for (auto& v : g[u]) {
            if (v == fa || v == son[u]) continue;
            solve(v, u, false, add, del, calc);
        }


        if (son[u] != -1) {
            solve(son[u], u, true, add, del, calc);
        }

        calc(u,u);
        for (auto& v : g[u]) {
            if (v == fa || v == son[u]) continue;
            calc_recursive(v, u, u, calc);
            update_recursive(v, u, add);
        }
        add(u);


        if (!keep) {
            update_recursive(u, fa, del);
        }
    }

private:
    template <typename Updater>
    void update_recursive(int u, int fa, Updater&& updater) {
        updater(u);
        for (auto& v : g[u]) {
            if (v != fa) update_recursive(v, u, updater);
        }
    }

    template <typename Calculator>
    void calc_recursive(int u, int fa, int root, Calculator&& calc) {
        calc(u, root);
        for (auto& v : g[u]) {
            if (v != fa) calc_recursive(v, u, root, calc);
        }
    }
};
/*
    对于每一个政党，记录政党的最远距离。
*/
void solve(){
    int n,k; cin >> n >> k;
    vector<int> type(n);
    vector<vector<int>> g(n);
    int root = -1;
    for (int i = 0; i < n; i++) {
        int a,p;cin >> a >> p; p--;
        if (p != -1) g[p].push_back(i);
        else root = i;
        type[i] = a;
    }
    DSUOnTree dsu(n,g); dsu.prepare(root,-1,0,0);
    vector<int> res(k + 1,0);
    map<int,int> mp;
    auto add = [&](int u){
        mp[type[u]] = max(mp[type[u]],dsu.depth[u]);
    };
    auto del = [&](int u){
        mp.clear();
    };
    auto calc = [&](int u,int root) {
        // depth[u]
        if (mp.contains(type[u]))
            res[type[u]] = max(res[type[u]],dsu.depth[u] + mp[type[u]] - 2 * dsu.depth[root]);
    };
    dsu.solve(root,-1,false,add,del,calc);
    for (int i = 1; i <= k; i++) {
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