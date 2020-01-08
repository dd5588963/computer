/*************************************************************************
	> File Name: ALGraph.cpp
	> Author: bw98
	> Mail:
	> Created Time: 2018��01��02�� ���ڶ� 09ʱ49��43��
 ************************************************************************/
#include"ALGraph.h"
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<iomanip>
#include<queue>
#include<stack>
using namespace std;
#define INF 0xfffff

//'>>' ���أ�ʵ��"hour:minute,+day" �Ĵ洢
istream & operator >> (istream & in, Time & time) {
	char c1, c2, c3;
	int hour, minute, day;
	in >> hour >> c1 >> minute >> c2 >> c3 >> day;
	time.day = day;
	time.minute = minute;
	time.hour = hour;
	return in;
}

//����Ԫ'<<'���������� Time ���������
ostream& operator << (ostream& out, Time& time) {
	//����֮��հ���'0'���
	cout.fill('0');
	//���ݵ��Ҷ�����������'+'��
	out << setw(2) << setiosflags(ios_base::right) << time.hour << ":" << setw(2) << time.minute << resetiosflags(ios_base::right) << "," << setiosflags(ios_base::showpos) << time.day << resetiosflags(ios_base::showpos); //setiosflags(ios_base::showpos) ��ʾ "+" ����
	cout.fill(' '); //fill setiosflags ���ֶ��ر�
	return out;
}

ALGraph::ALGraph(int size) {
	CityList = new VNode[size];
	int i;
	for (i = 0; i < size; ++i) {
		CityList[i].CityName = "NULL";
		CityList[i].CityOrd = i;
		CityList[i].FirstLine = NULL;
		CityList[i].Amount = 0;
	}
	CityNum = 0;
	MaxCityNum = size;
	ArcNum = 0;
}//ALGraph::ALGraph (int size)  

//��������
ALGraph::~ALGraph() {
	delete[]CityList;
}

//��ѯ���б�ţ��ҵ�������ţ��Ҳ�������-1
int ALGraph::searchCityNum(const string CityName) {
	int i;
	for (i = 0; i < CityNum; ++i) {
		if (CityList[i].CityName == CityName) {
			return i;
		}
	}
	return -1;
}//searchCityNum 

//����ͼ�洢�Ĵ�С��ֻ�����˳����������
void ALGraph::reSize(int size) {
	if (size <= CityNum) {
		return;
	}
	VNode* NewList = new VNode[size];
	int i;
	for (i = 0; i < CityNum; ++i) {
		NewList[i] = CityList[i]; //C++����ֱ�Ӹ�ֵ
	}
	for (i = CityNum; i < size; ++i) {
		CityList[i].CityName = "NULL";
		CityList[i].CityOrd = i;
		CityList[i].FirstLine = NULL;
		CityList[i].Amount = 0;
	}
	delete[]CityList;
	CityList = NewList;
	MaxCityNum = size;
}//reSize 

//�ֶ���ӳ���
void ALGraph::addCity(const string CityName) {
	//�Ѵ��ڳ���
	if (searchCityNum(CityName) != -1) {
		cout << "The city has in the list!!!" << endl;
		return;
	}
	//�ռ�����
	if (CityNum >= MaxCityNum) {
		++MaxCityNum;
		reSize(MaxCityNum);
	}
	CityList[CityNum].CityName = CityName;
	CityList[CityNum].CityOrd = CityNum;
	CityList[CityNum].FirstLine = NULL;
	CityList[CityNum].Amount = 0;
	++CityNum;
}//addCity

//���ļ��ж�ȡ����ӳ���
void ALGraph::addCityFromFile(const char FileName[MAXFILESIZE]) {
	cout << "��ʼ�� " << FileName << "�ж��벢��ӳ��У�" << endl;
	ifstream file(FileName); //��ʧ�ܷ���NULL
	if (!file) {
		cout << "��ʧ��!" << endl;
		return;
	}

	string line; //��ȡ��һ�����ݣ���һ���ǳ����ܸ���
	getline(file, line);
	istringstream istr(line); //����stringstream�� string �� int ������ת��
	int count;
	istr >> count;

	//�ռ�����
	if (CityNum + count >= MaxCityNum) {
		MaxCityNum = CityNum + count;
		reSize(MaxCityNum);
	}
	//�ӵڶ��п�ʼ��ȡ��������
	while (getline(file, line)) {
		istringstream istr2(line); //����istringstream��ֱ�ӽ�string���뵽name
		string name;
		istr2 >> name;
		addCity(name);
	}
	cout << "���е�����ϣ�" << endl;
	file.close(); //��ȡ��ر��ļ�
}//addCityFromFile

