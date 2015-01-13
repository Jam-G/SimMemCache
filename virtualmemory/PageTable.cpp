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
	if(valued[add] == true)//在内存中即pagetable hit
	{
		if(load_or_store == 's')//如果是写操作说明相应的块会被修改，将来替换时要写回硬盘
			dirtybit[add] = true;
		page_number = position[add];//将物理页号传回去
		return true;//返回pagetable hit信息
	}
	//缺页中断
	return false;//返回缺页中断消息 ，让整个系统去处理
}

bool PageTable::updata(unsigned long int pagenumber, unsigned long int ps, bool needwriteback, char load_or_store, unsigned long int &torerushtlb)//更新
{
	unsigned long int virtualpagenum = 0;
	bool needrerushtlb = false;
	if(needwriteback == true)//传进的物理页框号有内容需要要检测换出
	{
		for(int i = 0; i < 1 << (VIRTUALADD - PAGE_SIZE); i ++)//在pagetable中搜存放位置为ps的块看是否需要写回
		{
			if(valued[i] == true && position[i] == ps)//找到了换出的页，更新pagetable并检查是否要换出
			{
				torerushtlb = i;//记录下被换出的位置//以便把tlb中的相应取出
				needrerushtlb = true;
				if(dirtybit[i] == true)//需要换出
					;//写到磁盘中
				//更新，将装入位改掉
				valued[i] = false;//即已经换出
				break;//换出之后不需要继续循环
			}
		}
	}
	//内存的相应页块已经填充只要更改相应页表就好
	//进行完了上边的操作传进来的位置就是可以使用的位置了，则更改pagetable信息
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

void PageTable::clear()//重置
{
	for(int i = 0; i < 1 << (VIRTUALADD - PAGE_SIZE); i ++)
	{
		valued[i] = false;
		dirtybit[i] = false;
		position[i] = 0;
	}
}