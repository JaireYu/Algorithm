#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include <cstdlib>
#include<windows.h>
using namespace std;
float besttime;				//存储最优时间
vector<int> bestjob;		//以任务为key，机器为value 存储调度
float Calculate(vector<float> weight, vector<int> Job, int job_num, int machine_num) {
	vector<float> time(machine_num, 0);	
	for (int i = 0; i != job_num; i++) {
		if(Job[i] != INT_MAX)
			time[Job[i]] += weight[i];
	}
	return *max_element(time.begin(), time.end());
}
void Back_Track(vector<int>& Job, const vector<float>& weight, const vector<int>& wait_assign,
	const int& current_num, const int& job_num, const int& len, const int& machine_num) {
	if (current_num == len) {
		float timetemp = Calculate(weight, Job, job_num, machine_num);
		if (timetemp < besttime) {					//到达叶节点，更新besttime bestjob
			bestjob.assign(Job.begin(), Job.end());
			besttime = timetemp;
		}
		return;
	}	
	for (int j = 0; j != machine_num; j++) {
		Job[wait_assign[current_num]] = j;
		if (Calculate(weight, Job, wait_assign[current_num] + 1, machine_num) < besttime)//剪枝，如果比besttime小才进入下一个扩展节点
			Back_Track(Job, weight, wait_assign, current_num + 1, job_num, len, machine_num);
	}
	return;
}
void Back_Track_(vector<int>& Job, const vector<float>& weight,
	const int& current_num, const int& job_num, const int& machine_num) {
	if (current_num == job_num) {
		float timetemp = Calculate(weight, Job, job_num, machine_num);
		if (timetemp < besttime) {					//到达叶节点，更新besttime bestjob
			bestjob.assign(Job.begin(), Job.end());
			besttime = timetemp;
		}
		return;
	}
	for (int j = 0; j != machine_num; j++) {
		Job[current_num] = j;
		if (Calculate(weight, Job, current_num + 1, machine_num) < besttime)//剪枝，如果比besttime小才进入下一个扩展节点
			Back_Track_(Job, weight, current_num + 1, job_num, machine_num);
	}
	return;
}
void Schedule(vector<int> Job, const int& current_job_num, const int& current_machine_num, 
	const int& machine_num, const int& job_num, const vector<float>& weight) {
	//选择machine_num个任务n中取k个作为初始情况
	if (((machine_num - current_machine_num) > (job_num - current_job_num)) 
		|| (job_num - current_job_num) < 0)	//此时剩余的任务已经不够剩余机器的分配，前面未分配的任务过多(无效)
		return;
	else if (machine_num == current_machine_num && current_job_num <= job_num) { //两个判断边界已经完备
		//已经找到machine_num个任务分配个所有的机器(时间复杂度为O(k(2n-k)))，
		//下面用回溯(O((n-k)^(n)))来分配未分配的任务
		vector<int> wait_assign;		//待分配的任务编号
		for (int i = 0; i != job_num; i++) {
			if (Job[i] == INT_MAX)
				wait_assign.push_back(i);
		}
		int len = wait_assign.size();
		Back_Track(Job, weight, wait_assign, 0,job_num,  len, machine_num);
		return;
	}
	Schedule(Job, current_job_num + 1, current_machine_num, machine_num, job_num, weight); //当前任务不放机器
	Job[current_job_num] = current_machine_num;
	Schedule(Job, current_job_num + 1, current_machine_num + 1, machine_num, job_num, weight); //放机器
	return;
}
void Print_Out_Schedule_Plan(const vector<float>& weight, const int& job_num, const int& machine_num) {
	vector<float> TimeOfMachine(machine_num, 0);
	for (int i = 0; i != job_num; i++) {
		cout << "Begin " << i << "th task on " << bestjob[i] <<"th machine at time "<< TimeOfMachine[bestjob[i]] << endl;
		TimeOfMachine[bestjob[i]] += weight[i];
	}
	cout << "The total time is " << besttime << endl;
	return;
}
int main() {
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_endTime;
	LARGE_INTEGER m_beginTime;
	QueryPerformanceFrequency(&m_nFreq); // 获取时钟周期
	QueryPerformanceCounter(&m_beginTime);//获取当前时间
	int job_num;
	int machine_num;
	cout << "Enter the number of job" << endl;
	cin >> job_num;
	cout << "Enter the number of machine" << endl;
	cin >> machine_num;
	cout << "Enter the weight of the job" << endl;
	vector<float> weight(job_num);
	for (int i = 0; i != job_num; i++) {
		cin >> weight[i];
	}
	QueryPerformanceCounter(&m_beginTime);//获取当前时间
	if (machine_num >= job_num) {	//如果机器更多，无需调度
		besttime = *max_element(weight.begin(), weight.end());
		for (int i = 0; i != job_num; i++) {
			bestjob.push_back(i);
		}
	}
	else {
		//易知机器是对称的, 对于任务来讲没有区别，首先选出machine_num个任务分配给所有的机器作为初始
		vector<int> Job(job_num, INT_MAX); //标记INT_MAX代表未分配
		besttime = float(INT_MAX);
		Schedule(Job, 0, 0, machine_num, job_num, weight);
	}
	QueryPerformanceCounter(&m_endTime);//获取当前时间
	double runtime_2 = (double)(m_endTime.QuadPart - m_beginTime.QuadPart) / m_nFreq.QuadPart;
	Print_Out_Schedule_Plan(weight, job_num, machine_num);
	QueryPerformanceCounter(&m_beginTime);//获取当前时间
	vector<int> Job(job_num, INT_MAX); //标记INT_MAX代表未分配
	besttime = float(INT_MAX);
	Back_Track_(Job, weight, 0, job_num, machine_num);
	QueryPerformanceCounter(&m_endTime);//获取当前时间
	double runtime_1 = (double)(m_endTime.QuadPart - m_beginTime.QuadPart) / m_nFreq.QuadPart;
	Print_Out_Schedule_Plan(weight, job_num, machine_num);
	cout << "原始算法的时间为: " << runtime_1 << "s" << endl;
	cout << "改进算法的时间为: " << runtime_2 << "s" << endl;
	system("pause");
}