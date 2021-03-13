#include <iostream>
#include <string>
#include <stack>
#include <iomanip>

using namespace std;

#define acc 0       // ����״̬
#define error 16  // ����״̬

string s;//�����ַ���

//>0��ʾShift��<0��ʾReduce
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

//�ս��
string vt[8] = { "(", ")", "+", "-", "*", "/", "num", "$" };
//���ս��
string vn[3] = { "E", "T", "F" };
//����ʽ
string Production[9] = {"S->E", "E->E+T", "E->E-T", "E->T", "T->T*F", "T->T/F", "T->F", "F->(E)", "F->num" };

int index = 0;//��¼��ǰ����������ַ����ַ�λ��
int line = 1;//��¼������
bool flag = false;
string stackRec = "$";//��¼����ջ������
stack <string> Stack;//����ջ
stack <int> status;//״̬ջ
int NumOfStatusInStack = 1;//ջ��״̬��
int Status[100] = { 0 };//��¼״̬ջ������



void OutputStatus() {//���״̬��
	for (int i = 0; i < NumOfStatusInStack; i++)
		cout << Status[i];
}

void OutputRestString() {//���δ������ַ���
	for (int i = index; i < s.size(); i++)
		cout << s.at(i);
}

void Output() {//������衢 ״̬���� ���ż��� ���봮
	cout << line << "\t\t";
	OutputStatus();
	cout << "\t\t" << stackRec << "\t\t";
	OutputRestString();
	cout << "\t\t";
	line++;
}

//�ж����봮�Ƿ�Ϊ�Ϸ�������ɲ��ҵ�λ��
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
		cout << "\t���󣺴��ڷǷ�����" << endl;
		index = s.size();
	}
}

//�ƽ�
void Shift(int i, string ch) {
	Output();
	cout << "ACTION[" << status.top() << "," << s.at(index) << "]=S" << i << ",״̬" << i << "��ջ" << endl;
	status.push(i);//��״̬ i ѹ��״̬ջ
	Status[NumOfStatusInStack] = i;//Status ��¼״̬ջ������
	Stack.push(ch);//����ǰ���ٵ����봮����ѹ������ջ
	stackRec = stackRec + ch;//stackRec ��¼����ջ������
	if (ch == "num")
		index += 3;
	else
		index++;//��ǰ���ٵ����봮�ַ�������һλ
	NumOfStatusInStack++;//״̬����һ
}

//Goto
void Goto(stack <int> st1, stack <string> st2, string s) {
	int j = -1;
	int ch1 = st1.top();//��ǰ״̬
	string ch2 = st2.top();//����
	Locate(j, 3, vn, ch2);//��� ch2 �ڷ��ս�����е�λ��
	if (GoTo[ch1][j] == 0) {
		cout << "\t���󣺷Ƿ�״̬" << endl;
		index = s.size();
	}
	else {
		status.push(GoTo[ch1][j]);//��״̬��ջ
		Status[NumOfStatusInStack] = GoTo[ch1][j];
		NumOfStatusInStack++;
	}
}

//��Լ
void Reduce(int i, string s) {
	Output();
	cout << "R" << -i << ":" << Production[-i] << "��Լ�� GoTo(";
	int N = Production[-i].length() - 3;//��ȥǰ׺
	if (-i == 8)
		N = 1;
	for (int j = 0; j < N; j++) {//��ջ�е���״̬�ͷ���
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
	Stack.push(temp);//���Ž�ջ
	stackRec = stackRec + Stack.top();
	Goto(status, Stack, s);
	cout << status.top() << "��ջ" << endl;
	Status[NumOfStatusInStack] = status.top();
}

//Ԥ��������Ƴ���
void Control() {
	Stack.push("$");//��ʼ��
	status.push(0);
	s = s + "$";//�������ַ���ĩβ����$
	int loc = -1;//��¼��ȡ�ַ�����λ��
	while (index < s.size()) {
		int i = status.top();
		//�ַ�ת�ַ���
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
		if (flag == true) {//��ǰ�ַ����Ϸ�
			if (Action[i][loc] != acc && Action[i][loc] != error ) {
				if (Action[i][loc]>0) {//�ƽ�
					Shift(Action[i][loc], curStr);
				}
				else if (Action[i][loc]< 0) {//��Լ
					Reduce(Action[i][loc], curStr);
				}
			}
			else if (Action[i][loc] == error) {//����
				cout << "\t����:�Ƿ�״̬" << endl;
				break;
			}
			else if (Action[i][loc] == acc) {//����
				Output();
				cout << "acc" << "\t �����ɹ�" << endl;
				break;
			}
		}
		else if (flag == false)
			break;
	}
}

int main() {
	cout << "------------------------------------------------------------------" << endl;
	cout << "�ķ���" << endl;
	for (int j = 0; j < 9; j++)
	{
		cout << Production[j] << endl;
	}
	cout << "���ս����" << endl;
	for (int i = 0; i < 3; i++)
	{
		cout << vn[i] << "  ";
	}
	cout << endl;
	cout << "�ս����" << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << vt[i] << "  ";
	}
	
	cout << endl;
	cout << "-------------------------------SLR(1)����---------------------------------" << endl;
	cout << "�����ַ���" << endl;
	cin >> s;//����Ҫ�������ַ���
	cout << "-------------------------------------------------------------------------" << endl;
	cout <<"����" <<"\t\t" << "״̬ջ" << "\t\t" << "����ջ" <<"\t\t" << "����" <<"\t\t" << "����" << endl;
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