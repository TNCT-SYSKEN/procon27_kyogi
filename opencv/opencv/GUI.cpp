# include "GUI.h"

// �O���[�o���ϐ�
bool moving = false;

GUI::GUI() {
	img = cv::Mat(cv::Size(960, 540), CV_8UC3, cv::Scalar(100, 100, 100));
}

// �R�[���o�b�N�֐�
void my_mouse_callback(int event, int x, int y, int flags, void* param) {
	cv::Mat* image = static_cast<cv::Mat*>(param);

	switch (event) {
	case cv::EVENT_MOUSEMOVE:
		if (moving) {
		}
		break;

	case cv::EVENT_LBUTTONDOWN:
		moving = true;
		break;

	case cv::EVENT_LBUTTONUP:
		moving = false;
		break;
	}
}

void GUI::draw(vector<shared_ptr<Piece> > pieces, shared_ptr<cv::Mat> frame) {
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
			cv::warpAffine(*p, temp, mat, temp.size(), CV_INTER_LINEAR, cv::BORDER_TRANSPARENT);
		}

		cv::imshow("GUI", temp);

		// Esc�ŏI��
		if (cv::waitKey(15) == 27)
			break;
	}


	//cv::imshow("GUI", img);
}
