#include"TLB.h"
#include<fstream>
#include<iostream>
using namespace std;
TLB::TLB(int entrynum, int ways):ENTRYNUMBER(entrynum), WAYS(ways), ENTRYBIT(log((double)entrynum)/log(2.0)), WAYBIT(log((double)ways)/log(2.0))
{
	tag = new unsigned long int*[ENTRYNUMBER/WAYS];
	pagenumber = new unsigned long int*[ENTRYNUMBER/WAYS];
	use = new int*[ENTRYNUMBER/WAYS];
	for(int i = 0; i < ENTRYNUMBER/WAYS; i ++)
	{
		tag[i] = new unsigned long int[WAYS];
		pagenumber[i] = new unsigned long int[WAYS];
		use[i] = new int[WAYS];
	}
	for(int i = 0; i < ENTRYNUMBER/WAYS; i ++)//��ʼ��
	{
		for(int k = 0; k < WAYS; k ++)
		{
			tag[i][k] = 0;
			pagenumber[i][k] = 0;
			use[i][k] = 0;
		}
	}
}

//**********************************************************************************
bool TLB::access(unsigned long int virtual_page_number, unsigned long int &phsical_page_number, char load_or_store)
{
	unsigned long int tagadd = virtual_page_number >> (ENTRYBIT - WAYBIT);//����λ���������Ƴ������tagλ
	unsigned long int setadd = virtual_page_number & ((1 << (ENTRYBIT - WAYBIT)) - 1);//�������������,����(ENTRYBIT - WAYBIT)λλ1�������������εó�
	for(int i = 0; i < WAYS; i ++)//��һ���в����Ƿ�hit
	{
		if(use[setadd][i] != 0 && tag[setadd][i] == tagadd)//hit����ֻҪ����Ӧ��pagenumber�����ݸ������ã���Ȼ�󷵻�true����
		{
			unsigned int lrus = fabs((double)use[setadd][i]);
			phsical_page_number = pagenumber[setadd][i];
			//����TLB
			if(load_or_store == 's')//�����д��������Ӧҳ����Ϊdirty
				use[setadd][i] = -1;
			else
				use[setadd][i] < 0? use[setadd][i] = -1:use[setadd][i] = 1;//���ַ���λ�������ֵ��1
			for(int j = 0;j < WAYS; j ++)//�������ķ���ʱ���1,0����λ�ò��䣩�����Ų��䣨���ű�ʾdirty bit��
			{
				if(j != i && use[setadd][j] != 0 &&  fabs((double)use[setadd][i]) < lrus)
					use[setadd][j] < 0? use[setadd][j] --: use[setadd][j] ++;
			}
			return true;
		}
	}
	//ѭ������δ����˵��miss���򷵻�miss��Ϣ
	return false;
}

TLB::~TLB()
{
	if(tag != NULL && pagenumber != NULL && use != NULL)
	{
		for(int i = 0; i < ENTRYNUMBER/WAYS; i ++)
		{
			delete []tag[i];
			delete []pagenumber[i];
			delete []use[i];
		}
		delete []tag;
		tag = NULL;
		delete []pagenumber;
		pagenumber = NULL;
		delete []use;
		use = NULL;
	}
}

bool TLB::updata(unsigned long int pagenum, unsigned long int ps, unsigned long int &replace)//����пտ���ֱ��д�벢���£����޿�λ�ò���tlb����Ҫд����ֱ����䲢���£�����Ҫд��pagetable�򷵻���Ҫд������ҳ�ź������Ϣ
{
	unsigned long int tagadd = pagenum >> (ENTRYBIT - WAYBIT);//����λ���������Ƴ������tagλ
	unsigned long int setadd = pagenum & ((1 << (ENTRYBIT - WAYBIT)) - 1);//�������������,����(ENTRYBIT - WAYBIT)λλ1�������������εó�
	unsigned long int aim = 0;//�µ�Ҫд���λ��
	bool blank = false;
	int max = -1;
	for(int i = 0; i < WAYS; i ++)//��һ���в����Ƿ��пտ�
	{
		if(use[setadd][i] == 0)//�пտ�
		{
			aim = i;		
			max = 10000;//���ȡ��һ���ϴ������
			blank = true;
			break;
		}
	}
	if(blank == false)//û�пտ��ˣ����ҵ�Ҫ�滻�Ŀ�
	{
		for (int i = 0; i < WAYS; i ++)
		{
			if(fabs((double)use[setadd][i]) > max)
			{
				max = fabs((double)use[setadd][i]);
				aim = i;
			}
		}
		//ѭ���������ҵ���Ŀ��λ��
	}
	//װ��
	use[setadd][aim] = 1;//������ҳ�������Ϊ1
	pagenumber[setadd][aim] = ps;
	for(int j = 0;j < WAYS; j ++)//�������ķ���ʱ���1,0����λ�ò��䣩�����Ų��䣨���ű�ʾdirty bit��
	{
		if(j != aim && use[setadd][j] != 0 && fabs((double)use[setadd][j]) < max)
			use[setadd][j] < 0? use[setadd][j] --: use[setadd][j] ++;
	}
	//ѭ������δ����˵��miss���򷵻�miss��Ϣ
	if(blank == false && use[setadd][aim] < 0)// ��Ҫд��
	{
		replace = (tag[setadd][aim] << WAYBIT) + setadd;//����Ҫ��Ҳ���еĶ�Ӧ�����޸�λ��Ϊdirty
		tag[setadd][aim] = tagadd;//���װ����������
		return false;
	}
	tag[setadd][aim] = tagadd;//���װ����������
	return true;//�滻���
}

void TLB::rerush(unsigned long int page)
{
	unsigned long int tagadd = page >> (ENTRYBIT - WAYBIT);//����λ���������Ƴ������tagλ
	unsigned long int setadd = page & ((1 << (ENTRYBIT - WAYBIT)) - 1);//�������������,����(ENTRYBIT - WAYBIT)λλ1�������������εó�
	for(int i = 0; i < WAYS; i ++)
	{
		if(tag[setadd][i] == tagadd)//��tlb���Ƴ�
		{
			use[setadd][i] = 0;
			break;
		}
	}
}

void TLB::clear()
{
	for(int i = 0; i < ENTRYNUMBER/WAYS; i ++)//��ʼ��
	{
		for(int k = 0; k < WAYS; k ++)
		{
			tag[i][k] = 0;
			pagenumber[i][k] = 0;
			use[i][k] = 0;
		}
	}
}
