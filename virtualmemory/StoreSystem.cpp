//#include"com.h"
#include"StoreSystem.h"
StoreSystem::StoreSystem(int pagesize):cache(4, false, true, 64, 32), PAGE_SIZE(log((double)pagesize)/log(2.0) + 10)
{}
//************************************
StoreSystem::StoreSystem(int vadd, int padd, int pagesize,int tlben, int tlbw,  int cachesize, int cachelinesize, int cacheways):PAGE_SIZE(log((double)pagesize)/log(2.0) + 10),tlb(tlben, tlbw),pagetable(vadd,padd,pagesize),memory(padd, pagesize),cache(cacheways, false, true,cachesize,cachelinesize)
{}

void StoreSystem::access(unsigned long int virtual_address, char load_or_store, bool tlb_pagetable_cache[3])//ͨ�����鷵�ط���ϸ��
{
	unsigned long int physicaladd = 0;
	unsigned long int virtualpagenum = virtual_address >> PAGE_SIZE;//ȡ����ҳ��
	unsigned long int addinpage = (virtual_address << (32 - PAGE_SIZE)) >> (32 - PAGE_SIZE);//ȡҳ�ڵ�ַ
	unsigned long int physicalpagenum = 0;
	tlb_pagetable_cache[0] = tlb.access(virtualpagenum, physicalpagenum, load_or_store);//����tlb
	if(tlb_pagetable_cache[0] == false)//��tlb miss�����ҳ
	{
		//pagetableaccess ++;//����ʹ��
		tlb_pagetable_cache[1] = pagetable.access(virtualpagenum, load_or_store, physicalpagenum);
		if(tlb_pagetable_cache[1] == true)//δ����ȱҳ�жϼ�ҳ������
		{
			//pagetablehi ++;
			unsigned long int aim = 0;
			if(tlb.updata(virtualpagenum, physicalpagenum, aim) == false)//����
				pagetable.setdirty(aim);
		}
		else////��ȱҳ�ж�
		{
			bool compleate = memory.updata(physicalpagenum);//���ڴ����ҵ�һ���������õ�ҳ������ҳ���Ѹ��£�
			unsigned long int tlbpage = 0;
			if(compleate == false)//����ҳ��������Ҳ�����ǿ�ҳ��ֱ�ӵ���������
				cache.updata(physicalpagenum);
			if(pagetable.updata(virtualpagenum, physicalpagenum, !compleate, load_or_store, tlbpage) == true)//���淵����Ϣ����pagetable(��������滻)
				tlb.rerush(tlbpage);//��ҳ��������TLB�м���Ƿ������tlb����������tlb��������
			//���л���ҳ�򽫼��cache���Ƿ��и�ҳ�����ݣ�������Ҫ���
			unsigned long int aim = 0;
			if(tlb.updata(virtualpagenum, physicalpagenum, aim) == false)//����tlb�������µ����ҳ�ǵ�tlb��
				pagetable.setdirty(aim);//��û�пյ�tlb��Ŀ���򻻳�һ��tlb��Ŀ����tlb��Ŀ��¼ҳ״̬���޸Ĺ���ǵ�pagetable��
		}
	}
	memory.rerush(physicalpagenum);
	physicaladd = (physicalpagenum << PAGE_SIZE) + addinpage;//��ϳ������ַ
	//extern ofstream foutlog;
	//extern int counter;
	//foutlog <<dec << ++counter  << " " << physicaladd << endl;
	tlb_pagetable_cache[2] = cache.access(physicaladd, load_or_store);//����cache�Լ����������
}

void StoreSystem::clear()
{
	tlb.clear();
	pagetable.clear();
	memory.clear();
	cache.clear();
}