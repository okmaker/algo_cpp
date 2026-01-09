//  Luogu contest  P5268 [SNOI2017] 一个简单的询问
//  Created at 05.01.2026 21:52:20 in file P5268_SNOI2017_.cpp
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
const double eps = 1e-7;
int dx[] =  {0,0,1,-1};
int dy[] = {1,-1,0,0};
const int MOD = 1e9 + 7;
const int N = 1e5 + 5;
const int M = 1e5 + 7;
/*

*/
template <typename AnsType>
class MoAlgorithm {
public:
    struct Query {
        int l, r,type, id;
    };

    MoAlgorithm(int n, int q) : n(n), q(q) {
        blockSize = max(1ll, (int)sqrt(n));
        queries.reserve(q);
        ans.resize(q);
    }

    void addQuery(int l, int r,int type ,int id) {
        queries.push_back({l, r, type,id});
    }

    template <typename AddLeft, typename AddRight, typename RemoveLeft, typename RemoveRight, typename GetAns>
    void solve(AddLeft addLeft, AddRight addRight, RemoveLeft removeLeft, RemoveRight removeRight, GetAns getAns) {
        // 莫队排序：左端点所在块升序，块内右端点升序（奇偶优化）
        sort(queries.begin(), queries.end(), [&](const Query& a, const Query& b) {
            int blockA = a.l / blockSize;
            int blockB = b.l / blockSize;
            if (blockA != blockB) return blockA < blockB;
            // 奇偶优化：奇数块右端点升序，偶数块降序
            return (blockA & 1) ? (a.r < b.r) : (a.r > b.r);
        });

        int curL = -1, curR = -1;//这里和一般莫队不同，左右端点都分别代表了一个前缀。

        for (const auto& query : queries) {
            int L = query.l, R = query.r, id = query.id;
            //移动左右端点的逻辑一致，这个也跟普通莫队不一样。
            // 移动右端点
            while (curR < R) addRight(++curR);
            while (curR > R) removeRight(curR--);

            // 移动左端点
            while (curL < L) addLeft(++curL);
            while (curL > L) removeLeft(curL--);



            // 记录答案
            ans[id] += getAns() * query.type;
        }
    }

    const vector<AnsType>& getResults() const { return ans; }

private:
    int n, q, blockSize;
    vector<Query> queries;
    vector<AnsType> ans;
};
void solve(){
    //get(l1,r1,l2,r2) = f(r1,x) * f(r2,x) - f(r1,x) * f(l2 - 1,x) - f(r2,x) * f(l1 - 1,x) + f(l1 - 1,x) * f(l2 - 1,x)
    int n; cin >> n;
    int mx = 0;
    vector<int> a(n); for (int i = 0; i < n; ++i) cin >> a[i],mx = max(mx,a[i]);
    int m; cin >> m;
    MoAlgorithm<int> mo(n, m);
    for (int i = 0; i < m; ++i) {
        int l1, r1, l2, r2; cin >> l1 >> r1 >> l2 >> r2; l1--,r1--,r2--,l2--;
        mo.addQuery(min(r1,r2),max(r1,r2),1,i);
        if (l2 - 1 >= 0) mo.addQuery(min(l2 - 1,r1),max(l2 - 1,r1),-1,i);
        if (l1 - 1 >= 0) mo.addQuery(min(l1 - 1,r2),max(l1 - 1,r2),-1,i);
        if (l1 - 1 >= 0 && l2 - 1 >= 0) mo.addQuery(min(l1 - 1,l2 - 1),max(l1 - 1,l2 - 1),1,i);
    }
    vector<int> cntl(mx + 1),cntr(mx + 1);
    int res = 0;
    auto addLeft = [&](int idx) {
        cntl[a[idx]]++;
        res += cntr[a[idx]];
    };
    auto removeLeft = [&](int idx) {
        cntl[a[idx]]--;
        res -= cntr[a[idx]];
    };
    auto addRight = [&](int idx) {
        cntr[a[idx]]++;
        res += cntl[a[idx]];
    };
    auto removeRight = [&](int idx) {
        cntr[a[idx]]--;
        res -= cntl[a[idx]];
    };
    auto getAns = [&]() {
        return res;
    };
    mo.solve(addLeft,addRight,removeLeft,removeRight,getAns);
    auto& ans = mo.getResults();
    for (auto& x : ans) cout << x << endl;
}
signed main() {
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int T = 1;//cin >> T;
    while(T--){
        solve();
    }

    return 0;
}