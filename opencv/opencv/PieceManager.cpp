# include "PieceManager.h"

# include "Piece.h"

# include "Frame.h"

# include "fstream"

#define gosa 1

PieceManager::PieceManager() {
	pieces = vector<shared_ptr<Piece> >(); //ピ�Eスの惁E��
}

void PieceManager::init_pieces(vector<shared_ptr<cv::Mat> > images, shared_ptr<Frame> f) {
	for (int i = 0; i < images.size(); i++) {
		pieces.push_back(make_shared<Piece>(Piece(images[i], i)));
	}
	//frame = f;
}

//�����蔻��@�p�x���n���Ďg���Ă�������
bool PieceManager::hit_judge(cv::Point edge1, cv::Point edge2) {

	vector<shared_ptr<double>> angle1;
	vector<shared_ptr<double>> angle2;
	angle1.push_back(make_shared<double>());	//角度のチE�Eタ
	angle2.push_back(make_shared<double>());	//角度のチE�Eタ
	angle1 = pieces[edge1.y]->get_angle();
	angle2 = pieces[edge2.y]->get_angle();
	if (*angle1[edge1.x] + *angle2[edge2.x] > 180) {
		cout << "false " << *angle1[edge1.x] + *angle2[edge2.x] << endl;
		return false;
	}
	else {
		cout << "true " << *angle1[edge1.x] + *angle2[edge2.x] << endl;
		return true;
	}

}

