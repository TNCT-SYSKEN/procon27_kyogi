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
	pieces = vector<shared_ptr<Piece> >();
}

void PieceManager::init_pieces(vector<shared_ptr<cv::Mat> > images, shared_ptr<Frame> f) {
	for (int i = 0; i < images.size(); i++) {
		pieces.push_back(make_shared<Piece>(Piece(images[i], i)));
	}
	//frame = f;
}

//アルゴリズムを動かす
void PieceManager::algorithm_exe() {
	//比較するやつ
	//numberとangleを返すと楽
	search_apply();
}

//ここに比較する処理を書いてく
//値はポインタを使ってかえしてくだしあ
void PieceManager::search_apply() {
	
}

void PieceManager::pair_angle() {
	shared_ptr<double> test;
	vector<shared_ptr<double> > frame_angle;
	for (int i = 0; i < frame->get_angle().size(); i++) {
		frame_angle.push_back(test);
	}
	frame_angle = frame->get_angle();
	//枠の角度の確保

	vector<shared_ptr<double> > angles;
	for (int i = 0; i < pieces.size(); i++) {
		for (int j = 0; j < pieces[j]->get_angle().size(); j++) {
			angles.push_back(test);
		}
	}

	for (int i = 0; i < frame_angle.size(); i++) {
		for (int j = 0; j < pieces.size(); j++) {
			angles = pieces[j]->get_angle();
			//j番目のピースの角度
			for (int k = 0; k < angles.size(); k++) {
				if (*frame_angle[i] - *angles[k] <= gosa ||
				*frame_angle[i] - *angles[k] >= gosa) {
					pair_line();
					//誤差はdefineで指定。角度が一致するものを探索
					//↑修正。誤差は最もずれの小さい組み合わせとする。
					/*com_piece com;
					com.num_frame_angle = i;
					com.num_piece = j;
					com.num_angle = k;
					combination_angles.push_back(com);
					com.print();
					//piecemaneger.hにあるよ*/
				}
			}
		}
	}
}

void PieceManager::pair_line() {
	struct piece line;
	shared_ptr<double> test;
	vector<shared_ptr<double> > frame_line = frame->get_line_lengths();
	//frameの辺
	shared_ptr<double> lines;
	//lines = pieces[line.piece_number]->get_piece_line(line.piece_line);
	if (*frame_line[line.frame_line] - *lines > 0 - gosa) {
		//枠とピースの各辺を比較。ただしピースは枠を超えない
		//ここでのi,jは角度の処理に依存する

		//(枠を更新)
	}
}

/*void PieceManager::exec_algorithm() {
	search_angle();
	put_image();
}*/

/*void PieceManager::search_angle() {
	//フレームの角度
	vector<shared_ptr<double> > frame_angles = frame->get_angle();

	for (int i = 0; i < frame_angles.size(); i++) {
		for (int j = 0; j < pieces.size(); j++) {
			//ピース一つの角度
			vector<shared_ptr<double> > angles = pieces[j]->get_angle();
			for (int k = 0; k < angles.size(); k++) {
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
}*/

/*void PieceManager::put_image() {
	//枠とピースの頂点座標を取ってくる
	com_piece com = combination_angles[0];
	vector<shared_ptr<cv::Point> > frame_vertex = frame->get_vertex();
	vector<shared_ptr<cv::Point> > piece_vertex = pieces[com.num_piece]->get_vertex();

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
}*/