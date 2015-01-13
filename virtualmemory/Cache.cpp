#include"Cache.h"
#include<fstream>
#include<iostream>
#include<cmath>
using namespace std;



Cache::Cache()
{
	tag = NULL;

	replacepolicy = true;
	writepolicy = false;
	
	cachesize = 0;
	cachelinesize = 0;
	numberofsetbit = 0;
	numberoflinebit = 0;
}

Cache::Cache(int associativity,bool replacepolicy, bool writepolicy, int cachesize, int cachelinesize)//size是用bit表示的，入4B即为2bit
{
	this ->replacepolicy = replacepolicy;
	this ->writepolicy = writepolicy;
	this ->cachesize = log((double)cachesize)/log(2.0) + 10;
	this ->cachelinesize = log((double)cachelinesize)/log(2.0);;
	

	if(associativity == 0)//全相连
	{
		numberofsetbit = 0;
		numberoflinebit = this ->cachesize - this ->cachelinesize;
	}
	else if(associativity == 1)//直接相连
	{
		numberofsetbit = this ->cachesize - this->cachelinesize;
		numberoflinebit = 0;
	}
	else//组相连
	{
		numberoflinebit = log((double)associativity)/log(2.0);
		numberofsetbit = this ->cachesize - this ->cachelinesize - this ->numberoflinebit;
	}
	tag = new unsigned long int*[1 << this ->numberofsetbit];//分配指针
	use = new int *[1 << this ->numberofsetbit];
	for(int i = 0;i < (1 << this ->numberofsetbit); i ++)
	{
		tag[i] = new unsigned long int[1 << this ->numberoflinebit];//相当于二维数组分配
		use[i] = new int [1 << this ->numberoflinebit];
		for(int j = 0; j < 1 << this ->numberoflinebit; j ++)//初始化
		{
			tag[i][j] = 0;
			use[i][j] = 0;
		}
	}
}

Cache::~Cache()
{
	if(use != NULL)
	{
		for(int i = 0; i < 1 << numberoflinebit; i ++)
		{
			if(use[i] != NULL)
				delete []use[i];
		}
		delete use;
		use = NULL;
	}
	if(tag != NULL)
	{
		for(int i = 0; i < 1 << numberofsetbit; i ++)
		{
			if(tag[i] != NULL)
				delete []tag[i];
		}
		delete []tag;
		tag = NULL;
	}
}

bool Cache::access(unsigned long int add, char lors)
{
	unsigned long int tagadd = add >> (cachelinesize + numberofsetbit);
	unsigned long int setadd = ((unsigned long int)(add << (32 - numberofsetbit - cachelinesize)) >> (32 - numberofsetbit));
	if(numberofsetbit == 0)//全相连
		setadd = 0;//只有一组。就不用判断了
	for(int i = 0; i < (1 << numberoflinebit); i ++)
	{
		//取出第i 行的tag位；
		if(use[setadd][i] != 0 && tagadd == tag[setadd][i])//hit
		{
			/*if(lors == 'l')
				out << hex << add << "\t: load - hit;" << endl;
			if(lors == 's')
				out << hex << add << "\t: store - hit;" << endl;
				*/
			use[setadd][i] < 0? use[setadd][i] = -1:use[setadd][i] = 1;//保持符号位不变绝对值置1
			for(int j = 0;j < 1 << numberoflinebit; j ++)//将其他的访问时间加1,0（空位置不变），符号不变（符号表示dirty bit）
			{
				if(j != i && use[setadd][j] != 0)
					use[setadd][j] < 0? use[setadd][j] --: use[setadd][j] ++;
			}
			return true;	
		}
	}
	//miss
	/*
	if(lors == 'l')
		out << hex << add << "\t: load - miss;" << endl;
	if(lors == 's')
		out << hex << add << "\t: store - miss;" << endl;
		*/
	if(replacepolicy)//采取随机替换算法
		random(tag[setadd], use[setadd], tagadd);
	else//采取LRU替换算法
		LRU(tag[setadd], use[setadd], tagadd);
	return false;//返回miss信息
}
void Cache::random(unsigned long int *tag, int *use, unsigned long add)
{
	//尝试找到空块
	bool flag = false;
	int n = rand()%(1 << numberoflinebit);//随机生成一个替换出地址
	for(int i = 0; i < 1 << numberoflinebit; i ++)
	{
		if(use[i] == 0)//找到空块
		{
			tag[i] = add;
			use[i] = 1;//置1
			flag = true;//已换块
			n = i;
			break;
		}
	}
	if(!flag)//
	{
		//writethough不需要写回主存
		if(!writepolicy && use[n] < 0)//writeback，则写回主存
		{
			;//写回主存
		}
		tag[n] = add;
		use[n] = 1;
	}
	for(int j = 0; j < 1 << numberoflinebit; j ++)//将其他的访问时间加1,0（空位置不变），符号不变（符号表示dirty bit）
	{
		if(j != n && use[j] != 0)
			use[j] < 0? use[j] --: use[j] ++;
	}
}

