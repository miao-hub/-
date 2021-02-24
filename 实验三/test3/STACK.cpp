#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

class QUEUE
{
	int* const elems;//elems�����ڴ����ڴ�Ŷ��е�Ԫ��
	const int max;//elems��������Ԫ�ظ���Ϊmax
	int head, tail;//����ͷhead��βtail���ӿ�tail=head����ʼhead=tail=0��
public:
	QUEUE(int m);//��ʼ�����У��������m��Ԫ��
	QUEUE(const QUEUE& q);//��q�����ʼ������
	QUEUE(QUEUE&& q)noexcept;//��q�ƶ���ʼ������
	virtual operator int()const noexcept;//���ض��е�ʵ��Ԫ�ظ���
	virtual int size()const noexcept;//���ض�����������Ԫ�ظ���max
	virtual QUEUE& operator<<(int e);//��e�����β���������ص�ǰ����
	virtual QUEUE& operator>>(int& e);//�Ӷ��׳�Ԫ�ص�e�������ص�ǰ����
	virtual QUEUE& operator=(const QUEUE& q);//�����ֵ�����ر���ֵ����
	virtual QUEUE& operator=(QUEUE&& q)noexcept;//�ƶ���ֵ�����ر���ֵ����
	virtual char* print(char* s)const noexcept;//��ӡ������s������s
	virtual ~QUEUE();//���ٵ�ǰ����
};
class STACK :public QUEUE
{
	QUEUE q;
public:
	STACK(int m);//��ʼ��ջ�������2m-2��Ԫ��
	STACK(const STACK& s);//��ջs�����ʼ��ջ
	STACK(STACK&& s)noexcept;//��ջs�ƶ�������ʼ��ջ
	int size()const noexcept;//����ջ��������2m
	operator int()const noexcept;//����ջ��ʵ��Ԫ�ظ���
	STACK& operator<<(int e);//��e��ջ�������ص�ǰջ
	STACK& operator>>(int& e);//��ջ��e�������ص�ǰջ
	STACK& operator=(const STACK& s);//�����ֵ�����ر���ֵջ
	STACK& operator=(STACK&& s)noexcept;//�ƶ���ֵ�����ر���ֵջ
	char* print(char* b)const noexcept;//��ջ�׵�ջ����ӡջԪ��
	~STACK()noexcept;//����ջ
};
extern const char* TestSTACK(int& s);
int main()
{
	int s;
	const char* c = TestSTACK(s);
	cout << c << endl;
	cout << s << endl;
	return 0;
}
QUEUE::QUEUE(int m) :max(m), elems(new int[m])//��ʼ������
{
	head = tail = 0;
}

QUEUE::QUEUE(const QUEUE& q) : max(q.max), elems(new int[q.max])//�����ʼ������
{
	for (int i = q.head; i != q.tail; i++, i = i % q.max)
		*(int*)&elems[i] = q.elems[i];
	head = q.head;
	tail = q.tail;
}

QUEUE::QUEUE(QUEUE&& q)noexcept :max(q.max), elems(q.elems)//�ƶ���ֵ��ʼ������
{
	*(int**)&q.elems = 0;
	*(int*)&q.max = 0;
	head = q.head;
	q.head = 0;
	tail = q.tail;
	q.tail = 0;
}

QUEUE::operator int()const noexcept//���ض����е�Ԫ�ظ���
{
	if (head == tail)
		return 0;
	return (tail + max - head) % max;
}

int QUEUE::size()const noexcept//���ض����е��������Ԫ�ظ���
{
	return max;
}

QUEUE& QUEUE::operator<<(int e)//��Ӳ���
{
	if ((tail + 1) % max == head)
		throw "QUEUE is full!";
	elems[tail] = e;
	tail = (tail + 1) % max;
	return *this;
}

QUEUE& QUEUE::operator>>(int& e)//���Ӳ���
{
	if (head == tail)
		throw "QUEUE is empty!";
	e = elems[head];
	head = (head + 1) % max;
	return *this;
}

