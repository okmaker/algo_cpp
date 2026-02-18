//  Luogu contest  P2014 [CTSC1997] 选课
//  Created at 11.02.2026 16:55:26 in file P2014_CTSC1997_.cpp
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
    
*/
void solve() {
    int n,m; cin >> n >> m;
    vector<vector<int>> g(n + 1);
    vector<int> score(n + 1);
    for (int i = 1; i <= n; i++) {
        int k,s; cin >> k >> s;
        g[k].push_back(i);
        score[i] = s;
    }
    vector<int> dfn(n + 1),siz(n + 1),id(n + 1);
    int timer = 0;
    auto dfs = [&](this auto&& self,int cur) -> void {
        dfn[cur] = timer++;
        id[dfn[cur]] = cur;
        siz[cur] = 1;
        for (int nxt: g[cur]) {
            self(nxt);
            siz[cur] += siz[nxt];
        }
    };
    dfs(0);
    m += 1;
    //有依赖的dp，逆序遍历
    vector<vector<int>> f(n + 2,vector<int>(m + 1));
    //总共有n + 1个点
    //f[i][j] = f[i +  + size[i]][j]
    //f[i][j] = score[i] + f[i + 1][j - 1]
    for (int i = n;i >= 1;i--) {
        for (int j = 0;j <= m;j++) {
            f[i][j] = f[i + siz[id[i]]][j];
            if (j >= 1) f[i][j] = max(f[i][j],score[id[i]] + f[i + 1][j - 1]);
        }
    }
    // 最后只需要不要0节点，那么只需要m-1个。
    cout << f[1][m - 1] << endl;
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
