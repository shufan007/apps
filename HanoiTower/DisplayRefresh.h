#ifndef DISPLAYREFRESH_H
#define DISPLAYREFRESH_H
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

//===========================================================
typedef vector< int > VROW;
typedef vector< VROW > VVROW;
//-----------------------------------------------------------

class HanioAutoDisplay
{
public:
	HanioAutoDisplay();
	HanioAutoDisplay( int DishNumber );
	~HanioAutoDisplay(){};

	/***********************************************************************
	MoveTowerNumber(); 将最上方的盘子移动到指定位置
	MoveUpStep, MoveLevelStep, MoveDownStep 分别负责控制盘子的上移、
		水平移动和下移，输入参数分别为盘子起始和终止坐标信息和盘子号
	DisplayScreen()：在屏幕上显示当前状态，即显示当前的整个界面
	GetTheTowerTopDish(int MoveTimes); 获取当前最上方盘子的信息。
		参数 MoveTimes 是当前移动的步数
	SetDishShape: 根据的号码和坐标设置当前盘子的形状
	ClearDishShape：当盘子移动到下一位置，清除原位置的盘子信息，使其恢复原状
		SetDishShape 和 ClearDishShape 都只负责单步移动，
		若要实现整个移动过程，必须多次调用
	***********************************************************************/
	void MoveTowerNumber();
	void MoveUpStep( int StartXcoordinate ,int StartYcoordinate,int dishSize );
	void MoveLevelStep( int StartXcoordinate,int EndXcoordinate ,int dishSize);
	void MoveDownStep( int EndXcoordinate,int EndYcoordinate ,int dishSize);

	void DisplayScreen();
	void GetTheTowerTopDish(int MoveTimes);

	void SetDishShape(int SetDishNumShape,int Xcoordinate,int Ycoordinate);
	void ClearDishShape(int SetDishNumShape,int Xcoordinate,
		int Ycoordinate,bool FlagDishMoveDirect);
	//------------------------------------------------------------------------

private:
	vector<string > m_HanioScreen;  // 字符串界面
	vector <string > m_StrDish;		// 盘子字符串

	int m_Levelstep;	//水平移动步长控制参数
	int m_VerticalStep; //竖直移动步长控制参数
	int m_ScreenHeight;	//界面的高度
	int m_ScreenWidth;	//界面的宽度
	int m_TowerNumberLoctaion[3];//存放三个塔的位置
	int m_TowerHeight;	//塔的高度
	int m_TowerHeightYcoodinate ; //塔的纵坐标
	int m_DishNumber;	//盘子数

	VVROW m_DishLocationInformation;
	//定义了一个n×3维矩阵，存放盘子的位置信息，
	//和每个塔上的盘子信息

	int DishHeight[2];
	//DishHeight[0]存放startTower的深度信息,
	//DishHeight[1]存放endTower的深度信息

	int DishFromTheTower; //盘子移动的起始塔号
	int DishToTheTower;	//盘子移动的目标塔号
	int m_dishSize;		//获取盘子的大小信息
	int m_FinalHeight;	//盘子最终的高度
	int DelayTime;		//每一步移动的延迟时间

};


#endif
