#include<iostream>
#include<cmath>
using namespace std;

using ll = long long;

ll isSquare(ll x) {
    ll sx = round(sqrt(x));
    if (sx * sx == x)
        return sx;
    return -1;
}

ll distance(ll x0, ll y0, ll x1, ll y1) {
    ll dx = x0 - x1;
    ll dy = y0 - y1;
    return dx * dx + dy * dy;
}

int solve() {
    ll xA, yA, xB, yB, k;
    cin >> xA >> yA;
    cin >> xB >> yB;
    cin >> k;

    // Simplify the problem
    ll x = xB - xA;
    ll y = yB - yA;

    // Trivial Case
    if (x == 0 && y == 0) {
        int cnt = 2;
        for (ll i = -k + 1; i < k; ++i) {
            ll js = k * k - i * i;
            ll j = isSquare(js);
            if (j == -1) continue;
            cnt += 2;
        }
        return cnt;
    }

    // check the radius
    ll dis = x * x + y * y;
    ll chk = 4 * k * k;

    if (dis > chk)
        return 0;

    if (dis == chk)
        return x % 2 == 0 && y % 2 == 0;

    int ans = 0;
    for (ll i = -k; i <= k; ++i) {

        ll js = k * k - i * i;
        ll j = isSquare(js);

        if (j == -1) continue;

        ll d0 = distance(i, j, x, y);
        ll d1 = distance(i, -j, x, y);

        if (d0 == k * k || d1 == k * k) {
            ans = 2;
            break;
        }
    }

    return ans;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int t;
    cin >> t;
    for (; t--; )
        cout << solve() << '\n';
}