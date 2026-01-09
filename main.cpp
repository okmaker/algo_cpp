#include <bits/stdc++.h>
using namespace std;

template <typename AnsType>
class ModifyMoAlgorithm {
public:
    struct Query {
        int l, r, time, id;
        int param;  // ✅ 添加额外参数字段
    };

    struct Modify {
        int pos, oldVal, newVal;
    };

    ModifyMoAlgorithm(int n, vector<int>& arr) : n(n), arr(arr) {
        blockSize = max(1, (int)pow(n, 2.0/3.0));
    }

    void addQuery(int l, int r, int param = 0) {
        queries.push_back({l, r, (int)modifies.size(), (int)queries.size(), param});
    }

    void addModify(int pos, int newVal) {
        modifies.push_back({pos, arr[pos], newVal});
    }

    template <typename Add, typename Remove, typename GetAns>
    vector<AnsType> solve(Add add, Remove remove, GetAns getAns) {
        vector<AnsType> ans(queries.size());

        sort(queries.begin(), queries.end(), [&](const Query& a, const Query& b) {
            int blockA_L = a.l / blockSize;
            int blockB_L = b.l / blockSize;
            int blockA_R = a.r / blockSize;
            int blockB_R = b.r / blockSize;

            if (blockA_L != blockB_L) return blockA_L < blockB_L;
            if (blockA_R != blockB_R) return blockA_R < blockB_R;
            return a.time < b.time;
        });

        int curL = 0, curR = -1, curTime = 0;

        for (const auto& query : queries) {
            int L = query.l, R = query.r, T = query.time, id = query.id;

            // 处理修改
            while (curTime < T) {
                const auto& mod = modifies[curTime];
                if (curL <= mod.pos && mod.pos <= curR) {
                    remove(mod.pos);
                }
                arr[mod.pos] = mod.newVal;
                if (curL <= mod.pos && mod.pos <= curR) {
                    add(mod.pos);
                }
                curTime++;
            }

            while (curTime > T) {
                curTime--;
                const auto& mod = modifies[curTime];
                if (curL <= mod.pos && mod.pos <= curR) {
                    remove(mod.pos);
                }
                arr[mod.pos] = mod.oldVal;
                if (curL <= mod.pos && mod.pos <= curR) {
                    add(mod.pos);
                }
            }

            // 移动区间
            while (curR < R) add(++curR);
            while (curR > R) remove(curR--);
            while (curL < L) remove(curL++);
            while (curL > L) add(--curL);

            ans[id] = getAns(query.param);  // ✅ 传递参数
        }

        return ans;
    }

private:
    int n, blockSize;
    vector<int>& arr;
    vector<Query> queries;
    vector<Modify> modifies;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    ModifyMoAlgorithm<int> mo(n, a);

    for (int i = 0; i < m; i++) {
        char op;
        cin >> op;

        if (op == 'Q') {
            int A, B, K;
            cin >> A >> B >> K;
            A--; B--;  // 转为 0-indexed
            mo.addQuery(A, B, K);  // ✅ 传递 K 作为参数
        } else {
            int A, P;
            cin >> A >> P;
            A--;  // 转为 0-indexed
            mo.addModify(A, P);
        }
    }

    // 维护每种书的数量
    const int MAXVAL = 100005;
    vector<int> cnt(MAXVAL, 0);

    auto add = [&](int pos) {
        cnt[a[pos]]++;
    };

    auto remove = [&](int pos) {
        cnt[a[pos]]--;
    };

    auto getAns = [&](int K) {  // ✅ 接收参数 K
        return cnt[K];
    };

    auto ans = mo.solve(add, remove, getAns);

    for (int x : ans) {
        cout << x << "\n";
    }

    return 0;
}