//  LibreOJ contest  #516. 「LibreOJ β Round #2」DP 一般看规律
//  Created at 24.12.2025 20:29:43 in file 516_LibreOJ__Round_2DP_.cpp
//  By zfq
#include <bits/stdc++.h>
#define LIMIT 22
#define inf 0x3f3f3f3f3f3f
#define endl '\n'
// #include <ext/pb_ds/assoc_container.hpp>
// #include <ext/pb_ds/tree_policy.hpp>
#define int long long
using namespace std;
// __gnu_pbds::tree<int, __gnu_pbds::null_type,
//                  less<int>, __gnu_pbds::rb_tree_tag,
//                  __gnu_pbds::tree_order_statistics_node_update>
//     tr;
using pii = pair<int,int>;
using v2d = vector<vector<int>>;
const double eps = 1e-7;
int dx[] =  {0,0,1,-1};
int dy[] = {1,-1,0,0};
const int MOD = 1e9 + 7;
const int N = 1e5 + 5;
const int M = 1e5 + 7;
/*
    维护每一个数相关的索引序列。每次小的合并到大的，同时合并过去前先查询比他小的，以及比他大的更新答案。
*/
void solve(){
    int n,m; cin >> n >> m;
    map<int,set<int>> mp;
    int res = 2147483647;
    for (int i = 1,x; i <= n; i++) {
        cin >> x;
        if (!mp[x].empty()) {
            res = min(res,*prev(mp[x].end()));
        }
        mp[x].insert(i);
    }
    //枚举操作

    while (m--) {
        int x,y; cin >> x >> y;
        if (x == y || (mp[x].empty() && mp[y].empty())) {
            cout << res << endl;
            continue;
        }
        //将所有的x变成y
        set<int> &stx = mp[x];
        set<int> &sty = mp[y];
        if (sty.size() < stx.size()) {
            swap(stx,sty);
        }
        while (!stx.empty()) {
            int v = *stx.begin();
            auto it = sty.upper_bound(v);
            if (it != sty.end()) {
                res = min(res,abs(v - *it));
            }
            if (it != sty.begin()) {
                --it;
                res = min(res,abs(v - *it));
            }
            stx.erase(v);
            sty.insert(v);
        }
        mp[y] = sty;
        cout << res << endl;
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