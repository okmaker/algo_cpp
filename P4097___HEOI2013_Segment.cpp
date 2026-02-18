//  Luogu contest  P4097 【模板】李超线段树 / [HEOI2013] Segment
//  Created at 06.02.2026 22:12:00 in file P4097___HEOI2013_Segment.cpp
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
constexpr double eps = 1e-8;
constexpr int dx[] =  {0,0,1,-1};
constexpr int dy[] = {1,-1,0,0};
constexpr int MOD = 39989;
constexpr int N = 1e5 + 5;
constexpr int M = 1e5 + 7;
/*
    
*/
#include <bits/stdc++.h>
using namespace std;

/**
 * 通用动态开点李超线段树
 * * @tparam LineType   用户自定义的线段结构体，必须包含方法: ValType eval(CoordType x)
 * @tparam CoordType  坐标类型 (int, long long)
 * @tparam ValType    计算结果类型 (double, long long)
 * @tparam Compare    比较策略 (functor/lambda), 返回 true 代表 line_new 优于 line_old
 */
template<typename LineType, typename CoordType, typename ValType, typename Compare = less<ValType>>
class LiChaoTree {
public:
    struct Node {
        int ls, rs;
        LineType line;
        bool has_val;
    };

    vector<Node> tr;
    int root;
    CoordType MIN_X, MAX_X;
    LineType NULL_LINE;
    Compare comp;

    // 构造函数
    LiChaoTree(CoordType min_x, CoordType max_x, LineType null_line, Compare cmp_func = Compare())
        : MIN_X(min_x), MAX_X(max_x), NULL_LINE(null_line), comp(cmp_func) {
        root = 0;
        // 0号节点作为空节点哨兵
        tr.push_back({0, 0, NULL_LINE, false});
    }

    // 动态开点
    void new_node(int &u) {
        if (!u) {
            u = tr.size();
            tr.push_back({0, 0, NULL_LINE, false});
        }
    }

    // 核心：插入直线 (覆盖整个当前区间)
    void insert_internal(int &u, CoordType l, CoordType r, LineType new_line) {
        new_node(u);
        if (!tr[u].has_val) {
            tr[u].line = new_line;
            tr[u].has_val = true;
            return;
        }

        CoordType mid = l + (r - l) / 2;
        if (comp(new_line, tr[u].line, mid)) {
            swap(tr[u].line, new_line);
        }

        if (l == r) return;

        if (comp(new_line, tr[u].line, l)) insert_internal(tr[u].ls, l, mid, new_line);

        else if (comp(new_line, tr[u].line, r)) insert_internal(tr[u].rs, mid + 1, r, new_line);
    }

    // 插入线段 (仅覆盖 [ql, qr])
    void update_segment(int &u, CoordType l, CoordType r, CoordType ql, CoordType qr, LineType new_line) {
        if (ql <= l && r <= qr) {
            insert_internal(u, l, r, new_line);
            return;
        }
        new_node(u);
        CoordType mid = l + (r - l) / 2;
        if (ql <= mid) update_segment(tr[u].ls, l, mid, ql, qr, new_line);
        if (qr > mid) update_segment(tr[u].rs, mid + 1, r, ql, qr, new_line);
    }

    // 查询
    pair<ValType, LineType> query_internal(int u, CoordType l, CoordType r, CoordType x) {
        if (!u || !tr[u].has_val) return {NULL_LINE.eval(x), NULL_LINE};

        pair<ValType, LineType> res = {tr[u].line.eval(x), tr[u].line};

        if (l == r) return res;

        CoordType mid = l + (r - l) / 2;
        pair<ValType, LineType> child_res;

        if (x <= mid) child_res = query_internal(tr[u].ls, l, mid, x);
        else child_res = query_internal(tr[u].rs, mid + 1, r, x);

        if (comp(child_res.second, res.second, x)) {
            return child_res;
        }
        return res;
    }

public:
    //添加全局直线
    void add_line(LineType line) {
        insert_internal(root, MIN_X, MAX_X, line);
    }

    // 添加局部线段
    void add_segment(CoordType l, CoordType r, LineType line) {
        update_segment(root, MIN_X, MAX_X, l, r, line);
    }

    // 查询
    pair<ValType, LineType> query(CoordType x) {
        return query_internal(root, MIN_X, MAX_X, x);
    }
};



struct Line {
    double k, b;
    int id;
    double eval(int x) const { return k * x + b; }
};

struct Cmp {
    bool operator()(const Line& new_line, const Line& old_line, int x) const {
        if (old_line.id == 0) return true;
        if (new_line.id == 0) return false;

        double val_new = new_line.eval(x);
        double val_old = old_line.eval(x);

        if (fabs(val_new - val_old) < eps) {
            return new_line.id < old_line.id;
        }
        return val_new > val_old;
    }
};
void solve(){
    int n; cin >> n;

    // 空线段：id=0, 值=-1e18
    Line null_line = {0, -1e18, 0};
    LiChaoTree<Line, int, double, Cmp> lct(1, 40000, null_line);

    int lastans = 0, cnt = 0;
    while(n--) {
        int op; cin >> op;
        if (op == 1) {
            int x0, y0, x1, y1; cin >> x0 >> y0 >> x1 >> y1;
            x0 = (x0 + lastans - 1) % MOD + 1;
            x1 = (x1 + lastans - 1) % MOD + 1;
            y0 = (y0 + lastans - 1) % 1000000000 + 1;
            y1 = (y1 + lastans - 1) % 1000000000 + 1;
            if (x0 > x1) { swap(x0, x1); swap(y0, y1); }

            cnt++;
            Line line;
            line.id = cnt;

            line.k = (double)(y1 - y0) / (x1 - x0);
            line.b = y0 - line.k * x0;
            // 插入线段
            lct.add_segment(x0, x1, line);
        } else {
            int k; cin >> k;
            int x = (k + lastans - 1) % MOD + 1;
            auto res = lct.query(x);
            lastans = res.second.id;
            cout << lastans << endl;
        }
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