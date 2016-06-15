
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include "CEn.h"
using namespace std;
//=================================================================
extern int SleepTime;
// 分割线
extern string DivideLine1; 
extern string DivideLine2; 
extern string DivideLine3; 
//-----------------------------------------------------------------
void main()
{
	//	Name 用于接收输入的名字字符串
	char* Name = new char[20];

	cout<<DivideLine2<<endl<<endl;

	//	输入名字
	cout<<"Please input your name: ";
	cin>>Name;  cout<<endl; 

	cout<<"Please choose the word base you want to learn\n\n";

	cout<<"[1].六级词汇\n[2].考研词汇\n[3].GRE 词汇\n[4].TOEFL词汇\n\n";

	// ----------------------------------------------
	// 选择词库

	int MyChoice;
	char Input;

	cout<<"Input your choice: ";

	cin>>Input;	cout<<endl;

	if (Input=='a'||Input=='b'||Input=='c'||Input=='d')
	{
		MyChoice = Input-'a'+1;
	} 
	else if(Input=='1'||Input=='2'||Input=='3'||Input=='4')
	{
		MyChoice = Input-'0';
	}
	else	// 非法输入
	{
		MyChoice = 0;
	}

	// ----------------------------------------------
	//	处理非法输入

	while (MyChoice==0)		
	{
		cout<<"The number should be '1','2','3' or '4'\n";
		cout<<"Input your choice again: ";
		cin>>Input;	cout<<endl;
		
		if (Input=='a'||Input=='b'||Input=='c'||Input=='d')
		{
			MyChoice = Input-'a'+1;
		} 
		else if(Input=='1'||Input=='2'||Input=='3'||Input=='4')
		{
			MyChoice = Input-'0';
		}
		else
		{
			MyChoice = 0;
		}

	}

//-----------------------------------------------------
//	创建对象

	CEn MyEn(Name,MyChoice);	
	delete [] Name;

/******************************************************************
	获取记录文件信息,导入相应的词库信息记录，
	若个人记录文件存在则读取，否则，读取原始词库                                                                   
*******************************************************************/

	ifstream MyInFile; 
	MyInFile.open(MyEn.MyWordBase,1); 

	if (MyInFile)
	{
		MyEn.MyWordBaseLoad();

		cout<<endl<<DivideLine3<<endl;

		cout<<"The last time you did the test:  ";
		cout<<MyEn.LastTime<<endl;

		cout<<"\nwords left:  "<<MyEn.MyWordNum0<<endl;

		MyInFile.close();

	}
	//---------------------------------------------------------------
	//	若不存在我的个人词库记录，则使用原始词库

	else    
	{
		printf("This is the fist time you do the test of this word base!\n");
		
		printf("\n**************************** Read Me *******************************\n");

		printf(" Please choose the right chinese meaning to each word from '1','2',\n");
		printf(" '3'and '4' options. If you want to delete the word(s) which you \n");
		printf(" are familiar with  from your Word Base, Enter '11','22','33'and \n");
		printf(" '44' instand of '1','2','3'and '4'. Enter '0' to exit the test.\n");

		printf("\n********************************************************************\n");

		//---------------------------------------------------------------
		//	载入原始词库

		MyEn.OriginalWordBaseLoad(); 

		//	输出该词库中剩余单词数
		cout<<"\nwords: "<<MyEn.MyWordNum0<<endl;

	}

	cout<<DivideLine1<<endl;

	//---------------------------------------------------------------
	//	进行本次练习

	MyEn.ThisTest();	

	/*	*********************************************************
	如果本次测试中所做的单词数不为0，则保存历史记录和我的词库信息
	
	*********************************************************** */
	
	if (MyEn.ThisWordsNum)
	{
		
		MyEn.MyHistorySave();	//保存历史记录
		MyEn.MyWrongSave();		//保存错词库记录		
		MyEn.MyWordBaseSave();	 //保存我的词库信息
	}

	//---------------------------------------------------------------
	//程序退出控制

	//Sleep(10*SleepTime);
	char a[10];
	cout<<endl<<"Enter '0' to exit!"<<endl;
	while (cin>>a)
	{
		return;
	}

}
