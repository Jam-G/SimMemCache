#pragma once
const int MEMORYSIZE = 24;//24λ�����ַ��Ҳ��16MB���ڴ�
const int MYPAGESIZE = 12;//4KB

class Memory
{
	const int MEMORYSIZE;
	const int MYPAGESIZE;
	int *hascontain;
public:
	Memory(int memorysize = 24, int mpagesize = 4);
	~Memory();
	bool updata(unsigned long int &aimposition);//��ȱҳ�ж�ʱ��Ҫ���ڴ���и���Ѱ�ҿ�λ�û�LRU��ҳ��
	//�����п�ҳ����ֱ��д�룬����������ɵ�true��־��������λ�ã����򷵻�δ��ɵ�false��ѡ��Ҫ������ҳ��֮��Ĺ�������ҳ����и���ҳ��
	void rerush(unsigned long int physicalpagenumber);//��������ҳ��ķ���Ƶ�ʣ�LRU��־������hascontain�����ݣ�
	void clear();
};

