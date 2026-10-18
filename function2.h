#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
//#include <math.h>
//#include <queue>
//#include <stack>
//#include <iomanip>
 
// sometimes used
//#include <set>
//#include <map>
//#include <numeric>
//#include <list>
//#include <deque>
//#include <unordered_map>
//#include <ctime>
//#include <random>
//#include <cassert>
//#include <functional>
//#include <array>
 
typedef long long LL;
//typedef unsigned long long ULL;
//typedef long double LD;
using namespace std;
//using Graph = vector<vector<LL>>;
//using Edge = pair<LL, pair<LL, LL>>;
//typedef pair<LL, LL> P;

//const LL MOD=1000000007;
//const LL MOD=998244353;
//const LL MAX=200100;
//const LL NIL=-1;
//const LL INF=2000000000000000000;
//const LL INF=1000000;
//const LL SEG_VAL = 1 << 19;

// floyd, floyd_new_load, dijkstra, compression

vector<vector<LL>> p(n, vector<LL>(n, INF));

void floyd(LL n){
    for(LL k=0; k<n; k++){
        for(LL i=0; i<n; i++){
            for(LL j=0; j<n; j++){
                p[i][j]=min(p[i][j], p[i][k]+p[k][j]);
            }
        }
    }
}

void floyd_new_load(LL n, LL a, LL b, LL c){
    p[a][b]=min(p[a][b], c);
    p[b][a]=min(p[b][a], c);
    for(LL i=0; i<n; i++){
        for(LL j=0; j<n; j++){
            p[i][j]=min(p[i][j], p[i][a]+p[a][b]+p[b][j]);
        }
    }
    
    for(LL i=0; i<n; i++){
        for(LL j=0; j<n; j++){
            p[i][j]=min(p[i][j], p[i][b]+p[b][a]+p[a][j]);
        }
    }
}

// dijkstra
//　「プログラミングコンテスト攻略のためのアルゴリズムとデータ構造」を参考に実装
vector<vector<pair<LL, LL>>> adj(MAX);
vector<LL> color(MAX);
vector<LL> d(MAX);
LL n;

void dijkstra(){
    priority_queue<pair<LL, LL>, vector<pair<LL, LL>>, greater<pair<LL, LL>>> pq;
    for(LL i=0; i<n; i++){
        color[i]=0;
        d[i]=INF;
    }

    d[0]=0;
    pq.push(make_pair(0, 0));
    while(!pq.empty()){
        pair<LL, LL> f=pq.top();
        pq.pop();
        LL u=f.second;
        color[u]=1;
        if(d[u]<f.first){
            continue;
        }
        LL v;
        for(LL i=0; i<adj[u].size(); i++){
            v=adj[u][i].first;
            if(color[v]==1){
                continue;
            }
            if(d[v]>d[u]+adj[u][i].second){
                d[v]=d[u]+adj[u][i].second;
                pq.push(make_pair(d[v], v));
            }
        }
    }
}

// compression, zaatsu (slow)
LL comp(vector<LL> &a){
    set<LL> s(a.begin(),a.end());
    map<LL,LL> d;
    LL cnt=0;
    for(auto x: s){
        d[x]=cnt;
        cnt++;
    }
    for(auto &x: a){
        x=d[x];
    }

    return cnt;
}

//compression, zaatsu (fast)
// けんちょんさんのブログを参考に実装
// https://drken1215.hatenablog.com/entry/2021/08/09/235400
vector<LL> comp(vector<LL> a){
    vector<LL> b=a;
    sort(b.begin(), b.end());
    b.erase(unique(b.begin(), b.end()), b.end());

    vector<LL> res(a.size());
    for (LL i=0; i<a.size(); i++){
        res[i]=lower_bound(b.begin(), b.end(), a[i])-b.begin();
    }

    return res;
}