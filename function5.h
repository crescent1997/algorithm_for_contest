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

// rerooting, matrix_pow, trie tree, scc, suffixarray

// ABC348E
// ABCの解説コードの関数を元に解説AC
LL sum_c;
Graph g(MAX);
vector<LL> c(MAX);
vector<LL> subtree_sum_c(MAX);
vector<LL> subtree_sum_dist(MAX);
vector<LL> f(MAX);

pair<LL, LL> dfs(LL v, LL par){
    for(LL t : g[v]){
        if(t==par){
            continue;
        }
        pair<LL, LL> cd=dfs(t, v);
        subtree_sum_c[v]+=cd.first;
        subtree_sum_dist[v]+=cd.second;
    }
    subtree_sum_dist[v]+=subtree_sum_c[v];
    subtree_sum_c[v]+=c[v];
    return make_pair(subtree_sum_c[v], subtree_sum_dist[v]);
}

void rerooting(LL v, LL par, LL par_sum_c, LL par_sum_dist){
    f[v]=subtree_sum_dist[v]+par_sum_dist;
    for(LL t : g[v]){
        if(t==par){
            continue;
        }
        LL nc=par_sum_c+subtree_sum_c[v]-subtree_sum_c[t];
        LL nd=par_sum_dist+subtree_sum_dist[v]-subtree_sum_dist[t]-subtree_sum_c[t]+nc;
        cout << v << " " << t << " " << nc << " " << nd << endl;
        rerooting(t, v, nc, nd);
    }
    return;
}

//matrix pow
vector<vector<LL>> mod_matrixpow(vector<vector<LL>> a, LL n){
    LL sz=a.size();
    vector<vector<LL>> ans(sz, vector<LL>(sz, 0));
    vector<vector<LL>> cur_a=a;

    for(LL i=0; i<sz; i++){
        ans[i][i]=1;
    }
    while(n>0){
        vector<vector<LL>> tmp_ans(sz, vector<LL>(sz, 0));
        vector<vector<LL>> new_a(sz, vector<LL>(sz, 0));

        if((n&1)==1){
            for(LL i=0; i<sz; i++){
                for(LL j=0; j<sz; j++){
                    for(LL k=0; k<sz; k++){
                        tmp_ans[i][j]=(tmp_ans[i][j]+ans[i][k]*cur_a[k][j])%MOD;
                    }
                }
            }
            ans=tmp_ans;
        }
        for(LL i=0; i<sz; i++){
            for(LL j=0; j<sz; j++){
                for(LL k=0; k<sz; k++){
                    new_a[i][j]=(new_a[i][j]+cur_a[i][k]*cur_a[k][j])%MOD;
                }
            }
        }
        cur_a=new_a;
        n >>= 1;
    }
    return ans;
}

// trie tree ABC353E
// 始めて訪れた状態に対してnow_szで番号を割り当てる。ans+=cnt[now]はこの順番でその文字を踏んだ回数を足しているイメージかな。
// ABCの解説コード
struct trie{
    using ar = array<LL, 26>;
    vector<ar> pos;
    ar def;
    LL now_sz;
    LL ans;
    vector<LL> cnt;
    trie() = default;
    trie(LL len){
        pos.reserve(len+1);
        cnt.reserve(len+1);
        def.fill(-1);
        now_sz=0;
        ans=0;
        make_node();
    }
    LL make_node(){
        pos.push_back(def);
        cnt.push_back(0);
        return now_sz++;
    }
    void add(string &s){
        LL now=0;
        for(LL i=0; i<s.size(); i++){
            LL d=s[i]-'a';
            LL &nx=pos[now][d];
            if(nx==-1){
                nx=make_node();
            }
            now=nx;
            ans+=cnt[now]++;
        }
    }
};

// usage
LL n;
cin >> n;
trie tr(300000);
string s;
for(LL i=0; i<n; i++){
    string s;
    cin >> s;
    tr.add(s);
}

