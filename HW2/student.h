#pragma once

#include "stdafx.h"
using namespace std;

#define STUDENT_NUM 10

// 학생 구조체
struct student {
	string name;
	int score = 0;
};

// 학생들의 점수가 낮은 순서대로 정렬
bool cmp(const student& p1, const student& p2) {
	if (p1.score > p2.score) return true;
	else return false;
}