//�����·
void ALGraph::addLine() {
	string StartName, EndName;
	//��Ϣ����
	cout << "�����������У�" << endl;
	cin >> StartName;
	cout << "�������յ���У�" << endl;
	cin >> EndName;
	LineNode* temp = new LineNode;
	TrafficLine* info = new TrafficLine;
	temp->EndName = EndName;
	cout << "������������";
	cin >> info->LineName;
	cout << "���������ʱ��:(hh:mm,+d)";
	cin >> info->StartTime;
	cout << "�����뵽��ʱ�䣺(hh:mm,+d)";
	cin >> info->EndTime;
	info->SpendTime = info->EndTime - info->StartTime;
	cout << "������Ʊ�ۣ�";
	cin >> info->SpendMoney;
	temp->Info = info;
	insert(StartName, temp, EndName);
}//addLine 

//������·
void ALGraph::insert(string StartName, LineNode* temp, string EndName) {
	int StartNum = searchCityNum(StartName);
	if (StartNum == -1) {
		addCity(StartName); //���޳��У����½�
		StartNum = searchCityNum(StartName);
	}
	if (searchCityNum(EndName) == -1) {
		addCity(EndName); //���޳��У����½�
	}
	LineNode* p, * q; //q��p�ĺ��
	p = CityList[StartNum].FirstLine;

	//ԭ��û����·�����
	if (p == NULL) {
		CityList[StartNum].FirstLine = temp;
		temp->NextLine = NULL;
	}
	//ԭ������·�����
	else {
		q = p->NextLine;
		while (q != NULL && EndName != q->EndName) {
			p = q;
			q = q->NextLine;
		}
		p->NextLine = temp;
		temp->NextLine = q;
	}
	CityList[StartNum].Amount++;
	ArcNum++;
}//insert

//���ļ��ж�ȡ�������·
void ALGraph::addLineFromFile(const char FileName[MAXFILESIZE]) {
	cout << "��" << FileName << "�ж�ȡ��������·��" << endl;
	ifstream file(FileName);
	if (!file) {
		cout << "���ܴ��ļ���" << endl;
		return;
	}
	string catalogue; //�洢��һ�е�Ŀ¼
	getline(file, catalogue);

	//�ӵڶ��п�ʼ��ȡ������Ҫ�洢����Ϣ
	string line;
	while (getline(file, line)) {
		istringstream istr(line);
		LineNode* temp = new LineNode;
		TrafficLine* info = new TrafficLine;
		temp->Info = info;
		string StartName, EndName;
		istr >> StartName >> EndName >> info->LineName >> info->StartTime >> info->EndTime >> info->SpendTime >> info->SpendMoney;
		info->SpendTime = info->EndTime - info->StartTime;
		temp->EndName = EndName;
		insert(StartName, temp, EndName);
	}//while (getline(file,line))

	file.close(); //�򿪴洢��·���ļ���ϣ��ر�file
	cout << "��·������ϣ�" << endl;
}//addLineFromFile

//ɾ�����i����Ϊ��������·��
void ALGraph::delCityLine(int i) {
	LineNode* p, * q;
	p = CityList[i].FirstLine;
	if (p == NULL) { //�ձ�
		return;
	}
	//�ӵڶ������㿪ʼɾ
	while (p->NextLine) {
		q = p;
		p = q->NextLine;
		delete q->Info;
		delete q;
	}
	//ɾ�����һ�����
	if (p->NextLine == NULL) {
		delete p->Info;
		delete p;
	}
}//delCityLine 

//ɾ�����У���ɾ���Գ���Ϊ���ĺ�����г�
void ALGraph::delCity(string CityName) {
	int i, j;
	i = searchCityNum(CityName);
	//δ��ѯ���ó���
	if (i == -1) {
		cout << "û���ҵ��ó���!" << endl;
	}
	else {
		delCityLine(i); //ɾ���ó���Ϊ����������·
		for (j = i; j < CityNum - 1; ++j) {
			CityList[j] = CityList[j + 1];
		}
		--CityNum;
		cout << "ɾ�� " << CityName << " �ɹ��� " << endl;
	}
}//delCity

