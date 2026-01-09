#include <bits/stdc++.h>
using namespace std;

template <typename AnsType>
class ModifyMoAlgorithm {
public:
    struct Query {
        int l, r, time, id;
        int param;
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

    void addModify(int pos, int oldVal, int newVal) {
        modifies.push_back({pos, oldVal, newVal});
    }

    template <typename AddLeft, typename AddRight, typename RemoveLeft, typename RemoveRight, typename GetAns>
    vector<AnsType> solve(AddLeft addLeft, AddRight addRight,
                         RemoveLeft removeLeft, RemoveRight removeRight, GetAns getAns) {
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

            while (curTime < T) {
                const auto& mod = modifies[curTime];
                //移除旧值的贡献
                if (curL <= mod.pos && mod.pos <= curR) {
                    removeRight(mod.pos);
                }
                //变换为新的值，然后添加新值的贡献
                arr[mod.pos] = mod.newVal;
                if (curL <= mod.pos && mod.pos <= curR) {
                    addRight(mod.pos);
                }
                curTime++;
            }

            while (curTime > T) {
                curTime--;
                const auto& mod = modifies[curTime];
                //移除新值的贡献
                if (curL <= mod.pos && mod.pos <= curR) {
                    removeRight(mod.pos);
                }
                //获取老值的贡献
                arr[mod.pos] = mod.oldVal;
                if (curL <= mod.pos && mod.pos <= curR) {
                    addRight(mod.pos);
                }
            }

            while (curR < R) addRight(++curR);
            while (curR > R) removeRight(curR--);
            while (curL < L) removeLeft(curL++);
            while (curL > L) addLeft(--curL);

            ans[id] = getAns(query.param);
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

    vector<int> a_original(n);
    for (int i = 0; i < n; i++) {
        cin >> a_original[i];
    }

    // 收集所有值
    set<int> valueSet;
    for (int i = 0; i < n; i++) {
        valueSet.insert(a_original[i]);
    }

    struct Operation {
        char op;
        int arg1, arg2, arg3;
    };
    vector<Operation> operations;

    for (int i = 0; i < m; i++) {
        char op;
        cin >> op;

        if (op == 'Q') {
            int A, B, K;
            cin >> A >> B >> K;
            valueSet.insert(K);
            operations.push_back({op, A, B, K});
        } else {
            int A, P;
            cin >> A >> P;
            valueSet.insert(P);
            operations.push_back({op, A, P, 0});
        }
    }

    // 离散化
    vector<int> sortedValues(valueSet.begin(), valueSet.end());
    unordered_map<int, int> valToId;
    for (int i = 0; i < sortedValues.size(); i++) {
        valToId[sortedValues[i]] = i;
    }

    // 离散化原数组
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        a[i] = valToId[a_original[i]];
    }

    ModifyMoAlgorithm<int> mo(n, a);

    // 模拟修改来获取正确的 oldVal
    vector<int> currentArray = a;  // 复制一份用于追踪当前状态

    for (const auto& [op, arg1, arg2, arg3] : operations) {
        if (op == 'Q') {
            int A = arg1 - 1, B = arg2 - 1;
            int K = valToId[arg3];
            mo.addQuery(A, B, K);
        } else {  // op == 'C'
            int A = arg1 - 1;
            int P = valToId[arg2];
            int oldVal = currentArray[A];  // 记录修改前的值
            mo.addModify(A, oldVal, P);
            currentArray[A] = P;  // 更新当前状态
        }
    }

    // 维护每种书的数量
    vector<int> cnt(sortedValues.size(), 0);

    auto addLeft = [&](int pos) {
        cnt[a[pos]]++;
    };

    auto addRight = [&](int pos) {
        cnt[a[pos]]++;
    };

    auto removeLeft = [&](int pos) {
        cnt[a[pos]]--;
    };

    auto removeRight = [&](int pos) {
        cnt[a[pos]]--;
    };

    auto getAns = [&](int K) {
        return cnt[K];
    };

    auto ans = mo.solve(addLeft, addRight, removeLeft, removeRight, getAns);

    for (int x : ans) {
        cout << x << "\n";
    }

    return 0;
}