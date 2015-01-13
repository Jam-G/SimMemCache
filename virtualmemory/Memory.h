#pragma once
const int MEMORYSIZE = 24;//24位物理地址，也即16MB的内存
const int MYPAGESIZE = 12;//4KB

class Memory
{
	const int MEMORYSIZE;
	const int MYPAGESIZE;
	int *hascontain;
public:
	Memory(int memorysize = 24, int mpagesize = 4);
	~Memory();
	bool updata(unsigned long int &aimposition);//当缺页中断时，要把内存进行更新寻找空位置或LRU换页，
	//（若有空页框，则直接写入，并返回已完成的true标志并返回其位置，否则返回未完成的false并选出要换出的页号之后的工作交给页表进行更新页表）
	void rerush(unsigned long int physicalpagenumber);//更新物理页框的访问频率，LRU标志（就是hascontain的内容）
	void clear();
};

