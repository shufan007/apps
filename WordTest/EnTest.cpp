// EnTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <windows.h>
#include <iostream>
#include "stdio.h"
#include <ctime>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "CEn.h"
using namespace std;
//=======================================================
//	ȫ�ֱ�����

typedef vector< int > VI;

const int SleepTime = 500;		//ʱ����(����)
const int WordBaseKinds = 4;    //�ʿ��������
const int HisSkipLineNum = 13;  // ��ȡ��ʷ��¼ʱ������������

//  �ָ���
string DivideLine1 = "----------------------------------------------";  
string DivideLine2 = "=============================================="; 
string DivideLine3 = "**********************************************"; 

//===================================================================

//-------------------------------------------------------------------
//      ���캯��

CEn::CEn(char* name="My",int choice=1)
{
	//-------------------------------------------------
	//�û��������û��ʿ�ѡ��ʼ��

	MyName = new char[strlen(name)+2]; 
	//strcpy_s(MyName,strlen(name)+2,name);
	char* Blank=" ";
	strcpy(MyName,name);
	strcat(MyName,Blank);

	//-------------------------------------------------
	//�ʿ�ѡ���ʼ��
	WordBaseChoice = choice;
	//-------------------------------------------------
	// �ʿ�����ʼ��

	//ԭʼ�ʿ��и��ִʻ������Ϊ{1320,4792,6715,2512}

	WordBaseName[0] = "cet6";	// �����ʻ�
	WordBaseName[1] = "CGRE";	//���й������дʻ�
	WordBaseName[2] = "GRE";	//GRE�ʻ�
	WordBaseName[3] = "TOEFL";	//�и��ʻ�
	
	//---------------------------------------------------
	// �ļ�������ʼ��

	WordBaseDir   =  ".\\Word Base\\";	
	MyWordBaseDir = ".\\My Word Base\\";  
	MyRecordDir  = ".\\My Record\\";

	//---------------------------------------------------
	//	�ļ�����ʼ��

	char* WordBase_str = " Word Base.txt";
	char* History_str = " History Record.txt";	
	char* RightWrong_str = " Right Wrong.txt";
	char* Wrong_str = " Wrong Record.txt";


	MyStrCat4(MyWordBase,MyWordBaseDir,MyName,
		WordBaseName[WordBaseChoice-1],WordBase_str);

	MyStrCat4(MyRightWrong,MyWordBaseDir,MyName,
		WordBaseName[WordBaseChoice-1],RightWrong_str);

	MyStrCat4(MyHistoryRecord,MyRecordDir,MyName,
		WordBaseName[WordBaseChoice-1],History_str);

	MyStrCat4(MyWrongWords,MyRecordDir,MyName,
		WordBaseName[WordBaseChoice-1],Wrong_str);

}
//-------------------------------------------------------------
//		��������

CEn::~CEn(void)
{
	delete [] MyName;
	
}


//================================================================
//                    ԭʼ�ʿ����뺯��
//================================================================
void CEn::OriginalWordBaseLoad()
{
	RecordFlag = 0;
		
	ifstream InFile;
	char* FileName = new char[30];
	/*
	strcpy_s(FileName,strlen(WordBaseName[WordBaseChoice-1])+1,
	WordBaseName[WordBaseChoice-1]);
	strcat_s(FileName,5,'.txt');*/

	strcpy(FileName,WordBaseDir);	
	strcat(FileName,WordBaseName[WordBaseChoice-1]);
	strcat(FileName,".txt");

	InFile.open(FileName,1);

	
	if (InFile)
	{
		//---------------------------------------------------
		//	��ȡ�ʿ��еĵ��ʺ���˼

		int Num=0;
		for (string s; getline(InFile,s); )
		{
			if ((Num%2)==0)
			{
				Word.push_back(s);
			} 
			else
			{
				Mean.push_back(s);
			}
			
			Num++;
		}
		InFile.close();

		MyWordNum0 = Num/2;
		MyWordNum = MyWordNum0;
		
		TotalWordNum = Num/2;
		RightTimes.assign(MyWordNum,0);
		WrongTimes.assign(MyWordNum,0);

		//	��ʼ��TotalParameter		
		TotalParameter[0] = 0;
		TotalParameter[1] = 0;
		TotalParameter[2] = 0;
		TotalParameter[3] = 0;
		TotalParameter[4] = TotalWordNum;
		TotalParameter[5] = MyWordNum;

		//	��ʼ��HistoryLineNum
		HistoryLine = 0;
		for (int i=0; i<9; i++)
		{
			HistoryLineNum[i]=0;
		}

	} 
	else
	{
		cout<<"File open error!\n"; 
	}

	delete [] FileName;
}


