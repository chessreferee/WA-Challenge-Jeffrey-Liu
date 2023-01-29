#include "opencv2/opencv.hpp"


using namespace std;
using namespace cv; // don't need to do cv::imread

/*
The below function will filter through the area/size of the contour
*/
vector<Point> sizeFilter(vector<vector<Point>> contours) {
	vector<Point> points;
	if (contours.size() != 0) { // check that there are contours
		for (vector<Point> contour : contours) { // for each vector<point> in contours
			if (contourArea(contour) > 100 && contourArea(contour) < 1000) { // check to see if appropriate size
				cout << "made it here 2";
				Rect r; // initialize rectangle
				r = boundingRect(contour);
				//rectangle(iBGR, Scalar(r.x, r.y), Scalar(r.x + r.width, r.y + r.height), Scalar(255, 0, 0), 2, 8);

				Point p = Point(r.x + r.width, r.y + r.height);
				points.push_back(p); // add to vector
			}
		}
	}
	return points;
}

/*
false if in ignored area
true if not in ignored area
*/
bool InIgnoredArea(Point point, int rows, int cols) {
	double yGREAT = 4. / 5. * rows; // greatest x it can be
	double xGREAT = 2. / 5. * cols; // greatest y it can be
	if (0 <= point.x <= floor(xGREAT)) { // checks to make sure in valid x positions
		double m = -yGREAT / xGREAT; // the slope of the line
		double y_int = yGREAT; // the y intercept

		// check in left area
		int greatest_yLeft = m * point.x + y_int;
		if (point.y > greatest_yLeft) { // check to see if there is a y value too big
			return false; // y value too big for the x value
		}

		// check in right area
		int greatest_yRight = m * (cols - point.x) + y_int; // (cols - point.x) just flips from the right to the left
		if (point.y > greatest_yLeft) { // check to see if there is a y value too big
			return false; // y value too big for the x value
		}
		return true;
	}
	else {
		cout << "returned true, because x value is not valid for some reason";
		return false;
	}
}

/*
the below function will filter by disregarding the controus in the picture that doesn't matter due to it's location on it
This is done, because the cones are lined up straight like a road, so only the general area of where the cones are should matter.
*/
vector<Point> roadFilter(vector<Point> points, int rows, int cols) {
	vector<Point> placeholder;
	for (Point point : points) { // go through the vector and find points that are in areas that should be ignored

		if (!InIgnoredArea(point, rows, cols)) { // checks to see if not in ignored area
			placeholder.push_back(point);
		}
	}
	return placeholder;
}

/*
This method finds all the contours on the left side of the screen
*/
vector<Point> findLeftContours(vector<Point> points, Mat iBGR) {
	vector<Point> holder;

	// iterate to find left half of the screen
	for (Point point : points) {
		if (point.x <= iBGR.cols / 2) { // on the left half columns
			holder.push_back(point);
		}
	}

	return holder;
}

/*
This method finds all the contours on the right side of the screen.
*/
vector<Point> findRightContours(vector<Point> points, Mat iBGR) {
	vector<Point> holder;

	// iterate to find on the right half of the screen
	for (Point point : points) {
		if (point.x > iBGR.cols / 2) { // on right half of window
			holder.push_back(point);
		}
	}

	return holder;
}

/*
@param iBGR the image that will be drawn on
@param lineInfo - the best fit line's infor
@return mat with the line drawn to save the line drawn on the copy of iBGR
*/
Mat drawLine(Mat iBGR, Vec4f lineInfo) {
	Point p1 = Point(lineInfo[0] * 10000 + lineInfo[2], lineInfo[1] * 10000 + lineInfo[3]); // chose 10000 as a big number so the line will just go off the screen.
	Point p2 = Point(-lineInfo[0] * 10000 + lineInfo[2], -lineInfo[1] * 10000 + lineInfo[3]);
	line(iBGR, p1, p2, Scalar(0, 0, 255), 2, 8, 0);
	return iBGR;
}


