//  Luogu contest  CF1900D Small GCD
//  Created at 20.12.2025 21:10:03 in file CF1900D_Small_GCD.cpp
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
    先计数，某一个x 2x 3x ... 的总数
    c * (c - 1) * (c - 2) / 6
*/
void solve(){
    int n; cin >> n;
    int mx = 0;
    vector<int> nums(n);
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
        mx = max(mx,nums[i]);
    }
    vector<int> cnt(mx + 1),f(mx + 1);
    for (auto x: nums) cnt[x]++;
    int res = 0;
    for (int i = mx; i >= 1;i--) {
        int c = 0;
        for (int j = i; j <= mx; j += i) {
            c += cnt[j];
            f[i] -= f[j];
        }
        f[i] += c * (c - 1) * (c - 2) / 6;
        res += f[i] * i;
    }
}
signed main() {
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int T = 1;cin >> T;
    while(T--){
        solve();
    }
    
    return 0;
}