//=============================================================
//                    ���˴ʿ����뺯��
//=============================================================
void CEn::MyWordBaseLoad()
{

	RecordFlag = 1;
	ifstream InFile;

	InFile.open(MyWordBase,1);
	
	if (InFile)
	{
		/* ********************************************************
		�������� ���˼�¼�б���Ĳ�����Ϣ����ʼʱ�䡢�ϴ���ϰ��ʱ�䡢
		ͳ�����ݡ���ʷ������ռ��������ʣ�൥���б�
		********************************************************* */

		//	����ʱ����Ϣ
		getline(InFile,BeginTime);	// ��ʼʱ��	
		getline(InFile,LastTime);	// �ϴ���ϰ��ʱ��

		//---------------------------------------------------------
		//	����ͳ������

		string s;

		getline(InFile,s);		
		
		istringstream sin1(s);

		for (int ia,i=0; sin1>>ia; i++)
		{
			TotalParameter[i] = ia;
		}

		TotalWordNum = TotalParameter[4];	//ԭʼ������
		MyWordNum0 = TotalParameter[5];		//ʣ�൥����
		MyWordNum = MyWordNum0;

		//------------------------------------------------------
		//	������ʷ��¼�еĲ�����Ϣ

		getline(InFile,s);	//	��ʷ��¼������
		istringstream sin2(s);
		HistoryLine = 0;	//	��ʷ��¼����֮��
		for (i=0;sin2>>ia;i++)
		{
			HistoryLineNum[i] = ia;
			HistoryLine +=ia;
		}


		//-------------------------------------------------------
		//	��ȡ�ʿ��еĵ��ʺ���˼,��ͳ�Ƶ�����

		int Num=0;	//	��¼������

		for (i=0; getline(InFile,s); i++)
		{

			if ((i%2)==0)
			{

				Word.push_back(s);
			} 
			else
			{
				Num++;
				Mean.push_back(s);
			}						
		}
		InFile.close();

	} 
	else
	{
		cout<<"MyWordBase File open error!\n"; 
	}

	//----------------------------------------------------
	//	���뵥������ȷ�ʹ�������ļ�¼


	RightTimes.assign(MyWordNum,0);
	WrongTimes.assign(MyWordNum,0);

	ifstream InFile1;
	
	InFile1.open(MyRightWrong,1);

	if (InFile1)
	{		
		string s1;
		int ia;
		
		for (int i=0; i<MyWordNum; i++)
		{
			getline(InFile1,s1);
			istringstream sin3(s1);

			sin3>>ia;
			RightTimes[i]=ia;

			sin3>>ia;
			WrongTimes[i]=ia;
			
		}
		InFile1.close();
		
	}
	else
	{
		cout<<"MyHistoryRecord File open error!\n"; 
	}


	//-------------------------------------------------------------------
	//	������ʷ��¼

	/* **********************************************************************
	����ʷ�������롱ģ�飬���ڴӡ���ʷ��¼���ļ���������Ϣ���Ա㱾����ϰ֮��
	����������ݡ��ü�¼�к��е���ʣ���������Լ��������������ۻ�ͳ�Ƽ�¼��
	���10����ϰ��¼����������ϰ�Ĵ�������10��ʱ����¼ȫ�����룬��������ϰ
	�Ĵ�������10��ʱ���������µ�9�μ�¼��	
	*********************************************************************** */

	ifstream InFile2;
	
	InFile2.open(MyHistoryRecord,1);

	if (InFile2)
	{
		int i;	
		string s2;	
		//----------------------------------------------
		//	�����ۻ���¼���ݣ���ȡ��ʷ����


		for (i=0; i<HisSkipLineNum; i++)
		{
			getline(InFile2,s2);
		}
		
		//InFile2.seekg(HisSkipLineNum,ios::cur);

		//------------------------------------------------
		//	��ȡ�ۻ���¼֮�����ʷ��¼����

		for (i=0; i<HistoryLine; i++)
		{
			getline(InFile2,s2);
			HistoryRecord.push_back(s2);

		}
		InFile2.close();
	}
	else
	{
		cout<<"MyHistoryRecord File open error!\n"; 
	}

}


