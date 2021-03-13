#include <iostream>
#include <string>
#include <stack>
#include <iomanip>

using namespace std;

#define acc 0       // 接受状态
#define error 16  // 出错状态

string s;//输入字符串

//>0表示Shift，<0表示Reduce
int Action[16][8] = {
	{ 4, error, error, error, error,error,5,error },             //0
	{ error, error, 6, 7, error, error, error, acc },            //1
	{ error, -3, -3, -3, 8, 9, error, -3 },                         //2 
	{ error, -6, -6, -6, -6, -6, error, -6},                      //3 
	{ 4, error, error, error, error, error, 5, error },          //4 
	{ error, -8, -8, -8, -8, -8, error, -8 },						//5 
	{ 4, error, error, error, error, error, 5, error },          //6
	{ 4, error, error, error, error, error, 5, error },          //7
	{ 4, error, error, error, error, error, 5, error },          //8
	{ 4, error, error, error, error, error, 5, error },          //9
	{ error, 15, 6, 7, error, error, error, error },              //10
	{ error, -1, -1, -1, 8, 9, error, -1 },						     //11
	{ error, -2, -2, -2, 8, 9, error, -2 },							 //12
	{ error, -4, -4, -4, -4, -4, error, -4 },						 //13
	{ error, -5, -5, -5, -5, -5, error, -5 },						 //14
	{ error, -7, -7, -7, -7, -7, error, -7 }						 //15
};

int GoTo[16][3] = {
	{ 1, 2, 3 },                   // 0
	{ error, error, error },  // 1
	{ error, error, error },  // 2
	{ error, error, error },  // 3
	{ 10, 2, 3 },                 // 4
	{ error, error, error },  // 5
	{ error, 11, 3 },           // 6
	{ error, 12, 3 },           // 7
	{ error, error, 13 },     // 8
	{ error, error, 14 },     // 9
	{ error, error, error }, // 10
	{ error, error, error }, // 11
	{ error, error, error }, // 12
	{ error, error, error }, // 13
	{ error, error, error }, // 14
	{ error, error, error }, // 15
};

//终结符
string vt[8] = { "(", ")", "+", "-", "*", "/", "num", "$" };
//非终结符
string vn[3] = { "E", "T", "F" };
//产生式
string Production[9] = {"S->E", "E->E+T", "E->E-T", "E->T", "T->T*F", "T->T/F", "T->F", "F->(E)", "F->num" };

int index = 0;//记录当前处理的输入字符串字符位置
int line = 1;//记录步骤数
bool flag = false;
string stackRec = "$";//记录符号栈中内容
stack <string> Stack;//符号栈
stack <int> status;//状态栈
int NumOfStatusInStack = 1;//栈中状态数
int Status[100] = { 0 };//记录状态栈中内容



void OutputStatus() {//输出状态集
	for (int i = 0; i < NumOfStatusInStack; i++)
		cout << Status[i];
}

void OutputRestString() {//输出未处理的字符串
	for (int i = index; i < s.size(); i++)
		cout << s.at(i);
}

void Output() {//输出步骤、 状态集、 符号集、 输入串
	cout << line << "\t\t";
	OutputStatus();
	cout << "\t\t" << stackRec << "\t\t";
	OutputRestString();
	cout << "\t\t";
	line++;
}

//判断输入串是否为合法符号组成并找到位置
void Locate(int& i, int j, string arr[], string input) {
	flag = false;
	for (int k = 0; k < j; k++) {
		if (input == arr[k]) {
			flag = true;
			i = k;
			break;
		}
	}
	if (flag == false) {
		cout << "\t错误：存在非法符号" << endl;
		index = s.size();
	}
}

//移进
void Shift(int i, string ch) {
	Output();
	cout << "ACTION[" << status.top() << "," << s.at(index) << "]=S" << i << ",状态" << i << "入栈" << endl;
	status.push(i);//将状态 i 压进状态栈
	Status[NumOfStatusInStack] = i;//Status 记录状态栈的内容
	Stack.push(ch);//将当前面临的输入串符号压进符号栈
	stackRec = stackRec + ch;//stackRec 记录符号栈的内容
	if (ch == "num")
		index += 3;
	else
		index++;//当前面临的输入串字符往后移一位
	NumOfStatusInStack++;//状态数加一
}

