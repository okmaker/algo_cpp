#include <bits/stdc++.h>
using namespace std;

namespace FastIO {
    char buf[1 << 21], *p1 = buf, *p2 = buf;
    inline char getc() {
        return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, 1 << 21, stdin), p1 == p2) ? EOF : *p1++;
    }
    template <typename T>
    inline void read(T &x) {
        x = 0; char ch = getc();
        while (ch < '0' || ch > '9') ch = getc();
        while (ch >= '0' && ch <= '9') { x = x * 10 + ch - '0'; ch = getc(); }
    }
}
using namespace FastIO;

const int N = 1000005;
int head[N], nxt[N], to[N], tot;
long long a[N], sufxor[N], res[N];
int dep[N], n, m;

// 深度桶：记录全局范围内每一层所有节点的 sufxor 异或和
long long layer_bucket[N];

struct Query {
    int h, id;
};
vector<Query> qs[N];

inline void add_edge(int u, int v) {
    to[++tot] = v; nxt[tot] = head[u]; head[u] = tot;
}

// 第一遍 DFS：算基础子树异或和 sufxor[u]
void dfs_base(int u, int d) {
    dep[u] = d;
    sufxor[u] = a[u];
    for (int i = head[u]; i; i = nxt[i]) {
        dfs_base(to[i], d + 1);
        sufxor[u] ^= sufxor[to[i]];
    }
}

// 第二遍 DFS：利用差分思想处理深度限制
void dfs_solve(int u) {
    // 记录进入子树前，特定深度的全局异或值
    for (auto &q : qs[u]) {
        int target = dep[u] + q.h + 1;
        if (target < N) {
            // 此时记录的是“子树外”在该深度的异或状态
            res[q.id] = layer_bucket[target];
        }
    }

    // 更新当前节点贡献到全局桶
    layer_bucket[dep[u]] ^= sufxor[u];

    for (int i = head[u]; i; i = nxt[i]) {
        dfs_solve(to[i]);
    }

    // 遍历完子树后，用现在的全局桶异或掉进入前的状态，得到的就是该子树内部的贡献
    for (auto &q : qs[u]) {
        int target = dep[u] + q.h + 1;
        long long subtree_layer_xor = 0;
        if (target < N) {
            subtree_layer_xor = layer_bucket[target] ^ res[q.id];
        }
        // 最终答案 = 总子树异或和 ^ 距离超过 h 的子树层异或和
        res[q.id] = sufxor[u] ^ subtree_layer_xor;
    }
}

int main() {
    read(n); read(m);
    for (int i = 1; i <= n; i++) read(a[i]);
    for (int i = 2; i <= n; i++) {
        int f; read(f); add_edge(f, i);
    }
    for (int i = 0; i < m; i++) {
        int x, h; read(x); read(h);
        qs[x].push_back({h, i});
    }

    dfs_base(1, 1);
    dfs_solve(1);

    for (int i = 0; i < m; i++) {
        printf("%.3f\n", res[i] / 1000.0);
    }

    return 0;
}