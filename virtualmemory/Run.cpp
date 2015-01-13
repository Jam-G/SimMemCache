#include"StoreSystem.h"
#include<iostream>
#include<fstream>
//#include"com.h"
//ofstream foutlog("E:/guoruijun/testlog.txt");//����ʹ��
//int counter = 0;//����ʹ��
using namespace std;
//ofstream loger("E:/guoruijun/notime/cheack.txt");
void start();
void printmyinfo();
//int pagetableaccess = 0;
//int pagetablehi = 0;
int main()
{
	start();
	system("pause");
	return 0;
}

void start()
{
	printmyinfo();
	StoreSystem *ss = NULL;
	StoreSystem * getStoreSystem();
	ss = getStoreSystem();
	while(true)
	{
		cout << "<Info>instruction:  eixt or start>";
		string ins;
		cin >> ins;
		string tpath;
		if(ins == "exit")
			break;
		else if(ins == "start")
		{
			cout << "please input the basic path:";
			string bpath;
			cin >> bpath;
			while(true)
			{
				cout << "exit or test>";
				string eors;
				cin >> eors;
				if(eors == "exit")
					break;
				else if(eors == "test")
				{
					int load = 0, store = 0;//��¼���ķ��ʴ���(tlb�ķ��ʴ�����cache�ķ��ʴ������ܵķ��ʴ�����һ���ġ�pagetable �ķ��ʴ�������tlb��miss����	)
					int tlb_hit = 0, pagetable_hit = 0, cache_hit = 0;
					cout << "please input  the name of test file>";
					cin >> tpath;
					ifstream fin(bpath + "/" + tpath);
					ofstream fout(bpath + "/" + tpath + ".memory.log");
					void memorysystemaccesss(StoreSystem &ss, ifstream &fin, int &load, int &store, int &tlb_hit, int &pagetable_hit, int &cache_hit);
					cout  << "���ڴ����������Ժ�=^_^=" << endl;
					memorysystemaccesss(*ss, fin, load, store, tlb_hit, pagetable_hit, cache_hit);
			//cout << pagetableaccess << endl;
			//cout << pagetablehi << endl;
					fout << "\t*********************************************" << endl;
					fout <<"\t**total  access  times: " << dec << load + store << endl;
					fout << "\t**load           times: " << dec << load << endl;
					fout << "\t**store          times: " << dec << store << endl;
					fout << "\t**tlb    hit     times: " << dec << tlb_hit  << endl;
					fout << "\t**tlb      hit    rate: " << dec << tlb_hit/(double)(load + store) << endl;
					fout << "\t**pagetable     access: " << dec << load + store - tlb_hit << endl;
					fout << "\t**pagetable        hit: "<< dec << pagetable_hit << endl;
					fout << "\t**true  pagetable  hit: " << dec << pagetable_hit + tlb_hit << endl;
					fout << "\t**page          faults: " << dec << load + store - tlb_hit - pagetable_hit << endl;
					fout << "\t**pagetable  hit  rate: " << dec << (pagetable_hit + tlb_hit)/(double)(load + store) << endl;
					fout << "\t**cache    hit   times: " << dec << cache_hit  << endl;
					fout << "\t**cache    hit    rate: " << dec << cache_hit/(double)(load + store) << endl;
					fout << "\t*********************************************" << endl;
					cout << "�������=^_^=         \n���д��: " << bpath + "/" + tpath + ".memory.log " << endl;
					ss ->clear();
				}
			}
		}
	}
	delete ss;
}


void memorysystemaccesss(StoreSystem &ss, ifstream &fin, int &load, int &store, int &tlb_hit, int &pagetable_hit, int &cache_hit)
{
	
	char tempch;//����
	while(fin >> tempch)
	{
		bool ret[3] = {false, false, false};
		if(tempch == 's' || tempch == 'l')
		{
			if(tempch == 'l')
				load ++;
			else if(tempch == 's')
				store ++;
			unsigned long int add = 0;//���ʵ�ַ
			fin >> hex >> add;
			ss.access(add,  tempch, ret);
			if(ret[0] == true)
				tlb_hit ++;
			if(ret[1] == true)
				pagetable_hit ++;
			if(ret[2] == true)
			//{
				cache_hit ++;
				//loger << "cache hit" << endl;
			//}
			//else
				//loger << "cache miss" << endl;
		}
		else
		{
			cerr << "�������ݸ�ʽ����" << endl;
			exit(1);
		}
	}
}

void printmyinfo()
{
	cout << "//****************author @����� & Student ID @121220028*********************//" << endl;
	cout << "//****************vision @memorysystem_1.0 & with cache vision_4.0********//" << endl;
}


StoreSystem * getStoreSystem()
{
	cout << "please choose default or reset:";
	string dorr;
	cin >> dorr;
	if(dorr == "default")
		return new StoreSystem;
	else
	{
		int vaddl = 0, paddl = 0, pagesize = 0, tlbentry = 0, tlbways = 0, cachesize = 0, cachelinesize = 0,cacheways = 0;
		cout << "please input virtual address lenth:";
		cin >> vaddl;
		cout << "please input physical address lenth:";
		cin >> paddl;
		cout << "please input he page size(KB):";
		cin >> pagesize;
		cout << "please input the number of TLB entrys:";
		cin >> tlbentry;
		cout << "please input the TLB ways:";
		cin >> tlbways;
		cout << "please input the cache size(KB):";
		cin >> cachesize;
		cout  << "please input the cache line size(B):";
		cin >> cachelinesize;
		cout << "please input the ways of cache:";
		cin >> cacheways;
		return new StoreSystem(vaddl, paddl, pagesize, tlbentry, tlbways, cachesize, cachelinesize,cacheways);
	}
}