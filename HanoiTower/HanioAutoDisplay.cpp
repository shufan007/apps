
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

const char FlagDisplay = '*'; //��ʾ���ӵ��ַ�
const char FlagDisplaySpace = ' ';
//========================================================

HanioAutoDisplay::HanioAutoDisplay( int DishNumber )
{	
	int i=0,j=0;

	//��������ʼ��
	m_DishNumber =  DishNumber;

	//---------------------------------------------------
	//��ÿ�����ϵ�������Ϣ���г�ʼ�� 
	
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
	//���ñ�ʾ���ӵ��ַ���
	string s( ((2<<(m_DishNumber-1))+1),' ');
	m_StrDish.push_back(s) ;	//���Ҫ�ƶ����ӵ��ַ���

	for ( i=1 ; i<= DishNumber ; i++ )
	{
		string s1((2<<(i-1))+1,'*' ); //���ӵ�*��ʾ��������״
		
		m_StrDish.push_back( s1 ); //m_StrDish��ŵ������ӵ���״

	}
	//---------------------------------------------------
	//	����Ļ��ʼ��Ϊ����

	for ( i=0; i<m_ScreenHeight; i++ )
	{
		string s2(m_ScreenWidth, ' ');
		m_HanioScreen.push_back( s2 );
	}
	//---------------------------------------------------
	//	����Ļ�������һ���á�=����ʾ
	for ( j=0;j<m_ScreenWidth; j++)
	{
		m_HanioScreen[m_ScreenHeight-1][j] = '=';
	}
		
	//---------------------------------------------------
	//������������Ļ����ʾ

	for ( i=0; i<3; i++)
	{
		for (  j=m_ScreenHeight-m_TowerHeight; j< m_ScreenHeight-1 ; j++)
		{
			m_HanioScreen[j][m_TowerNumberLoctaion[i]] = '|' ;
		
		}
		
	}

	//---------------------------------------------------
	//��������Ϣ�ڵ�һ��������ʾ����������ھ�����

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
/* ��ʾ��������ʾ�����ݾ��� m_HanioScreen��ÿ�����ӵ��ƶ�ͨ������
	m_HanioScreen ��������ʵ��һ����̬��ʾ�Ĺ���      */
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
/* �������ƣ� GetTheTowerTopDish( int MoveTimes)   
   �������ܣ� ��ȡ�������㷨�и������ӵ�ÿ���ƶ���Ϣ�����ĸ������ĸ����������ӵĸ߶�  
   ���������MoveTimes������MoveTimes��ֵ����ȡÿ�����ӵ��ƶ���Ϣ���磺������3������ʱ��
        �����ڵ�һ���ƶ�ʱ����1�Ƶ���3.   
   ��������Ϣ��MoveInformationStart ��MoveInformationEnd�д�� 
   ���������DishFromTheTower���ӴӸ����Ƴ�
             DishToTheTower  �����ƶ������� 
             DishHeight[0] ΪҪ�ƶ���������DishFromTheTower���е�λ��
             DishHeight[1] ΪҪ�ƶ���������DishToTheTower���д�ŵ�λ����Ϣ
             m_dishSize ��ȡ����Ҫ�ƶ������ӵı�ţ���1��ʼ     */
/************************************************************************/

void HanioAutoDisplay::GetTheTowerTopDish( int MoveTimes)
{
	//DishFromTheTower��DishToTheTowerΪ���ţ��䷶Χ��1��2��3�е�һ������
	DishFromTheTower = MoveInformationStart[MoveTimes-1];
	DishToTheTower = MoveInformationEnd[MoveTimes-1];

//	DishHeight[0]�䷶Χ�Ǵ�1��ʼ�������Ǵ�0��ʼ
	//m_DishLocationInformation���飬���±��0��ʼ
	DishHeight[0] = m_DishLocationInformation[DishFromTheTower-1].size();
	DishHeight[1] = m_DishLocationInformation[DishToTheTower-1].size();
	m_dishSize = m_DishLocationInformation[DishFromTheTower-1][DishHeight[0]-1];

	//��Ҫ�Ƴ������ӣ�������ɾ����
	m_DishLocationInformation[DishFromTheTower-1].pop_back();
	m_DishLocationInformation[DishToTheTower-1].push_back(m_dishSize) ;
	//Ŀ�ľ��ǽ���һ��������ÿ�������ϵ�״̬��Ϣ
}
/************************************************************************/
/* ��������SetDishShape�������ƶ������ھ����е���״��
   ��������� SetDishNumShape�����ӵı����Ϣ
              Xcoordinate �������ƶ�ʱ��ĺ����꣬���������½�ʱ��
			     �������ĺ����꣬ƽ�Ƶ�ʱ��������������֮��
              Ycoordinate �������ƶ�ʱ��ģ���������ʱ��������꣬���������½�ʱ��
			      �䷶Χ��������ʼ�������꣬�����ĸ߶ȼ���һ����ֵ֮��*/           
/************************************************************************/
void HanioAutoDisplay::SetDishShape(int SetDishNumShape,
									int Xcoordinate,int Ycoordinate)
{
	//�������ӵı���������ӵ���״

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
/* ��������ClearDishShape������ƶ������ھ����е���״��
   ��������� ClearDishNumShape�����ӵı����Ϣ
              Xcoordinate �������ƶ�ʱ��ĺ����꣬���������½�ʱ��
				�������ĺ����꣬ƽ�Ƶ�ʱ��������������֮��
              Ycoordinate �������ƶ�ʱ��ģ���������ʱ��������꣬
				���������½�ʱ���䷶Χ��������ʼ�������꣬�����ĸ߶�
				����һ����ֵ֮��
			  FlagDishMoveDirect:ˮƽ�ƶ����������½��ƶ�ʱ�ı�־��
				�½�������ʱΪTrue��ˮƽ�ƶ�ΪFalse
/************************************************************************/
void HanioAutoDisplay::ClearDishShape(int SetDishNumShape,int Xcoordinate,
									  int Ycoordinate,bool FlagDishMoveDirect)
{
	for (  int j = 0; j < 2*(SetDishNumShape); j++)
	{
		if (FlagDishMoveDirect)
		{
			//�������ӵı���������ӵ���״
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
/*  �������ƣ�MoveTowerNumber( ) :��������֮���ƶ���һ����̬���̵Ŀ���
         StartXcoordinate,StartYcoordinate����ŵ������ӵ��ƶ�����ʼλ������
         EndXcoordinate,EndYcoordinate����ŵ������ӵ��ƶ����յ�λ��������Ϣ
         DishHeight[0] ΪҪ�ƶ���������DishFromTheTower�����Ƴ��ĸ߶���Ϣ��
		 Ҳ����Ŀǰ�����м���������Ϣ
         DishHeight[1] ΪҪ�ƶ���������DishToTheTower���д�ŵĸ߶���Ϣ��
		 Ҳ����Ŀǰ�����м���������Ϣ 
/************************************************************************/
void HanioAutoDisplay::MoveTowerNumber( )
{
	int StartXcoordinate,StartYcoordinate;
	int EndXcoordinate,EndYcoordinate;

	//����ʼ�����Ƴ��ĺ�������Ϣ
	StartXcoordinate = m_TowerNumberLoctaion[DishFromTheTower-1];

	//����ʼ�����Ƴ��ĸ߶���Ϣ
	StartYcoordinate = m_ScreenHeight-1-DishHeight[0]; 
	
	//����Ŀ�����еĺ�������Ϣ
	EndXcoordinate = m_TowerNumberLoctaion[DishToTheTower-1];

	//����Ŀ�����еĸ߶���Ϣ
	EndYcoordinate = m_ScreenHeight-2-DishHeight[1]; 
	
	//���������ƶ����̵���Ļ����
	MoveUpStep( StartXcoordinate ,StartYcoordinate,m_dishSize);

	//����ˮƽ�ƶ����̵���Ļ����
	MoveLevelStep(StartXcoordinate,EndXcoordinate,m_dishSize);

	//�����½��ƶ����̵���Ļ����
	MoveDownStep(EndXcoordinate,EndYcoordinate,m_dishSize);   

}

/************************************************************************/
/* �������ƣ�MoveUpStep���������Ӵ���ʼ����ʼ�����ƶ���һ����̬��ʾ���̣�������
		ɾ�����ӵ���ʼλ�ã� �ٸ�����һ���ƶ�ʱ����������Ļ�ϵ�һ��λ�ú���״��Ϣ    
   �����������������ʼ���еĺ��������������Ϣ�������ӵı����Ϣ
         StartXcoordinate�� StartYcoordinate�������ӵ���ʼλ�ã�
         dishSize ���������������ƶ������У�����Ļ����ʱ���ӵ���״��Ϣ
        ���ӵ��յ�����Ϊ��StartXcoordinate��m_FinalHeight+m_VerticalStep
/************************************************************************/
void HanioAutoDisplay::MoveUpStep( int StartXcoordinate ,
								  int StartYcoordinate,int dishSize)
{
	bool Flag =true; 
	/***********************************************************
	 Flag �����Ƿ��������ƶ��ı�־����Flag =true�����������ƶ���
	     ����ʱ��Ӧ����������״���䣬Ӧ�����|��
	 Flag =false�����ʾ�������Ƴ����ĸ߶ȣ����������|��
	***********************************************************/
                   
	for ( int i =StartYcoordinate; i > m_FinalHeight; )
	{
		if ( i >  m_TowerHeightYcoodinate )
		{
			//dishSize�ķ�ΧΪ1,2,3��4��...��m

			 //  ��������ƶ����������������е���Ϣ��
			ClearDishShape( dishSize, StartXcoordinate,i ,Flag); 
		} 
		else
		{
			Flag=false;

			//  ��������ƶ����������������е���Ϣ��
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
	

		system("cls");//ˢ��
		DisplayScreen();
		Sleep(DelayTime);
	}

	//Flag = false;
}


#endif