//=================================================================
//                    �����β��顱����

/******************************************************************
  ThisTest �������������ɡ����β��顱�Ŀ��ƹ��̣����������ͳ�����ݣ�
		Ϊ���������ṩ�������������
*******************************************************************/

void CEn::ThisTest()
{
	ThisWordsNum = 0;			//�����������ʵĸ��� 
	ThisWrongNum = 0;			//��¼���β���������Ĵʻ���� 

	VI HaveChoose;				//���β����м�¼��ѡ�ʻ� 
	int HaveChooseNum = 0;		//���β����м�¼��ѡ�ʻ����  
	
	int ThisDeleteNum = 0;		//����ɾ����ʵĸ��� 
	
	int ThisRightNum = 0;		//��������������ȷ�ĸ��� 
	int ThisCorrectRate;		//������ȷ��
	int ThisHistoryLineNum = 0; //���β�������ʷ��¼��������ռ������
		
	//-------------------------------------------------------------------
	/*********************************************************************
	���ɿ������̣��Ӵʿ������ѡ��һ��������Ϊ������ĵ��ʣ������ٴӴʿ���
	���ѡ���������壬���ϴ����鵥�ʵĴ������4��ѡ��û����顣
	*********************************************************************/

	int c=1;			// ѭ�����Ʊ���
	int n1 = MyWordNum;	// ѭ�����Ʊ���
	char c0[10];		//	c0 �������ԭʼ�����ַ�

	//--------------------------------------------------------------------
	/* c �� n1 Ϊѭ�����Ʊ�����c���������Ƿ�������飬���Ǵӽ��յ�����
	��Ϣ��ת����������cΪ0ʱ���˳����β���,�������������С�
	n1 ��¼�ʿ���ʣ�൥��������ʣ�൥�ʽ���ʱ��ֱ���������������
	--------------------------------------------------------------------*/

	while ( c && (n1>=10))
	{
		//--------------------------------------------------
		//Generate 4 different non-0 numbers
		//	ѡ����Ե���

		srand(time(0));	//�����������ʱ������

		// ���Ե����ڴʿ��е�λ��
		int num0 = rand() % MyWordNum; 
			
		//---------------------------------------
		//	�ر��ظ�ѡ��

		while (FindMember(HaveChoose,num0))	
		{
			num0 = rand() % MyWordNum;
		
		}
		
		//	���Ե������ĸ�ѡ���е�λ��
		int ChoosePos = rand() % 4;	

		// ��¼������ѡ�ʵ�λ��
		HaveChoose.push_back(num0);  
		HaveChooseNum++;

		//--------------------------------------------------
		//	������β��Ե���

		cout<<HaveChooseNum<<".< "
			<<Word[num0]<<" >"<<endl<<endl;

		//--------------------------------------------------
		//	����ѡ��,�����

		for (int i=0; i<4; i++)			
		{
			if (i!=ChoosePos)
			{
				int num = rand() % MyWordNum;

				//---------------------------------------
				//	�ر��ظ�ѡ��

				while (FindMember(HaveChoose,num))
				{
					num = rand() % MyWordNum;
				}
				cout<<"["<<i+1<<"] "<<Mean[num]<<endl;

			}
			else
			{
				cout<<"["<<ChoosePos+1<<"] "
					<<Mean[num0]<<endl;
			}
		}
		//------------------------------------------------
		//	�����Լ���ѡ��

		cout<<endl<<"Your choice: ";
		cin>>c0;			

		//--------------------------------------------------
		//	�쳣���봦��

		/* ******************************************************
		���ȴ��������ַ�������С�ڵ���2��������������������Ƿ�
		����Ҫ�󡣶������볤�ȷ���Ҫ����ַ���������ת��Ϊ������Ϣ��
		��һ���ж������Ƿ�Ϸ����Ƿ����룬����-1
		InputProcess�������������ַ������������ַ���ת��������
		******************************************************** */

		if (strlen(c0)<=2)
		{
			c = InputProcess(c0);				
		}

		/* *************************************************
		���������ַ������ȴ���2�������������ݲ����Ϲ涨�ģ�
		Ҫ���������룬ֱ������Ϸ�Ϊֹ��
		************************************************* */

		while ( (strlen(c0)>2) || c==-1 )
		{
			cout<<"The input should be '0','1','2','3','4'\n";
			cout<<" or '11','22','33','44'\n";
			cout<<"Your choice: ";

			cin>>c0; 


			if (strlen(c0)<=2)
			{
				c = InputProcess(c0);				
			}
		}

		/*  ************************************************
		������������Ƿ�Ϊ˫����'11','22','33','44'��˫����
		��ʾ�ô��Ѿ�����Ϥ�ʣ�����ѡ����ȷ���Խ��ô��ֶ�ɾ��
		����ķ�ʽ�ǣ������������θ�ѡ��ı�š�
		************************************************  */

		bool flag_c = 0;  // ����Ƿ�Ϊɾ����ʵ�����

		//	�ж��Ƿ�Ϊɾ����ʵ����룬����ȡ����ĸ�λ����
		if(c>10)
		{
			flag_c = 1;
			c=c%10;			
		}
		//------------------------------------------
		// ������Ϸ��ҷ�0���򱾴β�����Ч

		if (c)
		{
			ThisWordsNum++;   //���β��Ե��ʼ�1
			n1-=1;			  //��ʣ�ʻ�����1
		}

		Sleep(SleepTime);	//��ͣƬ�̣����û���ӳ

		//------------------------------------------
		//	��ȷѡ�ʴ���

		if (c==(ChoosePos+1))   
		{
			printf("  Right ++ \n");
			Sleep(SleepTime);
			ThisRightNum++;
			
			//	��¼�õ��ʵ��ۻ���ȷ��
			RightTimes[num0]+=1;	

			/*  ************************************************
			//����ɾ����ʣ����ôʱ��ֶ�ɾ�����ߣ�����ȷ�Ĵ���
			//�ȴ���Ĵ�����3ʱ������Ϊ�ô�����,�Ӹ��˴ʿ���ɾ��
			**************************************************  */

			if ( flag_c || (RightTimes[num0]-WrongTimes[num0] == 3))
			{
				ThisDeleteNum++;	//	����ɾ����ʵĸ���
				DeleteSign.push_back(num0);
			} 
		} 
		
		//------------------------------------------
		//	����ѡ�ʴ���

		else if (c)
		{
			printf("  Wrong -- \n");
			Sleep(SleepTime);
			printf("The key is  %d\n",ChoosePos+1);

			//	��¼�õ��ʵ��ۻ�����Ĵ���
			WrongTimes[num0]+=1;
			ThisWrongNum++;
			ThisWrongIdx.push_back(num0);

			Sleep(SleepTime);	//	��ͣƬ�̣����û���ӳ

		}
		cout<<DivideLine1<<endl<<endl;
	}

	//--------------------------------------------------
	//	������β��Եĵ��ʲ�Ϊ0�������ͳ������

	if (ThisWordsNum)
	{
		//-----------------------------------------------
		//	���㱾��ͳ������(��ȷ��)

		ThisCorrectRate = ThisRightNum*100/ThisWordsNum;
		
		MyWordNum = MyWordNum0 - ThisDeleteNum;

		//-----------------------------------------------
		//	���β��Ե�ͳ�ƽ��

		ThisParameter[0] = ThisWordsNum;
		ThisParameter[1] = ThisRightNum;
		ThisParameter[2] = ThisWrongNum;
		ThisParameter[3] = ThisCorrectRate;

		//-----------------------------------------------	
		//	�������������β��Ե�ͳ�ƽ��

		cout<<endl<<DivideLine2<<endl;
		printf("The statistical figures are as fallows:\n");
		printf("words  %d\n",ThisWordsNum);
		printf("right  %d\n",ThisRightNum);
		printf("wrong  %d\n",ThisWrongNum);
		printf("accuracy  %d%%\n\n",ThisCorrectRate);
		
		printf("words left  %d\n\n", MyWordNum);
		//-----------------------------------------------
		cout<<DivideLine1<<endl;

		//-----------------------------------------------
		//	������β���������Ĵ�
		
		if (ThisWrongNum)	
		{
			printf("The words you had done wrong are as follows:\n\n");
			for (int j=0;j<ThisWrongNum;j++)
			{
				cout<<Word[ThisWrongIdx[j]]<<"  "
					<<Mean[ThisWrongIdx[j]]<<endl;
			}
			
		} 
		cout<<DivideLine2<<endl;
		
		
		//-----------------------------------------------
		//	�����ۼƸ��˼�¼
		
		for (int i=0; i<3; i++)
		{
			TotalParameter[i] += ThisParameter[i];
		}
		TotalParameter[3] = TotalParameter[1]*100/TotalParameter[0];
		TotalParameter[5] =  MyWordNum;
		
		ThisHistoryLineNum = 7;

		//-----------------------------------------------
		//	���㱾�β�������ʷ��¼����ռ������

		if (ThisWrongNum)	
		{
			ThisHistoryLineNum = 8+ThisWrongNum;
		} 

		//-----------------------------------------------
		//	����ÿ����¼������,ȥ�����һ����¼�������²����ļ�¼

		for (i=8; i>0; i--)
		{
			HistoryLineNum[i] = HistoryLineNum[i-1];
		}

	}


	HistoryLineNum[0] = ThisHistoryLineNum;

}