void ALGraph::delLine() {
	string StartName, EndName;
	int StartNum;
	cout << "������������: ";
	cin >> StartName;
	StartNum = searchCityNum(StartName);
	if (StartNum == -1) {
		cout << "û�иó��г�������·!" << endl;
		return;
	}
	cout << "�������յ����: ";
	cin >> EndName;

	//��ʾ�����е�������·�����ж��Ƿ��ܵ�����յ����
	bool flag = false;
	LineNode* p, * q;
	cout << "���������յ���е�·����: " << endl;
	cout << "��������|�������|�����|����ʱ��|����ʱ��||||��ʱ|||Ʊ��" << endl;
	for (p = CityList[StartNum].FirstLine; p != NULL; p = p->NextLine) {
		if (p->EndName == EndName) {
			cout << setw(8) << StartName << " " << setw(8) << EndName << " " << setw(6) << p->Info->LineName << " " << p->Info->StartTime << " " << p->Info->EndTime << " " << p->Info->SpendTime << " " << setiosflags(ios_base::showpoint) << p->Info->SpendMoney << resetiosflags(ios_base::showpoint);
			flag = true;
		}
	}
	cout << endl;
	if (flag == false) {
		cout << "�ó�������û��ͨ��Ŀ�س��еİ�Σ�" << endl;
		return;
	}

	//�ó��������ܵ���Ŀ�س��У���ʼɾ����·
	string LineName;
	cout << "����������Ҫɾ���ĺ���ţ�" << endl;
	cin >> LineName;
	p = CityList[StartNum].FirstLine;
	q = p->NextLine;//q �� p �ĺ��

	//ֻ��һ��·����һ����������
	if (q == NULL) {
		if (p->Info->LineName != LineName) { //Ψһһ��·����ͨ��Ŀ�ĵص����
			cout << "û�иð�Σ�" << endl;
			return;
		}
		else {
			CityList[StartNum].FirstLine = q; //ע��ԭ�ڴ���Ҫ�ֶ��ͷ�
			CityList[StartNum].Amount--;
			ArcNum--;
			//ɾ���ڴ�
			delete p->Info; //��ͨ��ָ��p,���ṹ����Ľṹ��ָ��ָ����ڴ�free��  
			delete p; //ͨ��p�����ṹ����ڴ�free��
			p = NULL; //��ָ��ΪNULL,��ֹҰָ��
			return;
		}
	}//if(q == NULL)

	//�����������
	while (q->Info->LineName != LineName && q->NextLine) {
		p = q;
		q = q->NextLine;
	}

	//����ѭ�������ֿ��� 1.������� 2.��ͷ��
	//1.��ͷ�˵����
	if (q->Info->LineName != LineName) {
		cout << "û�а�Σ�" << endl;
		return;
	}
	//2.������ȵ����
	else {
		p->NextLine = q->NextLine;
		CityList[StartNum].Amount--;
		ArcNum--;
		delete q->Info;
		delete q;
		return;
	}

}//delLine 

//���޸���ϵ����ݸ��µ��ļ���
void ALGraph::updateFile(const char FileName[MAXFILESIZE], const string type) {
	ofstream file(FileName);
	if (!file) {//�ж��ļ��Ƿ���ڲ��ܴ�
		cout << "���ܴ��ļ�" << FileName << " ��ֻ��д���Ѵ��ڵ��ļ�" << FileName << "����ȷ���ļ��Ƿ���ڣ�" << endl;
		return;
	}

	int i;
	LineNode* p = NULL;
	if (type == "City" || type == "����") {
		//��һ�е�����и���
		file << CityNum << endl;
		for (i = 0; i < CityNum; ++i) {
			file << CityList[i].CityName << endl;
		}
		cout << "��������Ϣ���µ� " << FileName << " ���!" << endl;
	}//�����ļ��еĳ���

	else if (type == "Line" || type == "��·") {
		file << "��������|�������|�����|����ʱ��|����ʱ��||||��ʱ|||Ʊ��" << endl;
		for (i = 0; i < CityNum; ++i) {
			for (p = CityList[i].FirstLine; p != NULL; p = p->NextLine) {
				file << setw(8) << CityList[i].CityName << " " << setw(8) << p->EndName << " " << setw(6) << p->Info->LineName << " ";
				cout.fill('0');
				file << p->Info->StartTime << " " << p->Info->EndTime << " " << p->Info->SpendTime << " " << setiosflags(ios_base::showpoint) << p->Info->SpendMoney << resetiosflags(ios_base::showpoint);
				cout.fill(' ');
			}
		}
		cout << "����·��Ϣ���µ� " << FileName << " ���!" << endl;
	}//�����ļ��е���·
	file.close();
}//updateFile 

