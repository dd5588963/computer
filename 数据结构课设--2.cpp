﻿/*************************************************************************
	> File Name: main.cpp
	> Author: bw98
	> Mail:
	> Created Time: 2018年01月04日 星期四 10时59分38秒
 ************************************************************************/
#include<iostream>
#include<string>
#include"ALGraph.h"
using namespace std;

string Account = "123456";
string Password = "123456";
ALGraph ALG[2] = { ALGraph(30), ALGraph(50) };
char tool[7] = { "Train" };
char cityfile[15] = {"TrainCity.txt" };
char linefile[11] = {"Train.txt" };
void admin();
bool login();
void user();
void changePassword();
void adminALG(int option);


int main(void) {
	system("cls");
	int identity = 1; //身份选项 1.游客 2.管理员，输入 0 用于退出
	while (identity) {
		cout << endl << "==============欢迎全国交通咨询系统==============" << endl;
		cout << "你的身份是？" << endl;
		cout << "    1.管理员" << endl << "    2.普通用户" << endl;
		cout << "请输入数字(0 for exit)：";
		cin >> identity;
		if (!identity) { break; }
		switch (identity) {
		case 1:
			admin();
			system("cls");
			break;
		case 2:
			user();
			system("cls");
			break;
		default:
			cout << "输入不正确，请重新输入" << endl;
			break;
		}//switch(identity)
	}//while (indentity)
	system("cls");
	cout << endl << "==================感谢你的使用==================" << endl;
	return 0;
}//main

//管理员界面
void admin() {
	cout << "    开启管理员模式！" << endl;
	while (!login()) {
		cout << endl << "帐号或密码不正确，请再次输入" << endl;
	}
	cout << endl << "登陆成功！" << endl;
	int option = 1;
	while (option) {
		cout << "请输入你的选项：" << endl;
		cout << "1.更改密码" << endl << "2.全国列车班次的相关操作" << endl;
		cout << "请输入数字(按0返回选择身份界面)：" << endl;
		cin >> option;
		cout << endl;
		system("cls");
		if (!option) { break; }
		switch (option) {
		case 1:
			changePassword();
			break;
		case 2:
			adminALG(option);
			system("cls");
			break;
		default:
			cout << "请输入正确的选项！" << endl;
			break;
		}
	}//while(!login())
	cout << endl << endl << "操作完毕，退出管理员用户！" << endl << endl;
}//admin

//登入管理员账户函数
bool login() {
	string account, password;
	cout << "输入账户：";
	cin >> account;
	cout << "输入密码：";
	/*system("stty -echo");*/
	cin >> password;
	/*system("stty echo");*/
	if (account == Account && password == Password) {
		return true;
	}
	else {
		return false;
	}
}//login

//修改管理员账户密码函数
void changePassword() {
	string NewPassword1 = "1", NewPassword2 = "2";
	while (NewPassword1 != NewPassword2) {
		cout << "请输入新密码：";
		/*system("stty -echo");*/
		cin >> NewPassword1;
		/*system("stty echo");*/
		cout << "请再次输入密码：";
		/*system("stty -echo");*/
		cin >> NewPassword2;
		/*system("stty echo");*/
		if (NewPassword1 != NewPassword2) {
			cout << "两次输入的密码不同，请再次输入！" << endl;
		}
	}
	Password = NewPassword1;
	cout << "修改密码成功！" << endl;
}//changePassword

