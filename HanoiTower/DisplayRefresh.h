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
	MoveTowerNumber(); �����Ϸ��������ƶ���ָ��λ��
	MoveUpStep, MoveLevelStep, MoveDownStep �ֱ���������ӵ����ơ�
		ˮƽ�ƶ������ƣ���������ֱ�Ϊ������ʼ����ֹ������Ϣ�����Ӻ�
	DisplayScreen()������Ļ����ʾ��ǰ״̬������ʾ��ǰ����������
	GetTheTowerTopDish(int MoveTimes); ��ȡ��ǰ���Ϸ����ӵ���Ϣ��
		���� MoveTimes �ǵ�ǰ�ƶ��Ĳ���
	SetDishShape: ���ݵĺ�����������õ�ǰ���ӵ���״
	ClearDishShape���������ƶ�����һλ�ã����ԭλ�õ�������Ϣ��ʹ��ָ�ԭ״
		SetDishShape �� ClearDishShape ��ֻ���𵥲��ƶ���
		��Ҫʵ�������ƶ����̣������ε���
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
	vector<string > m_HanioScreen;  // �ַ�������
	vector <string > m_StrDish;		// �����ַ���

	int m_Levelstep;	//ˮƽ�ƶ��������Ʋ���
	int m_VerticalStep; //��ֱ�ƶ��������Ʋ���
	int m_ScreenHeight;	//����ĸ߶�
	int m_ScreenWidth;	//����Ŀ��
	int m_TowerNumberLoctaion[3];//�����������λ��
	int m_TowerHeight;	//���ĸ߶�
	int m_TowerHeightYcoodinate ; //����������
	int m_DishNumber;	//������

	VVROW m_DishLocationInformation;
	//������һ��n��3ά���󣬴�����ӵ�λ����Ϣ��
	//��ÿ�����ϵ�������Ϣ

	int DishHeight[2];
	//DishHeight[0]���startTower�������Ϣ,
	//DishHeight[1]���endTower�������Ϣ

	int DishFromTheTower; //�����ƶ�����ʼ����
	int DishToTheTower;	//�����ƶ���Ŀ������
	int m_dishSize;		//��ȡ���ӵĴ�С��Ϣ
	int m_FinalHeight;	//�������յĸ߶�
	int DelayTime;		//ÿһ���ƶ����ӳ�ʱ��

};


#endif