QUEUE& QUEUE::operator=(const QUEUE& q)//�����ֵ
{
	if (this == &q)
		return *this;
	*(int**)&elems = (int*)malloc(sizeof(int) * q.max);
	for (int i = q.head; i != q.tail; i++, i = i % q.max)
		elems[i] = q.elems[i];
	*(int*)&max = q.max;
	head = q.head;
	tail = q.tail;
	return *this;
}

QUEUE& QUEUE::operator=(QUEUE&& q)noexcept//�ƶ���ֵ
{
	if (this == &q)
		return *this;
	if (elems)
		delete[]elems;
	*(int**)&elems = q.elems;
	*(int**)&q.elems = 0;
	*(int*)&max = q.max;
	*(int*)&q.max = 0;
	head = q.head;
	q.head = 0;
	tail = q.tail;
	q.tail = 0;
	return *this;
}

char* QUEUE::print(char* s)const noexcept//��ӡ�����е�Ԫ�ص��ַ�����
{
	int i, j = 0, elem;
	for (i = head; i != tail; i++, i = i % max)
	{
		sprintf(s + j, "%d ", elems[i]);
		elem = elems[i];
		while (elem)
		{
			j++;
			elem = elem / 10;
		};
		j++;
	}
	s[j] = 0;
	return s;
}

QUEUE::~QUEUE()//��������
{
	if (elems)
		delete elems;//�ȵ��������������ͷſռ�
	*(int**)&elems = 0;
	*(int*)&max = 0;
	head = tail = 0;
}
STACK::STACK(int m):q(m),QUEUE(m)//��ʼ��ջ�������2m-2��Ԫ��
{

}

STACK::STACK(const STACK& s):q(s.q),QUEUE(s)//��ջs�����ʼ��ջ
{

}

STACK::STACK(STACK&& s)noexcept :q(move(s.q)), QUEUE(move(s))//��ջs�ƶ�������ʼ��ջ
{

}

int STACK::size()const noexcept//����ջ��������2m
{
	return (q.size() + QUEUE::size());
}

STACK::operator int()const noexcept//����ջ��ʵ��Ԫ�ظ���
{
	return (int(q) + QUEUE::operator int());
}

STACK& STACK::operator<<(int e)//��e��ջ�������ص�ǰջ
{
	if (QUEUE::operator int() < QUEUE::size() - 1)
		QUEUE::operator<<(e);
	else if (int(q) == q.size() - 1)
		throw "STACK is full!";
	else
		q << e;
	return *this;
}

STACK& STACK::operator>>(int& e)//��ջ��e�������ص�ǰջ
{
	int i;
	if (int(q))
	{
		for (i = 0; i<int(q) - 1; i++)
		{
			q >> e;
			q << e;
		}
		q >> e;
	}
	else if (QUEUE::operator int())
	{
		for (i = 0; i < QUEUE::operator int() - 1; i++)
		{
			QUEUE::operator>>(e);
			QUEUE::operator<<(e);
		}
		QUEUE::operator>>(e);
	}
	else
		throw "STACK is empty!";
	return *this;
}

STACK& STACK::operator=(const STACK& s)//�����ֵ�����ر���ֵջ
{
	if (this == &s)
		return *this;
	q = s.q;
	*(QUEUE*)this = s;
	return *this;
}

STACK& STACK::operator=(STACK&& s)noexcept//�ƶ���ֵ�����ر���ֵջ
{
	if (this == &s)
		return *this;
	q = (QUEUE&&)s.q;
	*(QUEUE*)this = (QUEUE&&)s;
	return *this;
}
char* STACK::print(char* b)const noexcept//��ջ�׵�ջ����ӡջԪ��
{
	char* b1=new char[1000];
	q.print(b1);
	QUEUE::print(b);
	if (b1)
	{
		strcat(b, " \0");
		strcat(b, b1);
	}
	return b;
}

STACK::~STACK()noexcept//����ջ
{

}