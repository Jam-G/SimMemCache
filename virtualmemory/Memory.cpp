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
	bool flag = false;//立起flag标志是否完成替换任务（或者说是否有空块）
	for(int i = 0; i < 1 << (MEMORYSIZE - MYPAGESIZE); i ++)//先寻找是否有空的页位置，若有则直接将该地址返回
	{
		if(hascontain[i] == 0)//找到空块直接将该块置为最近访问
		{
			aimposition = i;
			flag = true;
			break;
		}
	}
	if(flag == false)//没有空块
	{
		int max = -1;
		for(int i = 0; i < 1 << (MEMORYSIZE - MYPAGESIZE); i ++)//寻找最近最少用，也即使用LRU进行替换
		{
			if(hascontain[i] > max)
			{
				max = hascontain[i];
				aimposition = i;
			}
		}
	}
	rerush(aimposition);
	return flag;//返回是否完成了工作
}


void Memory::rerush(unsigned long int physicalpagenumber)
{
	hascontain[physicalpagenumber] = 1;
	for(int i = 0; i < 1 << (MEMORYSIZE - MYPAGESIZE); i ++)
	{
		if(i != physicalpagenumber && hascontain[i] != 0)//将非空块的访问过去时间自增
			hascontain[i] ++;
	}
}

void Memory::clear()//重置
{
	for(int i = 0; i < 1 << (MEMORYSIZE - MYPAGESIZE); i ++)
		hascontain[i] = 0;
}