int main(int argv, char** arc) {
	Mat iBGR = imread("red.png");
	//namedWindow("original", WINDOW_FREERATIO);
	//imshow("original", iBGR);

	Mat iHSV; // initialize an object for HSV picture
	cvtColor(iBGR, iHSV, COLOR_BGR2HSV);
	// imshow("new", iHSV);

	// below filters the image to find orange
	Mat colorFilter;
	int iLowH = 0;
	int iHighH = 22;

	int iLowS = 150; // wanted there to be a good amount of orange there
	int iHighS = 255;

	int iLowV = 100; // high low V, because the background is not lighted up well, so they have lower V values
	int iHighV = 255;
	
	inRange(iHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), colorFilter); // choose only orange
	//namedWindow("colorFilter", WINDOW_FREERATIO);
	//imshow("colorFilter", colorFilter);


	vector<vector<Point>> contours; // stores the contours
	vector<Vec4i> hierarchy;
	findContours(colorFilter, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	// // below just draws the contours
	//Mat iBGR_copy = iBGR.clone(); // create copy of original
	//drawContours(iBGR_copy, contours, -1, Scalar(0, 255, 0), 2);
	//namedWindow("iBGR_copy", WINDOW_FREERATIO);
	//imshow("iBGR_copy", iBGR_copy);
	/*
	if (contours.size() != 0) { // check that there are contours
		
		for (vector<Point> contour : contours) { // for each vector<point> in contours
			
			if (contourArea(contour) > 100 && contourArea(contour) < 1000) { // check to see if appropriate size
				cout << "made it here 2";
				Rect r; // initialize rectangle
				 r = boundingRect(contour);
				 //rectangle(iBGR, Scalar(r.x, r.y), Scalar(r.x + r.width, r.y + r.height), Scalar(255, 0, 0), 2, 8);
				 rectangle(iBGR, r, Scalar(0, 0, 255), 2, 8); // adds rectangle to iBGR
				 rectangle(colorFilter, r, Scalar(255, 255, 255), 2, 8); // adds rectangles to colorFilter
			}
		}
	}
	

	namedWindow("iBGR with rect", WINDOW_FREERATIO);
	imshow("iBGR with rect", iBGR);

	namedWindow("colorFilter with rect", WINDOW_FREERATIO);
	imshow("colorFilter with rect", colorFilter);
	*/
	
	vector<Point> points = sizeFilter(contours); // filters by size
	points = roadFilter(points, iBGR.rows, iBGR.cols); // filters the boxes on the sides.
	
	Mat iBGR_copy = iBGR.clone();
	// below draws circles with the midpoint of the circle being the midpoint of the contours
	for (Point point : points) {
		circle(iBGR_copy, point, 10, Scalar(0, 0, 255), 2);
		circle(colorFilter, point, 10, Scalar(255, 255, 255), 2);
	}

	//namedWindow("iBGR_copy with circle", WINDOW_FREERATIO);
	//imshow("iBGR_copy with circle", iBGR_copy);

	//namedWindow("colorFilter with circle", WINDOW_FREERATIO);
	//imshow("colorFilter with circle", colorFilter);

	// below will divide the window's contours to two groups, one left side and one right side
	vector<Point> leftContours = findLeftContours(points, iBGR);
	vector<Point> rightContours = findRightContours(points, iBGR);

	// find the best fit line, then draw the line on the left side
	Vec4f leftLineInfo;
	fitLine(leftContours, leftLineInfo, DIST_L1, 0, 0.01, 0.01);
	iBGR = drawLine(iBGR, leftLineInfo);

	// find the best fit line, then draw the line on the right side
	Vec4f rightLineInfo;
	fitLine(rightContours, rightLineInfo, DIST_L1, 0, 0.01, 0.01);
	iBGR = drawLine(iBGR, rightLineInfo);

	namedWindow("red.png", WINDOW_FREERATIO);
	imshow("red.png", iBGR);
	imwrite("answer.png", iBGR);
	waitKey();
}










