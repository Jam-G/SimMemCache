#pragma once

class TLB
{
	const int ENTRYNUMBER;
	const int WAYS;
	const int ENTRYBIT;
	const int WAYBIT;
	unsigned long int **tag;
	unsigned long int **pagenumber;
	int **use;//��¼TLB��ʹ��ʱ����Ƿ���dirty bit

public:
	TLB(int entrynum = 64, int ways = 4);
	~TLB();
	bool access(unsigned long int virtual_page_number, unsigned long int &phsical_page_number, char load_or_store);//�����Ƿ�Ϊhit����hit������ҳ��ͨ�����ô��أ���miss����Ҫ����ҳ��
	bool updata(unsigned long int virtualpagenumber, unsigned long int physicalpagenumber, unsigned long int &replace);//���º�����������pagetable�ĸ��º�����
	//�����tlb�ĸ��º���ʹ��pagetable��Ϊ����
	void rerush(unsigned long int page);//page����ҳ���Ƴ����棬���ñ����Ƴ�tlb��
	void clear();
};