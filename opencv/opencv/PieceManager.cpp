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
	//pieces = vector<Piece>();
}

void PieceManager::init_pieces(vector<shared_ptr<cv::Mat> > images, shared_ptr<Frame> f) {
	for (int i = 0; i < images.size(); i++) {
		pieces.push_back(make_shared<Piece>(Piece(images[i], i)));
	}
	frame = f;
}

void PieceManager::exec_algorithm() {
	search_angle();
	put_image();
	for (int i = combination_angles.size(); i >= 0; i--) {
		if (search_line(i)) {
			//�p�x�A�ӂƂ��ɍ����Ă��true
			//true�ł���Ƃ�combination_angles[i]�͐������ƌ�����
			//frame_line = combination_angles[i].num_frame_angle;
			//piece_number = combination_angles[i].num_piece;
			//piece_line = combination_angles[i].num_angle;
			//create_frame(frame_line, piece_number, piece_line);
			break;
		}
	}
	put_image();
}

void PieceManager::search_angle() {
	//�t���[���̊p�x
	vector<shared_ptr<double> > frame_angles = frame->angle;

	for (int i = 0; i < frame_angles.size(); i++) {
		for (int j = 0; j < pieces.size(); j++) {
			//�s�[�X��̊p�x
			vector<shared_ptr<double> > angles = pieces[j]->angle;
			for (int k = 0; k < angles.size(); k++) {
				int p_ang = (int)(*angles[k]);
				int f_ang = (int)(*frame_angles[i]);
				//���e�͈͂�+-2
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
	vector<shared_ptr<double> > sub_frame_line; //�g�̕ӂ���(�i�[)������z��
	vector<double> sub_piece_line; //�������s�[�X�̕ӂ��i�[����hoge
	vector<shared_ptr<double> > frame_lines = frame->line_lengths;
	vector<shared_ptr<double> > piece_lines = pieces[j]->line_lengths;
	for (int p = 0; p < frame_lines.size(); p++) {
		sub_frame_line.push_back(frame_lines[p]);
	}

	//�s�[�X�̏�����镔����T�����鏈��
	for (int p = 0; p < piece_lines.size(); p++) {
		if (i + p >= frame_lines.size()
			&& k + p >= piece_lines.size()) {
			set_frame = i + p - frame_lines.size();
			set_line = k + p - piece_lines.size();
		}
		else if (i + p >= frame_lines.size()) {
			set_frame = i + p - frame_lines.size();
			set_line = p;
		}
		else if (k + p >= piece_lines.size()) {
			set_frame = i;
			set_line = k + p - piece_lines.size();
		}
		else {
			set_frame = i;
			set_line = k;
		}

		//�g�ƃs�[�X�̕ӂ̔�r����
		//�����g�̕ӂƃs�[�X�̕ӂ������ł������Ȃ玟�̏����ŏ���
		//���e�͈͂�+-2
		if (abs(*frame_lines[set_frame] - *piece_lines[set_line]) <= 2) {
			sub_piece_line.push_back(set_line);
		}

	}

	//�����ŗv�f�S�폜
	frame->line_lengths.erase(frame->line_lengths.begin(), frame->line_lengths.end());

	//��������s�[�X�̏���pushback�A��������Ō���������(sub_piece_line)������
	//����̓I�ɂ�pieces[j]->get_piece_line(sub_piece_line[i])
	for (int p = k; p >= 0; p--) {
		int flag = 0;
		for (int h = 0; h < sub_piece_line.size(); h++) {
			if (p == h) {
				flag = 1;
			}
			if (flag == 1) {
				frame->line_lengths.push_back(piece_lines[p]);
				flag = 0;
			}
		}
	}
	for (int p = piece_lines.size(); p > k; p--) {
		int flag = 0;
		for (int h = 0; h < sub_piece_line.size(); h++) {
			if (p == h) {
				flag = 1;
			}
			if (flag == 1) {
				frame->line_lengths.push_back(piece_lines[p]);
				flag = 0;
			}
		}
	}

	//��������g�̏���pushback
	//���X�V
	//1�s�[�X�������o���ĂȂ�
	
	if (i == 0) {
		double test = *frame_lines[j] - *piece_lines[k];
		shared_ptr<double> diff(&test);
		frame->line_lengths.push_back(diff);
	}

	for (int p = 0; p < sub_piece_line.size(); p++) {
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
			if (frame_lines.size() == p) {
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
	//�g�ƃs�[�X�̒��_���W������Ă���
	//��ň���i��n��
	com_piece com = combination_angles[0];
	/*com_piece com = combination_angles[i];*/
	vector<shared_ptr<cv::Point> > frame_vertex = frame->vertex;
	vector<shared_ptr<cv::Point> > piece_vertex = pieces[com.num_piece]->vertex;

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

void PieceManager::line_search() {


}