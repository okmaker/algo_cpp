//  Luogu contest  P3627 [APIO2009] 抢掠计划
//  Created at 18.02.2026 20:46:02 in file P3627_APIO2009_.cpp
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
void solve(){
    int n,m; cin >> n >> m;
    v2d g(n + 1);
    for (int i = 0; i < m; i++) {
        int u,v; cin >> u >> v;
        g[u].push_back(v);
    }
    vector<int> moneys(n + 1);
    for (int i = 1; i <= n; i++) cin >> moneys[i];
    int start,p; cin >> start >> p;
    vector<int> isBar(n + 1,false);
    for (int i = 0; i < p; i++) {
        int x; cin >> x;
        isBar[x] = true;
    }
    //缩点，拓扑序dp，然后找是bar的最大值即可。
    vector<int> dfn(n + 1,-1),low(n + 1,-1),belong(n + 1,-1);
    int timer = 0,scc_cnt = 0;
    stack<int> stk;
    vector<int> weights(n + 1),ends(n + 1);
    auto tarjan = [&](this auto &&self,int cur) -> void {
        dfn[cur] = low[cur] = timer++;
        stk.push(cur);
        for (auto nxt: g[cur]) {
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
                v = stk.top();stk.pop();
                belong[v] = scc_cnt;
                weights[scc_cnt] += moneys[v];
                ends[scc_cnt] |= isBar[v];
            }while (v != cur);
            scc_cnt++;
        }
    };
    tarjan(start);
    //缩点
    v2d cg(scc_cnt);
    vector<int> ing(scc_cnt);
    for (int i = 1; i <= n; i++) {
        for (auto nxt: g[i]) {
            if (belong[i] != -1 && belong[nxt] != -1 && belong[i] != belong[nxt]) {
                cg[belong[i]].push_back(belong[nxt]);
                ing[belong[nxt]]++;
            }
        }
    }
    //拓扑序dp
    vector<int> f(scc_cnt);
    for (int i = scc_cnt - 1; i >= 0; i--) {
        if (ing[i] == 0) {
            f[i] = weights[i];
        }
        for (auto nxt: cg[i]) {
            f[nxt] = max(f[nxt],f[i] + weights[nxt]);
        }
    }
    int res = 0;
    for (int i = 0; i < scc_cnt; i++) {
        if (ends[i]) {
            res = max(res,f[i]);
        }
    }
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