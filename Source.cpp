#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <string>

using namespace std;
int countOfBalls = 20;
double randMToN(double M, double N);
void DrawCircle(float cx, float cy, float r, int num_segments);
void printing(std::string input);
void render();

class ball {
public:
	float x, y;
	float vx, vy;
	float size;
	int id;
	ball(float, float, float, float, float, int);

	void ballUpdate() {
		if (x >= 1.0 - size) {
			vx = -(abs(vx));

		}
		else if (x <= -(1.0 - size)) {
			vx = (abs(vx));
		}

		if (y >= 1.0 - size) {
			vy = -(abs(vy));

		}
		else if (y <= -(1.0 - size)) {
			vy = (abs(vy));
		}

		x += vx;
		y += vy;
		DrawCircle(x, y, size, 100);
	}
};

ball::ball(float a, float b, float c, float d, float e, int f) {
	x = a;
	y = b;
	vx = c;
	vy = d;
	size = e;
	id = f;

}

class qtree {
public:
	qtree *section1,*section2,*section3,*section4;
	bool divided = false;
	std::vector <ball> datapoints;
	float originx,originy,interval;

	qtree(std::vector <ball>,float,float,float); 
	qtree();
	void insertTree(std::vector <ball>* s1, std::vector <ball>* s2, std::vector <ball>* s3, std::vector <ball>* s4);

	void itree() {
		qtree atree = qtree();
		this->section1 = &atree;
		this->section1->divided = true;
		cout << &this->section1->divided << endl;
	}

};
void doCollisions(ball* cBall, qtree* queriedTree);

class myrectangle {
public:
	float x, y;//Origin of rectangle
	float width, height;//Size of rectangle.
	myrectangle(float,float,float,float);
	myrectangle(ball*);
};
bool intersect(myrectangle* rect1, myrectangle* rect2);

void queryCollisions(ball* targBall, qtree* thisTree, std::vector <qtree>* qqtree);
void checkForCollisions(qtree* currentTree);
bool pFormula(ball* ball1, ball* ball2);

std::vector <ball> balls;

void qtree::insertTree(std::vector <ball>* s1, std::vector <ball>* s2, std::vector <ball>* s3, std::vector <ball>* s4) {
	int finalMax = 3;
	//Section 1
	if (s1->size() > 0) {
		section1 = new qtree(*s1, originx + (interval * 0.5f), originy + (abs(interval) * 0.5f), abs(interval) * 0.5f);
		if (s1->size() > finalMax) { checkForCollisions(section1); }
	}
	//Section 2
	if (s2->size() > 0) {
		section2 = new qtree(*s2, originx - (interval * 0.5f), originy + (abs(interval) * 0.5f), abs(interval) * 0.5f);;
		if (s2->size() > finalMax) { checkForCollisions(section2); }

	}
	//Section 3
	if (s3->size() > 0) {
		section3 = new qtree(*s3, originx - (interval * 0.5f), originy - (abs(interval) * 0.5f), abs(interval) * 0.5f);
		if (s3->size() > finalMax) { checkForCollisions(section3); }
	}
	//Section 4
	if (s4->size() > 0) {
		section4 = new qtree(*s4, originx + (interval * 0.5f), originy - (abs(interval) * 0.5f), abs(interval) * 0.5f);
		if (s4->size() > finalMax) { checkForCollisions(section4); }
	}
}

qtree::qtree(std::vector <ball> inputData, float ox, float oy, float pInterval) {
	section1 = NULL;
	section2 = NULL;
	section3 = NULL;
	section4 = NULL;
	datapoints = inputData;
	originx = ox;
	originy = oy;
	interval = pInterval;
	divided = false;
}

qtree::qtree() {
	section1 = NULL;
	section2 = NULL;
	section3 = NULL;
	section4 = NULL;
}

qtree mainTree = qtree();