void Cache::LRU(unsigned long int* tag, int *use, unsigned long int add)
{
	unsigned long int n = 0;//替换地址
	bool flag = false;
	for(int i = 0; i < 1 << numberoflinebit; i ++)
	{
		if(use[i] == 0)//找到空块
		{
			tag[i] = add;
			use[i] = 1;//保持符号位不变绝对值置1
			flag = true;//已换块
			n = i;
			break;
		}
	}
	if(!flag)//
	{
		unsigned long int max = 0;
		for(int  i = 0; i < 1 << numberoflinebit; i ++)
		{
			if(fabs((double)use[i]) > max)
			{
				max = fabs((double)use[i]);
				n = i;
			}
		}
		//换出
		if(!replacepolicy && use[n] < 0)//wrieteback
		{
			;//写主存
		}
		//换入
		tag[n] = add;
		use[n] = 1;
	}
	//更新其他块
	for(int j = 0; j < 1 << numberoflinebit; j ++)//将其他的访问时间加1,0（空位置不变），符号不变（符号表示dirty bit）
	{
		if(j != n && use[j] != 0)
			use[j] < 0? use[j] --: use[j] ++;
	}
}


void Cache::updata(unsigned long int pagenum)
{
	unsigned long int tagbit = pagenum >> 2;//物理地址化分为三块。第一块长度为tagbit长度为24-行内地址-组地址（只有7位），
	//（因此传进的12位页号前面的7位是tag位其余5位是cache组号的高5位还有低9位没有给出需要进行循环查找）
	//第三块为行内地址长度为cachelinesize（为3），第二块是组地址，长度为setbit，组地址长度是cachesize - linebit（为16-2= 14）
	unsigned long int sethbit = (tagbit << 30) >> 23;//将高位的30位移除,然后移回原有位置，将低7位空出
	for(int i = 0; i < 1 << 7; i ++, sethbit ++)
	{
		for(int k = 0; k < 4; k ++)//循环遍历组内寻找tag
		{
			if(use[sethbit][k] != 0 && tag[sethbit][k] == tagbit)
			{
				if(use[sethbit][k] < 0)//dirty
					;//写到主存
				use[sethbit][k] == 0;//换出将其置空
			}
		}
	}
}

void Cache::clear()
{
	for(int i = 0; i < 1 << numberofsetbit; i ++)
		for(int j = 0; j < 1 << numberoflinebit; j ++)
		{
			tag[i][j] = 0;
			use[i][j] = 0;
		}
}

void Cache::printTagarray(ofstream &out)
{
	out << "tag:*************" << endl;
	for(int i = 0; i < 1 << numberofsetbit; i ++)
	{
		for(int j = 0; j < 1 << numberoflinebit; j ++)
			out << tag[i][j]  << ' ';
		out << endl;
	}
	out << endl;
}