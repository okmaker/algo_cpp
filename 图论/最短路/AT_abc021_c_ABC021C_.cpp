//  Luogu contest  AT_abc021_c [ABC021C] 正直者の高橋くん
//  Created at 30.12.2025 22:34:48 in file AT_abc021_c_ABC021C_.cpp
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
const double eps = 1e-7;
int dx[] = {0, 0, 1, -1};
int dy[] = {1, -1, 0, 0};
const int MOD = 1e9 + 7;
const int N = 1e5 + 5;
const int M = 1e5 + 7;
/*
    dijkstra + 最短路个数计数。
*/
void solve() {
    int n,start,target; cin >> n >> start >> target;
    int m; cin >> m;
    vector<vector<int>> g(n + 1);
    for (int i = 0; i < m; i++) {
        int x,y; cin >> x >> y;
        g[x].push_back(y);
        g[y].push_back(x);
    }
    vector<int> dis(n + 1,inf),f(n + 1,0);
    f[start] = 1;dis[start] = 0;
    //小根堆
    priority_queue<pii,vector<pii>,greater<pii>> q;
    q.push({0,start});
    while (!q.empty()) {
        auto [w,cur] = q.top();q.pop();
        if (w > dis[cur]) continue;
        for (int nxt: g[cur]) {
            int nw = w + 1;
            if (nw < dis[nxt]) {
                dis[nxt] = nw;
                q.push({nw,nxt});
                f[nxt] = f[cur];
            }else if (nw == dis[nxt]) {
                f[nxt] += f[cur];
                f[nxt] %= MOD;
            }
        }
    }
    cout << f[target] << endl;
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
