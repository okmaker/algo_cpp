//  Luogu contest  AT_abc437_e [ABC437E] Sort Arrays
//  Created at 10.01.2026 23:14:16 in file AT_abc437_e_ABC437E_Sort_Arrays.cpp
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
using v2d = vector<vector<int> >;
const double eps = 1e-7;
int dx[] = {0, 0, 1, -1};
int dy[] = {1, -1, 0, 0};
const int MOD = 1e9 + 7;
const int N = 1e5 + 5;
const int M = 1e5 + 7;
/*
    描述就是类似字典树。
*/
struct Node {
    map<int,Node*> next;
    vector<int> ids;
};
void solve() {
    int n; cin >> n;
    vector<Node*> pos(n + 1);
    Node* root = new Node();
    pos[0] = root;
    for (int i = 1; i <= n; ++i) {
        int x,y; cin >> x >> y;
        Node *fa = pos[x];
        if (!fa->next.contains(y)) fa->next[y] = new Node();
        Node* cur = fa->next[y];
        cur->ids.push_back(i);
        pos[i] = cur;
    }
    auto dfs = [&](this auto&& dfs,Node* cur) -> void {
        for (int id: cur->ids) cout << id << " ";
        for (auto son: cur->next | views::values) {
            dfs(son);
        }
    };
    dfs(root);

}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int T = 1; //cin >> T;
    while (T--) {
        solve();
    }

    return 0;
}
