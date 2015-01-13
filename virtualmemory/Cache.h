#pragma once
#include<bitset>
#include<fstream>
#include<iostream>
using namespace std;
//author @�������
//time 2013-11-6��
//studentID 121220028��
//V4.0��
//˵������������һ����������cache��д����ֱ������ʱ����ǰ�ÿ���е�������Ҳ��·����Ϊ1��
//��ȫ����ʱ����ǰ�������Ϊ1,Ҳ��·���͵�������
//Ϊ���������洢����ϵͳ����2.0�汾��cache�Ļ����������޸ģ����������ݲ����Ƴ���cache��ֻ��Ϊһ��cache�ɽ�����������
//�һ���������¼tag�Ĵ洢�ṹ���������µ�����ʹ�ó�����Ӽ��
class Cache
{
	unsigned long int **tag;//���ö���ά�����ʾtag array
	int **use;//��¼ʹ��ʱ����Ƿ���dirty bit
	//int associativity;//ӳ�䷽ʽ��1Ϊֱ��ӳ�䣬0Ϊȫ��������������Ϊ��������������
	bool replacepolicy;//�滻���ԣ�trueΪ����滻��falseΪLRU�㷨��
	bool writepolicy;//д���ԣ�trueΪwrite through�� falseΪwrite back��
	//cache��С����

	int cachelinesize;//��С���õ�ַ��bit������ʾ
	int cachesize;

	int numberofsetbit;//ȫ������Ϊ0��
	int numberoflinebit;//ֱ��������Ϊ0��

	void random(unsigned long int *tag, int *use, unsigned long int add);//����滻����//��һ���е�tag���벢�޸��滻
	void LRU(unsigned long int *tag, int *use, unsigned long int add);//LRU�滻����
public:
	Cache();
	Cache(int associativity,bool replacepolicy, bool writepolicy, int cachesize, int cachelinesize);
	~Cache();
	void printTagarray(ofstream &out);
	bool access(unsigned long int add, char lors);//��ȡ���ʣ�hit����true��miss��ִ���滻���Բ�����false��
	void updata(unsigned long int pagenumber);//�������ҳ������
	void clear();//�൱�ڹػ���cache����ȫ�����
};