//�ӂ̒����ƍ����p�^�[���̊���o���@������n���Ďg���Ă�������
void PieceManager::sertch_line(double to_find_line) {

	//�g�ݍ��킹���s�[�X���i�[����
	vector<vector<shared_ptr<double> > > marged_line;
	vector<vector<shared_ptr<double> > > line_clone;
	//��x�ʂ����Ƃ�����ӂ������߂̕ϐ�
	vector<vector<shared_ptr<double>>> stack_deadend;
	vector<shared_ptr<double> > line_once;
	shared_ptr<double> line_min;
	int test = 0;
	int flag = 0;
	int sameflag = 0;
	int limitflag = 0;
	int sp = 0;
	int reject = 1;
	stack<shared_ptr<double>> line_stack;
	//��x�ʂ����p�^�[�����L�^���Ă����ϐ�


	//�񎟌��z���clone�Ƀp�[�c�̕ӂ̑S�����i�[
	for (int i = 0; i < pieces.size(); i++) {
		line_once.push_back(shared_ptr<double>());
		line_once = pieces[i]->get_line_lengths();
		line_clone.push_back(vector<shared_ptr<double> >());
		line_clone[i] = line_once;
	}

	//��ԒZ���ӂ����߂�B��Ŏg��
	line_min = line_clone[0][0];
	for (int i = 0; i < line_clone.size(); i++){
		for (int j = 0; j < line_clone[i].size(); j++) {
			if (*line_min > *line_clone[i][j]) {
				line_min = line_clone[i][j];
			}
		}

	}



	//push������to_find_line����push�����ӂ̕��̒����������̂�Y�ꂸ��

	//�[���D��T���Ŏ���
	line_stack.push(line_clone[0][0]);
	stack_deadend.push_back(vector<shared_ptr<double> >());
	marged_line.push_back(vector<shared_ptr<double> >());

	while (!line_stack.empty()) {
		for (int i = 0; i < line_clone.size(); i++) {
			sameflag = 0;
			for (int j = 0; j < line_clone[i].size(); j++) {
			//�����}�`�̕ӂ��܂܂�Ȃ��悤�ɂ���
				/*for (int k = 0; k + 1 < stack_deadend[stack_deadend.size() - 1].size(); k++) {
					for (int l = 0; l + 1 < line_clone[i].size(); l++) {
						if (stack_deadend[stack_deadend.size() - 1][k] == line_clone[i][l]) {
							sameflag = 1;
						}
					}
				}
				if (sameflag == 1) {
					break;
				}*/
				for (int x = 0; x < line_clone[i].size(); x++) {
					for (int y = 0; y < stack_deadend[stack_deadend.size() - 1].size(); y++) {
						if (line_clone[i][x] == stack_deadend[stack_deadend.size() - 1][y]) {
							sameflag = 1;
						}
						if (sameflag == 1) {
							break;
						}
					}
					if (sameflag == 1) {
						break;
					}
				}
				if (sameflag == 1) {
					break;
				}


			//��x�ʂ����Ƃ���ɂ�����Ȃ��悤��
				limitflag = 0;
				for (int k = 0; k + 1 < stack_deadend.size(); k++) {
					stack_deadend[stack_deadend.size() - 1].push_back(line_clone[i][j]);

					if (stack_deadend[stack_deadend.size() - 1] == stack_deadend[k]) {
						limitflag = 1;
					}
					stack_deadend[stack_deadend.size() - 1].pop_back();
				}

				//for���̈ȉ��̏��������ׂĂ����Ƃ΂�
				if (limitflag == 1) {
					continue;
				}
				
				//to_find_line�ƌ덷��-10�܂ł̕ӂ�push
				if (*line_clone[i][j]<= to_find_line && *line_clone[i][j] >= to_find_line - 5) {
					line_stack.push(line_clone[i][j]);
					marged_line[marged_line.size() - 1].push_back(line_clone[i][j]);
					to_find_line -= *line_clone[i][j]; //�Ђ�
					stack_deadend[stack_deadend.size() - 1].push_back(line_clone[i][j]);
					flag = 1;
				}
				//���߂����ӂ̒������Z�����̂�push�������c��̕ӂ��ŏ��̕ӂ�菬�����Ȃ�Ȃ��悤��
				else if (*line_clone[i][j] < to_find_line && to_find_line - *line_clone[i][j] >= *line_min) {
					line_stack.push(line_clone[i][j]);
					marged_line[marged_line.size() - 1].push_back(line_clone[i][j]);
					to_find_line -= *line_clone[i][j];
					stack_deadend[stack_deadend.size() - 1].push_back(line_clone[i][j]);
					flag = 1;
				}
				if (flag == 1) {
					break;
				}
			}
			if (flag == 1) {
				break;
			}
		}

		//�K����ӂ��������Ă��Ȃ����flag��0�̂͂�(����ȏ����Ȃ�)
		if (flag == 0) {
			//���̍s��
			stack_deadend.push_back(vector < shared_ptr<double> > ());
			for (int i = 0; i < stack_deadend[stack_deadend.size() - 2].size(); i++) {
				stack_deadend[stack_deadend.size() - 1].push_back(stack_deadend[stack_deadend.size() - 2][i]);
			}
			stack_deadend[stack_deadend.size() - 1].pop_back();
			marged_line[marged_line.size() - 1].pop_back();
			to_find_line += *line_stack.top(); //����
			line_stack.pop(); //�Ȃ���
		}

		flag = 0;
		//���߂����ӂ̒�����10�ȉ��������Ƃ��ɓ����Ƃ݂Ȃ���pushback
		if (to_find_line <= 5) {
			marged_line.push_back(vector<shared_ptr<double> >());
			for (int i = 0; i < marged_line[marged_line.size() - 2].size(); i++) {
				marged_line[marged_line.size() - 1].push_back(marged_line[marged_line.size() - 2][i]);
			}
			marged_line[marged_line.size() - 1].pop_back();
			stack_deadend.push_back(vector < shared_ptr<double> >());
			for (int i = 0; i < stack_deadend[stack_deadend.size() - 2].size(); i++) {
				stack_deadend[stack_deadend.size() - 1].push_back(stack_deadend[stack_deadend.size() - 2][i]);
			}
			stack_deadend[stack_deadend.size() - 1].pop_back();
			to_find_line += *line_stack.top();
			line_stack.pop();
		}
	}
	for (int i = 0; i < stack_deadend.size(); i++) {
		for (int j = 0; j < stack_deadend[i].size(); j++) {
			cout << *stack_deadend[i][j] << " ";
		}
		cout << endl;
	}
	for (int i = 0; i < marged_line.size(); i++) {
		for (int j = 0; j < marged_line[i].size(); j++) {
			cout << *marged_line[i][j] << " ";
		}
		cout << endl;
	}
}

