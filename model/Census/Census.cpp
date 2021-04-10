// Census.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

#include "cv.h"
#include "highgui.h"

// #define IMGHEIGHT 240
// #define IMGWIDTH  320

int IMGHEIGHT;
int IMGWIDTH;

#define DOMAIN    16
#define WndWidth  9
#define CODEWIDTH 80

typedef struct
{
	int codebit[80];
}CodeStruct;

void SAD(unsigned char *LeftImg, unsigned char *RightImg, unsigned char *Disparity);
void SSD(unsigned char *LeftImg, unsigned char *RightImg, unsigned char *Disparity);
void NCC(unsigned char *LeftImg, unsigned char *RightImg, unsigned char *Disparity);
void NSSD(unsigned char *LeftImg, unsigned char *RightImg, unsigned char *Disparity);
void Census(unsigned char *LeftImg, unsigned char *RightImg, unsigned char *Disparity);
void Rank(unsigned char *LeftImg, unsigned char *RightImg, unsigned char *Disparity);
void CensusTransform(unsigned char *Image, CodeStruct Code[]);
int HammingDist(CodeStruct Code1, CodeStruct Code2);
void RankTransform(unsigned char *Image, unsigned char *RankValue);


void SAD(unsigned char *LeftImg, unsigned char *RightImg, unsigned char *Disparity)
{
	int i, j, d;
	int absDiff;
	int minSum;
	int disp;
	int *Col;
	int *Wnd;

	Col = new int[IMGWIDTH*DOMAIN];
	Wnd = new int[IMGWIDTH*DOMAIN];

	for (i = 0; i<IMGWIDTH*DOMAIN; i++)
	{
		Col[i] = 0;
		Wnd[i] = 0;
	}

	for (j = 0; j<IMGHEIGHT; j++)
	{
		for (i = 0; i<IMGWIDTH; i++)
		{
			minSum = 100000;
			disp = 0;
			for (d = 0; d<DOMAIN; d++)
			{
				if (i >= DOMAIN)
					absDiff = abs(LeftImg[j*IMGWIDTH + i] - RightImg[j*IMGWIDTH + i - d]);
				else
					absDiff = 0;

				Col[i*DOMAIN + d] = Col[i*DOMAIN + d] + absDiff;

				if (j>WndWidth)
				{
					if (i>DOMAIN)
						absDiff = abs(LeftImg[(j - WndWidth)*IMGWIDTH + i] - RightImg[(j - WndWidth)*IMGWIDTH + i - d]);
					else
						absDiff = 0;
					Col[i*DOMAIN + d] = Col[i*DOMAIN + d] - absDiff;
				}

				if (i<DOMAIN || j<WndWidth)
					Wnd[i*DOMAIN + d] = 0;
				else
					Wnd[i*DOMAIN + d] = Wnd[(i - 1)*DOMAIN + d] + Col[i*DOMAIN + d]
					- Col[(i - WndWidth)*DOMAIN + d];

				/*
				if( i>WndWidth )
				Wnd[i*DOMAIN+d]=Wnd[(i-1)*DOMAIN+d]+Col[i*DOMAIN+d]-Col[(i-WndWidth)*DOMAIN+d];
				else
				{
				if( i>d )
				absDiff=abs(LeftImg[j*IMGWIDTH+i]-RightImg[j*IMGWIDTH+i-d]);
				else
				absDiff=0;

				Wnd[i*DOMAIN+d]=Wnd[i*DOMAIN+d]+absDiff;
				if( j>WndWidth )
				{
				if( i>d )
				absDiff=abs(LeftImg[(j-WndWidth)*IMGWIDTH+i]-RightImg[(j-WndWidth)*IMGWIDTH+i-d]);
				else
				absDiff=0;
				Wnd[i*DOMAIN+d]=Wnd[i*DOMAIN+d]-absDiff;
				}
				}
				*/
				if (Wnd[i*DOMAIN + d]<minSum)
				{
					minSum = Wnd[i*DOMAIN + d];
					disp = d * 15;
				}

			}

			if (j>WndWidth && i>DOMAIN)
				Disparity[j * IMGWIDTH + i] = disp;

		}

	}

	delete[]Col;
	delete[]Wnd;

}

