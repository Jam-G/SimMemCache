#include"Memory.h"
#include<iostream>
Memory::Memory(int memorysize, int mpagesize):MEMORYSIZE(memorysize), MYPAGESIZE(log((double)mpagesize)/log(2.0) + 10)
{
	hascontain = new int[1 << (MEMORYSIZE - MYPAGESIZE)];
	for(int i = 0; i < 1 << (MEMORYSIZE - MYPAGESIZE); i ++)
		hascontain[i] = 0;
}

Memory::~Memory()
{
	if(hascontain != NULL)
		delete []hascontain;
	hascontain = NULL;
}

bool Memory::updata(unsigned long int &aimposition)
{
	bool flag = false;//����flag��־�Ƿ�����滻���񣨻���˵�Ƿ��пտ飩
	for(int i = 0; i < 1 << (MEMORYSIZE - MYPAGESIZE); i ++)//��Ѱ���Ƿ��пյ�ҳλ�ã�������ֱ�ӽ��õ�ַ����
	{
		if(hascontain[i] == 0)//�ҵ��տ�ֱ�ӽ��ÿ���Ϊ�������
		{
			aimposition = i;
			flag = true;
			break;
		}
	}
	if(flag == false)//û�пտ�
	{
		int max = -1;
		for(int i = 0; i < 1 << (MEMORYSIZE - MYPAGESIZE); i ++)//Ѱ����������ã�Ҳ��ʹ��LRU�����滻
		{
			if(hascontain[i] > max)
			{
				max = hascontain[i];
				aimposition = i;
			}
		}
	}
	rerush(aimposition);
	return flag;//�����Ƿ�����˹���
}


void Memory::rerush(unsigned long int physicalpagenumber)
{
	hascontain[physicalpagenumber] = 1;
	for(int i = 0; i < 1 << (MEMORYSIZE - MYPAGESIZE); i ++)
	{
		if(i != physicalpagenumber && hascontain[i] != 0)//���ǿտ�ķ��ʹ�ȥʱ������
			hascontain[i] ++;
	}
}

void Memory::clear()//����
{
	for(int i = 0; i < 1 << (MEMORYSIZE - MYPAGESIZE); i ++)
		hascontain[i] = 0;
}