void PieceManager::sertch_edge(vector<shared_ptr<double> > frame) {
	vector<vector<shared_ptr<double> > > edge_clone;
	vector<shared_ptr<double> > frame_edge;
	shared_ptr<double> nearest_edge = 0;
	double edge_sourse;
	
	for (int i = 0; i < pieces.size(); i++) {
		edge_clone.push_back(pieces[i]->get_angle());
	}
	for (int i = 0; i < frame.size(); i++) {
		frame_edge.push_back(frame[i]);
	}
	for (int i = 0; i < edge_clone.size(); i++) {
		for (int j = 0; j < edge_clone[i].size(); j++) {
			cout << *edge_clone[i][j] << " ";
		}
		cout << endl;
	}
	for (int i = 0; i < frame.size(); i++) {
		cout << *frame_edge[i] << endl;
	}
	cout << endl;
	//�g�̊p�̃T�C�Y�Ɉ�ԋ߂��p�������鏈��
	edge_sourse = abs(*frame_edge[0] - *edge_clone[0][0]);
	for (int i = 0; i < edge_clone.size(); i++) {
		for (int j = 0; j < edge_clone[i].size(); j++) {
			if (*edge_clone[i][j] < *frame_edge[0] + edge_sourse && *edge_clone[i][j] > *frame_edge[0] - edge_sourse){
				edge_sourse = abs(*frame_edge[0] - *edge_clone[i][j]);
				nearest_edge = edge_clone[i][j];
				cout << *edge_clone[i][j] << endl;
			}
		}
	}
	cout << endl;
	cout << edge_sourse << endl;
}
void PieceManager::Frame_update() {
	
}

void PieceManager::Frame_record() {

}

void PieceManager::answer_draw() {

}

void PieceManager::exec_algorithm() {
	search_angle();
	put_image();
}

