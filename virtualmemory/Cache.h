#pragma once
#include<bitset>
#include<fstream>
#include<iostream>
using namespace std;
//author @郭瑞军；
//time 2013-11-6；
//studentID 121220028；
//V4.0；
//说明：本程序按照一个组相连的cache来写，当直接相连时则就是把每组中的行数，也即路数设为1，
//当全相连时则就是把组数设为1,也即路数就等于行数
//为了配合虚拟存储整个系统我在2.0版本的cache的基础上做了修改，将访问数据部分移除了cache，只作为一个cache可进行逐条访问
//我还把用来记录tag的存储结构进行了重新调整，使得程序更加简洁
class Cache
{
	unsigned long int **tag;//采用二二维数组表示tag array
	int **use;//记录使用时间和是否是dirty bit
	//int associativity;//映射方式。1为直接映射，0为全相连，其他数字为组相连的组数；
	bool replacepolicy;//替换策略，true为随机替换，false为LRU算法；
	bool writepolicy;//写策略，true为write through， false为write back；
	//cache大小参数

	int cachelinesize;//大小均用地址需bit数来表示
	int cachesize;

	int numberofsetbit;//全相连则为0；
	int numberoflinebit;//直接相连则为0；

	void random(unsigned long int *tag, int *use, unsigned long int add);//随机替换策略//将一组中的tag传入并修改替换
	void LRU(unsigned long int *tag, int *use, unsigned long int add);//LRU替换策略
public:
	Cache();
	Cache(int associativity,bool replacepolicy, bool writepolicy, int cachesize, int cachelinesize);
	~Cache();
	void printTagarray(ofstream &out);
	bool access(unsigned long int add, char lors);//读取访问，hit返回true，miss则执行替换策略并返回false；
	void updata(unsigned long int pagenumber);//清除换出页的内容
	void clear();//相当于关机，cache内容全部清空
};