//=============================================================
//                    ��ʷ��¼���溯��
//=============================================================

void CEn::MyHistorySave()
{
	//--------------------------------------------------
	//	��ȡ��ǰʱ��

	time_t t = time(0); 

	//char date[12];		//��ʱ����Ϣȡ����
	//strftime( date, sizeof(date), "%Y/%m/%d",localtime(&t) );

	char tmp0[20];
	strftime( tmp0, sizeof(tmp0), "%Y/%m/%d/%X",localtime(&t) ); 
	string tmp(tmp0,16);	//��ʱ����Ϣȡ������

	if (!RecordFlag)
	{
		BeginTime = tmp;
	} 
	LastTime = tmp;

	
	//-----------------------------------------------
	//	����ʷ��¼�ı���������Ե�ͳ�ƽ��

	ofstream ocout; //����ļ�
    ocout.open(MyHistoryRecord); 

	//-----------------------------------------------
	// ���� ����ۻ���¼

	ocout<<endl<<"words left: < "<< MyWordNum<<" > ";
	ocout<<"(total: "<<TotalWordNum<<" )"<<endl;

	ocout<<DivideLine3<<endl;

	if (!BeginTime.compare(tmp))
	{
		ocout<<"\naccumulated records at ";
		ocout<<endl<<BeginTime<<endl;
	} 
	else
	{
		ocout<<"\naccumulated records:\n";
		ocout<<"From "<<BeginTime<<" To "<<tmp<<endl;
	}
	ocout<<DivideLine1<<endl;

	ocout<<"words  "<<TotalParameter[0]<<endl;
	ocout<<"right  "<<TotalParameter[1]<<endl;
	ocout<<"wrong  "<<TotalParameter[2]<<endl;
	ocout<<"accuracy  "<<TotalParameter[3]<<"%"<<endl;

	ocout<<DivideLine3<<endl;
	ocout<<DivideLine1<<endl;


	/***********************************************/
	/*  ����˴ʿ��л�ʣ��󼸸��ʣ���ֱ�������
		�����˴ʿ��л�ʣ���ٵļ�����ʱ�����˹���  */
	/***********************************************/

	if (MyWordNum<10)
	{
		cout<<"The last "<<MyWordNum<<" words : "<<endl;
		ocout<<"The last "<<MyWordNum<<" words : "<<endl;
		for (int i=0;i<MyWordNum;i++)
		{
			cout<<Word[i]<<"  "<<Mean[i]<<endl;
			ocout<<Word[i]<<"  "<<Mean[i]<<endl;
		}
	}

	//-----------------------------------------------
	//	�������¼�¼�����β��Լ�¼��

	ocout<<tmp<<endl<<endl;
	ocout<<"words  "<<ThisParameter[0]<<endl;
	ocout<<"right  "<<ThisParameter[1]<<endl;
	ocout<<"wrong  "<<ThisParameter[2]<<endl;
	ocout<<"accuracy  "<<ThisParameter[3]<<"%"<<endl;

	//-----------------------------------------------
	//	������β���������Ĵ�

	if (ThisWrongNum)
	{
		ocout<<endl;
		for (int j=0; j<ThisWrongNum; j++)
		{
			ocout<<Word[ThisWrongIdx[j]]<<"  "
				<<Mean[ThisWrongIdx[j]]<<endl;
		}

	} 
	ocout<<DivideLine1<<endl;

	//-----------------------------------------------
	//	�������ɼ�¼�����β���֮ǰ�ļ�¼��


	if (RecordFlag)
	{
		for (int i=0;i<HistoryLine;i++)
		{
			ocout<<HistoryRecord[i]<<endl;

		}
	} 

	ocout.close();

}


