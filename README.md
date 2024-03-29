# Stereo Matching Based on Visual Sensitive Information

With the spirit of reproducible research, this repository contains codes required to produce the results in the manuscript:

> H. Wang, M. S. Pathan, and S. Dev, Stereo Matching Based on Visual Sensitive Information, *6th IEEE International Conference on Image, Vision and Computing (ICIVC)*, 2021.

Please cite the above paper if you intent to use whole/part of the code. This code is only for academic and research purposes.


## Code
* `./Figure Code/`: This folder contains Census model code which can produce the `Figure1.png` after running. It should run in vs2012 + pcl1.8.0 + boost environment.
* `./Table Code/`: This folder contains the test code in middlebury. It will produce the information data of the proposed algorithm in `Table1.png` and `Table2.png`. I use opencv 2.4.8, other versions should be OK but not too low.

## Data
* `./Data/`: This folder contains the data folder contains Tsukuba, Venus, Teddy and cones datasets from Middlebury official test datasets. The current directory `images.xml` is the input item, it will read the contents of the file. In the previous directory `./sample/test_images`, the image in the images directory is the input item and the name is the same as the name in `images.xml` correspondingly. The output results are saved in the result directory under the current directory.
* `./Data_extended/`: This folder contains 27 data sets from Middlebury official test datasets. 

This is the process of using this code: 

Firstly, open Error_Non_31.sln in vs2012.

Secondly, Configure opencv (I use opencv 2.4.8, other versions should be OK, but not too low)

Thirdly, users need to do is put the disparity map you get into the corresponding folder. 
For example, Tsukuba's parallax maps are named tsukuba_dis.ppm , and put it in D:\Error_Non_31\Data\tsukuba.
The prefixes of Venus, Teddy and cones are same as Tsukuba. The other 27 stereo matching pairs were put in the corresponding folder in D:\Error_Non_31\Data_extended\Aloe folder.
For example, Aloe is named:_Aloe.ppm Put in D:\Error_Non_31\Data_extended\Aloe folder

Fourthly, the prefixes and suffixes (image format) of all parallax maps can be modified, but for convenience, it is not recommended to modify the program. 
As for image format conversion, we use XnConvert software to convert images among .pgm, .ppm, .png formats.

Fifthly, after testing the error rate, check the folder of the corresponding path (e.g. D:\Error_Non_31\Data\tsukuba) can get the image of the error rate of the non occluded area, in which the error pixels of the non occluded area are marked in red. The error rate is consistent with the non occluded error rate. Reminding that these two data sets includes test data and disparity map which are standard images.

## Figure
* `./Figure1.png`: This figure shows the comparison of different test results. The first column is the perspective image in the original image of the experimental data set, the second column is the ideal parallax image for reference, the third and fourth columns are the result images obtained by the classical algorithm census, and the fifth and sixth columns are stereo matching algorithms based on visual sensitive information. Among them, the red marks in the fourth and sixth columns indicate the mismatching areas of the marks when comparing the two result maps with the ideal disparity map. Through the display of subjective images, we can clearly see that the proposed algorithm can effectively calculate and generate the disparity of the original image, and the matching effect is better than that of the classical algorithm. 

<div align=center><img src="https://user-images.githubusercontent.com/80974728/113465074-e9326800-9463-11eb-9931-2a99fb8c48ec.png"/></div>

## Table
* `./Table1.png`: This table shows the error matching rate obtained by various algorithms. It shows that error matching rate of the proposed algorithm is lower than that of the same kind of algorithm, and the overall matching effect is better.
* `./Table2.png`: This table shows the mismatch rate under different Gaussian noise concentrations. It shows that show that when the noise concentration increases, the change degree of the error matching rate of this algorithm is lower, and the sensitivity to noise is lower.
