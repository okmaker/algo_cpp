//  Luogu contest  P4178 Tree
//  Created at 10.01.2026 12:26:19 in file P4178_Tree.cpp
//  By zfq
#include <bits/stdc++.h>
#include <bits/extc++.h>
#define LIMIT 22
#define inf 0x3f3f3f3f3f3f
#define endl '\n'

#define int long long
using namespace std;
using namespace __gnu_pbds;
using ordered_set = tree<pair<int, int>, null_type, less<>, rb_tree_tag, tree_order_statistics_node_update>;

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
        for (auto& edge : g[u]) {
            int v = edge[0],w = edge[1];
            if (v == fa) continue;
            prepare(v, u, d + w, dep + 1);
            siz[u] += siz[v];
            if (son[u] == -1 || siz[v] > siz[son[u]]) son[u] = v;
        }
    }


    template <typename Add, typename Del, typename Calc>
    void solve(int u, int fa, bool keep, Add&& add, Del&& del, Calc&& calc) {

        for (auto& edge : g[u]) {
            int v = edge[0];
            if (v == fa || v == son[u]) continue;
            solve(v, u, false, add, del, calc);
        }


        if (son[u] != -1) {
            solve(son[u], u, true, add, del, calc);
        }

        calc(u,u);

        add(u);
        for (auto& edge : g[u]) {
            int v = edge[0];
            if (v == fa || v == son[u]) continue;
            calc_recursive(v, u, u, calc);
            update_recursive(v, u, add);
        }



        if (!keep) {
            update_recursive(u, fa, del);
        }
    }

private:
    template <typename Updater>
    void update_recursive(int u, int fa, Updater&& updater) {
        updater(u);
        for (auto& edge : g[u]) {
            int v = edge[0];
            if (v != fa) update_recursive(v, u, updater);
        }
    }

    template <typename Calculator>
    void calc_recursive(int u, int fa, int root, Calculator&& calc) {
        calc(u, root);
        for (auto& edge : g[u]) {
            int v = edge[0];
            if (v != fa) calc_recursive(v, u, root, calc);
        }
    }
};
/*
    dis[u] + dis[v] - 2 * dis[root] <= k

    dis[v] <= k + 2 * dis[root] - dis[u]
    计数。有序表就可以，二分出来个数。
*/
void solve(){

    int n; cin >> n;
    vector<vector<array<int,2>>> g(n);
    for (int i = 0; i < n - 1; i++) {
        int u,v,w; cin >> u >> v >> w;
        u--; v--;
        g[u].push_back({v,w});
        g[v].push_back({u,w});
    }
    int k; cin >> k;
    DSUOnTree dsu(n,g); dsu.prepare(0,-1,0,0);
    ordered_set st;
    int res = 0;
    auto add = [&](int u) {
        st.insert({dsu.dist[u],u});
    };
    auto del = [&](int u) {
        st.clear();
    };
    auto calc = [&](int u, int root) {
        int target = k + 2 * dsu.dist[root] - dsu.dist[u];
        //st中找小于等于target的数量
        int cnt = st.order_of_key({target, inf});
        res += cnt;
    };
    dsu.solve(0,-1,false,add,del,calc);
    cout << res << endl;
}
signed main() {
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int T = 1;//cin >> T;
    while(T--){
        solve();
    }
    
    return 0;
}