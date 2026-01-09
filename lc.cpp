//
// Created by 1 on 2026/1/4.
//
#include <bits/stdc++.h>
using namespace std;
// --- 只增回滚莫队类模板 ---
template <typename AnsType>
class AddOnlyMo {
public:
    struct Query {
        int l, r,threshold,id;
    };

    AddOnlyMo(int n, int q) : n(n), q(q) {
        // 使用整数计算块大小，避免精度问题
        blockSize = max(1, (int)(n / sqrt(max(1.0, (double)q))));
        queries.reserve(q);
        ans.resize(q);
    }

    void addQuery(int l, int r, int threshold,int id) {
        queries.push_back({l, r, threshold,id});
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
                ans[id] = getAns(id);
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

            ans[id] = getAns(id);

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
class Solution {
public:
    vector<int> subarrayMajority(vector<int>& nums, vector<vector<int>>& queries) {
        //查询众数
        int n = nums.size(),m = queries.size();
        AddOnlyMo<int> mo(n,m);
        for (int i = 0; i < m; ++i) {
            auto [l,r,threshold] =  queries[i];
            mo.addQuery(l,r, threshold,i);
        }
        // 离散化
        vector<int> sorted(nums.begin(), nums.end());
        sort(sorted.begin(), sorted.end());
        sorted.erase(unique(sorted.begin(), sorted.end()),sorted.end());
        for (int i = 0; i < n; ++i) {
            nums[i] = lower_bound(sorted.begin(), sorted.end(), nums[i]) - sorted.begin();
        }
        //莫队
        vector<int> cnt(sorted.size(),0);
        int mode = -1,modecnt = 0;
        int backend_mode = -1,backend_modecnt = 0;
        auto add = [&](int idx) {
            cnt[nums[idx]]++;
            if (cnt[nums[idx]] > modecnt || (cnt[nums[idx] == modecnt && sorted[nums[idx]]] < mode)) {
                mode = sorted[nums[idx]];
                modecnt = cnt[nums[idx]];
            }
        };
        auto getAns = [&](int idx) {
            return  modecnt >= queries[idx][2] ? mode : -1;
        };
        auto snapshot = [&]() { backend_mode = mode;  backend_modecnt = modecnt ; };

        auto rollback = [&](int L, int R) {
            for (int i = L; i <= R; ++i) {
                cnt[nums[i]]--;
            }
            mode = backend_mode; modecnt = backend_modecnt;
        };

        auto reset = [&]() {
            // 只清空出现的种类，提高效率（此处 MAXN 也可）
            cnt.assign(n,0);
            modecnt = 0;
        };

        mo.solve(add, getAns, snapshot, rollback, reset);

        auto& ans = mo.getResults();
        return ans;
    }
};