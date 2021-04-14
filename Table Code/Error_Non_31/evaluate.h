#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "config.h"

static float TestResult_OnePixel_Val(const char* result_name, const char* gt_name, const char* mask_name, float scale, const char* result_bad = NULL) 
{
	cv::Mat img_result = cv::imread(result_name, cv::IMREAD_GRAYSCALE);
	cv::Mat img_groundTruth = cv::imread(gt_name, cv::IMREAD_GRAYSCALE);
	cv::Mat img_mask = cv::imread(mask_name, cv::IMREAD_GRAYSCALE);

	cv::Size size = img_result.size();
	cv::Mat img_bad;
	cv::cvtColor(img_result, img_bad, CV_GRAY2BGR);

	cv::Mat1b maskPtr = img_mask;
	int val_result, val_g;
	int num_err = 0, num_all = 0;
	for(int y = 0;y < size.height;y++)
	{
		for(int x = 0;x < size.width;x++)
		{
			if(maskPtr(y, x) == 255) 
			{
				val_result = img_result.at<uchar>(y, x);
				val_g = img_groundTruth.at<uchar>(y, x);

		    if(abs(val_result - val_g) > scale)  //1.0 pixel error rate in non occluded area
		    // if(abs(val_result - val_g) > scale/2.0) //0.5 error rate in non occluded area
				{
					img_bad.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 255);
					num_err++;
				}
				num_all++;
			}
		}
	}

	if(result_bad) 
	{
		cv::imwrite(result_bad, img_bad);
	}

	return float(num_err) / num_all;
}

static float TestResult_OnePixel_NonOcc(const char* result_name, const char* gt_name_l, const char* gt_name_r, int scale, const char* result_bad = NULL) 
{
	cv::Mat img_result = cv::imread(result_name, cv::IMREAD_GRAYSCALE);
	cv::Mat img_groundTruthL = cv::imread(gt_name_l, cv::IMREAD_GRAYSCALE);
	cv::Mat img_groundTruthR = cv::imread(gt_name_r, cv::IMREAD_GRAYSCALE);

	cv::Size size = img_result.size();
	cv::Mat img_bad;
	cv::cvtColor(img_result, img_bad, CV_GRAY2BGR);

	//int val_g, val_result, val_g_cor;
	float  val_g, val_result, val_g_cor;
	int num_err = 0, num_all = 0;
	for(int y = 0;y < size.height;y++) 
	{
		for(int x = 0;x < size.width;x++) 
		{
			val_g = img_groundTruthL.at<uchar>(y, x) / scale;
			if(val_g > 0 && x - val_g >= 0) 
			{
				val_g_cor = img_groundTruthR.at<uchar>(y, x - val_g) / scale;
				if(val_g == val_g_cor) 
				{
					val_result = img_result.at<uchar>(y, x) / scale;
					if(abs(val_result - val_g) > 1) 
					{
						img_bad.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 255);
						num_err++;
					}
					num_all++;
				}
			}
		}
	}

	if(result_bad) 
	{
		cv::imwrite(result_bad, img_bad);
	}

	return float(num_err) / num_all;
}

static int CompDepthRange(const char* gt_name, int scale) 
{
	int max_disparity=0;
	cv::Mat img_groundTruth = cv::imread(gt_name, cv::IMREAD_GRAYSCALE);
	cv::Size size = img_groundTruth.size();

	for(int y = 0;y < size.height;y++) 
	{
		for(int x = 0;x < size.width;x++) 
		{
			int val = img_groundTruth.at<uchar>(y, x);
			val /= scale;
			max_disparity = MAX(max_disparity, val);
		}
	}

	return max_disparity;
}