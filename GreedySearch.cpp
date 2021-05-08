#include <bits/stdc++.h>
using namespace std;

////////////////////////ASTAR//////////////////////////////

class AStarNode {
public:
	pair<int,int> point;
	int cost;
	double heuristicCost;
	AStarNode(pair<int,int> _point, int _cost) {
		point = _point;
		cost = _cost;
	}
	AStarNode(pair<int,int> _point, int _cost, double _heuristicCost) {
		point = _point;
		cost = _cost;
		heuristicCost = _heuristicCost;
	}
};

bool AStarQueueComp(const AStarNode* a, const AStarNode* b) {
	return ((double)a->cost + a->heuristicCost) <= ((double)b->cost + b->heuristicCost); 
}

bool AStarheightCondition(
	vector<vector<int>>& M,
	int MH,
	pair<int,int> point,
	pair<int,int> next
) {
	int now = M[point.first][point.second] >=0?0:abs(M[point.first][point.second]);	
	int then = M[next.first][next.second] >=0?0:abs(M[next.first][next.second]);	
	
	return abs(now-then) <= MH;
}

int AStarMovementCost(
	vector<vector<int>>& M,
	pair<int,int> point,
	pair<int,int> next
) {
	int heightNow = M[point.first][point.second] >=0?0:abs(M[point.first][point.second]);	
	int heightPrev = M[next.first][next.second] >=0?0:abs(M[next.first][next.second]);	
	int MudNow = M[point.first][point.second] >=0? M[point.first][point.second]:0;
	return MudNow + abs(heightNow - heightPrev);
}

double heuristicCost(pair<int,int> p1, pair<int,int> p2) {
	return 
		sqrt
		((p1.first-p2.first)*(p1.first-p2.first) + (p1.second-p2.second)*(p1.second-p2.second));
}

vector<AStarNode*> getAStarNeighbours(
	AStarNode* now,
	vector<vector<int>>& M,
	pair<int,int> t,
	int MH
) {
	vector<AStarNode*> _vec;
	int x = now->point.first, y = now->point.second;
	// top
	if(x-1>=0 && y-1>=0 // under limits
				&& AStarheightCondition(M, MH, now->point, {x-1, y-1})) { // is accessible from present location
		_vec.push_back(new AStarNode({x-1, y-1}, 
						now->cost + 14 + AStarMovementCost(M, now->point, {x-1, y-1}),
						heuristicCost(t, {x-1, y-1})));
	}
	if(x-1>=0 
				&& AStarheightCondition(M, MH, now->point, {x-1, y})) {
		_vec.push_back(new AStarNode({x-1, y}, 
						now->cost + 10 + AStarMovementCost(M, now->point, {x-1, y}),
						heuristicCost(t, {x-1, y})));
	}
	if(x-1>=0 && y+1<M[0].size() 
				&& AStarheightCondition(M, MH, now->point, {x-1, y+1})) {
		_vec.push_back(new AStarNode({x-1, y+1}, 
						now->cost + 14 + AStarMovementCost(M, now->point, {x-1, y+1}),
						heuristicCost(t, {x-1, y+1})));
	}
	
	// mid
	if(y-1>=0 
				&& AStarheightCondition(M, MH, now->point, {x, y-1})) {
		_vec.push_back(new AStarNode({x, y-1}, 
						now->cost + 10 + AStarMovementCost(M, now->point, {x, y-1}),
						heuristicCost(t, {x, y-1})));
	}
	if(y+1<M[0].size() 
				&& AStarheightCondition(M, MH, now->point, {x, y+1})) {
		_vec.push_back(new AStarNode({x, y+1}, 
						now->cost + 10 + AStarMovementCost(M, now->point, {x, y+1}),
						heuristicCost(t, {x, y+1})));
	}
	
	//bottom
	if(x+1<M.size() && y-1>=0 
				&& AStarheightCondition(M, MH, now->point, {x+1, y-1})) {
		_vec.push_back(new AStarNode({x+1, y-1}, 
						now->cost + 14 + AStarMovementCost(M, now->point, {x+1, y-1}),
						heuristicCost(t, {x+1, y-1})));
	}
	if(x+1<M.size() 
				&& AStarheightCondition(M, MH, now->point, {x+1, y})) {
		_vec.push_back(new AStarNode({x+1, y}, 
						now->cost + 10 + AStarMovementCost(M, now->point, {x+1, y}),
						heuristicCost(t, {x+1, y})));
	}
	if(x+1<M.size() && y+1<M[0].size() 
				&& AStarheightCondition(M, MH, now->point, {x+1, y+1})) {
		_vec.push_back(new AStarNode({x+1, y+1}, 
						now->cost + 14 + AStarMovementCost(M, now->point, {x+1, y+1}),
						heuristicCost(t, {x+1, y+1})));
	}
	return _vec;
}

