# include "PieceManager.h"

# include "Piece.h"

# include "Frame.h"

# include "fstream"

#define gosa 1
//test
struct piece {
	double frame_line;
	double piece_line;
	double piece_number;
};

PieceManager::PieceManager() {
	pieces = vector<shared_ptr<Piece>>();
}

void PieceManager::init_pieces(vector<shared_ptr<cv::Mat> > images, shared_ptr<Frame> f) {
	for (int i = 0; i < (int)images.size(); i++) {
		pieces.push_back(make_shared<Piece>(Piece(images[i], i)));
	}
	frame = f;
}

void PieceManager::exec_algorithm() {
	search_angle();
	put_image();
}

void PieceManager::search_angle() {
	//フレームの角度
	vector<shared_ptr<double> > frame_angles = frame->angle;

	for (int i = 0; i < (int)frame_angles.size(); i++) {
		for (int j = 0; j < (int)pieces.size(); j++) {
			//ピース一つの角度
			vector<shared_ptr<double> > angles = pieces[j]->angle;
			for (int k = 0; k < (int)angles.size(); k++) {
				int p_ang = (int)(*angles[k]);
				int f_ang = (int)(*frame_angles[i]);
				//許容範囲は+-2
				if (abs(p_ang - f_ang) <= 2) {
					com_piece com;
					com.num_frame_angle = i;
					com.num_piece = j;
					com.num_angle = k;
					combination_angles.push_back(com);
					com.print();
				}
			}
		}
	}
	cout << endl;
}

bool PieceManager::search_line(int p) {
	int i = combination_angles[p].num_angle;
	int j = combination_angles[p].num_piece;
	int k = combination_angles[p].num_frame_angle;
	//vector<shared_ptr<double> > frame_lines = frame->get_line_lengths();
	vector<shared_ptr<double> > frame_lines = frame->line_lengths;
	vector<shared_ptr<double> > piece_lines = pieces[j]->line_lengths;
	shared_ptr<double> piece_line_other;
	if (k != 0) {
		piece_line_other = pieces[j]->line_lengths[k - 1];
	}
	else if (k == 0) {
		piece_line_other = pieces[j]->line_lengths[piece_lines.size() - 1];
	}
	if (i == 0) {
		if (*frame_lines[frame_lines.size() - 1] - *piece_line_other) {
			if (*frame_lines[i] - *piece_lines[k] > 0) {
				return true;
			}
		}
	}
	else if (i != 0) {
		if (*frame_lines[i - 1] - *piece_line_other) {
			if (*frame_lines[i] - *piece_lines[k] > 0) {
				return true;
			}
		}
	}
	return false;
}

void PieceManager::create_frame(int i, int j, int k) {
	int set_frame, set_line;
	vector<shared_ptr<double> > sub_frame_line; //枠の辺を避難(格納)させる配列
	vector<double> sub_piece_line; //除かれるピースの辺を格納するhoge
	vector<shared_ptr<double> > frame_lines = frame->line_lengths;
	vector<shared_ptr<double> > piece_lines = pieces[j]->line_lengths;
	for (int p = 0; p < (int)frame_lines.size(); p++) {
		sub_frame_line.push_back(frame_lines[p]);
	}

	//ピースの除かれる部分を探索する処理
	for (int p = 0; p < (int)piece_lines.size(); p++) {
		if (i + p >= (int)frame_lines.size()
			&& k + p >= (int)piece_lines.size()) {
			set_frame = i + p - (int)frame_lines.size();
			set_line = k + p - (int)piece_lines.size();
		}
		else if (i + p >= (int)frame_lines.size()) {
			set_frame = i + p - (int)frame_lines.size();
			set_line = p;
		}
		else if (k + p >= (int)piece_lines.size()) {
			set_frame = i;
			set_line = k + p - (int)piece_lines.size();
		}
		else {
			set_frame = i;
			set_line = k;
		}

		//枠とピースの辺の比較処理
		//もし枠の辺とピースの辺が同じであったなら次の処理で除く
		//許容範囲は+-2
		if ((double)abs(*frame_lines[set_frame] - *piece_lines[set_line]) <= 2) {
			sub_piece_line.push_back(set_line);
		}

	}
	//テスト
	for (int p = 0; p < (int)sub_piece_line.size();p++) {
		cout << sub_piece_line[p];
	}

	//ここで要素全削除
	frame->line_lengths.erase(frame->line_lengths.begin(), frame->line_lengths.end());

	//ここからピースの情報をpushback、ただし上で見つけた部分(sub_piece_line)を除く
	//↑具体的にはpieces[j]->get_piece_line(sub_piece_line[i])
	for (int p = k; p >= 0; p--) {
		int flag = 0;
		for (int h = 0; h < (int)sub_piece_line.size(); h++) {
			if (p == h) {
				flag = 1;
			}
			if (flag == 1) {
				frame->line_lengths.push_back(piece_lines[p]);
				flag = 0;
			}
		}
	}
	for (int p = (int)piece_lines.size(); p > k; p--) {
		int flag = 0;
		for (int h = 0; h < (int)sub_piece_line.size(); h++) {
			if (p == h) {
				flag = 1;
			}
			if (flag == 1) {
				frame->line_lengths.push_back(piece_lines[p]);
				flag = 0;
			}
		}
	}

	//ここから枠の情報をpushback
	//↓更新
	//1ピース分しか出来てない
	
	if (i == 0) {
		double test = *frame_lines[j] - *piece_lines[k];
		shared_ptr<double> diff(&test);
		frame->line_lengths.push_back(diff);
	}

	for (int p = 0; p < (int)sub_piece_line.size(); p++) {
		if (p == i) {
			double test = *frame_lines[p] - *piece_lines[k];
			shared_ptr<double> diff(&test);
			frame->line_lengths.push_back(diff);
		}
		else if (i != 0) {
			if (i - 1 == p) {
				double test = *frame_lines[p] - *piece_lines[k-1];
				shared_ptr<double> diff(&test);
				frame->line_lengths.push_back(diff);
			}
		}
		else if (i == 0) {
			if ((int)frame_lines.size() == p) {
				double test = *frame_lines[p] - *piece_lines[k - 1];
				shared_ptr<double> diff(&test);
				frame->line_lengths.push_back(diff);
			}
		}
		else {
			double test = *frame_lines[p];
			shared_ptr<double> f_lines(&test);
			frame->line_lengths.push_back(f_lines);
		}
	}
}

