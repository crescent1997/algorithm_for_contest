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

// safe_pow, abs, moddinv, gcd, extended_gcd, is_prime, eratos, factorize_gen, argsort, combination, mod_pow, mod_div

LL safe_pow(LL a, LL b){
    LL res=1;
    for(LL i=0; i<b; i++){
        LD dres=res;
        dres*=a;
        if(dres>2e18){
            return 2e18;
        }
        res*=a;
    }
    return res;
}

LL labs(LL x){
    if(x>0){
        return x;
    }else{
        return x*(-1);
    }
}

LL modinv(LL a) {
    LL b = MOD, u = 1, v = 0;
    while (b) {
        LL t = a / b;
        a -= t * b;
        swap(a, b);
        u -= t * v;
        swap(u, v);
    }
    u %= MOD;
    if (u < 0){
        u += MOD;
    }
    return u;
}

LL gcd(LL a, LL b){
    LL tmp; 
    if(a<b){
        tmp = a;
        a = b;
        b = tmp;
    }
    tmp = a%b;
    while(tmp!=0){
        a = b;
        b = tmp;
        tmp = a%b;
    }

    return b;
}

// extended gcd
pair<LL, LL> extgcd(LL a, LL b){
    if(b==0){
        return make_pair(1, 0);
    }
    LL x;
    LL y;
    [y, x] = extgcd(b, a%b);
    y-=(a/b)*x;
    return make_pair(x, y);
}

bool is_prime(LL x){
    bool ans=true;
    for(LL i=2; i*i<=x; i++){
        if(x%i==0){
            ans=false;
            break;
        }
    }

    return ans;
}

vector<pair<LL, LL>> factorize_gen(LL n){
    vector<pair<LL, LL>> out;
    for(LL i=2; i*i<=n; i++){
        if(n%i!=0){
            continue;
        }
        LL num=0;
        while(n%i==0){
            num++;
            n/=i;
        }
        out.push_back(make_pair(i, num));
    }
    if(n!=1){
        out.push_back(make_pair(n, 1));
    }

    return out;
}

// eratos
vector<LL> isprime(MAX, 1);

void eratos(LL n){
    isprime[0]=0;
    isprime[1]=0;

    for(LL i=2; i*i<=n; i++){
        if(isprime[i]==1){
            LL j=i+i;
            while(j<=n){
                isprime[j]=0;
                j+=i;
            }
        }
    }

    return;
}

//argsort sample
    cin >> n;
    vector<size_t> indices(n);
    for(LL i=0; i<n; i++){
        cin >> tmp;
        p.push_back(tmp);
    }
    iota(indices.begin(), indices.end(), 0);

    sort(indices.begin(), indices.end(), [&p](size_t i1, size_t i2) {
        return p[i1] < p[i2];
    });

struct shop{
    LL a;
    LL b;

    shop(LL input_a, LL input_b)
    {
        a = input_a;
        b = input_b;
    }
};

bool cmp(const shop &x, const shop &y)
{
    return x.a < y.a;
}

ab.push_back(shop(tmp_a, tmp_b));
sort(ab.begin(), ab.end(), cmp);

// how to sort a(m) with different b(n) (m<=n)
sort(a[i].begin(), a[i].end(), [&](auto const& lhs, auto const& rhs){
    return b[lhs] > b[rhs];
});

// for combination list
vector<vector<LL>> v(n+1,vector<LL>(n+1,0));
for(LL i=0; i<v.size(); i++){
    v[i][0]=1;
    v[i][i]=1;
}
for(LL j=1; j<v.size(); j++){
    for(LL k=1; k<j; k++){
        v[j][k]=(v[j-1][k-1]+v[j-1][k]);
    }
}

// for one combination value
// けんちょんさんのブログを参考に作成
// https://drken1215.hatenablog.com/entry/2018/06/08/210000
vector<LL> fac(MAX), finv(MAX), inv(MAX);
void COMinit(){
    fac[0]=1;
    fac[1]=1;
    finv[0]=1;
    finv[1]=1;
    inv[1]=1;
    for(LL i=2; i<MAX; i++){
        fac[i]=fac[i-1]*i%MOD;
        inv[i]=MOD-inv[MOD%i]*(MOD/i)%MOD;
        finv[i]=finv[i-1]*inv[i]%MOD;
    }
}
 
LL COM(LL n, LL k){
    if(n<k){
        return 0;
    }
    if(n<0 || k<0){
        return 0;
    }
    return fac[n]*(finv[k]*finv[n-k]%MOD)%MOD;
}

// for one combination value fermi (takes time)
LL mod_pow(LL x, LL n){
    LL ans = 1;
    while(n>0){
        if((n&1)==1){
            ans=(ans*x)%MOD;
        }
        x=x*x%MOD;
        n>>=1;
    }
    return ans;
}

LL Div(LL a, LL b){
    return (a*mod_pow(b, MOD-2))%MOD;
}

LL mod_pow(LL x, LL n, LL p){
    x%=p;
    if(x==0){
        return 0;
    }

    LL ans=1;
    while(n>0){
        if((n&1)==1){
            ans=(ans*x)%p;
        }
        x=x*x%p;
        n>>=1;
    }

    return ans;
}

vector<LL> fac(MAX), finv(MAX);
void COMinit(){
    fac[0]=1;
    finv[0]=1;
    for (LL i=1; i<MAX; i++){
        fac[i]=(fac[i-1]*i)%MOD;
        finv[i]=Div(1, fac[i])%MOD;
    }
}
 
LL COM(LL n, LL k){
    if(n<k){
        return 0;
    }
    if(n<0 || k<0){
        return 0;
    }
    return (fac[n]*((finv[k]*finv[n-k])%MOD))%MOD;
}

LL comb_s(LL n, LL r){
    LL tmp_n=1;
    LL tmp_d=1;
    LL ans;
    if(r==0){
        return 1;
    }
    for(LL i=1; i<=r; i++){
        tmp_n=(tmp_n*(n+1-i))%MOD;
        tmp_d=(tmp_d*i)%MOD;
        ans=tmp_n*mod_pow(tmp_d,MOD-2);
    }

    ans%=MOD;

    return ans;    
}

// for low k
LL k=5010;
vector<vector<LL>> binom(k, vector<LL>(k, 0));
binom[0][0]=1;

for(LL i=1; i<k; i++){
    binom[i][0]=1;
    for(LL j=1; j<=i; j++){
        binom[i][j]=(binom[i-1][j-1]+binom[i-1][j])%m;
    }
}
