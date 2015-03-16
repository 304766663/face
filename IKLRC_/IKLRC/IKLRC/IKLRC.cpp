// IKLRC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<stdio.h>
#include<iostream>
#include<math.h>
#include<string>
#include<ctime>
#include<fstream>
#define Dimension 32*32  //����ά��(������y)
#define MIN 1.0e-06
using namespace std;

string filenametrain="Train.txt"; //ѵ���������ļ���
string filenametest="Test.txt";//���Ե������ļ���

int DataNumTrainTotal;//ѵ����������
int DataNumTestTotal;//������������
int DataNumTrain; 
int ReadNum;
int FileIndx;
//double r;//����������
double **Train_X_All;  //ѵ��������
double *Train_Y_All;   //ѵ����
double **Test_X_All;     //����������
double *Test_Y_All;      //����������
double **Train_X;
double *Label;        //������Ӧ��ż�
//double *W;
double *B;
//double *B1;
int LabelNum;
double RightCount;//��ȷ������Ե����
double RightRate;//������ȷ��
double dmin;//��С����
int Labelmin;//�洢����Ե��������ı�ע
double e;//�����㷨��ֹ���
// double temp;//�м����
// double **tmp1;//�м�ָ��
// double **tmp2;
double **XTX;
double **XTXInverse;
double *XTY;
double *Y;
double end_time;
double begin_time;
double ptime;

////////////////////////////////////////////////////////////////////////////////////
//���ڶ�ȡѵ����������
////////////////////////////////////////////////////////////////////////////////////
class GetNumAndDataTrain
{	
public:
	GetNumAndDataTrain();
	void initial();
	void GetData();
	void ReleaseTrainSpace();
};

//�ж��Ƿ����������ļ�
GetNumAndDataTrain::GetNumAndDataTrain()//���캯��
{
	fstream scanfile(filenametrain.c_str());
	if(!scanfile)
	{
		cout<<"can not open the source file!"<<endl;
		exit(0);
	}
	else
		cout<<"open file successfully!"<<endl;
	scanfile.close();
}

void GetNumAndDataTrain::initial()//������ݵ����
{	
	cout<<"�����ļ�ɨ����,������ݵ����,���Ժ�..."<<endl<<endl;

	double element;
	double Label;

	DataNumTrainTotal=0;
	fstream inf(filenametrain.c_str());
	while (!inf.eof())
	{
		ReadNum=0;
		while (ReadNum<Dimension)
		{
			if (!inf.eof()) 
			{
				inf>>element;
				ReadNum++;
			}
			else
			{
				if(ReadNum<Dimension)
					break;
			}
		}    
		if (inf.eof())
		{
			if(ReadNum<Dimension)
				continue;
		}
		inf>>Label;
		DataNumTrainTotal++;

	}
	inf.close();
	cout<<"ѵ���㹲������:"<<DataNumTrainTotal<<"��;"<<endl<<endl;
}
//��ȡ���ݵ�
void GetNumAndDataTrain::GetData()
{
	int i,j;
	double element;
	double Label;
	fstream infile(filenametrain.c_str());
	Train_X_All=new double*[DataNumTrainTotal];
	for(i=0;i<DataNumTrainTotal;i++)
	{
		Train_X_All[i]=new double[Dimension];
	}
	Train_Y_All=new double[DataNumTrainTotal];

	for(i=0;i<DataNumTrainTotal;i++)
	{

		for(j=0;j<Dimension;j++)
		{
			infile>>element;
			Train_X_All[i][j]=element;
		}
		infile>>Label;
		Train_Y_All[i]=Label;
	}
	infile.close();
}

void GetNumAndDataTrain::ReleaseTrainSpace()
{
	for(int i=0;i<DataNumTrainTotal;i++)
		delete Train_X_All[i];
	delete []Train_X_All;
	delete []Train_Y_All;
}

//-----------------------------��ȡ�������ݼ�---------------------------
class GetNumAndDataTest
{	
public:
	GetNumAndDataTest();
	void initial();
	void GetData();
	void ReleaseTestSpace();
};

GetNumAndDataTest::GetNumAndDataTest()//���캯��
{
	fstream scanfile(filenametest.c_str());
	if(!scanfile)
	{
		cout<<"can not open the source file!"<<endl;
		exit(0);
	}
	else
		cout<<"open file successfully!"<<endl;
	scanfile.close();
}

