#pragma once 
class PageTable
{
	const int VIRTUALADD ;
	const int PHYSICALADD;
	const int PAGE_SIZE;
	bool *valued;//装入位
	bool *dirtybit;//修改位
	unsigned long int *position;//存放位置，
public:
	PageTable(int virtualaddlen = 32, int physicaladdlen = 24, int pagesize = 4);
	~PageTable();
	bool access(unsigned long int add, char load_or_store, unsigned long int &page_number);
	bool updata(unsigned long int pagenumber, unsigned long int ps, bool neetwriteback, char load_or_store, unsigned long int &torerushtlb);//缺页中断得到处理之后更新pagetable
	//如果Memory中没有空块将会产生一个替换地址，本函数进行换出操作并更新pagetable
	//返回的是如果需要换出则返回的是被换出的页原来的虚拟页号，这个是给TLB使用的
	void setdirty(unsigned long int virtualpage);
	void clear();
};