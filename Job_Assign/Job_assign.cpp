#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
inline void SWAP(int i, int j, vector<int>& seq) {
	int temp = seq[i];
	seq[i] = seq[j];
	seq[j] = temp;
	return; 
}
bool Compare_with_best(float& best, const vector<int>& sequence, int flag, const int & current_pos,const vector<vector<float>>& weight){
	//flag是best更新标志位
	float current = 0;
	for(int i=0; i != current_pos; i ++){
		current += weight[sequence[i]][i];
		if (current >= best) {
			return false;
		}
	}
	if(flag == 1)
		best = current;
	return true;
} 
void Back_Track(vector<int>& sequence, const int & n, vector<int>& bestseq, float & besttime, const int& current, const vector<vector<float>> weight){
	if(current == n){
		if (Compare_with_best(besttime, sequence, 1 , n, weight) == true) {
			bestseq.assign(sequence.begin(), sequence.end());
		}
		return;
	}
	else {
		for (int i = current; i != n; i++) {
			SWAP(current, i, sequence);
			if (Compare_with_best(besttime, sequence, 0, n, weight) == true)
				Back_Track(sequence, n, bestseq, besttime, current + 1, weight);
			SWAP(current, i, sequence);
		}
	}
	return;
}
int main(){
	int n;
	cin >> n;
	float besttime = INFINITY;
	int current = 0;
	vector<vector<float>> weight;
	for (int i = 0; i != n; i++) {
		vector<float>  temp(n, 0);
		for (int j = 0; j != n; j++) {
			cin >> temp[j];
		}
		weight.push_back(temp);
	}
	vector<int> sequence(n, 0);
	for (int i = 0; i != n; i++) {
		sequence[i] = i;
	}
	vector<int> bestsequence;
	bestsequence.assign(sequence.begin(), sequence.end());
	Back_Track(sequence, n, bestsequence, besttime, current, weight);
	for (int i = 0; i != n; i++) {
		cout << "The "<< i <<"th person should do the " << bestsequence[i] << "th job" << endl;
	}
	system("pause");
}
