#pragma once
#include"Cache.h"
#include"TLB.h"
#include"PageTable.h"
#include"Memory.h"
class StoreSystem
{
	const int PAGE_SIZE;
	TLB tlb;
	PageTable  pagetable;
	Cache cache;
	Memory memory;
public:
	StoreSystem(int pagesize = 4);
	StoreSystem(int vadd, int padd, int pagesize,int tlben, int tlbw,  int cachesize, int cachelinesize, int cacheways);
	void access(unsigned long int virtual_addrss, char load_or_store, bool tlb_pagetable_cache[3]);//ͨ�����鷵�ط���ϸ��
	void clear();
};