#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>

using namespace std;

typedef pair<long long, long long> node;

int N;
bool DEBUG = true;
vector<node> pts_xy, pts_yx;
map<node, vector<node>> neighbour;
map<node, bool> visited;
map<node, long long> d;
node first, last;

void parse_entry() {
    cin >> N;
    long long x, y;
    scanf("%lld%lld", &x, &y);
    pts_xy.push_back(make_pair(x, y));
    pts_yx.push_back(make_pair(y, x));
    first = make_pair(x, y);
    for (int i = 1; i < N - 1; i++) {
        scanf("%lld%lld", &x, &y);
        pts_xy.push_back(make_pair(x, y));
        pts_yx.push_back(make_pair(y, x));
    }
    scanf("%lld%lld", &x, &y);
    pts_xy.push_back(make_pair(x, y));
    pts_yx.push_back(make_pair(y, x));
    last = make_pair(x, y);
}

void sort_pts() {
    sort(pts_xy.begin(), pts_xy.end());
    sort(pts_yx.begin(), pts_yx.end());
}

void create_edge (node v1, node v2) {
    neighbour.at(v1).push_back(v2);
    neighbour.at(v2).push_back(v1);
}

node reverse_node(node v) {
    return make_pair(v.second, v.first);
}

void create_graph() {
    neighbour.emplace(make_pair(*pts_xy.begin(), vector<node>()));
    visited.emplace(make_pair(*pts_xy.begin(), false));
    d.emplace(make_pair(*pts_xy.begin(), INT32_MAX));
    for (vector<node>::iterator v = pts_xy.begin(); v < pts_xy.end() - 1; v++) {
        neighbour.emplace(make_pair(*(v++), vector<node>()));
        create_edge(*v, *(v++));
        visited.emplace(make_pair(*(v++), false));
        d.emplace(make_pair(*(v++), INT32_MAX));
    }

    for (vector<node>::iterator v = pts_yx.begin(); v < pts_yx.end() - 1; v++) {
        create_edge(reverse_node(*v), reverse_node(*(v++)));
    }
}

long long road(node v, node u) {
    return min(abs(v.first - u.first), abs(v.second - u.second));
}

void dijkstra() {
    if (DEBUG) {
        cout << "Dijkstra start.\n";
    }
    d[first] = 0;
    priority_queue<pair<long long, node>> Q;
    Q.push(make_pair(0, first));
    while (!Q.empty()) {
        node u = Q.top().second;
        Q.pop();
        if (u == last) {
            break;
        }
        visited[u] = true;
        if (DEBUG) {
            cout << "Visited (" << u.first << ", " << u.second << ") = true\n";
        }
        for (auto v: neighbour[u]) {
            if (!visited[v]) {
                if (d[v] > d[u] + road(u, v)) {
                    d[v] = d[u] + road(u, v);
                    Q.push(make_pair(d[v], v));
                    if (DEBUG) {
                        cout << "Pushing on Q (" << v.first << ", " << v.second
                             << ")" << endl;
                    }
                }
            }
        }

    }
}

int main() {
    parse_entry();
    cout << INT32_MAX << endl;
    sort_pts();
    create_graph();
    dijkstra();
    cout << d[last];
    return 0;
}
