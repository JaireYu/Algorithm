/*

�����

*/
#include <algorithm>
#include<iostream>
#include<vector>
#include <cstdlib>
#include <time.h>
using namespace std;
class Node {
	char color;
	int key;
	Node* left;
	Node* right;
	Node* p;
public:
	Node(int k, char c) {
		left = nullptr;
		right = nullptr;
		p = nullptr;
		color = c;
		key = k;
	}
	~Node() {}
	friend class Red_Black_Tree;
};
class Red_Black_Tree {
	Node* root;
	//Node nil{ 'b', 0, nullptr, nullptr }; //����������Ա����ֻ����ô����
	Node* nil = new Node{ -1, 'b' };	//***ע��: �����������NOde nil, ����ĳ�Ա������Ȼ�޷��޸�nil��ֵ�����п��ܸı����ַ
	int flag; //������Ƿ�Ӧ������
public:
	Red_Black_Tree() {
		root = nil;
	}
	Red_Black_Tree(vector<int> Init_Vector) {
		root = nil;
		flag = 1;
		int len = Init_Vector.size();
		for (int i = 0; i != len; i++) {
			Insert(Init_Vector[i]);
		}
	}
	~Red_Black_Tree() {	//Ϊʲôreturn *this ���Զ���������������
		if(flag == 1){
			DistoryTree(root);
			delete nil;
		}
		else {
			flag = 1;
		}
	}
	void DistoryTree(Node* root) {
		if (root != nil) {
			DistoryTree(root->left);
			DistoryTree(root->right);
			delete(root);
		}
	}
	Red_Black_Tree Insert(int k) {					//BST���벿���ۺ��˿������������������������ǰ����nil��ʼ��ͦ�ɵ�
		Node* insert = new Node{ k, 'r' };	//�½ڵ��ú�ɫ
		Node* former = nil;
		Node* latter = root;
		while (latter != nil) {
			former = latter;
			if (k < latter->key) {
				latter = latter->left;
			}
			else {
				latter = latter->right;
			}
		}
		if (former == nil) {	//������û��former�ڵ�
			root = insert;
		}
		else {	//���ǿ���������ýڵ�ĸ��ڵ�ָ����
			if (k < former->key) {					//ע���ж��ǲ�������ʱ������ʹ��latter == former->left, ��Ϊ���ܶ���nil�������
				former->left = insert;
			}
			else {
				former->right = insert;
			}
		}
		insert->p = former;
		insert->left = nil;
		insert->right = nil;
		RBInsertFixup(insert);
		flag = 0;
		return *this;
	}
	void RBInsertFixup(Node* z) {	//����Ϊ�����
		while (z->p->color == 'r') {	//�����z�Ǹ������, �Ǹ�������������ѭ���������ײ���rҲû��Ҫ����
			//���ص���z���游��nil, ��Ϊ��z���Ǹ�������Ǻڵģ�z->pһ�����Ǹ�
			if (z->p->p->left == z->p) {	//case1 2 3
				Node* y = z->p->p->right;	//y��z������
				if (y->color == 'r') {	//case1
					z->p->color = 'b';
					y->color = 'b';
					z = z->p->p;
					z->color = 'r';
				}
				else {
					if (z->p->right == z) {	//case2
						z = z->p;
						Left_Rotate(z);
					}
					if (z->p->left == z) {	//case3 ����ֱ�ӽ��룬Ҳ������Ϊcase2�ĳн�
						z->p->color = 'b';
						z->p->p->color = 'r';
						Right_Rotate(z->p->p);
					}
				}
			}
			else {	//case 4 5 6
				Node* y = z->p->p->left;	//z������
				if (y->color == 'r') {
					z->p->color = 'b';
					y->color = 'b';
					z = z->p->p;
					z->color = 'r';
				}
				else {
					if (z == z->p->left) {
						z = z->p;
						Right_Rotate(z);
					}
					if (z == z->p->right) {
						z->p->p->color = 'r';
						z->p->color = 'b';
						Left_Rotate(z->p->p);
					}
				}
			}
		}
		root->color = 'b';	//��ֹ����ĵ��Ǹ��ڵ㣬�����ú�ɫ����
	}
	Red_Black_Tree Pointer_Delete(Node* z) {
		Node* y;
		if (z->left == nil || z->right == nil) {
			y = z;
		}
		else {
			y = z->right;
			while (y->left != nil) {
				y = y->left;
			}
		}
		Node* x = (y->left != nil) ? y->left : y->right;	//node x ��y����һ��
		x->p = y->p;		//x�ĸ��ڵ���y�ĸ��ڵ�
		if (y->p == nil) {	//���y�Ǹ���y->p->left��right������
			root = x;
		}
		else {
			if (y == y->p->left) {
				y->p->left = x;
			}
			else {
				y->p->right = x;
			}
		}
		if (y != z) {
			z->key = y->key;
		}
		if (y->color == 'b') {
			RBDeleteFixup(x);
		}
		delete y;
		flag = 0;
		return *this;
	}
	Red_Black_Tree Key_Delete(int k) {
		Node* WaitToDelete = Find(k);
		if (WaitToDelete != nil) {
			Pointer_Delete(WaitToDelete);
		}
		return *this;
	}
	void RBDeleteFixup(Node* x) {
		while (x != root && x->color == 'b') {	//ֻҪx��ȻΪ˫�ڻ��߲��Ǹ��ͽ�����ȥ
			if (x == x->p->left) {	//case1 2 3 4
				Node* sib = x->p->right;
				if (sib->color == 'r') {//case1: ����ת�Ƶ������������������û�м�else
					sib->color = 'b';
					x->p->color = 'r';
					Left_Rotate(x->p);
					sib = x->p->right;
				}
				if (sib->left->color == 'b' && sib->right->color == 'b') { //case2��case2��������ѭ����������һ��while��ִ����case2��Ӧ���˳�
					sib->color = 'r';
					x = x->p;	//��x���x->pʱ��x->p�Զ�����һ���ڣ����B��B^2, x�Զ���ȥһ���ڣ�������˫��
				}
				else {   //case3��case4������ģ�����һ��else����,����case4����case3���漴��
					if (sib->right->color == 'b') { //case3
						sib->left->color = 'b';
						sib->color = 'r';
						Right_Rotate(sib);
						sib = x->p->right;
					}
					//case4
					sib->color = x->p->color;
					x->p->color = 'b';
					sib->right->color = 'b';
					Left_Rotate(x->p);
					x = root;
				}
			}
			else {	//case 5 6 7 8
				Node* sib = x->p->left;
				if (sib->color == 'r') {//case5: ����ת�Ƶ������������������û�м�else
					sib->color = 'b';
					x->p->color = 'r';
					Right_Rotate(x->p);
					sib = x->p->left;
				}
				if (sib->left->color == 'b' && sib->right->color == 'b') { //case6��case6��������ѭ����������һ��while��ִ����case6��Ӧ���˳�
					x = x->p;
					sib->color = 'r';
				}
				else {   //case7��case8������ģ�����һ��else����,����case8����case7���漴��
					if (sib->left->color == 'b') { //case7
						sib->color = 'r';
						sib->right->color = 'b';
						Left_Rotate(sib);
						sib = x->p->left;
					}
					//case8
					sib->color = x->p->color;
					x->p->color = 'b';
					sib->left->color = 'b';
					Right_Rotate(x->p);
					x = root;
				}
			}
		}
		x->color = 'b';	//�޸�case2�ĺ���������case4�ĸ������
	}
	void Left_Rotate(Node* x) {	//����x���Һ��Ӳ���nil,������xΪ������
		Node* y = x->right;		//step1����\beta���ӵ�x���Һ���
		x->right = y->left;
		if (y->left != nil) {	//nil�ڵ��p��ʵ������
			y->left->p = x;
		}
		y->p = x->p;			//step2����y���ӵ�x��p�ڵ�
		if (x->p == nil) {		//���x��root��x��parentû��left����
			root = y;
		}
		else if (x == x->p->left) {
			x->p->left = y;
		}
		else {
			x->p->right = y;
		}
		y->left = x;			//step3��ʹx��Ϊy������
		x->p = y;
	}
	void Right_Rotate(Node* x) {	//������xΪ������
		Node* y = x->left;
		x->left = y->right;
		if (y->right != nil) {
			y->right->p = x;
		}
		y->p = x->p;
		if (x->p == nil) {
			root = y;
		}
		else if (x->p->left == x) {
			x->p->left = y;
		}
		else {
			x->p->right = y;
		}
		y->right = x;
		x->p = y;
	}
	Node* Find(int k) {
		return Search(k, root);
	}
	Node* Search(int k, Node* root) {
		if (root != nil) {
			if (root->key == k) {
				return root;
			}
			else if (k < root->key) {
				return Search(k, root->left);
			}
			else {
				return Search(k, root->right);
			}
		}
		else {
			return root;
		}
	}
	void Paint_Tree() {
		cout << "digraph G {" << endl;
		cout << "node[shape = record, style = filled, color = black, fontcolor = white];" << endl;
		cout << "nil [label=\"<f0> | <f1> nil | <f2>\"]" << endl;
		if (root != nil)
			Paint(root);
		cout << "}" << endl;
	}
	void Paint(Node* curnode) {
		if (curnode->color == 'r')
			cout << curnode->key << "[label=\"<f0> |<fl> " << curnode->key << "|<f2>\" color = red]" << endl;
		else
			cout << curnode->key << "[label=\"<f0> |<fl> " << curnode->key << "|<f2>\"]" << endl;
		if (curnode->left == nil) {
			cout << curnode->key << ":f0:sw -> " << "nil" << ":f1" << endl;
		}
		else {
			Paint(curnode->left);
			cout << curnode->key << ":f0:sw -> " << curnode->left->key << ":f1" << endl;
		}
		if (curnode->right == nil) {
			cout << curnode->key << ":f2:sw -> " << "nil" << ":f1" << endl;
		}
		else {
			Paint(curnode->right);
			cout << curnode->key << ":f2:sw -> " << curnode->right->key << ":f1" << endl;
		}
	}
};



