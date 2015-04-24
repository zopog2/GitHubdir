#include <stdio.h> 
#include <iostream>

#define SQRT_2 1.41421356	//�� ��� ����
#define twopi 6.2831852
#define pi 3.1415926
#define halfpi 1.5707963
#define pi_over_four 0.7853982

typedef union {				//double�� �������� ���� union
	double fp;
	unsigned long long ull;	//64bitũ���� ull, �����ڷ�ʹ� �ٸ��� �Ѳ����� �����Ѵ�.
} hack_structure;

double my_sqrt(double a, int n) {	//���� ����� ���� ������ ���ϴ� �Լ�, ��ư��� �̿�
	double x;
	int i;
	if (a <= 0.0) return a;

	x = (0.4 * a) + 0.6;			//�Է°��� ������ 1�̻� 2�̸����� ����� �ʱⰪ�� y=0.4x+0.6�Լ��� �̿� 
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
		
	expo = (short)(x.ull >> 52);	//���� 3�ڸ��� ���� ������ ���ϱ� ���� 52��Ʈ(16������ 13�ڸ�)�� shift����
	expo -= 0x3ff;

	x.ull &= 0x000fffffffffffff;
	x.ull += 0x3ff0000000000000;

	x.fp = my_sqrt(x.fp, 10);		//�����κи� my_sqrt�Լ��� ��� ������ 10ȸ����

	if (expo & 1) {
		++expo;
		odd = 1;
	}

	expo /= 2;						//������ ������ ����

	expo += 0x3ff;
	x.ull &= 0x000fffffffffffff;
	x.ull += ((unsigned long long)expo << 52);	//������ ������ ������ ���� ������ ����

	if (odd) x.fp /= SQRT_2;
	return x.fp;
}
//---------------------------------


unsigned long sqrt_i(unsigned long a)
{
	if (a == 0 || a > 2147483647)	//unsigned longŸ���� 2147483647�� ������ doubleŸ�Կ��� ������ �ȴ�. �̷��Ѱ�� 0�� ��ȯ�Ѵ�.
		return 0;
	int count = 0;					//�ְ��ڸ� ����� ���� ����
	unsigned long root = 1;			//�ӽ� ������
	unsigned long add = 1;			//root���� ���ڸ� ���� ��, �� ������ ���ϰ� �������� �� �ڸ��� 0����1���� �Ǻ��Ѵ�.
	unsigned long com = a;			//�Է°� a�� �����ϱ� ���� a��� ��ȭ�ϴ� ����
	while (com != 0)				//�Ѻ�Ʈ�� shift������ 0�� �ɶ����� �ݺ��Ͽ� count�� ����, �ְ��ڸ��� ã�´�.
	{
		com >>= 1;
		count++;
	}
	count = (count - 1) / 2 ;		//�������� �������� �ְ��ڸ��� ��ȯ
	root = (root << count);			//�ʱ� root�� count��ŭ �����·� ����
	add = root >> 1;				//add�� root���� ���ڸ� ���� ���·� ����
	while (count > 0)
	{		
		if (root * root == a)		//root�� �ٷ� �������� ���
			return root;
		root += add;				//add�� ���غ��� �Ǵ� root������ a���� ������ �ش��ڸ� 1 ũ�� �ش��ڸ�0�̹Ƿ� ���󺹱�
		if (root * root > a)
			root -= add;
		count--;					//�����ؾ��� �ڸ��� ����
		add >>= 1;				//add �Ѻ�Ʈ ����
	}
	return root;

}

//--------------------------------

double sine_d(double x)
{
	int signflag = 1;			//�Էµȼ��� ��ȣ �Ǻ��� ���� ���� ����� 1 ������ -1
	double result = 0;			//�ӽ� ���
	if (x < 0)					//�Էµ� ���� ��ȣ�Ǻ�, ���Լ��� ������ �̿�
	{							
		signflag = -1;			//������ ��� signflag�� -1�� ����, ���� ���밪�� ���� ����� ��ȯ
		x = -x;
	}
		
	long n = (long)(x / halfpi + 0.5);	//halfpi�� ���� �ֱ�� ���
		
	x -= n * halfpi;					//������ ���� �ֱ���� �̿��Ͽ� ���� 45�� ���Ϸ� ����

	double s1 = 1.0 / (2.0*3.0);		//�Լ�ȣ���� ���ϱ� ���� ���, ���Ϸ��޼��� �̿��Ѵ�.
	double s2 = 1.0 / (4.0*5.0);
	double s3 = 1.0 / (6.0*7.0);
	double s4 = 1.0 / (8.0*9.0);

	double c1 = 1.0 / (1.0*2.0);
	double c2 = 1.0 / (3.0*4.0);
	double c3 = 1.0 / (5.0*6.0);
	double c4 = 1.0 / (7.0*8.0);
	double z = 0;

	n = n & 0x3;						//&������ ���� �ֱ���� 4�� ���� ������ ���, �ش翵�� ã��

	switch (n)							//�ﰢ�Լ� ������ �̿��� �� ������ ���� ����� ���� ��ȯ, �Է½� ������� -1�� ���Ѵ�.
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
	double result = 0;			//���Լ� �̹Ƿ� ��ȣ����� �ʿ������ �ʴ´�.
	if (x < 0)					//������ ��� ���밪�� ���� ����� ���� ��ȯ
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
	switch (n)						//���� sine�� ����, cosine�� sine�� halfpi��ŭ �����̵��� ���̹Ƿ� �� ������ �ð�������� ȸ��
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
