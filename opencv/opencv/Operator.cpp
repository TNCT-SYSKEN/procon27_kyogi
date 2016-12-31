# include "Operator.h"

# define piece_number 1

int i;
int hoge = 0;

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
	//images->push_back(ほげほげ);
	//ここぐらいはやってもらわないとこまる割合的に(結局やった
	//for (int i = 1; i <= piece_number;i++) {
		//string str = "item/img (";
		//str += to_string(i);
		//str += ").png";
		//cut_image(str);
		string str = "item/test.png";
		cut_image(str);
	//	hoge = 1;
	//}
	for (int i = 0; i < 4; i++) {
		//string str = "new_item/img";
		//string str = "item/img (";
		string str = "new_item/dst";
		str += to_string(i);
		str += ".jpg";
		cv::Mat a = cv::imread(str, 0);
		cv::imshow(str, a);
		images.push_back(make_shared<cv::Mat> (cv::imread(str, 0)));
	}

	//枠の読み込み
	//frame = make_shared<Frame>(make_shared<cv::Mat> (cv::imread("item/frame.png", 0)), 0);
}

void Operator::init_pieces() {
	piece_manager->init_pieces(images,frame);
}

// コールバック関数
/*void mouse_callback(int event, int x, int y, int flags, void* param) {
	cv::Mat* image = static_cast<cv::Mat*>(param);

	switch (event) {

	case cv::EVENT_LBUTTONDOWN:
		drawing_box = true;
		box = cv::Rect(x, y, 0, 0);
		start.x = x;
		start.y = y;
		cout << "start" << endl;
		cout << start << endl;
		break;

	case cv::EVENT_LBUTTONUP:
		drawing_box = false;
		stop.x = x;
		stop.y = y;
		cout << "stop" << endl;
		cout << stop << endl;
		/*if (box.width < 0) {
		box.x += box.width;
		box.width *= -1;
		}
		if (box.height < 0) {
		box.y += box.height;
		box.height *= -1;
		}
		draw_box(image, box);

		cv::Mat cut_img(piece, cv::Rect(start, stop));
		string name = "item/image (";
		name += to_string(flag);
		name += ").png";
		cv::imwrite(name, cut_img);
		flag++;

		break;
	}
}*/

/*void Operator::cut_image(string str) {
	std::string name = "Box Example";
	box = cv::Rect(-1, -1, 0, 0);
	piece = cv::imread(str, 0);
	// ウィンドウを生成
	if (hoge != 1) {
		cv::imshow(str, piece);

		// コールバックを設定
		cv::setMouseCallback(str, mouse_callback, (void *)&piece);

		cv::waitKey(0);
	}
	else {
		cv::Mat cut_img(piece, cv::Rect(start, stop));
		string name = "item/image (";
		name += to_string(flag);
		name += ").png";
		cv::imwrite(name, cut_img);
		flag++;
	}
	

}*/

void Operator::cut_image(string str) {
	//画像の読み取り
	cv::Mat piece = cv::imread(str, 0);
	//分割するための点を定義
	vector<cv::Point> p = { { 0, 0 },{ piece.cols / 2, 0 },{ 0, piece.rows / 2 },{ piece.cols / 2, piece.rows / 2 } };
	for (auto itr = p.begin(); itr != p.end(); ++itr) {
		// 関心領域を元の画像から矩形で切り出す
		cv::Mat roi = piece(cv::Rect(itr->x, itr->y, piece.cols / 2, piece.rows / 2));
		// 連番で画像を書き出す
		cv::imwrite("new_item/dst" + std::to_string(itr - p.begin()) + ".jpg", roi);
	}
}
