#pragma once
#include <stack>
#include <queue>

namespace zzk{

template < class T >
class TreeNode
{
	T m_val;

	TreeNode<T> * m_left;
	TreeNode<T> * m_right;
public:
	TreeNode(const T &val) :
		m_val(val)
	{

	}

	template <class T>
	friend class Tree;
};

template <class T>
class Tree
{
	TreeNode<T> * m_root;

	static TreeNode<T> *  maketree(const T * val, const T & end, int &i)
	{//使用数组制树，遇到end为空，从val的i号元素开始制树
		if (val[i] == end)
		{
			i++;
			return nullptr;
		}

		TreeNode<T> * root = new TreeNode<T>(val[i]);
		i++;

		root->m_left = maketree(val, end, i);
		root->m_right = maketree(val, end, i);

		return root;
	}
public:
	Tree() :
		m_root(nullptr)
	{

	}

	Tree(const T * val, const T & end)
	{
		int i = 0;
		m_root = maketree(val, end, i);
	}

	void pre_order()//非递归型二叉树前序遍历
	{
        //打印根节点
        //有右孩子，则入栈
        //有左孩子，则进入（将左孩子作为新根）
        //没左孩子取栈顶为新根，栈顶为空则跳出
		TreeNode<T> * cur = m_root;//当前根
		stack<TreeNode<T> *> st;//记录栈

		while (cur)//当前根不为空则一直进行
		{
			cout << cur->m_val << ' ';//打印根

			if (cur->m_right)//有右孩子
			{
				st.push(cur->m_right);//右孩子入栈
			}

			if (cur->m_left)//有左孩子
			{
				cur = cur->m_left;//左孩子入栈
			}
			else//没有左孩子
			{
				if (st.empty())//栈顶为空
				{
					cur = nullptr;//置当前根为空
				}
				else//栈顶不为空
				{
					cur = st.top();//取栈顶
					st.pop();//删除栈顶
				}
			}
		}
	}

	void in_order()//中序遍历
	{
		TreeNode<T> * cur = m_root;
		stack<TreeNode<T> *> st;
			//左孩子入栈，进入左孩子，直到最左为止
		while (cur || !st.empty())
		{
			for (; cur; cur = cur->m_left)
			{
				st.push(cur);
			}

				cur = st.top();//取栈顶
				st.pop();//删栈顶
				cout << cur->m_val << ' ';//打印栈顶(左孩子)

				cur = cur->m_right;
				//没有右孩子则取下一个栈顶
				//有右孩子则进入右孩子，将右孩子及其所有左孩子都入栈
		}
	}

	void post_order()//后序遍历
	{
		//1.从当前节点开始遍历左子树，将所有的节点入栈，并且置各节点tag为0，直到左子树为空
		//2.访问栈顶节点，置其tag为1，访问它的右孩子，若右孩子存在则再次以右孩子为cur，入栈所有左孩子（重复1号过程）
		//3.循环取出栈顶元素，若tag为1则取出打印，直到遇到栈顶元素的tag为0时跳出，访问2号过程
		TreeNode<T> * cur = m_root;
		stack<TreeNode<T> *> st;//节点栈
		stack<bool> tag;//标记栈，1代表要打印，0代表不打印

		while (cur || !st.empty())
		{
			for (; cur; cur = cur->m_left)
			{
				st.push(cur);
				tag.push(false);
			}//遍历至树的最左孩子，遍历过的每个节点都入栈，并且对应的标记栈都置为0
			//找cur节点的所有左孩子（左孩子的左孩子的……）
			while (!st.empty() && tag.top())//如果栈不为空且栈顶节点对应标记为1进入
			{
				cur = st.top();//取栈顶
				cout << cur->m_val << ' ';//打印栈顶
				st.pop();
				tag.pop();
				//删除已打印节点
				cur = nullptr;//置已打印节点为空
			}
			//打印节点栈中有的节点且该节点的tag被置为1的节点
			if (!st.empty())
			{
				tag.top() = true;//置栈顶为可打印状态
				cur = st.top();//取栈顶
				cur = cur->m_right;//取栈顶右孩子
			}
			//置节点的tag为可打印状态1，随后进入该节点的右孩子
		}
	}
	void level_order()//层序遍历
	{
		//根据队列的特点，先入先出，依次从左到右从上到下入队所有节点，并且取出打印即可
		queue<TreeNode<T> *> qu;
		TreeNode<T> * tmp;

		qu.push(m_root);//根节点入队

		while (!qu.empty())//队列为空时跳出
		{
			tmp = qu.front();//取队头
			cout << tmp->m_val << ' ';//打印
			qu.pop();//删队头

			if (tmp->m_left)//如果队头有左孩子
			{
				qu.push(tmp->m_left);//左孩子入队
			}

			if (tmp->m_right)//有右孩子
			{
				qu.push(tmp->m_right);//右孩子入队
			}
		}
	}

	void everyline_level_order()
	{
		//层序遍历按行打印每一层
		queue<TreeNode<T> *> qu;
		TreeNode<T> * tmp;
		int length = 1;

		qu.push(m_root);

		while (!qu.empty())
		{
			for (int i = 0; i < length; i++)
			{ 
				tmp = qu.front();
				cout << tmp->m_val << ' ';
				qu.pop();

				if (tmp->m_left)
				{
					qu.push(tmp->m_left);
				}

				if (tmp->m_right)
				{
					qu.push(tmp->m_right);
				}
			}
			cout << endl;
			length = qu.size();
		}
	}

	void max_level_order()
	{//层序遍历打印每行最大值
		queue<TreeNode<T> *> qu;
		TreeNode<T> * tmp;
		int length = 1;
		T max;
		
		qu.push(m_root);

		while (!qu.empty())
		{
			max = qu.front()->m_val;
			for (int i = 0; i < length; i++)
			{
				tmp = qu.front();
				
				if (max < tmp->m_val)
				{
					max = tmp->m_val;
				}
				qu.pop();

				if (tmp->m_left)
				{
					qu.push(tmp->m_left);
				}

				if (tmp->m_right)
				{
					qu.push(tmp->m_right);
				}
			}
			cout << max << ' ';
			length = qu.size();
		}
	}

};



};

