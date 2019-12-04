#include <iostream>
#include <ctime>
#include "AVLTree.h"
using namespace std;

int main()
{//测试
	zzk::AVLTree<int> t;
	
	t.insert(16);
	t.insert(3);
	t.insert(7);
	t.insert(11);
	t.insert(9);
	t.insert(26);
	t.insert(18);
	t.insert(14);
	t.insert(15);

	//t.insert(4);
	//t.insert(2);
	//t.insert(6);
	//t.insert(1);
	//t.insert(3);
	//t.insert(5);
	//t.insert(15);
	//t.insert(7);
	//t.insert(16);
	//t.insert(14);

	vector<int> v = t.InOrder();

	for (auto& i : v)
	{
		cout << i << ' ';
	}
	return 0;
}