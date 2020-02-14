#include <bits/stdc++.h>
using namespace std;
#define mp make_pair

typedef pair<int, int> edge;
typedef pair<edge, int> triple;
map<edge , int> mapping;
vector<edge> rev_map;
int N , E , K ,sat_vars;
int nextVar;
map<triple, int> dict;


//id of eid'th edge in kid'th clique
inline int e2i(int eid , int kid){return (eid - 1)*K + kid + N*K;} //offset by NK
inline int n2i(int nid , int kid){return (nid - 1)*K + kid;}
inline void sort_edge(edge& e){
	if(e.first > e.second) swap(e.first , e.second);
}
// This function maps three variables to linear dimension 
// i is the edge number
// j & l are the cliques
inline int ijl2i(int i , int j , int l){
	triple t = mp(mp(i,j),l);
	if(dict.find(t) == dict.end()){
		dict[t] = nextVar + 1;
		nextVar++;
		return nextVar;
	}
	else{
		return dict.find(t)->second;
	}
}

//returns the id of edge connecting non intersecting nodes of p and q
int connecting_edge(int p , int q){
	int a = rev_map[p].first , b = rev_map[p].second , c = rev_map[q].first , d = rev_map[q].second;
	edge temp;
	if(a == c){
		temp = make_pair(b,d);
	}
	else if(a == d){
		temp = make_pair(c,b);
	}
	else if(b == c){
		temp = make_pair(a,d);
	}
	else if(b == d){
		temp = make_pair(a,c);
	}
	else{
		temp = make_pair(a,c);
	}
	sort_edge(temp);
	if(mapping.find(temp) == mapping.end()){
		return -1;
	}
	return (mapping.find(temp)->second);
}

int main(void){
	cin >> N >> E >> K;
	nextVar = N*K + E*K;
	rev_map.resize(E + 1);
	edge curr;
	for(int i = 1 ; i <= E ; i++){
		cin >> curr.first >> curr.second;
		sort_edge(curr);
		mapping[curr] = i;
		rev_map[i] = curr;
	}

	cout << "c "<< N << " " << K << endl; 
	sat_vars = E * K;
	int clauses = E + K  + 2*K*E + (N*(N-1)*K)/2  + K*(K-1) + 3*N*K*(K - 1);
	int total_variables = N*K + E*K + K*(K-1)*N;
	cout << "p cnf " << total_variables  << " " << clauses << endl;

	//the condition that any edge has to be atleast in one clique
	// No. of clauses: E
	for(int i=1; i<=E; i++){
		for(int j=1; j<=K; j++)
			cout << e2i(i,j) <<" ";
		cout << "0" << endl;
	}

	//each clique should have atleast one edge
	// No. of clauses: K
	for(int j = 1 ; j <= K ; j++){
		for(int i = 1 ; i <= E ; i++)
			cout << e2i(i,j) << " ";
		cout << "0" << endl;
	}
	
	// completeness of clique
	// No. of clauses: C(N,2) * K 
	int c, d1, d2,b, c1 , c2, bp;
	for(int a=1; a<=N; a++){
		for(int b=a+1; b<=N; b++){
			edge e = make_pair(a,b);
			if(mapping.find(e) == mapping.end()){ //edge is absent from graph			
				for(int j=1; j<=K; j++){
					cout << "-" << n2i(a,j) << " -" << n2i(b,j) << " 0" << endl;
				}
			}
			else{
				int i = mapping.find(e)->second;
				for(int j=1; j<=K; j++){
					c = e2i(i,j);
					d1 = n2i(a,j), d2 = n2i(b,j);
					cout << "-" << c << " " << d1 << " 0" << endl;
					cout << "-" << c << " " << d2 << " 0" << endl;
					cout << "-" << d1 << " -" << d2 << " " << c << " 0" << endl;
				}
			}
		}
	}

	//no clique is a subset of another
	// No. of clauses: K * (K - 1)
	for(int j = 1 ; j <= K ; j++){
		for(int l = j+1 ; l <= K ; l++){
			for(int i = 1 ; i <= N ; i++)
				cout << ijl2i(i,j,l) << " ";
			cout << "0" << endl;
			for(int i = 1 ; i <= N ; i++)
				cout << ijl2i(i,l,j) << " ";
			cout << "0" << endl;
		}
	}

	// No. of clauses: 3 * N * K * (K - 1)
	for(int j = 1 ; j <= K ; j++){
		for(int l = j+1 ; l <= K ; l++){
			for(int i = 1 ; i <= N ; i++){
				b = ijl2i(i,j,l);
				c1 = n2i(i,j);
				c2 = n2i(i,l);
				cout << "-" << b << " " << c1 << " 0" << endl;
				cout << "-" << b << " -" << c2 << " 0" << endl;
				cout << "-" << c1 << " " << b << " " << c2 << " 0" << endl;
				bp = ijl2i(i,l,j);
				cout << "-" << bp << " " << c2 << " 0" << endl;
				cout << "-" << bp << " -" << c1 << " 0" << endl;
				cout << "-" << c2 << " " << bp << " " << c1 << " 0" << endl;
			}
		}
	}	
	return 0;
}

/*
	Test4 -> 571 sec : 10 min
	Test3 -> 1404 sec: 23 min 
*/