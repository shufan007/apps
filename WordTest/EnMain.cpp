
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
// �ָ���
extern string DivideLine1; 
extern string DivideLine2; 
extern string DivideLine3; 
//-----------------------------------------------------------------
void main()
{
	//	Name ���ڽ�������������ַ���
	char* Name = new char[20];

	cout<<DivideLine2<<endl<<endl;

	//	��������
	cout<<"Please input your name: ";
	cin>>Name;  cout<<endl; 

	cout<<"Please choose the word base you want to learn\n\n";

	cout<<"[1].�����ʻ�\n[2].���дʻ�\n[3].GRE �ʻ�\n[4].TOEFL�ʻ�\n\n";

	// ----------------------------------------------
	// ѡ��ʿ�

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
	else	// �Ƿ�����
	{
		MyChoice = 0;
	}

	// ----------------------------------------------
	//	����Ƿ�����

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
//	��������

	CEn MyEn(Name,MyChoice);	
	delete [] Name;

/******************************************************************
	��ȡ��¼�ļ���Ϣ,������Ӧ�Ĵʿ���Ϣ��¼��
	�����˼�¼�ļ��������ȡ�����򣬶�ȡԭʼ�ʿ�                                                                   
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
	//	���������ҵĸ��˴ʿ��¼����ʹ��ԭʼ�ʿ�

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
		//	����ԭʼ�ʿ�

		MyEn.OriginalWordBaseLoad(); 

		//	����ôʿ���ʣ�൥����
		cout<<"\nwords: "<<MyEn.MyWordNum0<<endl;

	}

	cout<<DivideLine1<<endl;

	//---------------------------------------------------------------
	//	���б�����ϰ

	MyEn.ThisTest();	

	/*	*********************************************************
	������β����������ĵ�������Ϊ0���򱣴���ʷ��¼���ҵĴʿ���Ϣ
	
	*********************************************************** */
	
	if (MyEn.ThisWordsNum)
	{
		
		MyEn.MyHistorySave();	//������ʷ��¼
		MyEn.MyWrongSave();		//�����ʿ��¼		
		MyEn.MyWordBaseSave();	 //�����ҵĴʿ���Ϣ
	}

	//---------------------------------------------------------------
	//�����˳�����

	//Sleep(10*SleepTime);
	char a[10];
	cout<<endl<<"Enter '0' to exit!"<<endl;
	while (cin>>a)
	{
		return;
	}

}
