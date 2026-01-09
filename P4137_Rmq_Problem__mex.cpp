//  Luogu contest  P4137 Rmq Problem / mex
//  Created at 04.01.2026 19:22:56 in file P4137_Rmq_Problem__mex.cpp
//  By zfq
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

using namespace std;

/**
 * @brief 只删不增莫队模板 (Delete-Only Mo's Algorithm)
 * 适用于：删除容易、回滚容易，但增加较难或不满足单调性的问题（如 MEX）。
 * 原理：从全集开始，R 只有向左减小，L 临时向右增加并回滚。
 */
template <typename AnsType>
class DeleteOnlyMo {
public:
    struct Query {
        int l, r, id;
    };

    DeleteOnlyMo(int n, int q) : n(n), q(q) {
        blockSize = max(1, (int)(n / sqrt(max(1, q))));
        queries.reserve(q);
        ans.resize(q);
    }

    void addQuery(int l, int r, int id) {
        queries.push_back({l, r, id});
    }

    /**
     * @param del 删除索引 i 的函数: void(int i)
     * @param add 增加索引 i 的函数: void(int i) -- 用于左指针回滚
     * @param getAns 获取当前答案: AnsType()
     * @param resetToFull 重置到 [0, n-1] 全集状态: void()
     */
    template <typename Del, typename Add, typename GetAns, typename ResetToFull>
    void solve(Del del, Add add, GetAns getAns, ResetToFull resetToFull) {
        // 排序逻辑：L 所在块升序，R 降序
        sort(queries.begin(), queries.end(), [&](const Query& a, const Query& b) {
            int b1 = a.l / blockSize, b2 = b.l / blockSize;
            return b1 != b2 ? b1 < b2 : a.r > b.r;
        });

        int curL = 0, curR = n - 1, lastBlock = -1;
        AnsType snapshotAns;

        for (auto& qry : queries) {
            int L = qry.l, R = qry.r, id = qry.id;
            int bL = L / blockSize;

            // 如果跨块，重置为全集 [0, n-1]
            if (bL != lastBlock) {
                resetToFull();
                curL = 0;
                curR = n - 1;
                lastBlock = bL;
            }

            // 1. 右指针 R 移动：向左删，移动后不可逆（在本块内）
            while (curR > R) del(curR--);

            // 2. 左指针 L 移动：临时向右删
            snapshotAns = getAns(); // 记录当前状态（快照）
            int savedL = curL;
            while (curL < L) del(curL++);

            // 3. 获取答案
            ans[id] = getAns();

            // 4. 回滚左指针：将 [savedL, L-1] 段加回来
            while (curL > savedL) {
                curL--;
                add(curL);
            }
            // 恢复答案状态（针对 MEX 这种只减不增的变量）
            // 注意：如果 getAns 本身就是根据当前状态计算的，某些情况下此步可由 add 函数自然完成
            // 但在 MEX 中，我们直接手动还原快照值
            setInternalAns(snapshotAns);
        }
    }

    // 辅助：由于 MEX 具有单调性，回滚时直接覆盖值比重算快
    void setInternalAns(AnsType val) { currentInternalAns = val; }
    AnsType getInternalAns() { return currentInternalAns; }

    const vector<AnsType>& getResults() const { return ans; }

private:
    int n, q, blockSize;
    vector<Query> queries;
    vector<AnsType> ans;
    AnsType currentInternalAns;
};

// --- 业务代码：解决 MEX 问题 ---

int main() {
    // 快速 IO
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        // 离散化优化：MEX 不可能超过 n
        if (a[i] > n) a[i] = n + 1;
    }

    DeleteOnlyMo<int> mo(n, m);
    for (int i = 0; i < m; i++) {
        int l, r;
        cin >> l >> r;
        mo.addQuery(l - 1, r - 1, i);
    }

    // 状态变量
    vector<int> cnt(n + 2, 0);
    int currentMEX = 0;

    // 删除操作：如果计数归零且比当前 MEX 小，更新 MEX
    auto del = [&](int idx) {
        int val = a[idx];
        if (val <= n) {
            if (--cnt[val] == 0) {
                if (val < currentMEX) currentMEX = val;
            }
        }
    };

    // 增加操作：仅用于回滚，不需要更新 currentMEX（因为 snapshot 会处理）
    auto add = [&](int idx) {
        int val = a[idx];
        if (val <= n) {
            cnt[val]++;
        }
    };

    auto reset = [&]() {
        fill(cnt.begin(), cnt.end(), 0);
        for (int x : a) if (x <= n) cnt[x]++;
        currentMEX = 0;
        while (cnt[currentMEX] > 0) currentMEX++;
    };

    // 执行
    mo.solve(
        del,
        add,
        [&]() { return currentMEX; },
        reset
    );

    // 输出结果
    for (auto res : mo.getResults()) {
        cout << res << "\n";
    }

    return 0;
}