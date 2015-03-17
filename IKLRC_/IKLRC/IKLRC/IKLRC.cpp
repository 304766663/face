// IKLRC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<stdio.h>
#include<iostream>
#include<math.h>
#include<string>
#include<ctime>
#include<fstream>
#define Dimension 64*64//32*32  //����ά��(������y)
#define MIN 1.0e-06
using namespace std;

enum TestType{
	Normal, 
	Increase
};

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
double *Beta;
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

//����ѧϰ
double **B;
double*d;
double c;
double *dAn_;
double *An_dT;
double dAn_dT;

TestType testType = Increase;

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
	Y = new double[Dimension];
	Beta = new double[DataNumTrain];
	XTY = new double[DataNumTrain];

	int i, j;
	switch(testType)
	{
	case Normal:
		{
			//XTXInverse m m
			//XTY        m 1
			//B			 m 1
			//Y			 n 1			
			XTX = new double*[DataNumTrain];	
			XTXInverse = new double*[DataNumTrain];	
			for(i = 0;i < DataNumTrain; ++i)
			{
				XTX[i] = new double[DataNumTrain];
				XTXInverse[i] = new double[DataNumTrain];
			}
			break;
		}
	case Increase:
		{
			d = new double[DataNumTrain];
			dAn_ = new double[DataNumTrain];
			An_dT = new double[DataNumTrain];			
			
			B = new double*[DataNumTrain];
			XTXInverse = new double*[DataNumTrain];			
			for(i = 0;i < DataNumTrain; ++i)
			{
				d[i] = 0;
				dAn_[i] = 0;
				An_dT[i] = 0;

				B[i] = new double[DataNumTrain];
				XTXInverse[i] = new double[DataNumTrain];				
				for (j = 0; j < DataNumTrain; ++j)
				{
					XTXInverse[i][j] = 0;
					B[i][j] = 0;
				}
			}
			break;
		}
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
	delete []Beta;
	delete []Y;
	delete []XTY;
	ReleaseTempSpace(XTXInverse, DataNumTrain);
	switch(testType)
	{
	case Normal:
		{	
			ReleaseTempSpace(XTX, DataNumTrain);
			break;
		}
	case Increase:
		{
			//delete dAn_dT;
			delete []d;
			delete []dAn_;
			delete []An_dT;			
			ReleaseTempSpace(B, DataNumTrain);
			break;
		}
	}
}

void addXmYm(double **p, double **q, int m, int n, double **ret)
{
	int i, j;
	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < m; ++j)
		{
			ret [i][j] = p[i][j] + q[i][j];
		}
	}
}

void multiplyXmYm(double **p, double **q, int m, int n, int l, double **ret)
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
			{
				sum += p[i][k] * q[k][j];
			}

			ret[i][j] = (fabs(sum)<MIN)? 0: sum;
		}	
	}
}

void multiplyXtYm(double **p, double **q, int m, int n, int l, double **ret)
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

void multiplyXvYm(double *p, double **q, int m, int n, double *ret)//ym yn
{
	int i, j;
	//multiply
	for(i = 0;i < n; ++i)
	{		
		ret[i] = 0;
		for(j = 0 ; j < m; ++j)
		{
			ret[i] += p[j]*q[j][i];
		}	
		if (fabs(ret[i]) < MIN)
		{
			ret[i] = 0;
		}
	}
}

void multiplyXmYv(double **p, double *q, int m, int n, double *ret)
{
	int i, j;
	//multiply
	for(i = 0;i < m; ++i)
	{
		ret[i] = 0;
		for(j = 0 ; j < n; ++j)
		{
			ret[i] += p[i][j]*q[j];
		}	
		if (fabs(ret[i]) < MIN)
		{
			ret[i] = 0;
		}
	}
}

void multiplyXtYv(double **p, double *q, int m, int n, double *ret)
{
	int i, j;
	//multiply
	for(i = 0;i < m; ++i)
	{
		ret[i] = 0;
		for(j = 0 ; j < n; ++j)
		{
			ret[i] += p[j][i]*q[j];
		}	
		if (fabs(ret[i]) < MIN)
		{
			ret[i] = 0;
		}
	}
}