void PieceManager::search_angle() {
	//�t���[���̊p�x
	vector<shared_ptr<double> > frame_angles = frame->get_angle();

	for (int i = 0; i < frame_angles.size(); i++) {
		for (int j = 0; j < pieces.size(); j++) {
			//�s�[�X��̊p�x
			vector<shared_ptr<double> > angles = pieces[j]->get_angle();
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

void PieceManager::put_image() {
	//�g�ƃs�[�X�̒��_���W������Ă���
	com_piece com = combination_angles[0];
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



void PieceManager::marge_piece() {
	vector<vector<shared_ptr<double> > > line_clone;
	vector<vector<shared_ptr<double> > > edge_clone;
	int wait_count = 0;
	ofstream outputfile("hoge.txt");

	for (int i = 0; i < pieces.size(); i++) {
		edge_clone.push_back(pieces[i]->get_angle());
	}

	for (int i = 0; i < pieces.size(); i++) {
		line_clone.push_back(pieces[i]->get_line_lengths());
	}
	//������360�ɋ߂��s�[�X��T��
	for (int i = 0; i < edge_clone.size(); i++) {
		for (int j = 0; j < edge_clone[i].size(); j++) {
			for (int k = 0; k < edge_clone.size(); k++) {
				for (int l = 0; l < edge_clone[k].size(); l++) {
					if (*edge_clone[i][j] + *edge_clone[k][l] < 360 + 3 && *edge_clone[i][j] + *edge_clone[k][l] > 360 - 3 && k > i) {
						int flag1 = 0;
						int flag2 = 0;
						//cout << *edge_clone[i][j] << "," << *edge_clone[k][l] << "," << *edge_clone[i][j] + *edge_clone[k][l] << endl;
						if (j == 0) {
							if (l == 0) {
								if (*line_clone[i][j] - *line_clone[k][l] < gosa && *line_clone[i][j] - *line_clone[k][l] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][line_clone[i].size() - 1] - *line_clone[k][line_clone[k].size() - 1] < gosa && *line_clone[i][line_clone[i].size() - 1] - *line_clone[k][line_clone[k].size() - 1] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][line_clone[i].size() - 1] - *line_clone[k][l] < gosa && *line_clone[i][line_clone[i].size() - 1] - *line_clone[k][l] > -gosa) {
									flag2++;
								}
								if (*line_clone[i][j] - *line_clone[k][line_clone[k].size() - 1] < gosa && *line_clone[i][j] - *line_clone[k][line_clone[k].size() - 1] > -gosa) {
									flag2++;
								}

								if (flag1 == 2 || flag2 == 2) {
									wait_count++;
									cout << i + 1 << "�Ԃ�" << k + 1 << "�Ԃ̃s�[�X��360�ł����������ł�" << endl << endl;
									outputfile << i + 1 << "��," << k + 1 << "��, 180" << endl << endl;
								}
							}
							else {
								if (*line_clone[i][j] - *line_clone[k][l] < gosa && *line_clone[i][j] - *line_clone[k][l] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][line_clone[i].size() - 1] - *line_clone[k][l - 1] < gosa && *line_clone[i][line_clone[i].size() - 1] - *line_clone[k][l - 1] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][line_clone[i].size() - 1] - *line_clone[k][l] < gosa && *line_clone[i][line_clone[i].size() - 1] - *line_clone[k][l] > -gosa) {
									flag2++;
								}
								if (*line_clone[i][j] - *line_clone[k][l - 1] < gosa && *line_clone[i][j] - *line_clone[k][l - 1] > -gosa) {
									flag2++;
								}

								if (flag1 == 2 || flag2 == 2) {
									wait_count++;
									cout << i + 1 << "�Ԃ�" << k + 1 << "�Ԃ̃s�[�X��360�ł����������ł�" << endl << endl;
									outputfile << i + 1 << "��," << k + 1 << "��, 180" << endl << endl;
								}
							}
						}
						else {
							if (l == 0) {
								if (*line_clone[i][j] - *line_clone[k][l] < gosa && *line_clone[i][j] - *line_clone[k][l] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][j - 1] - *line_clone[k][line_clone[k].size() - 1] < gosa && *line_clone[i][j - 1] - *line_clone[k][line_clone[k].size() - 1] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][j - 1] - *line_clone[k][l] < gosa && *line_clone[i][j - 1] - *line_clone[k][l] > -gosa) {
									flag2++;
								}
								if (*line_clone[i][j] - *line_clone[k][line_clone[k].size() - 1] < gosa && *line_clone[i][j] - *line_clone[k][line_clone[k].size() - 1] > -gosa) {
									flag2++;
								}

								if (flag1 == 2 || flag2 == 2) {
									wait_count++;
									cout << i + 1 << "�Ԃ�" << k + 1 << "�Ԃ̃s�[�X��360�ł����������ł�" << endl << endl;
									outputfile << i + 1 << "��," << k + 1 << "��, 180" << endl << endl;
								}
							}
							else {
								if (*line_clone[i][j] - *line_clone[k][l] < gosa && *line_clone[i][j] - *line_clone[k][l] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][j - 1] - *line_clone[k][l - 1] < gosa && *line_clone[i][j - 1] - *line_clone[k][l - 1] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][j - 1] - *line_clone[k][l] < gosa && *line_clone[i][j - 1] - *line_clone[k][l] > -gosa) {
									flag2++;
								}
								if (*line_clone[i][j] - *line_clone[k][l - 1] < gosa && *line_clone[i][j] - *line_clone[k][l - 1] > -gosa
									) {
									flag2++;
								}

								if (flag1 == 2 || flag2 == 2) {
									wait_count++;
									cout << i + 1 << "�Ԃ�" << k + 1 << "�Ԃ̃s�[�X��360�ł����������ł�" << endl << endl;
									outputfile << i + 1 << "��," << k + 1 << "��, 180" << endl << endl;
								}
							}
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < edge_clone.size(); i++) {
		for (int j = 0; j < edge_clone[i].size(); j++) {
			for (int k = 0; k < edge_clone.size(); k++) {
				for (int l = 0; l < edge_clone[k].size(); l++) {
					if (*edge_clone[i][j] + *edge_clone[k][l] < 180 + 3 && *edge_clone[i][j] + *edge_clone[k][l] > 180 - 3 && k > i) {
						int flag1 = 0;
						int flag2 = 0;
						//cout << *edge_clone[i][j] << "," << *edge_clone[k][l] << "," << *edge_clone[i][j] + *edge_clone[k][l] << endl;
						if (j == 0) {
							if (l == 0) {
								if (*line_clone[i][j] - *line_clone[k][l] < gosa && *line_clone[i][j] - *line_clone[k][l] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][line_clone[i].size() - 1] - *line_clone[k][line_clone[k].size() - 1] < gosa && *line_clone[i][line_clone[i].size() - 1] - *line_clone[k][line_clone[k].size() - 1] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][line_clone[i].size() - 1] - *line_clone[k][l] < gosa && *line_clone[i][line_clone[i].size() - 1] - *line_clone[k][l] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][j] - *line_clone[k][line_clone[k].size() - 1] < gosa && *line_clone[i][j] - *line_clone[k][line_clone[k].size() - 1] > -gosa) {
									flag1++;
								}

								if (flag1 >= 1) {
									wait_count++;
									cout << i + 1 << "�Ԃ�" << k + 1 << "�Ԃ̃s�[�X��180�ł����������ł�" << endl << endl;
									outputfile << i + 1 << "��," << k + 1 << "��, 180" << endl << endl;
								}
							}
							else {
								if (*line_clone[i][j] - *line_clone[k][l] < gosa && *line_clone[i][j] - *line_clone[k][l] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][line_clone[i].size() - 1] - *line_clone[k][l - 1] < gosa && *line_clone[i][line_clone[i].size() - 1] - *line_clone[k][l - 1] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][line_clone[i].size() - 1] - *line_clone[k][l] < gosa && *line_clone[i][line_clone[i].size() - 1] - *line_clone[k][l] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][j] - *line_clone[k][l - 1] < gosa && *line_clone[i][j] - *line_clone[k][l - 1] > -gosa) {
									flag1++;
								}

								if (flag1 >= 1) {
									wait_count++;
									cout << i + 1 << "�Ԃ�" << k + 1 << "�Ԃ̃s�[�X��180�ł����������ł�" << endl << endl;
									outputfile << i + 1 << "��," << k + 1 << "��, 180" << endl << endl;
								}
							}
						}
						else {
							if (l == 0) {
								if (*line_clone[i][j] - *line_clone[k][l] < gosa && *line_clone[i][j] - *line_clone[k][l] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][j - 1] - *line_clone[k][line_clone[k].size() - 1] < gosa && *line_clone[i][j - 1] - *line_clone[k][line_clone[k].size() - 1] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][j - 1] - *line_clone[k][l] < gosa && *line_clone[i][j - 1] - *line_clone[k][l] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][j] - *line_clone[k][line_clone[k].size() - 1] < gosa && *line_clone[i][j] - *line_clone[k][line_clone[k].size() - 1] > -gosa) {
									flag1++;
								}

								if (flag1 >= 1) {
									wait_count++;
									cout << i + 1 << "�Ԃ�" << k + 1 << "�Ԃ̃s�[�X��180�ł����������ł�" << endl << endl;
									outputfile << i + 1 << "��," << k + 1 << "��, 180" << endl << endl;
								}
							}
							else {
								if (*line_clone[i][j] - *line_clone[k][l] < gosa && *line_clone[i][j] - *line_clone[k][l] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][j - 1] - *line_clone[k][l - 1] < gosa && *line_clone[i][j - 1] - *line_clone[k][l - 1] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][j - 1] - *line_clone[k][l] < gosa && *line_clone[i][j - 1] - *line_clone[k][l] > -gosa) {
									flag1++;
								}
								if (*line_clone[i][j] - *line_clone[k][l - 1] < gosa && *line_clone[i][j] - *line_clone[k][l - 1] > -gosa) {
									flag1++;
								}

								if (flag1 >= 1) {
									wait_count++;
									cout << i + 1 << "�Ԃ�" << k + 1 << "�Ԃ̃s�[�X��180�ł����������ł�" << endl << endl;
									outputfile << i + 1 << "��," << k + 1 << "��, 180" << endl << endl;
								}
							}
						}
					}
				}
			}
		}
	}
	outputfile.close();

}