void SSD(unsigned char *LeftImg, unsigned char *RightImg, unsigned char *Disparity)
{
	int i, j, d;
	int minSum;
	int sqDiff;
	int disp;
	int *Col;
	int *Wnd;

	Col = new int[IMGWIDTH*DOMAIN];
	Wnd = new int[IMGWIDTH*DOMAIN];

	for (i = 0; i<IMGWIDTH*DOMAIN; i++)
	{
		Col[i] = 0;
		Wnd[i] = 0;
	}

	for (j = 0; j<IMGHEIGHT; j++)
		for (i = 0; i<IMGWIDTH; i++)
		{
			minSum = 100000;
			disp = 0;
			for (d = 0; d<DOMAIN; d++)
			{
				if (i>DOMAIN)
					sqDiff = (int)pow((double)(LeftImg[j*IMGWIDTH + i] - RightImg[j*IMGWIDTH + i - d]), 2);
				else
					sqDiff = 0;

				Col[i*DOMAIN + d] = Col[i*DOMAIN + d] + sqDiff;

				if (j>WndWidth)
				{
					if (i>DOMAIN)
						sqDiff = (int)pow((double)(LeftImg[(j - WndWidth)*IMGWIDTH + i] - RightImg[(j - WndWidth)*IMGWIDTH + i - d]), 2);
					else
						sqDiff = 0;
					Col[i*DOMAIN + d] = Col[i*DOMAIN + d] - sqDiff;
				}


				if (i<DOMAIN || j<WndWidth)
					Wnd[i*DOMAIN + d] = 0;
				else
					Wnd[i*DOMAIN + d] = Wnd[(i - 1)*DOMAIN + d] + Col[i*DOMAIN + d]
					- Col[(i - WndWidth)*DOMAIN + d];

				if (Wnd[i*DOMAIN + d]<minSum)
				{
					minSum = Wnd[i*DOMAIN + d];
					disp = d * 15;
				}

			}
			if (j>WndWidth && i>DOMAIN)
				Disparity[j * IMGWIDTH + i] = disp;

		}

	delete[]Col;
	delete[]Wnd;

}

void Rank(unsigned char *LeftImg, unsigned char *RightImg, unsigned char *Disparity)
{
	unsigned char *lfRankValue, *rtRankValue;

	lfRankValue = new unsigned char[IMGHEIGHT*IMGWIDTH];
	rtRankValue = new unsigned char[IMGHEIGHT*IMGWIDTH];

	RankTransform(LeftImg, lfRankValue);
	RankTransform(RightImg, rtRankValue);
	SAD(lfRankValue, rtRankValue, Disparity);

}

void RankTransform(unsigned char *Image, unsigned char *RankValue)
{
	int i, j, p, q;
	int TRANSWIND;
	int count;
	unsigned char center, neighbor;

	TRANSWIND = 4;
	for (i = 0; i<IMGHEIGHT; i++)
		for (j = 0; j<IMGWIDTH; j++)
		{
			center = Image[i*IMGWIDTH + j];
			count = 0;

			for (p = i - TRANSWIND; p <= i + TRANSWIND; p++)//9*9 wnd
			{
				for (q = j - TRANSWIND; q <= j + TRANSWIND; q++)
				{
					if (!((p == i) && (q == j)))
					{
						if (p<0 || p >= IMGHEIGHT || q<0 || q >= IMGWIDTH)
							neighbor = 0;
						else
							neighbor = Image[p*IMGWIDTH + q];

						if (neighbor > center)
							count++;
					}
				}
			}
			RankValue[i*IMGWIDTH + j] = count;
		}
}



