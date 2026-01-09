//  Luogu contest  P5906 【模板】回滚莫队&不删除莫队
//  Created at 04.01.2026 18:13:03 in file P5906_.cpp
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
// --- 0-indexd ---
template <typename AnsType>
class AddOnlyMo {
public:
    struct Query {
        int l, r, id;
    };

    AddOnlyMo(int n, int q) : n(n), q(q) {
        // 使用整数计算块大小，避免精度问题
        blockSize = max(1ll, (int)(n / sqrt(max(1.0, (double)q))));
        queries.reserve(q);
        ans.resize(q);
    }

    void addQuery(int l, int r, int id) {
        queries.push_back({l, r, id});
    }

    template <typename AddLeft,typename AddRight, typename GetAns, typename Snapshot, typename Rollback, typename Reset>
    void solve(AddLeft addLeft,AddRight addRight, GetAns getAns, Snapshot snapshot, Rollback rollback, Reset reset) {
        // 排序规则：左端点块升序，块内右端点升序
        sort(queries.begin(), queries.end(), [&](const Query& a, const Query& b) {
            int b1 = a.l / blockSize, b2 = b.l / blockSize;
            if (b1 != b2) return b1 < b2;
            return a.r < b.r;
        });

        int curR = -1, lastBlock = -1;

        for (auto& query : queries) {
            int L = query.l, R = query.r, id = query.id;
            int bL = L / blockSize;
            int bR = R / blockSize;

            // 1. 块改变时重置
            if (bL != lastBlock) {
                reset();
                lastBlock = bL;
                // 右指针初始化为当前块的末尾
                curR = (bL + 1) * blockSize - 1;
            }

            // 2. 情况1：左右端点在同一块内，直接暴力
            if (bL == bR) {
                snapshot();
                for (int i = L; i <= R; ++i) addRight(i);
                ans[id] = getAns();
                rollback(L, R); // 撤销本次暴力增加的贡献
                continue;
            }

            // 3. 情况2：跨块，右指针单调移动
            while (curR < R) {
                addRight(++curR);
            }

            // 4. 左指针临时向左移动并回滚
            snapshot();
            int blockRight = (bL + 1) * blockSize;
            for (int i = blockRight - 1; i >= L; --i) {
                addLeft(i);
            }

            ans[id] = getAns();

            // 回滚左指针区间 [L, blockRight - 1]
            rollback(L, blockRight - 1);
        }
    }

    const vector<AnsType>& getResults() const { return ans; }

private:
    int n, q, blockSize;
    vector<Query> queries;
    vector<AnsType> ans;
};
/*
    
*/
void solve(){
    int n; cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; ++i) cin >> nums[i];

    vector<int> sorted(nums.begin(), nums.end());
    sort(sorted.begin(), sorted.end());
    sorted.erase(unique(sorted.begin(), sorted.end()),sorted.end());
    for (int i = 0; i < n; ++i) {
        nums[i] = lower_bound(sorted.begin(), sorted.end(), nums[i]) - sorted.begin();
    }
    int m; cin >> m;
    AddOnlyMo<int> mo(n, m);
    for (int i = 0; i < m; ++i) {
        int l,r; cin >> l >> r; l--,r--;
        mo.addQuery(l,r,i);
    }
    vector<int> first(sorted.size(),-1),mostRight(sorted.size(),-1);
    int maxDis = 0;
    int snapshot_maxDis = 0;
    auto addRight = [&](int idx) {
        if (first[nums[idx]] == -1) first[nums[idx]] = idx;
        mostRight[nums[idx]] = idx;
        maxDis = max(maxDis,mostRight[nums[idx]] - first[nums[idx]]);
    };
    auto addLeft = [&](int idx) {
        if (mostRight[nums[idx]] == -1) mostRight[nums[idx]] = idx;
        maxDis = max(maxDis,mostRight[nums[idx]] - idx);
    };
    auto getAns = [&](){ return maxDis; };

    auto snapshot = [&]() { snapshot_maxDis = maxDis; };

    auto rollback = [&](int L, int R) {
        for (int i = L; i <= R; ++i) {
            if (mostRight[nums[i]] == i) mostRight[nums[i]] = -1;
            //这里需要清空first，因为暴力的时候用了first。
            if (first[nums[i]] == i) first[nums[i]] = -1;
        }
        maxDis = snapshot_maxDis;
    };

    auto reset = [&]() {
        first.assign(sorted.size(),-1);
        mostRight.assign(sorted.size(),-1);
        maxDis = 0;
    };

    mo.solve(addLeft,addRight, getAns, snapshot, rollback, reset);
    auto& res = mo.getResults();
    for (int i = 0; i < m; ++i) {
        cout << res[i] << endl;
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