/*************************************************************************
	> File Name: Algraph.h
	> Author: bw98
	> Mail:
	> Created Time: 2018��01��02�� ���ڶ� 08ʱ32��45��
 ************************************************************************/

#ifndef _ALGRAPH_H
#define _ALGRAPH_H
#include<iostream>
#include<string>
#include<assert.h>
#define MAXFILESIZE 50
typedef struct Time {
	//ʱ���ʽ: hour:minute,day
	int day;
	int hour;
	int minute;
	//�����'-'���أ�ʵ����Time����������
	friend Time operator - (Time& et, Time& st) {
		Time t = { 0, 0, 0 };
		t.minute = et.minute - st.minute;
		if (t.minute < 0) {
			t.minute += 60;
			t.hour--;
		}

		t.hour = t.hour + et.hour - st.hour;
		if (t.hour < 0) {
			t.hour += 24;
			t.day--;
		}
		if (et.day < st.day) {
			t.day++;
		}
		t.day = t.day + et.day - st.day;
		return t;
	}
}Time;

typedef struct InfoType {//������Ϣ
	std::string LineName; //����򳵴κţ����8�ַ�(��\0) 
	Time StartTime, EndTime; //����ʱ����Ŀ��ʱ��
	Time SpendTime; //��·�ϻ��ѵ�ʱ��
	float SpendMoney; //Ʊ��
}TrafficLine;

typedef struct ArcNode {//���㣬�߱�
	std::string EndName; //�û�ĩβ�ĳ���
	struct ArcNode* NextLine; //�ػ�ָ���̱����ָ��
	TrafficLine* Info; //�û���·����Ϣ 
}LineNode;

typedef struct VNode {//ͷ���,�����
	std::string CityName; //����
	int CityOrd; //���б��,����ƥ��
	LineNode* FirstLine;//�ص�һ����ָ���̱����ָ��
	int Amount;//��ͨ���߰��
}VNode;

//����ڵ㣬����id�͵�Դ����Ĺ������,���ȶ�����Ҫ������
struct Node; //������С����
struct Node1; //������С·��

class ALGraph {
public:
	ALGraph(int size);
	~ALGraph();
	int searchCityNum(const std::string CityName); //��ѯ���б��
	void addCity(const std::string CityName); //�ֶ���ӳ���
	void addCityFromFile(const char FileName[MAXFILESIZE]); //���ļ���ȡ����ӳ���
	void insert(std::string StartName, LineNode* temp, std::string EndName); //������·
	void addLine(); //�����·
	void addLineFromFile(const char FileName[MAXFILESIZE]); //���ļ���ȡ�������·
	void reSize(int size); //���·�������б������ǳ����������
	void delCityLine(int i); //ɾ���Գ���Ϊ������·
	void delLine(); //ɾ����·
	void delCity(std::string CityName); //ɾ�����У���ɾ���Ըó���Ϊ���ĺ�����г�
	void updateFile(const char FileName[MAXFILESIZE], const std::string type); //�޸ĺ�����ļ�

	void showCity(); //������г���
	void showLine(); //���������·
	void dijkstra_Money(int v0, int* parent, Node* dis); //��С����·��
	int timeTransWeight(const Time& t); //ʱ��ת��ΪȨֵ,��ת��Ϊ����
	void dijkstra_Time(int v0, int* parent, Node1* dis); //��Сʱ��·��
	//void bfs_Transit (); //��С��ת�����Լ�·��
	void showShortestPath(const std::string type); //���ò���ӡ���·��

private:
	VNode* CityList; //�����������ʾ�����б�
	int CityNum; //���и�������������
	int MaxCityNum; //��ǰ�����и���
	int ArcNum; // ������
};
#endif
