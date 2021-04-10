// StereoMatch_RankTransform.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include <math.h>
#include "time.h"

#include <iostream>
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	//Time Start
	clock_t start,finish;
	unsigned long seconds;
	start=clock();
	
	cout<<"--------StereoMatch_RankTransform--------"<<endl;
	
	//--------------------------Set Search Parameters--------------------------
	int windowSize =21;
	int DSR = 20;
	int K_d=16;

	//input file name : tsukuba teddy cones venus;
	char * filenameL ="tsukubaL.png" ;
	char * filenameR ="tsukubaR.png" ;

	//--------------------------Read in Grey image --------------------------
	IplImage *imageL_Grey = cvLoadImage(filenameL,0);
	IplImage *imageR_Grey = cvLoadImage(filenameR,0);

	//--------------------------Read in Color image --------------------------
	IplImage *imageL_Color = cvLoadImage(filenameL,1);
	IplImage *imageR_Color = cvLoadImage(filenameR,1);

	if (imageL_Color==NULL){cout << "No valid image input."<<endl; return 1;}  
	if (imageR_Color==NULL){cout << "No valid image input."<<endl; return 1;}  

	int width = imageL_Color->width;
	int height = imageL_Color->height;
	cout<<"Read in Image...."<<endl;
	cout<<"Image Size: "<<width<<" X "<<height<<endl;
	//--------------------------Create Disparity Map--------------------------
	IplImage *DisparityMap = cvCreateImage(cvSize(width,height),8,1);
	for (int j = 0 ; j< height ; j++)
	{
		uchar *pDisparity = (uchar*)DisparityMap->imageData + DisparityMap->widthStep * j;
		for(int i = 0 ; i < width ; i++)
		{
			pDisparity[i] = 0;
		}
	}
	//--------------------------MinCost Map--------------------------
	IplImage *MinCostMap = cvCreateImage(cvSize(width,height),8,1);
	for (int j = 0 ; j< height ; j++)
	{
		uchar *pMinCostMap0 = (uchar*)MinCostMap->imageData + MinCostMap->widthStep * j;
		for(int i = 0 ; i < width ; i++)
		{
			pMinCostMap0[i] = 0;
		}
	}


	//--------------------------Obtain Disparity Map--------------------------

	cout<<"Obtain Disparity Map:";
	int CoutMeasure = 1;
	CvMat* LeftRank = cvCreateMat(windowSize,windowSize,CV_32SC1);
	CvMat* RightRank = cvCreateMat(windowSize,windowSize,CV_32SC1);
	for (int j = windowSize/2 ; j< height-windowSize/2 ; j++)
	{
		uchar *pDisparity = (uchar*)DisparityMap->imageData + DisparityMap->widthStep * j;

		uchar *pMinCostMap = (uchar*)MinCostMap->imageData + MinCostMap->widthStep * j;

		for(int i = windowSize/2 ; i < width-windowSize/2 ; i++)
		{
			int SearchStart = i-DSR;
			int SearchEnd = i;

			//=================compute the cost of current pixel's support window==============
			//Rank Transform
			if (1 == CoutMeasure)
			{
				cout<<"by Rank Transform"<<endl;
				CoutMeasure--;
			}
			double MaxErr = 0;

			for (int k = SearchStart ; k <= SearchEnd ; k++)
			{
				double SumErr = 0;
				double SumWight = 0;

				uchar *PixelGrey_LCenter=(uchar*)imageL_Grey->imageData + imageL_Grey->widthStep * j + i;
				uchar *PixelGrey_RCenter=(uchar*)imageR_Grey->imageData + imageR_Grey->widthStep * j + k;

				//Compute LeftRank(i,j) & Right(k,j) Transform Matrix
				for ( int RankY = -windowSize/2 ; RankY <= windowSize/2 ; RankY++ )
				{
					int* pLeftRank = (int*)(LeftRank->data.ptr + LeftRank->step * (RankY+windowSize/2));
					int* pRightRank = (int*)(RightRank->data.ptr + RightRank->step * (RankY+windowSize/2));

					uchar* PixelGrey_L = (uchar*)imageL_Grey->imageData + imageL_Grey->widthStep*(j+RankY);
					uchar* PixelGrey_R = (uchar*)imageR_Grey->imageData + imageR_Grey->widthStep*(j+RankY);

					for ( int RankX = -windowSize/2 ; RankX <= windowSize/2 ; RankX++ )
					{
						int DiffThreshold_v = 9;
						int DiffThreshold_u = 2;

						//Compute LeftRank
						int LeftDiff = PixelGrey_L[i+RankX]-PixelGrey_LCenter[0];
						//cout<<"<"<<LeftDiff<<",";
						if (LeftDiff < -DiffThreshold_v)
						{
							pLeftRank[RankX+windowSize/2]= -2;
						}
						else if ( (LeftDiff >= -DiffThreshold_v)&&(LeftDiff < -DiffThreshold_u) )
						{
							pLeftRank[RankX+windowSize/2]= -1;
						}
						else if ( (LeftDiff >= -DiffThreshold_u)&&(LeftDiff <= DiffThreshold_u) )
						{
							pLeftRank[RankX+windowSize/2]= 0;
						}
						else if ( (LeftDiff > DiffThreshold_u)&&(LeftDiff <= DiffThreshold_v) )
						{
							pLeftRank[RankX+windowSize/2]= 1;
						}
						else if(LeftDiff > DiffThreshold_v)
						{
							pLeftRank[RankX+windowSize/2]= 2;
						}

						//Compute RightRank
						int RightDiff = PixelGrey_R[k+RankX]-PixelGrey_RCenter[0];
						//cout<<RightDiff<<">"<<"  ";

						if (RightDiff < -DiffThreshold_v)
						{
							pRightRank[RankX+windowSize/2]= -2;
						}
						else if ( (RightDiff >= -DiffThreshold_v)&&(RightDiff < -DiffThreshold_u) )
						{
							pRightRank[RankX+windowSize/2]= -1;
						}
						else if ( (RightDiff >= -DiffThreshold_u)&&(RightDiff <= DiffThreshold_u) )
						{
							pRightRank[RankX+windowSize/2]= 0;
						}
						else if ( (RightDiff > DiffThreshold_u )&&(RightDiff <= DiffThreshold_v) )
						{
							pRightRank[RankX+windowSize/2]= 1;
						}
						else if ( RightDiff > DiffThreshold_v )
						{
							pRightRank[RankX+windowSize/2]= 2;
						}

					}
					//cout<<endl;
				}//LeftRank & Right Transform Finish
				//cout<<"-----------"<<endl;
				//Obtain F Matrix
				//Compute the Match Cost
				for(int y = -windowSize/2 ; y <= windowSize/2 ; y++)
				{
					int* pLeftRank = (int*)(LeftRank->data.ptr + LeftRank->step * (y+windowSize/2));
					int* pRightRank = (int*)(RightRank->data.ptr + RightRank->step * (y+windowSize/2));

					for(int x = -windowSize/2 ; x<= windowSize/2 ; x++)
					{
						int Err = (pLeftRank[windowSize/2+x] == pRightRank[windowSize/2+x]) ;
						//cout<<Err<<"   ";
						//Cost accumulation
						SumErr += Err;
					}
					//cout<<endl;
				}//the cost of current window
				double  CurrErr = SumErr/(windowSize*windowSize);
				//WTA choice the Min mean Cost , Save the d;
				if (CurrErr >= MaxErr)
				{
					MaxErr = CurrErr;
					pDisparity[i] = abs(i-k) * K_d ;
					pMinCostMap[i] = MaxErr*100;
				}
			}//pixel(i,j),Rank transform finish.//d
			//===========Rank Transform End!
		}//i
		if (j%10==0)
		{
			cout<<".";
		}
	}//j

	//--------------------------Save and Show the Initial Disparity Map--------------------------
	cout<<endl<<"Save and Show the initial Disparity Map"<<endl;
	cvNamedWindow("DisparityMap" , CV_WINDOW_AUTOSIZE);
	cvShowImage("DisparityMap", DisparityMap);
	cvSaveImage("DisparityMap.png",DisparityMap);
	;;
	cvNamedWindow("MinCostMap" , CV_WINDOW_AUTOSIZE);
	cvShowImage("MinCostMap", MinCostMap);
	cvSaveImage("MinCostMap.png",MinCostMap);

	//------------------Median Filtering------------------
	;;
	int FiltWindow = 3;
	int FiltRound = 1; 
	IplImage *DisparityMapFilted = cvCreateImage(cvSize(width,height),8,1);
	cout<<">>Median Filtering:"<<endl<<"  ......"<<endl;
	IplImage *Temp = cvCreateImage(cvSize(DisparityMapFilted->width,DisparityMapFilted->height),8,1); //如果多次滤波则Temp是必要的
	cvCopy(DisparityMap, Temp, NULL);	
	for (int k = 0 ; k<FiltRound ;k++)
	{
		cvSmooth(Temp,DisparityMapFilted,CV_MEDIAN,FiltWindow);
		cvCopy(DisparityMapFilted,Temp,NULL);
	}
	cvReleaseImage(&Temp);
	cvSaveImage("DisparityMapFilted.png",DisparityMapFilted);
	cvNamedWindow("DisparityMapFilted" , CV_WINDOW_AUTOSIZE);
	cvShowImage("DisparityMapFilted", DisparityMapFilted);

	//------------------End------------------
	cout<<"End!"<<endl;
	cvWaitKey(0);

	cvDestroyAllWindows();
	cvReleaseImage(&DisparityMapFilted);
	cvReleaseImage(&DisparityMap);

	cvReleaseMat(&LeftRank);
	cvReleaseMat(&RightRank);
	cvReleaseImage(&imageL_Color);
	cvReleaseImage(&imageR_Color);
	cvReleaseImage(&imageL_Grey);
	cvReleaseImage(&imageR_Grey);

	cvReleaseImage(&DisparityMap);
	
	return 0;
}