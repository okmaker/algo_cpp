//  Luogu contest  AT_indeednow_2015_quala_3 説明会
//  Created at 29.12.2025 21:38:45 in file AT_indeednow_2015_quala_3_.cpp
//  By zfq
#include <bits/stdc++.h>
// #include <bits/extc++.h>
#define LIMIT 22
#define inf 0x3f3f3f3f3f3f
#define endl '\n'

#define int long long
using namespace std;
// using namespace __gnu_pbds;
// using ordered_set = tree<int, null_type, less<>, rb_tree_tag, tree_order_statistics_node_update>;

using pii = pair<int,int>;
using v2d = vector<vector<int> >;
const double eps = 1e-7;
int dx[] = {0, 0, 1, -1};
int dy[] = {1, -1, 0, 0};
const int MOD = 1e9 + 7;
const int N = 1e5 + 5;
const int M = 1e5 + 7;
/*
    对于单个询问，显然可以二分搜索出答案
    多个询问都可以，那么考虑整体二分。
*/
struct Query {
    int k,id;
};
void solve() {
    int n; cin >> n;
    vector<int> scores;
    int mx = 0;
    for (int i = 0,x; i < n; i++) {
        cin >> x,mx = max(mx,x);
        if (x != 0) scores.push_back(x);
    }
    sort(scores.begin(),scores.end());
    int q; cin >> q;
    vector<int> limits(q);
    vector<int> qid(q);
    for (int i = 0; i < q; i++) {
        cin >> limits[i];
        qid[i] = i;
    }
    vector<int> res(q);
    //整体二分
    vector<int> lset,rset;
    auto cal = [&](this auto &&cal,int ql,int qr,int vl,int vr) -> void {
        if (ql > qr) return;
        if (vl == vr) {
            for (int i = ql; i <= qr; i++) {
                res[qid[i]] = vl;
            }
            return;
        }
        int mid = (vl + vr) >> 1;
        //check 查询分数比mid大的个数
        int cnt = scores.end() -  lower_bound(scores.begin(), scores.end(),mid);
        //分配集合
        lset.clear();rset.clear();
        for (int i = ql; i <= qr; i++) {
            int id = qid[i];
            if (limits[id] >= cnt) {
                lset.push_back(id);
            }else {
                rset.push_back(id);
            }
        }
        int j = ql;
        for(int i = 0;i < lset.size();i++){
            qid[j++] = lset[i];
        }
        for(int i = 0;i < rset.size();i++){
            qid[j++] = rset[i];
        }
        int sz = lset.size();
        cal(ql,ql + sz - 1,vl,mid);
        cal(ql + sz,qr,mid + 1,vr);
    };
    cal(0,q-1,0,mx + 1);
    for (int i = 0; i < q; i++) {
        cout << res[i] << endl;
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
