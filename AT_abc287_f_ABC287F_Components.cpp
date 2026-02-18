//  Luogu contest  AT_abc287_f [ABC287F] Components
//  Created at 14.02.2026 15:55:32 in file AT_abc287_f_ABC287F_Components.cpp
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
constexpr int MOD = 998244353;
constexpr int N = 1e5 + 5;
constexpr int M = 1e5 + 7;
/*
    对于某一个点，选或者不选，
    f[i][0/1]代表了节点u选或者不选，子树的联通分量数为i的个数。
    当加入的节点为孤立点
    f[0][0] = 1 f[0][1] = 0
    f[1][0] = 0 f[1][1] = 1
*/
void solve(){
    int n; cin >> n;
    vector<vector<int>> g(n + 1);
    for (int i = 1; i < n; i++) {
        int a,b; cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    vector<int> siz(n + 1);
    auto dfs = [&](this auto &&self,int cur,int fa) -> vector<array<int,2>> {
        siz[cur] = 1;
        vector<array<int,2>> f = {{1,0},{0,1}};
        //枚举孩子合并
        for (int nxt: g[cur]) {
            if (nxt == fa) continue;
            auto nxt_f = self(nxt,cur);
            vector<array<int,2>> temp(siz[cur] + siz[nxt] + 1);

            for (int i = 0; i <= siz[cur]; i++) {
                for (int j = 0; j <= siz[nxt]; j++) {
                    //当前节点不选，那么子节点如何选都会断开，方案数i + j
                    temp[i + j][0] += (nxt_f[j][0] + nxt_f[j][1]) % MOD * f[i][0] % MOD;
                    temp[i + j][0] %= MOD;
                    //当前点选，孩子点不选
                    temp[i + j][1] += nxt_f[j][0]* f[i][1] % MOD;
                    temp[i + j][1] %= MOD;
                    //当前点选，孩子点选
                    if (i + j - 1 >= 0) temp[i + j - 1][1] += nxt_f[j][1]* f[i][1] % MOD, temp[i + j - 1][1] %= MOD;
                }
            }
            siz[cur] += siz[nxt];
            f = temp;
        }
        return f;
    };
    auto f = dfs(1,0);
    for (int i = 1; i <= n; i++) {
        cout << (f[i][0] + f[i][1]) % MOD << endl;
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