void Census(unsigned char *LeftImg, unsigned char *RightImg, unsigned char *Disparity)
{
	int i, j, d;
	int p, q;
	int Hdist;
	int minSum;
	int disp;
	int *Col;
	int *Wnd;

	CodeStruct *lfCode, *rtCode;

	lfCode = new CodeStruct[IMGHEIGHT*IMGWIDTH];
	rtCode = new CodeStruct[IMGHEIGHT*IMGWIDTH];

	/*/////CENSUS TRANSFORM/////*/
	for (i = 0; i<IMGHEIGHT*IMGWIDTH; i++)
		for (j = 0; j<CODEWIDTH; j++)
		{
			lfCode[i].codebit[j] = 0;
			rtCode[i].codebit[j] = 0;
		}


	CensusTransform(LeftImg, lfCode);
	CensusTransform(RightImg, rtCode);

	Col = new int[IMGWIDTH*DOMAIN];
	Wnd = new int[IMGWIDTH*DOMAIN];

	for (i = 0; i<IMGWIDTH*DOMAIN; i++)
	{
		Col[i] = 0;
		Wnd[i] = 0;
	}

	for (j = 0; j<IMGHEIGHT; j++)
		for (i = 0; i<IMGWIDTH; i++)
		{
			minSum = 100000;
			disp = 0;
			for (d = 0; d<DOMAIN; d++)
			{
				if (i>DOMAIN)
					Hdist = HammingDist(lfCode[j*IMGWIDTH + i], rtCode[j*IMGWIDTH + i - d]);
				else
					Hdist = 0;

				Col[i*DOMAIN + d] = Col[i*DOMAIN + d] + Hdist;

				if (j >= WndWidth)
				{
					if (i>DOMAIN)
						Hdist = HammingDist(lfCode[(j - WndWidth)*IMGWIDTH + i], rtCode[(j - WndWidth)*IMGWIDTH + i - d]);
					else
						Hdist = 0;
					Col[i*DOMAIN + d] = Col[i*DOMAIN + d] - Hdist;
				}

				if (i<DOMAIN)
					Wnd[i*DOMAIN + d] = 0;
				else
					Wnd[i*DOMAIN + d] = Wnd[(i - 1)*DOMAIN + d] + Col[i*DOMAIN + d]
					- Col[(i - WndWidth)*DOMAIN + d];

				/*
				if( i>WndWidth )
				Wnd[i*DOMAIN+d]=Wnd[(i-1)*DOMAIN+d]+Col[i*DOMAIN+d]-Col[(i-WndWidth)*DOMAIN+d];
				else
				{
				if( i>d )
				Hdist=HammingDist(lfCode[j*IMGWIDTH+i],rtCode[j*IMGWIDTH+i-d]);
				else
				Hdist=0;

				Wnd[i*DOMAIN+d]=Wnd[i*DOMAIN+d]+Hdist;
				if( j>WndWidth )
				{
				if( i>d )
				Hdist=HammingDist(lfCode[(j-WndWidth)*IMGWIDTH+i],rtCode[(j-WndWidth)*IMGWIDTH+i-d]);
				else
				Hdist=0;
				Wnd[i*DOMAIN+d]=Wnd[i*DOMAIN+d]-Hdist;
				}
				}
				*/

				if (Wnd[i*DOMAIN + d] <= minSum)
				{
					minSum = Wnd[i*DOMAIN + d];
					disp = d * 15;
				}

			}
			if (j>WndWidth && i>DOMAIN)
				Disparity[j * IMGWIDTH + i] = disp;

		}

	delete[]lfCode;
	delete[]rtCode;
}

void CensusTransform(unsigned char *Image, CodeStruct Code[])
{
	int i, j, p, q;
	int TRANSWIND;
	int bit;
	unsigned char center, neighbor;

	TRANSWIND = 4;
	for (i = 0; i<IMGHEIGHT; i++)
		for (j = 0; j<IMGWIDTH; j++)
		{
			center = Image[i*IMGWIDTH + j];
			bit = 0;

			for (p = i - TRANSWIND; p <= i + TRANSWIND; p++)//9*9 wnd
			{
				for (q = j - TRANSWIND; q <= j + TRANSWIND; q++)
				{
					if (!((p == i) && (q == j)))
					{
						if (p<0 || p >= IMGHEIGHT || q<0 || q >= IMGWIDTH)
							neighbor = 0;
						else
							neighbor = Image[p*IMGWIDTH + q];

						if (neighbor > center)
							Code[i*IMGWIDTH + j].codebit[bit] = 1;
						else
							Code[i*IMGWIDTH + j].codebit[bit] = 0;
						bit++;
					}
				}
			}
		}
}