//������г���
void ALGraph::showCity() {
	int i;
	string a;
	if (CityNum == 0) {
		cout << "ϵͳ��û���κγ��е���Ϣ!" << endl;
		return;
	}
	cout << "ϵͳ���� " << CityNum << " �����е���Ϣ" << endl;
	for (i = 0; i < CityNum; ++i) {
		cout << i << CityList[i].CityName << endl;
	}
}//showCity 

void ALGraph::showLine() {
	if (ArcNum == 0) {
		cout << "ϵͳ��û���κ���·����Ϣ��" << endl;
		return;
	}
	cout << "ϵͳ���� " << ArcNum << " ����·����Ϣ" << endl;
	cout << "��������|�������|�����|����ʱ��|����ʱ��||||��ʱ|||Ʊ��" << endl;
	//������г�����· 
	int i;
	LineNode* p = NULL;
	for (i = 0; i < CityNum; ++i) {
		p = CityList[i].FirstLine;
		while (p) {
			cout << setw(8) << CityList[i].CityName << " " << setw(8) << p->EndName << " " << setw(6) << p->Info->LineName << " ";
			cout.fill('0');
			cout << p->Info->StartTime << " " << p->Info->EndTime << " " << p->Info->SpendTime << " " << setiosflags(ios_base::showpoint) << p->Info->SpendMoney << endl;
			cout.fill(' '); //fill()����setw(),��Ҫ�ֶ�����
			p = p->NextLine;
		}
	}
}//showLine 

//���ٻ���·��
struct Node {
	int id; //Դ����id
	float money; //������루���ã�
	//����stl�����ȶ��еĵ�����������greater,��������Ҫ����С���ѣ���������������� <
	friend bool operator < (struct Node a, struct Node b) {
		return a.money > b.money;
	}
};
void ALGraph::dijkstra_Money(int v0, int* parent, Node* dis) {
	priority_queue<Node> q; //�Ż�����(����)��ȡ����Сֵ�������������д洢��̾����������ı��
	//parent[]��¼ÿ������ĸ��׽��
	//dis[]��¼Դ�㵽ÿ��������룬������ΪԴ�㵽���ж������̾���
	bool *visited=new bool[MaxCityNum]; //�ж��±��Ӧ�Ķ����Ƿ�������·������˵�Ƿ������·������
	//��ʼ��
	int i;
	for (i = 0; i < CityNum; ++i) {
		dis[i].id = i;
		dis[i].money = INF;
		parent[i] = -1; //ÿ�����㶼û�и����
		visited[i] = false; //��δ�ҵ����·
	}
	dis[v0].money = 0; //Դ�㵽Դ�����·ȨֵΪ0
	q.push(dis[v0]); //ѹ�����
	while (!q.empty()) { //���п�˵����������v0���������������·��
		Node cd = q.top(); //ȡ��С������붥��
		q.pop();
		int u = cd.id;

		if (visited[u]) { //������ˣ������������и�����̾���ȵȲ���
			continue;
		}
		visited[u] = true;
		LineNode* p = CityList[u].FirstLine;
		//�ɳڲ���
		while (p) { //�������������ڵĶ��㣬�����ɳڲ��������¹�����룬ѹ�����
			int v = searchCityNum(p->EndName);
			float m = p->Info->SpendMoney;
			if (!visited[v] && dis[v].money > dis[u].money + m) {
				dis[v].money = dis[u].money + m;
				parent[v] = u;
				q.push(dis[v]);
			}
			p = p->NextLine;
		}
	}// while (!q.empty()) 
}//dijkstra_Money

