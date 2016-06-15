
#ifdef WIN32
#pragma warning (disable: 4514 4786)
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <string>
#include "DisplayRefresh.h"
#include <windows.h>
using namespace std ;

//========================================================
 extern vector < int > MoveInformationStart; 
 extern vector < int > MoveInformationEnd;

const char FlagDisplay = '*'; //表示盘子的字符
const char FlagDisplaySpace = ' ';
//========================================================

HanioAutoDisplay::HanioAutoDisplay( int DishNumber )
{	
	int i=0,j=0;

	//盘子数初始化
	m_DishNumber =  DishNumber;

	//---------------------------------------------------
	//对每个塔上的盘子信息进行初始化 
	
	m_DishLocationInformation.resize(3);

	for ( i=DishNumber; i > 0; i--)	
	{
		m_DishLocationInformation[0].push_back(i);			
	}

	//---------------------------------------------------
		
	DelayTime = 100 ;
	m_ScreenWidth = DishNumber*4*4 ;
	m_ScreenHeight = DishNumber+4 ;
	int Location = m_ScreenWidth/4;
	m_TowerNumberLoctaion[0] = Location ;
	m_TowerNumberLoctaion[1] = Location *2 ;
	m_TowerNumberLoctaion[2] = Location *3 ;
	m_VerticalStep = 1 ;
	m_TowerHeight = DishNumber+2 ;
	m_FinalHeight = m_ScreenHeight-(m_TowerHeight+m_VerticalStep);

	m_TowerHeightYcoodinate = m_ScreenHeight-m_TowerHeight-1;

	m_Levelstep =4;
	//设置表示盘子的字符串
	string s( ((2<<(m_DishNumber-1))+1),' ');
	m_StrDish.push_back(s) ;	//清除要移动盘子的字符串

	for ( i=1 ; i<= DishNumber ; i++ )
	{
		string s1((2<<(i-1))+1,'*' ); //盘子的*表示，盘子形状
		
		m_StrDish.push_back( s1 ); //m_StrDish存放的是盘子的形状

	}
	//---------------------------------------------------
	//	将屏幕初始化为空屏

	for ( i=0; i<m_ScreenHeight; i++ )
	{
		string s2(m_ScreenWidth, ' ');
		m_HanioScreen.push_back( s2 );
	}
	//---------------------------------------------------
	//	将屏幕的最低下一行用“=”显示
	for ( j=0;j<m_ScreenWidth; j++)
	{
		m_HanioScreen[m_ScreenHeight-1][j] = '=';
	}
		
	//---------------------------------------------------
	//将三个塔在屏幕上显示

	for ( i=0; i<3; i++)
	{
		for (  j=m_ScreenHeight-m_TowerHeight; j< m_ScreenHeight-1 ; j++)
		{
			m_HanioScreen[j][m_TowerNumberLoctaion[i]] = '|' ;
		
		}
		
	}

	//---------------------------------------------------
	//将盘子信息在第一个塔上显示出来，存放在矩阵中

	for ( i=0 ; i < m_DishNumber ; i++ )
	{
		for (  j = 0; j < 2*(DishNumber-i); j++)
		{
			m_HanioScreen[m_ScreenHeight-2-i][ m_TowerNumberLoctaion[0]]
				= FlagDisplay;
			m_HanioScreen[m_ScreenHeight-2-i][ m_TowerNumberLoctaion[0]+j]
				= FlagDisplay;
			m_HanioScreen[m_ScreenHeight-2-i][ m_TowerNumberLoctaion[0]-j]
				= FlagDisplay;

		}
	}

}
/************************************************************************/
/* 显示程序：其显示的内容就是 m_HanioScreen，每次盘子的移动通过更新
	m_HanioScreen 的内容来实现一个动态显示的过程      */
/************************************************************************/
void HanioAutoDisplay::DisplayScreen()
{
	
	for (vector <string>::iterator  iter = m_HanioScreen.begin();
	iter != m_HanioScreen.end(); iter++)
	{
		cout << *iter << endl;
	}
	cout << "The size of the dish :   [ "<<m_dishSize<<" ]" <<endl; 

	return;
}

/************************************************************************/
/* 函数名称： GetTheTowerTopDish( int MoveTimes)   
   函数功能： 获取汉若塔算法中个，盘子的每次移动信息，从哪个塔到哪个塔，及盘子的高度  
   输入参数：MoveTimes，根据MoveTimes的值，获取每次盘子的移动信息，如：当共有3个盘子时，
        盘子在第一次移动时由塔1移到塔3.   
   其塔的信息在MoveInformationStart 和MoveInformationEnd中存放 
   输出参数：DishFromTheTower盘子从该塔移出
             DishToTheTower  盘子移动该塔中 
             DishHeight[0] 为要移动的盘子在DishFromTheTower塔中的位置
             DishHeight[1] 为要移动的盘子在DishToTheTower塔中存放的位置信息
             m_dishSize 获取的是要移动的盘子的编号，从1开始     */
