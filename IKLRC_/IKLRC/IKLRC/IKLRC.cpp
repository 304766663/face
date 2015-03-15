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
int select;
int FileIndx;
double r;//����������
double **Train_X_All;  //ѵ��������
double *Train_Y_All;   //ѵ����
double **Test_X_All;     //����������
double *Test_Y_All;      //����������
double **Train_X;
double *Label;        //������Ӧ��ż�
double *W;
double *B;
double *B1;
int LabelNum;
double RightCount;//��ȷ������Ե����
double RightRate;//������ȷ��
double dmin;//��С����
int Labelmin;//�洢����Ե��������ı�ע
double e;//�����㷨��ֹ���
double temp;//�м����
double **tmp1;//�м�ָ��
double **tmp2;
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

void ReleaseTempSpace(double **p,int n)//�ͷſռ�
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
//��������ŷ�Ͼ���
double Distance(double *u,double *v,int n)
{
	int i;
	double sum=0;
	for(i=0;i<n;i++)
		sum+=(u[i]-v[i])*(u[i]-v[i]);
	return sum;
}
void ReleaseSpace()//�ͷ���ʱ�ռ�
{
	ReleaseTempSpace(tmp1,DataNumTrain);
	ReleaseTempSpace(tmp2,DataNumTrain);
	delete []B1;
	delete []B;
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
		for(select=2; select < 3; select++)
		{	
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
				ReadData(GetTrain,GetTest);
				RightCount = 0;
				W = new double[Dimension];
				begin_time=clock();           //ʱ�����
				for(k=0;k<DataNumTestTotal;k++)//��ÿ�����Ե���з���
				{
					dmin=6000000;//�洢���Ե���ÿ�������С����
					for(i=0;i<LabelNum;i++)//����ÿ��������Ե�ľ���
					{	
						DataNumTrain=LabelCount(Label[i]);//����Label[i]���ѵ���������
						VariableInit();//�ֲ������ĳ�ʼ��
						GenerateDataForTraining(Label[i]);//���Label[i]���ѵ������		
						//�õ�����Ȩ��С���˷����Bi
						m=0;
						while(1)
						{
							m++;
							for(j=0;j<Dimension;j++)//�����W����
							{
								temp=0;
								for(l=0;l<DataNumTrain;l++)
								{
									temp+=B[l]*Train_X[l][j];
								}
								temp=Test_X_All[k][j]-temp;//����в�
								W[j]=w(temp);//����Ȩ����
							}
							for(j=0;j<DataNumTrain;j++)//����tmp1=X'W
								for(l=0;l<Dimension;l++)
								{
									tmp1[j][l]=Train_X[j][l]*W[l];
								}
								for(j=0;j<DataNumTrain;j++)//����tmp2=tmp1*X=X'WX
								{
									for(l=0;l<DataNumTrain;l++)
									{
										temp=0;
										for(t=0;t<Dimension;t++)
											temp+=tmp1[j][t]*Train_X[l][t];
										tmp2[j][l]=temp;
									}
								}
								for(j=0;j<DataNumTrain;j++)//����B1=X'Wy
								{
									temp=0;
									for(l=0;l<Dimension;l++)
										temp+=tmp1[j][l]*Test_X_All[k][l];
									B1[j]=temp;
								}
								Gauss(tmp2,B1,DataNumTrain);
								e=Distance(B,B1,DataNumTrain);//�������ε���������
								for(j=0;j<DataNumTrain;j++)
									B[j]=B1[j];
								if(e<MIN)
									break;
						}//end while
						for(j=0;j<Dimension;j++)//����y'=XB'����W��
						{
							temp=0;
							for(l=0;l<DataNumTrain;l++)
								temp+=Train_X[l][j]*B[l];
							W[j]=temp;
						}
						e=Distance(W,Test_X_All[k],Dimension);//����e=||y-y'||
						if(e<dmin)
						{
							Labelmin=Label[i];//��¼����������±�
							dmin=e;
						}
						ReleaseSpace();//�ͷ���ʱ�ռ�
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
			}//end CrossValidationTime
			outfile<<"    "<<select<<"    "<<average/10<<"    "<<atime/10<<endl;
		}//end select
	}
	outfile.close();
	return 1;
}//end main