bool findPresence(list<AStarNode*>& _list, AStarNode* next) {
	for(auto x: _list) 
		if (x->point.first == next->point.first && x->point.second == next->point.second) 
			return true;
	
	return false;
}
bool findPresence(map<pair<int,int>, int>& map, AStarNode* next) {
	return map.find({next->point}) != map.end(); 
}

void onOpenPresenceAction (
	list<AStarNode*>& open, 
	AStarNode* child,
	vector<vector<pair<int,int>>>& parent,
	AStarNode* front
) {
	auto k = open.begin();
	while(k != open.end()) {
		if((*k)->point == child->point && child->cost < (*k)->cost) {
			k = open.erase(k);
			open.push_back(child);
			parent[child->point.first][child->point.second] = front->point;
		} else {
			k++;
		}
	}
}

void onClosedPresenceAction (
	list<AStarNode*>& open,
	map<pair<int,int>, int>& closed,
	AStarNode* child,
	vector<vector<pair<int,int>>>& parent,
	AStarNode* front
) {
	auto k = closed.find(child->point);
	if (k != closed.end() && child->cost < k->second) {
		closed.erase(k);
		open.push_back(child);
		parent[child->point.first][child->point.second] = front->point;
	}
}

void AStarPrintCircuit(
	pair<int,int> t, 
	pair<int,int> start,
	vector<vector<pair<int,int>>>& parent
) {	
	pair<int,int> l = t;
	if (parent[t.first][t.second].first == -1){
		cout<<"FAIL"<<endl;
		return;	
	}
	
	list<pair<int,int>> _kx;		
	//cout<<"soln time bcs"<<endl;
	while(l != start) {
		_kx.push_back(l);
		l = parent[l.first][l.second];
	}
	_kx.push_back(start);
	
	while(!_kx.empty()) {
		cout<<_kx.back().second<<","<<_kx.back().first<<" ";
		_kx.pop_back();
	}
	cout<<endl;	
}

void AStar(
	pair<int,int> start,
	vector<pair<int,int>>& targets, 
	vector<vector<int>>& M,
	int MH
) {
	for(auto t: targets) {
		vector<vector<pair<int,int>>> 
			parent(M.size(), vector<pair<int,int>>(M[0].size(), {-1, -1})); //stores parent of each AStarNode during BFS
		list<AStarNode*> open;
		open.push_back(new AStarNode(start, 0, heuristicCost(start, t)));
		//list<AStarNode*> closed;
		map<pair<int,int>, int> closed;
		
		while(open.size() >0) {
			AStarNode* frontOne = open.front();
			open.pop_front();
	//			cout<<frontOne->point.first<<","<<frontOne->point.second<<"-"<<frontOne->cost<<endl;
			if(frontOne->point == t) break;
			
			vector<AStarNode*> neighbours = getAStarNeighbours(frontOne, M, t, MH);
			for (AStarNode* i: neighbours) {
	//				cout<<i->point.first<<","<<i->point.second<<"@"<<i->cost<<" ";
				bool openPresence = findPresence(open, i);
				bool closedPresence = findPresence(closed, i);
				if(!openPresence && !closedPresence) {
					open.push_back(i);
					parent[i->point.first][i->point.second] = frontOne->point;
				} else if (openPresence) {
					onOpenPresenceAction(open, i, parent, frontOne);
				} else if (closedPresence) {
					onClosedPresenceAction(open, closed, i, parent, frontOne);
				}
			}
			closed[frontOne->point] = frontOne->cost;
			open.sort(AStarQueueComp);
		}	
		AStarPrintCircuit(t, start, parent);
	}
	
//	cout<<"parent::::"<<endl;
//	for(auto i: parent) {
//		for(auto j: i) {
//			cout<<j.first<<","<<j.second<<"\t";
//		}
//		cout<<endl;
//	}
}
////////////////////////BFS//////////////////////////////
#include <bits/stdc++.h>
using namespace std;

