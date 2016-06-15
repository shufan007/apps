// EnTest.cpp : 定义控制台应用程序的入口点。
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
//	全局变量区

typedef vector< int > VI;

const int SleepTime = 500;		//时间间隔(毫秒)
const int WordBaseKinds = 4;    //词库的种类数
const int HisSkipLineNum = 13;  // 读取历史记录时须跳过的行数

//  分割线
string DivideLine1 = "----------------------------------------------";  
string DivideLine2 = "=============================================="; 
string DivideLine3 = "**********************************************"; 

//===================================================================

//-------------------------------------------------------------------
//      构造函数

CEn::CEn(char* name="My",int choice=1)
{
	//-------------------------------------------------
	//用户名初，用户词库选项始化

	MyName = new char[strlen(name)+2]; 
	//strcpy_s(MyName,strlen(name)+2,name);
	char* Blank=" ";
	strcpy(MyName,name);
	strcat(MyName,Blank);

	//-------------------------------------------------
	//词库选择初始化
	WordBaseChoice = choice;
	//-------------------------------------------------
	// 词库名初始化

	//原始词库中各种词汇的数量为{1320,4792,6715,2512}

	WordBaseName[0] = "cet6";	// 六级词汇
	WordBaseName[1] = "CGRE";	//（中国）考研词汇
	WordBaseName[2] = "GRE";	//GRE词汇
	WordBaseName[3] = "TOEFL";	//托福词汇
	
	//---------------------------------------------------
	// 文件夹名初始化

	WordBaseDir   =  ".\\Word Base\\";	
	MyWordBaseDir = ".\\My Word Base\\";  
	MyRecordDir  = ".\\My Record\\";

	//---------------------------------------------------
	//	文件名初始化

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
//		析构函数

CEn::~CEn(void)
{
	delete [] MyName;
	
}


//================================================================
//                    原始词库载入函数
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
		//	读取词库中的单词和意思

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

		//	初始化TotalParameter		
		TotalParameter[0] = 0;
		TotalParameter[1] = 0;
		TotalParameter[2] = 0;
		TotalParameter[3] = 0;
		TotalParameter[4] = TotalWordNum;
		TotalParameter[5] = MyWordNum;

		//	初始化HistoryLineNum
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
//                    个人词库载入函数
//=============================================================
void CEn::MyWordBaseLoad()
{

	RecordFlag = 1;
	ifstream InFile;

	InFile.open(MyWordBase,1);
	
	if (InFile)
	{
		/* ********************************************************
		首先载入 个人记录中保存的参数信息：起始时间、上次练习的时间、
		统计数据、历史数据所占的行数、剩余单词列表
		********************************************************* */

		//	载入时间信息
		getline(InFile,BeginTime);	// 起始时间	
		getline(InFile,LastTime);	// 上次练习的时间

		//---------------------------------------------------------
		//	载入统计数据

		string s;

		getline(InFile,s);		
		
		istringstream sin1(s);

		for (int ia,i=0; sin1>>ia; i++)
		{
			TotalParameter[i] = ia;
		}

		TotalWordNum = TotalParameter[4];	//原始单词数
		MyWordNum0 = TotalParameter[5];		//剩余单词数
		MyWordNum = MyWordNum0;

		//------------------------------------------------------
		//	载入历史记录中的参数信息

		getline(InFile,s);	//	历史记录的行数
		istringstream sin2(s);
		HistoryLine = 0;	//	历史记录行数之和
		for (i=0;sin2>>ia;i++)
		{
			HistoryLineNum[i] = ia;
			HistoryLine +=ia;
		}


		//-------------------------------------------------------
		//	读取词库中的单词和意思,并统计单词数

		int Num=0;	//	记录单词数

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
	//	载入单词做正确和错误次数的记录


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
	//	载入历史记录

	/* **********************************************************************
	“历史数据载入”模块，用于从“历史记录”文件中载入信息。以便本次练习之后
	保存更新数据。该记录中含有单词剩余数量（以及单词总数）、累积统计记录、
	最近10次练习记录。当已做练习的次数少于10次时，记录全部载入，当已做练习
	的次数多于10次时，载入最新的9次记录。	
	*********************************************************************** */

	ifstream InFile2;
	
	InFile2.open(MyHistoryRecord,1);

	if (InFile2)
	{
		int i;	
		string s2;	
		//----------------------------------------------
		//	跳过累积记录数据，读取历史数据


		for (i=0; i<HisSkipLineNum; i++)
		{
			getline(InFile2,s2);
		}
		
		//InFile2.seekg(HisSkipLineNum,ios::cur);

		//------------------------------------------------
		//	读取累积记录之后的历史记录数据

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
//                    “本次测验”函数

/******************************************************************
  ThisTest 函数：用于生成“本次测验”的控制过程，并计算各种统计数据，
		为其它函数提供计算所需的数据
*******************************************************************/

void CEn::ThisTest()
{
	ThisWordsNum = 0;			//本次所做单词的个数 
	ThisWrongNum = 0;			//记录本次测试中做错的词汇个数 

	VI HaveChoose;				//本次测试中记录已选词汇 
	int HaveChooseNum = 0;		//本次测试中记录已选词汇个数  
	
	int ThisDeleteNum = 0;		//本次删除熟词的个数 
	
	int ThisRightNum = 0;		//本次所做单词正确的个数 
	int ThisCorrectRate;		//本次正确率
	int ThisHistoryLineNum = 0; //本次测试在历史记录中至少所占的行数
		
	//-------------------------------------------------------------------
	/*********************************************************************
	生成控制流程：从词库中随机选择一个单词作为待测验的单词，另外再从词库中
	随机选择三个词义，加上待测验单词的词义组成4个选项供用户测验。
	*********************************************************************/

	int c=1;			// 循环控制变量
	int n1 = MyWordNum;	// 循环控制变量
	char c0[10];		//	c0 用来存放原始输入字符

	//--------------------------------------------------------------------
	/* c 和 n1 为循环控制变量，c用来控制是否结束测验，它是从接收的输入
	信息中转化而来，当c为0时，退出本次测验,否则测验继续进行。
	n1 记录词库中剩余单词数，当剩余单词较少时则直接输出，结束测验
	--------------------------------------------------------------------*/

	while ( c && (n1>=10))
	{
		//--------------------------------------------------
		//Generate 4 different non-0 numbers
		//	选择测试单词

		srand(time(0));	//产生随机数的时间种子

		// 测试单词在词库中的位置
		int num0 = rand() % MyWordNum; 
			
		//---------------------------------------
		//	回避重复选词

		while (FindMember(HaveChoose,num0))	
		{
			num0 = rand() % MyWordNum;
		
		}
		
		//	测试单词在四个选项中的位置
		int ChoosePos = rand() % 4;	

		// 记录本次所选词的位置
		HaveChoose.push_back(num0);  
		HaveChooseNum++;

		//--------------------------------------------------
		//	输出本次测试单词

		cout<<HaveChooseNum<<".< "
			<<Word[num0]<<" >"<<endl<<endl;

		//--------------------------------------------------
		//	生成选项,并输出

		for (int i=0; i<4; i++)			
		{
			if (i!=ChoosePos)
			{
				int num = rand() % MyWordNum;

				//---------------------------------------
				//	回避重复选词

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
		//	输入自己的选择

		cout<<endl<<"Your choice: ";
		cin>>c0;			

		//--------------------------------------------------
		//	异常输入处理

		/* ******************************************************
		首先处理输入字符串长度小于等于2的情况，检查输入的内容是否
		符合要求。对于输入长度符合要求的字符串，将其转化为数字信息，
		进一步判断内容是否合法，非法输入，返回-1
		InputProcess用来处理输入字符串，将输入字符串转化成数字
		******************************************************** */

		if (strlen(c0)<=2)
		{
			c = InputProcess(c0);				
		}

		/* *************************************************
		对于输入字符串长度大于2，或者输入内容不符合规定的，
		要求重新输入，直到输入合法为止。
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
		标记输入数字是否为双数字'11','22','33','44'，双数字
		表示该词已经是熟悉词，本次选择正确可以将该词手动删除
		输入的方式是，连续输入两次该选项的标号。
		************************************************  */

		bool flag_c = 0;  // 标记是否为删除熟词的输入

		//	判断是否为删除熟词的输入，并提取输入的个位数字
		if(c>10)
		{
			flag_c = 1;
			c=c%10;			
		}
		//------------------------------------------
		// 当输入合法且非0，则本次测试有效

		if (c)
		{
			ThisWordsNum++;   //本次测试单词加1
			n1-=1;			  //所剩词汇数减1
		}

		Sleep(SleepTime);	//暂停片刻，供用户反映

		//------------------------------------------
		//	正确选词处理

		if (c==(ChoosePos+1))   
		{
			printf("  Right ++ \n");
			Sleep(SleepTime);
			ThisRightNum++;
			
			//	记录该单词的累积正确数
			RightTimes[num0]+=1;	

			/*  ************************************************
			//本次删除熟词，当该词被手动删除或者，做正确的次数
			//比错误的次数多3时，就认为该词已熟,从个人词库中删除
			**************************************************  */

			if ( flag_c || (RightTimes[num0]-WrongTimes[num0] == 3))
			{
				ThisDeleteNum++;	//	本次删除熟词的个数
				DeleteSign.push_back(num0);
			} 
		} 
		
		//------------------------------------------
		//	错误选词处理

		else if (c)
		{
			printf("  Wrong -- \n");
			Sleep(SleepTime);
			printf("The key is  %d\n",ChoosePos+1);

			//	记录该单词的累积做错的次数
			WrongTimes[num0]+=1;
			ThisWrongNum++;
			ThisWrongIdx.push_back(num0);

			Sleep(SleepTime);	//	暂停片刻，供用户反映

		}
		cout<<DivideLine1<<endl<<endl;
	}

	//--------------------------------------------------
	//	如果本次测试的单词不为0，则计算统计数据

	if (ThisWordsNum)
	{
		//-----------------------------------------------
		//	计算本次统计数据(正确率)

		ThisCorrectRate = ThisRightNum*100/ThisWordsNum;
		
		MyWordNum = MyWordNum0 - ThisDeleteNum;

		//-----------------------------------------------
		//	本次测试的统计结果

		ThisParameter[0] = ThisWordsNum;
		ThisParameter[1] = ThisRightNum;
		ThisParameter[2] = ThisWrongNum;
		ThisParameter[3] = ThisCorrectRate;

		//-----------------------------------------------	
		//	在命令窗口输出本次测试的统计结果

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
		//	输出本次测试中做错的词
		
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
		//	计算累计个人记录
		
		for (int i=0; i<3; i++)
		{
			TotalParameter[i] += ThisParameter[i];
		}
		TotalParameter[3] = TotalParameter[1]*100/TotalParameter[0];
		TotalParameter[5] =  MyWordNum;
		
		ThisHistoryLineNum = 7;

		//-----------------------------------------------
		//	计算本次测试在历史记录中所占的行数

		if (ThisWrongNum)	
		{
			ThisHistoryLineNum = 8+ThisWrongNum;
		} 

		//-----------------------------------------------
		//	保存每条记录的行数,去掉最后一个记录，加上新产生的记录

		for (i=8; i>0; i--)
		{
			HistoryLineNum[i] = HistoryLineNum[i-1];
		}

	}


	HistoryLineNum[0] = ThisHistoryLineNum;

}


//=============================================================
//                    历史记录保存函数
//=============================================================

void CEn::MyHistorySave()
{
	//--------------------------------------------------
	//	获取当前时间

	time_t t = time(0); 

	//char date[12];		//将时间信息取到天
	//strftime( date, sizeof(date), "%Y/%m/%d",localtime(&t) );

	char tmp0[20];
	strftime( tmp0, sizeof(tmp0), "%Y/%m/%d/%X",localtime(&t) ); 
	string tmp(tmp0,16);	//将时间信息取到分钟

	if (!RecordFlag)
	{
		BeginTime = tmp;
	} 
	LastTime = tmp;

	
	//-----------------------------------------------
	//	在历史记录文本中输出测试的统计结果

	ofstream ocout; //输出文件
    ocout.open(MyHistoryRecord); 

	//-----------------------------------------------
	// 首先 输出累积记录

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
	/*  如果人词库中还剩最后几个词，则直接输出，
		当个人词库中还剩很少的几个词时启动此功能  */
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
	//	其次输出新记录（本次测试记录）

	ocout<<tmp<<endl<<endl;
	ocout<<"words  "<<ThisParameter[0]<<endl;
	ocout<<"right  "<<ThisParameter[1]<<endl;
	ocout<<"wrong  "<<ThisParameter[2]<<endl;
	ocout<<"accuracy  "<<ThisParameter[3]<<"%"<<endl;

	//-----------------------------------------------
	//	输出本次测试中做错的词

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
	//	最后输出旧纪录（本次测试之前的记录）


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
//                    错词库保存函数
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
		/* 下面的程序段用于在词库中查找需要保存在错词库中的单词，
			如果某个单词的做错次数非0，并且不是将被删除的单词，则将该词及其 
		 	词义放入错词记录中，同时放入相应的做错次数，统计所有错词个数 */
		/*****************************************************************/

		for (int i=0; i<MyWordNum0; i++)
		{
			if (WrongTimes[i] && (!FindMember(DeleteSign,i)))
			{
				//	连接单词及其意思，作为一个字符串
				s = Word[i] + "   " + Mean[i];

				WrongWord.insert(pair<int, string>(WrongTimes[i], s));

				WrongNum++;

			}
			
		}

		//	将待保存信息输出到错词库文件
		ocout<<endl<<DivideLine2<<endl<<endl;

		//	首先输出时间信息
		ocout<<"From  "<<BeginTime<<endl;
		ocout<<"Until "<<LastTime<<endl<<endl;

		//	输出错词库中所含有词的数量
		ocout<<"Total words: < "<<WrongNum<<" >"<<endl;

		ocout<<DivideLine1<<endl;
		ocout<<"The words you had done wrong are as follows:"
			<<endl<<endl;

		/*************************************************************/
		/*  下面的程序段用于输出所有做错单词的列表，以及对应做错的次数，
			 所有记录条目按做错次数降序排列						 	*/
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
//					我的词库保存函数
//=============================================================

void CEn::MyWordBaseSave()	  
{
	ofstream ocout; 
    ocout.open(MyWordBase); 
	ocout<<BeginTime<<endl;
	ocout<<LastTime<<endl;

	// 输出统计参数
	for (int i=0; i<6; i++)
	{
		ocout<<TotalParameter[i]<<" ";
		
	}
	ocout<<endl;

	//-----------------------------------------------
	//	输出历史记录的行数

	for (i=0; i<9; i++)
	{
		ocout<<HistoryLineNum[i]<<" ";

	}

	ocout<<endl;

	//-----------------------------------------------
	//	输出单词及其意思

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
	//	保存正确和错误次数的记录

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


//==========================子函数区==============================

/*************************************************************/
/*  MyStrCat4 用于连接4个字符串，将后面的字符串连接到
		目标字符串dest中， 用于生成相应的文件名				*/
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
/*  FindMember 从向量中查找元素:如果向量为空或者没有要查找   */
/*		的元素，返回0，否则返回下标+1						*/
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
/* InputProcess 用来处理输入字符串，将输入字符串转化成数字  */
/*	 对于非法输入，返回-1									*/
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
		c = atoi(str);		// 将数字字符串转化为数字
	}
	else
	{
		c=-1;
	}

	return c;

}

//----------------------------------------------------------------------

