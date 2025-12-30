//  Luogu contest  AT_abc226_e [ABC226E] Just one
//  Created at 30.12.2025 21:08:03 in file AT_abc226_e_ABC226E_Just_one.cpp
//  By zfq
#include <bits/stdc++.h>
// #include <bits/extc++.h>
#define LIMIT 22
#define inf 0x3f3f3f3f3f3f
#define endl '\n'

#define int long long
using namespace std;
// using namespace __gnu_pbds;
// using ordered_set = tree<int, null_type, less<>, rb_tree_tag, tree_order_statistics_node_update>;

using pii = pair<int,int>;
using v2d = vector<vector<int>>;
const double eps = 1e-7;
int dx[] =  {0,0,1,-1};
int dy[] = {1,-1,0,0};
const int MOD = 998244353;
const int N = 1e5 + 5;
const int M = 1e5 + 7;
/*
    基环树判定
*/
void solve(){
    int n,m; cin >> n >> m;
    vector<vector<int>> g(n + 1);
    for (int i = 1; i <= m; i++) {
        int u,v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    if (n != m) {
        cout << 0 << endl;
        return;
    }
    //枚举，必须是基环树森林，不然无解。连通图 点数等于边数，那么是基环树。
    int cnte,cntv;
    vector<int> vis(n + 1);
    auto dfs = [&](this auto&& dfs,int cur) -> void {
        vis[cur] = 1;
        cntv += 1;
        for (int nxt: g[cur]) {
            cnte+= 1;
            if (vis[nxt]) continue;
            dfs(nxt);
        }
    };
    int res = 1;
    for (int i = 1; i <= n; i++) {
        if (!vis[i]) {
            cnte = cntv = 0;
            dfs(i);
            if (cnte / 2 != cntv) {
                cout << 0 << endl;
                return;
            }
            res = res * 2 % MOD;
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