void BFSPrintCircuit(
	vector<pair<int,int>>& targets, 
	pair<int,int> start,
	vector<vector<pair<int,int>>>& parent
) {
	for(auto t: targets) {
		pair<int,int> l = t;
		if (parent[t.first][t.second].first == -1){
			cout<<"FAIL"<<endl;
			continue;	
		}
		
		list<pair<int,int>> _kx;		
		//cout<<"soln time bcs"<<endl;
		while(l != start) {
			_kx.push_back(l);
			l = parent[l.first][l.second];
		}
		_kx.push_back(start);
		
		while(!_kx.empty()) {
			cout<<_kx.back().second<<","<<_kx.back().first<<" ";
			_kx.pop_back();
		}
		cout<<endl;
	}
}

bool BFSheightCondition(
	vector<vector<int>>& M,
	int MH,
	pair<int,int> point,
	pair<int,int> next
) {
	int now = M[point.first][point.second] >=0?0:abs(M[point.first][point.second]);	
	int then = M[next.first][next.second] >=0?0:abs(M[next.first][next.second]);	
	
	return abs(now-then) <= MH;
}
vector<pair<int,int>> getBFSFreshNeighbours(
	pair<int,int> point,
	vector<vector<int>>& M,
	vector<vector<int>>& state,
	int MH
) {
	vector<pair<int,int>> _vec;
	int x = point.first, y = point.second;
	// top
	if(x-1>=0 && y-1>=0 // under limits
				&& state[x-1][y-1] == 0  // state fresh
				&& BFSheightCondition(M, MH, point, {x-1, y-1})) { // is accessible from present location
		_vec.push_back({x-1, y-1});
	}
	if(x-1>=0 
				&& state[x-1][y] == 0  // 
				&& BFSheightCondition(M, MH, point, {x-1, y})) {
		_vec.push_back({x-1, y});
	}
	if(x-1>=0 && y+1<M[0].size() 
				&& state[x-1][y+1] == 0
				&& BFSheightCondition(M, MH, point, {x-1, y+1})) {
		_vec.push_back({x-1, y+1});
	}
	
	// mid
	if(y-1>=0 
				&& state[x][y-1] == 0
				&& BFSheightCondition(M, MH, point, {x, y-1})) {
		_vec.push_back({x, y-1});
	}
	if(y+1<M[0].size() 
				&& state[x][y+1] == 0
				&& BFSheightCondition(M, MH, point, {x, y+1})) {
		_vec.push_back({x, y+1});
	}
	
	//bottom
	if(x+1<M.size() && y-1>=0 
				&& state[x+1][y-1] == 0
				&& BFSheightCondition(M, MH, point, {x+1, y-1})) {
		_vec.push_back({x+1, y-1});
	}
	if(x+1<M.size() 
				&& state[x+1][y] == 0
				&& BFSheightCondition(M, MH, point, {x+1, y})) {
		_vec.push_back({x+1, y});
	}
	if(x+1<M.size() && y+1<M[0].size() 
				&& state[x+1][y+1] == 0
				&& BFSheightCondition(M, MH, point, {x+1, y+1})) {
		_vec.push_back({x+1, y+1});
	}	
	return _vec;
}
void BFS(
	pair<int,int> start,
	vector<pair<int,int>>& targets, 
	vector<vector<int>>& M,
	int MH
) {	
	vector<vector<int>> state(M.size(), vector<int>(M[0].size(), 0)); // 0 fresh, 1 in process, 2 done
	vector<vector<pair<int,int>>> 
			parent(M.size(), vector<pair<int,int>>(M[0].size(), {-1, -1})); //stores parent of each node during BFS
	
	list<pair<int,int>> _list;
	_list.push_back(start);
	
	while(!_list.empty()) {
		// fetch start point
		pair<int,int> point = _list.front();
		_list.pop_front();
		state[point.first][point.second] = 2;
		//cout<<point.first<<","<<point.second<<" - ";
		
		// check which neighbours are fresh and accessible
		for(auto x: getBFSFreshNeighbours(point, M, state, MH)) {
			//cout<<x.first<<","<<x.second<<" ";
			parent[x.first][x.second] = point;
			state[x.first][x.second] = 1;
			_list.push_back(x);
		}
		//cout<<endl;
	}
	
	BFSPrintCircuit(targets, start, parent);
}
////////////////////////UCS//////////////////////////////
#include <bits/stdc++.h>
using namespace std;

