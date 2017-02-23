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
	frame = f;
}

void PieceManager::exec_algorithm() {
	int i;
	search_angle();
	for (i = combination_angles.size(); i >= 0; i--) {
		if (search_line(i)) {
			//角度、辺ともに合ってればtrue
			//trueであるときcombination_angles[i]は正しいと言える
			break;
		}
	}
	put_image(i);
}

void PieceManager::search_angle() {
	//フレームの角度
	vector<shared_ptr<double> > frame_angles = frame->get_angle();
	double deviation = 2; //保持されているframeとピースの角度の最も小さい誤差。初期値は2
	int flag = 0;

	for (int i = 0; i < frame_angles.size(); i++) {
		for (int j = 0; j < pieces.size(); j++) {
			//ピース一つの角度
			vector<shared_ptr<double> > angles = pieces[j]->get_angle();
			com_piece com;
			for (int k = 0; k < angles.size(); k++) {
				int p_ang = (int)(*angles[k]);
				int f_ang = (int)(*frame_angles[i]);
				//許容範囲は+-2
				if (f_ang - p_ang > 0 || f_ang - p_ang < deviation) {
					com.num_frame_angle = i;
					com.num_piece = j;
					com.num_angle = k;
					deviation = p_ang - f_ang;
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
	vector<shared_ptr<double> > frame_lines = frame->get_line_lengths();
	shared_ptr<double> piece_lines = pieces[j]->get_piece_line(k);
	if (j == 0 && k != 0) {
		shared_ptr<double> piece_line_other = pieces[pieces.size() - 1]->get_piece_line(k - 1);
	}
	else if (k == 0 && j != 0) {
		shared_ptr<double> piece_line_other = pieces[j - 1]->get_piece_line(sizeof(*pieces[j - 1]) - 1);
	}
	else if (k == 0 && j == 0) {
		shared_ptr<double> piece_line_other = pieces[pieces.size() - 1]->get_piece_line(sizeof(*pieces[j - 1]) - 1);
	}
	else if (k != 0 && j != 0) {
		shared_ptr<double> piece_lines_other = pieces[j - 1]->get_piece_line(k - 1);
	}
	if (*frame_lines[i] - *piece_lines > 0) {
		return true;
	}
	return false;
}

void PieceManager::put_image(int i) {
	//枠とピースの頂点座標を取ってくる
	com_piece com = combination_angles[i];
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
}