void GetNumAndDataTest::initial()//������ݵ����
{
	cout<<"�����ļ�ɨ����,������ݵ����,���Ժ�..."<<endl<<endl;

	double element;
	double Label;
	DataNumTestTotal=0;
	fstream inf(filenametest.c_str());
	while (!inf.eof())
	{
		ReadNum=0;
		while (ReadNum<Dimension)
		{
			if (!inf.eof())
			{
				inf>>element;
				ReadNum++;
			}
			else
			{
				if(ReadNum<Dimension)
					break;
			}
		}    
		if (inf.eof())
		{
			if(ReadNum<Dimension)
				continue;
		}
		inf>>Label;
		DataNumTestTotal++;
	}
	inf.close();
	cout<<"���Ե㹲������:"<<DataNumTestTotal<<"��;"<<endl<<endl;
}
//��ȡ���ݵ�
void GetNumAndDataTest::GetData()
{
	int i,j;
	double element;
	double Label;
	fstream infile(filenametest.c_str());
	Test_X_All=new double*[DataNumTestTotal];
	for(i=0;i<DataNumTestTotal;i++)
	{
		Test_X_All[i]=new double[Dimension];
	}
	Test_Y_All=new double[DataNumTestTotal];

	for(i=0;i<DataNumTestTotal;i++)
	{
		for(j=0;j<Dimension;j++)
		{
			infile>>element;
			Test_X_All[i][j]=element;
		}
		infile>>Label;
		Test_Y_All[i]=Label;
	}
	infile.close();
}


void GetNumAndDataTest::ReleaseTestSpace()
{
	for(int i=0;i<DataNumTestTotal;i++)
		delete Test_X_All[i];
	delete []Test_X_All;
	delete []Test_Y_All;
}

//////////////////////////////////////////////////////////////////////////////////////////

void GetLabelOrdered()//���������㷨,�����ѵ���ļ��ж������
{
	int i,j;
	LabelNum=0;
	int decide=0;
	double *remember;
	remember=new double[DataNumTrainTotal];            //���²�ͬ��ע
	for(i=0;i<DataNumTrainTotal;i++)
	{
		j=0;
		while(j<i)
		{
			if(Train_Y_All[i]==Train_Y_All[j])
				decide++;
			j++;
		}
		if(decide==0)
		{
			remember[LabelNum]=Train_Y_All[i];
			LabelNum++;
		}
		else
			decide=0;
	}

	Label=new double[LabelNum];

	for(i=0;i<LabelNum;i++)
		Label[i]=remember[i];
	delete []remember;
}

void ReadData(GetNumAndDataTrain GetTrain,GetNumAndDataTest GetTest)
{
	GetTrain.initial();         //���ѵ�����ݼ��������ܸ���
	GetTrain.GetData();         //��������
	GetTest.initial();         //��ò������ݼ��������ܸ���
	GetTest.GetData();         //��������
	GetLabelOrdered();			//���ѵ������������Լ����ע
}
//�������עΪn�����ж��ٸ�ѵ������
int LabelCount(int n)
{
	int i,num=0;
	for(i=0;i<DataNumTrainTotal;i++)
	{
		if(Train_Y_All[i]==n)
			num++;
	}
	return num;
}


void GenerateDataForTraining(int label)
{
	int i,j;
	Train_X=new double*[DataNumTrain];
	j=0;
	for(i=0;i<DataNumTrainTotal;i++)
	{
		if(Train_Y_All[i]==label)
		{
			Train_X[j]=Train_X_All[i];
			j++;
		}
	}
}
//��������ŷ�Ͼ���
double Distance(double *u,double *v,int n)
{
	int i;
	double sum=0;
	for(i=0;i<n;i++)
		sum+=(u[i]-v[i])*(u[i]-v[i]);
	return sum;
}

