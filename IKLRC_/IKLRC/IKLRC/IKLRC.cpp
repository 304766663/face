// IKLRC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<stdio.h>
#include<iostream>
#include<math.h>
#include<string>
#include<ctime>
#include<fstream>
#define Dimension 64*64//32*32  //数据维数(不包括y)
#define MIN 1.0e-06
using namespace std;

enum TestType{
	Normal, 
	Increase
};

string filenametrain="Train.txt"; //训练点数据文件名
string filenametest="Test.txt";//测试点数据文件名

int DataNumTrainTotal;//训练样本总数
int DataNumTestTotal;//测试样本总数
int DataNumTrain; 
int ReadNum;
int FileIndx;
//double r;//胡贝尔参数
double **Train_X_All;  //训练样本点
double *Train_Y_All;   //训练点
double **Test_X_All;     //测试样本点
double *Test_Y_All;      //测试样本点
double **Train_X;
double *Label;        //样本对应标号集
//double *W;
double *Beta;
//double *B1;
int LabelNum;
double RightCount;//正确分类测试点个数
double RightRate;//分类正确率
double dmin;//最小距离
int Labelmin;//存储与测试点最近的类的标注
double e;//迭代算法终止误差
// double temp;//中间变量
// double **tmp1;//中间指针
// double **tmp2;
double **XTX;
double **XTXInverse;
double *XTY;
double *Y;
double end_time;
double begin_time;
double ptime;

//增量学习
double **B;
double*d;
double c;
double *dAn_;
double *An_dT;
double dAn_dT;

TestType testType = Increase;

////////////////////////////////////////////////////////////////////////////////////
//用于读取训练点数据类
////////////////////////////////////////////////////////////////////////////////////
class GetNumAndDataTrain
{	
public:
	GetNumAndDataTrain();
	void initial();
	void GetData();
	void ReleaseTrainSpace();
};

//判断是否能正常打开文件
GetNumAndDataTrain::GetNumAndDataTrain()//构造函数
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

void GetNumAndDataTrain::initial()//获得数据点个数
{	
	cout<<"数据文件扫描中,获得数据点个数,请稍后..."<<endl<<endl;

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
	cout<<"训练点共有数据:"<<DataNumTrainTotal<<"个;"<<endl<<endl;
}
//读取数据点
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

//-----------------------------读取测试数据集---------------------------
class GetNumAndDataTest
{	
public:
	GetNumAndDataTest();
	void initial();
	void GetData();
	void ReleaseTestSpace();
};

GetNumAndDataTest::GetNumAndDataTest()//构造函数
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

void GetNumAndDataTest::initial()//获得数据点个数
{
	cout<<"数据文件扫描中,获得数据点个数,请稍后..."<<endl<<endl;

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
	cout<<"测试点共有数据:"<<DataNumTestTotal<<"个;"<<endl<<endl;
}
//读取数据点
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



void GetLabelOrdered()//插入排序算法,计算出训练文件有多少类别
{
	int i,j;
	LabelNum=0;
	int decide=0;
	double *remember;
	remember=new double[DataNumTrainTotal];            //记下不同标注
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
	GetTrain.initial();         //获得训练数据集中数据总个数
	GetTrain.GetData();         //读入数据
	GetTest.initial();         //获得测试数据集中数据总个数
	GetTest.GetData();         //读入数据
	GetLabelOrdered();			//获得训练集的类个数以及类标注
}
//计算类标注为n的类有多少个训练对象
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
//计算两点欧氏距离
double Distance(double *u,double *v,int n)
{
	int i;
	double sum=0;
	for(i=0;i<n;i++)
		sum+=(u[i]-v[i])*(u[i]-v[i]);
	return sum;
}

//解方程组的列主元Gauss消去法
void Gauss(double **p,double *q,int n)
{
	int i,j,k,l,ik;
	double max,temp;
	double *tmp;
	for(k=0;k<=n-2;k++)
	{
		max=p[k][k];
		l=k;
		for(ik=k;ik<=n-1;ik++)//找列主元最大元素
		{
			if(fabs(p[ik][k])>fabs(max))//l存储该列的最大主元素所在行
			{
				l=ik;
				max=p[l][k];
			}
		}
		if(l!=k)//如果最大列主元元素所在行l不等于k,则换行
		{
			tmp=p[l];
			p[l]=p[k];
			p[k]=tmp;//这三行代码是用来交换矩阵的ik行与k行,即交换指针
			temp=q[l];
			q[l]=q[k];
			q[k]=temp;//交换右端向量
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
void ReleaseTempSpace(double **p,int n)//释放空间
{
	for(int i = 0;i < n;++ i)
		delete p[i];
	delete []p;
}
void VarRelease()//释放临时空间
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
		for(ik = k; ik <= n-1; ++ ik)//找列主元最大元素
		{
			if(fabs(tmpMat[ik][k]) - fabs(max) > 1e-6)//l存储该列的最大主元素所在行
			{
				l = ik;
				max = tmpMat[l][k];
			}
		}
		if(l != k)//如果最大列主元元素所在行l不等于k,则换行
		{
			tmpR = tmpMat[l];
			tmpMat[l] = tmpMat[k];
			tmpMat[k] = tmpR;//这三行代码是用来交换矩阵的ik行与k行,即交换指针
			
			tmpR = q[l];
			q[l] = q[k];
			q[k] = tmpR;//交换右端向量				
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
	//回溯
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
	//first row 归1
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
	//代公式
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
	outfile<<"输出包括如下内容:"<<endl;
	outfile<<"(1).损失函数类型,其中1:Least-Squares 2:Huber 3:Bisquare"<<endl;
	outfile<<"(2).计算精度"<<endl;
	outfile<<"(3).一个完整文件计算时间"<<endl;	
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
				begin_time=clock();           //时间起点
				for(k = 0;k < DataNumTestTotal;++ k)//对每个测试点进行分类
				{
					dmin = 1e20;//存储测试点与每个类的最小距离
					for(i = 0;i < LabelNum; ++i)//计算每个类与测试点的距离
					{	
						DataNumTrain = LabelCount((int)Label[i]);//计算Label[i]类的训练对象个数
						VarInit();//局部变量的初始化
						GenerateDataForTraining((int)Label[i]);//获得Label[i]类的训练对象		
						//求XTX
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

						e = Distance(Y, Test_X_All[k], Dimension);//计算e=||y-y'||
						if(e - dmin < MIN)
						{
							Labelmin = (int)Label[i];//记录最近距离类下标
							dmin = e;
						}
						VarRelease();//释放临时空间
					}//已计算出与该测试点最近的类
					if(Labelmin==Test_Y_All[k])
						RightCount++;//计算正确分类频数
				}//结束测试文件的分类
				end_time=clock();            //时间终点
				ptime=(double)(end_time-begin_time)/CLOCKS_PER_SEC;
				RightRate=RightCount/(double)DataNumTestTotal;//计算测试文件分类正确率
				cout<<"精度:"<<RightRate<<endl;	
				GetTrain.ReleaseTrainSpace();//释放训练空间
				GetTest.ReleaseTestSpace();//释放测试空间
				average+=RightRate;
				atime+=ptime;
			}//end FileIndx
			outfile<<"    "<<average/10<<"    "<<atime/10<<endl;
			cout << "平均精度 :" << average/10 << endl;
//		}//end select
	}
	outfile.close();
	return 1;
}//end main

