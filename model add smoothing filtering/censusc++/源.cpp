#include<iostream>   
#include<cv.h>   
#include<highgui.h>   

using namespace std;
int GetHammingWeight(unsigned int value);
int main() {
	/*Half of the window size for the census transform*/
	int hWin = 11;
	int bitlength = 0;
	if ((2 * hWin + 1)*(2 * hWin + 1) % 32 == 0)
	{
		bitlength = (2 * hWin + 1)*(2 * hWin + 1) / 32;
	}
	else {
		bitlength = (2 * hWin + 1)*(2 * hWin + 1) / 32 + 1;
	}
	cout << "hWin: " << hWin << ";  " << "bit length:  " << bitlength << endl;
	cout << "Census test" << endl;
	// char stopKey;   
	IplImage * leftImage = cvLoadImage("C:\\Users\\60136\\Desktop\\ÔÓÏî\\left.jpg", 0);
	IplImage * rightImage = cvLoadImage("C:\\Users\\60136\\Desktop\\ÔÓÏî\\right.jpg", 0);

	int imageWidth = leftImage->width;
	int imageHeight = leftImage->height;

	IplImage * CensusImage = cvCreateImage(cvGetSize(leftImage), leftImage->depth, 1);
	IplImage * MatchLevelImage = cvCreateImage(cvGetSize(leftImage), leftImage->depth, 1);

	int minDBounds = 0;
	int maxDBounds = 31;
	// int leftCensus[imageHeight][imageWidth][bitlength] = {0};     
	unsigned  int *leftCensus = new unsigned int[imageHeight*imageWidth*bitlength];
	unsigned  int *rightCensus = new unsigned int[imageHeight*imageWidth*bitlength];
	for (int i = 0;i < imageHeight*imageWidth*bitlength;i++)
	{
		leftCensus[i] = 0;
		rightCensus[i] = 0;
	}
	int pointCnt = 0;




	cvNamedWindow("Left", 1);
	cvNamedWindow("Right", 1);
	cvNamedWindow("Census", 1);
	cvNamedWindow("MatchLevel", 1);

	cvShowImage("Left", leftImage);
	cvShowImage("Right", rightImage);



	/*Census Transform */
	int i, j, m, n, k, l;
	unsigned char centerPixel = 0;
	unsigned char neighborPixel = 0;
	int bitCount = 0;
	unsigned int bigger = 0;
	for (i = 0; i < leftImage->height;i++) {
		for (j = 0; j< leftImage->width;j++) {
			centerPixel = *((unsigned char *)leftImage->imageData + i*leftImage->widthStep + j);
			bitCount = 0;

			for (m = i - hWin; m <= i + hWin;m++)
			{
				for (n = j - hWin; n <= j + hWin;n++)
				{
					bitCount++;
					if (m < 0 || m >= leftImage->height || n < 0 || n >= leftImage->width)
					{
						neighborPixel = 0;
					}
					else {
						neighborPixel = *((unsigned char *)leftImage->imageData + m*leftImage->widthStep + n);
					}
					bigger = (neighborPixel > centerPixel) ? 1 : 0;
					leftCensus[(i*imageWidth + j)*bitlength + bitCount / 32] |= (bigger << (bitCount % 32));
				}
			}

		}
	}

	for (i = 0; i < rightImage->height;i++) {
		for (j = 0; j< rightImage->width;j++) {
			centerPixel = *((unsigned char *)rightImage->imageData + i*rightImage->widthStep + j);
			bitCount = 0;

			for (m = i - hWin; m <= i + hWin;m++)
			{
				for (n = j - hWin; n <= j + hWin;n++)
				{
					bitCount++;
					if (m < 0 || m >= rightImage->height || n < 0 || n >= rightImage->width)
					{
						neighborPixel = 0;
					}
					else {
						neighborPixel = *((unsigned char *)rightImage->imageData + m*rightImage->widthStep + n);
					}
					bigger = (neighborPixel > centerPixel) ? 1 : 0;
					rightCensus[(i*imageWidth + j)*bitlength + bitCount / 32] |= (bigger << (bitCount % 32));
				}
			}

		}
	}
	int sum = 0;
	unsigned int *matchLevel = new unsigned int[maxDBounds - minDBounds + 1];
	int tempMin = 0;
	int tempIndex = 0;
	unsigned  char *dst;
	unsigned char pixle = 0;
	for (i = 0; i < rightImage->height;i++) {
		for (j = 0; j< rightImage->width;j++) {


			for (k = minDBounds;k <= maxDBounds;k++)
			{
				sum = 0;
				for (l = 0;l< bitlength;l++)
				{
					if (((i*imageWidth + j + k)*bitlength + l) < imageHeight*imageWidth*bitlength)
					{
						sum += GetHammingWeight(rightCensus[(i*imageWidth + j)*bitlength + l]
							^ leftCensus[(i*imageWidth + j + k)*bitlength + l]);
					}
					else {
						//sum += 0;   
						// cout<<".";   
					}

				}
				matchLevel[k] = sum;
			}

			/*Ñ°ÕÒ×î¼ÑÆ¥Åäµã*/
			tempMin = 0;
			tempIndex = 0;
			for (m = 1;m < maxDBounds - minDBounds + 1;m++)
			{
				if (matchLevel[m] < matchLevel[tempIndex])
				{
					tempMin = matchLevel[m];
					tempIndex = m;
				}
			}

			if (tempMin > (2 * hWin + 1)*(2 * hWin + 1)*0.2)
			{
				tempMin = 0;
				pointCnt++;
			}
			else {
				tempMin = 255;
			}
			dst = (unsigned char *)CensusImage->imageData + i*CensusImage->widthStep + j;
			*dst = tempIndex * 8;

			dst = (unsigned char *)MatchLevelImage->imageData + i*MatchLevelImage->widthStep + j;
			*dst = tempMin;

			//cout<< tempIndex<<"  " <<tempMin<<endl;;   

		}
	}
	cout << "pointCnt:  " << pointCnt << endl;
	cvShowImage("Census", CensusImage);
	cvShowImage("MatchLevel", MatchLevelImage);
	cvSaveImage("depth.jpg", CensusImage);
	cvSaveImage("matchLevel.jpg", MatchLevelImage);

	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&leftImage);
	cvReleaseImage(&rightImage);
	return 0;
}


int GetHammingWeight(unsigned int value)
{
	if (value == 0) return 0;

	int a = value;
	int b = value - 1;
	int c = 0;

	int count = 1;
	while (c = a & b)
	{
		count++;
		a = c;
		b = c - 1;
	}
	return count;
}