//=============================================================
//                    ��ʿⱣ�溯��
//============================================================= 

void CEn::MyWrongSave()  
{	

	if (ThisWrongNum)
	{
		ofstream ocout;	
		ocout.open(MyWrongWords); 

		int WrongNum = 0;
		string s;

		/*****************************************************************/
		/* ����ĳ���������ڴʿ��в�����Ҫ�����ڴ�ʿ��еĵ��ʣ�
			���ĳ�����ʵ����������0�����Ҳ��ǽ���ɾ���ĵ��ʣ��򽫸ôʼ��� 
		 	��������ʼ�¼�У�ͬʱ������Ӧ�����������ͳ�����д�ʸ��� */
		/*****************************************************************/

		for (int i=0; i<MyWordNum0; i++)
		{
			if (WrongTimes[i] && (!FindMember(DeleteSign,i)))
			{
				//	���ӵ��ʼ�����˼����Ϊһ���ַ���
				s = Word[i] + "   " + Mean[i];

				WrongWord.insert(pair<int, string>(WrongTimes[i], s));

				WrongNum++;

			}
			
		}

		//	����������Ϣ�������ʿ��ļ�
		ocout<<endl<<DivideLine2<<endl<<endl;

		//	�������ʱ����Ϣ
		ocout<<"From  "<<BeginTime<<endl;
		ocout<<"Until "<<LastTime<<endl<<endl;

		//	�����ʿ��������дʵ�����
		ocout<<"Total words: < "<<WrongNum<<" >"<<endl;

		ocout<<DivideLine1<<endl;
		ocout<<"The words you had done wrong are as follows:"
			<<endl<<endl;

		/*************************************************************/
		/*  ����ĳ��������������������ʵ��б��Լ���Ӧ����Ĵ�����
			 ���м�¼��Ŀ�����������������						 	*/
		/*************************************************************/

		multimap< int, string, greater<int> >::iterator iter;

		for(iter = WrongWord.begin(); iter != WrongWord.end(); iter++)		
		{
			
			ocout<<iter->first<<" "<<iter->second<<endl;
			
		}

		ocout.close();

	}
		
}

