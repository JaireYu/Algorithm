#include<iostream>
#include<vector>
#include<fstream>
#include<algorithm>
#include<queue>
#include<stack>
#include <cstdlib>
#include<windows.h>
using namespace std;
struct edge {
	int node1;
	int node2;
};
bool compare(const edge &edge1, const edge &edge2) {
	if (edge1.node1 < edge2.node1)
		return true;
	else if (edge1.node1 == edge2.node1 && edge1.node2 < edge2.node2)
		return true;
	else
		return false;
}
void Print_Path(const vector<int>& pre, const int& dst) {
	stack<int> Path;
	Path.push(dst);
	cout << "The path is: ";
	int i = pre[dst];
	while(i != -1){
		Path.push(i);
		i = pre[i];
	}
	cout << Path.top();
	Path.pop();
	while (!Path.empty()) {
		cout << " -> " << Path.top();
		Path.pop();
	}
	cout << endl;
}
void Print_Path(const vector<int>& predst, const vector<int>& presrc, const int& min) {
	cout << min << endl;
	stack<int> Path;
	Path.push(min);
	int i = presrc[min];
	while (i != -1) {
		Path.push(i);
		i = presrc[i];
	}
	cout << Path.top();
	Path.pop();
	while (!Path.empty()) {
		cout << " -> " << Path.top();
		Path.pop();
	}
	i = predst[min];
	while (i != -1) {
		cout << " -> " << i;
		i = predst[i];
	}
	cout << endl;
}
int main() {
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_endTime;
	LARGE_INTEGER m_beginTime;
	QueryPerformanceFrequency(&m_nFreq); // 获取时钟周期
	vector<edge> CSRtemp;
	ifstream file;
	file.open("C:\\Users\\yujiarui\\Desktop\\facebook_combined.txt", ios::in);
	while (!file.eof()) {
		int x, y;
		file >> x;
		file >> y;
		edge temp1{ x,y };
		edge temp2{ y,x };
		CSRtemp.push_back(temp1);
		CSRtemp.push_back(temp2);
	}
	sort(CSRtemp.begin(), CSRtemp.end(), compare);
	int len = CSRtemp.size();
	int row_num = CSRtemp[len - 1].node1;
	vector<int> node;
	vector<int> row_start(row_num + 2, -1);
	int last_num = CSRtemp[0].node1 - 1;
	int offset = 0;
	for (int i = 0; i < len; i++) {
		int num = CSRtemp[i].node1;
		if (num == last_num) {
			node.push_back(CSRtemp[i].node2);
		}
		else {
			row_start[num] = offset;
			node.push_back(CSRtemp[i].node2);
		}
		offset++;
		last_num = num;
	}
	row_start[row_num + 1] = offset;
	for (int i = row_num + 1; i != -1; i--) {
		if (row_start[i] == -1) {
			row_start[i] = row_start[i + 1];
		}
	}
	char c;
	cout << "Single BFS" << endl;
	while (1) {
	start:
		cout << "quit?" << endl;
		cin >> c;
		if (c == 'q')
			break;
		vector<int> visited(row_num + 1, 0);
		queue<int> Queue;
		vector<int> pre(row_num + 1, -1);
		int src;
		int dst;
		cout << "Input the Two Node to be Checked" << endl;
		cout << "Input Src:";
		cin >> src;
		cout << "Input Dst:";
		cin >> dst;
		QueryPerformanceCounter(&m_beginTime);//获取当前时间
		Queue.push(src);
		visited[src] = 1;
		if (src == dst) {
			cout << "It can be accessed" << endl;
			Print_Path(pre, dst);
			goto start;
			return 0;
		}
		while (!Queue.empty()) {
			int len = Queue.size();
			for (int i = 0; i != len; i++) {
				int temp = Queue.front();
				for (int j = row_start[temp]; j != row_start[temp + 1]; j++) {
					if (node[j] == dst) {
						pre[node[j]] = temp;
						cout << "It can be accessed" << endl;
						Print_Path(pre, dst);
						QueryPerformanceCounter(&m_endTime);//获取当前时间
						cout << (double)(m_endTime.QuadPart - m_beginTime.QuadPart) / m_nFreq.QuadPart << endl;
						goto start;
					}
					if (visited[node[j]] == 0) {
						pre[node[j]] = temp;
						visited[node[j]] = 1;
						Queue.push(node[j]);
					}
				}
				Queue.pop();
			}
		}
		cout << "It cannot be accessed" << endl;
		QueryPerformanceCounter(&m_endTime);//获取当前时间
		cout << (double)(m_endTime.QuadPart - m_beginTime.QuadPart) / m_nFreq.QuadPart << endl;
		goto start;
	}
	cout << "double BFS" << endl;
	while (1) {
	start_:
		cout << "quit?" << endl;
		cin >> c;
		if (c == 'q')
			break;
		vector<int> visited(row_num + 1, 0);
		queue<int> Queuesrc;
		vector<int> presrc(row_num + 1, -1);
		queue<int> Queuedst;
		vector<int> predst(row_num + 1, -1);
		int src;
		int dst;
		cout << "Input the Two Node to be Checked" << endl;
		cout << "Input Src:";
		cin >> src;
		cout << "Input Dst:";
		cin >> dst;
		QueryPerformanceCounter(&m_beginTime);//获取当前时间
		if (src == dst) {
			cout << "It can be accessed" << endl;
		}
		Queuedst.push(dst);
		Queuesrc.push(src);
		visited[dst] = 1;
		visited[src] = 2;
		while (!Queuedst.empty() || !Queuesrc.empty()) {
			if (Queuedst.size()> Queuesrc.size()) {
				int len = Queuedst.size();
				for (int i = 0; i != len; i++) {
					int temp = Queuedst.front();
					for (int j = row_start[temp]; j != row_start[temp + 1]; j++) {
						if (visited[node[j]] == 2) {
							predst[node[j]] = temp;
							cout << "It can be accessed" << endl;
							Print_Path(predst, presrc, node[j]);
							QueryPerformanceCounter(&m_endTime);//获取当前时间
							cout << (double)(m_endTime.QuadPart - m_beginTime.QuadPart) / m_nFreq.QuadPart << endl;
							goto start_;
						}
						if (visited[node[j]] == 0) {
							predst[node[j]] = temp;
							visited[node[j]] = 1;
							Queuedst.push(node[j]);
						}
					}
					Queuedst.pop();
				}
			}
			else {
				int len = Queuesrc.size();
				for (int i = 0; i != len; i++) {
					int temp = Queuesrc.front();
					for (int j = row_start[temp]; j != row_start[temp + 1]; j++) {
						if (visited[node[j]] == 1) {
							presrc[node[j]] = temp;
							cout << "It can be accessed" << endl;
							Print_Path(predst, presrc, node[j]);
							QueryPerformanceCounter(&m_endTime);//获取当前时间
							cout << (double)(m_endTime.QuadPart - m_beginTime.QuadPart) / m_nFreq.QuadPart << endl;
							goto start_;
						}
						if (visited[node[j]] == 0) {
							presrc[node[j]] = temp;
							visited[node[j]] = 2;
							Queuesrc.push(node[j]);
						}
					}
					Queuesrc.pop();
				}
			}
		}
		cout << "It cannot be accessed" << endl;
		QueryPerformanceCounter(&m_endTime);//获取当前时间
		cout << (double)(m_endTime.QuadPart - m_beginTime.QuadPart) / m_nFreq.QuadPart << endl;
		goto start_;
	}
	file.close();
}