cout << tr.ans << endl;

// trie ABC403E
// ABCの解説コードをChatGPTを使ってC++に変換
class Trie{
    public:
        struct Node{
            vector<LL> child;
            bool f=false;
            set<LL> Z;
        
            Node() : child(26, -1){
                f=false;
            }
        };
        
        vector<Node> nodes;
        set<LL> Z;
        
        Trie(){
            nodes.push_back(Node());
        }
        
        void add(const string& s, LL i, bool is_Y){
            LL k=0;
            for(char ch : s){
                LL c=ch-'a';

                if(nodes[k].child[c]==-1){
                    nodes[k].child[c]=nodes.size();
                    nodes.push_back(Node());
                }
                if(is_Y){
                    nodes[k].Z.insert(i);
                    if(nodes[k].f){
                        Z.insert(i);
                    }
                }
                k=nodes[k].child[c];
            }
        
            if(is_Y){
                nodes[k].Z.insert(i);
                if(nodes[k].f){
                    Z.insert(i);
                }
            }else{
                nodes[k].f=true;
                Z.insert(nodes[k].Z.begin(), nodes[k].Z.end());
                nodes[k].Z.clear();
            }
        }
};

// 強連結成分分解
// けんちょんさんのブログ記事を元に実装
// https://drken1215.hatenablog.com/entry/2023/05/06/133900
struct SCC{
    using Edge=LL;
    using SGraph=vector<vector<Edge>>;

    // input
    SGraph G;
    SGraph rG;

    // result
    vector<vector<LL>> scc;
    vector<LL> cmp;
    SGraph dag;

    // constructor
    SCC(LL N) : G(N), rG(N) {}

    // add edge
    void addedge(LL u, LL v){
        G[u].push_back(v);
        rG[v].push_back(u);
    }

    // decomp
    vector<bool> seen;
    vector<LL> vs;
    vector<LL> rvs;
    void dfs(LL v){
        seen[v]=true;
        for(auto e : G[v]){
            if(!seen[e]){
                dfs(e);
            }
        }
        vs.push_back(v);
    }
    void rdfs(LL v, LL k){
        seen[v]=true;
        cmp[v]=k;
        for(auto e : rG[v]){
            if(!seen[e]){
                rdfs(e, k);
            }
        }
        rvs.push_back(v);
    }

    // reconstruct
    set<pair<LL, LL>> newEdges;
    void reconstruct(){
        LL N = (LL)G.size();
        LL dV = (LL)scc.size();
        dag.assign(dV, vector<Edge>());
        newEdges.clear();
        for(LL i=0; i<N; i++){
            LL u=cmp[i];
            for(auto e : G[i]){
                LL v=cmp[e];
                if(u==v){
                    continue;
                }
                if(!newEdges.count({u, v})){
                    dag[u].push_back(v);
                    newEdges.insert({u, v});
                }
            }
        }
    }

    // main
    void solve(){
        // first dfs
        LL N=(LL)G.size();
        seen.assign(N, false);
        vs.clear();
        for(LL v=0; v<N; v++){
            if(!seen[v]){
                dfs(v);
            }
        }

        // back dfs
        LL k=0;
        scc.clear();
        cmp.assign(N, -1);
        seen.assign(N, false);
        for(LL i=N-1; i>=0; i--){
            if(!seen[vs[i]]){
                rvs.clear();
                rdfs(vs[i], k);
                k++;
                scc.push_back(rvs);
            }
        }

        // reconstruct
        reconstruct();
    }
};

