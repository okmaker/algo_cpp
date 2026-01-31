//  Luogu contest  P1127 词链
//  Created at 19.01.2026 13:44:18 in file P1127_.cpp
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
constexpr double eps = 1e-7;
constexpr int dx[] =  {0,0,1,-1};
constexpr int dy[] = {1,-1,0,0};
constexpr int MOD = 1e9 + 7;
constexpr int N = 1e5 + 5;
constexpr int M = 1e5 + 7;
/*
    每一个字母的开头和结尾字母就是节点，整个字母就是一个边权为字母的边。
    经过的边路径如何记录？  记录到这个节点的边权即可。
*/
void solve(){
    int n = 26;
    int m; cin >> m;
    vector<vector<pair<int,string>>> g(n);
    vector<int> deg(n);
    vector<string> words;
    for (int i = 0; i < m; i++) {
        string s; cin >> s;
        words.push_back(s);
    }
    //ranges::sort(words,greater<>());
    for (int i = 0; i < m; i++) {
        string& s = words[i];
        g[s[0] - 'a'].push_back({s.back() - 'a',s});
        deg[s[0] - 'a'] -= 1;
        deg[s.back() - 'a'] += 1;
    }
    //找起点
    int start = -1,end = -1;
    for (int i = 0; i < n; i++) {
        if (abs(deg[i]) > 1 || (start != -1 && deg[i] == -1) || (end != -1 && deg[i] == 1)) {
            cout << "***" << endl;
            return;
        }
        if (deg[i] == -1) start = i;
        if (deg[i] == 1) end = i;
    }
    if ((start == -1) ^ (end == -1)) {
        cout << "***" << endl;
        return;
    }
    if (start == -1) {
        for (int i = 0; i < n; i++) {
            if (!g[i].empty()) { // 找到第一个有出边的点
                start = i;
                break;
            }
        }
        if (start == -1) {
            cout << "***" << endl;
            return;
        }
    }

    //找路径即可。
    for (int i = 0; i < n; i++) ranges::sort(g[i], [](auto &a,auto &b){return a.second > b.second;});
    vector<string> path;
    auto dfs = [&](this auto &&dfs,int cur,string w) -> void {
        while (g[cur].size()) {
            auto [nxt,nw] = g[cur].back();
            g[cur].pop_back();
            dfs(nxt,nw);
        }
        path.push_back(w);
    };
    dfs(start, "");
    path.pop_back();
    if (path.size() != m) {
        cout << "***" << endl;
        return;
    }
    ranges::reverse(path);
    for (int i = 0; i < path.size() - 1; i++) {
        cout << path[i] << ".";
    }
    cout << path.back() << endl;
}
signed main() {
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int T = 1;//cin >> T;
    while(T--){
        solve();
    }
    
    return 0;
}