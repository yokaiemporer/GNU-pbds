#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
 
using namespace std;
using namespace __gnu_pbds;
 
#define fi first
#define se second
#define mp make_pair
#define pb push_back
#define fbo find_by_order
#define ook order_of_key
 
typedef long long ll;
typedef pair<int,int> ii;
typedef vector<ll> vi;
typedef long double ld; 
typedef tree<ll, null_type, less<ll>, rb_tree_tag, tree_order_statistics_node_update> pbds;

const int N = 150000; //number of problems
const int M = 150000; //number of students

int L[N+10];
int R[N+10];
int a[N+10]; //array of fixed score
ll T; 
int n,m; 

void read()
{
	cin>>n>>m;
	for(int i=0;i<n;i++) //problem score
	{
		cin>>L[i]>>R[i];
	}
	for(int i=0;i<m;i++) a[i]=-1;
	int q; cin>>q;
	for(int i=0;i<q;i++)
	{
		int rk, sc; cin>>rk>>sc;
		a[m-rk] = sc;
	}
	cin>>T; //total score	
}

vector<ll> sortedL,sortedR;

struct ConvexHull 
{
    struct Line 
    {
        ll m, c;

        Line (ll _m, ll _c) : m(_m), c(_c) {}

        ll pass(ll x) {
            return m * x + c;
        }
    };
    deque<Line> d;
    bool irrelevant(Line Z) 
    {
        if (int(d.size()) < 2) return false;
    
        Line X = d[int(d.size())-2], Y = d[int(d.size())-1];

        return (X.c - Z.c) * (Y.m - X.m) <= (X.c - Y.c) * (Z.m - X.m);
    }
    void push_line(ll m, ll c) 
    {
        Line l = Line(m,c);
        while (irrelevant(l)) d.pop_back();
        d.push_back(l);
    }
    ll query(ll x) {
        while (int(d.size()) > 1 && (d[0].c - d[1].c <= x * (d[1].m - d[0].m))) d.pop_front();
        return d.front().pass(x);
    }
};

bool check_naive(vector<int> b) //check if your assignment is valid
{
	ll sumB = 0;
	ll sumL = 0;
	sort(b.begin(),b.end());
	for(int i=0;i<b.size();i++)
	{
		sumB+=b[i];
		if(a[i]!=-1) assert(a[i]==b[i]);
		if(i>0) assert(b[i]>=b[i-1]);
	}
	for(int i=0;i<n;i++) sumL+=L[i];
	assert(int(b.size())==m&&sumB==T);
	ll cursum=0;
	for(int i=0;i<=m;i++)
	{
		for(int j=0;j<=n;j++)
		{
			ll s1 = sortedR[j]+cursum+(n-j)*1LL*(m-i);
			ll s2 = sortedL[j]+cursum+(n-j)*1LL*(m-i);
			if(s1<sumB||s2<sumL) return false;
		}
		if(i<m) cursum+=b[i];
	}
	return true;
}

bool check(vector<int> b) //check if your assignment is valid
{
	ll sumB = 0;
	ll sumL = 0;
	sort(b.begin(),b.end());
	for(int i=0;i<b.size();i++)
	{
		sumB+=b[i];
		if(a[i]!=-1) assert(a[i]==b[i]);
		if(i>0) assert(b[i]>=b[i-1]);
	}
	for(int i=0;i<n;i++) sumL+=L[i];
	assert(int(b.size())==m&&sumB==T);
	ll cursum=0;
	ConvexHull ch1,ch2;
	for(int j=n;j>=0;j--)
	{
		ch1.push_line(n-j,-sortedR[j]+j*1LL*m);
		ch2.push_line(n-j,-sortedL[j]+j*1LL*m);
	}
	for(int i=0;i<=m;i++)
	{
		ll v1 = -ch1.query(i);
		ll v2 = -ch2.query(i);
		if(v1<sumB-(cursum+n*1LL*m)||v2<sumL-(cursum+n*1LL*m)) return false;
		if(i<m) cursum+=b[i];
	}
	return true;
}

void greedyrange(vector<int> &v, int l, int r, int ub, ll &S) 
{
	if(S<=0) return ;
	ll ext = 0;
	for(int i=l;i<=r;i++)
	{
		ext+=ub-v[i];
	}
	if(ext<=S)
	{
		S-=ext;
		for(int i=l;i<=r;i++)
		{
			v[i]=ub;
		}
		return ;
	}
	deque<ii> dq; 
	for(int i=l;i<=r;i++) 
	{
		if(!dq.empty()&&dq.back().fi==v[i])
		{
			dq.back().se++;
		}
		else
		{
			dq.pb({v[i],1});
		}
	}
	while(S>0&&dq.size()>1)
	{
		int L = dq[0].fi; int cnt = dq[0].se;
		int R = dq[1].fi;
		//I have (R-L)*cnt before absolute merge
		if((R-L)*1LL*cnt<=S)
		{
			S-=(R-L)*1LL*cnt;
			dq[1].se+=cnt;
			dq.pop_front();
			continue;
		}
		//not enough space liao
		ll q = S/cnt;
		ll rem = S%cnt;
		dq[0].fi+=q;
		if(rem>0)
		{
			ii tmp = dq.front();
			dq.pop_front();
			dq.push_front({rem,tmp.fi+1});
			dq.push_front({cnt-rem,tmp.fi});
		}
		S=0;
		break;
	}
	//S>0
	if(S>0)
	{
		assert(int(dq.size())==1);
		ll q = S/(r-l+1);
		ll rem = S%(r-l+1);
		for(int i=l;i<=r;i++)
		{
			v[i]=dq[0].fi+q;
		}
		int ptr=r;
		for(int i=0;i<rem;i++)
		{
			v[ptr--]++;
		}
		S=0;
	}
	else
	{
		int ptr=l;
		for(ii x:dq)
		{
			for(int j=0;j<x.se;j++) v[ptr++]=x.fi;
		}
	}
}

