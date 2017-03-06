#include "common.h"


int exe() {
	Mat origin_img[4];
	Mat gray[4];
	Mat first_pano[2];
	Mat result_pano;

	//枚数指定
	for (int i = 0; i < 4; i++) {
		ostringstream addres;
		addres << "img\\img00" << i << ".jpg";
		origin_img[i] = imread(addres.str(), 1);
		//画像が無ければ-1を返して終了
		if (origin_img[i].data == NULL) {
			return -1;
		}
		//グレースケール化
		cvtColor(origin_img[i], gray[i], CV_BGR2GRAY);
	}

	SiftFeatureDetector detector(1000);
	SiftDescriptorExtractor extrator;

	vector<KeyPoint> kp[2];
	Mat desc[2];
	for (int i = 0; i < 2; i++) {
		detector.detect(gray[i], kp[2]);
		extrator.compute(gray[i], kp[i], desc[i]);
	}

	vector<DMatch> matches;
	BruteForceMatcher<L2<float> > matcher;
	matcher.match(desc[0], desc[1], matches);

	vector<Vec2f> points1(matches.size());
	vector<Vec2f> points2(matches.size());

	for (int i = 0; i < matches.size(); i++) {
		points1[i][0] = kp[0][matches[i].queryIdx].pt.x;
		points1[i][1] = kp[0][matches[i].queryIdx].pt.y;

		points2[i][0] = kp[1][matches[i].trainIdx].pt.x;
		points2[i][1] = kp[1][matches[i].trainIdx].pt.y;
	}

	Mat match_img;
	drawMatches(origin_img[0], kp[0], origin_img[1], kp[1], matches, match_img);
	imshow("draw img", match_img);
	waitKey();
}

int main() {

	exe();

	waitKey();

	return 0;
}