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

// normal unionfind, weighted unionfind, kruskal

// normal unionfind
// けんちょんさんのブログ記事を元に実装
// https://drken1215.hatenablog.com/entry/2025/01/17/000600
struct UnionFind{
    // core member
    vector<LL> par;
    vector<LL> nex;

    // constructor
    UnionFind() {}
    UnionFind(LL N) : par(N, -1), nex(N){
        init(N);
    }
    void init(LL N){
        par.assign(N, -1);
        nex.resize(N);
        for(LL i=0; i<N; i++){
            nex[i]=i;
        }
    }
    
    // core methods
    LL root(LL x){
        if(par[x]<0){
            return x;
        }else{
            return par[x]=root(par[x]);
        }
    }
    
    bool same(LL x, LL y){
        return root(x)==root(y);
    }
    
    bool merge(LL x, LL y){
        x=root(x);
        y=root(y);
        if(x==y){
            return false;
        }
        if(par[x]>par[y]){
            swap(x, y); // merge technique
        }
        par[x]+=par[y];
        par[y]=x;
        swap(nex[x], nex[y]);
        return true;
    }
    
    LL size(LL x){
        return -par[root(x)];
    }
    
    // get group
    vector<LL> group(int x) {
        vector<LL> res({x});
        while(nex[res.back()]!=x){
            res.push_back(nex[res.back()]);
        }
        return res;
    }
    vector<vector<LL>> groups(){
        vector<vector<LL>> member(par.size());
        for(LL v=0; v <(LL)par.size(); v++){
            member[root(v)].push_back(v);
        }
        vector<vector<LL>> res;
        for(LL v=0; v<(LL)par.size(); v++){
            if(!member[v].empty()){
                res.push_back(member[v]);
            }
        }
        return res;
    }
    
    // debug
    friend ostream& operator << (ostream &s, UnionFind uf){
        const vector<vector<LL>> &gs = uf.groups();
        for(const vector<LL> &g : gs){
            s << "group: ";
            for (int v : g) s << v << " ";
            s << endl;
        }
        return s;
    }
};

// weighted unionfind
// けんちょんさんの記事のコードを使用
// https://drken1215.hatenablog.com/entry/2023/11/12/040700
template<class T> struct WeightedUnionFind {
    // core member
    vector<int> par;
    vector<T> weight;

    // constructor
    WeightedUnionFind() { }
    WeightedUnionFind(int N, T zero = 0) : par(N, -1), weight(N, zero) {}
    void init(int N, T zero = 0) {
        par.assign(N, -1);
        weight.assign(N, zero);
    }
    
    // core methods
    int root(int x) {
        if (par[x] < 0) return x;
        else {
            int r = root(par[x]);
            weight[x] += weight[par[x]];
            return par[x] = r;
        }
    }
    bool same(int x, int y) {
        return root(x) == root(y);
    }
    int size(int x) {
        return -par[root(x)];
    }
    
    // v[y] - v[x] = w
    bool merge(int x, int y, T w) {
        w += get_weight(x), w -= get_weight(y);
        x = root(x), y = root(y);
        if (x == y) return false;
        if (par[x] > par[y]) swap(x, y), w = -w; // merge technique
        par[x] += par[y];
        par[y] = x;
        weight[y] = w;
        return true;
    }
    
    // get v[x]
    T get_weight(int x) {
        root(x);
        return weight[x];
    }
    
    // get v[y] - v[x]
    T get_diff(int x, int y) {
        return get_weight(y) - get_weight(x);
    }
    
    // get groups
    vector<vector<int>> groups() {
        vector<vector<int>> member(par.size());
        for (int v = 0; v < (int)par.size(); ++v) {
            member[root(v)].push_back(v);
        }
        vector<vector<int>> res;
        for (int v = 0; v < (int)par.size(); ++v) {
            if (!member[v].empty()) res.push_back(member[v]);
        }
        return res;
    }
    
    // debug
    friend ostream& operator << (ostream &s, WeightedUnionFind uf) {
        const vector<vector<int>> &gs = uf.groups();
        for (const vector<int> &g : gs) {
            s << "group: ";
            for (int v : g) s << v << "(" << uf.get_weight(v) << ") ";
            s << endl;
        }
        return s;
    }
};

// kruskal
// 問題解決力を鍛える！アルゴリズムとデータ構造を参照
class Edge{
    public:
    LL s;
    LL t;
    LL c;
    Edge(LL s=0, LL t=0, LL c=0) : s(s), t(t), c(c) {}
    bool operator < (const Edge &e) const{
        return c < e.c;
    }
};

int main(){
    ...
    LL ans=0;

    sort(edges.begin(), edges.end());

    for(LL i=0; i<m; i++){
        Edge e=edges[i];
        if(!uf.same(e.s, e.t)){
            ans+=e.c;
            uf.merge(e.s, e.t);
        }
    }

    cout << ans << endl;
}
