//  Luogu contest  AT_abc247_e [ABC247E] Max Min
//  Created at 19.01.2026 22:28:15 in file AT_abc247_e_ABC247E_Max_Min.cpp
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
    
*/
void solve(){
    int n,mn,mx; cin >> n >> mx >> mn;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    auto f = [&](int l,int r) -> int {
        int c = 0;
        int pre = 0;
        for (int i = 0; i < n; i++) {
            if (a[i] < l || a[i] > r) {
                pre = i + 1;
                continue;
            }
            c += i - pre + 1;
        }
        return c;
    };
    int res = f(mn,mx) - f(mn + 1,mx) - f(mn,mx - 1) + f(mn + 1,mx - 1);
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