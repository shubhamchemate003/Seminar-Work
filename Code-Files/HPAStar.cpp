#include <bits/stdc++.h>

using namespace std;
using namespace chrono;

#define pii pair<int, int>
#define x first
#define y second
#define int long long

const int dx[] = {1, 0, -1, 0};
const int dy[] = {0, 1, 0, -1};
const int D = 1000, K = 4;
const int inf = 1e10;

char grid[D][D];
int n, m, dist[D][D];
bool abstract_node[D][D]; // nodes of abstract graph
map<pii, set<pair<pii, int>>> abstract_graph; // abstract graph

void print_grid(int n, int m) {
    // for (int i = 1; i <= n; i++) {
    //     for (int j = 1; j <= m; j++)
    //         cout << grid[i][j] << ' ';
    //     cout << '\n';
    // }

    int an = (n + K - 1) / K, am = (m + K - 1) / K;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            cout << abstract_node[i][j];
            if (j % am == 0) cout << ' ';
        }
        if (i % an == 0) cout << '\n';
        cout << '\n';
    }
}

// distance to all nodes in the cluster from source S
void DistanceToAllNodes(pii S, int t, int b, int l, int r) {
    // using Dijkstra's single source shortest path algorithm
    for (int i = t; i <= b; i++)
        for (int j = l; j <= r; j++)
            dist[i][j] = inf;

    dist[S.x][S.y] = 0;
    priority_queue<pair<int, pii>, vector<pair<int, pii>>, greater<pair<int, pii>>> openlist;
    openlist.push({0, {S.x, S.y}});

    while (!openlist.empty()) {
        int curx, cury;
        tie(curx, cury) = openlist.top().second;
        openlist.pop();

        for (int d = 0; d < 4; d++) {
            int nxtx = curx + dx[d];
            int nxty = cury + dy[d];

            if (nxtx<t or nxtx > b)
                continue;
            if (nxty<l or nxty > r)
                continue;

            if (grid[nxtx][nxty] != '.')
                continue;

            if (dist[nxtx][nxty] > dist[curx][cury] + 1) {
                dist[nxtx][nxty] = dist[curx][cury] + 1;
                openlist.push({dist[nxtx][nxty], {nxtx, nxty}});
            }
        }
    }
}

void build_abstract() {
    int an = (n + K - 1) / K, am = (m + K - 1) / K;

    for (int i = 1; i <= n; i += an) {
        for (int j = 1; j <= m; j += am) {
            int t = i, b = min(i + an - 1, n);
            int l = j, r = min(j + am - 1, m);

            for (int x = t; x <= b; x++)
                if (grid[x][r] == '.' and r + 1 <= m and grid[x][r + 1] == '.')
                    abstract_node[x][r] = abstract_node[x][r + 1] = 1;

            for (int y = l; y <= r; y++)
                if (grid[b][y] == '.' and b + 1 <= n and grid[b + 1][y] == '.')
                    abstract_node[b][y] = abstract_node[b + 1][y] = 1;

            for (int x = t; x <= b; x++) {
                if (abstract_node[x][r] == 0 or x == t or x == b)
                    continue;

                int prevx = x - 1, nxtx = x + 1, cnt = 0;
                if (prevx >= 1 and (grid[prevx][r] == '.' and grid[prevx][r + 1] == '.'))
                    cnt += 2;
                if (nxtx <= n and (grid[nxtx][r] == '.' and grid[nxtx][r + 1] == '.'))
                    cnt += 2;

                if (cnt == 4)
                    abstract_node[x][r] = abstract_node[x][r + 1] = 0;
            }

            for (int y = l; y <= r; y++) {
                if (abstract_node[b][y] == 0 or y == l or y == r)
                    continue;

                int prevy = y - 1, nxty = y + 1, cnt = 0;
                if (prevy >= 1 and (grid[b][prevy] == '.' and grid[b][nxty] == '.'))
                    cnt += 2;
                if (nxty <= m and (grid[b + 1][prevy] == '.' and grid[b + 1][nxty] == '.'))
                    cnt += 2;

                if (cnt == 4)
                    abstract_node[b][y] = abstract_node[b + 1][y] = 0;
            }
        }
    }

    // go in each cluster and create intra-edges
    for (int i = 1; i <= n; i += an) {
        for (int j = 1; j <= m; j += am) {
            int t = i, b = min(i + an - 1, n);
            int l = j, r = min(j + am - 1, m);

            set<pii> abs_nodes;
            for (int x = t; x <= b; x++) {
                if (abstract_node[x][l])
                    abs_nodes.insert({x, l});
                if (abstract_node[x][r])
                    abs_nodes.insert({x, r});
            }
            for (int y = l; y <= r; y++) {
                if (abstract_node[t][y])
                    abs_nodes.insert({t, y});
                if (abstract_node[b][y])
                    abs_nodes.insert({b, y});
            }

            vector<pii> abstract_nodes;
            for (auto node : abs_nodes)
                abstract_nodes.push_back(node);

            int abstract_nodes_count = abstract_nodes.size();
            for (int p = 0; p < abstract_nodes_count; p++) {
                pii u = abstract_nodes[p];
                DistanceToAllNodes(u, t, b, l, r);
                for (int q = 0; q < abstract_nodes_count; q++) {
                    pii v = abstract_nodes[q];
                    if (u != v and dist[v.x][v.y] != inf)
                        abstract_graph[u].insert({v, dist[v.x][v.y]});
                }
            }
        }
    }

    // create inter-edges
    for (int i = 1; i <= n; i += an) {
        for (int j = 1; j <= m; j += am) {
            int t = i, b = min(i + an - 1, n);
            int l = j, r = min(j + am - 1, m);

            for (int x = t; x <= b; x++) {
                if (abstract_node[x][r] and r + 1 <= m and abstract_node[x][r + 1]) {
                    abstract_graph[ {x, r}].insert({{x, r + 1}, 1});
                    abstract_graph[ {x, r + 1}].insert({{x, r}, 1});
                }
            }

            for (int y = l; y <= r; y++) {
                if (abstract_node[b][y] and b + 1 <= n and abstract_node[b + 1][y]) {
                    abstract_graph[ {b, y}].insert({{b + 1, y}, 1});
                    abstract_graph[ {b + 1, y}].insert({{b, y}, 1});
                }
            }
        }
    }

    // abstract graph formation -- done!
}

