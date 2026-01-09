//  Luogu contest  P8773 [蓝桥杯 2022 省 A] 选数异或
//  Created at 04.01.2026 21:44:08 in file P8773__2022__A_.cpp
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
    每次新增一个数，就让他和x异或，看异或后的数是否存在。存在那个数存在就为true，否则为false
*/
template <typename AnsType>
class MoAlgorithm {
public:
    struct Query {
        int l, r, id;
    };

    MoAlgorithm(int n, int q) : n(n), q(q) {
        blockSize = max(1ll, (int)sqrt(n));
        queries.reserve(q);
        ans.resize(q);
    }

    void addQuery(int l, int r, int id) {
        queries.push_back({l, r, id});
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

        int curL = 0, curR = -1;

        for (const auto& query : queries) {
            int L = query.l, R = query.r, id = query.id;

            // 移动右端点
            while (curR < R) addRight(++curR);
            while (curR > R) removeRight(curR--);

            // 移动左端点
            while (curL < L) removeLeft(curL++);
            while (curL > L) addLeft(--curL);

            // 记录答案
            ans[id] = getAns();
        }
    }

    const vector<AnsType>& getResults() const { return ans; }

private:
    int n, q, blockSize;
    vector<Query> queries;
    vector<AnsType> ans;
};
void solve(){
    int n, m, x;
    cin >> n >> m >> x;

    vector<int> arr(n);
    for (int i = 0; i < n; ++i) cin >> arr[i];

    MoAlgorithm<int> mo(n, m);
    for (int i = 0; i < m; ++i) {
        int l, r;
        cin >> l >> r;
        l--; r--;
        mo.addQuery(l, r, i);
    }

    int valid = 0;
    vector<int> cnt(1 << 20, 0);  // ✅ 2^20 = 1048576，直接开数组

    auto add = [&](int idx) {
        valid += cnt[arr[idx] ^ x];
        cnt[arr[idx]]++;
    };

    auto del = [&](int idx) {
        cnt[arr[idx]]--;
        valid -= cnt[arr[idx] ^ x];
    };

    auto getAns = [&]() {
        return valid;
    };

    mo.solve(add, add, del, del, getAns);

    auto& ans = mo.getResults();
    for (int i = 0; i < m; ++i) {
        if (ans[i] > 0) cout << "yes" << endl;
        else cout << "no" << endl;
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