//管理员对航班图、列车图的操作函数
void adminALG(int option) {
	int func = 1;
	while (func) {
		cout << "    现在修改的交通图是：" << tool[option - 2] << endl;
		cout << "请选择下列操作之一" << endl;
		cout << "    1.从文件中添加城市" << endl;
		cout << "    2.手动添加城市" << endl;
		cout << "    3.删除城市" << endl;
		cout << "    4.从文件中添加线路" << endl;
		cout << "    5.手动添加线路" << endl;
		cout << "    6.删除线路" << endl;
		cout << "    7.显示所有城市" << endl;
		cout << "    8.显示所有线路" << endl;
		cout << "    9.保存修改到文件" << endl;
		cout << "    10.查询花费最少的路径" << endl;
		cout << "    11.查询耗时最少的路径" << endl;
		cout << "请输入数字(输入0退出操作界面)：";
		cin >> func;
		cout << endl;
		system("cls");
		//对应功能的调用
		string name;
		if (!func) {
			break;
		}
		switch (func) {
		case 1:
			cout << "    1.从文件中添加城市!" << endl;
			ALG[option - 2].addCityFromFile(cityfile);
			break;
		case 2:
			cout << "    2.手动添加城市!" << endl;
			cin >> name;
			ALG[option - 2].addCity(name);
			break;
		case 3:
			cout << "    3.删除城市!" << endl;
			cin >> name;
			ALG[option - 2].delCity(name);
			break;
		case 4:
			cout << "    4.从文件中添加线路!" << endl;
			ALG[option - 2].addLineFromFile(linefile);
			break;
		case 5:
			cout << "    5.手动添加线路!" << endl;
			ALG[option - 2].addLine();
			break;
		case 6:
			cout << "    6.删除线路!" << endl;
			ALG[option - 2].delLine(); //删除线路
			break;
		case 7:
			cout << "    7.显示所有城市!" << endl;
			ALG[option - 2].showCity();
			break;
		case 8:
			cout << "    8.显示所有线路!" << endl;
			ALG[option - 2].showLine();
			break;
		case 9:
			cout << "    9.保存修改到文件!" << endl;
			ALG[option - 2].updateFile(cityfile, "City");
			ALG[option - 2].updateFile(cityfile, "Line");
			break;
		case 10:
			cout << "    10.查询花费最少的路径!" << endl;
			ALG[option - 2].showShortestPath("Money");
			break;
		case 11:
			cout << "    11.查询耗时最少的路径!" << endl;
			ALG[option - 2].showShortestPath("Time");
			break;
		default:
			cout << "输入不正确，请重新输入" << endl;
			break;
		}//switch(func) 
		cout << endl;
	}//while(func)
}//adminGPL 管理员对图进行操作 

//用户登陆界面
void user() {
	cout << "    开启普通用户模式" << endl << endl;
	int t = 1, func = 1;
	while (t) {
		cout <<"你的出行方式为火车" << endl;
		cout << "请输入数字(输入0返回身份认证界面)：" << endl;
		cin >> t;
		if (!t) {
			break;
		}
		if (t != 1 && t != 2) {
			cout << "输入不正确，请重新输入！" << endl;
			continue;
		}
		cout << endl;
		while (func) {
			cout << "    现在是关于全国火车的信息查询！" << endl;
			cout << "请选择下列操作之一" << endl;
			cout << "    1.查询花费最少的路径" << endl;
			cout << "    2.查询耗时最少的路径" << endl;
			cout << "    3.显示所有城市" << endl;
			cout << "    4.显示所有线路" << endl;
			cout << "请输入数字(按0返回选择交通工具界面):";
			cin >> func;
			if (!func) {
				break;
			}
			cout << endl;
			switch (func) {
			case 1:
				cout << "1.查询花费最少的路径！" << endl;
				ALG[t - 1].showShortestPath("Money");
				break;
			case 2:
				cout << "2.查询耗时最少的路径！" << endl;
				ALG[t - 1].showShortestPath("Time");
				break;
			case 3:
				cout << "3.显示所有城市！" << endl;
				ALG[t - 1].showCity();
				break;
			case 4:
				cout << "4.显示所有线路！" << endl;
				ALG[t - 1].showLine();
				break;
			default:
				cout << "输入不正确，请重新输入！" << endl;
				break;
			}//switch(func)
			cout << endl;

		}//while(func)
	}//while(t)
}//user 