// connect node to abstract graph
void connect_to_abstract(pii node) {
    // find cluster of current node
    int an = (n + K - 1) / K, am = (m + K - 1) / K;
    int l = (node.y) / am * am + 1, r = (node.y + am - 1) / am * am;
    int t = (node.x) / an * an + 1, b = (node.x + an - 1) / an * an;
    r = min(r, m), b = min(b, n);

    set<pii> abs_nodes;
    for (int x = t; x <= b; x++) {
        if (abstract_node[x][l])
            abs_nodes.insert({x, l});
        if (abstract_node[x][r])
            abs_nodes.insert({x, r});
    }
    for (int y = l; y <= r; y++) {
        if (abstract_node[t][y])
            abs_nodes.insert({t, y});
        if (abstract_node[b][y])
            abs_nodes.insert({b, y});
    }

    vector<pii> abstract_nodes;
    for (auto node : abs_nodes)
        abstract_nodes.push_back(node);

    pii u = node;
    int abstract_nodes_count = abstract_nodes.size();
    DistanceToAllNodes(u, t, b, l, r);
    for (int q = 0; q < abstract_nodes_count; q++) {
        pii v = abstract_nodes[q];
        if (u != v and dist[v.x][v.y] != inf) {
            abstract_graph[u].insert({v, dist[v.x][v.y]});
            abstract_graph[v].insert({u, {dist[v.x][v.y]}});
        }
    }
}

int getHeuristic(int x, int y, int dx, int dy) {
    return abs(x - dx) + abs(y - dy);
}

// run HPAStar on abstract graph
bool run_HPAStar(pii S, pii D) {
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            dist[i][j] = inf;

    priority_queue<pair<int, pii>, vector<pair<int, pii>>, greater<pair<int, pii>>> openlist;

    dist[S.x][S.y] = 0;
    openlist.push({0, {S.x, S.y}});

    while (!openlist.empty()) {
        pii cur = openlist.top().y;
        openlist.pop();

        if (cur == D)
            return 1;

        for (auto nxt : abstract_graph[cur]) {
            int newcost = dist[cur.x][cur.y] + nxt.y;
            if (newcost < dist[nxt.x.x][nxt.x.y]) {
                dist[nxt.x.x][nxt.x.y] = newcost;
                int heu = getHeuristic(nxt.x.x, nxt.x.y, D.x, D.y);
                openlist.push({heu, {nxt.x.x, nxt.x.y}});
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

    pii S, D;
    cin >> S.x >> S.y;
    cin >> D.x >> D.y;

    build_abstract();

    connect_to_abstract(S);
    connect_to_abstract(D);

    auto begin = std::chrono::high_resolution_clock::now();

    if (run_HPAStar(S, D))
        cout << "Path found-3\n";
    else cout << "Path not found-3\n";

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << " ns" << std::endl;

    return 0;
}