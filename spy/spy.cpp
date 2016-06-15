//#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <ctime>
#include <fstream>
#include <vector>
//#include <map>
#include <sstream>
#include <string>
//#include <wchar.h>
//#include <locale.h>

using namespace std;

//============================
typedef vector<string> VS;
typedef vector<int> VI;

//*****************  Global variables  ******************
int DelayTime = 500;

int SpyRate = 4;  //1/4
const int LimitNumOfWords = 1000;
VS Words1;
VS Words2;

int RecentWordsIndex[LimitNumOfWords];
// used to store the word which have been used
int RecentWordsNumber = 0;

const int LimitNumOfNames = 100;
VS Names;

int NumberOfWords = 0;
int NumberOfNames = 0;
int NumberOfSpy =0;
int WordIndex =0;
bool SpyWordIndex = 0;

int MappingNumber[LimitNumOfNames] = {0};
//indicate which word(1 or 2) the name mapped
int SpyPlayerIndex[LimitNumOfNames] = {0};
bool WordUseUpFlag = 0;
bool NotContinueFlag = 0;

//const string WordBaseStr = "\\word base\\words.txt";
//const string NameStr = "name.txt";
int i = 0;
//========================================
//******************** Functions:***********************

//-Get all the words to array of Words
void GetWords(); 

//-Get all the names to array of Names
void GetNames(); 

//-Choose word pair, and mapping to all the names,
void ChooseWordAndMappingToNames();

 //-Gave the order of all the name as well as  its corresponding word, 
 //display them in the screen
void NameAndWordDisplay();
 
//-Start the progress and and control times of the games
void StartAndTimesControl();
//=======================================

//Get all the words to array of Words
void GetWords()
{
	ifstream InFile;		//get the file of the word base
	InFile.open("words.txt");
	//InFile.open(WordBaseStr);

    if (InFile)
    {
        string s;
        string word;       
	for (i = 0; getline(InFile,s); i++)
	{
		istringstream sin(s);
		sin>>word;
		Words1.push_back(word);
		sin>>word;
		//Words2[i] = word;
		Words2.push_back(word);
	}
	NumberOfWords = i;
	InFile.close();
    }
    else
    {
    	cout<<"WordBase File open error!\n"; 
    }
}

//Get all the names to array of Names
void GetNames()
{
     ifstream InFile;
    //InFile.open(NameStr);
    InFile.open("name.txt");

    if (InFile)
    {
        string s;
        string name;      
        for ( i = 0; getline(InFile,s); i++)
        {
            istringstream sin(s);
            sin>>name;
	     Names.push_back(name);
        }
	NumberOfNames = i;
	InFile.close();
    }
    else
    {
    	cout<<"Name File open error!\n"; 
    }
}

//Choose word pair, and mapping to all the names
void ChooseWordAndMappingToNames()
{
	// judge if the words have used up
	if ( NumberOfWords == RecentWordsNumber)
	{
		system("clear");
		printf("\n*********************************************************\n");
		printf("\n  The words has used up !!\n");
		printf("  and the game should be over !\n");
		printf("  please update the words in  'words.txt' if you want to continue.\n");		
		printf("\n*********************************************************\n");
		Sleep(5*DelayTime);		
		WordUseUpFlag = 1;		
		return ;
	}
	 
	/* ******************************************************************
	-Chose the word pair and spy word at random,
	-repeat some word should be avoid during one game period,
	  so the words which have been used should be stored and check by each word choice
	******************************************************************** */
	srand(time(0));
	WordIndex = rand()%NumberOfWords;
	SpyWordIndex = rand()%2;
	
	i = 0;
	while ( i < RecentWordsNumber)
	{
		if (WordIndex == RecentWordsIndex[i])
		{
			WordIndex = rand()%NumberOfWords;
			i = 0;
		}
		else
		{
			i++;
		}
	}

	// store the word that have been used
	RecentWordsIndex[RecentWordsNumber] = WordIndex;
	RecentWordsNumber++;
	
	//clear MappingNumber
	for (i = 0; i< NumberOfNames; i++)
	{
		MappingNumber[i] = 0;	
	}
	
	/* *********************************************
	-Mapping the spy (by index), 
	  if more than one spy should, should avoid more spies mapping to one person
	********************************************** */
	for (i=0;	 i< NumberOfSpy; i++)
	{
		int thisIndex = rand()%NumberOfNames;
		
		int j=0;
		while(j<i)
		{
			if( thisIndex == SpyPlayerIndex[i])
			{
				thisIndex = rand()%NumberOfNames;
				j=0;				
			}
			else
			{
				j++;
			}
		}	
		MappingNumber[thisIndex] = 1;
		SpyPlayerIndex[i] = thisIndex;		
	}
	
}

