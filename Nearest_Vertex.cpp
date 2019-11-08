#include<iostream>
#include<vector>
using namespace std;
typedef struct vertex{
	vertex(float a, float b){
		this->x = a;
		this->y = b;
	}
	float x;
	float y;
}vertex;
void Qsort(vector<vertex> vertexset){
}
int main(){
	int n;
	cout << "Type the number of vertex" << endl;
	cin >> n;
	vecter<vertex> vertexset;
	for(int i = 0; i != n; i++){
		int x,y;
		cin >> x;
		cin >> y;
		vertex V(x, y);
		vertexset.push_back(V);
	}
}
