#pragma once

#include <vector>
#include <stack>

namespace zzk {

	template <class T>
	class TreeNode{//节点类
		int m_bf;//平衡因子
		T m_data;//数据
		TreeNode<T>* m_left;//左孩子
		TreeNode<T>* m_right;//右孩子
		TreeNode<T>* m_parent;//父亲
	public:
		TreeNode(const T& val = T()) :
			m_bf(0),
			m_data(val),
			m_left(nullptr),
			m_right(nullptr),
			m_parent(nullptr)
		{}

		template <class T>
		friend class AVLTree;
	};

	template <class T>
	class AVLTree{//树类
		TreeNode<T>* m_root;//节点
		void destroy(TreeNode<T>* root){
			if (root){
				destroy(root->m_left);
				destroy(root->m_right);
				delete root;
			}
		}

		void lRound(TreeNode<T>* pre){//左旋 pre的平衡因子减2
			//左旋执行条件：pre->bf == 2 cur->bf == 1
			//左旋执行结果：pre->bf = 0 cur ->bf =0;
			//三步操作
			//三个基本节点pre、cur、cur->left
			//对pre进行左旋，使得cur成为新的根/新的pre
			//1.先将pre的父亲与cur连接，若parent不存在则pre为根，将cur置为新根
			//2.将cur的左孩子赋给pre的右孩子
			//3.将pre改为cur的左孩子  修改pre与cur的平衡因子为0
			TreeNode<T>* parent = pre->m_parent;
			TreeNode<T>* cur = pre->m_right;

			cur->m_parent = parent;
			if (parent)//1.begin
			{
				if (parent->m_left == pre)
				{
					parent->m_left = cur;//若pre时parent的左孩子将cur连在左孩子上
				}
				else
				{
					parent->m_right = cur;//若pre时parent的右孩子将cur连在右孩子上
				}
			}
			else
			{
				m_root = cur;
			}//1.end == 2.begin
			pre->m_right = cur->m_left;
			if (cur->m_left)
			{
				cur->m_left->m_parent = pre;
			}
			//2.end == 3.begin
			cur->m_left = pre;
			pre->m_parent = cur;
			
			cur->m_bf = pre->m_bf = 0;//3.end
		}

		void rRound(TreeNode<T>* pre){//右旋pre的平衡因子加2
			TreeNode<T>* parent = pre->m_parent; 
			TreeNode<T>* cur = pre->m_left;

			cur->m_parent = parent;
			if (parent)
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
				m_root = cur;
			}

			pre->m_left = cur->m_right;
			if (cur->m_right)
			{
				cur->m_right->m_parent = pre;
			}

			cur->m_right = pre;
			pre->m_parent = cur;

			cur->m_bf = pre->m_bf = 0;
		}

		void rlRound(TreeNode<T>* pre){//右左旋：将根右左，先进行右旋形成根右右，再左旋
			TreeNode<T>* right = pre->m_right;
			TreeNode<T>* newroot = right->m_left;

			int flag = newroot->m_bf;

			rRound(pre->m_right);
			lRound(pre);

			if (flag == -1)
			{
				right->m_bf = 1;
			}
			else if (flag == 1)
			{
				pre->m_bf = -1;
			}
		}

		void lrRound(TreeNode<T>* pre){//左右旋：根左右，先进行左旋形成根左左，再右旋
			TreeNode<T>* left = pre->m_left;
			TreeNode<T>* newroot = left->m_right;

			int flag = newroot->m_bf;

			lRound(pre->m_left);
			rRound(pre);

			if (flag == -1)
			{
				pre->m_bf = 1;
			}
			else if (flag == 1)
			{
				left->m_bf = -1;
			}
		}
	public:
		AVLTree() :
			m_root(nullptr)
		{}

		~AVLTree()
		{
			destroy(m_root);
		}

