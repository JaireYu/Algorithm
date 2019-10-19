#include<iostream>
#include<vector>
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
	//Node nil{ 'b', 0, nullptr, nullptr }; //多个参数类成员变量只能这么声明
	Node nil = Node{ -1, 'b' };
public:
	Red_Black_Tree() {
		root = &nil;
	}
	Red_Black_Tree(vector<int> Init_Vector) {
		int len = Init_Vector.size();
		for (int i = 0; i != len; i++) {
			Insert(Init_Vector[i]);
		}
	}
	~Red_Black_Tree() {
		DistoryTree(root);
		delete &nil;
	}
	void DistoryTree(Node* root) {
		if (root != &nil) {
			DistoryTree(root->left);
			DistoryTree(root->right);
			delete(root);
		}
	}
	Red_Black_Tree Insert(int k) {					//BST插入部分综合了空树和正常树的两种情况，让前驱从nil开始，挺巧的
		Node* insert = new Node{ k, 'r' };	//新节点置红色
		Node* former = &nil;
		Node* latter = root;
		while (latter != &nil) {
			former = latter;
			if (k < latter->key) {
				latter = latter->left;
			}
			else {
				latter = latter->right;
			}
		}
		if (former == &nil) {	//空树，没有former节点
			root = insert;
		}
		else {	//不是空树，处理该节点的父节点指针域
			if (latter == former->left) {
				former->left = insert;
			}
			else {
				former->right = insert;
			}
		}
		insert->p = former;
		insert->left = &nil;
		insert->right = &nil;
		RBInsertFixup(insert);
		return (*this);
	}
	void RBInsertFixup(Node* z) {	//调整为红黑树
		while (z->p->color == 'r') {	//最后处理z是根的情况, 是根的情况不会进入循环，若父亲不是r也没必要调整
			//不必担心z的祖父是nil, 因为若z不是根，则根是黑的，z->p一定不是根
			if (z->p->p->left == z->p) {	//case1 2 3
				Node* y = z->p->p->right;	//y是z的叔叔
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
					if (z->p->left == z) {	//case3 可以直接进入，也可以作为case2的承接
						z->p->color = 'b';
						z->p->p->color = 'r';
						Right_Rotate(z->p->p);
					}
				}
			}
			else {	//case 4 5 6
				Node* y = z->p->p->left;	//z的叔叔
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
			root->color = 'b';	//防止插入的点是根节点，将根用黑色覆盖
		}
	}
	Red_Black_Tree Delete(Node* z) {
		Node* y;
		if (z->left == &nil || z->right == &nil) {
			y = z;
		}
		else {
			y = z->right;
			while (y->left != &nil) {
				y = y->left;
			}
		}
		Node* x = (y->left != &nil) ? y->left : y->right;	//node x 是y的下一个
		x->p = y->p;		//x的父节点变成y的父节点
		if (y->p == &nil) {	//如果y是根，y->p->left，right无意义
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
		return *this;
	}
	void RBDeleteFixup(Node* x) {
		while (x != root || x->color == 'b') {	//只要x仍然为双黑或者不是根就进行下去
			if (x == x->p->left) {	//case1 2 3 4
				Node* sib = x->p->right;
				if (sib->color == 'r') {//case1: 不会转移到自身，所以下面的条件没有加else
					sib->color = 'b';
					x->p->color == 'r';
					Right_Rotate(x->p);
					sib = x->p->right;
				}
				if (sib->left->color == 'b' && sib->right->color == 'b') { //case2：case2可以自我循环，所以在一个while内执行完case2就应该退出
					sib->color = 'r';
					x = x->p;	//当x变成x->p时，x->p自动加上一个黑，变成B或B^2, x自动减去一个黑，不再是双黑
				}
				else {   //case3，case4是连贯的，放在一个else块中,而且case4放在case3后面即可
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
				if (sib->color == 'r') {//case5: 不会转移到自身，所以下面的条件没有加else
					sib->color = 'b';
					x->p->color = 'r';
					Right_Rotate(x->p);
					sib = x->p->left;
				}
				if (sib->left->color == 'b' && sib->right->color) { //case6：case6可以自我循环，所以在一个while内执行完case6就应该退出
					x = x->p;
					sib->color = 'r';
				}
				else {   //case7，case8是连贯的，放在一个else块中,而且case8放在case7后面即可
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
		x->color = 'b';	//修改case2的红黑情况，和case4的根红情况
	}
	void Left_Rotate(Node* x) {	//假设x的右孩子不是nil,左旋以x为根的树
		Node* y = x->right;		//step1：将\beta连接到x的右孩子
		x->right = y->left;
		if (y->left != &nil) {	//nil节点的p无实际意义	
			y->left->p = x;
		}
		y->p = x->p;			//step2：将y连接到x的p节点
		if (x->p == &nil) {		//如果x是root，x的parent没有left子域
			root = y;
		}
		else if (x == x->p->left) {
			x->p->left = y;
		}
		else {
			x->p->right = y;
		}
		y->left = x;			//step3：使x成为y的左孩子
		x->p = y;
	}
	void Right_Rotate(Node* x) {	//右旋以x为根的树
		Node* y = x->left;
		x->left = y->right;
		if (y->right != &nil) {
			y->right->p = x;
		}
		y->p = x->p;
		if (x->p == &nil) {
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
};
int main() {

}