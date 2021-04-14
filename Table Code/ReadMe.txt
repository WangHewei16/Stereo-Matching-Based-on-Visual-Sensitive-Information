1. Open Error_Non_31.sln in vs2012.
2. Configure opencv (I use opencv 2.4.8, other versions should be OK, but not too low)
3.  What user need to do is put the disparity map you get into the corresponding folder. 
For example, Tsukuba's parallax maps are named tsukuba_dis.ppm , and put it in D:\Error_Non_31\Data\tsukuba.
The prefixes of Venus, Teddy and cones are same as Tsukuba. The other 27 stereo matching pairs were put in the corresponding folder in D:\Error_Non_31\Data_extended\Aloe folder.
For example, Aloe is named:_Aloe.ppm Put in D:\Error_Non_31\Data_extended\Aloe folder
4. The prefixes and suffixes (image format) of all parallax maps can be modified, but for convenience, it is not recommended to modify the program. 
As for image format conversion, I use XnConvert software to convert images among .pgm, .ppm, .png formats.
5. After testing the error rate, check the folder of the corresponding path (e.g. D:\Error_Non_31\Data\tsukuba) can get the image of the error rate of the non occluded area, 
in which the error pixels of the non occluded area are marked in red. The error rate is consistent with the non occluded error rate.
