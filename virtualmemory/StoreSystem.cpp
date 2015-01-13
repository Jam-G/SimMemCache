//#include"com.h"
#include"StoreSystem.h"
StoreSystem::StoreSystem(int pagesize):cache(4, false, true, 64, 32), PAGE_SIZE(log((double)pagesize)/log(2.0) + 10)
{}
//************************************
StoreSystem::StoreSystem(int vadd, int padd, int pagesize,int tlben, int tlbw,  int cachesize, int cachelinesize, int cacheways):PAGE_SIZE(log((double)pagesize)/log(2.0) + 10),tlb(tlben, tlbw),pagetable(vadd,padd,pagesize),memory(padd, pagesize),cache(cacheways, false, true,cachesize,cachelinesize)
{}

void StoreSystem::access(unsigned long int virtual_address, char load_or_store, bool tlb_pagetable_cache[3])//通过数组返回访问细节
{
	unsigned long int physicaladd = 0;
	unsigned long int virtualpagenum = virtual_address >> PAGE_SIZE;//取虚拟页号
	unsigned long int addinpage = (virtual_address << (32 - PAGE_SIZE)) >> (32 - PAGE_SIZE);//取页内地址
	unsigned long int physicalpagenum = 0;
	tlb_pagetable_cache[0] = tlb.access(virtualpagenum, physicalpagenum, load_or_store);//访问tlb
	if(tlb_pagetable_cache[0] == false)//若tlb miss则访问页
	{
		//pagetableaccess ++;//测试使用
		tlb_pagetable_cache[1] = pagetable.access(virtualpagenum, load_or_store, physicalpagenum);
		if(tlb_pagetable_cache[1] == true)//未发生缺页中断即页表命中
		{
			//pagetablehi ++;
			unsigned long int aim = 0;
			if(tlb.updata(virtualpagenum, physicalpagenum, aim) == false)//将调
				pagetable.setdirty(aim);
		}
		else////若缺页中断
		{
			bool compleate = memory.updata(physicalpagenum);//从内存中找到一个最近最不常用的页（若空页则已更新）
			unsigned long int tlbpage = 0;
			if(compleate == false)//即有页被换出（也即不是空页框直接调入的情况）
				cache.updata(physicalpagenum);
			if(pagetable.updata(virtualpagenum, physicalpagenum, !compleate, load_or_store, tlbpage) == true)//主存返回信息更新pagetable(包括完成替换)
				tlb.rerush(tlbpage);//有页被换出则到TLB中检测是否调入了tlb，若调入了tlb则把它清掉
			//若有换出页则将检测cache中是否有该页的内容，若有则要清除
			unsigned long int aim = 0;
			if(tlb.updata(virtualpagenum, physicalpagenum, aim) == false)//更新tlb，将最新调入的页记到tlb中
				pagetable.setdirty(aim);//若没有空的tlb项目，则换出一个tlb项目，若tlb项目记录页状态被修改过则记到pagetable中
		}
	}
	memory.rerush(physicalpagenum);
	physicaladd = (physicalpagenum << PAGE_SIZE) + addinpage;//组合出物理地址
	//extern ofstream foutlog;
	//extern int counter;
	//foutlog <<dec << ++counter  << " " << physicaladd << endl;
	tlb_pagetable_cache[2] = cache.access(physicaladd, load_or_store);//访问cache以及下面的主存
}

void StoreSystem::clear()
{
	tlb.clear();
	pagetable.clear();
	memory.clear();
	cache.clear();
}