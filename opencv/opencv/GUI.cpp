# include "GUI.h"

// �O���[�o���ϐ�
bool moving = false;
cv::Point mouse;
vector<shared_ptr<Piece> > pieces;
shared_ptr<Piece> piece = nullptr;

GUI::GUI() {
	img = cv::Mat(cv::Size(960, 540), CV_8UC3, cv::Scalar(100, 100, 100));
}

// �R�[���o�b�N�֐�
void my_mouse_callback(int event, int x, int y, int flags, void* param) {
	cv::Mat* image = static_cast<cv::Mat*>(param);

	switch (event) {
	case cv::EVENT_MOUSEMOVE:
		if (moving) {
			//Mouse�̈ʒu�����s�[�X�ɑ��
			piece->adr.x = x;
			piece->adr.y = y;
		}
		break;

	case cv::EVENT_LBUTTONDOWN:
		moving = true;
		for (int i = 0; i < pieces.size(); i++) {
			//Piece�̉摜�f�[�^���擾
			shared_ptr<cv::Mat> image = pieces[i]->image;
			//Piece�̈ʒu���擾
			cv::Point adr = pieces[i]->adr;
			//�N���b�N�����͈͓��ɂ��邩����
			if (x > adr.x && y > adr.y && x < adr.x + image->size().width && y < adr.y + image->size().height) {
				piece = pieces[i];
				break;
			}
			//�Ȃ����false
			if (i == pieces.size() - 1)	moving = false;
		}
		break;

	case cv::EVENT_LBUTTONUP:
		moving = false;
		piece = nullptr;
		break;
	}
}

void GUI::draw(vector<shared_ptr<Piece> > pie, shared_ptr<cv::Mat> frame) {
	pieces = pie;
	//�O�i�摜�̕ό`�s��

	//�N���[��
	cv::Mat temp = img.clone();

	// �E�B���h�E�𐶐�
	cv::namedWindow("GUI", CV_WINDOW_AUTOSIZE);

	// �R�[���o�b�N��ݒ�
	cv::setMouseCallback("GUI", my_mouse_callback, (void *)&img);

	// Main loop
	while (1) {
		// image��temp�ɃR�s�[
		img.copyTo(temp);

		//																								���s�ړ��̗�
		cv::Mat mat = (cv::Mat_<double>(2, 3) << 1.0, 0.0, 100, 0.0, 1.0, 100);
		cv::warpAffine(*frame, temp, mat, temp.size(), CV_INTER_LINEAR, cv::BORDER_TRANSPARENT);
		
		for (int i = 0; i < pieces.size(); i++) {
			shared_ptr<cv::Mat> p = pieces[i]->image;
			mat = (cv::Mat_<double>(2, 3) << 1.0, 0.0, pieces[i]->adr.x, 0.0, 1.0, pieces[i]->adr.y);
			cv::warpAffine(*p, temp, mat, temp.size(), CV_INTER_LINEAR, cv::BORDER_TRANSPARENT);
		}

		if (moving) {
		}

		cv::imshow("GUI", temp);

		// Esc�ŏI��
		if (cv::waitKey(15) == 27)
			break;
	}


	//cv::imshow("GUI", img);
}
