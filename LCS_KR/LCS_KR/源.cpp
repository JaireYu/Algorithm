#include<iostream>
#include<vector>
#include<string>
using namespace std;
struct Node {
	int arrow;
	int Pathlen;
};
class Solution_LCS {
public:
	int longestCommonSubsequence(string text1, string text2) {
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
		return result.size();
	}
};
class Solution_KR {
public:
	int strStr(string haystack, string needle) {
		size_t hsize = haystack.size();
		size_t nsize = needle.size();
		if (hsize < nsize)
			return -1;
		vector<int> pos;
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
		return pos.size() == 0 ? -1 : pos[0];
	}
};