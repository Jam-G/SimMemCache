#pragma once

class TLB
{
	const int ENTRYNUMBER;
	const int WAYS;
	const int ENTRYBIT;
	const int WAYBIT;
	unsigned long int **tag;
	unsigned long int **pagenumber;
	int **use;//记录TLB的使用时间和是否是dirty bit

public:
	TLB(int entrynum = 64, int ways = 4);
	~TLB();
	bool access(unsigned long int virtual_page_number, unsigned long int &phsical_page_number, char load_or_store);//返回是否为hit，若hit则物理页号通过引用传回，若miss则需要访问页表
	bool updata(unsigned long int virtualpagenumber, unsigned long int physicalpagenumber, unsigned long int &replace);//更新函数，类似于pagetable的更新函数，
	//设想把tlb的更新函数使用pagetable作为参数
	void rerush(unsigned long int page);//page所标页号移除主存，将该表项移除tlb；
	void clear();
};