		bool insert(const T& val){//插入
			//1.树为空时插入
			if (m_root == nullptr)
			{
				m_root = new TreeNode<T>(val);
				return true;
			}//1.end

			TreeNode<T>* cur = m_root;
			TreeNode<T>* pre = nullptr;

			while (cur)
			{
				if (val < cur->m_data)//若是插入值小于已有值
				{
					pre = cur;//修改cur为pre
					cur = cur->m_left;//修改cur为pre的左孩子
				}
				else if (val > cur->m_data)
				{
					pre = cur;
					cur = cur->m_right;//修改cur为pre的右孩子
				}
				else
				{//插入值与已有值相等 插入失败！
					return false;
				}
			}
			//拿到要插入的节点位置cur
			
			cur = new TreeNode<T>(val);//插入节点cur
			if (val < pre->m_data)
			{
				pre->m_left = cur;
			}
			else
			{
				pre->m_right = cur;
			}

			cur->m_parent = pre;
			//将新节点与父节点相连接
			while (pre){//修改bf并检测bf，出现问题则旋转解决
				if (pre->m_left == cur)
				{//cur挂在左树上pre->bf-1
					pre->m_bf--;
				}
				else
				{//cur挂在右树上pre->bf+1
					pre->m_bf++;
				}
				//检查df是否满足条件
				if (pre->m_bf == 0)
				{
					break;//为0最平衡情况直接跳出
				}
				else if (pre->m_bf == 1 || pre->m_bf == -1)
				{
					cur = pre;
					pre = pre->m_parent;//向上检查pre的平衡因子
				}
				else//四种需要旋转的基本情况
				{
					if (pre->m_bf == 2)//根右
					{
						if (cur->m_bf == 1)
						{
							lRound(pre);//根右右
						}
						else
						{
							rlRound(pre);//根右左
						}
					}
					else//根左
					{
						if (cur->m_bf == 1)
						{
							lrRound(pre);//根左右
						}
						else
						{
							rRound(pre);//根左左
						}
					}
					break;
				}
			}
			return true;
		}

		bool erase(const T& val)
		{
			if (m_root == nullptr)
			{
				return false;
			}

			TreeNode<T>* cur = m_root;
			TreeNode<T>* pre = m_root;

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
					break;
				}
			}

			if (cur == nullptr)
			{
				return false;
			}

			if (cur->m_left && cur->m_right)
			{
				TreeNode<T>* cur2 = cur->m_left;
				TreeNode<T>* pre2 = cur;

				if (cur2->m_right)
				{
					for (; cur2->m_right; pre2 = cur2, cur2 = cur2->m_right);
					pre2->m_right = cur2->m_left;
					cur2->m_left = cur->m_left;
				}

				cur2->m_right = cur->m_right;

				if (cur == pre)
				{
					m_root = cur2;
				}
				else
				{
					if (cur->m_data < pre->m_data)
					{
						pre->m_left = cur2;
					}
					else
					{
						pre->m_right = cur2;
					}
				}

				delete cur;
			}
			else if (cur->m_left)
			{
				if (cur == pre)
				{
					m_root = cur->m_left;
				}
				else
				{
					if (cur->m_data < pre->m_data)
					{
						pre->m_left = cur->m_left;
					}
					else
					{
						pre->m_right = cur->m_left;
					}
				}
				delete cur;
			}
			else
			{
				if (cur == pre)
				{
					m_root = cur->m_right;
				}
				else
				{
					if (cur->m_data < pre->m_data)
					{
						pre->m_left = cur->m_right;
					}
					else
					{
						pre->m_right = cur->m_right;
					}
				}

				delete cur;
			}
		}

		std::vector<T> InOrder()
		{
			std::stack<TreeNode<T>*> s;
			std::vector<T> res;
			TreeNode<T>* cur = m_root;

			while (cur || !s.empty())
			{
				for (; cur; cur = cur->m_left)
				{
					s.push(cur);
				}

				if (!s.empty())
				{
					cur = s.top();
					res.push_back(cur->m_data);
					s.pop();

					cur = cur->m_right;
				}
			}

			return res;
		}
	};



};