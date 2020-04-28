#include<iostream>
#include<vector>
#include<map>
#include<queue>
#include<algorithm>
#include<string>
#include<fstream>
#include<sstream>
#include<stack>
using namespace std;

struct Tail_State {
	vector<vector<int>> tail;
	vector<int> move;
};

vector<string> DIRECT{ "l", "u", "r", "d" };

map<vector<vector<int>>, Tail_State> Visited;

vector<vector<int>> ZEROPOS{ {4, 3},{4, 4} };
vector<vector<int>> NUMSPOS
{ {-1,-1}, {0, 0}, {0, 1},{0, 2},{0, 3}, {0, 4},
{2, 0}, {1, 0}, {1, 2}, {1, 3}, {1, 4}, 
{2, 2}, {2, 3}, {2, 4}, {3, 0}, {3, 1},
{3, 2}, {3, 3}, {3, 4}, {4, 0}, {4, 1}, {4, 2} };

inline bool isvalid(int i, int j) {
	return i >= 0 && i <= 4 && j >= 0 && j <= 4;
}

int CalG(vector<vector<int>> State) { // 给出这个状态的g的估计
	int Count = 0;
	int m = State.size();
	int n = State[0].size();
	int Flag7 = 0;
	vector<vector<int>> zero;
	for (int i = 0; i != m; i++) {
		for (int j = 0; j != n; j++) {
			if (State[i][j] == 7) {
				if (Flag7 == 0) {
					Count += (abs(NUMSPOS[7][0] - i) + abs(NUMSPOS[7][1] - j)) * 4;
					Flag7 = 1;
				}
			}
			else if (State[i][j] == 0) {
				vector<int> temp;
				temp.push_back(i);
				temp.push_back(j);
				zero.push_back(temp);
			}
			else {
				if (State[i][j] == 1 && State[i][j] == 2)
					Count += (abs(NUMSPOS[State[i][j]][0] - i) + abs(NUMSPOS[State[i][j]][1] - j)) * 2;
				else
					Count += (abs(NUMSPOS[State[i][j]][0] - i) + abs(NUMSPOS[State[i][j]][1] - j));
			}
		}
	}
	int zero_loss1 = abs(zero[0][0] - ZEROPOS[0][0]) + abs(zero[0][1] - ZEROPOS[0][1]) + abs(zero[1][0] - ZEROPOS[1][0]) + abs(zero[1][1] - ZEROPOS[1][1]);
	int zero_loss2 = abs(zero[1][0] - ZEROPOS[0][0]) + abs(zero[1][1] - ZEROPOS[0][1]) + abs(zero[0][0] - ZEROPOS[1][0]) + abs(zero[0][1] - ZEROPOS[1][1]);
	Count += min(zero_loss1, zero_loss2)*0;
	Count += abs(zero[0][0] - zero[1][0]) + abs(zero[0][1] - zero[1][1]) - 1;
	return Count;
}

struct State {
	int f;
	int g;
	int total;
	vector<vector<int>> state;
	State(int f, vector<vector<int>> state) {
		this->f = f;
		this->g = CalG(state);
		this->total = f + g;
		this->state = state;
	}
	friend bool operator < (State a, State b)
	{
		if (a.total > b.total) {    //重载小于号使得小的先出队列
			return true;
		}
		else if (a.total < b.total) {
			return false;
		}
		else {
			return a.f> b.f;
		}
	}
};

vector<vector<int>> ReadFromFile(string file_name) {
	vector<int> Temp(5, 0);
	vector<vector<int>> Start(5, Temp);
	ifstream fin(file_name);
	string str;
	for (int i = 0; i != 5; i++) {
		getline(fin, str);
		replace(str.begin(), str.end(), ',', ' ');
		stringstream ss;
		ss.str(str);
		for (int j = 0; j != 5; j++) {
			ss >> Start[i][j];
		}
	}
	for (int i = 0; i != 5; i++) {
		for (int j = 0; j != 5; j++) {
			cout << Start[i][j];
		}
	}
	fin.close();
	return Start;
}