void greedy(vector<int> &v, ll &S)
{
	if(S<=0) return ;
	vi ans;
	vector<ii> ranges;
	int l=0;
	for(int i=0;i<m;i++)
	{
		if(a[i]==-1) continue;
		if(l<=i-1)
		{
			ranges.pb({l,i-1});
		}
		l=i+1;
	}
	if(l<m) ranges.pb({l,m-1});
	for(ii x:ranges)
	{
		int r=x.se;
		int ub = n;
		if(r+1<m&&a[r+1]!=-1) ub=a[r+1];
		greedyrange(v,x.fi,x.se,ub,S);
	}
}

ii solve_full()
{
	sortedL.clear(); sortedR.clear();
	sortedL.pb(0); sortedR.pb(0);
	for(int i=0;i<n;i++)
	{
		sortedL.pb(L[i]);
		sortedR.pb(R[i]);
	}
	sort(sortedL.begin(),sortedL.end());
	sort(sortedR.begin(),sortedR.end());
	for(int i=1;i<=n;i++)
	{
		sortedL[i]+=sortedL[i-1];
		sortedR[i]+=sortedR[i-1];
	}
	//at least k people tie for first?
	int lo = 1; int hi = m; 
	int anstie = -1;
	int ansm = 0;
	vector<int> testb;
	vi ori(m,-1);
	while(lo<=hi)
	{
		int mid=(lo+hi)>>1;
		vector<int> b;
		int curmin=0;
		ll cursum=0;
		for(int i=0;i<m;i++)
		{
			if(a[i]!=-1) curmin=a[i];
			b.pb(curmin);
			cursum+=b[i];
		}
		//left T - cursum stuff to add :(
		//fix the maximum M
		bool pos=0;
		int forcedM=-1;
		for(int j=m-mid;j<m;j++)
		{
			if(a[j]>=0)
			{
				if(forcedM>=0&&forcedM!=a[j]) forcedM=-2;
				forcedM = a[j];
			}
		}
		if(forcedM>=-1)
		{
			int L2 = curmin; int R2 = n;
			if(forcedM>=0) L2=R2=forcedM;
			//otherwise L2 is the smallest d+curmin such that there EXIST a good covering
			if(forcedM<0)
			{
				int lo2 = curmin; int hi2 = max(0LL,min(ll(n),(T-cursum)/mid+curmin)); //add to everyone i guess
				L2=int(1e9);
				while(lo2<=hi2)
				{
					int mid2=(lo2+hi2)>>1;
					vector<int> nwb = b;
					ll rem = T - cursum;
					int M = mid2;
					for(int j=m-mid;j<m;j++)
					{
						rem+=b[j];
						rem-=M;
						nwb[j]=M;
						if(a[j]>=0&&nwb[j]!=a[j]) {rem=-ll(1e18);}
						ori[j]=a[j];
						a[j]=M;
					}
					greedy(nwb, rem);
					for(int j=m-mid;j<m;j++)
					{
						a[j]=ori[j];
					}
					if(rem==0)
					{
						hi2=mid2-1;
						L2=mid2;
					}
					else 
					{
						lo2=mid2+1;
					}
				}
			}			
			//how to figure out L2 otherwise!?
			while(L2<=R2)
			{
				int M = (L2+R2)>>1;
				vector<int> nwb = b;
				ll rem = T - cursum;
				for(int j=m-mid;j<m;j++)
				{
					rem+=b[j];
					rem-=M;
					nwb[j]=M;
					if(a[j]>=0&&nwb[j]!=a[j]) {rem=-ll(1e18);}
					ori[j]=a[j];
					a[j]=M;
				}
				greedy(nwb, rem);
				if(rem==0&&check(nwb))
				{
					testb=nwb; ansm=M;
					pos=1; L2=M+1;
				}
				else
				{
					R2=M-1;
				}
				for(int j=m-mid;j<m;j++)
				{
					a[j]=ori[j];
				}
			}
		}
		if(pos)
		{
			anstie=mid;
			lo=mid+1;
		}
		else hi=mid-1;
	}
	if(anstie==-1)
	{
		return {-1,-1};
	}
	return {anstie,ansm};
}


int main()
{
	ios_base::sync_with_stdio(0); cin.tie(0);
	//freopen("student-scores.in","r",stdin);
	read();
	ii sol2 = solve_full();
	cout<<sol2.fi<<' '<<sol2.se<<'\n';
}