#include<iostream>
#include<vector>
using namespace std;
inline void Swap(vector<int>& A, int i, int j) {
	int temp = A[i];
	A[i] = A[j];
	A[j] = temp;
}
class solution {
public:
	vector<int> Qsort(vector<int>& A, int b, int e) {
		if (b < e) {
			int m = Partition2(A, b, e);
			Qsort(A, b, m - 1);
			Qsort(A, m + 1, e);
		}
		return A;
	}
	int Partition(vector<int>& A, int b, int e) {
		int Axisvalue = A[e];
		int i = b - 1;
		for (int j = b; j < e; j++) {  
			//j遍历数组，找到小于轴值的就放在i的左边
			if (A[j] <= Axisvalue) {
				i = i + 1;
				Swap(A, i, j);
			}
		}
		Swap(A, i + 1, e);
		return i + 1;
	}
	int Partition2(vector<int>& A, int b, int e) {
		int Axisvalue = A[e];
		int i = b, j = e-1, flag = 0;
		int k;
		int axis = j + 1;
		while (i <= j) {
			if (flag == 0) { //从左向右扫描找第一个大于
				axis = j + 1;
				for (k = i; k <= j; k++) {
					if (A[k] >= Axisvalue) {
						Swap(A, k, j+1);
						axis = k;
						break;
					}
				}
				i = k + 1;
				flag = 1;
			}
			else {	//从右到左找第一个小于
				axis = i - 1;
				for (k = j; k >= i; k--) {
					if (A[k] <= Axisvalue) {
						Swap(A, k, i-1);
						axis = k;
						break;
					}
				}
				j = k - 1;
				flag = 0;
			}
		}
		return axis;
	}
};
int main() {
	vector<int> test = {1,1,1,1,1,1,1,1 };
	test = solution().Qsort(test, 0, test.size() - 1);
	for (int i = 0; i < 8; i++) {
		cout << test[i];
	}
	system("pause");
}