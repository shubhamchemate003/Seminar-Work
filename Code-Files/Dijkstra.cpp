#include <bits/stdc++.h>

using namespace std;
using namespace chrono;

#define pii pair<int, int>
#define int long long

const int dx[] = {1, 0, -1, 0};
const int dy[] = {0, 1, 0, -1};
const int D = 1000;
const int inf = 1e10;

char grid[D][D];
int n, m, dist[D][D];

bool run_dijekstra(int ux, int uy, int vx, int vy) {
    priority_queue<pair<int, pii>, vector<pair<int, pii>>, greater<pair<int, pii>>> openlist;

    dist[ux][uy] = 0;
    openlist.push({0, {ux, uy}});

    while (!openlist.empty()) {
        int curx, cury;
        tie(curx, cury) = openlist.top().second;
        openlist.pop();

        if (curx == vx and cury == vy)
            return 1;

        for (int d = 0; d < 4; d++) {
            int nxtx = curx + dx[d];
            int nxty = cury + dy[d];

            if (nxtx<1 or nxtx > n)
                continue;
            if (nxty<1 or nxty > m)
                continue;

            if (grid[nxtx][nxty] != '.')
                continue;

            if (dist[nxtx][nxty] > dist[curx][cury] + 1) {
                dist[nxtx][nxty] = dist[curx][cury] + 1;
                openlist.push({dist[nxtx][nxty], {nxtx, nxty}});
            }
        }
    }

    return 0;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("../input1.txt", "r", stdin);
    freopen("../output1.txt", "w", stdout);
#endif


    cin >> n >> m;

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            cin >> grid[i][j];

    int ux, uy, vx, vy;
    cin >> ux >> uy;
    cin >> vx >> vy;

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            dist[i][j] = inf;

    if (grid[ux][uy] != '.') {
        cout << "Invalid Source\n";
        return 0;
    }
    if (grid[vx][vy] != '.') {
        cout << "Invalid Destination\n";
        return 0;
    }

    auto begin = std::chrono::high_resolution_clock::now();

    if (!run_dijekstra(ux, uy, vx, vy))
        cout << "Path not found-1\n";
    else cout << "Path found-1\n";

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << " ns" << std::endl;

    return 0;
}