//����ʱ��·��
struct Node1 {
	int id; //Դ����id
	int tt; //�������(ʱ��)
	Time et; //����ʱ��
	friend bool operator < (struct Node1 a, struct Node1 b) {
		return a.tt > b.tt;
	}
};
int ALGraph::timeTransWeight(const Time& t) {
	return (t.day * 24 + t.hour) * 60 + t.minute;
}

void ALGraph::dijkstra_Time(int v0, int* parent, Node1* dis) {
	priority_queue<Node1> q1;
	//parent[]��¼ÿ������ĸ��׽��
	//dis[]��¼Դ�㵽ÿ��������룬������ΪԴ�㵽���ж������̾���
	bool *visited=new bool[MaxCityNum]; //�ж��±��Ӧ�Ķ����Ƿ�������·������˵�Ƿ������·������
	int i;
	for (i = 0; i < CityNum; ++i) {
		dis[i].id = i;
		dis[i].tt = INF;
		dis[i].et = { 0, 0, 0 };
		parent[i] = -1; //��һ�����㶼û�и����
		visited[i] = false; //��δ�ҵ����·��
	}
	dis[v0].tt = 0;
	q1.push(dis[v0]);
	while (!q1.empty()) {
		Node1 cd = q1.top(); //ȡ����̾���ĵ�����
		q1.pop();
		int u = cd.id;

		if (visited[u]) {
			continue;
		}
		visited[u] = 1;

		LineNode* p = CityList[u].FirstLine;
		//�ҳ��������ڵ㣬�����ɳڲ�����������dis
		while (p) {
			int v = searchCityNum(p->EndName);
			int t = timeTransWeight(p->Info->SpendTime);
			Time st = p->Info->StartTime; //������·��ʼʱ��
			//������ת��ʱ�俪��
			if (u != v0) { //ע��Դ�㵽�κε㶼û����תʱ��
				int change = timeTransWeight(st - dis[u].et); //��ǰ·�ߵĿ���ʱ��-ʼ��վ����һ��վʱ�� 
				t += change;
			}
			if (!visited[v] && dis[v].tt > dis[u].tt + t) {
				dis[v].tt = dis[u].tt + t;
				dis[v].et = p->Info->EndTime;
				parent[v] = u;
				q1.push(dis[v]);
			}
			p = p->NextLine;
		}//while (p)
	}//while (!q1.empty()) 
}//dijkstra_Time

