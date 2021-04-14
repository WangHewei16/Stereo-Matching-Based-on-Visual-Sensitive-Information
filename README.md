## Code
* `./Figure Code/`: This folder contains Census model code which can produce the `Figure1.png` after running. It should run in vs2012 + pcl1.8.0 + boost environment.
* `./Table Code/`: This folder contains the test code in middlebury. It will produce the information data of the proposed algorithm in `Table1.png` and `Table2.png`. I use opencv 2.4.8, other versions should be OK but not too low.

## Data
* `./Data/`: This folder contains the data folder contains Tsukuba, Venus, Teddy and cones datasets from Middlebury official test datasets.
* `./Data_extended/`: This folder contains 27 data sets from Middlebury official test datasets. 

These two data sets includes test data and disparity map which are standard images.

## Figure
* `./Figure1.png`: This figure shows the comparison of different test results. The first column is the perspective image in the original image of the experimental data set, the second column is the ideal parallax image for reference, the third and fourth columns are the result images obtained by the classical algorithm census, and the fifth and sixth columns are stereo matching algorithms based on visual sensitive information. Among them, the red marks in the fourth and sixth columns indicate the mismatching areas of the marks when comparing the two result maps with the ideal disparity map. Through the display of subjective images, we can clearly see that the proposed algorithm can effectively calculate and generate the disparity of the original image, and the matching effect is better than that of the classical algorithm. 

<div align=center><img src="https://user-images.githubusercontent.com/80974728/113465074-e9326800-9463-11eb-9931-2a99fb8c48ec.png"/></div>

## Table
* `./Table1.png`: This table shows the error matching rate obtained by various algorithms. It shows that error matching rate of the proposed algorithm is lower than that of the same kind of algorithm, and the overall matching effect is better.
* `./Table2.png`: This table shows the mismatch rate under different Gaussian noise concentrations. It shows that show that when the noise concentration increases, the change degree of the error matching rate of this algorithm is lower, and the sensitivity to noise is lower.
