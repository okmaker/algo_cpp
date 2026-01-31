//  Luogu contest  P7771 【模板】欧拉路径
//  Created at 18.01.2026 17:25:23 in file P7771_.cpp
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

void solve(){
    int n,m; cin >> n >> m;
    vector<vector<int>> g(n + 1);
    vector<int> in(n + 1),out(n + 1);
    for (int i = 1; i <= m; i++) {
        int u,v; cin >> u >> v;
        g[u].push_back(v);
        in[v]++;
        out[u]++;
    }
    //找起点
    int start = -1,end = -1;
    for (int i = 1; i <= n; i++) {
        int d = out[i] - in[i];
        if (abs(d) > 1 || (d == 1 && start != -1) || (d == -1 && end != -1)) {
            cout << "No" << endl;
            return;
        }
        if (d == 1) start = i;
        if (d == -1) end = i;
    }
    //有可能全为0
    //起点和终点要么都存在，要么都不存在
    if ((start != -1) ^ (end != -1)) {
        cout << "No" << endl;
        return;
    }
    if (start == -1) start = 1;
    for (int i = 1; i <= n; i++) {
        ranges::sort(g[i],greater<int>());
    }
    vector<int> res;
    //dfs找欧拉路径
    auto dfs = [&](this auto&& dfs,int cur) -> void {
        while (g[cur].size()) {
            int nxt = g[cur].back();
            g[cur].pop_back();
            dfs(nxt);
        }
        res.push_back(cur);
    };
    dfs(start);
    ranges::reverse(res);
    for (auto x:  res) cout << x << " ";
}
signed main() {
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int T = 1;//cin >> T;
    while(T--){
        solve();
    }
    
    return 0;
}