//�ⷽ���������ԪGauss��ȥ��
void Gauss(double **p,double *q,int n)
{
	int i,j,k,l,ik;
	double max,temp;
	double *tmp;
	for(k=0;k<=n-2;k++)
	{
		max=p[k][k];
		l=k;
		for(ik=k;ik<=n-1;ik++)//������Ԫ���Ԫ��
		{
			if(fabs(p[ik][k])>fabs(max))//l�洢���е������Ԫ��������
			{
				l=ik;
				max=p[l][k];
			}
		}
		if(l!=k)//����������ԪԪ��������l������k,����
		{
			tmp=p[l];
			p[l]=p[k];
			p[k]=tmp;//�����д������������������ik����k��,������ָ��
			temp=q[l];
			q[l]=q[k];
			q[k]=temp;//�����Ҷ�����
		}
		for(i=k+1;i<=n-1;i++)
		{
			temp=p[i][k]/p[k][k];
			for(j=k;j<=(n-1);j++)
				p[i][j]=p[i][j]-temp*p[k][j];
			q[i]=q[i]-temp*q[k];
		}
	}
	for(k=n-1;k>=0;k--)
	{	
		if(k==n-1)
			temp=0;
		else
		{
			temp=0;
			for(j=k+1;j<=n-1;j++)
				temp+=p[k][j]*q[j];
		}
		q[k]=(q[k]-temp)/p[k][k];
		if(fabs(q[k])<MIN)
			q[k]=0;
	}
}
void VarInit()
{
	//XTXInverse m m
	//XTY        m 1
	//B			 m 1
	//Y			 n 1
	Y = new double[Dimension];
	B = new double[DataNumTrain];
	XTY = new double[DataNumTrain];
	XTX = new double*[DataNumTrain];	
	XTXInverse = new double*[DataNumTrain];	
	for(int j = 0;j < DataNumTrain; ++j)
	{
		XTX[j] = new double[DataNumTrain];
		XTXInverse[j] = new double[DataNumTrain];
	}		
}
void ReleaseTempSpace(double **p,int n)//�ͷſռ�
{
	for(int i = 0;i < n;++ i)
		delete p[i];
	delete []p;
}
void VarRelease()//�ͷ���ʱ�ռ�
{
	ReleaseTempSpace(XTXInverse, DataNumTrain);
	delete []B;
	delete []Y;
	delete []XTY;
}
void multiplyXY(double **p, double **q, int m, int n, int l, double** ret)
{
	int i, j, k;
	double sum;
	//multiply
	for(i = 0;i < m; ++i)
	{		
		for(j = 0 ; j < l; ++j)
		{
			sum = 0;
			for(k = 0 ;k < n; k ++)
				sum += p[i][k] * q[k][j];

			ret[i][j] = (fabs(sum)<MIN)? 0: sum;
		}	
	}
}

void multiplyXTY(double **p, double **q, int m, int n, int l, double** ret)
{
	int i, j, k;
	double sum;
	//multiply
	for(i = 0;i < m; ++i)
	{		
		for(j = 0 ; j < l; ++j)
		{
			sum = 0;
			for(k = 0 ;k < n; k ++)
				sum += p[k][i] * q[k][j];

			ret[i][j] = (fabs(sum)<MIN)? 0: sum;
		}	
	}
}

void multiplyXYv(double **p, double *q, int m, int n, double* ret)
{
	int i, k;
	double sum;
	//multiply
	for(i = 0;i < m; ++i)
	{		
		sum = 0;
		for(k = 0 ;k < n; k ++)
			sum += p[i][k] * q[k];

		ret[i] = (fabs(sum)<MIN)? 0: sum;		
	}
}

void multiplyXTYv(double **p, double *q, int m, int n, double* ret)
{
	int i, k;
	double sum;
	//multiply
	for(i = 0;i < m; ++i)
	{		
		sum = 0;
		for(k = 0 ;k < n; k ++)
			sum += p[k][i] * q[k];

		ret[i] = (fabs(sum)<MIN)? 0: sum;		
	}
}

void multiplyXXT(double **p, int m, int n, double** ret)
{
	int i, j, k;
	double sum;
	//multiply
	for(i = 0;i < m; ++i)
	{		
		for(j = 0 ; j < m; ++j)
		{
			sum = 0;
			for(k = 0 ;k < n; k ++)
				sum += p[i][k] * p[j][k];

			ret[i][j] = (fabs(sum)<MIN)? 0: sum;
		}	
	}
}