int main(int argc,char ** argv) {
	std::cout << "Hello" << std::endl;
	glutInit(&argc, argv);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("WINDOW_NAME");
	float sz = 0.75 * (1.0 / float(countOfBalls));
	cout << "Size = " << sz << endl;
	for (int i = 0; i <= countOfBalls - 1; i++) {
		balls.push_back(ball(-1.0 + sz + (float(i) * 1.5 / (countOfBalls - 1.0)), randMToN(0.00001, 0.5), randMToN(0.00001, 0.00005), randMToN(0.00001, 0.00005), sz, i));
	}
	mainTree = qtree(balls, 0.0, 0.0, 1.0);
	glutDisplayFunc(render);
	glutMainLoop();
	return 0;
}

myrectangle::myrectangle(ball* b1) {
	x = b1->x - b1->size;
	y = b1->y - b1->size;
	width = b1->size * 2.0;
	height = b1->size * 2.0;
}

myrectangle::myrectangle(float xP, float yP, float wP, float hP) {
	x = xP;
	y = yP;
	width = wP;
	height = hP;
}

bool intersect(myrectangle * rect1, myrectangle * rect2) {
	return (((rect2->x <= rect1->x + rect1->width && rect2->x + rect2->width >= rect1->x) ||
		(rect2->x + rect2->width >= rect1->x && rect2->x + rect2->width <= rect1->x + rect1->width)) && ((rect2->y <= rect1->y + rect1->height && rect2->y + rect2->height>= rect1->y) ||
			(rect2->y + rect2->height >= rect1->y && rect2->y + rect2->height <= rect1->y + rect1->height)));
}

void checkForCollisions(qtree* currentTree) {
	std::vector <ball> aBalls = currentTree->datapoints;
	float orgx = currentTree->originx;
	float orgy = currentTree->originy;
	float iterval = currentTree->interval;
	std::vector <ball> sect1;
	std::vector <ball> sect2;
	std::vector <ball> sect3;
	std::vector <ball> sect4;
	glBegin(GL_LINES);
	glVertex2f(orgx - iterval, orgy);
	glVertex2f(orgx + iterval, orgy);
	glVertex2f(orgx, orgy - iterval);
	glVertex2f(orgx, orgy + iterval);
	glEnd();
	int cTotal = 0;
	for (int bNum = 0; bNum <= aBalls.size() - 1; bNum++) {
		if (aBalls[bNum].x >= orgx && aBalls[bNum].y >= orgy) {sect1.push_back(aBalls[bNum]);}
		else if (aBalls[bNum].x <= orgx && aBalls[bNum].y >= orgy ) { sect2.push_back(aBalls[bNum]);}
		else if (aBalls[bNum].x <= orgx &&
			aBalls[bNum].y <= orgy) {sect3.push_back(aBalls[bNum]);}
		else if (aBalls[bNum].x >= orgx &&
			aBalls[bNum].y <= orgy) { sect4.push_back(aBalls[bNum]);}
		else {}
	}
	if (sect1.size() > 0 || sect2.size() > 0 || sect3.size() > 0 || sect4.size() > 0) {
		currentTree->insertTree(&sect1, &sect2, &sect3, &sect4);
	}
	else { return; }
}

double randMToN(double M, double N)
{
	return M + (rand() / (RAND_MAX / (N - M)));
}