class Node {
public:
	pair<int,int> point;
	int cost;
	Node(pair<int,int> _point, int _cost) {
		point = _point;
		cost = _cost;
	}
};

bool QueueComp(const Node* a, const Node* b) {
	return a->cost < b->cost;
}

bool UCSheightCondition(
	vector<vector<int>>& M,
	int MH,
	pair<int,int> point,
	pair<int,int> next
) {
	int now = M[point.first][point.second] >=0?0:abs(M[point.first][point.second]);	
	int then = M[next.first][next.second] >=0?0:abs(M[next.first][next.second]);	
	
	return abs(now-then) <= MH;
}

vector<Node*> getUCSNeighbours(
	Node* now,
	vector<vector<int>>& M,
	int MH
) {
	vector<Node*> _vec;
	int x = now->point.first, y = now->point.second;
	// top
	if(x-1>=0 && y-1>=0 // under limits
				&& UCSheightCondition(M, MH, now->point, {x-1, y-1})) { // is accessible from present location
		_vec.push_back(new Node({x-1, y-1}, now->cost + 14));
	}
	if(x-1>=0 
				&& UCSheightCondition(M, MH, now->point, {x-1, y})) {
		_vec.push_back(new Node({x-1, y}, now->cost + 10));
	}
	if(x-1>=0 && y+1<M[0].size() 
				&& UCSheightCondition(M, MH, now->point, {x-1, y+1})) {
		_vec.push_back(new Node({x-1, y+1}, now->cost + 14));
	}
	// mid
	if(y-1>=0 
				&& UCSheightCondition(M, MH, now->point, {x, y-1})) {
		_vec.push_back(new Node({x, y-1}, now->cost + 10));
	}
	if(y+1<M[0].size() 
				&& UCSheightCondition(M, MH, now->point, {x, y+1})) {
		_vec.push_back(new Node({x, y+1}, now->cost + 10));
	}
	
	//bottom
	if(x+1<M.size() && y-1>=0 
				&& UCSheightCondition(M, MH, now->point, {x+1, y-1})) {
		_vec.push_back(new Node({x+1, y-1}, now->cost + 14));
	}
	if(x+1<M.size() 
				&& UCSheightCondition(M, MH, now->point, {x+1, y})) {
		_vec.push_back(new Node({x+1, y}, now->cost + 10));
	}
	if(x+1<M.size() && y+1<M[0].size() 
				&& UCSheightCondition(M, MH, now->point, {x+1, y+1})) {
		_vec.push_back(new Node({x+1, y+1}, now->cost + 14));
	}
	return _vec;
}

bool findPresence(list<Node*>& _list, Node* next) {
	for(auto x: _list) 
		if (x->point.first == next->point.first && x->point.second == next->point.second) 
			return true;
	
	return false;
}
bool findPresence(map<pair<int,int>, int>& map, Node* next) {
	return map.find({next->point}) != map.end(); 
}

void onOpenPresenceAction (
	list<Node*>& open, 
	Node* child,
	vector<vector<pair<int,int>>>& parent,
	Node* front
) {
	auto k = open.begin();
	while(k != open.end()) {
		if((*k)->point == child->point && child->cost < (*k)->cost) {
			k = open.erase(k);
			open.push_back(child);
			parent[child->point.first][child->point.second] = front->point;
		} else {
			k++;
		}
	}
}

void onClosedPresenceAction (
	list<Node*>& open,
	map<pair<int,int>, int>& closed,
	Node* child,
	vector<vector<pair<int,int>>>& parent,
	Node* front
) {
	auto k = closed.find(child->point);
	if (k != closed.end() && child->cost < k->second) {
		closed.erase(k);
		open.push_back(child);
		parent[child->point.first][child->point.second] = front->point;
	}
}

