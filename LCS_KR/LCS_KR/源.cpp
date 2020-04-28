#include<iostream>
#include<vector>
#include<string>
#include<math.h>
#include <cstdlib>
#include<windows.h>
using namespace std;
struct Node {
	int arrow;
	int Pathlen;
};
class Solution_LCS {
public:
	string longestCommonSubsequence(string text1, string text2) {
		int len1 = text1.size();
		int len2 = text2.size();
		Node sample{ 0,0 };
		vector<vector<Node>> Path;
		for (int i = 0; i != len2 + 1; i++) {
			vector<Node> Sample;
			for (int j = 0; j != len1 + 1; j++) {
				Node test{ 0, 0 };
				Sample.push_back(test);
			}
			Path.push_back(Sample);
		}
		for (int i = 1; i != len2 + 1; i++) {
			for (int j = 1; j != len1 + 1; j++) {
				if (text2[i - 1] == text1[j - 1]) {
					Path[i][j].arrow = 1;
					Path[i][j].Pathlen = Path[i - 1][j - 1].Pathlen + 1;
				}
				else {
					if (Path[i - 1][j].Pathlen < Path[i][j - 1].Pathlen) {
						Path[i][j].arrow = 0; //point left
						Path[i][j].Pathlen = Path[i][j - 1].Pathlen;
					}
					else {
						Path[i][j].arrow = 2;
						Path[i][j].Pathlen = Path[i - 1][j].Pathlen;
					}
				}
			}
		}
		string result("");
		int i = len2; int j = len1;
		while (i != 0 && j != 0) {
			if (Path[i][j].arrow == 1) {
				result.push_back(text1[j - 1]);
				i = i - 1;
				j = j - 1;
			}
			else if (Path[i][j].arrow == 0) {
				j = j - 1;
			}
			else {
				i = i - 1;
			}
		}
		return result;
	}
};
class Solution_KR {
public:
	vector<int> strStr(string haystack, string needle) {
		vector<int> pos;
		size_t hsize = haystack.size();
		size_t nsize = needle.size();
		if (hsize < nsize)
			return pos;
		size_t hashneedlenum = 0;
		size_t hashstacknum = 0;
		for (int i = 0; i != nsize; i++) {
			hashstacknum = (hashstacknum << 1) + char(haystack[i]);
			hashneedlenum = (hashneedlenum << 1) + char(needle[i]);
		}
		if (hashstacknum == hashneedlenum && haystack.substr(0, nsize) == needle) {
			pos.push_back(0);
		}
		for (int i = 0; i != hsize - nsize; i++) {
			hashstacknum = (hashstacknum << 1) - (haystack[i] << nsize) + haystack[i + nsize];
			if (hashstacknum == hashneedlenum && haystack.substr(i + 1, nsize) == needle) {
				pos.push_back(i + 1);
			}
		}
		return pos;
	}
};
class Solution_KR_BF {
public:
	vector<int> strStr(string haystack, string needle) {
		vector<int> pos;
		size_t hsize = haystack.size();
		size_t nsize = needle.size();
		if (hsize < nsize)
			return pos;
		size_t hashneedlenum = 0;
		size_t hashstacknum = 0;
		for (int i = 0; i != nsize; i++) {
			hashstacknum = (hashstacknum << 1) + char(haystack[i]);
			hashneedlenum = (hashneedlenum << 1) + char(needle[i]);
		}
		if (hashstacknum == hashneedlenum && haystack.substr(0, nsize) == needle) {
			pos.push_back(0);
		}
		for (int i = 0; i != hsize - nsize; i++) {
			hashstacknum = hashstacknum *2 - haystack[i]*pow(2, nsize) + haystack[i + nsize];
			if (hashstacknum == hashneedlenum && haystack.substr(i + 1, nsize) == needle) {
				pos.push_back(i + 1);
			}
		}
		return pos;
	}
};
int main() {
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_endTime;
	LARGE_INTEGER m_beginTime;
	QueryPerformanceFrequency(&m_nFreq); // 获取时钟周期
	string LCS1;
	string LCS2;
	cin >> LCS1;
	cin >> LCS2;
	QueryPerformanceCounter(&m_beginTime);//获取当前时间
	string LCS = Solution_LCS().longestCommonSubsequence(LCS1, LCS2);
	reverse(LCS.begin(), LCS.end());
	cout << LCS;
	QueryPerformanceCounter(&m_endTime);//获取当前时间
	double runtime_2 = (double)(m_endTime.QuadPart - m_beginTime.QuadPart) / m_nFreq.QuadPart;
	string LCS3;
	string LCS4;
	cin >> LCS3;
	cin >> LCS4;
	QueryPerformanceCounter(&m_beginTime);//获取当前时间
	int len = Solution_KR_BF().strStr(LCS3, LCS4).size();
	for (int i = 0; i != len; i++) {
		cout << Solution_KR_BF().strStr(LCS3, LCS4)[i] << endl;
	}
	QueryPerformanceCounter(&m_endTime);//获取当前时间
	double runtime_0 = (double)(m_endTime.QuadPart - m_beginTime.QuadPart) / m_nFreq.QuadPart;
	QueryPerformanceCounter(&m_beginTime);//获取当前时间
	len = Solution_KR().strStr(LCS3, LCS4).size();
	for (int i = 0; i != len; i++) {
		cout << Solution_KR().strStr(LCS3, LCS4)[i] << endl;
	}
	QueryPerformanceCounter(&m_endTime);//获取当前时间
	double runtime_1 = (double)(m_endTime.QuadPart - m_beginTime.QuadPart) / m_nFreq.QuadPart;
	cout << "LCStime is " << runtime_2 << "s" << endl;
	cout << "KRtime with bitwise operation " << runtime_1 << "s" << endl;
	cout << "KRtime without bitwise operation  " << runtime_0 << "s" << endl;
	system("pause");
}