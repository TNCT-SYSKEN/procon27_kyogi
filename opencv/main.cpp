# include "Operator.h"


int main(int argc, char **argv) {
	auto ope = make_unique<Operator>();

	ope->exec();
	cv::waitKey();

	return 0;
}