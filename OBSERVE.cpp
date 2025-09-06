#include <bits/stdc++.h>
#define int long long
#define FOR(i, a, b) for (int i = (a), _b = (b); i <= _b; ++i)
#define REV(i, a, b) for (int i = (a), _b = (b); i >= _b; --i)
using namespace std;

#define MAXN 1000005
int n, m, k;
int a[MAXN];
set<int> s[MAXN];

struct SMT {
    int n;
    vector<int> st, lz;
    SMT(int _n = 0){
        n = _n;
        st.assign((n << 2) + 5, 0);
        lz.assign((n << 2) + 5, 0);
    }

    void push(int id){
        if (lz[id] == 0) return;
        st[id << 1] += lz[id];
        lz[id << 1] += lz[id];
        st[id << 1 | 1] += lz[id];
        lz[id << 1 | 1] += lz[id];
        lz[id] = 0;
    }

    void upd(int id, int l, int r, int u, int v, int val){
        if (v < l || r < u) return;
        if (u <= l && r <= v){
            st[id] += val;
            lz[id] += val;
            return;
        }
        int mid = (l + r) >> 1;
        push(id);
        upd(id << 1, l, mid, u, v, val);
        upd(id << 1 | 1, mid + 1, r, u, v, val);
        st[id] = max(st[id << 1], st[id << 1 | 1]);
    }
    void upd(int u, int v, int val){
        if (u > v) return;
        upd(1, 1, n, u, v, val);
    }

    int get(int id, int l, int r, int u, int v) {
        if (v < l || r < u) return 0;
        if (u <= l && r <= v) return st[id];
        int mid = (l + r) >> 1;
        push(id);
        return max(get(id << 1, l, mid, u, v), get(id << 1 | 1, mid + 1, r, u, v));
    }
    int get(int u, int v) {
        if (u > v) return 0;
        return get(1, 1, n, u, v);
    }
} tree;

namespace xulibandau {

    int cnt[MAXN];

    int sum = 0;

    void add(int x){
        if (cnt[x] == 0) sum++;
        cnt[x]++;
    }

    void del(int x){
        if (cnt[x] == 1) sum--;
        cnt[x]--;
    }

    void solve(){
        FOR(i, 1, n){
            add(a[i]);
            if (i >= k) {
                tree.upd(i, i, sum);
                del(a[i - k + 1]);
            }
        }
    }
}

void delSet(int color, int id){
    auto it = s[color].lower_bound(id);
    assert(it != s[color].end() && *it == id);
    int okRight = min(n, *it + k - 1);
    auto next_it = it; next_it++;
    if (next_it != s[color].end()) okRight = min(okRight, *next_it - 1);
    int okLeft = *it;
    auto prev_it = it;
    if (prev_it != s[color].begin()){
        prev_it--;
        okLeft = max(okLeft, *prev_it + k);
    }
    tree.upd(okLeft, okRight, -1);
    s[color].erase(it);
}

void addSet(int color, int id){
    auto it = s[color].lower_bound(id);
    assert(it == s[color].end() || (it != s[color].end() && *it != id));
    int okRight = min(n, id + k - 1);
    if (it != s[color].end()) okRight = min(okRight, *it - 1);
    int okLeft = id;
    auto prev_it = it;
    if (prev_it != s[color].begin()){
        prev_it--;
        okLeft = max(okLeft, *prev_it + k);
    }
    tree.upd(okLeft, okRight, 1);
    s[color].insert(id);
}

void solve(){
    cin >> n >> m >> k;
    FOR(i, 1, n){
        cin >> a[i];
        s[a[i]].insert(i);
    }
    tree = SMT(n);
    xulibandau::solve();
    FOR(i, 1, m){
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 2){
            l = l + k - 1;
            cout << tree.get(l, r) << '\n';
        } else {
            delSet(a[l], l);
            a[l] = r;
            addSet(a[l], l);
        }
    }
}

/*
5 3 3
1 2 3 3 3
2 1 3
1 3 2
2 1 5
*/

#define task "OBSERVE"
int32_t main(){
    if (fopen(task".inp","r")){
        freopen(task".inp","r",stdin);
        freopen(task".out","w",stdout);
    }
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    solve();
    return 0;
}
