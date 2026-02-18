//  Luogu contest  P2656 采蘑菇
//  Created at 16.02.2026 09:53:18 in file P2656_.cpp
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
using v2d = vector<vector<int> >;
constexpr double eps = 1e-7;
constexpr int dx[] = {0, 0, 1, -1};
constexpr int dy[] = {1, -1, 0, 0};
constexpr int MOD = 1e9 + 7;
constexpr int N = 1e5 + 5;
constexpr int M = 1e5 + 7;
/*
    缩点，对于scc内的边，能取知道为0，算出代价
    然后根据拓扑序，dp计算最多的答案
*/
void solve() {
    int n,m; cin >> n >> m;
    vector<vector<pii>> g(n + 1);
    vector<array<int,4>> edges(m);
    for (int i = 0; i < m; i++) {
        cin >> edges[i][0] >> edges[i][1] >> edges[i][2];
        double t; cin >> t;
        edges[i][3] = t * 10;
        g[edges[i][0]].push_back({edges[i][1],i});
    }
    int start; cin >> start;
    vector<int> dfn(n + 1,-1),low(n + 1,-1),belong(n + 1,-1);
    int timer = 0,scc_cnt = 0;
    stack<int> stk;
    auto tarjan = [&](this auto &&self,int cur) -> void {
        dfn[cur] = low[cur] = timer++;
        stk.push(cur);
        for (auto [nxt,i] : g[cur]) {
            if (dfn[nxt] == -1) {
                self(nxt);
                low[cur] = min(low[cur],low[nxt]);
            }else if (belong[nxt] == -1) {
                low[cur] = min(low[cur],dfn[nxt]);
            }
        }
        if (dfn[cur] == low[cur]) {
            int v;
            do {
                v = stk.top();
                stk.pop();
                belong[v] = scc_cnt;
            }while (v != cur);
            scc_cnt++;
        }
    };
    tarjan(start);
    //缩点
    vector<vector<pii>> cg(scc_cnt);
    vector<int> f(scc_cnt,-inf),weight(scc_cnt);
    for (int i = 0; i < m; i++) {
        int u = edges[i][0],v = edges[i][1];
        if (dfn[u] == -1 || dfn[v] == -1) continue;
        if (belong[u] == belong[v]) {
            //计算权重。
            int w = edges[i][2],c = edges[i][3];
            while (w) {
                weight[belong[u]] += w;
                w = w * c / 10;
            }
        }else {
            //不同的就连边
            int w = edges[i][2];
            cg[belong[u]].push_back({w,belong[v]});
        }
    }
    //拓扑序dp
    f[belong[start]] = weight[belong[start]];
    //反着来就是拓扑序
    for (int i = scc_cnt - 1; i >= 0; i--) {
        for (auto [w,nxt]: cg[i]) {
            f[nxt] = max(f[nxt], f[i] + w + weight[nxt]);
        }
    }
    int res = 0;
    for (int i = 0; i < scc_cnt; i++) {
        res = max(res, f[i]);
    }
    cout << res << endl;

}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int T = 1; //cin >> T;
    while (T--) {
        solve();
    }

    return 0;
}
