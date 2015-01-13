#pragma once 
class PageTable
{
	const int VIRTUALADD ;
	const int PHYSICALADD;
	const int PAGE_SIZE;
	bool *valued;//װ��λ
	bool *dirtybit;//�޸�λ
	unsigned long int *position;//���λ�ã�
public:
	PageTable(int virtualaddlen = 32, int physicaladdlen = 24, int pagesize = 4);
	~PageTable();
	bool access(unsigned long int add, char load_or_store, unsigned long int &page_number);
	bool updata(unsigned long int pagenumber, unsigned long int ps, bool neetwriteback, char load_or_store, unsigned long int &torerushtlb);//ȱҳ�жϵõ�����֮�����pagetable
	//���Memory��û�пտ齫�����һ���滻��ַ�����������л�������������pagetable
	//���ص��������Ҫ�����򷵻ص��Ǳ�������ҳԭ��������ҳ�ţ�����Ǹ�TLBʹ�õ�
	void setdirty(unsigned long int virtualpage);
	void clear();
};