//���ò���ӡ���·��
void ALGraph::showShortestPath(const string type) {
	string StartCity, EndCity;
	int StartNum, EndNum;
	//ȷ����������
	showCity();
	cout << "�����б����ϣ�������������У�";
	cin >> StartCity;
	StartNum = searchCityNum(StartCity);
	//������в�����
	while (StartNum == -1) {
		cout << "��ѯ�����ó��У����ٴ�����������У�" << endl;
		cin >> StartCity;
		StartNum = searchCityNum(StartCity);
	}
	//ȷ������Ŀ�ĳ���
	showCity();
	cout << "�����б����ϣ�������Ŀ�ĳ��У�";
	cin >> EndCity;
	EndNum = searchCityNum(EndCity);
	//������в�����
	while (EndNum == -1) {
		cout << "��ѯ�����ó��У����ٴ�����Ŀ�ĳ��У�" << endl;
		cin >> EndCity;
		EndNum = searchCityNum(EndCity);
	}

	int *path=new int[MaxCityNum]; //��¼ÿ������ĸ��׽�㣬�൱����һ��·��
	int time_minute = 0; //��ʼ�ص���Ŀ�ĵص�����ʱ�䣬 ��λ:����
	float money_yuan = 0; //��ʼ�ص���Ŀ�ĵص����ٷ��ã���λ:Ԫ

	if (type == "Money") {
		Node *dis=new Node[MaxCityNum];
		dijkstra_Money(StartNum, path, dis);
		money_yuan = dis[EndNum].money;
	}
	else {
		Node1 *dis=new Node1[MaxCityNum];
		dijkstra_Time(StartNum, path, dis);
		time_minute = dis[EndNum].tt;
	}

	//��v0 �� EndCity ������·����EndCity �����·������û�и����
	if (path[EndNum] == -1) {
		cout << "�Բ��𣬴�" << StartCity << "��" << EndCity << "��ʱû��·�ߵ��" << endl;
		return;
	}
	else {
		stack<int> s;
		int step = EndNum;
		while (step != StartNum) {
			s.push(step);
			step = path[step];
		}
		int father = step;
		int child;

		//�����ʡǮ·��
		if (type == "Money") {
			cout << "��ʡǮ·��" << endl;
			int tt = 0; //total time,��ʱ�俪��
			Time et = { 0, 0, 0 };
			while (!s.empty()) {
				child = s.top();
				s.pop();
				LineNode* p = CityList[father].FirstLine;
				float mm = INF; //min money,��ǰ��¼�������ٽ�Ǯ,�����ܽ�Ǯ
				int i = 0;
				int count; //��¼ָ���ƶ��Ĵ��������㶨λ
				while (p) {
					if (p->EndName == CityList[child].CityName && mm >= p->Info->SpendMoney) {
						mm = p->Info->SpendMoney;
						count = i;
					}
					p = p->NextLine;
					++i;
				}
				p = CityList[father].FirstLine;
				i = 0;
				while (i != count) {
					p = p->NextLine;
					++i;
				}
				tt += timeTransWeight(p->Info->SpendTime);
				if (father != StartNum) {
					tt = tt + timeTransWeight(p->Info->StartTime - et);
					cout << "��Ҫ��ת�ȴ� " << timeTransWeight(p->Info->StartTime - et) << "���ӣ�" << endl;
				}
				cout << setw(8) << CityList[father].CityName << " " << setw(8) << p->EndName << " " << setw(6) << p->Info->LineName << " ";
				cout.fill('0');
				cout << p->Info->StartTime << " " << p->Info->EndTime << " " << p->Info->SpendTime << " " << setiosflags(ios_base::showpoint) << p->Info->SpendMoney << endl;
				cout.fill(' ');

				et = p->Info->EndTime;
				father = child;
			}//while(!s.empty())
			cout << "һ������" << money_yuan << "Ԫ��" << tt << "���ӣ�" << endl;
		}//�����ʡǮ·��

	//�����ʡʱ���·��
		else {
			cout << "��ʡʱ��·��: " << endl;
			float mm = 0; //�ܹ���Ҫ�Ľ�Ǯ
			Time et = { 0, 0, 0 };
			while (!s.empty()) {
				child = s.top();
				s.pop();
				LineNode* p = CityList[father].FirstLine;
				int tt = INF; //��ǰ��¼������Сʱ��,������ʱ��
				int ot = 0; //ot �����ϻ��˵���ʱ��
				int i = 0, count; //count��¼ָ���ƶ��Ĵ��������㶨λ
				while (p) {
					if (p->EndName == CityList[child].CityName) {
						if (!s.empty() && child != EndNum) {
							ot = timeTransWeight(p->Info->SpendTime) + timeTransWeight(p->Info->StartTime - et);
						}
						else {
							ot = timeTransWeight(p->Info->SpendTime);
						}
						if (tt >= ot) {
							tt = ot;
							count = i;
						}
					}
					p = p->NextLine;
					++i;
				}//while (p)
				p = CityList[father].FirstLine;
				i = 0;
				while (i != count) {
					p = p->NextLine;
					mm += p->Info->SpendMoney;
					++i;
				}
				if (father != StartNum) {
					cout << "��Ҫ��ת�ȴ� " << timeTransWeight(p->Info->StartTime - et) << "���ӣ�" << endl;
				}
				cout << setw(8) << CityList[father].CityName << " " << setw(8) << p->EndName << " " << setw(6) << p->Info->LineName << " ";
				cout.fill('0');
				cout << p->Info->StartTime << " " << p->Info->EndTime << " " << p->Info->SpendTime << " " << setiosflags(ios_base::showpoint) << p->Info->SpendMoney << endl; //showpointֻ��float��Ч,�����ֶ�ȡ��
				cout.fill(' ');

				et = p->Info->EndTime;
				father = child;
			}//while(!s.empty())
			cout << "һ������" << mm << "Ԫ��" << time_minute << "���ӣ�" << endl;
		}//�����ʡʱ���·��
	}//��·��
}//showShortestPath
