# include "AnalysisGraphics.h"

void Input(int event, int x, int y, int flag, void *param = NULL);
	int anscount = 0;
	int counter = 0;
	cv::Point vertex[32][50];
int main() {
	/*int i = 2;
	string hoge = "item/";
	hoge += "frame";
	hoge += ".png";
	//hoge += "(";
	//hoge += (char)('0' + i);
	//hoge += ")";
	cout << hoge << endl;



	cv::Mat im = cv::imread(hoge, 1);
	if (!im.data) {
		return -1;
	}
	imshow("�Ă���", im);


	/*cv::Mat im = cv::imread("item/piece (1).png", 0);
	cv::Mat color_im = cv::imread("item/piece (1).png", 1);
	cv::Mat im_canny, im_sobel, im_laplacian;
	cv::Mat img = cv::Mat::zeros(600, 600, CV_8UC3);*/

	int number = 0;
	cv::Mat piece[50];
	cout << "�s�[�X�̐������" << endl;
	cin >> number;
	string imgplece;
	string imgname;
	for (int i = 0; i < number; i++) {
		imgplece = "item/piece";
		imgplece += " (";
		imgplece += (char)('1' + i);
		imgplece += ").png";
		imgname = "piece";
		imgname += (char)('1' + i);
		piece[i] = cv::imread(imgplece,-1);
		if (!piece[i].data) {
			return -1;
		}
		imshow(imgname, piece[i]);
		counter = i;
		cvSetMouseCallback(imgname.c_str(), Input);
		cout << endl;
		anscount = 0;

		cvWaitKey(0);
		cvDestroyWindow(imgname.c_str());
	}
	int j = 0, k = 0, z = 0;
	double root = 0;
	int line_end = 0;
	double naiseki[50];
	double naiseki_x[50];
	double naiseki_y[50];
	double sen[50];
	double angle[32];
	double pie = 3.141592;

	int line_size;
	/*for (int i = 0; i < number; i++) {
		counter = i;
		imgname = "piece";
		imgname += (char)('1' + i);
		cvSetMouseCallback(imgname.c_str(), Input);

		cvWaitKey(0);
	}*/
	/*measureVertex(anscount, ten, ans, line_end);

	//�V��������o�������_�̏o��
	for (int i = 0; i < anscount; i++) {
		cout << ans[i] << endl;
	}*/


	/*measureLine(anscount, vertex, root, sen);



	measureAngle(anscount, vertex, naiseki, angle, naiseki_x, naiseki_y, sen, pie);



	remakeFigure(anscount, img, vertex);

	imshow("original", im);
	imshow("�͂�", color_im);
	imshow("�Đ���", img);*/

	cout << "end" << endl;
	cv::waitKey(0);

}

//���_�̊���o��
void measureVertex(int &anscount, cv::Point ten[], cv::Point ans[], int line_end) {
	int standard = 0;
	int count = 0;
	anscount = 0;
	for (int j = 0; j < 1600; j++) {
		for (int x = 0; x < line_end; x++) {
			if (ten[standard].x + j >= ten[x].x && ten[standard].x - j <= ten[x].x && ten[standard].y + j >= ten[x].y && ten[standard].y - j <= ten[x].y && ten[standard] != ten[x]) {
				if (j <= 5) {
					ans[anscount] = (ten[standard] + ten[x]) / 2;
					cout << "!";
					anscount++;
				}
				cout << ten[standard] << ten[x] << endl;
				if (x % 2 == 1) {
					standard = x - 1;
				}
				else {
					standard = x + 1;
				}
				count++;
				j = 0;
				break;
			}
		}
		if (count == line_end / 2) {
			break;
		}
	}
	for (int i = 0; i < anscount; i++) {
		for (int j = 0; j < anscount - i; j++) {
			if (ans[i] == ans[j] && i != j) {
				ans[j].x = 0;
				ans[j].y = 0;
				anscount = j;
			}
		}
	}
}

//���_����ӂ̊���o��
void measureLine(int anscount, cv::Point vertex[], double &root, double sen[]) {
	for (int i = 0; i < anscount; i++) {
		if (i == anscount - 1) {
			root = (vertex[i].x - vertex[0].x)*(vertex[i].x - vertex[0].x) + (vertex[i].y - vertex[0].y)*(vertex[i].y - vertex[0].y);
			sen[i] = sqrt(root);
		}
		else {
			root = (vertex[i].x - vertex[i + 1].x)*(vertex[i].x - vertex[i + 1].x) + (vertex[i].y - vertex[i + 1].y)*(vertex[i].y - vertex[i + 1].y);
			sen[i] = sqrt(root);
		}
	}
	//�ӂ̏o��
	for (int i = 0; i < anscount; i++) {
		cout << sen[i] << endl;
	}
}


