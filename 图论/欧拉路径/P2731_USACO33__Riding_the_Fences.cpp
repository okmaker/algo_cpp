//  Luogu contest  P2731 [USACO3.3] 骑马修栅栏 Riding the Fences
//  Created at 18.01.2026 20:30:29 in file P2731_USACO33__Riding_the_Fences.cpp
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
    int n = 500;
    int m; cin >> m;
    vector<vector<pii>> g(n + 1);// (点，边id)
    vector<int> deg(n + 1);
    for (int i = 0; i < m; i++) {
        int u,v; cin >> u >> v;
        g[u].push_back({v,i});
        g[v].push_back({u,i});
        deg[u]++;deg[v]++;
    }
    //找起点
    int odd = 0;
    for (int i = 1; i <= n; i++) {
        if (deg[i] & 1) odd++;
    }
    if (odd != 0 && odd != 2) {
        cout << "No" << endl;
        return;
    }
    int start = -1;
    for (int i = 1; i <= n; i++) {
        if (odd == 0 && deg[i] > 0) {
            start = i;
            break;
        }
        if (odd == 2 && (deg[i] & 1)) {
            start = i;
            break;
        }
    }
    vector<int> vis(m),ptr(n + 1),res;
    for (int i = 1; i <= n; i++) {
        sort(g[i].begin(),g[i].end());
    }
    auto dfs = [&](this auto &&dfs,int cur) -> void{
        for (int &i = ptr[cur]; i < g[cur].size();i++) {
            if (vis[g[cur][i].second]) continue;
            vis[g[cur][i].second] = true;
            dfs(g[cur][i].first);
        }
        res.push_back(cur);
    };
    dfs(start);
    reverse(res.begin(), res.end());
    for (long long re : res) {
        cout << re << endl;
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