int HammingDist(CodeStruct Code1, CodeStruct Code2)
{
	int i;
	int dist;

	dist = 0;
	for (i = 0; i<CODEWIDTH; i++)
	{
		if (Code1.codebit[i] != Code2.codebit[i])
			dist++;
	}
	return dist;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i, j;
	unsigned char *LeftImg, *RightImg, *Disparity;

	double pixVal;
	IplImage *Left, *Right;
	IplImage *Result_Census, *Result_SSD, *Result_SAD, *Result_Rank;
	CvSize ImgSize;

	Left = cvLoadImage("left.ppm", 0);
	Right = cvLoadImage("right.ppm", 0);

	IMGHEIGHT = Left->height;
	IMGWIDTH = Left->width;

	LeftImg = new unsigned char[IMGHEIGHT*IMGWIDTH];
	RightImg = new unsigned char[IMGHEIGHT*IMGWIDTH];
	Disparity = new unsigned char[IMGHEIGHT*IMGWIDTH];

	ImgSize.height = IMGHEIGHT;
	ImgSize.width = IMGWIDTH;

	Result_Census = cvCreateImage(ImgSize, IPL_DEPTH_8U, 1);
	//Result_SAD = cvCreateImage(ImgSize, IPL_DEPTH_8U, 1);
	//Result_SSD = cvCreateImage(ImgSize, IPL_DEPTH_8U, 1);
	//Result_Rank = cvCreateImage(ImgSize, IPL_DEPTH_8U, 1);

	for (j = 0; j<IMGHEIGHT; j++)
	{
		for (i = 0; i<IMGWIDTH; i++)
		{
			pixVal = cvGetReal2D(Left, j, i);
			LeftImg[j*IMGWIDTH + i] = (unsigned char)pixVal;

			pixVal = cvGetReal2D(Right, j, i);
			RightImg[j*IMGWIDTH + i] = (unsigned char)pixVal;

			pixVal = 0.0;
			Disparity[j*IMGWIDTH + i] = (unsigned char)pixVal;
		}
	}

	//cvSetZero(Result_Rank);
	//cvSetZero(Result_SAD);
	//cvSetZero(Result_SSD);

	//	SAD( LeftImg, RightImg, Disparity );
	//	SSD( LeftImg, RightImg, Disparity );

	clock_t start;
	clock_t end;
	start = clock();

	Census(LeftImg, RightImg, Disparity);

	end = clock();

	for (j = 0; j<IMGHEIGHT; j++){
		for (i = 0; i<IMGWIDTH; i++)
		{
			cvSetReal2D(Result_Census, j, i, Disparity[j*IMGWIDTH + i]);
		}
	}


	printf("time: %.3f s\n", (double)(end - start) / CLOCKS_PER_SEC);
	cvNamedWindow("Census", 1);
	cvShowImage("Census", Result_Census);




	for (j = 0; j<IMGHEIGHT; j++)
	{
		for (i = 0; i<IMGWIDTH; i++)
		{
			pixVal = 0.0;
			Disparity[j*IMGWIDTH + i] = (unsigned char)pixVal;
		}
	}
	Rank(LeftImg, RightImg, Disparity);

	//for (j = 0; j<IMGHEIGHT; j++){
	//	for (i = 0; i<IMGWIDTH; i++)
	//	{
	//		cvSetReal2D(Result_Rank, j, i, Disparity[j*IMGWIDTH + i]);
	//	}
	//}
	//cvNamedWindow("Rank", 1);
	//cvShowImage("Rank", Result_Rank);



	//for (j = 0; j<IMGHEIGHT; j++)
	//{
	//	for (i = 0; i<IMGWIDTH; i++)
	//	{
	//		pixVal = 0.0;
	//		Disparity[j*IMGWIDTH + i] = (unsigned char)pixVal;
	//	}
	//}
	//SSD(LeftImg, RightImg, Disparity);

	//for (j = 0; j<IMGHEIGHT; j++){
	//	for (i = 0; i<IMGWIDTH; i++)
	//	{
	//		cvSetReal2D(Result_SSD, j, i, Disparity[j*IMGWIDTH + i]);
	//	}
	//}
	//cvNamedWindow("SSD", 1);
	//cvShowImage("SSD", Result_SSD);



	//for (j = 0; j<IMGHEIGHT; j++)
	//{
	//	for (i = 0; i<IMGWIDTH; i++)
	//	{
	//		pixVal = 0.0;
	//		Disparity[j*IMGWIDTH + i] = (unsigned char)pixVal;
	//	}
	//}
	//SAD(LeftImg, RightImg, Disparity);

	//for (j = 0; j<IMGHEIGHT; j++){
	//	for (i = 0; i<IMGWIDTH; i++)
	//	{
	//		cvSetReal2D(Result_SAD, j, i, Disparity[j*IMGWIDTH + i]);
	//	}
	//}
	//cvNamedWindow("SAD", 1);
	//cvShowImage("SAD", Result_SAD);



	cvWaitKey(0);
	cvSaveImage("dispCensus.jpg", Result_Census);
	//cvSaveImage("dispSAD.jpg", Result_SAD);
	//cvSaveImage("dispSSD.jpg", Result_SSD);
	//cvSaveImage("dispRank.jpg", Result_Rank);
	//cvDestroyWindow("SAD");
	//cvDestroyWindow("SSD");
	//cvDestroyWindow("Rank");
	cvDestroyWindow("Census");

	return 0;
}

