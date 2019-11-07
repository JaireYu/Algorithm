#include<iostream>
#include<vector>
#include<map>
#include<Algothrim>
using namespace std;
int Calculate(vector<int> weight, vector<int> Job, int assign, int n, int k){
	vector<int> time(k, 0);	//n is the num of schedule assigned, and k is the number of machine
	for(int i=0; i != n; i ++){
		time[Job[i]] += weight[Job[i]]; 
	}
	return *max(time.begin(), time.end());
}
int main(){
//n岔排列树
	int job_num;
	int machine_num;
	vector<int> weight;
	cout<<"Enter the number of job"<<endl;
	cin>> job_num >>endl;
	cout<<"Enter the number of machine"<< endl;
	cin>> machine_num >>endl;
	cout << "Enter the weight of the job" << endl;
	for(int i=0; i != job_num; i ++){
		cin >> weight[i]; 
	}
	if(machine_num >= job_num){
		return *max(weight.begin(), weight.end());
	}
	else{
		//考虑在job中选取machine个组成全排列
		//在此基础上考虑回溯
	}
}

