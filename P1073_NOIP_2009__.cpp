//  Luogu contest  P1073 [NOIP 2009 提高组] 最优贸易
//  Created at 18.02.2026 21:21:50 in file P1073_NOIP_2009__.cpp
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
    先缩点，变成dag，这样可以dp，需要维护每一个点的最大值和最小值。
    dp过程维护一个前缀最小值。随时更新答案即可。
*/
void solve(){
    int n,m; cin >> n >> m;
    vector<int> prices(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> prices[i];
    }
    v2d g(n + 1);
    for (int i = 0; i < m; i++) {
        int x,y,z; cin >> x >> y >> z;
        g[x].push_back(y);
        if (z == 2) {
            g[y].push_back(x);
        }
    }
    //scc
    vector<int> dfn(n + 1,-1),low(n + 1,-1),belong(n + 1,-1);
    int timer = 0,scc_cnt = 0;
    stack<int> stk;
    //缩点后维护的信息
    vector<int> sccMax(n,-inf),sccMin(n,inf);
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
                v = stk.top();stk.pop();
                belong[v] = scc_cnt;
                sccMax[scc_cnt] = max(sccMax[scc_cnt],prices[v]);
                sccMin[scc_cnt] = min(sccMin[scc_cnt],prices[v]);
            }while (v != cur);
            scc_cnt++;
        }
    };
    tarjan(1);
    if (dfn[n] == -1) {
        cout << "0" << endl;
        return;
    }
    //缩点dp
    v2d cg(scc_cnt);
    vector<int> ing(scc_cnt);
    for (int i = 1; i <= n; i++) {
        if (dfn[i] == -1) continue;
        for (int nxt: g[i]) {
            if (dfn[nxt] != -1 && belong[i] != belong[nxt]) {
                cg[belong[i]].push_back(belong[nxt]);
                ing[belong[nxt]]++;
            }
        }
    }
    vector<int> premin(scc_cnt,inf);
    premin[belong[1]] = sccMin[belong[1]];
    int res = sccMax[belong[1]] - sccMin[belong[1]];
    for (int i = scc_cnt - 1; i >= 0; i--) {
        for (int nxt: cg[i]) {
            premin[nxt] = min(premin[nxt],min(premin[i],sccMin[nxt]));
            res = max(res,sccMax[nxt] - premin[nxt]);
        }
    }
    cout << res << endl;

};
signed main() {
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int T = 1;//cin >> T;
    while(T--){
        solve();
    }
    
    return 0;
}