//Goto
void Goto(stack <int> st1, stack <string> st2, string s) {
	int j = -1;
	int ch1 = st1.top();//当前状态
	string ch2 = st2.top();//符号
	Locate(j, 3, vn, ch2);//求得 ch2 在非终结符表中的位置
	if (GoTo[ch1][j] == 0) {
		cout << "\t错误：非法状态" << endl;
		index = s.size();
	}
	else {
		status.push(GoTo[ch1][j]);//新状态进栈
		Status[NumOfStatusInStack] = GoTo[ch1][j];
		NumOfStatusInStack++;
	}
}

//归约
void Reduce(int i, string s) {
	Output();
	cout << "R" << -i << ":" << Production[-i] << "归约， GoTo(";
	int N = Production[-i].length() - 3;//减去前缀
	if (-i == 8)
		N = 1;
	for (int j = 0; j < N; j++) {//从栈中弹出状态和符号
		status.pop();
		Stack.pop();
		NumOfStatusInStack--;
		if(-i==8)
			stackRec.erase(stackRec.length() - 3,3);
		else
			stackRec.erase(stackRec.length() - 1);
	}
	cout << status.top() << "," << Production[-i].at(0) << ")=";
	string temp = "a";
	temp[0] = Production[-i].at(0);
	Stack.push(temp);//符号进栈
	stackRec = stackRec + Stack.top();
	Goto(status, Stack, s);
	cout << status.top() << "入栈" << endl;
	Status[NumOfStatusInStack] = status.top();
}

//预测分析控制程序
void Control() {
	Stack.push("$");//初始化
	status.push(0);
	s = s + "$";//在输入字符串末尾加上$
	int loc = -1;//记录读取字符串的位置
	while (index < s.size()) {
		int i = status.top();
		//字符转字符串
		char tmp = s.at(index);
		string curStr = "a";
		curStr[0] = tmp;
		
		//num
		if (tmp == 'n') {
			tmp= s.at(index+1);
			if (tmp == 'u') {
				tmp = s.at(index+2);
				if (tmp == 'm') {
					curStr="num";
				}
			}
		}
		Locate(loc, 8, vt, curStr);
		if (flag == true) {//当前字符串合法
			if (Action[i][loc] != acc && Action[i][loc] != error ) {
				if (Action[i][loc]>0) {//移进
					Shift(Action[i][loc], curStr);
				}
				else if (Action[i][loc]< 0) {//归约
					Reduce(Action[i][loc], curStr);
				}
			}
			else if (Action[i][loc] == error) {//出错
				cout << "\t错误:非法状态" << endl;
				break;
			}
			else if (Action[i][loc] == acc) {//接受
				Output();
				cout << "acc" << "\t 分析成功" << endl;
				break;
			}
		}
		else if (flag == false)
			break;
	}
}

int main() {
	cout << "------------------------------------------------------------------" << endl;
	cout << "文法：" << endl;
	for (int j = 0; j < 9; j++)
	{
		cout << Production[j] << endl;
	}
	cout << "非终结符：" << endl;
	for (int i = 0; i < 3; i++)
	{
		cout << vn[i] << "  ";
	}
	cout << endl;
	cout << "终结符：" << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << vt[i] << "  ";
	}
	
	cout << endl;
	cout << "-------------------------------SLR(1)分析---------------------------------" << endl;
	cout << "输入字符串" << endl;
	cin >> s;//输入要分析的字符串
	cout << "-------------------------------------------------------------------------" << endl;
	cout <<"步骤" <<"\t\t" << "状态栈" << "\t\t" << "符号栈" <<"\t\t" << "输入" <<"\t\t" << "动作" << endl;
	cout << endl;
	Control();
	while (!Stack.empty()) {
		Stack.pop();
	}
	while (!status.empty()) {
		status.pop();
	}
	return 0;
}