void UpdateSuccessors(priority_queue<State> & Successors, int& EndFlag) {
	State minval = Successors.top();
	Successors.pop();
	//首先探索7的位置
	vector<vector<int>> MATRIX = minval.state;
	int X_7, Y_7;
	for (int i = 0; i != 5; i++) {
		for (int j = 0; j != 5; j++) {
			if (MATRIX[i][j] == 7) {
				X_7 = i;
				Y_7 = j;
				goto label;
			}
		}
	}
label:
	if (isvalid(X_7, Y_7 - 1) && isvalid(X_7 + 1, Y_7) && MATRIX[X_7][Y_7 - 1] == 0 && MATRIX[X_7 + 1][Y_7] == 0) { // 7的左面有空隙
		vector<vector<int>> MATRIX_TEMP = MATRIX;
		MATRIX_TEMP[X_7][Y_7 - 1] = 7;
		MATRIX_TEMP[X_7 + 1][Y_7] = 7;
		MATRIX_TEMP[X_7][Y_7 + 1] = 0;
		MATRIX_TEMP[X_7 + 1][Y_7 + 1] = 0;
		if (Visited.find(MATRIX_TEMP) == Visited.end()) { //还没访问过
			Successors.push(State(minval.f + 1, MATRIX_TEMP));
			vector<int> Temp;
			Temp.push_back(7);
			Temp.push_back(0);
			Tail_State Value{ MATRIX, Temp };
			Visited[MATRIX_TEMP] = Value;
			if (CalG(MATRIX_TEMP) == 0) {
				EndFlag = 1;
				return;
			}
		}
	}
	if (isvalid(X_7 - 1, Y_7) && isvalid(X_7 - 1, Y_7 + 1) && MATRIX[X_7 - 1][Y_7] == 0 && MATRIX[X_7 - 1][Y_7 + 1] == 0) { // 7的上面有空隙
		vector<vector<int>> MATRIX_TEMP = MATRIX;
		MATRIX_TEMP[X_7 - 1][Y_7] = 7;
		MATRIX_TEMP[X_7 - 1][Y_7 + 1] = 7;
		MATRIX_TEMP[X_7][Y_7] = 0;
		MATRIX_TEMP[X_7 + 1][Y_7 + 1] = 0;
		if (Visited.find(MATRIX_TEMP) == Visited.end()) { //还没访问过
			Successors.push(State(minval.f + 1, MATRIX_TEMP));
			vector<int> Temp;
			Temp.push_back(7);
			Temp.push_back(1);
			Tail_State Value{ MATRIX, Temp };
			Visited[MATRIX_TEMP] = Value;
			if (CalG(MATRIX_TEMP) == 0) {
				EndFlag = 1;
				return;
			}
		}
	}
	if (isvalid(X_7, Y_7+2) && isvalid(X_7 + 1, Y_7 + 2) && MATRIX[X_7][Y_7 + 2] == 0 && MATRIX[X_7 + 1][Y_7 + 2] == 0) { // 7的右面有空隙
		vector<vector<int>> MATRIX_TEMP = MATRIX;
		MATRIX_TEMP[X_7][Y_7 + 2] = 7;
		MATRIX_TEMP[X_7 + 1][Y_7 + 2] = 7;
		MATRIX_TEMP[X_7][Y_7] = 0;
		MATRIX_TEMP[X_7 + 1][Y_7 + 1] = 0;
		if (Visited.find(MATRIX_TEMP) == Visited.end()) { //还没访问过
			Successors.push(State(minval.f + 1, MATRIX_TEMP));
			vector<int> Temp;
			Temp.push_back(7);
			Temp.push_back(2);
			Tail_State Value{ MATRIX, Temp };
			Visited[MATRIX_TEMP] = Value;
			if (CalG(MATRIX_TEMP) == 0) {
				EndFlag = 1;
				return;
			}
		}
	}
	if (isvalid(X_7 + 1, Y_7) && isvalid(X_7 + 2, Y_7 + 1) && MATRIX[X_7 + 1][Y_7] == 0 && MATRIX[X_7 + 2][Y_7 + 1] == 0) { // 7的下面有空隙
		vector<vector<int>> MATRIX_TEMP = MATRIX;
		MATRIX_TEMP[X_7 + 1][Y_7] = 7;
		MATRIX_TEMP[X_7 + 2][Y_7 + 1] = 7;
		MATRIX_TEMP[X_7][Y_7] = 0;
		MATRIX_TEMP[X_7][Y_7 + 1] = 0;
		if (Visited.find(MATRIX_TEMP) == Visited.end()) { //还没访问过
			Successors.push(State(minval.f + 1, MATRIX_TEMP));
			vector<int> Temp;
			Temp.push_back(7);
			Temp.push_back(3);
			Tail_State Value{ MATRIX, Temp };
			Visited[MATRIX_TEMP] = Value;
			if (CalG(MATRIX_TEMP) == 0) {
				EndFlag = 1;
				return;
			}
		}
	}
	//寻找0的位置:
	vector<vector<int>> zero_pos;
	for (int i = 0; i != 5; i++) {
		for (int j = 0; j != 5; j++) {
			if (MATRIX[i][j] == 0) {
				vector<int> temp;
				temp.push_back(i);
				temp.push_back(j);
				zero_pos.push_back(temp);
			}
		}
	}
	int x = zero_pos[0][0];
	int y = zero_pos[0][1];
	if (isvalid(x - 1, y) && MATRIX[x - 1][y] != 0 && MATRIX[x - 1][y] != 7) { //上
		vector<vector<int>> MATRIX_TEMP = MATRIX;
		MATRIX_TEMP[x][y] = MATRIX[x - 1][y];
		MATRIX_TEMP[x - 1][y] = 0;
		if (Visited.find(MATRIX_TEMP) == Visited.end()) { //还没访问过
			Successors.push(State(minval.f + 1, MATRIX_TEMP));
			vector<int> Temp;
			Temp.push_back(MATRIX[x-1][y]);
			Temp.push_back(3);
			Tail_State Value{ MATRIX, Temp };
			Visited[MATRIX_TEMP] = Value;
			if (CalG(MATRIX_TEMP) == 0) {
				EndFlag = 1;
				return;
			}
		}
	}
	if (isvalid(x + 1, y) && MATRIX[x + 1][y] != 0 && MATRIX[x + 1][y] != 7) { //下
		vector<vector<int>> MATRIX_TEMP = MATRIX;
		MATRIX_TEMP[x][y] = MATRIX[x + 1][y];
		MATRIX_TEMP[x + 1][y] = 0;
		if (Visited.find(MATRIX_TEMP) == Visited.end()) { //还没访问过
			Successors.push(State(minval.f + 1, MATRIX_TEMP));
			vector<int> Temp;
			Temp.push_back(MATRIX[x + 1][y]);
			Temp.push_back(1);
			Tail_State Value{ MATRIX, Temp };
			Visited[MATRIX_TEMP] = Value;
			if (CalG(MATRIX_TEMP) == 0) {
				EndFlag = 1;
				return;
			}
		}
	}
	if (isvalid(x, y-1) && MATRIX[x][y-1] != 0 && MATRIX[x][y-1] != 7) { //左
		vector<vector<int>> MATRIX_TEMP = MATRIX;
		MATRIX_TEMP[x][y] = MATRIX[x][y-1];
		MATRIX_TEMP[x][y-1] = 0;
		if (Visited.find(MATRIX_TEMP) == Visited.end()) { //还没访问过
			Successors.push(State(minval.f + 1, MATRIX_TEMP));
			vector<int> Temp;
			Temp.push_back(MATRIX[x][y-1]);
			Temp.push_back(2);
			Tail_State Value{ MATRIX, Temp };
			Visited[MATRIX_TEMP] = Value;
			if (CalG(MATRIX_TEMP) == 0) {
				EndFlag = 1;
				return;
			}
		}
	}
	if (isvalid(x, y + 1) && MATRIX[x][y + 1] != 0 && MATRIX[x][y + 1] != 7) { //右
		vector<vector<int>> MATRIX_TEMP = MATRIX;
		MATRIX_TEMP[x][y] = MATRIX[x][y + 1];
		MATRIX_TEMP[x][y + 1] = 0;
		if (Visited.find(MATRIX_TEMP) == Visited.end()) { //还没访问过
			Successors.push(State(minval.f + 1, MATRIX_TEMP));
			vector<int> Temp;
			Temp.push_back(MATRIX[x][y+1]);
			Temp.push_back(0);
			Tail_State Value{ MATRIX, Temp };
			Visited[MATRIX_TEMP] = Value;
			if (CalG(MATRIX_TEMP) == 0) {
				EndFlag = 1;
				return;
			}
		}
	}
	x = zero_pos[1][0];
	y = zero_pos[1][1];
	if (isvalid(x - 1, y) && MATRIX[x - 1][y] != 0 && MATRIX[x - 1][y] != 7) { //上
		vector<vector<int>> MATRIX_TEMP = MATRIX;
		MATRIX_TEMP[x][y] = MATRIX[x - 1][y];
		MATRIX_TEMP[x - 1][y] = 0;
		if (Visited.find(MATRIX_TEMP) == Visited.end()) { //还没访问过
			Successors.push(State(minval.f + 1, MATRIX_TEMP));
			vector<int> Temp;
			Temp.push_back(MATRIX[x - 1][y]);
			Temp.push_back(3);
			Tail_State Value{ MATRIX, Temp };
			Visited[MATRIX_TEMP] = Value;
			if (CalG(MATRIX_TEMP) == 0) {
				EndFlag = 1;
				return;
			}
		}
	}
	if (isvalid(x + 1, y) && MATRIX[x + 1][y] != 0 && MATRIX[x + 1][y] != 7) { //下
		vector<vector<int>> MATRIX_TEMP = MATRIX;
		MATRIX_TEMP[x][y] = MATRIX[x + 1][y];
		MATRIX_TEMP[x + 1][y] = 0;
		if (Visited.find(MATRIX_TEMP) == Visited.end()) { //还没访问过
			Successors.push(State(minval.f + 1, MATRIX_TEMP));
			vector<int> Temp;
			Temp.push_back(MATRIX[x + 1][y]);
			Temp.push_back(1);
			Tail_State Value{ MATRIX, Temp };
			Visited[MATRIX_TEMP] = Value;
			if (CalG(MATRIX_TEMP) == 0) {
				EndFlag = 1;
				return;
			}
		}
	}
	if (isvalid(x, y - 1) && MATRIX[x][y - 1] != 0 && MATRIX[x][y - 1] != 7) { //左
		vector<vector<int>> MATRIX_TEMP = MATRIX;
		MATRIX_TEMP[x][y] = MATRIX[x][y - 1];
		MATRIX_TEMP[x][y - 1] = 0;
		if (Visited.find(MATRIX_TEMP) == Visited.end()) { //还没访问过
			Successors.push(State(minval.f + 1, MATRIX_TEMP));
			vector<int> Temp;
			Temp.push_back(MATRIX[x][y - 1]);
			Temp.push_back(2);
			Tail_State Value{ MATRIX, Temp };
			Visited[MATRIX_TEMP] = Value;
			if (CalG(MATRIX_TEMP) == 0) {
				EndFlag = 1;
				return;
			}
		}
	}
	if (isvalid(x, y + 1) && MATRIX[x][y + 1] != 0 && MATRIX[x][y + 1] != 7) { //右
		vector<vector<int>> MATRIX_TEMP = MATRIX;
		MATRIX_TEMP[x][y] = MATRIX[x][y + 1];
		MATRIX_TEMP[x][y + 1] = 0;
		if (Visited.find(MATRIX_TEMP) == Visited.end()) { //还没访问过
			Successors.push(State(minval.f + 1, MATRIX_TEMP));
			vector<int> Temp;
			Temp.push_back(MATRIX[x][y + 1]);
			Temp.push_back(0);
			Tail_State Value{ MATRIX, Temp };
			Visited[MATRIX_TEMP] = Value;
			if (CalG(MATRIX_TEMP) == 0) {
				EndFlag = 1;
				return;
			}
		}
	}
}

