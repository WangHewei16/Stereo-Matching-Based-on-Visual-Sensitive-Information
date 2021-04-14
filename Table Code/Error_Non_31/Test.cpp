/************************************************************
Function£º Test Error Project for stereo pairs
Description:
			1)Test error for 2001/2003 /2005 /2006 MiddleBury DataSets
            2)And mark bad pixels with red dot
			3)Replace the offline "Middlebury Evaluation Version2"
			4)"OpenCV" library is needed(we use version of 2.4.8),
			  and software platform is "Visual Studio 2010"
			Tips:We also recommand you to use other version of "OpenCV" and "Visual Studio"
Author£º Peng Yao
Data£º 2015/06
Institute£º Key Laboratory of Computer Vision and System
			300384, Tianjin ,China
			Tianjin University of Technology
Email: yp19880120@sina.com
*************************************************************/

#include <cstdlib>
#include <cstdio>
#include "evaluate.h"
#include <fstream>

enum METHOD {MST_D_RAW, MST_D_REFINED};

void stereo_launch(const char *left_img, const char *right_img, const char *disp_img, int max_level, int scale, float sigma, METHOD m) 
{
	char command_buf[1023];
	sprintf(command_buf, "%s %s %s %s %d %d %f %d", RUN_PATH, left_img, right_img, disp_img, max_level, scale, sigma, m == MST_D_RAW ? 0 : 1);
}

float TestRoutine_Batch_MiddleburyStandard(float sigma, METHOD bST_type) 
{
	stereo_launch( ROOT_PATH "Data/tsukuba/imL.png", ROOT_PATH "Data/tsukuba/imR.png", ROOT_PATH "Data/tsukuba/tsukuba_dis.ppm", 15, 16, sigma, bST_type);
	stereo_launch( ROOT_PATH "Data/venus/imL.png", ROOT_PATH "Data/venus/imR.png", ROOT_PATH "Data/venus/venus_dis.ppm", 19, 8, sigma, bST_type);
	stereo_launch( ROOT_PATH "Data/teddy/imL.png", ROOT_PATH "Data/teddy/imR.png", ROOT_PATH "Data/teddy/teddy_dis.png", 59, 4, sigma, bST_type);
	stereo_launch( ROOT_PATH "Data/cones/imL.png", ROOT_PATH "Data/cones/imR.png", ROOT_PATH "Data/cones/cones_dis.ppm", 59, 4, sigma, bST_type);

	float err_average=0, err_average_non_occ=0, err_non_occ=0;
	err_average += TestResult_OnePixel_Val(ROOT_PATH "Data/tsukuba/tsukuba_dis.png", ROOT_PATH "Data/tsukuba/groundtruth.pgm", ROOT_PATH "Data/tsukuba/mask.bmp", 16);
	err_average += TestResult_OnePixel_Val(ROOT_PATH "Data/tsukuba/tsukuba_dis.png", ROOT_PATH "Data/tsukuba/groundtruth.pgm", ROOT_PATH "Data/tsukuba/tsukuba_mask_disc.bmp", 16);
	err_average += err_non_occ = TestResult_OnePixel_Val(ROOT_PATH "Data/tsukuba/tsukuba_dis.png", ROOT_PATH "Data/tsukuba/groundtruth.pgm", ROOT_PATH "Data/tsukuba/tsukuba_mask_nonocc.bmp", 16, ROOT_PATH "Data/tsukuba/tsukuba_bad.png");
	printf("  Tsukuba: %.02f%%\n",100 * err_non_occ);
	err_average_non_occ += err_non_occ;
	
	err_average += TestResult_OnePixel_Val(ROOT_PATH "Data/venus/venus_dis.png", ROOT_PATH "Data/venus/groundtruth.png", ROOT_PATH "Data/venus/mask.bmp", 8);
	err_average += TestResult_OnePixel_Val(ROOT_PATH "Data/venus/venus_dis.png", ROOT_PATH "Data/venus/groundtruth.png", ROOT_PATH "Data/venus/venus_mask_disc.bmp", 8);
	err_average += err_non_occ = TestResult_OnePixel_Val(ROOT_PATH "Data/venus/venus_dis.png", ROOT_PATH "Data/venus/groundtruth.png", ROOT_PATH "Data/venus/venus_mask_nonocc.bmp", 8, ROOT_PATH "Data/venus/venus_bad.png");
	printf("  Venus: %.02f%%\n",100 * err_non_occ);
	err_average_non_occ+=err_non_occ;
	
	err_average += TestResult_OnePixel_Val(ROOT_PATH "Data/teddy/teddy_dis.png", ROOT_PATH "Data/teddy/groundtruth.png", ROOT_PATH "Data/teddy/mask.bmp", 4);
	err_average += TestResult_OnePixel_Val(ROOT_PATH "Data/teddy/teddy_dis.png", ROOT_PATH "Data/teddy/groundtruth.png", ROOT_PATH "Data/teddy/teddy_mask_disc.bmp", 4);
	err_average += err_non_occ = TestResult_OnePixel_Val(ROOT_PATH "Data/teddy/teddy_dis.png", ROOT_PATH "Data/teddy/groundtruth.png", ROOT_PATH "Data/teddy/teddy_mask_nonocc.bmp", 4, ROOT_PATH "Data/teddy/teddy_bad.png");
	printf("  Teddy: %.02f%%\n",100 * err_non_occ);
	err_average_non_occ+=err_non_occ;

	err_average += TestResult_OnePixel_Val(ROOT_PATH "Data/cones/cones_dis.png", ROOT_PATH "Data/cones/groundtruth.png", ROOT_PATH  "Data/cones/mask.bmp", 4);
	err_average += TestResult_OnePixel_Val(ROOT_PATH "Data/cones/cones_dis.png", ROOT_PATH "Data/cones/groundtruth.png", ROOT_PATH  "Data/cones/cones_mask_disc.bmp", 4);
	err_average += err_non_occ = TestResult_OnePixel_Val(ROOT_PATH "Data/cones/cones_dis.png", ROOT_PATH "Data/cones/groundtruth.png", ROOT_PATH "Data/cones/cones_mask_nonocc.bmp", 4, ROOT_PATH "Data/cones/cones_bad.png");
	printf("  Cones: %.02f%%\n",100 * err_non_occ);
	err_average_non_occ+=err_non_occ;

	return (100 * err_average_non_occ / 4);
}

