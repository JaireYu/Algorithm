#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include <cstdlib>
#include<windows.h>
using namespace std;
float besttime;				//�洢����ʱ��
vector<int> bestjob;		//������Ϊkey������Ϊvalue �洢����
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
		if (timetemp < besttime) {					//����Ҷ�ڵ㣬����besttime bestjob
			bestjob.assign(Job.begin(), Job.end());
			besttime = timetemp;
		}
		return;
	}	
	for (int j = 0; j != machine_num; j++) {
		Job[wait_assign[current_num]] = j;
		if (Calculate(weight, Job, wait_assign[current_num] + 1, machine_num) < besttime)//��֦�������besttimeС�Ž�����һ����չ�ڵ�
			Back_Track(Job, weight, wait_assign, current_num + 1, job_num, len, machine_num);
	}
	return;
}
void Back_Track_(vector<int>& Job, const vector<float>& weight,
	const int& current_num, const int& job_num, const int& machine_num) {
	if (current_num == job_num) {
		float timetemp = Calculate(weight, Job, job_num, machine_num);
		if (timetemp < besttime) {					//����Ҷ�ڵ㣬����besttime bestjob
			bestjob.assign(Job.begin(), Job.end());
			besttime = timetemp;
		}
		return;
	}
	for (int j = 0; j != machine_num; j++) {
		Job[current_num] = j;
		if (Calculate(weight, Job, current_num + 1, machine_num) < besttime)//��֦�������besttimeС�Ž�����һ����չ�ڵ�
			Back_Track_(Job, weight, current_num + 1, job_num, machine_num);
	}
	return;
}
void Schedule(vector<int> Job, const int& current_job_num, const int& current_machine_num, 
	const int& machine_num, const int& job_num, const vector<float>& weight) {
	//ѡ��machine_num������n��ȡk����Ϊ��ʼ���
	if (((machine_num - current_machine_num) > (job_num - current_job_num)) 
		|| (job_num - current_job_num) < 0)	//��ʱʣ��������Ѿ�����ʣ������ķ��䣬ǰ��δ������������(��Ч)
		return;
	else if (machine_num == current_machine_num && current_job_num <= job_num) { //�����жϱ߽��Ѿ��걸
		//�Ѿ��ҵ�machine_num�������������еĻ���(ʱ�临�Ӷ�ΪO(k(2n-k)))��
		//�����û���(O((n-k)^(n)))������δ���������
		vector<int> wait_assign;		//�������������
		for (int i = 0; i != job_num; i++) {
			if (Job[i] == INT_MAX)
				wait_assign.push_back(i);
		}
		int len = wait_assign.size();
		Back_Track(Job, weight, wait_assign, 0,job_num,  len, machine_num);
		return;
	}
	Schedule(Job, current_job_num + 1, current_machine_num, machine_num, job_num, weight); //��ǰ���񲻷Ż���
	Job[current_job_num] = current_machine_num;
	Schedule(Job, current_job_num + 1, current_machine_num + 1, machine_num, job_num, weight); //�Ż���
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
	QueryPerformanceFrequency(&m_nFreq); // ��ȡʱ������
	QueryPerformanceCounter(&m_beginTime);//��ȡ��ǰʱ��
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
	QueryPerformanceCounter(&m_beginTime);//��ȡ��ǰʱ��
	if (machine_num >= job_num) {	//����������࣬�������
		besttime = *max_element(weight.begin(), weight.end());
		for (int i = 0; i != job_num; i++) {
			bestjob.push_back(i);
		}
	}
	else {
		//��֪�����ǶԳƵ�, ������������û����������ѡ��machine_num�������������еĻ�����Ϊ��ʼ
		vector<int> Job(job_num, INT_MAX); //���INT_MAX����δ����
		besttime = float(INT_MAX);
		Schedule(Job, 0, 0, machine_num, job_num, weight);
	}
	QueryPerformanceCounter(&m_endTime);//��ȡ��ǰʱ��
	double runtime_2 = (double)(m_endTime.QuadPart - m_beginTime.QuadPart) / m_nFreq.QuadPart;
	Print_Out_Schedule_Plan(weight, job_num, machine_num);
	QueryPerformanceCounter(&m_beginTime);//��ȡ��ǰʱ��
	vector<int> Job(job_num, INT_MAX); //���INT_MAX����δ����
	besttime = float(INT_MAX);
	Back_Track_(Job, weight, 0, job_num, machine_num);
	QueryPerformanceCounter(&m_endTime);//��ȡ��ǰʱ��
	double runtime_1 = (double)(m_endTime.QuadPart - m_beginTime.QuadPart) / m_nFreq.QuadPart;
	Print_Out_Schedule_Plan(weight, job_num, machine_num);
	cout << "ԭʼ�㷨��ʱ��Ϊ: " << runtime_1 << "s" << endl;
	cout << "�Ľ��㷨��ʱ��Ϊ: " << runtime_2 << "s" << endl;
	system("pause");
}