/************************************************************************/

void HanioAutoDisplay::GetTheTowerTopDish( int MoveTimes)
{
	//DishFromTheTower，DishToTheTower为塔号，其范围是1，2，3中的一个数字
	DishFromTheTower = MoveInformationStart[MoveTimes-1];
	DishToTheTower = MoveInformationEnd[MoveTimes-1];

//	DishHeight[0]其范围是从1开始，而不是从0开始
	//m_DishLocationInformation数组，其下标从0开始
	DishHeight[0] = m_DishLocationInformation[DishFromTheTower-1].size();
	DishHeight[1] = m_DishLocationInformation[DishToTheTower-1].size();
	m_dishSize = m_DishLocationInformation[DishFromTheTower-1][DishHeight[0]-1];

	//将要移出的盘子，从塔中删除。
	m_DishLocationInformation[DishFromTheTower-1].pop_back();
	m_DishLocationInformation[DishToTheTower-1].push_back(m_dishSize) ;
	//目的就是建立一个盘子在每个柱子上的状态信息
}
/************************************************************************/
/* 函数名：SetDishShape：设置移动盘子在矩阵中的形状，
   输入参数： SetDishNumShape：盘子的编号信息
              Xcoordinate ：盘子移动时候的横坐标，在上升和下降时，
			     等于塔的横坐标，平移的时候，其坐标在两塔之间
              Ycoordinate ：盘子移动时候的，盘子上升时候的纵坐标，在上升和下降时，
			      其范围在盘子起始的众坐标，与塔的高度加上一个数值之间*/           
/************************************************************************/
void HanioAutoDisplay::SetDishShape(int SetDishNumShape,
									int Xcoordinate,int Ycoordinate)
{
	//根据盘子的编号设置盘子的形状

	for ( int j = 0; j < 2*(SetDishNumShape); j++)
	{	
		m_HanioScreen[Ycoordinate][ Xcoordinate ]   
			= FlagDisplay;
		m_HanioScreen[Ycoordinate][ Xcoordinate+j]
			= FlagDisplay;
		m_HanioScreen[Ycoordinate][ Xcoordinate-j]
			= FlagDisplay;
		
		}
}
/************************************************************************/
/* 函数名：ClearDishShape：清除移动盘子在矩阵中的形状，
   输入参数： ClearDishNumShape：盘子的编号信息
              Xcoordinate ：盘子移动时候的横坐标，在上升和下降时，
				等于塔的横坐标，平移的时候，其坐标在两塔之间
              Ycoordinate ：盘子移动时候的，盘子上升时候的纵坐标，
				在上升和下降时，其范围在盘子起始的众坐标，与塔的高度
				加上一个数值之间
			  FlagDishMoveDirect:水平移动或则上升下降移动时的标志，
				下降或上升时为True，水平移动为False
/************************************************************************/
void HanioAutoDisplay::ClearDishShape(int SetDishNumShape,int Xcoordinate,
									  int Ycoordinate,bool FlagDishMoveDirect)
{
	for (  int j = 0; j < 2*(SetDishNumShape); j++)
	{
		if (FlagDishMoveDirect)
		{
			//根据盘子的编号设置盘子的形状
			m_HanioScreen[Ycoordinate][ Xcoordinate ] = '|'; 
		
		} 
		else
		{
			m_HanioScreen[Ycoordinate][ Xcoordinate] = FlagDisplaySpace;	
		}
		
		m_HanioScreen[Ycoordinate][ Xcoordinate+j]
			= FlagDisplaySpace;
		m_HanioScreen[Ycoordinate][ Xcoordinate-j]
			= FlagDisplaySpace;
		
		}
}
/************************************************************************/
/*  函数名称：MoveTowerNumber( ) :盘子在塔之间移动的一个动态过程的控制
         StartXcoordinate,StartYcoordinate：存放的是盘子的移动的起始位置坐标
         EndXcoordinate,EndYcoordinate：存放的是盘子的移动的终点位置坐标信息
         DishHeight[0] 为要移动的盘子在DishFromTheTower塔中移出的高度信息，
		 也就是目前塔中有几个盘子信息
         DishHeight[1] 为要移动的盘子在DishToTheTower塔中存放的高度信息，
		 也就是目前塔中有几个盘子信息 
/************************************************************************/
void HanioAutoDisplay::MoveTowerNumber( )
{
	int StartXcoordinate,StartYcoordinate;
	int EndXcoordinate,EndYcoordinate;

	//从起始塔中移出的横坐标信息
	StartXcoordinate = m_TowerNumberLoctaion[DishFromTheTower-1];

	//从起始塔中移出的高度信息
	StartYcoordinate = m_ScreenHeight-1-DishHeight[0]; 
	
	//放入目标塔中的横坐标信息
	EndXcoordinate = m_TowerNumberLoctaion[DishToTheTower-1];

	//放入目标塔中的高度信息
	EndYcoordinate = m_ScreenHeight-2-DishHeight[1]; 
	
	//控制上升移动过程的屏幕更新
	MoveUpStep( StartXcoordinate ,StartYcoordinate,m_dishSize);

	//控制水平移动过程的屏幕更新
	MoveLevelStep(StartXcoordinate,EndXcoordinate,m_dishSize);

	//控制下降移动过程的屏幕更新
	MoveDownStep(EndXcoordinate,EndYcoordinate,m_dishSize);   

}

