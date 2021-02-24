#define _CRT_SECURE_NO_WARNINGS 
#include <iomanip> 
#include <exception>
#include <typeinfo>
#include <string.h>
#include <iostream>
using namespace std;
template <typename T>
class MAT {
	T* const e;							//ָ���������;���Ԫ�ص�ָ��
	const int r, c;						//�������r����c��С
public:
	MAT(int r, int c);				//������
	MAT(const MAT& a);				//�������
	MAT(MAT&& a)noexcept;			//�ƶ�����
	virtual ~MAT()noexcept;
	virtual T* const operator[ ](int r);//ȡ����r�еĵ�һ��Ԫ�ص�ַ��rԽ�����쳣
	virtual MAT operator+(const MAT& a)const;	//����ӷ������ܼ����쳣
	virtual MAT operator-(const MAT& a)const;	//������������ܼ����쳣
	virtual MAT operator*(const MAT& a)const;	//����˷������ܳ����쳣
	virtual MAT operator~()const;					//����ת��
	virtual MAT& operator=(const MAT& a);		//�����ֵ����
	virtual MAT& operator=(MAT&& a)noexcept;	//�ƶ���ֵ����
	virtual MAT& operator+=(const MAT& a);		//��+=������
	virtual MAT& operator-=(const MAT& a);		//��-=������
	virtual MAT& operator*=(const MAT& a);		//��*=������
//print�����s������s�����ÿո���������ûس�����
	virtual char* print(char* s)const noexcept;
};

template MAT<int>;			//����ʵ����
template MAT<long long>;		//����ʵ����
extern const char* TestMAT(int & s);	//����ʵ����
int main()
{
	int s1=0;
	const char* c1 = TestMAT(s1);
	cout << c1 << endl;
	cout << s1 << endl;
	return 0;	
}

template <typename T>
MAT<T>::MAT(int r, int c):r(r),c(c),e(new T[r*c])//������
{
	for (int m = 0; m < r; m++)
		for (int n = 0; n < c; n++)
			e[m * c + n] = 0;
}

template <typename T>
MAT<T>::MAT(const MAT& a):r(a.r),c(a.c),e(new T[a.r*a.c]) //�������
{
	for (int m = 0; m < r; m++)
		for (int n = 0; n < c; n++)
			e[m*c+n] = a.e[m*c+n];
}

template <typename T>
MAT<T >::MAT(MAT&& a)noexcept:r(a.r),c(a.c),e(a.e)//�ƶ�����
{
	*(int *)&a.r = 0;
	*(int *)&a.c = 0;
	*(T **)&a.e = 0;
}

template <typename T>
MAT<T>:: ~MAT()noexcept
{
	if (e)
	{
		delete e;
		*(T **)&e = 0;
	}
	*(int *)&r = 0;
	*(int *)&c = 0;
}

template <typename T>
T* const MAT<T>::operator[ ](int r)//ȡ����r�еĵ�һ��Ԫ�ص�ַ��rԽ�����쳣
{
	if (r >= (this->r) || r < 0)
		throw "wrong!";
	return &e[r*c];
}

template <typename T>
MAT<T> MAT<T>::operator+(const MAT& a)const	//����ӷ������ܼ����쳣
{
	if (!(r == a.r && c == a.c))
		throw "wrong!";
	MAT<T> *b=new MAT<T>(r, c);
	for (int m=0; m < r; m++)
		for (int n=0; n < c; n++)
			(*b)[m][n] = e[m*c+n] + a.e[m*c+n];
	return *b;
}

template <typename T>
MAT<T> MAT<T>::operator-(const MAT& a)const	//������������ܼ����쳣
{
	if (!(r == a.r && c == a.c))
		throw "wrong!";
	MAT<T> *b=new MAT<T>(r, c);
	for (int m=0; m < r; m++)
		for (int n=0; n < c; n++)
			(*b)[m][n] = e[m*c+n] - a.e[m*c+n];
	return *b;
}

template <typename T>
MAT<T> MAT<T>::operator*(const MAT& a)const	//����˷������ܳ����쳣
{
	if (c != a.r)
		throw "wrong!";
	MAT<T> *b=new MAT<T>(r, a.c);
	for (int i = 0; i < r; i++)
		for (int j = 0; j < a.c; j++)
			for (int n = 0; n < c; n++)
				(*b)[i][j] += e[i*c+n] * a.e[n*a.c+j];
	return *b;
}

template <typename T>
MAT<T> MAT<T>:: operator~()const					//����ת��
{
	MAT<T> *a=new MAT<T>(c, r);
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			(*a)[j][i] = e[i*c+j];
	return *a;
}

template <typename T>
MAT<T>& MAT<T>::operator=(const MAT& a)		//�����ֵ����
{
	if (this == &a)
		return *this;
	if (!(r == a.r && c == a.c))
		throw "wrong!";
	*(int *)&r = a.r;
	*(int *)&c = a.c;
	if (e)
		delete []e;
	*(T **)&e = new T[r * c];
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			e[i*c+j] = a.e[i*c+j];
	return *this;
}

template <typename T>
MAT<T>& MAT<T>::operator=(MAT&& a)noexcept	//�ƶ���ֵ����
{
	if (this == &a)
		return *this;
	if (e)
		delete []e;
	*(int *)&r = a.r;
	*(int *)&a.r = 0;
	*(int *)&c = a.c;
	*(int *)&a.c = 0;
	*(T **)&e = a.e;
	*(T **)&a.e = 0;
	return *this;
}

template <typename T>
MAT<T>& MAT<T>::operator+=(const MAT& a)		//��+=������
{
	if (!(r == a.r && c == a.c))
		throw "wrong!";
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			e[i * c + j] += a.e[i * c + j];
	return *this;
}

template <typename T>
MAT<T>& MAT<T>::operator-=(const MAT& a)		//��-=������
{
	if (!(r == a.r && c == a.c))
		throw "wrong!";
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			e[i*c+j] -= a.e[i*c+j];
	return *this;
}

template <typename T>
MAT<T>& MAT<T>::operator*=(const MAT& a)		//��*=������
{
	if (c!=a.r)
		throw "wrong!";
	*this = *this * a;
	return *this;
}

template <typename T>
char* MAT<T>::print(char* s)const noexcept
{
	int m = 0;
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
			if (!strcmp(typeid(e[0]).name(), "int"))
			{
				sprintf(s + m, "%6d ", e[i * c + j]);
				m += 7;
			}
			else if (!strcmp(typeid(e[0]).name(), "long long"))
			{
				sprintf(s + m, "%6ld ", e[i * c + j]);
				m += 7;
			}
			else if(!strcmp(typeid(e[0]).name(),"double"))
			{
				sprintf(s + m, "%8lf ", e[i * c + j]);
				m += 9;
			}
		s[m-1] = '\n';
	}
	s[m] = '\0';
	return s;
}
