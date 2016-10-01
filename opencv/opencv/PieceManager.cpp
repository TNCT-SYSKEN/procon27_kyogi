# include "PieceManager.h"

# include "Piece.h"

PieceManager::PieceManager() {
	pieces = vector<shared_ptr<Piece> >(); //ピ�Eスの惁E��
}

void PieceManager::init_pieces(vector<shared_ptr<cv::Mat> > images) {
	for (int i = 0; i < images.size(); i++) {
		pieces.push_back(make_shared<Piece>(Piece(images[i], i)));
	}
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
	int sp_ex = 0;
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
				limitflag = 0;
			//�����}�`�̕ӂ��܂܂�Ȃ��悤�ɂ���
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
				sp = 0;
				for (int x = 0; x < stack_deadend.size() - 1; x++) {
					sp = 0;
					for (int y = 0; y < stack_deadend[x].size(); y++) {

						if (stack_deadend[x].size() == 1 || y == stack_deadend[x].size() - 1) {
							if (stack_deadend[x][y] == line_clone[i][j]) {
								sp++;
							}
						}
						
						else{
							if (stack_deadend[x][y] == stack_deadend[stack_deadend.size() - 1][y]) {
								sp++;
							}
						}
					}
					if (sp == stack_deadend[x].size()) {
						limitflag = 1;
					}
					if (limitflag == 1) {
						break;
					}
				}
				//for���̈ȉ��̏��������ׂĂ����Ƃ΂�
				if (limitflag == 1) {
					continue;
				}
				
				//to_find_line�ƌ덷��+-10�܂ł̕ӂ�push
				if (*line_clone[i][j]<= to_find_line && *line_clone[i][j] >= to_find_line - 10) {
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
		if (to_find_line <= 10) {
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