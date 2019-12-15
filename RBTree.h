#pragma once

namespace zzk{

enum Color {RED, BLACK};//颜色，红0黑1

template <class T>
class RBTreeNode//树节点类
{
	T m_data;//数据
	RBTreeNode<T> * m_left;//左孩子
	RBTreeNode<T> * m_right;//右孩子
	RBTreeNode<T> * m_parent;//父亲
	Color m_color;//节点颜色

public:
	RBTreeNode(const T & val = T(), Color color = RED):
		m_left(nullptr),
		m_right(nullptr),
		m_parent(nullptr),
		m_data(val),
		m_color(color)
	{}//构造，带参&默认

	template <class T>
	friend class RBTree;//友元类-树类
};

template <class T>
class RBTree//树类
{
	RBTreeNode<T> * m_head;//头节点

	void lRound(RBTreeNode<T> * pre)//左旋--与AVL树的旋转一致
    //颜色的改变在旋转后处理
	{
		RBTreeNode<T> * parent = pre->m_parent;
		RBTreeNode<T> * cur = pre->m_right;

		cur->m_parent = parent;
		if (parent != m_head)
		{
			if (parent->m_left == pre)
			{
				parent->m_left = cur;
			}
			else
			{
				parent->m_right = cur;
			}
		}
		else
		{
			m_head->m_parent = cur;
			cur->m_parent = m_head;
		}

		pre->m_right = cur->m_left;
		if (cur->m_left)
		{
			cur->m_left->m_parent = pre;
		}

		cur->m_left = pre;
		pre->m_parent = cur;
	}

	void rRound(RBTreeNode<T> * pre)//右旋--与AVL树的旋转一致
    ////颜色的改变在旋转后处理
	{
		RBTreeNode<T> * parent = pre->m_parent;
		RBTreeNode<T> * cur = pre->m_left;

		cur->m_parent = parent;
		if (parent != m_head)
		{
			if (parent->m_left == pre)
			{
				parent->m_left = cur;
			}
			else
			{
				parent->m_right = cur;
			}
		}
		else
		{
			m_head->m_parent = cur;
			cur->m_parent = m_head;
		}

		pre->m_left = cur->m_right;
		if (cur->m_right)
		{
			cur->m_right->m_parent = pre;
		}

		cur->m_right = pre;
		pre->m_parent = cur;
	}
public:
	RBTree()
	{
		m_head = new RBTreeNode < T > ;
	}

	RBTreeNode<T> * & getRoot()//获取头节点-
	{
		return m_head->m_parent;
	}

	RBTreeNode<T> * leftMost()//获取树的第一个节点为最左节点--val最小的
	{
		RBTreeNode<T> * cur = getRoot();

		for (; cur->m_left; cur = cur->m_left);
		return cur;
	}

	RBTreeNode<T> * rightMost()//获取树的第二个节点为最右节点--val最大的
	{
		RBTreeNode<T> * cur = getRoot();

		for (; cur->m_right; cur = cur->m_right);
		return cur;
	}

	bool insert(const T & val)//插入
	{
		RBTreeNode<T> * & root = getRoot();

		if (root)
		{
			RBTreeNode<T> * cur = root;
			RBTreeNode<T> * pre = nullptr;

			while (cur)
			{
				if (val < cur->m_data)
				{
					pre = cur;
					cur = cur->m_left;
				}
				else if (val > cur->m_data)
				{
					pre = cur;
					cur = cur->m_right;
				}
				else
				{
					return false;
				}
			}

			cur = new RBTreeNode<T>(val);
			if (val < pre->m_data)
			{
				pre->m_left = cur;
			}
			else
			{
				pre->m_right = cur;
			}

			cur->m_parent = pre;

			/*****************红黑树*****************/

			if (pre->m_color == RED)
			{
				RBTreeNode<T> * grand = pre->m_parent;
				RBTreeNode<T> * uncle;

				if (pre == grand->m_left)
				{
					while (pre != m_head && pre->m_color == RED)
					{
						grand = pre->m_parent;
						uncle = grand->m_right;

						if (uncle && uncle->m_color == RED) //第三种情况
						{
							pre->m_color = BLACK;
							uncle->m_color = BLACK;
							grand->m_color = RED;

							cur = grand;
							pre = cur->m_parent;
						}
						else
						{
							if (cur == pre->m_right) //第五种情况转成第四种
							{
								lRound(pre);
								RBTreeNode<T> * tmp;
								tmp = pre;
								pre = cur;
								cur = tmp;
							}

							rRound(grand);        //第四种情况
							pre->m_color = BLACK;
							grand->m_color = RED;
							break;
						}
					}
				}
				else /*************上下是镜像**************/
				{
					while (pre != m_head && pre->m_color == RED)
					{
						grand = pre->m_parent;
						uncle = grand->m_left;

						if (uncle && uncle->m_color == RED)
						{
							pre->m_color = BLACK;
							uncle->m_color = BLACK;
							grand->m_color = RED;

							cur = grand;
							pre = cur->m_parent;
						}
						else
						{
							if (cur == pre->m_left)
							{
								rRound(pre);
								RBTreeNode<T> * tmp;
								tmp = pre;
								pre = cur;
								cur = tmp;
							}

							lRound(grand);
							pre->m_color = BLACK;
							grand->m_color = RED;
							break;
						}
					}
				}
			}
			//省略的else是第二种情况
		}
		else //第一种情况
		{
			root = new RBTreeNode<T>(val, BLACK);
			root->m_parent = m_head;
			m_head->m_parent = root;
		}

		root->m_color = BLACK;
		m_head->m_left = leftMost();
		m_head->m_right = rightMost();
		return true;
	}

	RBTreeNode<T> * increasement(RBTreeNode<T> * cur)//++
	//++操作，寻找当前节点的后一个节点。
	//后一个节点：将所有节点排序后，找到当前节点，后一个节点就是顺序下的后一个节点
	{
		RBTreeNode<T> * tmp = cur;
		if (cur->m_right)
		{
			tmp = cur->m_right;
			for (; tmp->m_left; tmp = tmp->m_left);
		}
		else
		{
			tmp = tmp->m_parent;
			for (; cur != tmp->m_left && tmp != m_head; tmp = tmp->m_parent)
			{
				cur = tmp;
			}
			if (tmp == m_head)
			{
				return nullptr;
			}
		}
		return tmp;
	}

	RBTreeNode<T> *  decreasement(RBTreeNode<T> * cur)//--
	//与++操作类似
	{
		RBTreeNode<T> * tmp = cur;
		if (cur->m_left)
		{
			tmp = cur->m_left;
			for (; tmp->m_right; tmp = tmp->m_right);
		}
		else
		{
			tmp = tmp->m_parent;
			for (; cur != tmp->m_right && tmp != m_head; tmp = tmp->m_parent)
			{
				cur = tmp;
			}
			if (tmp == m_head)
			{
				return nullptr;
			}
		}
		return tmp;
	}

	void me_printf()//打印
	{
		RBTreeNode<T> * cur = m_head->m_right;
		while (cur)
		{
			cout << cur->m_data << ' ';
			cur = decreasement(cur);
		}
	}
};

};