int main() {
	srand((unsigned)time(NULL));
	Red_Black_Tree RBT;
	int n;
	cin >> n;
	vector<int> Z;
	for (int i = 0; i < 1000; ++i){
		Z.push_back(i + 1);
	}

	random_shuffle(Z.begin(), Z.end());
	for (int i = 0; i != n; i++) {
		Z.push_back(rand() % n);
		RBT.Insert(Z[i]);
	}
	RBT.Paint_Tree();
	RBT.Key_Delete(Z[n / 2]);
	RBT.Key_Delete(Z[n / 4]);
	cout << "----------------------After delete-----------------------" << endl;
	RBT.Paint_Tree();
	RBT.Insert(10000).Key_Delete(1);
	RBT.Paint_Tree();
	system("pause");
}
/*

������

*/
/*
#include <algorithm>
#include<iostream>
#include<vector>
#include <cstdlib>
#include <time.h>
using namespace std;
class Node {
	char color;
	int key;
	int high;
	int max;
	Node* left;
	Node* right;
	Node* p;
public:
	Node(int k, int h, char c) {
		left = nullptr;
		right = nullptr;
		p = nullptr;
		color = c;
		key = k;
		high = h;
		max = h;
	}
	~Node() {}
	friend class Red_Black_Tree;
};
class Red_Black_Tree {
	Node* root;
	//Node nil{ 'b', 0, nullptr, nullptr }; //����������Ա����ֻ����ô����
	Node* nil = new Node{ -1, 0, 'b' };	//***ע��: �����������NOde nil, ����ĳ�Ա������Ȼ�޷��޸�nil��ֵ�����п��ܸı����ַ
	int flag; //������Ƿ�Ӧ������
public:
	Red_Black_Tree() {
		root = nil;
	}
	Red_Black_Tree(vector<int> Init_Vectork, vector<int> Init_Vectorh) {
		root = nil;
		flag = 1;
		int len = Init_Vectork.size();
		for (int i = 0; i != len; i++) {
			Insert(Init_Vectork[i], Init_Vectorh[i]);
		}
	}
	~Red_Black_Tree() {	//Ϊʲôreturn *this ���Զ���������������
		if (flag == 1) {
			DistoryTree(root);
			delete nil;
		}
		else {
			flag = 1;
		}
	}
	inline int MAX(int a, int b, int c) {
		if (a > b && a > c) {
			return a;
		}
		else if (b > a && b > c) {
			return b;
		}
		else {
			return c;
		}
	}
	void DistoryTree(Node* root) {
		if (root != nil) {
			DistoryTree(root->left);
			DistoryTree(root->right);
			delete(root);
		}
	}
	Red_Black_Tree Insert(int k, int h) {					//BST���벿���ۺ��˿������������������������ǰ����nil��ʼ��ͦ�ɵ�
		Node* insert = new Node{ k, h,'r' };	//�½ڵ��ú�ɫ
		Node* former = nil;
		Node* latter = root;
		while (latter != nil) {
			former = latter;
			latter->max = max(latter->max, insert->max);
			if (k < latter->key) {
				latter = latter->left;
			}
			else {
				latter = latter->right;
			}
		}
		if (former == nil) {	//������û��former�ڵ�
			root = insert;
		}
		else {	//���ǿ���������ýڵ�ĸ��ڵ�ָ����
			if (k < former->key) {					//ע���ж��ǲ�������ʱ������ʹ��latter == former->left, ��Ϊ���ܶ���nil�������
				former->left = insert;
			}
			else {
				former->right = insert;
			}
		}
		insert->p = former;
		insert->left = nil;
		insert->right = nil;
		RBInsertFixup(insert);
		flag = 0;
		return *this;
	}
	void RBInsertFixup(Node* z) {	//����Ϊ�����
		while (z->p->color == 'r') {	//�����z�Ǹ������, �Ǹ�������������ѭ���������ײ���rҲû��Ҫ����
			//���ص���z���游��nil, ��Ϊ��z���Ǹ�������Ǻڵģ�z->pһ�����Ǹ�
			if (z->p->p->left == z->p) {	//case1 2 3
				Node* y = z->p->p->right;	//y��z������
				if (y->color == 'r') {	//case1
					z->p->color = 'b';
					y->color = 'b';
					z = z->p->p;
					z->color = 'r';
				}
				else {
					if (z->p->right == z) {	//case2
						z = z->p;
						Left_Rotate(z);
					}
					if (z->p->left == z) {	//case3 ����ֱ�ӽ��룬Ҳ������Ϊcase2�ĳн�
						z->p->color = 'b';
						z->p->p->color = 'r';
						Right_Rotate(z->p->p);
					}
				}
			}
			else {	//case 4 5 6
				Node* y = z->p->p->left;	//z������
				if (y->color == 'r') {
					z->p->color = 'b';
					y->color = 'b';
					z = z->p->p;
					z->color = 'r';
				}
				else {
					if (z == z->p->left) {
						z = z->p;
						Right_Rotate(z);
					}
					if (z == z->p->right) {
						z->p->p->color = 'r';
						z->p->color = 'b';
						Left_Rotate(z->p->p);
					}
				}
			}
		}
		root->color = 'b';	//��ֹ����ĵ��Ǹ��ڵ㣬�����ú�ɫ����
	}
	Red_Black_Tree Pointer_Delete(Node* z) {
		Node* y;
		if (z->left == nil || z->right == nil) {
			y = z;
		}
		else {
			y = z->right;
			while (y->left != nil) {
				y = y->left;
			}
		}
		Node* x = (y->left != nil) ? y->left : y->right;	//node x ��y����һ��
		x->p = y->p;		//x�ĸ��ڵ���y�ĸ��ڵ�
		if (y->p == nil) {	//���y�Ǹ���y->p->left��right������
			root = x;
		}
		else {
			if (y == y->p->left) {
				y->p->left = x;
			}
			else {
				y->p->right = x;
			}
		}
		if (y != z) {
			z->key = y->key;
			z->high = y->high;
			z->max = MAX(z->high, z->left->max, z->right->max);
		}
		Node* g = y->p;
		while (g != nil)
		{
			g->max = MAX(g->max, g->left->max, g->right->max);
			g = g->p;
		}
		if (y->color == 'b') {
			RBDeleteFixup(x);
		}
		delete y;
		flag = 0;
		return *this;
	}
	Red_Black_Tree Key_Delete(int k) {
		Node* WaitToDelete = Find(k);
		if (WaitToDelete != nil) {
			Pointer_Delete(WaitToDelete);
		}
		return *this;
	}
	void RBDeleteFixup(Node* x) {
		while (x != root && x->color == 'b') {	//ֻҪx��ȻΪ˫�ڻ��߲��Ǹ��ͽ�����ȥ
			if (x == x->p->left) {	//case1 2 3 4
				Node* sib = x->p->right;
				if (sib->color == 'r') {//case1: ����ת�Ƶ������������������û�м�else
					sib->color = 'b';
					x->p->color = 'r';
					Left_Rotate(x->p);
					sib = x->p->right;
				}
				if (sib->left->color == 'b' && sib->right->color == 'b') { //case2��case2��������ѭ����������һ��while��ִ����case2��Ӧ���˳�
					sib->color = 'r';
					x = x->p;	//��x���x->pʱ��x->p�Զ�����һ���ڣ����B��B^2, x�Զ���ȥһ���ڣ�������˫��
				}
				else {   //case3��case4������ģ�����һ��else����,����case4����case3���漴��
					if (sib->right->color == 'b') { //case3
						sib->left->color = 'b';
						sib->color = 'r';
						Right_Rotate(sib);
						sib = x->p->right;
					}
					//case4
					sib->color = x->p->color;
					x->p->color = 'b';
					sib->right->color = 'b';
					Left_Rotate(x->p);
					x = root;
				}
			}
			else {	//case 5 6 7 8
				Node* sib = x->p->left;
				if (sib->color == 'r') {//case5: ����ת�Ƶ������������������û�м�else
					sib->color = 'b';
					x->p->color = 'r';
					Right_Rotate(x->p);
					sib = x->p->left;
				}
				if (sib->left->color == 'b' && sib->right->color == 'b') { //case6��case6��������ѭ����������һ��while��ִ����case6��Ӧ���˳�
					x = x->p;
					sib->color = 'r';
				}
				else {   //case7��case8������ģ�����һ��else����,����case8����case7���漴��
					if (sib->left->color == 'b') { //case7
						sib->color = 'r';
						sib->right->color = 'b';
						Left_Rotate(sib);
						sib = x->p->left;
					}
					//case8
					sib->color = x->p->color;
					x->p->color = 'b';
					sib->left->color = 'b';
					Right_Rotate(x->p);
					x = root;
				}
			}
		}
		x->color = 'b';	//�޸�case2�ĺ���������case4�ĸ������
	}
	void Left_Rotate(Node* x) {	//����x���Һ��Ӳ���nil,������xΪ������
		Node* y = x->right;		//step1����\beta���ӵ�x���Һ���
		x->right = y->left;
		if (y->left != nil) {	//nil�ڵ��p��ʵ������	
			y->left->p = x;
		}
		y->p = x->p;			//step2����y���ӵ�x��p�ڵ�
		if (x->p == nil) {		//���x��root��x��parentû��left����
			root = y;
		}
		else if (x == x->p->left) {
			x->p->left = y;
		}
		else {
			x->p->right = y;
		}
		y->left = x;			//step3��ʹx��Ϊy������
		x->p = y;
		x->max = MAX(x->high, x->right->max, x->left->max);
		y->max = MAX(y->high, y->right->max, y->left->max);
	}
	void Right_Rotate(Node* x) {	//������xΪ������
		Node* y = x->left;
		x->left = y->right;
		if (y->right != nil) {
			y->right->p = x;
		}
		y->p = x->p;
		if (x->p == nil) {
			root = y;
		}
		else if (x->p->left == x) {
			x->p->left = y;
		}
		else {
			x->p->right = y;
		}
		y->right = x;
		x->p = y;
		x->max = MAX(x->high, x->right->max, x->left->max);
		y->max = MAX(y->high, y->right->max, y->left->max);
	}
	Node* Find(int k) {
		return Search(k, root);
	}
	Node* Search(int k, Node* root) {
		if (root != nil) {
			if (root->key == k) {
				return root;
			}
			else if (k < root->key) {
				return Search(k, root->left);
			}
			else {
				return Search(k, root->right);
			}
		}
		else {
			return root;
		}
	}
	bool Overlap(int l, int h, int tk, int th) {
		if (tk > h) {
			return false;
		}
		else if (l > th) {
			return false;
		}
		return true;
	}
	void Find_Overlap(int k, int h) {
		Node* p = root;
		while (p != nil && !Overlap(p->key, p->high, k, h)) {
			if (p->left != nil && p->left->max >= k)
				p = p->left;
			else
				p = p->right;
		}
		if (p == nil)
			cout << "Not found" << endl;
		else
			cout << "Find overlap: [" << p->key << ", " << p->high << "]" << endl;
	}
	void Paint_Tree() {
		cout << "digraph G {" << endl;
		cout << "node[shape = record, style = filled, color = black, fontcolor = white];" << endl;
		cout << "nil [label=\"<f0> | <f1> nil | <f2>\"]" << endl;
		if (root != nil)
			Paint(root);
		cout << "}" << endl;
	}
	void print(Node* curnode) {
		cout << "\"[" << curnode->key << ", " << curnode->high << "]" << "/max: " << curnode->max << "\"";
	}
	void print_1(Node* curnode) {
		cout << "\\\"[" << curnode->key << ", " << curnode->high << "]" << "/max: " << curnode->max << "\\\"";
	}
	void Paint(Node* curnode) {
		if (curnode->color == 'r') {
			print(curnode);
			cout << "[label=\"<f0> |<fl> ";
			print_1(curnode);
			cout << "|<f2>\" color = red]" << endl;
		}

		else {
			print(curnode);
			cout << "[label=\"<f0> |<fl> ";
			print_1(curnode);
			cout << "|<f2>\"]" << endl;
		}

		if (curnode->left == nil) {
			print(curnode);
			cout << ":f0:sw -> " << "nil" << ":f1" << endl;
		}
		else {
			Paint(curnode->left);
			print(curnode);
			cout << ":f0:sw -> ";
			print(curnode->left);
			cout << ":f1" << endl;
		}
		if (curnode->right == nil) {
			print(curnode);
			cout << ":f2:sw -> " << "nil" << ":f1" << endl;
		}
		else {
			Paint(curnode->right);
			print(curnode);
			cout << ":f2:sw -> ";
			print(curnode->right);
			cout << ":f1" << endl;
		}
	}
};



int main() {
	srand((unsigned)time(NULL));
	Red_Black_Tree RBT;
	int n;
	cin >> n;
	vector<int> Z;
	for (int i = 0; i < 500; ++i) {
		Z.push_back(i + 1);
	}

	random_shuffle(Z.begin(), Z.end());
	for (int i = 0; i != n; i++) {
		RBT.Insert(Z[i], rand()%(499-Z[i]) + Z[i]);
	}
	RBT.Paint_Tree();
	cout << "----------------------After delete-----------------------" << endl;
	RBT.Key_Delete(Z[n/2]);
	RBT.Key_Delete(Z[n/4]);
	RBT.Find_Overlap(18, 22);
	RBT.Paint_Tree();
	system("pause");
}*/