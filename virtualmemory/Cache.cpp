#include"Cache.h"
#include<fstream>
#include<iostream>
#include<cmath>
using namespace std;



Cache::Cache()
{
	tag = NULL;

	replacepolicy = true;
	writepolicy = false;
	
	cachesize = 0;
	cachelinesize = 0;
	numberofsetbit = 0;
	numberoflinebit = 0;
}

Cache::Cache(int associativity,bool replacepolicy, bool writepolicy, int cachesize, int cachelinesize)//size����bit��ʾ�ģ���4B��Ϊ2bit
{
	this ->replacepolicy = replacepolicy;
	this ->writepolicy = writepolicy;
	this ->cachesize = log((double)cachesize)/log(2.0) + 10;
	this ->cachelinesize = log((double)cachelinesize)/log(2.0);;
	

	if(associativity == 0)//ȫ����
	{
		numberofsetbit = 0;
		numberoflinebit = this ->cachesize - this ->cachelinesize;
	}
	else if(associativity == 1)//ֱ������
	{
		numberofsetbit = this ->cachesize - this->cachelinesize;
		numberoflinebit = 0;
	}
	else//������
	{
		numberoflinebit = log((double)associativity)/log(2.0);
		numberofsetbit = this ->cachesize - this ->cachelinesize - this ->numberoflinebit;
	}
	tag = new unsigned long int*[1 << this ->numberofsetbit];//����ָ��
	use = new int *[1 << this ->numberofsetbit];
	for(int i = 0;i < (1 << this ->numberofsetbit); i ++)
	{
		tag[i] = new unsigned long int[1 << this ->numberoflinebit];//�൱�ڶ�ά�������
		use[i] = new int [1 << this ->numberoflinebit];
		for(int j = 0; j < 1 << this ->numberoflinebit; j ++)//��ʼ��
		{
			tag[i][j] = 0;
			use[i][j] = 0;
		}
	}
}

Cache::~Cache()
{
	if(use != NULL)
	{
		for(int i = 0; i < 1 << numberoflinebit; i ++)
		{
			if(use[i] != NULL)
				delete []use[i];
		}
		delete use;
		use = NULL;
	}
	if(tag != NULL)
	{
		for(int i = 0; i < 1 << numberofsetbit; i ++)
		{
			if(tag[i] != NULL)
				delete []tag[i];
		}
		delete []tag;
		tag = NULL;
	}
}

bool Cache::access(unsigned long int add, char lors)
{
	unsigned long int tagadd = add >> (cachelinesize + numberofsetbit);
	unsigned long int setadd = ((unsigned long int)(add << (32 - numberofsetbit - cachelinesize)) >> (32 - numberofsetbit));
	if(numberofsetbit == 0)//ȫ����
		setadd = 0;//ֻ��һ�顣�Ͳ����ж���
	for(int i = 0; i < (1 << numberoflinebit); i ++)
	{
		//ȡ����i �е�tagλ��
		if(use[setadd][i] != 0 && tagadd == tag[setadd][i])//hit
		{
			/*if(lors == 'l')
				out << hex << add << "\t: load - hit;" << endl;
			if(lors == 's')
				out << hex << add << "\t: store - hit;" << endl;
				*/
			use[setadd][i] < 0? use[setadd][i] = -1:use[setadd][i] = 1;//���ַ���λ�������ֵ��1
			for(int j = 0;j < 1 << numberoflinebit; j ++)//�������ķ���ʱ���1,0����λ�ò��䣩�����Ų��䣨���ű�ʾdirty bit��
			{
				if(j != i && use[setadd][j] != 0)
					use[setadd][j] < 0? use[setadd][j] --: use[setadd][j] ++;
			}
			return true;	
		}
	}
	//miss
	/*
	if(lors == 'l')
		out << hex << add << "\t: load - miss;" << endl;
	if(lors == 's')
		out << hex << add << "\t: store - miss;" << endl;
		*/
	if(replacepolicy)//��ȡ����滻�㷨
		random(tag[setadd], use[setadd], tagadd);
	else//��ȡLRU�滻�㷨
		LRU(tag[setadd], use[setadd], tagadd);
	return false;//����miss��Ϣ
}
void Cache::random(unsigned long int *tag, int *use, unsigned long add)
{
	//�����ҵ��տ�
	bool flag = false;
	int n = rand()%(1 << numberoflinebit);//�������һ���滻����ַ
	for(int i = 0; i < 1 << numberoflinebit; i ++)
	{
		if(use[i] == 0)//�ҵ��տ�
		{
			tag[i] = add;
			use[i] = 1;//��1
			flag = true;//�ѻ���
			n = i;
			break;
		}
	}
	if(!flag)//
	{
		//writethough����Ҫд������
		if(!writepolicy && use[n] < 0)//writeback����д������
		{
			;//д������
		}
		tag[n] = add;
		use[n] = 1;
	}
	for(int j = 0; j < 1 << numberoflinebit; j ++)//�������ķ���ʱ���1,0����λ�ò��䣩�����Ų��䣨���ű�ʾdirty bit��
	{
		if(j != n && use[j] != 0)
			use[j] < 0? use[j] --: use[j] ++;
	}
}

