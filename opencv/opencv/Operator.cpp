# include "Operator.h"

# define piece_number 1

Operator::Operator() {
	piece_manager = make_shared<PieceManager>();
	gui = make_shared<GUI>();
	images = vector<shared_ptr<cv::Mat> >();
}

void Operator::exec() {
	read_image();
	init_pieces();
	gui->draw(piece_manager->get_pieces(), frame);
}

void Operator::read_image() {
	//読み込んだ画像はimagesにpushしていく
	int count = 0;
	for (int i = 1;;i++) {
		ostringstream oss;
		oss << "item/piece (" << i << ").png";
		cv::Mat test = cv::imread(oss.str(), 1);
		//読み込む画像が無くなったらbreak
		if (test.data == NULL) {
			break;
		}
		count += cut_image(oss.str(),count);
	}
	//切り分けた画像をpushする
	for(int i = 0;i < count;i++){
		ostringstream img;
		img << "new_item/cut" << i << ".jpg";
		//確か白黒にしてからpushしてたので二値化
		cv::Mat bin_img = cv::imread(change_bw(img.str(), i),1);
	}

	//とりあえず枠の二値化
	frame_bin();
	//枠の読み込み
	frame = make_shared<Frame>(make_shared<cv::Mat> (cv::imread("new_item/frame.png", 0)), 0);
}

void Operator::init_pieces() {
	piece_manager->init_pieces(images,frame);
}

int Operator::cut_image(string str,int count) {
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
	
	cv::imshow("test", piece);

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
		ostringstream oss;
		oss << "new_item/cut" << i + count << ".jpg";
		cv::imwrite(oss.str(), cut_img);
	}

	cv::imshow("hoge", ans);

	cv::imshow("hogehoge", piece);

	return roi.size();
}

string Operator::change_bw(string img,int i) {
	cv::Mat gray = cv::imread(img, 0);
	ostringstream test;
	test << "test" << i;
	cv::imshow(test.str(), gray);
	cv::Mat bin;
	ostringstream oss;
	threshold(gray, bin, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	oss << "new_item/bin" << i << ".jpg";
	string new_img = oss.str();
	cv::imwrite(new_img, bin);
	return new_img;
}

void Operator::frame_bin() {
	cv::Mat gray = cv::imread("item/frame.png",0);
	cv::Mat bin;
	threshold(gray, bin, 0, 255, CV_THRESH_BINARY/* | CV_THRESH_OTSU*/);
	//bin = ~bin;
	cv::imshow("frame_bin",bin);
	cv::imwrite("new_item/frame.png",bin);
}
