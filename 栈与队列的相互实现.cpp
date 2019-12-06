#include <iostream>
#include <queue>
#include <stack>
using namespace std;
//用队列实现栈
template<class T>
class queueStack
{
	queue<T> m_qu1;
public:
	queueStack()
	{

	}

	void push(T x)//插入方式不用改变
	{
		m_qu1.push(x);
	}

	void pop()//删除方式改变为尾删
	{
		queue<T> m_qu2;//临时队列
		while (m_qu1.size() > 1)
		{
			m_qu2.push(m_qu1.front());//将原队列的头元素放到临时队列中
			m_qu1.pop();//将原队列头元素删掉（头删）
			//循环两步操作直到只剩最后一个待删除元素为止
		}
		m_qu1 = m_qu2;//将临时队列赋给原队列，覆盖掉待删除元素
	}

	T top()//取栈顶元素
	{
		queue<T> m_qu2;//临时队列
		while (m_qu1.size() > 1)
		{
			m_qu2.push(m_qu1.front());//将原队列的头元素放到临时队列中
			m_qu1.pop();//将原队列头元素删掉（头删）
			//循环两步操作直到只剩最后一个待删除元素为止
		}
		T tmp = m_qu1.front();//此时原队列只剩一个元素即栈顶元素，记录该值
		m_qu2.push(m_qu1.front());//将最后一个元素入临时队
		m_qu1 = m_qu2;//将临时队赋给原队列
		return tmp;//返回栈顶
	}
};
template<class T>
class stackQueue
{
	stack<T> m_s1;
	stack<T> m_s2;
public:
	stackQueue()
	{

	}

	void push(T i)//插入操作一致
	{
		m_s1.push(i);
	}

	void pop()//删除改为头删
	{
		while (m_s1.size() > 1)
		{
			m_s2.push(m_s1.top());//取原栈栈顶入临时栈
			m_s1.pop();//删除原栈栈顶（尾删）
            //循环两步操作直到只剩最后一个待删除元素为止
		}
		m_s1.pop();//将原栈中的待删除元素删掉

		while (!m_s2.empty())
		{
			m_s1.push(m_s2.top());//临时栈栈顶入原栈
			m_s2.pop();//删除临时栈栈顶（尾删）
            //循环两步操作到临时栈为空
		}
	}

	T front()//取队头
	{
		while (m_s1.size() > 1)
		{
			m_s2.push(m_s1.top());//取原栈栈顶入临时栈
			m_s1.pop();//删除原栈栈顶（尾删）
            //循环两步操作直到只剩最后一个待删除元素为止
		}
		T tmp = m_s1.top();//记录原栈中的 最后一个元素 == 队头元素

		while (!m_s2.empty())
		{
			m_s1.push(m_s2.top());//临时栈栈顶入原栈
			m_s2.pop();//删除临时栈栈顶（尾删）
            //循环两步操作到临时栈为空
		}

		return tmp;//返回队头元素
	}

	T back()//取队尾
	{
		return m_s1.top();//队尾 == 栈顶
	}
};

int main()
{   //测试
    queueStack<int> qs;

	qs.push(1);
	qs.push(2);
	qs.push(3);
	qs.push(4);

	cout << qs.top() << endl;
	qs.pop();
	cout << qs.top() << endl;
	qs.pop();
	cout << qs.top() << endl;
	qs.pop();
	cout << qs.top() << endl;
	qs.pop();

	stackQueue<int> sq;

	sq.push(2);
	sq.push(3);
	sq.push(4);

	cout << sq.front() << endl;
	sq.pop();
	cout << sq.front() << endl;
	sq.pop();
	cout << sq.front() << endl;
	sq.pop();

	return 0;
}