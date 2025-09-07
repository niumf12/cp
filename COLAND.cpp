#include <bits/stdc++.h>
#define MASK(i)     (1 << (i))
#define FOR(i, a, b) for (int i = (a), _b = (b); i <= _b; ++i)
#define REV(i, a, b) for (int i = (a), _b = (b); i >= _b; --i)
using namespace std;

int n, m, t;
#define MAXN 105
bool a[MAXN][MAXN];
int query[MAXN];
const int dx[] = {0, 1, 0, -1};
const int dy[] = {-1, 0, 1, 0};
const int MAXD = 3;
const int MAXTPLT = 10;
int maxSizeTplt = 0;
int group[MAXN][MAXN];
int SIZE[MAXN];
int sz[MASK(MAXTPLT)];

struct dpVal {
    bool a[MAXN][MAXN];
    int mi;
    dpVal(){
        mi = 0;
        memset(a, 0, sizeof a);
    }
};

dpVal dp[MASK(MAXTPLT)];

bool valid(int i, int j){
    return (i >= 1 && i <= n && j >= 1 && j <= m);
}

void ReadInput(){
    cin >> n >> m >> t;
    FOR(i, 1, n){
        FOR(j, 1, m){
            char ch; cin >> ch;
            a[i][j] = ch - '0';
        }
    }
    FOR(i, 1, t){
        cin >> query[i];
    }
}

namespace TachVungLienThong{

    bool vi[MAXN][MAXN];
    int cnt = 0;

    void dfs(int i, int j){
        vi[i][j] = true;
        group[i][j] = cnt;
        SIZE[cnt - 1]++;
        FOR(k, 0, MAXD){
            int ii = i + dy[k];
            int jj = j + dx[k];
            if (valid(ii, jj) && vi[ii][jj] == 0 && a[ii][jj] == 1){
                dfs(ii, jj);
            }
        }
    }

    void solve(){
        FOR(i, 1, n){
            FOR(j, 1, m){
                if (a[i][j] == 0 || vi[i][j] == 1) continue;
                cnt++;
                dfs(i, j);
            }
        }
        maxSizeTplt = cnt;
    }
}

namespace InitDpBitmask {

    dpVal process(int x){
        dpVal res = dpVal();
        FOR(i, 1, n){
            FOR(j, 1, m){
                if (group[i][j] == x) res.a[i][j] = 1;
            }
        }
        return res;
    }

    void solve(){
        assert(maxSizeTplt <= 10);
        FOR(mask, 0, MASK(maxSizeTplt) - 1){
            dp[mask].mi = 1e6;
        }
        FOR(i, 0, maxSizeTplt - 1){
            dp[MASK(i)] = process(i + 1);
        }
    }
}

namespace ProcessDpBitmask{

    int dist[MAXN][MAXN];
    pair<int, int> par[MAXN][MAXN];

    dpVal combine(const dpVal &a, const dpVal &b){
        dpVal res = dpVal();
        res.mi = a.mi + b.mi;
        memset(dist, -1, sizeof dist);
        deque<pair<int, int>> dq;
        FOR(i, 1, n){
            FOR(j, 1, m){
                if (a.a[i][j] == 1){
                    dist[i][j] = 0;
                    dq.push_back(make_pair(i, j));
                    res.a[i][j] = 1;
                }
                if (b.a[i][j] == 1) res.a[i][j] = 1;
            }
        }
        while (dq.size()){
            pair<int, int> tmp = dq.front();
            dq.pop_front();
            FOR(k, 0, MAXD){
                int ii = tmp.first + dy[k];
                int jj = tmp.second + dx[k];
                if (valid(ii, jj) && dist[ii][jj] == -1){
                    par[ii][jj] = make_pair(tmp.first, tmp.second);
                    dist[ii][jj] = dist[tmp.first][tmp.second] + (b.a[ii][jj] == 0);
                    if (b.a[ii][jj] == 0) dq.push_back(make_pair(ii, jj));
                        else dq.push_front(make_pair(ii, jj));
                }
            }
        }
        pair<int, int> target = make_pair(-1, -1);
        int mi = 1e6;
        FOR(i, 1, n){
            FOR(j, 1, m){
                if (b.a[i][j] == 1){
                    if (dist[i][j] < mi){
                        mi = dist[i][j];
                        target = make_pair(i, j);
                    }
                }
            }
        }
        res.mi += mi;
        assert(target != make_pair(-1, -1));
        while (a.a[target.first][target.second] == 0){
            res.a[target.first][target.second] = 1;
            target = make_pair(par[target.first][target.second].first, par[target.first][target.second].second);
        }
        return res;
    }

    void solve(){
        FOR(mask, 1, MASK(maxSizeTplt) - 1){
            if (__builtin_popcount(mask) < 2) continue;
            vector<int> bit;
            for (int i = mask; i > 0; i -= i & -i) bit.push_back(__builtin_ctz(i));
            for (int &i : bit){
                int prevMask = mask ^ MASK(i);
                dpVal Merge = combine(dp[prevMask], dp[MASK(i)]);
                if (dp[mask].mi > Merge.mi){
                    dp[mask] = Merge;
                }
            }
        }
    }
}

namespace ProcessFinalValue {

    int res[MAXN];
    int suffMin[MAXN * MAXN];

    void solve(){
        FOR(mask, 1, MASK(maxSizeTplt) - 1){
            int last_bit = __builtin_ctz(mask);
            int prevMask = mask ^ MASK(last_bit);
            sz[mask] = sz[prevMask] + SIZE[last_bit];
        }
        memset(suffMin, 0x3f, sizeof suffMin);
        FOR(mask, 1, MASK(maxSizeTplt) - 1){
            int id = sz[mask];
            suffMin[id] = min(suffMin[id], dp[mask].mi);
        }
        REV(i, n * m, 1) suffMin[i] = min(suffMin[i + 1], suffMin[i]);
        FOR(i, 1, t){
            int val = query[i];
            if (suffMin[val] > 1e6) cout << -1 << ' ';
                else cout << suffMin[val] << ' ';
        }
    }
}

#define task "COLAND"
int32_t main(){
    if (fopen(task".inp","r")){
        freopen(task".inp","r",stdin);
        freopen(task".out","w",stdout);
    }
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    ReadInput();
    TachVungLienThong::solve();
    InitDpBitmask::solve();
    ProcessDpBitmask::solve();
    ProcessFinalValue::solve();
    return 0;
}
