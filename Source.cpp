#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <string>

int countOfBalls = 12;
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

};

void queryCollisions(qtree targetTree);
class myrectangle {
public:
	float x, y;//Origin of rectangle
	float width, height;//Size of rectangle.
	myrectangle(float,float,float,float);

	bool containRect(myrectangle rectx) {
		return ((x + width >= rectx.x || x <= rectx.x + width) && 
			(y + height >= rectx.y || y <= rectx.y + height));
	}
};

void checkForCollisions(qtree* currentTree);
std::vector <ball> balls;
qtree::qtree(std::vector <ball> inputData, float ox, float oy, float pInterval) {
	section1 = NULL;
	section2 = NULL;
	section3 = NULL;
	section4 = NULL;
	datapoints = inputData;
	originx = ox;
	originy = oy;
	interval = pInterval;
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
	for (int i = 0; i <= countOfBalls - 1; i++) {
		//randMToN(-1.0 + sz,1.0 - sz)
		balls.push_back(ball(-1.0 + sz + (float(i) * 1.5 / (countOfBalls - 1.0)), 0.0, randMToN(0.00001, 0.00005), randMToN(0.00001, 0.00005), sz, i));
	}
	mainTree = qtree(balls, 0.0, 0.0, 1.0);
	glutDisplayFunc(render);
	glutMainLoop();
	return 0;
}


myrectangle::myrectangle(float xP, float yP, float wP, float hP) {
	x = xP;
	y = yP;
	width = wP;
	height = hP;
}



void checkForCollisions(qtree* currentTree) {

	std::vector <ball> aBalls = (*(qtree *)currentTree).datapoints;
	float orgx = (*(qtree*)currentTree).originx;
	float orgy = (*(qtree*)currentTree).originy;
	float iterval = (*(qtree*)currentTree).interval;
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
		if (aBalls[bNum].x - aBalls[bNum].size >= orgx && aBalls[bNum].x + aBalls[bNum].size >= orgx &&
			aBalls[bNum].y - aBalls[bNum].size >= orgy && aBalls[bNum].y + aBalls[bNum].size >= orgy) { //Positive x and y section
			//std::cout << "Printing Section 1..." << std::endl;
			sect1.push_back(aBalls[bNum]);
		}
		else if (aBalls[bNum].x - aBalls[bNum].size <= orgx && aBalls[bNum].x + aBalls[bNum].size <= orgx &&
			aBalls[bNum].y - aBalls[bNum].size >= orgy && aBalls[bNum].y + aBalls[bNum].size >= orgy) { //Positive x and y section
			//std::cout << "Printing Section 2..." << std::endl;
			sect2.push_back(aBalls[bNum]);
		}
		else if (aBalls[bNum].x - aBalls[bNum].size <= orgx && aBalls[bNum].x + aBalls[bNum].size <= orgx &&
			aBalls[bNum].y - aBalls[bNum].size <= orgy && aBalls[bNum].y + aBalls[bNum].size <= orgy) { //Positive x and y section
			//std::cout << "Printing Section 3..." << std::endl;
			sect3.push_back(aBalls[bNum]);
		}
		else if (aBalls[bNum].x - aBalls[bNum].size >= orgx && aBalls[bNum].x + aBalls[bNum].size >= orgx &&
			aBalls[bNum].y - aBalls[bNum].size <= orgy && aBalls[bNum].y + aBalls[bNum].size <= orgy) { //Positive x and y section
			//std::cout << "Printing Section 4..." << std::endl;
			sect4.push_back(aBalls[bNum]);
		}

	}
	
	//std::cout << "Section 1: " << sect1.size() << " Section 2 : " << sect2.size() << " Section 3 : " << sect3.size() << " Section 4 : " << sect4.size() << std::endl;
	if (sect1.size() > 0 || sect2.size() > 0 || sect3.size() > 0 || sect4.size() > 0) {
	
		(*(qtree*)currentTree).divided = true;
		int finalMax = 3;
		//Section 1
		if (sect1.size() > finalMax) {
			qtree q1Tree = qtree(sect1,orgx+(iterval * 0.5f),orgy+(abs(iterval)*0.5f),abs(iterval)*0.5f);
			(*(qtree*)currentTree).section1 = &q1Tree;
			checkForCollisions((*(qtree*)currentTree).section1);
		}
		else if (sect1.size() > 0) { }
		//Section 2
		if (sect2.size() > finalMax) {
			qtree q2tree = qtree(sect2, orgx - (iterval * 0.5), orgy + (abs(iterval) * 0.5), abs(iterval) * 0.5);
			(*(qtree*)currentTree).section2 = &q2tree;
			checkForCollisions((*(qtree*)currentTree).section2);
		}
		else if (sect2.size() > 0) { }
		//Section 3
		if (sect3.size() > finalMax) {
			qtree q3tree = qtree(sect3, orgx - (iterval * 0.5), orgy - (abs(iterval) * 0.5), abs(iterval) * 0.5);
			(*(qtree*)currentTree).section3 = &q3tree;
			checkForCollisions((*(qtree*)currentTree).section3);
		}
		else if (sect3.size() > 0) {  }
		//Section 4
		if (sect4.size() > finalMax) {
			qtree q4tree = qtree(sect4, orgx + (iterval * 0.5), orgy - (abs(iterval) * 0.5), abs(iterval) * 0.5);
			(*(qtree*)currentTree).section4 = &q4tree;
			checkForCollisions((*(qtree*)currentTree).section4);
		}
		else if (sect4.size() > 0) {  }
	}
	else{return;}

}
double randMToN(double M, double N)
{
	return M + (rand() / (RAND_MAX / (N - M)));
}
void printing(std::string input) {
	std::cout << input << std::endl;
}
void queryCollisions(qtree targetTree) {

	for (int qi = 0; qi <= targetTree.datapoints.size(); qi++) {
		//gets the x and y component of circle
		float rectxaxis = targetTree.datapoints[qi].x - targetTree.datapoints[qi].size;
		float rectyaxis = targetTree.datapoints[qi].y - targetTree.datapoints[qi].size;
		float rectsize = targetTree.datapoints[qi].size * 2.0;
		myrectangle rect1 = myrectangle(rectxaxis, rectyaxis, rectsize, rectsize);
		
	}


}

void render(void) {
	
	//std::vector <ball> secballs;
	//for (int x = 0; x <= 2; x++) { secballs.push_back(balls[x+1]); }
	//qtree secTree = qtree(secballs);
	//qtree* ptr = &secTree;
	//firstTree.divided = true;
	//firstTree.section1 = ptr;
	//std::cout << firstTree.divided << std::endl;
	//std::cout << firstTree.section1->datapoints[0].x << std::endl;
	//std::cout << (*(std::vector <ball> *)(firstTree.datapoints)).size() << std::endl;

	//std::vector <ball> testData = *(std::vector <ball> *)firstTree.datapoints;
	//std::cout << firstTree.divided << std::endl;
	//std::cout << testData.size() << std::endl;
	
	while (true) {
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	
		void* tst;
		checkForCollisions(&mainTree);
		//std::cout << mainTree.section1 << std::endl;
		if (mainTree.section1 != NULL) { std::cout << mainTree.section1->divided << std::endl; }
		
		for (int num = 0; num <= mainTree.datapoints.size() - 1; num++) {
			mainTree.datapoints[num].ballUpdate();
		}
		//Insert Section start
		
		
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