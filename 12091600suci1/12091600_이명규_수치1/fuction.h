#include <stdio.h> 
#include <iostream>

#define SQRT_2 1.41421356	//각 상수 정의
#define twopi 6.2831852
#define pi 3.1415926
#define halfpi 1.5707963
#define pi_over_four 0.7853982

typedef union {				//double형 제곱근을 위한 union
	double fp;
	unsigned long long ull;	//64bit크기의 ull, 강의자료와는 다르게 한꺼번에 제어한다.
} hack_structure;

double my_sqrt(double a, int n) {	//최종 계산을 위한 제곱근 구하는 함수, 뉴튼방식 이용
	double x;
	int i;
	if (a <= 0.0) return a;

	x = (0.4 * a) + 0.6;			//입력값의 범위가 1이상 2미만임을 고려해 초기값을 y=0.4x+0.6함수를 이용 
	for (i = 0; i < n; i++) x = 0.5 * (a / x + x);
	return x;
}

double sqrt_d(double a)
{		
	short expo;
	unsigned short sign;
	char odd = 0; 
	hack_structure x;
	x.fp = a;
		
	expo = (short)(x.ull >> 52);	//앞의 3자리를 남겨 지수를 구하기 위해 52비트(16진수로 13자리)를 shift연산
	expo -= 0x3ff;

	x.ull &= 0x000fffffffffffff;
	x.ull += 0x3ff0000000000000;

	x.fp = my_sqrt(x.fp, 10);		//가수부분만 my_sqrt함수로 계산 루프는 10회고정

	if (expo & 1) {
		++expo;
		odd = 1;
	}

	expo /= 2;						//지수를 반으로 나눔

	expo += 0x3ff;
	x.ull &= 0x000fffffffffffff;
	x.ull += ((unsigned long long)expo << 52);	//반으로 나눠준 지수와 계산된 가수를 결합

	if (odd) x.fp /= SQRT_2;
	return x.fp;
}
//---------------------------------


unsigned long sqrt_i(unsigned long a)
{
	if (a == 0 || a > 2147483647)	//unsigned long타입은 2147483647를 넘으면 double타입에서 음수가 된다. 이러한경우 0을 반환한다.
		return 0;
	int count = 0;					//최고자리 계산을 위한 변수
	unsigned long root = 1;			//임시 제곱근
	unsigned long add = 1;			//root보다 한자리 적은 수, 이 변수를 더하고 비교함으로 각 자리가 0인지1인지 판별한다.
	unsigned long com = a;			//입력값 a를 보존하기 위해 a대신 변화하는 변수
	while (com != 0)				//한비트씩 shift연산후 0이 될때가지 반복하여 count를 누적, 최고자리를 찾는다.
	{
		com >>= 1;
		count++;
	}
	count = (count - 1) / 2 ;		//구해지는 제곱근의 최고자리로 변환
	root = (root << count);			//초기 root는 count만큼 간상태로 시작
	add = root >> 1;				//add는 root보다 한자리 작은 상태로 시작
	while (count > 0)
	{		
		if (root * root == a)		//root가 바로 구해지는 경우
			return root;
		root += add;				//add를 더해본후 판단 root제곱이 a보다 작으면 해당자리 1 크면 해당자리0이므로 원상복귀
		if (root * root > a)
			root -= add;
		count--;					//진행해야할 자리수 감소
		add >>= 1;				//add 한비트 감소
	}
	return root;

}

//--------------------------------

double sine_d(double x)
{
	int signflag = 1;			//입력된수의 부호 판별을 위한 변수 양수는 1 음수는 -1
	double result = 0;			//임시 결과
	if (x < 0)					//입력된 수의 부호판별, 기함수의 성질을 이용
	{							
		signflag = -1;			//음수의 경우 signflag를 -1로 설정, 각을 절대값이 같은 양수로 전환
		x = -x;
	}
		
	long n = (long)(x / halfpi + 0.5);	//halfpi에 대한 주기수 계산
		
	x -= n * halfpi;					//위에서 구한 주기수를 이용하여 각을 45도 이하로 줄임

	double s1 = 1.0 / (2.0*3.0);		//함수호출을 피하기 위한 상수, 테일러급수를 이용한다.
	double s2 = 1.0 / (4.0*5.0);
	double s3 = 1.0 / (6.0*7.0);
	double s4 = 1.0 / (8.0*9.0);

	double c1 = 1.0 / (1.0*2.0);
	double c2 = 1.0 / (3.0*4.0);
	double c3 = 1.0 / (5.0*6.0);
	double c4 = 1.0 / (7.0*8.0);
	double z = 0;

	n = n & 0x3;						//&연산을 통해 주기수를 4로 나눈 나머지 계산, 해당영역 찾음

	switch (n)							//삼각함수 성질을 이용해 각 영역에 대한 계산결과 직접 반환, 입력시 음수라면 -1을 곱한다.
	{
	case 0:
		z = x*x;
		result = signflag * ((((s4*z - 1.0)*s3*z + 1.0)*s2*z - 1.0)*s1*z + 1.0)*x;
		break;
	case 1:
		z = x*x;
		result = signflag * ((((c4*z - 1.0)*c3*z + 1.0)*c2*z - 1.0)*c1*z + 1.0);
		break;
	case 2:
		z = x*x;
		result = signflag * (-((((s4*z - 1.0)*s3*z + 1.0)*s2*z - 1.0)*s1*z + 1.0)*x);
		break;
	case 3:
		z = x*x;
		result = signflag * (-((((c4*z - 1.0)*c3*z + 1.0)*c2*z - 1.0)*c1*z + 1.0));
		break;
	default:
		return 0;
	}
	return result;
}

double cosine_d(double x)
{
	double result = 0;			//우함수 이므로 부호계산을 필요로하지 않는다.
	if (x < 0)					//음수의 경우 절대값이 같은 양수로 각을 전환
	{
		x = -x;
	}

	long n = (long)(x / halfpi + 0.5);

	x -= n * halfpi;

	double s1 = 1.0 / (2.0*3.0);
	double s2 = 1.0 / (4.0*5.0);
	double s3 = 1.0 / (6.0*7.0);
	double s4 = 1.0 / (8.0*9.0);

	double c1 = 1.0 / (1.0*2.0);
	double c2 = 1.0 / (3.0*4.0);
	double c3 = 1.0 / (5.0*6.0);
	double c4 = 1.0 / (7.0*8.0);
	double z = 0;

	n = n & 0x3;
	switch (n)						//위의 sine과 동일, cosine은 sine을 halfpi만큼 평행이동한 것이므로 한 영역씩 시계방향으로 회전
	{
	case 0:
		z = x*x;
		result = ((((c4*z - 1.0)*c3*z + 1.0)*c2*z - 1.0)*c1*z + 1.0);
		break;
	case 1:
		z = x*x;
		result = -((((s4*z - 1.0)*s3*z + 1.0)*s2*z - 1.0)*s1*z + 1.0)*x;
		break;
	case 2:
		z = x*x;
		result = (-((((c4*z - 1.0)*c3*z + 1.0)*c2*z - 1.0)*c1*z + 1.0));
		break;
	case 3:
		z = x*x;
		result = ((((s4*z - 1.0)*s3*z + 1.0)*s2*z - 1.0)*s1*z + 1.0)*x;
		break;
	default:
		return 0;
	}
	return result;
}
