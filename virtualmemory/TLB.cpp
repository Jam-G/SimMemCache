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
	for(int i = 0; i < ENTRYNUMBER/WAYS; i ++)//初始化
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
	unsigned long int tagadd = virtual_page_number >> (ENTRYBIT - WAYBIT);//将低位的组索引移出，获得tag位
	unsigned long int setadd = virtual_page_number & ((1 << (ENTRYBIT - WAYBIT)) - 1);//获得组所索引号,即用(ENTRYBIT - WAYBIT)位位1的数将数据屏蔽得出
	for(int i = 0; i < WAYS; i ++)//在一组中查找是否hit
	{
		if(use[setadd][i] != 0 && tag[setadd][i] == tagadd)//hit，则只要把相应的pagenumber的内容赋给引用，。然后返回true即可
		{
			unsigned int lrus = fabs((double)use[setadd][i]);
			phsical_page_number = pagenumber[setadd][i];
			//更新TLB
			if(load_or_store == 's')//如果是写操作则相应页是置为dirty
				use[setadd][i] = -1;
			else
				use[setadd][i] < 0? use[setadd][i] = -1:use[setadd][i] = 1;//保持符号位不变绝对值置1
			for(int j = 0;j < WAYS; j ++)//将其他的访问时间加1,0（空位置不变），符号不变（符号表示dirty bit）
			{
				if(j != i && use[setadd][j] != 0 &&  fabs((double)use[setadd][i]) < lrus)
					use[setadd][j] < 0? use[setadd][j] --: use[setadd][j] ++;
			}
			return true;
		}
	}
	//循环结束未返回说明miss。则返回miss信息
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

bool TLB::updata(unsigned long int pagenum, unsigned long int ps, unsigned long int &replace)//如果有空块则直接写入并更新，若无空位置并且tlb不需要写回则直接填充并更新，若需要写回pagetable则返回需要写的虚拟页号和相关信息
{
	unsigned long int tagadd = pagenum >> (ENTRYBIT - WAYBIT);//将低位的组索引移出，获得tag位
	unsigned long int setadd = pagenum & ((1 << (ENTRYBIT - WAYBIT)) - 1);//获得组所索引号,即用(ENTRYBIT - WAYBIT)位位1的数将数据屏蔽得出
	unsigned long int aim = 0;//新的要写入的位置
	bool blank = false;
	int max = -1;
	for(int i = 0; i < WAYS; i ++)//在一组中查找是否有空块
	{
		if(use[setadd][i] == 0)//有空块
		{
			aim = i;		
			max = 10000;//随便取的一个较大的整数
			blank = true;
			break;
		}
	}
	if(blank == false)//没有空块了，则找到要替换的块
	{
		for (int i = 0; i < WAYS; i ++)
		{
			if(fabs((double)use[setadd][i]) > max)
			{
				max = fabs((double)use[setadd][i]);
				aim = i;
			}
		}
		//循环结束则找到了目标位置
	}
	//装入
	use[setadd][aim] = 1;//不是脏页则给他置为1
	pagenumber[setadd][aim] = ps;
	for(int j = 0;j < WAYS; j ++)//将其他的访问时间加1,0（空位置不变），符号不变（符号表示dirty bit）
	{
		if(j != aim && use[setadd][j] != 0 && fabs((double)use[setadd][j]) < max)
			use[setadd][j] < 0? use[setadd][j] --: use[setadd][j] ++;
	}
	//循环结束未返回说明miss。则返回miss信息
	if(blank == false && use[setadd][aim] < 0)// 需要写回
	{
		replace = (tag[setadd][aim] << WAYBIT) + setadd;//即需要将也表中的对应表项修改位置为dirty
		tag[setadd][aim] = tagadd;//完成装入最后的任务
		return false;
	}
	tag[setadd][aim] = tagadd;//完成装入的最后任务
	return true;//替换完成
}

void TLB::rerush(unsigned long int page)
{
	unsigned long int tagadd = page >> (ENTRYBIT - WAYBIT);//将低位的组索引移出，获得tag位
	unsigned long int setadd = page & ((1 << (ENTRYBIT - WAYBIT)) - 1);//获得组所索引号,即用(ENTRYBIT - WAYBIT)位位1的数将数据屏蔽得出
	for(int i = 0; i < WAYS; i ++)
	{
		if(tag[setadd][i] == tagadd)//在tlb中移出
		{
			use[setadd][i] = 0;
			break;
		}
	}
}

void TLB::clear()
{
	for(int i = 0; i < ENTRYNUMBER/WAYS; i ++)//初始化
	{
		for(int k = 0; k < WAYS; k ++)
		{
			tag[i][k] = 0;
			pagenumber[i][k] = 0;
			use[i][k] = 0;
		}
	}
}