// usage ABC357E
    LL n;
    cin >> n;

    SCC g(n);
    vector<vector<LL>> ot(n);
    vector<LL> to(n);
    UnionFind uf(n);

    for(LL i=0; i<n; i++){
        LL a;
        cin >> a;
        a--;
        ot[a].push_back(i);
        to[i]=a;
        g.addedge(i, a);
    }

    queue<LL> q;
    vector<LL> dp(n, 1);

    g.solve();

    for(auto vs : g.scc){
        LL nv=vs.size();
        bool ok=(nv>=2 || to[vs[0]]==vs[0]);
        if(!ok){
            continue;
        }
        for(int v : vs){
            q.push(v);
            dp[v]=nv;
            uf.merge(v, vs[0]);
        }
    }

    while(!q.empty()){
        LL u=q.front();
        q.pop();
        for(LL v : ot[u]){
            if(uf.same(u, v)){
                continue;
            }
            dp[v]+=dp[u];
            q.push(v);
        }
    }

    LL ans=0;
    for(LL x : dp){
        ans+=x;
    }

    cout << ans << endl;

// suffix array
// けんちょんさんのブログのコードを使用
// https://drken1215.hatenablog.com/entry/2023/07/25/143000
template<class Str> struct SuffixArray {
    // data
    Str str;
    vector<int> sa;    // sa[i] : the starting index of the i-th smallest suffix (i = 0, 1, ..., n)
    vector<int> rank;  // rank[sa[i]] = i
    vector<int> lcp;   // lcp[i]: the lcp of sa[i] and sa[i+1] (i = 0, 1, ..., n-1)
    SparseTable<int> st;  // use for calcultating lcp(i, j)

    // getter
    int& operator [] (int i) {
        return sa[i];
    }
    vector<int> get_sa() { return sa; }
    vector<int> get_rank() { return rank; }
    vector<int> get_lcp() { return lcp; }

    // constructor
    SuffixArray() {}
    SuffixArray(const Str& str_) : str(str_) {
        build_sa();
    }
    void init(const Str& str_) {
        str = str_;
        build_sa();
    }
    void build_sa() {
        vector<int> s;
        unordered_map<int,int> dict;
        for (int i = 0; i < (int)str.size(); ++i) {
            if (!dict.count(str[i])) dict[str[i]] = dict.size();
        }
        for (int i = 0; i < (int)str.size(); ++i) {
            s.push_back(dict[str[i]] + 1);
        }
        s.push_back(0);
        sa = sa_is(s, dict.size());
        calcLCP(s);
        buildSparseTable();
    }

    // SA-IS
    // upper: # of characters
    vector<int> sa_is(vector<int> &s, long long upper = 100000) {
        int N = (int)s.size();
        if (N == 0) return {};
        else if (N == 1) return {0};
        else if (N == 2) {
            if (s[0] < s[1]) return {0, 1};
            else return {1, 0};
        }

        vector<int> isa(N);
        vector<bool> ls(N, false);
        for (int i = N - 2; i >= 0; --i) {
            ls[i] = (s[i] == s[i + 1]) ? ls[i + 1] : (s[i] < s[i + 1]);
        }
        vector<int> sum_l(upper + 1, 0), sum_s(upper + 1, 0);
        for (int i = 0; i < N; ++i) {
            if (!ls[i]) ++sum_s[s[i]];
            else ++sum_l[s[i] + 1];
        }
        for (int i = 0; i <= upper; ++i) {
            sum_s[i] += sum_l[i];
            if (i < upper) sum_l[i + 1] += sum_s[i];
        }

        auto induce = [&](const vector<int> &lms) -> void {
            fill(isa.begin(), isa.end(), -1);
            vector<int> buf(upper + 1);
            copy(sum_s.begin(), sum_s.end(), buf.begin());
            for (auto d: lms) {
                if (d == N) continue;
                isa[buf[s[d]]++] = d;
            }
            copy(sum_l.begin(), sum_l.end(), buf.begin());
            isa[buf[s[N - 1]]++] = N - 1;
            for (int i = 0; i < N; ++i) {
                int v = isa[i];
                if (v >= 1 && !ls[v - 1]) {
                    isa[buf[s[v - 1]]++] = v - 1;
                }
            }
            copy(sum_l.begin(), sum_l.end(), buf.begin());
            for (int i = N - 1; i >= 0; --i) {
                int v = isa[i];
                if (v >= 1 && ls[v - 1]) {
                    isa[--buf[s[v - 1] + 1]] = v - 1;
                }
            }
        };
            
        vector<int> lms, lms_map(N + 1, -1);
        int M = 0;
        for (int i = 1; i < N; ++i) {
            if (!ls[i - 1] && ls[i]) {
                lms_map[i] = M++;
            }
        }
        lms.reserve(M);
        for (int i = 1; i < N; ++i) {
            if (!ls[i - 1] && ls[i]) {
                lms.push_back(i);
            }
        }
        induce(lms);

        if (M) {
            vector<int> lms2;
            lms2.reserve(isa.size());
            for (auto v: isa) {
                if (lms_map[v] != -1) lms2.push_back(v);
            }
            int rec_upper = 0;
            vector<int> rec_s(M);
            rec_s[lms_map[lms2[0]]] = 0;
            for (int i = 1; i < M; ++i) {
                int l = lms2[i - 1], r = lms2[i];
                int nl = (lms_map[l] + 1 < M) ? lms[lms_map[l] + 1] : N;
                int nr = (lms_map[r] + 1 < M) ? lms[lms_map[r] + 1] : N;
                bool same = true;
                if (nl - l != nr - r) same = false;
                else {
                    while (l < nl) {
                        if (s[l] != s[r]) break;
                        ++l, ++r;
                    }
                    if (l == N || s[l] != s[r]) same = false;
                }
                if (!same) ++rec_upper;
                rec_s[lms_map[lms2[i]]] = rec_upper;
            }
            auto rec_sa = sa_is(rec_s, rec_upper);

            vector<int> sorted_lms(M);
            for (int i = 0; i < M; ++i) {
                sorted_lms[i] = lms[rec_sa[i]];
            }
            induce(sorted_lms);
        }
        return isa;
    }

    // find min id that str.substr(sa[id]) >= T
    int lower_bound(const Str& T) {
        int left = -1, right = sa.size();
        while (right - left > 1) {
            int mid = (left + right) / 2;
            if (str.compare(sa[mid], string::npos, T) < 0)
                left = mid;
            else
                right = mid;
        }
        return right;
    }

    // find min id that str.substr(sa[id], T.size()) > T
    int upper_bound(const Str& T) {
        int left = -1, right = sa.size();
        while (right - left > 1) {
            int mid = (left + right) / 2;
            if (str.compare(sa[mid], T.size(), T) <= 0)
                left = mid;
            else
                right = mid;
        }
        return right;
    }

    // search
    bool is_contain(const Str& T) {
        int lb = lower_bound(T);
        if (lb >= sa.size()) return false;
        return str.compare(sa[lb], T.size(), T) == 0;
    }

    // find lcp
    void calcLCP(const vector<int> &s) {
        int N = (int)s.size();
        rank.assign(N, 0), lcp.assign(N - 1, 0);
        for (int i = 0; i < N; ++i) rank[sa[i]] = i;
        int h = 0;
        for (int i = 0; i < N - 1; ++i) {
            int pi = sa[rank[i] - 1];
            if (h > 0) --h;
            for (; pi + h < N && i + h < N; ++h) {
                if (s[pi + h] != s[i + h]) break;
            }
            lcp[rank[i] - 1] = h;
        }
    }
    
    // build sparse table for calculating lcp
    void buildSparseTable() {
        st.init(lcp);
    }

    // calc lcp of str.sutstr(a) and str.substr(b)
    int getLCP(int a, int b) {
        return st.get(min(rank[a], rank[b]), max(rank[a], rank[b]));
    }

    // debug
    void dump() {
        for (int i = 0; i < sa.size(); ++i) {
            cout << i << ": " << sa[i] << ", " << str.substr(sa[i]) << endl;
        }
    }
};
