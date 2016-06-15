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
// MoveInformationStart ��������ƶ�����ʼλ��
// MoveInformationEnd ��������ƶ���Ŀ��λ��
 vector <int > MoveInformationStart;
 vector <int> MoveInformationEnd;
//---------------------------------------
// �ú���������õ��������ƶ�����ֹλ�÷ֱ����
//	��Ӧ��������

void move(int x,int y)
{
    MoveInformationStart.push_back(x);
	MoveInformationEnd.push_back(y);
}
//--------------------------------------------------------
//��ŵ������ĵݹ麯��
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
//	������

int main()
{
	int m ,i;
	
	cout<<"\n��ʾ��Ϊʹ��ʾ��ʱ��϶̣��������ӵĸ��� ";
	cout<<"n ��ò�Ҫ̫��(����n<=5),\n\n";

	// �������ӵĸ��� 
	cout << " input the number of disks: " ;
	cin >> m  ; 

	//�����������ӵĸ��������Ӹ�������̫��
	while (m>10)  
	{
		cout<<"you should better input a smaller m,for m<=5\n"
		<<"input a new m:  \n";

		cin>>m;
	}
	
	cout  << " the step to moving " << m << endl ;
	
	//��������
	HanioAutoDisplay ThanioAutoDisplay(m);

	//���ú�ŵ������ļ��㺯�����õ��ƶ����ӵ���ֹλ������
	//����ȡ MoveInformationStart �� MoveInformationEnd ��ֵ

	hanoi( m,1,2,3);

	int ntimes = (2<<(m-1))-1;	//�����ƶ����ӵ��ܴ���

	//��˳���ƶ�����
	for ( i=1; i<=ntimes ; i++ )
	{
		ThanioAutoDisplay.GetTheTowerTopDish( i);
		ThanioAutoDisplay.MoveTowerNumber();
	}
	return 0;

}

#endif