//Gave the order of all the name as well as  its corresponding word, display them in the screen
void NameAndWordDisplay()
{
	
	char DisplyWordInput;
	char DisplyNameInput;
	for (i=0; i<NumberOfNames; i++)
	{
		for(int j =0; j<1; j++)
		{
			// clear screen	
			system("clear");	
			printf("\n=============================================\n");	
			Sleep(200);			
			cout<<endl<<"		<  "<<Names[i]<<"  >"<<endl;
		}
		
		Sleep(DelayTime);
		cout<<endl<<"	---------------------------"<<endl;
		cout<<endl<<"	>> Disply the word ? [Y/N] :  ";
		cin>>DisplyWordInput;
		cout<<endl<<"	---------------------------"<<endl;
		
		if ( 'Y' == DisplyWordInput  || 'y' == DisplyWordInput )
		{		
			if (MappingNumber[i] == 1)
			{
				if(0 == SpyWordIndex)
				{
					cout<<endl<<"       		[  "<<Words1[WordIndex]<<"  ] "<<endl;
				}
				else
				{
					cout<<endl<<"       		[ "<<Words2[WordIndex]<<" ] "<<endl;
				}				
			}
			else
			{
				if(0 == SpyWordIndex)
				{
					cout<<endl<<"       		[ "<<Words2[WordIndex]<<" ] "<<endl;
				}
				else
				{
					cout<<endl<<"       		[ "<<Words1[WordIndex]<<" ] "<<endl;
				}
			}					

		}
		else
		{
			NotContinueFlag = 1;		
			//printf("\nNotContinueFlag1 = %d\n",NotContinueFlag);		
			break;
		}

		Sleep(DelayTime);
		printf("\n=============================================\n");
		Sleep(2*DelayTime);
		if (i<NumberOfNames-1)
		{
			cout<<endl<<"	>> Turn to next player ?  [Y/N]:  ";
			cin>>DisplyNameInput;
			if ( 'Y' == DisplyNameInput  || 'y' == DisplyNameInput )
			{
				continue;
			}
			else
			{
				break;
			}
		}				
	}	
}

void StartAndTimesControl()
{
	GetWords();
	GetNames();

	NumberOfSpy = NumberOfNames / SpyRate;
		
	if (NumberOfSpy < 1)
	{
		NumberOfSpy  =1;
	}
	
	char TimesControlInput = 'Y';
	while( 'Y' == TimesControlInput  || 'y' == TimesControlInput)
	{
		ChooseWordAndMappingToNames();
		//printf("\nWordUseUpFlag = %d\n",WordUseUpFlag);	
			
		if (0 == WordUseUpFlag )
		{
			system("clear");
			NameAndWordDisplay();
		}
		else
		{
			return;
		}
		//printf("\nNotContinueFlag2 = %d\n",NotContinueFlag);

		if (0 == NotContinueFlag)
		{
			Sleep(2*DelayTime);		
			system("clear");
			cout<<endl<<"   	>> Disply the spy ?  [Y/N] :  ";
			char DispSpyInput;		
			cin>>DispSpyInput;		
			if ( 'Y' == DispSpyInput  || 'y' == DispSpyInput )
			{
				cout<<endl<<" *************  The spy word is :  **************"<<endl;
			
				if(0 == SpyWordIndex)
				{
					cout<<endl<<"             	[ "<<Words1[WordIndex]<<" ] "<<endl;
				}
				else
				{
					cout<<endl<<"             	[ "<<Words2[WordIndex]<<" ] "<<endl;
				}				
			}
		}
		
		cout<<endl<<" ************************************************"<<endl;

		Sleep(DelayTime);
		cout<<endl<<"   >> Start another round of the game ? [Y/N] :  ";		
		cin>>TimesControlInput;
		
	}
	
}

/* ********************* main() *************************** */
int main()
{
	StartAndTimesControl();
	return 0;
}
/* ****************************************************** */
