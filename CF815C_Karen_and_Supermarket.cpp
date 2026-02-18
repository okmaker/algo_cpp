//  Luogu contest  CF815C Karen and Supermarket
//  Created at 14.02.2026 12:42:53 in file CF815C_Karen_and_Supermarket.cpp
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
    int n,b; cin >> n >> b;
    vector<int> costs(n),diffs(n);
    vector<vector<int>> g(n);
    for (int i = 0; i < n; i++) {
        cin >> costs[i] >> diffs[i];
        if (i > 0) {
            int x; cin >> x;x--;
            g[x].push_back(i);
        }
    }
    vector<int> siz(n);
    //考虑维度互换的树上01背包
    //f[i][0/1]代表了选i次选或者不选的最小代价
    auto dfs = [&](this auto &&self,int cur) -> vector<array<int,2>> {
        vector<array<int,2>> sub_f;
        sub_f.push_back({0,0});
        siz[cur] = 0;
        int tot_sz = 0;
        for (int nxt: g[cur]) {
            auto nxt_f = self(nxt);
            tot_sz += siz[nxt];
            //下一轮的结果
            vector<array<int,2>> temp(tot_sz + 1,{inf,inf});
            //合并
            for (int i = 0; i <= siz[cur]; i++) {
                for (int j = 0; j <= siz[nxt]; j++) {
                    temp[i + j][0] = min(temp[i + j][0],nxt_f[j][0] + sub_f[i][0]);
                    temp[i + j][1] = min(temp[i + j][1],min(nxt_f[j][1],nxt_f[j][0]) + sub_f[i][1]);
                }
            }
            siz[cur] += siz[nxt];
            sub_f = temp;
        }
        siz[cur] += 1;
        vector<array<int,2>> f(siz[cur] + 1,{inf,inf});
        //枚举子节点
        for (int i = 0; i < siz[cur]; i++) {
            //直接不买
            f[i][0] = min(f[i][0],sub_f[i][0]);
            f[i + 1][0] = min(f[i + 1][0],sub_f[i][0] + costs[cur]);
            f[i + 1][1] = min(f[i + 1][1],sub_f[i][1] + costs[cur] - diffs[cur]);
        }
        return f;
    };
    auto f = dfs(0);
    int res = 0;
    //枚举答案
    for (int i = 0; i <= siz[0]; i++) {
        if (min(f[i][0],f[i][1]) <= b) res = i;
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