void measureAngle(int anscount, cv::Point vertex[], double naiseki[], double angle[], double naiseki_x[], double naiseki_y[], double sen[], double pie) {
	double sum = 0;
	int binary[50] = { 0 };
	for (int i = 0; i < anscount; i++) {
		if (i == anscount - 1) {
			naiseki_x[i] = (vertex[0].x - vertex[i].x) * (vertex[0].x - vertex[1].x);
			naiseki_y[i] = (vertex[0].y - vertex[i].y) * (vertex[0].y - vertex[1].y);
			naiseki[i] = naiseki_x[i] + naiseki_y[i];
			angle[i] = naiseki[i] / (sen[i] * sen[0]);
			angle[i] = acos(angle[i]);
			angle[i] = angle[i] * 180.0 / pie;
		}
		else if (i == anscount - 2) {
			naiseki_x[i] = (vertex[i + 1].x - vertex[i].x) * (vertex[i + 1].x - vertex[0].x);
			naiseki_y[i] = (vertex[i + 1].y - vertex[i].y) * (vertex[i + 1].y - vertex[0].y);
			naiseki[i] = naiseki_x[i] + naiseki_y[i];
			angle[i] = naiseki[i] / (sen[i] * sen[i + 1]);
			angle[i] = acos(angle[i]);
			angle[i] = angle[i] * 180.0 / pie;
		}
		else {
			naiseki_x[i] = (vertex[i + 1].x - vertex[i].x) * (vertex[i + 1].x - vertex[i + 2].x);
			naiseki_y[i] = (vertex[i + 1].y - vertex[i].y) * (vertex[i + 1].y - vertex[i + 2].y);
			naiseki[i] = naiseki_x[i] + naiseki_y[i];
			angle[i] = naiseki[i] / (sen[i] * sen[i + 1]);
			angle[i] = acos(angle[i]);
			angle[i] = angle[i] * 180.0 / pie;
		}
	}
	cout << endl;

	int roop = pow(2, anscount);
	for (int i = 0; i < roop; i++) {
		//���]���K�v�Ȃ����break�B
		if ((anscount - 2) * 180 < sum + 3 && (anscount + 2) * 180 > sum + 3) {
			break;
		}
		//���]�̃p�^�[����S���ԗ����邽�߂̏���
		int memory = i;
		for (int j = 0; j < anscount; j++) {
			binary[j] = 0;
		}
		for (int j = anscount - 1; j >= 0; j--) {
			int divison = pow(2, j);
			if (memory / divison >= 1) {
				binary[j] = 1;
				memory = memory - divison;
			}
			cout << binary[j];
		}
		//���]�����鏈��
		for (int j = 0; j < anscount; j++) {
			if (binary[j] == 1) {
				angle[j] = 360 - angle[j];
			}
		}
		//���]�������̂��������킹��
		sum = 0;
		for (int j = 0; j < anscount; j++) {
			sum = sum + angle[j];
		}
		//���]���Ă��ꂪ�����Ă��邩�m�F���鏈���B�덷��+-3���B
		if ((anscount - 2) * 180 < sum + 3 && (anscount + 2) * 180 > sum + 3) {
			cout << endl;
			break;
		}
		//������Ƃ����]�������̂����ɖ߂�
		for (int j = 0; j < anscount; j++) {
			if (binary[j] == 1) {
				angle[j] = 360 - angle[j];
			}
		}
		cout << endl;
	}
	for (int i = 0; i < anscount; i++) {
		static double data = angle[0];
		if (i == anscount - 1) {
			angle[i] = data;
		}
		else {
			angle[i] = angle[i + 1];
		}
	}
	//�p�x�̏o��
	for (int i = 0; i < anscount; i++) {
		cout << angle[i] << endl;
	}
}

//���͂𐔒l�Ƃ��đ������A�\������
void Input(int event, int x, int y, int flags, void *param){
	switch (event) {
	case CV_EVENT_LBUTTONDOWN:
		vertex[anscount][counter].x = x;
		vertex[anscount][counter].y = y;
		cout << vertex[anscount][counter] << ",";
		anscount++;
		cout << anscount << endl;
		break;
	case CV_EVENT_RBUTTONDOWN:
		anscount--;
		break;
	default:
		break;
	}
}

void remakeFigure(int anscount, cv::Mat &img, cv::Point ans[]) {
	//�}�`�̍Č`��
	for (int i = 0; i < anscount; i++) {
		if (i != anscount - 1) {
			line(img, cv::Point(ans[i].x, ans[i].y), cv::Point(ans[i + 1].x, ans[i + 1].y), cv::Scalar(0, 0, 255), 3, 8);
		}
		else {
			line(img, cv::Point(ans[i].x, ans[i].y), cv::Point(ans[0].x, ans[0].y), cv::Scalar(0, 0, 255), 3, 8);
		}
	}
}