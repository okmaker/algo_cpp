//  Luogu contest  P3374 【模板】树状数组 1
//  Created at 16.01.2026 22:45:32 in file P3374__1.cpp
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
/*
    
*/
// indexed-0 ,左闭右开区间[l,r)
namespace atcoder {
	int ceil_pow2(int n) {
		int x = 0;
		while ((1U << x) < (unsigned int)(n)) x++;
		return x;
	}
	template <class S, S(*op)(S, S), S(*e)()> struct segtree {
	public:
		segtree() : segtree(0) {}
		explicit segtree(int n) : segtree(std::vector<S>(n, e())) {}
		explicit segtree(const std::vector<S>& v) : _n((int)(v.size())) {
			log = ceil_pow2(_n);
			size = 1 << log;
			d = std::vector<S>(2 * size, e());
			for (int i = 0; i < _n; i++) d[size + i] = v[i];
			for (int i = size - 1; i >= 1; i--) {
				update(i);
			}
		}

		void set(int p, S x) {
			assert(0 <= p && p < _n);
			p += size;
			d[p] = x;
			for (int i = 1; i <= log; i++) update(p >> i);
		}

		S get(int p) const {
			assert(0 <= p && p < _n);
			return d[p + size];
		}

		S prod(int l, int r) const {
			assert(0 <= l && l <= r && r <= _n);
			S sml = e(), smr = e();
			l += size;
			r += size;

			while (l < r) {
				if (l & 1) sml = op(sml, d[l++]);
				if (r & 1) smr = op(d[--r], smr);
				l >>= 1;
				r >>= 1;
			}
			return op(sml, smr);
		}

		S all_prod() const { return d[1]; }

		template <bool(*f)(S)> int max_right(int l) const {
			return max_right(l, [](S x) { return f(x); });
		}
		template <class F> int max_right(int l, F f) const {
			assert(0 <= l && l <= _n);
			assert(f(e()));
			if (l == _n) return _n;
			l += size;
			S sm = e();
			do {
				while (l % 2 == 0) l >>= 1;
				if (!f(op(sm, d[l]))) {
					while (l < size) {
						l = (2 * l);
						if (f(op(sm, d[l]))) {
							sm = op(sm, d[l]);
							l++;
						}
					}
					return l - size;
				}
				sm = op(sm, d[l]);
				l++;
			} while ((l & -l) != l);
			return _n;
		}

		template <bool(*f)(S)> int min_left(int r) const {
			return min_left(r, [](S x) { return f(x); });
		}
		template <class F> int min_left(int r, F f) const {
			assert(0 <= r && r <= _n);
			assert(f(e()));
			if (r == 0) return 0;
			r += size;
			S sm = e();
			do {
				r--;
				while (r > 1 && (r % 2)) r >>= 1;
				if (!f(op(d[r], sm))) {
					while (r < size) {
						r = (2 * r + 1);
						if (f(op(d[r], sm))) {
							sm = op(d[r], sm);
							r--;
						}
					}
					return r + 1 - size;
				}
				sm = op(d[r], sm);
			} while ((r & -r) != r);
			return 0;
		}

	private:
		int _n, size, log;
		std::vector<S> d;

		void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
	};

}  // namespace atcoder
using namespace atcoder;
auto op(int a,int b) {
	return a + b;
}
auto e() {
	return 0ll;
}
void solve(){
    int n,m; cin >> n >> m;
	vector<int> arr(n);
	for (int i = 0; i < n; i++) cin >> arr[i];
	segtree<int,op,e> seg(arr);
	while (m--) {
		int op,x,y;cin >> op >> x >> y;
		if (op == 1) {
			seg.set(x - 1,seg.get(x - 1) + y);
		} else {
			cout << seg.prod(x - 1,y) << endl;
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