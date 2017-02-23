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
			//�p�x�A�ӂƂ��ɍ����Ă��true
			//true�ł���Ƃ�combination_angles[i]�͐������ƌ�����
			break;
		}
	}
	put_image(i);
}

void PieceManager::search_angle() {
	//�t���[���̊p�x
	vector<shared_ptr<double> > frame_angles = frame->get_angle();
	double deviation = 2; //�ێ�����Ă���frame�ƃs�[�X�̊p�x�̍ł��������덷�B�����l��2
	int flag = 0;

	for (int i = 0; i < frame_angles.size(); i++) {
		for (int j = 0; j < pieces.size(); j++) {
			//�s�[�X��̊p�x
			vector<shared_ptr<double> > angles = pieces[j]->get_angle();
			com_piece com;
			for (int k = 0; k < angles.size(); k++) {
				int p_ang = (int)(*angles[k]);
				int f_ang = (int)(*frame_angles[i]);
				//���e�͈͂�+-2
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
	//�g�ƃs�[�X�̒��_���W������Ă���
	com_piece com = combination_angles[i];
	vector<shared_ptr<cv::Point> > frame_vertex = frame->get_vertex();
	vector<shared_ptr<cv::Point> > piece_vertex = pieces[com.num_piece]->get_vertex();

	//�s�[�X�̊p�x�������ꏊ�Ɉړ�
	cv::Point diff = *frame_vertex[com.num_frame_angle] - *piece_vertex[com.num_angle];
	for (int i = 0; i < piece_vertex.size(); i++) {
		*piece_vertex[i] += diff;
	}
	pieces[com.num_piece]->put_flag = true;

	//�w�肳�ꂽ���_�𒆐S�ɉ�]
	//�X�����ł��߂��Ȃ�悤�ɉ�]���s��
	cv::Point root = *piece_vertex[com.num_angle];
	cout << com.num_angle << endl;
	//��̗��ׂ̒��_�i�s�[�X
	cv::Point next_piece_vertex[2];
	//��̗��ׂ̒��_�i�t���[��
	cv::Point next_frame_vertex[2];
	//���ׂ̂Q�_�̊i�[�s�[�X
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
	//���ׂ̂Q�_�̊i�[�t���[��
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
	//�ł��߂������̂𔻒�
	cv::Point C;	//�ϐ����N�\
	double radian = 0;
	int dif = 10000;
	//�t���[���̒��_
	cv::Point fp = *frame_vertex[com.num_frame_angle];
	for (int i = 0; i < 360; i++) {//�Ƃ肠����360�x����
		double rad = i / 180.0*3.1419;
		double sum = 0;
		//�t���[���̂Q�ӂ̂��ꂼ��̌X��
		double slope[2];
		slope[0] = (fp.y - next_frame_vertex[0].y) / (double)(fp.x - next_frame_vertex[0].x);
		slope[1] = (fp.y - next_frame_vertex[1].y) / (double)(fp.x - next_frame_vertex[1].x);
		if (slope[0] > slope[1])	swap(slope[0], slope[1]);

		for (int j = 0; j < 2; j++) {//���ׂ̂ւ�
			cv::Point src = next_piece_vertex[j];
			//��]�����Ă݂�
			C.x = root.x + (src.x - root.x) * cos(rad) - (src.y - root.y) * sin(rad);
			C.y = root.y + (src.x - root.x) * sin(rad) + (src.y - root.y) * cos(rad);

			//�����炩�ɂ͂ݏo�Ă�����̂͏Ȃ�
			if ((C.x + 5 < frame->get_min_vertex().x) || (C.y + 5 < frame->get_min_vertex().y) ||
				(C.x - 5 >= frame->get_max_vertex().x) || (C.y - 5 >= frame->get_max_vertex().y)) {
				sum = 100000;
				break;
			}

			//�s�[�X�̂Q�ӂ̌X��
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