void Print_Path(string outputfile) {
	ofstream fout(outputfile, ios::out);
	vector<vector<int>> END{ {1,2,3,4,5},{7,7,8,9,10},{6,7,11,12,13},{14,15,16,17,18},{19,20,21,0,0} };
	Tail_State pre = Visited[END];
	stack<vector<int>> Moves;
	while (pre.move[1] != -1) {
		Moves.push(pre.move);
		pre = Visited[pre.tail];
	}
	while (!Moves.empty()) {
		fout << Moves.top()[0] << " " << DIRECT[Moves.top()[1]] << endl;
		Moves.pop();
	}
}

int main() {
	string Test_Path = "D:\\GithubLocalRepo\\Algorithm\\AStar\\3";
	vector<vector<int>> start_state = ReadFromFile(Test_Path + ".txt");
	State start(0, start_state);
	priority_queue<State> Successors;
	Successors.push(start);
	vector<int> STOP{ 0,-1 };
	Tail_State TARGET{ start_state, STOP };
	Visited[start_state] = TARGET;
	int EndFlag = 0;
	while (1) {
		if (EndFlag == 1) {
			cout << "Search Finished!" << endl;
			Print_Path(Test_Path + "Path.txt");
			break;
		}
		else {
			UpdateSuccessors(Successors, EndFlag);
		}
	}
	system("pause");
}