void queryCollisions(ball* targBall,qtree * thisTree,std::vector <qtree>* qqtree) {
	if (thisTree->divided) {
		myrectangle* _rt1 = new myrectangle(targBall);
		myrectangle quad1 = myrectangle(thisTree->originx, thisTree->originy,
			thisTree->interval, thisTree->interval);
		myrectangle quad2 = myrectangle(thisTree->originx-(thisTree->interval), thisTree->originy,
			(thisTree->interval), thisTree->interval);
		myrectangle quad3 = myrectangle(thisTree->originx - (thisTree->interval), thisTree->originy - (thisTree->interval),
			(thisTree->interval), (thisTree->interval));
		myrectangle quad4 = myrectangle(thisTree->originx, thisTree->originy - (thisTree->interval),
			thisTree->interval, (thisTree->interval));
		if (intersect(_rt1, &quad1) && thisTree->section1 != NULL) {
			if (thisTree->section1->divided == false) {
				doCollisions(targBall, thisTree->section1);
				qqtree->push_back(*thisTree->section1); }
			queryCollisions(targBall, thisTree->section1, qqtree);			
		} 
		if (intersect(_rt1, &quad2) && thisTree->section2 != NULL) {
			if (thisTree->section2->divided == false) { 
				doCollisions(targBall, thisTree->section2);
				qqtree->push_back(*thisTree->section2); }
			 queryCollisions(targBall, thisTree->section2, qqtree);
		}
		if (intersect(_rt1, &quad3) && thisTree->section3 != NULL) {
			if (thisTree->section3->divided == false) { 
				doCollisions(targBall, thisTree->section3);
				qqtree->push_back(*thisTree->section3); }
			queryCollisions(targBall, thisTree->section3, qqtree);
		}
		if (intersect(_rt1, &quad4) && thisTree->section4 != NULL) {
			if (thisTree->section4->divided == false) { 
				doCollisions(targBall, thisTree->section4);
				qqtree->push_back(*thisTree->section4); }
			 queryCollisions(targBall, thisTree->section4, qqtree);
		}
		delete _rt1;
		_rt1 = NULL;
	}
}

void deleteMem(qtree* xtree) {
	if (xtree->divided == true) {
		if (xtree->section1 != NULL) { deleteMem(xtree->section1); delete xtree->section1; xtree->section1 = NULL; }
		if (xtree->section2 != NULL) { deleteMem(xtree->section2); delete xtree->section2; xtree->section2 = NULL;}
		if (xtree->section3 != NULL) { deleteMem(xtree->section3); delete xtree->section3; xtree->section3 = NULL;}
		if (xtree->section4 != NULL) { deleteMem(xtree->section4);delete xtree->section4; xtree->section4 = NULL;
		}

	}

}

bool pFormula(ball* ball1, ball* ball2) {
		auto ydis = (ball2->y - ball1->y);
		auto xdis = (ball2->x - ball1->x);
		return sqrtf((xdis * xdis) + (ydis * ydis)) <= (ball1->size + ball2->size);
}

void doCollisions(ball* cBall,qtree* queriedTree) {
	for (int cint = 0; cint <= queriedTree->datapoints.size() - 1;cint++) {
		if (queriedTree->datapoints[cint].id != cBall->id) {
			ball* secBall = &(&mainTree)->datapoints[queriedTree->datapoints[cint].id];
			if (pFormula(secBall,cBall)) {
				auto ydis = (cBall->y - secBall->y);
				auto xdis = (cBall->x - secBall->x);
				float distance = sqrtf((xdis * xdis) + (ydis * ydis));
				float fOverlap = 0.5f * (distance - secBall->size - cBall->size);
				secBall->x += fOverlap * xdis / distance;
				secBall->y += fOverlap * ydis / distance;
				cBall->x -= fOverlap * xdis / distance;
				cBall->y -= fOverlap * ydis / distance;

			}
		}
	}
}

void render(void) {	
	while (true) {
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		qtree* treePtr = &mainTree;
		deleteMem(treePtr);
		checkForCollisions(treePtr);
		std::vector <qtree> queryQ;
		for (int num = 0; num <= treePtr->datapoints.size() - 1; num++) {
			treePtr->datapoints[num].ballUpdate();
			queryCollisions(&treePtr->datapoints[num], treePtr, &queryQ);
		}
		glFlush();
	}
}

void DrawCircle(float cx, float cy, float r, int num_segments) {
	glBegin(GL_LINE_LOOP);
	for (int ii = 0; ii < num_segments; ii++) {
		float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle 
		float x = r * cosf(theta);//calculate the x component 
		float y = r * sinf(theta);//calculate the y component 
		glVertex2f(x + cx, y + cy);//output vertex 
	}
	glEnd();
}