#include"PageTable.h"
#include<iostream>
#include<cmath>
PageTable::PageTable(int virtualaddlen, int physicaladdlen, int pagesize):VIRTUALADD(virtualaddlen), PHYSICALADD(physicaladdlen),PAGE_SIZE(log((double)pagesize)/log(2.0) + 10)
{
	valued = new bool[1 << (VIRTUALADD - PAGE_SIZE)];
	dirtybit = new bool[1 << (VIRTUALADD - PAGE_SIZE)];
	position = new unsigned long int[1 << (VIRTUALADD - PAGE_SIZE)];
	for(int i = 0; i < 1 << (VIRTUALADD - PAGE_SIZE); i ++)
	{
		valued[i] = false;
		dirtybit[i] = false;
		position[i] = 0;
	}
}


PageTable::~PageTable()
{
	if(valued != NULL)
		delete []valued;
	valued = NULL;
	if(dirtybit != NULL)
		delete []dirtybit;
	dirtybit = NULL;
	if(position != NULL)
		delete []position;
	position = NULL;
}

bool PageTable::access(unsigned long int add, char load_or_store, unsigned long int &page_number)
{
	if(valued[add] == true)//���ڴ��м�pagetable hit
	{
		if(load_or_store == 's')//�����д����˵����Ӧ�Ŀ�ᱻ�޸ģ������滻ʱҪд��Ӳ��
			dirtybit[add] = true;
		page_number = position[add];//������ҳ�Ŵ���ȥ
		return true;//����pagetable hit��Ϣ
	}
	//ȱҳ�ж�
	return false;//����ȱҳ�ж���Ϣ ��������ϵͳȥ����
}

bool PageTable::updata(unsigned long int pagenumber, unsigned long int ps, bool needwriteback, char load_or_store, unsigned long int &torerushtlb)//����
{
	unsigned long int virtualpagenum = 0;
	bool needrerushtlb = false;
	if(needwriteback == true)//����������ҳ�����������ҪҪ��⻻��
	{
		for(int i = 0; i < 1 << (VIRTUALADD - PAGE_SIZE); i ++)//��pagetable���Ѵ��λ��Ϊps�Ŀ鿴�Ƿ���Ҫд��
		{
			if(valued[i] == true && position[i] == ps)//�ҵ��˻�����ҳ������pagetable������Ƿ�Ҫ����
			{
				torerushtlb = i;//��¼�±�������λ��//�Ա��tlb�е���Ӧȡ��
				needrerushtlb = true;
				if(dirtybit[i] == true)//��Ҫ����
					;//д��������
				//���£���װ��λ�ĵ�
				valued[i] = false;//���Ѿ�����
				break;//����֮����Ҫ����ѭ��
			}
		}
	}
	//�ڴ����Ӧҳ���Ѿ����ֻҪ������Ӧҳ��ͺ�
	//���������ϱߵĲ�����������λ�þ��ǿ���ʹ�õ�λ���ˣ������pagetable��Ϣ
	position[pagenumber] =ps;
	valued[pagenumber] = true;
	if(load_or_store == 's')
		dirtybit[pagenumber]  = true;
	else
		dirtybit[pagenumber] = false;
	return needrerushtlb;
}

void PageTable::setdirty(unsigned long int virtualpage)
{
	if(valued[virtualpage] == true)
		dirtybit[virtualpage] = true;
}

void PageTable::clear()//����
{
	for(int i = 0; i < 1 << (VIRTUALADD - PAGE_SIZE); i ++)
	{
		valued[i] = false;
		dirtybit[i] = false;
		position[i] = 0;
	}
}