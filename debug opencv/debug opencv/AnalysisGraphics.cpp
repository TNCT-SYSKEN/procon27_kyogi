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

	int j = 0, k = 0, z = 0;
	double root = 0;
	int line_end = 0;
	double naiseki[50];
	double naiseki_x[50];
	double naiseki_y[50];
	double sen[32][50];
	double angle[32][50];
	double pie = 3.141592;

	int number = 0;
	cv::Mat piece[50];
	cout << "�s�[�X�̐������" << endl;
	cin >> number;
	string imgplece;
	string imgname;
	//Number of piece �̗�
	for (int nop = 0; nop < number; nop++) {
		imgplece = "item/piece";
		imgplece += " (";
		imgplece += (char)('1' + nop);
		imgplece += ").png";
		imgname = "piece";
		imgname += (char)('1' + nop);
		piece[nop] = cv::imread(imgplece,-1);
		if (!piece[nop].data) {
			return -1;
		}
		imshow(imgname, piece[nop]);
		counter = nop;
		cvSetMouseCallback(imgname.c_str(), Input);
		cout << endl;
		anscount = 0;

		cvWaitKey(0);
		cvDestroyWindow(imgname.c_str());

		measureLine(anscount, vertex, root, sen, nop);

		measureAngle(anscount, vertex, naiseki, angle, sen, pie, nop);
	}

	int line_size;
	/*for (int i = 0; i < number; i++) {
		counter = i;
		imgname = "piece";
		imgname += (char)('1' + i);
		cvSetMouseCallback(imgname.c_str(), Input);

		cvWaitKey(0);
	}*/
	//measureVertex(anscount, ten, ans, line_end);

	//�V��������o�������_�̏o��
	/*for (int i = 0; i < anscount; i++) {
		cout << ans[i] << endl;
	}*/


	//measureLine(anscount, vertex, root, sen);



	//measureAngle(anscount, vertex, naiseki, angle, naiseki_x, naiseki_y, sen, pie);



	//remakeFigure(anscount, img, vertex);

	//imshow("original", im);
	//imshow("�͂�", color_im);
	//imshow("�Đ���", img);

	for (int i = 0; i < number; i++) {
		imgname = "piece";
		imgname += (char)('1' + i);
		imshow(imgname, piece[i]);
	}

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
void measureLine(int anscount, cv::Point vertex[][50], double &root, double sen[][50], int nop) {
	for (int i = 0; i < anscount; i++) {
		if (i == anscount - 1) {
			root = (vertex[i][nop].x - vertex[0][nop].x)*(vertex[i][nop].x - vertex[0][nop].x) + (vertex[i][nop].y - vertex[0][nop].y)*(vertex[i][nop].y - vertex[0][nop].y);
			sen[i][nop] = sqrt(root);
		}
		else {
			root = (vertex[i][nop].x - vertex[i + 1][nop].x)*(vertex[i][nop].x - vertex[i + 1][nop].x) + (vertex[i][nop].y - vertex[i + 1][nop].y)*(vertex[i][nop].y - vertex[i + 1][nop].y);
			sen[i][nop] = sqrt(root);
		}
	}
	//�ӂ̏o��
	for (int i = 0; i < anscount; i++) {
		cout << sen[i][nop] << endl;
	}
}


void measureAngle(int anscount, cv::Point vertex[][50], double naiseki[], double angle[][50], double sen[][50], double pie, int nop) {
	double sum = 0;
	int binary[50] = { 0 };
	for (int i = 0; i < anscount; i++) {
		if (i == anscount - 1) {
			naiseki[i] = (vertex[0][nop].x - vertex[i][nop].x) * (vertex[0][nop].x - vertex[1][nop].x) + (vertex[0][nop].y - vertex[i][nop].y) * (vertex[0][nop].y - vertex[1][nop].y);
			angle[i][nop] = naiseki[i] / (sen[i][nop] * sen[0][nop]);
			angle[i][nop] = acos(angle[i][nop]);
			angle[i][nop] = angle[i][nop] * 180.0 / pie;
		}
		else if (i == anscount - 2) {
			naiseki[i] = (vertex[i + 1][nop].x - vertex[i][nop].x) * (vertex[i + 1][nop].x - vertex[0][nop].x) + (vertex[i + 1][nop].y - vertex[i][nop].y) * (vertex[i + 1][nop].y - vertex[0][nop].y);
			angle[i][nop] = naiseki[i] / (sen[i][nop] * sen[i + 1][nop]);
			angle[i][nop] = acos(angle[i][nop]);
			angle[i][nop] = angle[i][nop] * 180.0 / pie;
		}
		else {
			naiseki[i] = (vertex[i + 1][nop].x - vertex[i][nop].x) * (vertex[i + 1][nop].x - vertex[i + 2][nop].x) + (vertex[i + 1][nop].y - vertex[i][nop].y) * (vertex[i + 1][nop].y - vertex[i + 2][nop].y);
			angle[i][nop] = naiseki[i] / (sen[i][nop] * sen[i + 1][nop]);
			angle[i][nop] = acos(angle[i][nop]);
			angle[i][nop] = angle[i][nop] * 180.0 / pie;
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
				angle[j][nop] = 360 - angle[j][nop];
			}
		}
		//���]�������̂��������킹��
		sum = 0;
		for (int j = 0; j < anscount; j++) {
			sum = sum + angle[j][nop];
		}
		//���]���Ă��ꂪ�����Ă��邩�m�F���鏈���B�덷��+-3���B
		if ((anscount - 2) * 180 < sum + 3 && (anscount + 2) * 180 > sum + 3) {
			cout << endl;
			break;
		}
		//������Ƃ����]�������̂����ɖ߂�
		for (int j = 0; j < anscount; j++) {
			if (binary[j] == 1) {
				angle[j][nop] = 360 - angle[j][nop];
			}
		}
		cout << endl;
	}
	for (int i = anscount - 2; i >= 0; i--) {
		static double data = angle[anscount - 1][nop];
		if (i == 0) {
			angle[i][nop] = data;
		}
		else {
			angle[i + 1][nop] = angle[i][nop];
		}
	}
	//�p�x�̏o��
	for (int i = 0; i < anscount; i++) {
		cout << angle[i][nop] << endl;
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