//=============================================================
//					�ҵĴʿⱣ�溯��
//=============================================================

void CEn::MyWordBaseSave()	  
{
	ofstream ocout; 
    ocout.open(MyWordBase); 
	ocout<<BeginTime<<endl;
	ocout<<LastTime<<endl;

	// ���ͳ�Ʋ���
	for (int i=0; i<6; i++)
	{
		ocout<<TotalParameter[i]<<" ";
		
	}
	ocout<<endl;

	//-----------------------------------------------
	//	�����ʷ��¼������

	for (i=0; i<9; i++)
	{
		ocout<<HistoryLineNum[i]<<" ";

	}

	ocout<<endl;

	//-----------------------------------------------
	//	������ʼ�����˼

	for (i=0; i<MyWordNum0; i++)
	{
		if (!FindMember(DeleteSign,i))
		{
			ocout<<Word[i]<<endl;
			ocout<<Mean[i]<<endl;
		}
	}
	ocout.close();

	//-----------------------------------------------
	//	������ȷ�ʹ�������ļ�¼

	ofstream ocout2; 
    ocout2.open(MyRightWrong); 
	

	for (i=0; i<MyWordNum0; i++)
	{
		if (!FindMember(DeleteSign,i))
		{
			ocout2<<RightTimes[i]<<" "
				<<WrongTimes[i]<<endl;
		}

	}
	ocout2.close();

}


