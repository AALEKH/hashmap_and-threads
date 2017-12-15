#include "test_thread.h"
#include <iostream>

int main() {
	WriteBuffer *wb_ = new WriteBuffer();
	wb_->WritePart(1);
	wb_->WritePart(2);
	wb_->WritePart(3);
	wb_->WritePart(4);
	wb_->WritePart(5);
	wb_->WritePart(6);
	wb_->WritePart(7);
	wb_->WritePart(8);
	wb_->WritePart(9);
	wb_->WritePart(10);
	wb_->Close();
	return 0;
}