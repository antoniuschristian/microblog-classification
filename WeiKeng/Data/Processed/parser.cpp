#include <cstdio>
#include <map>
#include <set>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

#define REP(i,n) for(int i=0,_n=(n);i<_n;++i)
#define FOR(i,a,b) for(int i=(a),_b=(b);i<=_b;++i)
#define FORD(i,a,b) for(int i=(a),_b=(b);i>=_b;--i)
#define FOREACH(it,arr) for (__typeof((arr).begin()) it=(arr).begin(); it!=(arr).end(); it++)


struct tdata {
	string value;
	map <string,vector<tdata> > next;
};

string line;

string parse_recurse(string s, int &i, char open, char close) {
	string ret;
	ret += s[i];
	i++;
	int  cnt = 1;
	bool quote = false;
	while ( cnt != 0 ) {
		if ( !quote && s[i] == open  ) cnt++;
		if ( !quote && s[i] == close ) cnt--;
		if ( s[i] == '\"' ) quote = !quote;
		ret += s[i];
		i++;
	}

	return ret;
}

string clean(string s) {
	int a = 0, b = s.size()-1;
	while ( a < s.size() && s[a] == ' ' ) a++;
	while ( b >= 0 && s[b] == ' ' ) b--;
	return s.substr(a,b-a+1);
}

vector <string> split_array(string s) {
	vector <string> ret;
	string r;
	int i = 0;
	while ( i < s.size() && s[i] == ' ' ) i++;
	while ( i < s.size() ) {
		if ( s[i] == '{' ) ret.push_back(parse_recurse(s,i,'{','}'));
		else if ( s[i] == ',' ) {
			ret.push_back(clean(r));
			r = "";
		}
		else {
			r += s[i];
		}
		i++;
	}

	if ( r != "" ) ret.push_back(clean(r));

	return ret;
}

map <string,vector<tdata> > parse(string s) {
	map <string,vector<tdata> > ret;
	
	int i = 0;
	while ( i < s.size() && s[i] == '{'  ) i++;

	do {
		string attr, value;
		while ( i < s.size() && s[i] == ' '  ) i++;
		while ( i < s.size() && s[i] == '\"' ) i++;	
		while ( i < s.size() && s[i] != '\"' ) attr += s[i], i++;
		while ( i < s.size() && s[i] == '\"' ) i++;
		while ( i < s.size() && s[i] == ' '  ) i++;
		while ( i < s.size() && s[i] == ':'  ) i++;
		while ( i < s.size() && s[i] == ' '  ) i++;
		
		bool recurse = false;
		if ( s[i] == '{' ) {
			string r = parse_recurse(s,i,'{','}');
			recurse = true;
			ret[attr].push_back((tdata){"",parse(r)});
		}
		else if ( s[i] == '[' ) {
			int cnt = 1;
			i++;
			string r;
			bool quote = false;
			while ( i < s.size() && cnt != 0 ) {
				if ( !quote && s[i] == '[' ) cnt++;
				if ( !quote && s[i] == ']' ) cnt--;
				if ( s[i] == '\"' ) quote = !quote;
				if ( cnt != 0 ) r += s[i];
				i++;
			}
			vector <string> v = split_array(r);
			REP(i,v.size()) {
				if ( v[i][0] == '{' ) {
					ret[attr].push_back((tdata){"",parse(v[i])});
				}
				else {
					ret[attr].push_back((tdata){v[i]});
				}
			}
		}
		else if ( s[i] != '\"' ) {
			while ( i < s.size() && s[i] != ' ' && s[i] != '}' ) value += s[i], i++;
		}
		else {
			while ( i < s.size() && s[i] == '\"' ) i++;	
			while ( i < s.size() && !(s[i] == '\"' && s[i-1] != '\\') ) value += s[i], i++;
			while ( i < s.size() && s[i] == '\"' ) i++;	
		}
		while ( i < s.size() && s[i] == ' ' ) i++;
		while ( i < s.size() && s[i] == ',' ) i++;
	
		if ( !recurse) ret[attr].push_back((tdata){value});

	} while ( i < s.size() && s[i] != '}' );

	return ret;
}


set <string> xxx;


void show(string p, map<string,vector<tdata> > m) {
	string prefix = (p == "" ? "" : (p + "-"));
	FOREACH(it,m) {
		vector <tdata> &v = it->second;
		if ( v.size() == 1 ) {
			if ( v[0].next.empty() && v[0].value != "" ) {
				cout << prefix << it->first << " -- " << v[0].value << endl;
				xxx.insert(prefix+it->first);
			}
			else if ( !v[0].next.empty() )
				show(prefix+it->first,v[0].next);
		}
		else {
			REP(i,v.size()) {
				if ( v[i].next.empty() && v[i].value != "" ) {
					cout << prefix << it->first << " -- " << v[i].value << endl;
					xxx.insert(prefix+it->first);
				}
				else if ( !v[i].next.empty() )
					show(prefix+it->first,v[i].next);
			}
		}
	}
}


int main()
{
	while (	getline(cin,line) ) {
		if ( cin.eof() ) break;
		map <string,vector<tdata> > t = parse(line);
		show("",t);
		cout << "#" << endl;
	}

	FILE *fattr = fopen( "attr.list", "w" );
	FOREACH(it,xxx) fprintf( fattr, "%s\n", it->c_str() );
	return 0;
}