void PieceManager::line_dt(vector<shared_ptr<double> > frame_angle,vector<shared_ptr<double> > frame_line) {
	int a = 0;
	int c = 0;
	vector<vector<pair<double, double> > > line_pt; //�����̑g�ݍ��킹�Řg�̕ӂ̒����ɍ������̂��L�^
	vector<vector<pair<double, double > > > angle_pt; //�����̑g�ݍ��킹�Řg�̊p�x�ɍ������̂��L�^
	vector<vector<shared_ptr<double> > > line_length;
	vector<vector<shared_ptr<double> > > angle;

	for (int i = 0; i < pieces.size(); i++) {
		line_length.push_back(pieces[i]->get_line_lengths());
		angle.push_back(pieces[i]->get_angle());
	}
	
	/*cout << endl << "hogehoge" << endl;
	for (int i = 0; i < line_length.size(); i++) {
		for (int j = 0; j < line_length[i].size(); j++) {
			cout << "hoge" << *line_length[i][j] << " , ";
		}
		cout << endl;
	}

	for (int i = 0; i < angle.size();i++) {
		for (int j = 0; j < angle[i].size(); j++) {
			cout << "hogeen" << *angle[i][j] << " , ";
		}
		cout << endl;
	}

	for (int i = 0; i < frame_line.size(); i++) {
		cout << "hogegen?" << *frame_line[i] << " , ";
	}*/

	for (int x = 0;x < frame_line.size();x++) {
		angle_pt.push_back(vector<pair<double, double> >());
		for (int i = 0; i < pieces.size(); i++) {
			for (int j = 0; j < angle[i].size(); j++) {
				if (*angle[i][j] < *frame_angle[x] + gosa && *angle[i][j] > *frame_angle[x] - gosa) {
					cout << "angle[" << i << "][" << j << "]�@�@�@�@�@�@�@ �p" << *angle[i][j] << "�ł��B" << endl;
					angle_pt.push_back(vector<pair<double, double> >());
					angle_pt[a].push_back(make_pair(i, j)); //�s�[�X�P�̂Řg�̊p�x�ƈ�v������̂�ێ�
					a++;
				}
				for (int k = i + 1; k < pieces.size(); k++) {
					for (int g = 0; g < angle[k].size(); g++) {
						if (*angle[i][j] < 180 - *angle[k][g] + gosa && *angle[i][j] > 180 - *angle[k][g] - gosa) {
							cout << "���킹��180�ɂȂ�p�x�̑g�ݍ��킹�� pair[" << i << "][" << j
								<< "]" << *angle[i][j] << " & [" << k << "][" << g << "]" << *angle[k][g] << endl;
						}
						else if (*angle[i][j] < 90 - *angle[k][g] + gosa && *angle[i][j] > 90 - *angle[k][g] - gosa) {
							cout << "���킹��90�ɂȂ�p�x�̑g�ݍ��킹�� pair[" << i << "][" << j
								<< "]" << *angle[i][j] << " & [" << k << "][" << g << "]" << *angle[k][g] << endl;
						}
						//�g�̕ӂ̒������炠�Ă͂߂��s�[�X�̗אڂ���ӂ̒����������������Ƃ��A��������v����ӂ����s�[�X�̒T��
						if (*frame_line[x] - *line_length[i][j] < *line_length[k][g] + gosa && *frame_line[x] - *line_length[i][j] > *line_length[k][g] - gosa) {
							cout << "pair[" << i << "][" << j << "]" << *line_length[i][j] << " & " <<
								"[" << k << "][" << g << "]" << *line_length[k][g] << endl;
							cout << j << ", " << line_length[i].size() << endl;
							if (j == line_length[i].size() - 1) {
								if (g == 0) {
									if (*line_length[i].front() < *line_length[k].back() + gosa && *line_length[i].front() > *line_length[k].back() - gosa) {
										cout << "�ӂ̑g�ݍ��킹�� [" << i << "][0]" << line_length[i].front() << " & [" << k << 
										"][" << line_length[k].size()-1 << "]" << *line_length[k].back() << endl;
									}
								}
								else if(g == line_length[i].size() - 1) {
									if (*line_length[i].front() < *line_length[k][g - 1] + gosa && *line_length[i].front() > *line_length[k][g - 1] - gosa) {
										cout << endl << "�ӂ̑g�ݍ��킹�� [" << i << "][0]" << *line_length[i].front() <<
										" & [" << k << "][" << g - 1 << "]" << *line_length[k][g - 1] << endl;
									}
								}
								else {
									if (*line_length[i].front() < *line_length[k][g - 1] + gosa && *line_length[i].front() > *line_length[k][g - 1] - gosa) {
										cout << endl << "�ӂ̑g�ݍ��킹�� [" << i << "][0]" << *line_length[i].front() <<
											" & [" << k << "][" << g - 1 << "]" << *line_length[k][g - 1] << endl;
									}
									else if (*line_length[i][j - 1] < *line_length[k][g + 1] + gosa && *line_length[i][j - 1] > *line_length[k][g + 1] - gosa) {
										cout << endl << "�ӂ̑g�ݍ��킹�� [" << i << "][" << j - 1 << "]" << *line_length[i][j - 1] <<
											" & [" << k << "][" << g + 1 << "]" << *line_length[k][g + 1] << endl;
									}
								}
							}
							else {
								//cout << "�X�[�p�[" << endl;
								if (g == 0) {
									cout << endl << "hogearts" << endl;
									if (*line_length[i][j + 1] < *line_length[k].back() + gosa && *line_length[i][j + 1] > *line_length[k].back() - gosa) {
										cout << endl << "�ӂ̑g�ݍ��킹�� [" << i << "][" << j + 1 << "]" << *line_length[i][j + 1] 
											<< " & [" << k << "][" << line_length[k].size() - 1 << "]" << *line_length[k].back() << endl;
									}
								}
								else if(g = line_length[k].size()-1){
									cout << "hogegegegegegege" << endl;
									if (*line_length[i][j + 1] < *line_length[k][g - 1] + gosa && *line_length[i][j + 1] > *line_length[k][g - 1] - gosa) {
										cout << endl << "�ӂ̑g�ݍ��킹�� [" << i << "][" << j + 1 << "]" << *line_length[i][j + 1] 
										<< " & [" << k << "][" << g - 1 << "]" << *line_length[k][g - 1] << endl;
									}
								}
								else {
									if (*line_length[i][j - 1] < *line_length[k][g + 1] + gosa && *line_length[i][j - 1] > *line_length[k][g + 1] - gosa) {
										cout << endl << "�ӂ̑g�ݍ��킹�� [" << i << "][" << j - 1 << "]" << *line_length[i][j - 1]
											<< " & [" << k << "][" << g + 1 << "]" << *line_length[k][g + 1] << endl;
 									}
								}
							}
						}
						if (j == line_length[i].size() - 1) {
							if (g == 0) {
								if (line_length[i].front() == line_length[k].back()) {
									cout << endl << "hogehoge" << endl;
								}
							}
						}

<<<<<<< HEAD
						if (*angle[i][j] + *angle[k][g] < *frame_angle[x] + gosa && *angle[i][j] + *angle[k][g] > *frame_angle[x] - gosa) {
							cout << "angle[" << i << "][" << j << "] & " << "angle[" << k << "][" << g << "] " <<
								"�p" << *angle[i][j] << "��" << *angle[k][g] << "���Z�b�g�ł��B" << endl;
							angle_pt.push_back(vector<pair<double, double> >());
							angle_pt[a].push_back(make_pair(i, j));
							angle_pt[a].push_back(make_pair(k, g));
							a++;
						}
						else if (*angle[i][j] + *angle[k][g] < *frame_angle[x] - gosa) {
=======
						if (flag1 == 2) {
							cout << "���ꂭ�����܂�" << endl;
						}
						if (flag2 == 2) {
							cout << "���ꂭ������" << endl;
>>>>>>> origin/master
						}
					}
				}
			}
		}
	}

<<<<<<< HEAD
	/*for (int i = 0; i < a; i++) {
		cout << endl;
		for (int j = 0;j < angle_pt[i].size();j++) {
			cout << "[" << angle_pt[i][j].first << "][" << angle_pt[i][j].second << "] ";
		}
	}*/
}

/*while (dx) {
	angle_pt[a].push_back(make_pair(i, j));
	
}*/
=======
	//�s�[�X����������
	
>>>>>>> origin/master

