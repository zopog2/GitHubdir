
#include "fuction.h"
using namespace std;

//12091600 이명규 수치프로그래밍 과제1

int main()
{
	int sel = -1;	//스위치문을 위한 변수
	double a = 0;	//입력받는 값
	cout << "Select fuction" << endl;

	while (sel != 0)
	{
		cout << endl;
		cout << "1: square root(double type)" << endl;
		cout << "2 : square root(unsigned long type)" << endl;
		cout << "3 : sine(double type)" << endl;
		cout << "4 : cosine(double type)" << endl;
		cout << "0 : quit" << endl;

		cin >> sel;

		switch (sel)		//각각의 인터페이스 구현
		{
		case 1:
			cout << "Input double number : ";
			cin >> a;
			printf("Result is %lf\n", sqrt_d(a));
			break;
		case 2:
			cout << "Input int number : ";
			cin >> a;
			printf("Result is %d\n", sqrt_i(a));
			break;
		case 3:
			cout << "Input double number : ";
			cin >> a;
			printf("Result is %fl\n", sine_d(a));
			break;
		case 4:
			cout << "Input double number : ";
			cin >> a;
			printf("Result is %fl\n", cosine_d(a));
			break;
		case 0:								//종료
			cout << "Good bye!" << endl;
			break;
		default:
			cout << "You have wrong number!" << endl;	//선택에 대한 예외처리
			
		}

	}

	
	return 0;
}
