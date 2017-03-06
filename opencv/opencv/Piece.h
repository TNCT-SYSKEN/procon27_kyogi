#pragma once

# include "Common.h"


//一つのピースのクラス
/*
使い方
直接変数はいじれないようにしています。
値は変更する方法は初期化の時に与える時だけ
もしその他に必要な時があれば教えて下さい。対応します
値を見たい場合にはアクセサのgetの関数を使って行ってください。
*/
class Piece {
public:
	//デフォルトコンストラクタはなしで！
	/*Piece(int number_of_corner, shared_ptr<vector<double> > line_lengths,
	shared_ptr<vector<cv::Point> > angle, shared_ptr<vector<cv::Point> > vertex);*/
	Piece(shared_ptr<cv::Mat> img, int num);
	/*
	//アクセサ
	//int get_number_of_corner() const { return number_of_corner; }
	//vector<shared_ptr<double> > get_line_lengths() const { return line_lengths; }
	//vector<shared_ptr<double> > get_angle() const { return angle; }
	//vector<shared_ptr<cv::Point> > get_vertex() const { return vertex; }
	//shared_ptr<double> get_piece_line(double number) const { return line_lengths[number]; }
	//void get_vertex(vector<shared_ptr<cv::Point> > ver) { vertex = ver; }
	//必要そうなのでseter
	/*void get_number_of_corner(int num ) { number_of_corner = num; }
	void get_line_lengths(shared_ptr<vector<double> > lengths) { line_lengths = lengths; }
	void get_angle(shared_ptr<vector<cv::Point> > ang) { angle = ang; }
	*/

	//頂点を求める
	void search_vertex();
	//辺を求める
	void search_line();
	//角度を求める
	void search_angle();

	void cut_image();

	/*----GUI用----*/
	//画像ドゥエータ(データ)
	shared_ptr<cv::Mat> image;
	//反転したかどうか
	bool reverse_flag;
	//敷き詰めたかどうか(敷き詰めた場合はGUIに表示
	bool put_flag;
	//色の指定(特に意味はない，きれいだから？
	cv::Scalar color;

	//頂点の始点と終点を返す関数
	cv::Point get_min_vertex();
	cv::Point get_max_vertex();


	int number;
	//何角形か
	int number_of_corner;
	//辺の長さ(r)
	vector<shared_ptr<double> > line_lengths;
	//角度(θ)
	vector<shared_ptr<double> > angle;
	//頂点
	vector<shared_ptr<cv::Point> > vertex;

};