char cTestFilePool[256][256]=
{
	//         ground_truth_L           //                ground_truth_R                //Your Disparity Map Result	(.ppm)		  //  Left Image                            // Right Image							  // Bad Pixels of Result
	ROOT_PATH "Data_extended/Aloe/disp1.png", ROOT_PATH "Data_extended/Aloe/disp5.png", ROOT_PATH "Data_extended/Aloe/_Aloe.ppm", ROOT_PATH "Data_extended/Aloe/view1.png", ROOT_PATH "Data_extended/Aloe/view5.png", ROOT_PATH "Data_extended/Aloe/_Aloe_bad.png",
	ROOT_PATH "Data_extended/Art/disp1.png", ROOT_PATH "Data_extended/Art/disp5.png", ROOT_PATH "Data_extended/Art/_Art.ppm", ROOT_PATH "Data_extended/Art/view1.png", ROOT_PATH "Data_extended/Art/view5.png", ROOT_PATH "Data_extended/Art/_Art_bad.png",
	ROOT_PATH "Data_extended/Baby1/disp1.png", ROOT_PATH "Data_extended/Baby1/disp5.png", ROOT_PATH "Data_extended/Baby1/_Baby1.ppm", ROOT_PATH "Data_extended/Baby1/view1.png", ROOT_PATH "Data_extended/Baby1/view5.png", ROOT_PATH "Data_extended/Baby1/_Baby1_bad.png",
	ROOT_PATH "Data_extended/Baby2/disp1.png", ROOT_PATH "Data_extended/Baby2/disp5.png", ROOT_PATH "Data_extended/Baby2/_Baby2.ppm", ROOT_PATH "Data_extended/Baby2/view1.png", ROOT_PATH "Data_extended/Baby2/view5.png", ROOT_PATH "Data_extended/Baby2/_Baby2_bad.png",
	ROOT_PATH "Data_extended/Baby3/disp1.png", ROOT_PATH "Data_extended/Baby3/disp5.png", ROOT_PATH "Data_extended/Baby3/_Baby3.ppm", ROOT_PATH "Data_extended/Baby3/view1.png", ROOT_PATH "Data_extended/Baby3/view5.png", ROOT_PATH "Data_extended/Baby3/_Baby3_bad.png",
	ROOT_PATH "Data_extended/Books/disp1.png", ROOT_PATH "Data_extended/Books/disp5.png", ROOT_PATH "Data_extended/Books/_Books.ppm", ROOT_PATH "Data_extended/Books/view1.png", ROOT_PATH "Data_extended/Books/view5.png", ROOT_PATH "Data_extended/Books/_Books_bad.png",
	ROOT_PATH "Data_extended/Bowling1/disp1.png", ROOT_PATH "Data_extended/Bowling1/disp5.png", ROOT_PATH "Data_extended/Bowling1/_Bowling1.ppm", ROOT_PATH "Data_extended/Bowling1/view1.png", ROOT_PATH "Data_extended/Bowling1/view5.png", ROOT_PATH "Data_extended/Bowling1/_Bowling1_bad.png",
	ROOT_PATH "Data_extended/Bowling2/disp1.png", ROOT_PATH "Data_extended/Bowling2/disp5.png", ROOT_PATH "Data_extended/Bowling2/_Bowling2.ppm", ROOT_PATH "Data_extended/Bowling2/view1.png", ROOT_PATH "Data_extended/Bowling2/view5.png", ROOT_PATH "Data_extended/Bowling2/_Bowling2_bad.png",
	ROOT_PATH "Data_extended/Cloth1/disp1.png", ROOT_PATH "Data_extended/Cloth1/disp5.png", ROOT_PATH "Data_extended/Cloth1/_Cloth1.ppm", ROOT_PATH "Data_extended/Cloth1/view1.png",ROOT_PATH  "Data_extended/Cloth1/view5.png", ROOT_PATH "Data_extended/Cloth1/_Cloth1_bad.png",
	ROOT_PATH "Data_extended/Cloth2/disp1.png", ROOT_PATH "Data_extended/Cloth2/disp5.png", ROOT_PATH "Data_extended/Cloth2/_Cloth2.ppm", ROOT_PATH "Data_extended/Cloth2/view1.png",ROOT_PATH  "Data_extended/Cloth2/view5.png", ROOT_PATH "Data_extended/Cloth2/_Cloth2_bad.png",
	ROOT_PATH "Data_extended/Cloth3/disp1.png", ROOT_PATH "Data_extended/Cloth3/disp5.png", ROOT_PATH "Data_extended/Cloth3/_Cloth3.ppm", ROOT_PATH "Data_extended/Cloth3/view1.png", ROOT_PATH "Data_extended/Cloth3/view5.png", ROOT_PATH "Data_extended/Cloth3/_Cloth3_bad.png",
	ROOT_PATH "Data_extended/Cloth4/disp1.png", ROOT_PATH "Data_extended/Cloth4/disp5.png", ROOT_PATH "Data_extended/Cloth4/_Cloth4.ppm", ROOT_PATH "Data_extended/Cloth4/view1.png", ROOT_PATH "Data_extended/Cloth4/view5.png", ROOT_PATH "Data_extended/Cloth4/_Cloth4_bad.png",
	ROOT_PATH "Data_extended/Dolls/disp1.png", ROOT_PATH "Data_extended/Dolls/disp5.png", ROOT_PATH "Data_extended/Dolls/_Dolls.ppm", ROOT_PATH "Data_extended/Dolls/view1.png", ROOT_PATH "Data_extended/Dolls/view5.png", ROOT_PATH "Data_extended/Dolls/_Dolls_bad.png",
	ROOT_PATH "Data_extended/Flowerpots/disp1.png", ROOT_PATH "Data_extended/Flowerpots/disp5.png", ROOT_PATH "Data_extended/Flowerpots/_Flowerpots.ppm", ROOT_PATH "Data_extended/Flowerpots/view1.png", ROOT_PATH "Data_extended/Flowerpots/view5.png", ROOT_PATH "Data_extended/Flowerpots/_Flowerpots_bad.png",
	ROOT_PATH "Data_extended/Lampshade1/disp1.png", ROOT_PATH "Data_extended/Lampshade1/disp5.png", ROOT_PATH "Data_extended/Lampshade1/_Lampshade1.ppm", ROOT_PATH "Data_extended/Lampshade1/view1.png", ROOT_PATH "Data_extended/Lampshade1/view5.png", ROOT_PATH "Data_extended/Lampshade1/_Lampshade1_bad.png",
	ROOT_PATH "Data_extended/Lampshade2/disp1.png", ROOT_PATH "Data_extended/Lampshade2/disp5.png", ROOT_PATH "Data_extended/Lampshade2/_Lampshade2.ppm", ROOT_PATH "Data_extended/Lampshade2/view1.png", ROOT_PATH "Data_extended/Lampshade2/view5.png", ROOT_PATH "Data_extended/Lampshade2/_Lampshade2_bad.png",
	ROOT_PATH "Data_extended/Laundry/disp1.png", ROOT_PATH "Data_extended/Laundry/disp5.png", ROOT_PATH "Data_extended/Laundry/_Laundry.ppm", ROOT_PATH "Data_extended/Laundry/view1.png", ROOT_PATH "Data_extended/Laundry/view5.png", ROOT_PATH "Data_extended/Laundry/_Laundry_bad.png",
	ROOT_PATH "Data_extended/Midd1/disp1.png", ROOT_PATH "Data_extended/Midd1/disp5.png", ROOT_PATH "Data_extended/Midd1/_Midd1.ppm", ROOT_PATH "Data_extended/Midd1/view1.png", ROOT_PATH "Data_extended/Midd1/view5.png", ROOT_PATH "Data_extended/Midd1/_Midd1_bad.png",
	ROOT_PATH "Data_extended/Midd2/disp1.png", ROOT_PATH "Data_extended/Midd2/disp5.png", ROOT_PATH "Data_extended/Midd2/_Midd2.ppm", ROOT_PATH "Data_extended/Midd2/view2.png", ROOT_PATH "Data_extended/Midd1/view5.png", ROOT_PATH "Data_extended/Midd2/_Midd2_bad.png",
	ROOT_PATH "Data_extended/Moebius/disp1.png", ROOT_PATH "Data_extended/Moebius/disp5.png", ROOT_PATH "Data_extended/Moebius/_Moebius.ppm", ROOT_PATH "Data_extended/Moebius/view1.png", ROOT_PATH "Data_extended/Moebius/view5.png", ROOT_PATH"Data_extended/Moebius/_Moebius_bad.png",
	ROOT_PATH "Data_extended/Monopoly/disp1.png", ROOT_PATH "Data_extended/Monopoly/disp5.png", ROOT_PATH "Data_extended/Monopoly/_Monopoly.ppm", ROOT_PATH "Data_extended/Monopoly/view1.png", ROOT_PATH "Data_extended/Monopoly/view5.png", ROOT_PATH"Data_extended/Monopoly/_Monopoly_bad.png",
	ROOT_PATH "Data_extended/Plastic/disp1.png", ROOT_PATH "Data_extended/Plastic/disp5.png", ROOT_PATH "Data_extended/Plastic/_Plastic.ppm", ROOT_PATH "Data_extended/Plastic/view1.png", ROOT_PATH "Data_extended/Plastic/view5.png", ROOT_PATH"Data_extended/Plastic/_Plastic_bad.png",
	ROOT_PATH "Data_extended/Reindeer/disp1.png", ROOT_PATH "Data_extended/Reindeer/disp5.png", ROOT_PATH "Data_extended/Reindeer/_Reindeer.ppm", ROOT_PATH "Data_extended/Reindeer/view1.png", ROOT_PATH "Data_extended/Reindeer/view5.png", ROOT_PATH "Data_extended/Reindeer/_Reindeer_bad.png",
	ROOT_PATH "Data_extended/Rocks1/disp1.png", ROOT_PATH "Data_extended/Rocks1/disp5.png", ROOT_PATH "Data_extended/Rocks1/_Rocks1.ppm", ROOT_PATH "Data_extended/Rocks1/view1.png", ROOT_PATH "Data_extended/Rocks1/view5.png", ROOT_PATH "Data_extended/Rocks1/_Rocks1_bad.png",
	ROOT_PATH "Data_extended/Rocks2/disp1.png", ROOT_PATH "Data_extended/Rocks2/disp5.png", ROOT_PATH "Data_extended/Rocks2/_Rocks2.ppm", ROOT_PATH "Data_extended/Rocks2/view1.png", ROOT_PATH "Data_extended/Rocks2/view5.png", ROOT_PATH "Data_extended/Rocks2/_Rocks2_bad.png",
	ROOT_PATH "Data_extended/Wood1/disp1.png", ROOT_PATH "Data_extended/Wood1/disp5.png", ROOT_PATH "Data_extended/Wood1/_Wood1.ppm", ROOT_PATH "Data_extended/Wood1/view1.png", ROOT_PATH "Data_extended/Wood1/view5.png", ROOT_PATH "Data_extended/Wood1/_Wood1_bad.png",	
	ROOT_PATH "Data_extended/Wood2/disp1.png", ROOT_PATH "Data_extended/Wood2/disp5.png", ROOT_PATH "Data_extended/Wood2/_Wood2.ppm", ROOT_PATH "Data_extended/Wood2/view1.png", ROOT_PATH "Data_extended/Wood2/view5.png", ROOT_PATH "Data_extended/Wood2/_Wood2_bad.png",	
};