void multiplyXmXt(double **p, int m, int n, double **ret)
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
			{
				sum += p[i][k] * p[j][k];
			}

			ret[i][j] = (fabs(sum)<MIN)? 0: sum;
		}	
	}
}

void multiplyXvYv(double *p, double *q, int m, double& ret)
{
	int i;
	ret = 0;
	for(i = 0; i < m; ++i)
	{
		ret += p[i]*q[i];
	}
	if (fabs(ret) < MIN)
	{
		ret = 0;
	}
}

void multiplyXvYv(double *p, double *q, int m, double **ret)
{
	int i, j;
	//multiply
	for(i = 0; i < m; ++i)
	{		
		for(j = 0; j < m; ++j)
		{
			ret[i][j] = p[i]*q[j];
			if (fabs(ret[i][j]) < MIN)
			{
				ret[i][j] = 0;
			}
		}	
	}
}

void multiplyNX(double c, double **p, int m, int n, double **ret)
{
	int i, j;
	for (i = 0; i < m; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			ret[i][j] = c*p[i][j];
			if (fabs(ret[i][j]) < MIN)
			{
				ret[i][j] = 0;
			}
		}
	}
}

void inverse_Normal(double **p, int n, double **q)
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

void train_Increase(double* x, int n)
{	
	multiplyXmYv(Train_X, x, n, Dimension, d);//d
	multiplyXvYv(x, x, Dimension, c);	//c
	c += MIN;
	//����ʽ
	multiplyXvYm(d, XTXInverse, n, n, dAn_);	//dAn_
	multiplyXmYv(XTXInverse, d, n, n, An_dT);//An_dT
	multiplyXvYv(d, An_dT, n, dAn_dT);		//dAn_dT

	multiplyXvYv(dAn_, An_dT, n, B);	//init B

	//complete B
	double sum;
	int i,j;
	for(i = 0;i < n; ++i)
	{
		sum = 0;
		for(j = 0;j < n; ++j)
		{
			sum += d[j]*XTXInverse[j][i];
		}
		B[n][i] = -sum;

		sum = 0;
		for(j = 0;j < n; ++j)
		{
			sum += d[j]*XTXInverse[i][j];
		}
		B[i][n] = -sum;
	}
	B[n][n] = 1;

	multiplyNX((c - dAn_dT), B, n, n, B);
	addXmYm(XTXInverse, B, n+1, n+1, XTXInverse);
}

void inverse_Increase()
{	
	multiplyXmXt(Train_X, 1, Dimension, XTXInverse);
	dispMat(XTXInverse, DataNumTrain, DataNumTrain);
	XTXInverse[0][0] = 1/(XTXInverse[0][0] + MIN);
	for(int i = 0;i < DataNumTrain - 1; ++i)
	{
		train_Increase(Train_X[i], i + 1);
	}	
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
				string sTest = "Result\\";
				string sTrain = "Result\\";
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
						DataNumTrain = LabelCount((int)Label[i]);//����Label[i]���ѵ���������
						VarInit();//�ֲ������ĳ�ʼ��
						GenerateDataForTraining((int)Label[i]);//���Label[i]���ѵ������		
						//��XTX
						if (testType == Normal)
						{
							multiplyXmXt(Train_X, DataNumTrain, Dimension, XTX);
							inverse_Normal(XTX, DataNumTrain, XTXInverse);
						}
						else
						{
							inverse_Increase();
						}						
						//dispMat(XTXInverse, DataNumTrain, DataNumTrain);
						multiplyXmYv(Train_X, Test_X_All[k], DataNumTrain, Dimension, XTY);
						multiplyXmYv(XTXInverse, XTY, DataNumTrain, DataNumTrain, Beta);
						multiplyXtYv(Train_X, Beta, Dimension, DataNumTrain, Y);

						e = Distance(Y, Test_X_All[k], Dimension);//����e=||y-y'||
						if(e - dmin < MIN)
						{
							Labelmin = (int)Label[i];//��¼����������±�
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
			cout << "ƽ������ :" << average/10 << endl;
//		}//end select
	}
	outfile.close();
	return 1;
}//end main

