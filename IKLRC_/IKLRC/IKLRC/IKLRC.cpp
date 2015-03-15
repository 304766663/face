// IKLRC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<stdio.h>
#include<iostream>
#include<math.h>
#include<string>
#include<ctime>
#include<fstream>
#define Dimension 32*32  //数据维数(不包括y)
#define MIN 1.0e-06
using namespace std;

string filenametrain="Train.txt"; //训练点数据文件名
string filenametest="Test.txt";//测试点数据文件名

int DataNumTrainTotal;//训练样本总数
int DataNumTestTotal;//测试样本总数
int DataNumTrain; 
int ReadNum;
int select;
int FileIndx;
double r;//胡贝尔参数
double **Train_X_All;  //训练样本点
double *Train_Y_All;   //训练点
double **Test_X_All;     //测试样本点
double *Test_Y_All;      //测试样本点
double **Train_X;
double *Label;        //样本对应标号集
double *W;
double *B;
double *B1;
int LabelNum;
double RightCount;//正确分类测试点个数
double RightRate;//分类正确率
double dmin;//最小距离
int Labelmin;//存储与测试点最近的类的标注
double e;//迭代算法终止误差
double temp;//中间变量
double **tmp1;//中间指针
double **tmp2;
double end_time;
double begin_time;
double ptime;

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

void ReleaseTempSpace(double **p,int n)//释放空间
{
	for(int i=0;i<n;i++)
		delete p[i];
	delete []p;
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
void ReleaseSpace()//释放临时空间
{
	ReleaseTempSpace(tmp1,DataNumTrain);
	ReleaseTempSpace(tmp2,DataNumTrain);
	delete []B1;
	delete []B;
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
void VariableInit()
{
	int j;
	B=new double[DataNumTrain];
	B1=new double[DataNumTrain];
	tmp1=new double*[DataNumTrain];			
	tmp2=new double*[DataNumTrain];	
	for(j=0;j<DataNumTrain;j++)
	{
		tmp1[j]=new double[Dimension];
		tmp2[j]=new double[DataNumTrain];
		B[j]=1;
	}		
}
int _tmain(int argc, _TCHAR* argv[])
{
	int i,j,k,l,m,t;
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
		for(select=2; select < 3; select++)
		{	
			average=0;
			atime=0;
			for(FileIndx = 1;FileIndx <= 10;++ FileIndx)
			{
				string sTest = "D:\\小桌面\\2014.6\\项目\\test\\Result\\";
				string sTrain = "D:\\小桌面\\2014.6\\项目\\test\\Result\\";
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
				ReadData(GetTrain,GetTest);
				RightCount = 0;
				W = new double[Dimension];
				begin_time=clock();           //时间起点
				for(k=0;k<DataNumTestTotal;k++)//对每个测试点进行分类
				{
					dmin=6000000;//存储测试点与每个类的最小距离
					for(i=0;i<LabelNum;i++)//计算每个类与测试点的距离
					{	
						DataNumTrain=LabelCount(Label[i]);//计算Label[i]类的训练对象个数
						VariableInit();//局部变量的初始化
						GenerateDataForTraining(Label[i]);//获得Label[i]类的训练对象		
						//用迭代加权最小二乘法求解Bi
						m=0;
						while(1)
						{
							m++;
							for(j=0;j<Dimension;j++)//计算出W矩阵
							{
								temp=0;
								for(l=0;l<DataNumTrain;l++)
								{
									temp+=B[l]*Train_X[l][j];
								}
								temp=Test_X_All[k][j]-temp;//计算残差
								W[j]=w(temp);//计算权函数
							}
							for(j=0;j<DataNumTrain;j++)//计算tmp1=X'W
								for(l=0;l<Dimension;l++)
								{
									tmp1[j][l]=Train_X[j][l]*W[l];
								}
								for(j=0;j<DataNumTrain;j++)//计算tmp2=tmp1*X=X'WX
								{
									for(l=0;l<DataNumTrain;l++)
									{
										temp=0;
										for(t=0;t<Dimension;t++)
											temp+=tmp1[j][t]*Train_X[l][t];
										tmp2[j][l]=temp;
									}
								}
								for(j=0;j<DataNumTrain;j++)//计算B1=X'Wy
								{
									temp=0;
									for(l=0;l<Dimension;l++)
										temp+=tmp1[j][l]*Test_X_All[k][l];
									B1[j]=temp;
								}
								Gauss(tmp2,B1,DataNumTrain);
								e=Distance(B,B1,DataNumTrain);//计算两次迭代结果误差
								for(j=0;j<DataNumTrain;j++)
									B[j]=B1[j];
								if(e<MIN)
									break;
						}//end while
						for(j=0;j<Dimension;j++)//计算y'=XB'存在W中
						{
							temp=0;
							for(l=0;l<DataNumTrain;l++)
								temp+=Train_X[l][j]*B[l];
							W[j]=temp;
						}
						e=Distance(W,Test_X_All[k],Dimension);//计算e=||y-y'||
						if(e<dmin)
						{
							Labelmin=Label[i];//记录最近距离类下标
							dmin=e;
						}
						ReleaseSpace();//释放临时空间
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
			}//end CrossValidationTime
			outfile<<"    "<<select<<"    "<<average/10<<"    "<<atime/10<<endl;
		}//end select
	}
	outfile.close();
	return 1;
}//end main
