//  Luogu contest  P4254 [JSOI2008] Blue Mary 开公司
//  Created at 06.02.2026 21:34:01 in file P4254_JSOI2008_Blue_Mary_.cpp
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
    李超线段树模版
*/
struct Line {
    double k,b;
}lines[N << 1];
int tr[N<<2];
double getY(int id,int x) {
    return lines[id].k * x + lines[id].b;
}
//在区间[start,end]增加一个lines[id]的线段
void update(int rt,int l,int r,int start,int end,int id) {
    int mid = (l + r) >> 1;
    if (start <= l && r <= end) {
        //如果新的线段更高，那么需要更新当前区间的最高线段id
        if (getY(tr[rt],mid) < getY(id,mid)) swap(tr[rt],id);
        //同时可能会影响小区间，继续去更新小区间
        if (getY(tr[rt],l) < getY(id,l)) update(rt<<1,l,mid,start,end,id);
        if (getY(tr[rt],r) < getY(id,r)) update(rt<<1|1,mid+1,r,start,end,id);
        return;
    }
    if (start <= mid) update(rt<<1,l,mid,start,end,id);
    if (mid < end) update(rt<<1|1,mid+1,r,start,end,id);
}
double query(int rt,int l,int r,int x) {
    if (l == r) {
        return getY(tr[rt],x);
    }
    int mid = (l + r) >> 1;
    double res = getY(tr[rt],x);
    if (x <= mid) res = max(res,query(rt<<1,l,mid,x));
    else res = max(res,query(rt<<1|1,mid+1,r,x));
    return res;
}

void solve() {
    int n; cin >> n;
    for (int i = 1; i <= n; ++i) {
        string op; cin >> op;
        if (op == "Query") {
            int x; cin >> x;
            double res = query(1,1,N,x);
            cout << (int)(res / 100.0) << endl;
        }
        else {
            double s,p; cin >> s >> p;
            lines[i].k = p;
            lines[i].b = s - p;
            update(1,1,N,1,N,i);
        }
    }
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