void inverse(double **p, int n, double **q)
{
	int i, j, k, l, ik;
	double max, tmpM;
	double *tmpR;
	double **tmpMat = new double *[n];
	for (i = 0;i < n; ++i)
	{
		tmpMat[i] = new double[n];
		for (j = 0;j < n; ++j)
		{
			q[i][j] = 0;
			tmpMat[i][j] = p[i][j];
			if (i == j)
			{
				tmpMat[i][j] += MIN;
				q[i][j] = 1;
			}
		}
	}

	for(k = 0; k <= n-2; ++ k)
	{
		max = tmpMat[k][k];
		l = k;
		for(ik = k; ik <= n-1; ++ ik)//������Ԫ���Ԫ��
		{
			if(fabs(tmpMat[ik][k]) - fabs(max) > 1e-6)//l�洢���е������Ԫ��������
			{
				l = ik;
				max = tmpMat[l][k];
			}
		}
		if(l != k)//����������ԪԪ��������l������k,����
		{
			tmpR = tmpMat[l];
			tmpMat[l] = tmpMat[k];
			tmpMat[k] = tmpR;//�����д������������������ik����k��,������ָ��
			
			tmpR = q[l];
			q[l] = q[k];
			q[k] = tmpR;//�����Ҷ�����				
		}
		for(i = k+1;i < n; ++ i)
		{
			tmpM = tmpMat[i][k] / tmpMat[k][k];
			for(j = 0; j < n; ++ j)
			{
				if (j >= k)
				{
					tmpMat[i][j] = tmpMat[i][j] - tmpM*tmpMat[k][j];
				}
				q[i][j] -= tmpM*q[k][j];
			}
		}
	}
	//����
	for (i = n-2; i >= 0; -- i)
	{		
		for (j = 0;j <= i; ++ j)
		{			
			tmpM = tmpMat[j][i + 1] / tmpMat[i + 1][i + 1];
			for (int retIndx = 0; retIndx < n; ++ retIndx)
			{
				q[j][retIndx] -= tmpM*q[i + 1][retIndx];
			}
			tmpMat[j][i + 1] = 0;
		}
		for (int j = 0;j < n; ++ j)
		{
			q[i+1][j] /= tmpMat[i+1][i+1];	
		}
		tmpMat[i+1][i+1] = 1;
	}
	//first row ��1
	for (int j = 0;j < n; ++ j)
	{
		q[0][j] /= tmpMat[0][0];	
	}
	tmpMat[0][0] = 1;
}