void PieceManager::put_image() {
	//枠とピースの頂点座標を取ってくる
	//後で引数iを渡す
	com_piece com = combination_angles[0];
	/*com_piece com = combination_angles[i];*/
	vector<shared_ptr<cv::Point> > frame_vertex = frame->vertex;
	vector<shared_ptr<cv::Point> > piece_vertex = pieces[com.num_piece]->vertex;

	//ピースの角度が同じ場所に移動
	cv::Point diff = *frame_vertex[com.num_frame_angle] - *piece_vertex[com.num_angle];
	for (int i = 0; i < piece_vertex.size(); i++) {
		*piece_vertex[i] += diff;
	}
	pieces[com.num_piece]->put_flag = true;

	//指定された頂点を中心に回転
	//傾きが最も近くなるように回転を行う
	cv::Point root = *piece_vertex[com.num_angle];
	cout << com.num_angle << endl;
	//基準の両隣の頂点（ピース
	cv::Point next_piece_vertex[2];
	//基準の両隣の頂点（フレーム
	cv::Point next_frame_vertex[2];
	//両隣の２点の格納ピース
	if (com.num_angle == 0) {
		next_piece_vertex[0] = *piece_vertex[piece_vertex.size() - 1];
		next_piece_vertex[1] = *piece_vertex[com.num_angle + 1];
	}
	else if (com.num_angle == piece_vertex.size() - 1) {
		next_piece_vertex[0] = *piece_vertex[piece_vertex.size() - 1];
		next_piece_vertex[1] = *piece_vertex[0];
	}
	else {
		next_piece_vertex[0] = *piece_vertex[com.num_angle - 1];
		next_piece_vertex[1] = *piece_vertex[com.num_angle + 1];
	}
	//両隣の２点の格納フレーム
	if (com.num_frame_angle == 0) {
		next_frame_vertex[0] = *frame_vertex[frame_vertex.size() - 1];
		next_frame_vertex[1] = *frame_vertex[com.num_angle + 1];
	}
	else if (com.num_angle == frame_vertex.size() - 1) {
		next_frame_vertex[0] = *frame_vertex[frame_vertex.size() - 1];
		next_frame_vertex[1] = *frame_vertex[0];
	}
	else {
		next_frame_vertex[0] = *frame_vertex[com.num_angle - 1];
		next_frame_vertex[1] = *frame_vertex[com.num_angle + 1];
	}
	//最も近しいものを判定
	cv::Point C;	//変数名クソ
	double radian = 0;
	int dif = 10000;
	//フレームの頂点
	cv::Point fp = *frame_vertex[com.num_frame_angle];
	for (int i = 0; i < 360; i++) {//とりあえず360度試す
		double rad = i / 180.0*3.1419;
		double sum = 0;
		//フレームの２辺のそれぞれの傾き
		double slope[2];
		slope[0] = (fp.y - next_frame_vertex[0].y) / (double)(fp.x - next_frame_vertex[0].x);
		slope[1] = (fp.y - next_frame_vertex[1].y) / (double)(fp.x - next_frame_vertex[1].x);
		if (slope[0] > slope[1])	swap(slope[0], slope[1]);

		for (int j = 0; j < 2; j++) {//両隣のへん
			cv::Point src = next_piece_vertex[j];
			//回転させてみる
			C.x = root.x + (src.x - root.x) * cos(rad) - (src.y - root.y) * sin(rad);
			C.y = root.y + (src.x - root.x) * sin(rad) + (src.y - root.y) * cos(rad);

			//あきらかにはみ出ているものは省く
			if ((C.x + 5 < frame->get_min_vertex().x) || (C.y + 5 < frame->get_min_vertex().y) ||
				(C.x - 5 >= frame->get_max_vertex().x) || (C.y - 5 >= frame->get_max_vertex().y)) {
				sum = 100000;
				break;
			}

			//ピースの２辺の傾き
			double piece_slope = (fp.y - C.y) / (double)(fp.x - C.x);
			if (abs(piece_slope - slope[0]) < abs(piece_slope - slope[1])) {
				sum += abs(piece_slope - slope[0]);
				slope[0] = 100000000;
			}
			else {
				sum += abs(piece_slope - slope[1]);
				slope[1] = 100000000;
			}
		}
		if (sum < dif) {
			dif = sum;
			radian = rad;
		}
	}
	for (int i = 0; i < piece_vertex.size(); i++) {
		cv::Point src = *piece_vertex[i];
		C.x = root.x + (src.x - root.x) * cos(radian) - (src.y - root.y) * sin(radian);
		C.y = root.y + (src.x - root.x) * sin(radian) + (src.y - root.y) * cos(radian);
		*piece_vertex[i] = C;
	}
}

void PieceManager::line_search() {


}