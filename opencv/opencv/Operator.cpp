# include "Operator.h"

# define piece_number 1

Operator::Operator() {
	piece_manager = make_shared<PieceManager>();
	gui = make_shared<GUI>();
	images = vector<shared_ptr<cv::Mat> >();
}

void Operator::exec() {
	read_image();
	//init_pieces();
}

void Operator::read_image() {
	//読み込んだ画像はimagesにpushしていく
	int i = 0;
		string str = "item/test.jpg";
		cut_image(str);
	//切り分けた画像をpushする
		while (1) {
			string img = "new_item/cut";
			img += i;
			img += ".jpg";
			//確か白黒にしてからpushしてたので二値化
			cv::Mat bin_img = cv::imread(change_bw(img, i),1);
			i++;
		}

	//枠の読み込み
	frame = make_shared<Frame>(make_shared<cv::Mat> (cv::imread("item/frame.png", 0)), 0);
}

void Operator::init_pieces() {
	piece_manager->init_pieces(images,frame);
}

void Operator::cut_image(string str) {
	//輪郭のアレ
	vector<vector<cv::Point>> contours;

	vector < cv::Point > approx;
	//画像の読み取り
	cv::Mat piece = cv::imread(str, 0);

	cv::Mat ans = cv::imread(str, 1);

	vector<cv::Rect> roi;

	vector<cv::Mat> cut_ans;

	int roicount = 0;

	cv::threshold(piece, piece, 70, 255, CV_THRESH_BINARY);

	cv::findContours(piece, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	for (auto contour = contours.begin(); contour != contours.end(); contour++) {
		cv::polylines(piece, *contour, true, cv::Scalar(255, 255, 255), 2);
	}

	cout << contours.size() << endl;

	for (auto contour = contours.begin(); contour != contours.end(); contour++) {
		// 直線近似してます
		cv::approxPolyDP(cv::Mat(*contour),approx, 0.01 * cv::arcLength(*contour, true), true);

		double area = cv::contourArea(approx);

		if (area > 1000.0) {
			//青で囲む場合            
			//cv::polylines(ans, approx, true, cv::Scalar(255, 0, 255), 2);
			std::stringstream sst;
			sst << "area : " << area;
			//cv::putText(ans, sst.str(), approx[0], CV_FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 128, 0));

			cv::Rect brect = cv::boundingRect(cv::Mat(approx).reshape(2));
			roi.push_back(brect);
		}
	}

	cout << roi.size() << endl;

	for (int i = 0; i < roi.size(); i++) {
		cv::Mat cut_img(ans, roi[i]);
		string cut = "new_item/cut";
		cut += to_string(i);
		cut += ".jpg";
		cv::imwrite(cut, cut_img);
	}

	cv::imshow("hoge", ans);

	cv::imshow("hogehoge", piece);
}

string Operator::change_bw(string img,int i) {
	cv::Mat gray = cv::imread(img, 0);
	cv::Mat bin;
	ostringstream oss;
	threshold(gray, bin, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	oss << "new_item/bin" << i << ".jpg";
	string new_img = oss.str();
	cv::imwrite(new_img, bin);
	return new_img;
}
