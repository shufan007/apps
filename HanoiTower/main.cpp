#ifdef WIN32
//#pragma warning (disable: 4514 4786)

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>

#include "DisplayRefresh.h"

using namespace std;
//===============================================
// MoveInformationStart 存放盘子移动的起始位置
// MoveInformationEnd 存放盘子移动的目标位置
 vector <int > MoveInformationStart;
 vector <int> MoveInformationEnd;
//---------------------------------------
// 该函数将计算得到的盘子移动的起止位置分别放入
//	相应的向量中

void move(int x,int y)
{
    MoveInformationStart.push_back(x);
	MoveInformationEnd.push_back(y);
}
//--------------------------------------------------------
//汉诺塔问题的递归函数
void hanoi( int n,int TowerOne, int TowerTwo, int TowerThree)
{
	if ( n == 1)
	{
		move( TowerOne, TowerThree );

	}
	else{
		hanoi( n-1, TowerOne, TowerThree, TowerTwo );
		move( TowerOne, TowerThree );
		hanoi( n-1, TowerTwo, TowerOne, TowerThree);
	}
}

//===========================================================
//	主函数

int main()
{
	int m ,i;
	
	cout<<"\n提示：为使演示的时间较短，输入盘子的个数 ";
	cout<<"n 最好不要太大(建议n<=5),\n\n";

	// 输入盘子的个数 
	cout << " input the number of disks: " ;
	cin >> m  ; 

	//控制输入盘子的个数，盘子个数不易太多
	while (m>10)  
	{
		cout<<"you should better input a smaller m,for m<=5\n"
		<<"input a new m:  \n";

		cin>>m;
	}
	
	cout  << " the step to moving " << m << endl ;
	
	//创建对象
	HanioAutoDisplay ThanioAutoDisplay(m);

	//调用汉诺塔问题的计算函数，得到移动盘子的起止位置序列
	//即获取 MoveInformationStart 和 MoveInformationEnd 的值

	hanoi( m,1,2,3);

	int ntimes = (2<<(m-1))-1;	//计算移动盘子的总次数

	//按顺序移动盘子
	for ( i=1; i<=ntimes ; i++ )
	{
		ThanioAutoDisplay.GetTheTowerTopDish( i);
		ThanioAutoDisplay.MoveTowerNumber();
	}
	return 0;

}

#endif