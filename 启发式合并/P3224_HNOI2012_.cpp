//  Luogu contest  P3224 [HNOI2012] 永无乡
//  Created at 24.12.2025 22:48:46 in file P3224_HNOI2012_.cpp
//  By zfq   https://www.luogu.com.cn/problem/P3224
#include <bits/stdc++.h>
#include <bits/extc++.h>
#define LIMIT 22
#define inf 0x3f3f3f3f3f3f
#define endl '\n'

#define int long long
using namespace std;
using namespace __gnu_pbds;
using pii = pair<int,int>;
using v2d = vector<vector<int>>;
using ordered_set = tree<pii, null_type, less<>, rb_tree_tag, tree_order_statistics_node_update>;


const double eps = 1e-7;
int dx[] =  {0,0,1,-1};
int dy[] = {1,-1,0,0};
const int MOD = 1e9 + 7;
const int N = 1e5 + 5;
const int M = 1e5 + 7;
/*
    
*/
void solve(){
    int n,m;cin >> n >> m;
    vector<int> fa(n + 1);
    vector<ordered_set*> trs(n + 1,nullptr);
    for (int i = 1,x; i <= n; i++) {
        cin >> x;
        fa[i] = i;
        trs[i] = new ordered_set();
        trs[i]->insert({x,i});
    }
    auto find = [&](this auto &&self,int x) -> int {
        if (x != fa[x]) {
            fa[x] = self(fa[x]);
        }
        return fa[x];
    };
    auto merge = [&](int x,int y) -> void {
        int fx = find(x),fy = find(y);
        if (fx == fy) return;
        //fx -> fy
        if (trs[fx]->size() < trs[fy]->size()) {
            swap(fx,fy);
            swap(trs[fx],trs[fy]);
        }
        fa[fx] = fa[fy];
        //启发式合并，小到大
        for (auto &[v,idx]: *trs[fx]) {
            trs[fy]->insert({v,idx});
        }
        delete trs[fx];
    };
    while (m--) {
        int u,v; cin >> u >> v;
        merge(u,v);
    }
    int q; cin >> q;
    while (q--) {
        char op; cin >> op;
        if (op == 'B') {
            int x,y; cin >> x >> y;
            merge(x,y);
        }else {
            int x,k; cin >> x >> k;
            int fx = find(x);
            ordered_set* tr = trs[fx];
            if (tr->size() < k) {
                cout << -1 << endl;
            }else {
                cout << tr->find_by_order(k - 1)->second << endl;
            }
        }
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