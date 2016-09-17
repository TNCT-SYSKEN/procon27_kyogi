﻿#pragma once

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
	Piece(shared_ptr<cv::Mat> img, int number);

	//アクセサ
	int get_number_of_corner() const { return number_of_corner; }
	vector<shared_ptr<double> > get_line_lengths() const { return line_lengths; }
	vector<shared_ptr<cv::Point> > get_angle() const { return angle; }
	vector<shared_ptr<cv::Point> > get_vertex() const { return vertex; }
	//必要そうなのでseter
	/*void get_number_of_corner(int num ) { number_of_corner = num; }
	void get_line_lengths(shared_ptr<vector<double> > lengths) { line_lengths = lengths; }
	void get_angle(shared_ptr<vector<cv::Point> > ang) { angle = ang; }
	void get_vertex(shared_ptr<vector<cv::Point> > ver){ vertex = ver; }
	*/

	//頂点を求める
	void search_vertex();
	//辺を求める
	void search_line();
	//角度を求める
	void search_angle();

private:
	//何番目のピースか
	int number;
	//何角形か
	int number_of_corner;
	//辺の長さ(r)
	vector<shared_ptr<double> > line_lengths;
	//角度(θ)
	vector<shared_ptr<cv::Point> > angle;
	//頂点
	vector<shared_ptr<cv::Point> > vertex;

	//画像ドゥエータ(データ)
	shared_ptr<cv::Mat> image;

	//GUI用

};