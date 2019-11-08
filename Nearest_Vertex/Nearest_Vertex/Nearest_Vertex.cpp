#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<math.h>
#include<string>
using namespace std;
struct vertex {
	double x;
	double y;
};
struct BestPair {
	vertex a;
	vertex b;
	double Distance;
};
struct vertex_pos {
	vertex a;
	char pos;
};
inline void Swap(vector<vertex>& vertexset, int i, int j) {
	vertex temp = vertexset[i];
	vertexset[i] = vertexset[j];
	vertexset[j] = temp;
}
int Partition(vector<vertex>& vertexset, int b, int e, char x_or_y) {
	int i = b - 1;
	if (x_or_y == 'x') {
		double Axisvalue = vertexset[e].x;
		for (int j = b; j < e; j++) {
			//j遍历数组，找到小于轴值的就放在i的左边
			if (vertexset[j].x <= Axisvalue) {
				i = i + 1;
				Swap(vertexset, i, j);
			}
		}
		Swap(vertexset, i + 1, e);
	}
	else {
		double Axisvalue = vertexset[e].y;
		for (int j = b; j < e; j++) {
			//j遍历数组，找到小于轴值的就放在i的左边
			if (vertexset[j].y <= Axisvalue) {
				i = i + 1;
				Swap(vertexset, i, j);
			}
		}
		Swap(vertexset, i + 1, e);
	}
	return i + 1;
}
void Qsort(vector<vertex>& vertexset, int b, int e, char x_or_y) {
	if (b < e) {
		int m = Partition(vertexset, b, e, x_or_y);
		Qsort(vertexset, b, m - 1, x_or_y);
		Qsort(vertexset, m + 1, e, x_or_y);
	}
}
inline double distance(vertex a, vertex b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
BestPair Select_Min(const BestPair& a, const BestPair& b, const BestPair& c) {
	if (a.Distance <= b.Distance && a.Distance <= c.Distance) {
		return a;
	}
	else if (b.Distance <= a.Distance && b.Distance <= c.Distance) {
		return b;
	}
	else {
		return c;
	}
}
BestPair Select_Min(const BestPair& a, const BestPair& b) {
	if (a.Distance <= b.Distance) {
		return a;
	}
	else {
		return b;
	}
}
BestPair Merge(const vector<vertex_pos>& Mid_Place, const double& Delta) {
	double min_distance = Delta;
	vertex a{ INT_MAX, INT_MAX };
	vertex b{ INT_MAX, INT_MAX };
	BestPair Merge_Best{ a, b, INT_MAX };
	int n = Mid_Place.size();
	for (int i = 0; i != n; i++) {
		for (int j = i + 1; j < i + 8 && j < n; j++) {
			double potential = distance(Mid_Place[j].a, Mid_Place[i].a);
			if (potential < min_distance /*&& Mid_Place[j].pos != Mid_Place[i].pos*/) { //为什么有这句就不行？？
				Merge_Best.a = Mid_Place[i].a;
				Merge_Best.b = Mid_Place[j].a;
				Merge_Best.Distance = potential;
				min_distance = potential;
			}
		}
	}
	return Merge_Best;
}
BestPair BF_Get_Nearest_Vertex(const vector<vertex>& vertextset, int a, int b) {
	double min_distance = INT_MAX;
	vertex t1{ INT_MAX, INT_MAX };
	vertex t2{ INT_MAX, INT_MAX };
	BestPair t{ t1, t2, INT_MAX };
	for (int i = a; i != b; i++) {
		for (int j = i + 1; j < b; j++) {
			double temp = distance(vertextset[i], vertextset[j]);
			if (temp < min_distance) {
				t.a = vertextset[i];
				t.b = vertextset[j];
				t.Distance = temp;
				min_distance = temp;
			}
		}
	}
	return t;
}
BestPair Get_Nearest_Vertex(const vector<vertex>& vertexset, const vector<vertex>& vertexset_ysort,
	int begin, int end) {
	vertex a{ INT_MAX, INT_MAX };
	vertex b{ INT_MAX, INT_MAX };
	BestPair t{ a, b, INT_MAX };
	if (end - begin <= 3) {
		return BF_Get_Nearest_Vertex(vertexset, begin, end);
	}
	else {
		int Mid_Num = (begin + end) / 2;
		BestPair temp1 = Get_Nearest_Vertex(vertexset, vertexset_ysort, begin, Mid_Num);
		BestPair temp2 = Get_Nearest_Vertex(vertexset, vertexset_ysort, Mid_Num + 1, end);
		double Delta = min(temp1.Distance, temp2.Distance);
		double left = vertexset[Mid_Num + 1].x - Delta;	//计算左界
		double right = vertexset[Mid_Num].x + Delta;		//计算右界
		if (left <= vertexset[Mid_Num].x && right >= vertexset[Mid_Num + 1].x) {	//合理范围(剪枝，原算法的改进)
			vector<vertex_pos> Mid_Place;
			int n = vertexset.size();
			for (int i = 0; i != n; i++) {
				double x = vertexset_ysort[i].x;
				if (x <= right && x >= left) {
					if (x <= vertexset[Mid_Num].x) {
						vertex_pos temp{ vertexset_ysort[i], 'l' };
						Mid_Place.push_back(temp);
					}
					else {
						vertex_pos temp{ vertexset_ysort[i], 'r' };
						Mid_Place.push_back(temp);
					}
				}
			}
			BestPair temp3 = Merge(Mid_Place, Delta);
			return Select_Min(temp1, temp2, temp3);
		}
		else {
			return Select_Min(temp1, temp2);
		}
	}
}

int main() {
	for (int i = 1; i != 100; i++) {
		vector<vertex> vertexset;
		int n = 200;
		ifstream inputfile;
		inputfile.open("C:\\Users\\yujiarui\\Desktop\\" + to_string(i) + ".txt", ios::in);
		for (int i = 0; i != n; i++) {
			double x, y;
			inputfile >> x;
			inputfile >> y;
			vertex V{ x, y };
			vertexset.push_back(V);
		}
		Qsort(vertexset, 0, n - 1, 'x');
		vector<vertex> vertexset_ysort;
		vertexset_ysort.assign(vertexset.begin(), vertexset.end());
		Qsort(vertexset_ysort, 0, n - 1, 'y');
		BestPair Nearest_Vertex = Get_Nearest_Vertex(vertexset, vertexset_ysort, 0, n);
		BestPair compare = BF_Get_Nearest_Vertex(vertexset, 0, n);
		if (/*((compare.a.x != Nearest_Vertex.a.x || compare.a.y != Nearest_Vertex.a.y
			|| compare.b.x != Nearest_Vertex.b.x || compare.b.y != Nearest_Vertex.b.y) &&
			(compare.a.x != Nearest_Vertex.b.x || compare.a.y != Nearest_Vertex.b.y
				|| compare.b.x != Nearest_Vertex.a.x || compare.b.y != Nearest_Vertex.a.y))
			||*/ abs(compare.Distance - Nearest_Vertex.Distance) > 0.00001){ 
			cout << Nearest_Vertex.a.x << endl;
			cout << Nearest_Vertex.a.y << endl;
			cout << Nearest_Vertex.b.x << endl;
			cout << Nearest_Vertex.b.y << endl;
			cout << compare.a.x << endl;
			cout << compare.a.y << endl;
			cout << compare.b.x << endl;
			cout << compare.b.y << endl;
			cout << compare.Distance << endl;
			cout << Nearest_Vertex.Distance << endl;
		}
		else {
			cout << "TRUE" << endl;
			cout << Nearest_Vertex.a.x << endl;
			cout << Nearest_Vertex.a.y << endl;
			cout << Nearest_Vertex.b.x << endl;
			cout << Nearest_Vertex.b.y << endl;
			cout << compare.a.x << endl;
			cout << compare.a.y << endl;
			cout << compare.b.x << endl;
			cout << compare.b.y << endl;
			cout << compare.Distance << endl;
			cout << Nearest_Vertex.Distance << endl;
		}

	}
	system("pause");
	
}
/*
int main() {
	int n;
	cout << "Type the number of vertex" << endl;
	cin >> n;
	vector<vertex> vertexset;
	cout << "Input the vertexes:" << endl;
	for (int i = 0; i != n; i++) {
		double x, y;
		cin >> x;
		cin >> y;
		vertex V{ x, y };
		vertexset.push_back(V);
	}
	Qsort(vertexset, 0, n - 1, 'x');
	vector<vertex> vertexset_ysort;
	vertexset_ysort.assign(vertexset.begin(), vertexset.end());
	Qsort(vertexset_ysort, 0, n - 1, 'y');
	BestPair Nearest_Vertex = Get_Nearest_Vertex(vertexset, vertexset_ysort, 0, n);
	cout << "The nearest vertex pair is: ";
	cout << "(" << Nearest_Vertex.a.x << ", " << Nearest_Vertex.a.y << ")    ";
	cout << "(" << Nearest_Vertex.b.x << ", " << Nearest_Vertex.b.y << ")" << endl;
	cout << "The nearest distance is: "<< Nearest_Vertex.Distance << endl;
	system("pause");
}*/