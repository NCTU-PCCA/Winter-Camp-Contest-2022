#include <bits/stdc++.h>
using namespace std;

namespace TwoSat
{
#define endl '\n'
#define pb emplace_back
#define all(x) x.begin(), x.end()
#define IOS ios::sync_with_stdio(false); cin.tie(nullptr)
    
    using vi  = vector <int>;
    using vb  = vector <bool>;
    using vvi  = vector <vi>;
     
    struct SCC {
        /* Graph */
        int n;
        vvi g, gt;
        vi ans;

        /* SCC & DAG */
        int comp;
        vi id;
        vvi gg, c;

        /* Helper */
        vb vis;
        vi order, in;

        void d0(int x) {
            vis[x] = 1;
            for (int y : gt[x])
                if (!vis[y])
                    d0(y);
            order.pb(x);
        }
        void d1(int x) {
            vis[x] = 1;
            id[x] = comp;
            c[comp].pb(x);
            for (int y : g[x])
                if (!vis[y])
                    d1(y);
        }
        void Kosaraju() {
            vis.assign(n, 0); order.clear();
            for (int i = 0; i < n; ++i) 
                if (!vis[i])
                    d0(i);

            vis.assign(n, 0); c.clear(); id.assign(n, 0); 
            reverse(all(order));
            for (int i : order) 
                if (!vis[i])
                    c.pb(vi{}), d1(i), ++comp;

            gg.assign(comp, vi{}); in.assign(comp, 0);
            for (int i = 0; i < n; ++i)
                for (int j : g[i])
                    if (id[i] != id[j])
                        gg[id[i]].pb(id[j]), ++in[id[j]];
        }

        /* Main */
        void init(int n_) {
            n = n_ << 1; comp = 0;
            g.assign(n, vi{}); gt.assign(n, vi{});
        }
        void add_edge(int i, int a, int j, int b) {
            i = (i << 1) | a;
            j = (j << 1) | b;
            g[i ^ 1].pb(j); gt[j].pb(i ^ 1);
            g[j ^ 1].pb(i); gt[i].pb(j ^ 1);
        }
        int solve() {
            Kosaraju();
            for (int i = 0; i < n; i += 2)
                if (id[i] == id[i | 1]) // Invalid
                    return 0;
            
            queue <int> q;
            stack <int> s;
            for (int i = 0; i < comp; ++i)
                if (!in[i])
                    q.push(i);
            
            while (!q.empty()) {
                int i = q.front(); q.pop();
                s.push(i);
                for (int j : gg[i]) {
                    --in[j];
                    if (!in[j])
                        q.push(j);
                }
            }

            ans.assign(n >> 1, 0); vis.assign(n >> 1, 0);
            while (!s.empty()) {
                int i = s.top(); s.pop();
                bool flag = 0;
                for (int x : c[i]) {
                    if (vis[x >> 1]) {
                        flag = 1;
                        break;
                    }
                }
                if (flag) continue;
                for (int x : c[i]) {
                    vis[x >> 1] = 1;
                    ans[x >> 1] = (x & 1);
                }
            }
            return 1;
        }
        vi getAns() {
            return ans;
        }
    };

#undef endl 
#undef pb 
#undef all 
#undef IOS 
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    struct way
    {
        int ei;
        
        int to;
        char color;
        bool important;
    };
    
    struct relation
    {
        bool bad;
        int from, to;
    };
    
    int n, m, p;
    cin >> n >> m >> p;
    
    string col_pref;
    cin >> col_pref;
    
    vector<vector<way>> adjEdge(n);
    for (int i = 0; i < m; i++)
    {
        int a, b;
        char c, x, y;
        
        cin >> a >> b >> c >> x >> y;
        --a, --b;
        
        adjEdge[a].push_back({i, b, c, x=='y'});
        adjEdge[b].push_back({i, a, c, y=='y'});
    }
    
    vector<relation> rels;
    for (int i = 0; i < p; i++)
    {
        string rel;
        int a, b;
        
        cin >> rel >> a >> b;
        --a, --b;
        
        rels.push_back({rel=="bad", a, b});
    }
    
    TwoSat::SCC agent;
    agent.init(m);
    
    auto addTrue = [&](int i)
    {
        agent.add_edge(i, 1, i, 1);
    };
    
    auto addXOR = [&](int i, int j)
    {
        agent.add_edge(i, 1, j, 1);
        agent.add_edge(i, 0, j, 0);
    };
    
    auto addNAND = [&](int i, int j)
    {
        agent.add_edge(i, 0, j, 0);
    };
    
    for (int i = 0; i < n; i++)
    {
        if (col_pref[i] != 'x')
        {
            for (way & w : adjEdge[i])
            {
                if (w.color == col_pref[i] && w.important)
                {
                    addTrue(w.ei);
                }
            }
        }
    }
    
    for (int i = 0; i < n; i++)
    {
        vector<way> & adj = adjEdge[i];
        for (auto it = adj.begin(); it != adj.end(); it++)
        {
            way & wa = *it;
            for (auto it2 = next(it); it2 != adj.end(); it2++)
            {
                way & wb = *it2;
                if (wa.color != wb.color)
                {
                    if (wa.important && wb.important)
                    {
                        addXOR(wa.ei, wb.ei);
                    }
                    else
                    {
                        addNAND(wa.ei, wb.ei);
                    }
                }
            }
        }
    }
    
    for (relation & rel : rels)
    {
        for (way & wa : adjEdge[rel.from])
        {
            for (way & wb : adjEdge[rel.to])
            {
                if ((wa.color != wb.color) ^ (rel.bad))
                {
                    addNAND(wa.ei, wb.ei);
                }
            }
        }
    }
    
    if (!agent.solve())
    {
        cout << "No\n";
    }
    else
    {
        cout << "Yes\n";
        
        for (int i = 0; i < n; i++)
        {
            for (way & w : adjEdge[i])
            {
                if (agent.ans[w.ei])
                {
                    cout << w.color;
                    break;
                }
            }
        }
        
        cout << "\n";
    }
}