void UCSPrintCircuit(
	vector<pair<int,int>>& targets, 
	pair<int,int> start,
	vector<vector<pair<int,int>>>& parent
) {
	for(auto t: targets) {
		pair<int,int> l = t;
		if (parent[t.first][t.second].first == -1){
			cout<<"FAIL"<<endl;
			continue;	
		}
		
		list<pair<int,int>> _kx;		
		//cout<<"soln time bcs"<<endl;
		while(l != start) {
			_kx.push_back(l);
			l = parent[l.first][l.second];
		}
		_kx.push_back(start);
		
		while(!_kx.empty()) {
			cout<<_kx.back().second<<","<<_kx.back().first<<" ";
			_kx.pop_back();
		}
		cout<<endl;
	}
}

void UCS(
	pair<int,int> start,
	vector<pair<int,int>>& targets, 
	vector<vector<int>>& M,
	int MH
) {
	vector<vector<pair<int,int>>> 
		parent(M.size(), vector<pair<int,int>>(M[0].size(), {-1, -1})); //stores parent of each node during BFS
	list<Node*> open;
	open.push_back(new Node(start, 0));
	//list<Node*> closed;
	map<pair<int,int>, int> closed;
	
	while(open.size() >0) {
		Node* frontOne = open.front();
		open.pop_front();
//			cout<<frontOne->point.first<<","<<frontOne->point.second<<"-"<<frontOne->cost<<endl;
//		if(isNodeATarget(frontOne, start, t, parent)) break;
		
		vector<Node*> neighbours = getUCSNeighbours(frontOne, M, MH);
		for (Node* i: neighbours) {
//				cout<<i->point.first<<","<<i->point.second<<"@"<<i->cost<<" ";
			bool openPresence = findPresence(open, i);
			bool closedPresence = findPresence(closed, i);
			if(!openPresence && !closedPresence) {
				open.push_back(i);
				parent[i->point.first][i->point.second] = frontOne->point;
			} else if (openPresence) {
				onOpenPresenceAction(open, i, parent, frontOne);
			} else if (closedPresence) {
				onClosedPresenceAction(open, closed, i, parent, frontOne);
			}
		}
//			cout<<endl<<"&#### open ";
		closed[frontOne->point] = frontOne->cost;
		open.sort(QueueComp);
//		for(auto i: open) cout<<i->point.first<<","<<i->point.second<<"@"<<i->cost<<" ";
//		cout<<endl<<"%#### closed ";
//		for(auto i: closed) cout<<i->point.first<<","<<i->point.second<<"@"<<i->cost<<" ";
//		cout<<"^"<<endl;
	}
	
//	cout<<"parent::::"<<endl;
//	for(auto i: parent) {
//		for(auto j: i) {
//			cout<<j.first<<","<<j.second<<"\t";
//		}
//		cout<<endl;
//	}
	UCSPrintCircuit(targets, start, parent);
}

/////////////////////////////////////////////////////////
int main() {
	ifstream fin("input.txt");
	//freopen("output.txt","w",stdout);
	
	string strategy;fin>>strategy;      	//cout<<strategy<<endl;
	int W, H; fin>>W>>H;					//cout<<W<<" "<<H<<endl;
	int X, Y; fin>>X>>Y;   					//cout<<X<<" "<<Y<<endl;
	int MH; fin>>MH;						//cout<<MH<<endl;
	int N; fin>>N;							//cout<<N<<endl;
	vector<pair<int,int>> targets;
	for(int i=0; i<N; i++) {
		int a, b; fin>>a>>b;
		targets.push_back({b, a});
	}
	//for(auto i: targets) cout<<i.first<<" "<<i.second<<endl;
	
	vector<vector<int>> M;
	for(int i=0; i<H; i++) {
		M.push_back(vector<int>());
		for(int j=0; j<W; j++) {
			int a; fin>>a;
			M.back().push_back(a);
		}
	}
	//for(auto i: M) {for(auto j: i) cout<<j<<" "; cout<<endl;}	
	
	if(strategy == "BFS") {
		BFS({Y, X}, targets, M, MH);
	} else if(strategy == "UCS") {
		UCS({Y, X}, targets, M, MH);
	} else if(strategy == "A*") {
		AStar({Y, X}, targets, M, MH);
	} 
	
	return 0;
}
