//  Luogu contest  P2272 [ZJOI2007] 最大半连通子图
//  Created at 18.02.2026 22:10:58 in file P2272_ZJOI2007_.cpp
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
    scc中可以互相到达，那么一定都选，缩点后变为dag，
    对于每一个节点 f[cur] -> f[nxt]
    让f[nxt]最大，如果相等，那么数量累加即可。

    出现了错误，原因在于没有考虑缩点后会出现重边。
*/
void solve(){
    int n,m,x; cin >> n >> m >> x;
    v2d g(n + 1);
    for (int i = 0; i < m; i++) {
        int a,b; cin >> a >> b;
        g[a].push_back(b);
    }
    vector<int> dfn(n + 1,-1),low(n + 1,-1),belong(n + 1,-1);
    int timer = 0,scc_cnt = 0;
    stack<int> stk;
    vector<int> cnts(n + 1);
    auto tarjan = [&](this auto &&self,int cur) -> void {
        dfn[cur] = low[cur] = timer++;
        stk.push(cur);
        for (int nxt: g[cur]) {
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
                cnts[scc_cnt]++;
            }while (v != cur);
            scc_cnt++;
        }
    };
    for (int i = 1; i <= n; i++) {
        if (dfn[i] == -1) tarjan(i);
    }
    vector<int> ing(n + 1);
    v2d cg(scc_cnt);
    vector<pii> sorted;
    for (int i = 1; i <= n; i++) {
        for (int nxt: g[i]) {
            if (belong[i] != belong[nxt]) {
                sorted.push_back({belong[i],belong[nxt]});
            }
        }
    }
    ranges::sort(sorted);
    sorted.erase(ranges::unique(sorted).begin(),sorted.end());
    for (auto [u,v]: sorted) {
        cg[u].push_back(v);
        ing[v] ++;
    }
    vector<int> fmx(scc_cnt,-inf),fcnt(scc_cnt);
    for (int i = scc_cnt - 1; i >= 0; i--) {
        if (ing[i] == 0) {
            fmx[i] = cnts[i];
            fcnt[i] = 1;
        }
        for (int nxt: cg[i]) {
            if (fmx[nxt] < fmx[i] + cnts[nxt]) {
                fmx[nxt] = fmx[i] + cnts[nxt];
                fcnt[nxt] = fcnt[i];
            }else if (fmx[nxt] == fmx[i] + cnts[nxt]) {
                fcnt[nxt] += fcnt[i];
                fcnt[nxt] %= x;
            }
        }
    }
    int res = -inf,cnt = 0;
    for (int i = 0; i < scc_cnt; i++) {
        if (fmx[i] > res) {
            res = fmx[i];
            cnt = fcnt[i];
        }else if (fmx[i] == res) {
            cnt += fcnt[i];
            cnt %= x;
        }
    }
    cout << res << endl;
    cout << cnt << endl;
}
signed main() {
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int T = 1;//cin >> T;
    while(T--){
        solve();
    }
    
    return 0;
}