#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>

using namespace std;

typedef long long ll;

// --- 只增回滚莫队类模板 ---
template <typename AnsType>
class AddOnlyMo {
public:
    struct Query {
        int l, r, id;
    };

    AddOnlyMo(int n, int q) : n(n), q(q) {
        // 使用整数计算块大小，避免精度问题
        blockSize = max(1, (int)(n / sqrt(max(1.0, (double)q))));
        queries.reserve(q);
        ans.resize(q);
    }

    void addQuery(int l, int r, int id) {
        queries.push_back({l, r, id});
    }

    template <typename Add, typename GetAns, typename Snapshot, typename Rollback, typename Reset>
    void solve(Add add, GetAns getAns, Snapshot snapshot, Rollback rollback, Reset reset) {
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
                for (int i = L; i <= R; ++i) add(i);
                ans[id] = getAns();
                rollback(L, R); // 撤销本次暴力增加的贡献
                continue;
            }

            // 3. 情况2：跨块，右指针单调移动
            while (curR < R) {
                add(++curR);
            }

            // 4. 左指针临时向左移动并回滚
            snapshot();
            int blockRight = (bL + 1) * blockSize;
            for (int i = blockRight - 1; i >= L; --i) {
                add(i);
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



int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<int> nums(n);
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    //离散化
    vector<int> sorted(nums.begin(), nums.end());
    sort(sorted.begin(), sorted.end());
    sorted.erase(unique(sorted.begin(), sorted.end()),sorted.end());
    for (int i = 0; i < n; ++i) {
        nums[i] = lower_bound(sorted.begin(), sorted.end(), nums[i]) - sorted.begin();
    }


    AddOnlyMo<ll> mo(n, m);
    for (int i = 0; i < m; ++i) {
        int l, r;
        cin >> l >> r;
        mo.addQuery(l - 1, r - 1, i); // 转换为 0-based
    }
    ll curMax = 0;
    ll snapshotMax = 0;
    vector<int> cnt(sorted.size());
    // 核心逻辑 Lambda
    auto add = [&](int idx) {
        cnt[nums[idx]]++;
        curMax = max(curMax, (ll)cnt[nums[idx]] * sorted[nums[idx]]);
    };

    auto getAns = [&]() -> ll{ return curMax; };

    auto snapshot = [&]() { snapshotMax = curMax; };

    auto rollback = [&](int L, int R) {
        for (int i = L; i <= R; ++i) {
            cnt[nums[i]]--;
        }
        curMax = snapshotMax;
    };

    auto reset = [&]() {
        // 只清空出现的种类，提高效率（此处 MAXN 也可）
        cnt.assign(n,0);
        curMax = 0;
    };

    mo.solve(add, getAns, snapshot, rollback, reset);

    const auto& results = mo.getResults();
    for (ll res : results) {
        printf("%lld\n", res);
    }

    return 0;
}