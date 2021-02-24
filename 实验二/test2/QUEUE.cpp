#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <malloc.h>
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
extern const char* TestQUEUE(int& s);
int main()
{
	int s = 0;
	const char *c = TestQUEUE(s);
	cout << s << endl;
	cout << c << endl;
	return 0;
}

QUEUE::QUEUE(int m):max(m),elems(new int[m])//��ʼ������
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
	*(int **)&q.elems = 0;
	*(int *)&q.max = 0;
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
	*(int **)&elems = (int *)malloc(sizeof(int)*q.max);
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
		delete []elems;
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
	int i, k, m, data;
	char c[1000];
	m = 0;
	for (i = head; i != tail; i++, i = i % max)
	{
		data = elems[i];
		for (k = 0;; k++)
		{
			c[k] = data % 10 + '0';
			data = data / 10;
			if (data == 0)
				break;
		}
		for (; k >= 0; k--, m++)
			s[m] = c[k];
		s[m++] = ' ';
	}
	s[m] = '\0';
	return s;
}

QUEUE::~QUEUE()//��������
{
	if (elems)
		delete elems;//�ȵ��������������ͷſռ�
	*(int **)&elems = 0;
	*(int*)&max = 0;
	head = tail = 0;
}