void Cache::LRU(unsigned long int* tag, int *use, unsigned long int add)
{
	unsigned long int n = 0;//�滻��ַ
	bool flag = false;
	for(int i = 0; i < 1 << numberoflinebit; i ++)
	{
		if(use[i] == 0)//�ҵ��տ�
		{
			tag[i] = add;
			use[i] = 1;//���ַ���λ�������ֵ��1
			flag = true;//�ѻ���
			n = i;
			break;
		}
	}
	if(!flag)//
	{
		unsigned long int max = 0;
		for(int  i = 0; i < 1 << numberoflinebit; i ++)
		{
			if(fabs((double)use[i]) > max)
			{
				max = fabs((double)use[i]);
				n = i;
			}
		}
		//����
		if(!replacepolicy && use[n] < 0)//wrieteback
		{
			;//д����
		}
		//����
		tag[n] = add;
		use[n] = 1;
	}
	//����������
	for(int j = 0; j < 1 << numberoflinebit; j ++)//�������ķ���ʱ���1,0����λ�ò��䣩�����Ų��䣨���ű�ʾdirty bit��
	{
		if(j != n && use[j] != 0)
			use[j] < 0? use[j] --: use[j] ++;
	}
}


void Cache::updata(unsigned long int pagenum)
{
	unsigned long int tagbit = pagenum >> 2;//�����ַ����Ϊ���顣��һ�鳤��Ϊtagbit����Ϊ24-���ڵ�ַ-���ַ��ֻ��7λ����
	//����˴�����12λҳ��ǰ���7λ��tagλ����5λ��cache��ŵĸ�5λ���е�9λû�и�����Ҫ����ѭ�����ң�
	//������Ϊ���ڵ�ַ����Ϊcachelinesize��Ϊ3�����ڶ��������ַ������Ϊsetbit�����ַ������cachesize - linebit��Ϊ16-2= 14��
	unsigned long int sethbit = (tagbit << 30) >> 23;//����λ��30λ�Ƴ�,Ȼ���ƻ�ԭ��λ�ã�����7λ�ճ�
	for(int i = 0; i < 1 << 7; i ++, sethbit ++)
	{
		for(int k = 0; k < 4; k ++)//ѭ����������Ѱ��tag
		{
			if(use[sethbit][k] != 0 && tag[sethbit][k] == tagbit)
			{
				if(use[sethbit][k] < 0)//dirty
					;//д������
				use[sethbit][k] == 0;//���������ÿ�
			}
		}
	}
}

void Cache::clear()
{
	for(int i = 0; i < 1 << numberofsetbit; i ++)
		for(int j = 0; j < 1 << numberoflinebit; j ++)
		{
			tag[i][j] = 0;
			use[i][j] = 0;
		}
}

void Cache::printTagarray(ofstream &out)
{
	out << "tag:*************" << endl;
	for(int i = 0; i < 1 << numberofsetbit; i ++)
	{
		for(int j = 0; j < 1 << numberoflinebit; j ++)
			out << tag[i][j]  << ' ';
		out << endl;
	}
	out << endl;
}