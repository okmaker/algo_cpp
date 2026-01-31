//  Luogu contest  U224391 【模板】强连通分量
//  Created at 31.01.2026 12:59:06 in file U224391_.cpp
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


void solve(){
    int n,m; cin >> n >> m;
    vector<vector<int>> g(n);
    for (int i = 0,u,v; i < m; i++) {
        cin >> u >> v;u--,v--;
        g[u].push_back(v);
    }
    vector<int> dfn(n,-1),low(n),belong(n,-1);
    stack<int> stk;
    int timer = 0,scc_cnt = 0;
    vector<vector<int>> sccG;
    auto tarjan = [&](this auto &&tarjan,int cur) -> void{
        dfn[cur] = low[cur] = timer++;
        stk.push(cur);
        for (int nxt: g[cur]) {
            if (dfn[nxt] == -1) {
                tarjan(nxt);
                low[cur] = min(low[cur],low[nxt]);
            }else if (belong[nxt] == -1) {
                low[cur] = min(low[cur],dfn[nxt]);
            }
        }
        if (low[cur] == dfn[cur]) {
            int v;sccG.emplace_back();
            do {
                v = stk.top();stk.pop();
                belong[v] = scc_cnt;
                sccG.back().push_back(v);
            }while (v != cur);
            scc_cnt++;
        }
    };
    for (int i = 0; i < n; i++) {
        if (dfn[i] == -1) tarjan(i);
    }
    cout << scc_cnt << endl;
    for (int i = 0; i < scc_cnt; i++) {
        cout << sccG[i].size() << " ";
        for (int x: sccG[i]) cout << x+1 << " ";
        cout << endl;
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