//==========================�Ӻ�����==============================

/*************************************************************/
/*  MyStrCat4 ��������4���ַ�������������ַ������ӵ�
		Ŀ���ַ���dest�У� ����������Ӧ���ļ���				*/
/*************************************************************/

void CEn::MyStrCat4(char* dest,char* str1,
					char* str2,char* str3,char* str4) 
{
	/*
	strcpy_s(dest,strlen(str1)+1,str1);
	strcat_s(dest,strlen(str2)+1,str2);
	strcat_s(dest,strlen(str3)+1,str3);
	*/

	strcpy(dest,str1);
	strcat(dest,str2);
	strcat(dest,str3);
	strcat(dest,str4);
}

//-------------------------------------------------------------------------
/*************************************************************/
/*  FindMember �������в���Ԫ��:�������Ϊ�ջ���û��Ҫ����   */
/*		��Ԫ�أ�����0�����򷵻��±�+1						*/
/************************************************************/

int CEn::FindMember(vector<int> v,int mem) 
{
	if (v.empty())
	{
		return 0;
	}
	vector< int >::iterator it;
	for (it = v.begin(); it != v.end(); ++it)
	{	
		if (*it==mem)
		{
			return (*it)+1;
		}
	}
	return 0;
	
}

//----------------------------------------------------------------------
/*************************************************************/
/* InputProcess �������������ַ������������ַ���ת��������  */
/*	 ���ڷǷ����룬����-1									*/
/************************************************************/

int  CEn::InputProcess(char* str)
{
	int c;
	if (*str=='a'||*str=='b'||*str=='c'||*str=='d')
	{
		c = *str-'a'+1;
	} 
	else if(*str=='0'||*str=='1'||*str=='2'||*str=='3'||*str=='4') 
	{
		c = atoi(str);		// �������ַ���ת��Ϊ����
	}
	else
	{
		c=-1;
	}

	return c;

}

//----------------------------------------------------------------------

