#include <bits/stdc++.h>
#define int long long
#define FOR(i, a, b) for (int i = (a), _b = (b); i <= _b; ++i)
#define REV(i, a, b) for (int i = (a), _b = (b); i >= _b; --i)
using namespace std;

int n, k, MOD;
#define MAXN 100005
int dp[MAXN];

void add(int &a, const int &b){
    a += b;
    if (a >= MOD) a -= MOD;
}

void addDP(int x){
    for (int i = x; i <= n; ++i) add(dp[i], dp[i - x]);
}

void solve(){
    cin >> n >> k >> MOD;
    dp[0] = 1;
    addDP(1); addDP(2);
    int prev = k + 2;
    int sum = (1) + (k + 2);
    int res = 0;
    int cnt = 2;
    while (sum <= n){
        res = (res + dp[n - sum]) % MOD;
        sum = sum + (prev + k + 1);
        prev += k + 1;
        addDP(++cnt);
    }
    cout << res;
}

#define task "CANDY"
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