/************************************************************************/
/* 函数名称：MoveUpStep：控制盘子从起始塔开始向上移动的一个动态显示过程，包括先
		删除盘子的起始位置， 再更新下一步移动时，盘子在屏幕上的一个位置和形状信息    
   输入参数：盘子在起始塔中的横坐标和纵坐标信息，和盘子的编号信息
         StartXcoordinate， StartYcoordinate控制盘子的起始位置；
         dishSize 控制在整个上升移动过程中，在屏幕更新时盘子的形状信息
        盘子的终点坐标为：StartXcoordinate和m_FinalHeight+m_VerticalStep
/************************************************************************/
void HanioAutoDisplay::MoveUpStep( int StartXcoordinate ,
								  int StartYcoordinate,int dishSize)
{
	bool Flag =true; 
	/***********************************************************
	 Flag 控制是否在塔上移动的标志，若Flag =true，则在塔上移动，
	     清屏时，应保持塔的形状不变，应输出‘|’
	 Flag =false，则表示盘子已移出塔的高度，则不用输出‘|’
	***********************************************************/
                   
	for ( int i =StartYcoordinate; i > m_FinalHeight; )
	{
		if ( i >  m_TowerHeightYcoodinate )
		{
			//dishSize的范围为1,2,3，4，...，m

			 //  清除掉在移动过程中盘子在塔中的信息，
			ClearDishShape( dishSize, StartXcoordinate,i ,Flag); 
		} 
		else
		{
			Flag=false;

			//  清除掉在移动过程中盘子在塔中的信息，
			ClearDishShape( dishSize, StartXcoordinate,i ,Flag); 
			
		}
		i -= m_VerticalStep;
		if ( i <= m_FinalHeight )
		{
			i = m_FinalHeight;
			SetDishShape( dishSize,StartXcoordinate,i);
		} 
		else
		{
			SetDishShape( dishSize,StartXcoordinate,i);
		}
		
		system("cls");
		DisplayScreen();
		Sleep(DelayTime);
	}

}
//====================================================================
void HanioAutoDisplay::MoveLevelStep( int StartXcoordinate,
									 int EndXcoordinate,int dishSize )
{
	bool Flag =false;
	int j;
	if (StartXcoordinate < EndXcoordinate )
	{
		for (j=StartXcoordinate; j < EndXcoordinate ;  )
		{
			ClearDishShape( dishSize, j,m_FinalHeight ,Flag);
			j+= m_Levelstep;
			if (j >= EndXcoordinate )
			{
				j = EndXcoordinate ;
				SetDishShape( dishSize,j,m_FinalHeight);
			} 
			else
			{
				SetDishShape( dishSize,j,m_FinalHeight);
			}
			
			system("cls");
			DisplayScreen();
			Sleep(DelayTime);
		}
	} 
	else
	{
		for (j=StartXcoordinate; j > EndXcoordinate ;  )
		{
			ClearDishShape( dishSize, j,m_FinalHeight ,Flag);
			j -= m_Levelstep;
			if (j <= EndXcoordinate )
			{
				j =EndXcoordinate ;
				SetDishShape( dishSize,j,m_FinalHeight);
			} 
			else
			{
				SetDishShape( dishSize,j,m_FinalHeight);
			}
			SetDishShape( dishSize,j,m_FinalHeight);
			system("cls");
			DisplayScreen();
			Sleep(DelayTime);
		}
	}
	
	
}

//====================================================================
void HanioAutoDisplay::MoveDownStep( int EndXcoordinate,
									int EndYcoordinate,int dishSize )
{
	bool Flag =true;
	int i;
	for ( i = m_FinalHeight ;i < EndYcoordinate; )
	{
		if ( i <= m_TowerHeightYcoodinate )
		{
			Flag=false;
			ClearDishShape( dishSize, EndXcoordinate,i ,Flag);
		} 
		else
		{
			Flag=true;
			ClearDishShape( dishSize, EndXcoordinate,i ,Flag);
			
		}
		//ClearDishShape( dishSize, EndXcoordinate,i ,Flag);
		i += m_VerticalStep;
		if (i >= EndYcoordinate)
		{
			i = EndYcoordinate;
			SetDishShape( dishSize,EndXcoordinate,i);

		} 
		else
		{
			SetDishShape( dishSize,EndXcoordinate,i);
		}
	

		system("cls");//刷屏
		DisplayScreen();
		Sleep(DelayTime);
	}

	//Flag = false;
}


#endif