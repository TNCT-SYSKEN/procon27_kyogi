# include "GUI.h"

// グローバル変数
bool lmoving = false;
cv::Point mouse;
vector<shared_ptr<Piece> > pieces;
shared_ptr<Piece> piece = nullptr;
cv::Point click_point;	//クリックしたところの座標
						//回転
vector<cv::Point> src_point; //元の座標の配列
bool rmoving = false;
cv::Point min_point;

GUI::GUI() {
	img = cv::Mat(cv::Size(960, 540), CV_8UC3, cv::Scalar(100, 100, 100));
}

// コールバック関数
void my_mouse_callback(int event, int x, int y, int flags, void* param) {
	switch (event) {
	case cv::EVENT_MOUSEMOVE:
		if (lmoving) {
			vector<shared_ptr<cv::Point> > vertex = piece->vertex;
			//Mouseの位置ををピースに代入
			for (int i = 0; i < piece->number_of_corner; i++) {
				vertex[i]->x = src_point[i].x + (x - click_point.x);
				vertex[i]->y = src_point[i].y + (y - click_point.y);
			}
		}
		else if (rmoving) {
			vector<shared_ptr<cv::Point> > vertex = piece->vertex;
			//回転した値を代入
			cv::Point C;
			double rad = (y - click_point.y) / 50.0;
			for (int i = 0; i < piece->number_of_corner; i++) {
				C.x = min_point.x + (src_point[i].x - min_point.x) * cos(rad) - (src_point[i].y - min_point.y) * sin(rad);
				C.y = min_point.y + (src_point[i].x - min_point.x) * sin(rad) + (src_point[i].y - min_point.y) * cos(rad);
				*vertex[i] = C;
			}
		}
		break;
		//左クリックでピースを移動
	case cv::EVENT_LBUTTONDOWN:
		lmoving = true;
		for (int i = 0; i < pieces.size(); i++) {
			if (!pieces[i]->put_flag)	continue;
			vector<shared_ptr<cv::Point> > vertex = pieces[i]->vertex;
			cv::Point mi = pieces[i]->get_min_vertex();
			cv::Point ma = pieces[i]->get_max_vertex();
			//クリックした範囲内にあるか判定
			if (x > mi.x && y > mi.y && x < ma.x  && y < ma.y) {
				for (int i = 0; i < vertex.size(); i++) {
					src_point.push_back(*vertex[i]);
				}
				piece = pieces[i];
				click_point = cv::Point(x, y);
				break;
			}
			//なければfalse
			if (i == pieces.size() - 1) lmoving = false;
		}
		break;

	case cv::EVENT_LBUTTONUP:
		src_point.clear();
		lmoving = false;
		piece = nullptr;
		break;
		//右クリックで回転を行う
	case cv::EVENT_RBUTTONDOWN:
		//現在の座標を格納
		click_point.x = x;
		click_point.x = y;

		rmoving = true;
		for (int i = 0; i < pieces.size(); i++) {
			if (!pieces[i]->put_flag)	continue;
			vector<shared_ptr<cv::Point> > vertex = pieces[i]->vertex;
			cv::Point mi = pieces[i]->get_min_vertex();
			cv::Point ma = pieces[i]->get_max_vertex();
			if (x > mi.x && y > mi.y && x < ma.x  && y < ma.y) {
				//変更前のデータを残しておく
				for (int i = 0; i < vertex.size(); i++) {
					src_point.push_back(*vertex[i]);
				}
				min_point = *(pieces[i]->vertex)[0];
				piece = pieces[i];
				break;
			}
			if (i == pieces.size() - 1) { rmoving = false; }
		}
		break;
	case cv::EVENT_RBUTTONUP:
		src_point.clear();
		rmoving = false;
		piece = nullptr;
		break;
	}
}

void GUI::draw(vector<shared_ptr<Piece> > pie, shared_ptr<Frame> frame) {
	pieces = pie;
	//前景画像の変形行列

	//クローン
	cv::Mat temp = img.clone();

	// ウィンドウを生成
	cv::namedWindow("GUI", CV_WINDOW_AUTOSIZE);

	// コールバックを設定
	cv::setMouseCallback("GUI", my_mouse_callback, (void *)&img);

	//ピース描画用
	cv::Point app[50];
	//フレーム描画用
	cv::Point frame_app[50];

	//フレーム描画
	vector<shared_ptr<cv::Point> > frame_vertex = frame->vertex;
	for (int j = 0; j < frame->number_of_corner; j++) {
		app[j] = *frame_vertex[j];
	}
	//Frameの描画
	for (int j = 0; j < frame->number_of_corner; j++) {
		if (j != frame->number_of_corner - 1) {
			line(img, cv::Point(frame_vertex[j]->x, frame_vertex[j]->y),
				cv::Point(frame_vertex[j + 1]->x, frame_vertex[j + 1]->y), frame->color, 1, 8);

		}
		else {
			line(img, cv::Point(frame_vertex[j]->x, frame_vertex[j]->y),
				cv::Point(frame_vertex[0]->x, frame_vertex[0]->y), frame->color, 1, 8);

		}
		//cv::circle(img, cv::Point(frame_vertex[j]->x, frame_vertex[j]->y), 5, cv::Scalar(0, 200, 0), 1, 8);
	}

	// Main loop
	while (1) {
		// imageをtempにコピー
		img.copyTo(temp);

		//ピースの描画
		for (int i = 0; i < pieces.size(); i++) {
			shared_ptr<Piece> p = pieces[i];
			//if (!p->put_flag)	continue;
			vector<shared_ptr<cv::Point> > vertex = p->vertex;
			//角の数
			int corner = p->number_of_corner;
			//普通の配列じゃないとコンパイラが通らないので
			for (int j = 0; j < corner; j++) {
				app[j] = *vertex[j];
			}
			//多角形の書き込み
			//cv::fillConvexPoly(temp, app, corner, p->color);
			//各線を書き込む
			for (int j = 0; j < corner; j++) {
				if (j != corner - 1) {
					line(temp, cv::Point(vertex[j]->x, vertex[j]->y),
						cv::Point(vertex[j + 1]->x, vertex[j + 1]->y), p->color, 1, 8);

				}
				else {
					line(temp, cv::Point(vertex[j]->x, vertex[j]->y),
						cv::Point(vertex[0]->x, vertex[0]->y), p->color, 1, 8);

				}
				//cv::circle(temp, cv::Point(vertex[j]->x, vertex[j]->y), 5, cv::Scalar(0, 200, 0), 1, 8);
			}
		}

		cv::imshow("GUI", temp);

		// Escで終了
		if (cv::waitKey(15) == 27)
			break;
	}
}