void dispMat(double **p, int m, int n)
{
	for (int i = 0;i < m; ++i)
	{
		for (int j = 0;j < m; ++j)
		{
			cout << p[i][j] <<" ";
		}
		cout << endl;
	}
	cout << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{

	////////////////
	int i,k;
	double average;
	double atime;
	char TrainFileName[100],TestFileName[100];
	ofstream outfile("Result_Vector.txt",ios::out);
	outfile<<"���������������:"<<endl;
	outfile<<"(1).��ʧ��������,����1:Least-Squares 2:Huber 3:Bisquare"<<endl;
	outfile<<"(2).���㾫��"<<endl;
	outfile<<"(3).һ�������ļ�����ʱ��"<<endl;	
	string s[4]=
	{
		//"Yale_64x64",
		//"ORL_32x32", 
		"Yale_32x32"
	};

	for(int h=0;h<1;h++)
	{
// 		for(select=2; select < 3; select++)
// 		{	
			average=0;
			atime=0;
			for(FileIndx = 1;FileIndx <= 10;++ FileIndx)
			{
				string sTest = "D:\\С����\\2014.6\\��Ŀ\\test\\Result\\";
				string sTrain = "D:\\С����\\2014.6\\��Ŀ\\test\\Result\\";
				sTrain += s[h] + "\\StTrainFile";
				sTest += s[h] + "\\StTestFile";
				strcpy(TrainFileName, sTrain.c_str());
				strcpy(TestFileName, sTest.c_str()); 
				sprintf(TrainFileName + strlen(sTrain.c_str()), "%d.txt",FileIndx);
				sprintf(TestFileName + strlen(sTest.c_str()), "%d.txt",FileIndx);
				filenametrain = TrainFileName;
				filenametest = TestFileName; 
				GetNumAndDataTrain GetTrain;
				GetNumAndDataTest GetTest;
				ReadData(GetTrain, GetTest);
				RightCount = 0;
				begin_time=clock();           //ʱ�����
				for(k = 0;k < DataNumTestTotal;++ k)//��ÿ�����Ե���з���
				{
					dmin = 1e20;//�洢���Ե���ÿ�������С����
					for(i = 0;i < LabelNum; ++i)//����ÿ��������Ե�ľ���
					{	
						DataNumTrain = LabelCount(Label[i]);//����Label[i]���ѵ���������
						VarInit();//�ֲ������ĳ�ʼ��
						GenerateDataForTraining(Label[i]);//���Label[i]���ѵ������		
						//��XTX
						multiplyXXT(Train_X, DataNumTrain, Dimension, XTX);
						inverse(XTX, DataNumTrain, XTXInverse);
						//dispMat(XTXInverse, DataNumTrain, DataNumTrain);
						multiplyXYv(Train_X, Test_X_All[k], DataNumTrain, Dimension, XTY);
						multiplyXYv(XTXInverse, XTY, DataNumTrain, DataNumTrain, B);
						multiplyXTYv(Train_X, B, Dimension, DataNumTrain, Y);

						e = Distance(Y, Test_X_All[k], Dimension);//����e=||y-y'||
						if(e - dmin < MIN)
						{
							Labelmin = Label[i];//��¼����������±�
							dmin = e;
						}
						VarRelease();//�ͷ���ʱ�ռ�
					}//�Ѽ������ò��Ե��������
					if(Labelmin==Test_Y_All[k])
						RightCount++;//������ȷ����Ƶ��
				}//���������ļ��ķ���
				end_time=clock();            //ʱ���յ�
				ptime=(double)(end_time-begin_time)/CLOCKS_PER_SEC;
				RightRate=RightCount/(double)DataNumTestTotal;//��������ļ�������ȷ��
				cout<<"����:"<<RightRate<<endl;	
				GetTrain.ReleaseTrainSpace();//�ͷ�ѵ���ռ�
				GetTest.ReleaseTestSpace();//�ͷŲ��Կռ�
				average+=RightRate;
				atime+=ptime;
			}//end FileIndx
			outfile<<"    "<<average/10<<"    "<<atime/10<<endl;
//		}//end select
	}
	outfile.close();
	return 1;
}//end main


/*

double **p = new double *[3];
for (int i = 0;i < 3; ++i)
{
p[i] = new double[4];
for (int j = 0;j < 4; ++j)
{
p[i][j] = i * j + 1;
}
}
for (int i = 0;i < 3; ++i)
{
for (int j = 0;j < 4; ++j)
{
cout << p[i][j] << " ";
}
cout <<endl;
}

tmp1 = new double *[3];
for (int i = 0;i < 3; ++i)
{
tmp1[i] = new double[3];
}

multiplyXXT(p, 3, 4, tmp1);
for (int i = 0;i < 3; ++i)
{
for (int j = 0;j < 3; ++j)
{
cout << tmp1[i][j] << " ";
}
cout <<endl;
}

tmp2 = new double *[3];
for (int i =0;i < 3; ++i)
{
tmp2[i] = new double[4];
}
multiplyXY(tmp1, p, 3, 3, 4, tmp2);
for (int i = 0;i < 3; ++i)
{
for (int j = 0;j < 4; ++j)
{
cout << tmp2[i][j] << " ";
}
cout <<endl;
}
}*/

/*
double **A;
double **B;
double*d;
double c
double *dAn_;
double *An_dT;
double dAn_dT
d = new double[DataNumTrain];
An_dT = new double[DataNumTrain];
A = new double*[DataNumTrain];
B = new double*[DataNumTrain];
for(int i = 0;i < DataNumTrain; ++i)
{
A[i] = new double[DataNumTrain];
B[i] = new double[DataNumTrain];
}

int m;
int n;

void iTrain(double* x)
{	
multiplyXYv(Train_X, n, Dim, x, d);
multiplyXvYv(x, x, DataNumTrain, c);
//����ʽ
multiplyXvY(d, A, n, n, dAn_);
multiplyXYv(A, d, n, n, An_dT);
multiplyXvYv(d, An_dT, n, dAn_dT);

multiplyXY(dAn_, An_dT, n, 1, n, B);

//complete B
double sum;
int i,j;
for(i = 0;i < n; ++i)
{
sum = 0;
for(j = 0;j < n; ++j)
{
sum += d[j]*A[j][i];
}
B[n][i] = -sum;

sum = 0;
for(j = 0;j < n; ++j)
{
sum += d[j]*A[i][j];
}
B[i][n] = -sum;
}
B[n][n] = 1;

multiplyNX((c - dAn_dT), B);
add(A, B, n+1, n+1, A);
}

void train()
{	
multiplyXTX(Train_X, 1, DataNumTrain, A);
A[0][0] = 1/A[0][0];
m = DataNumTrain;
n = 1;	
for(int i = 0;i < DataNumTrain; ++i)
{
iTrain(Train_X[i]);
}	
}

*/