char extendNames[256][256] = 
{
	"Aloe", "Art", "Baby1", "Baby2", "Baby3", "Books", "Bowling1","Bowling2","Cloth1","Cloth2", 
	"Cloth3","Cloth4","Dolls", "Flowerpots", "Lampshade1", "Lampshade2", "Laundry","Midd1", "Midd2","Moebius" ,
	"Monopoly","Plastic","Reindeer" ,"Rocks1", "Rocks2", "Wood1", "Wood2" ,
};

//test non-occ regions only
float TestRoutine_Batch_MiddleburyExtended(float sigma, METHOD bST_type)
{
	int scale=3;
	float err_ave=0;
	for(int i = 0;i < 27;i++)
	{
		int idx=i*6;  
		char* gt_name_l = cTestFilePool[idx]; //groundtruth_L
		char* gt_name_r = cTestFilePool[idx+1]; //groundtruth_R
		char* filename_disparity_map = cTestFilePool[idx+2]; //Your own result of disparity map
		char* filename_left_image = cTestFilePool[idx+3]; //Left Image of Stereo-Pairs
		char* filename_right_image = cTestFilePool[idx+4]; //Right Image of Stereo-Pairs
		char* filename_bad_image = cTestFilePool[idx+5]; //Bad Pixels of Result
		int max_disparity = CompDepthRange(gt_name_l, scale)+1;

		//stereo_launch(filename_left_image, filename_right_image, filename_disparity_map, max_disparity, scale, sigma, bST_type);
		float err_non_occ = TestResult_OnePixel_NonOcc(filename_disparity_map, gt_name_l, gt_name_r, scale, filename_bad_image);
		printf("  %s: %.02f%%\n",extendNames[i], 100 * err_non_occ);
		err_ave += err_non_occ * 100;
	}

	err_ave /= 27;
	return err_ave;
}

int main(int argc, const char **argv) 
{
	float error1, error2;
	const char *head = "*****  Stereo Matching Evaluation *****\n\n";
	printf("%sNon-Occluded Region (error rate):\n",head);
	//check executable file
	std::fstream _file;
	_file.open(RUN_PATH, std::ios::in);
	if(!_file) 
	{
		printf("Cannot find executable file %s, quit.\n", RUN_PATH);
		exit(1);
	} 
	else 
	{
		_file.close();
	}

	error1 = TestRoutine_Batch_MiddleburyStandard(0.1f, MST_D_RAW);
	error2 = TestRoutine_Batch_MiddleburyExtended(0.1f, MST_D_RAW);
	printf("Avg. non-occ err in 31 pairs: %.02f%%\n", (4 * error1 + 27* error2) / 31);

	printf("\nMethod MST_D_REFINED(error rate):\n");
	system("pause");
	return 0;
}