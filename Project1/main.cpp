#include "freeglut.h"
#include<time.h>
#include<string>
float* enemiesX;
float* enemiesY;
bool* show;
bool count = true;
float x;
int enemy_counter=0;
float xr = 2.5;
int score=0;
bool stop = false;
bool pause = false;
int height=500;
int width=2*height/3;
int health = 0;
int changers = 0;
std::string temp_message = "";
int temp_type;
int announcment_counter = 0;
float* generateEnemyXCoords() 
{
	static float  xcoords[5];
	static bool tempShow[5];
	srand((unsigned)time(NULL));
	for (int i = 0; i < 5; ++i) 
	{
		xcoords[i] = 0.1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (16.0 - 0.1)));
		tempShow[i] = false;
		
	}
	show = tempShow;
	return xcoords;
}
void printAnnouncment(std::string message, int type)
{
	temp_message = message;
	temp_type = type;
	switch (type)
	{
	case 1:
		glColor3f(1.0, 0.0, 0.0);
		break;
	case 2:
		glColor3f(0.0, 1.0, 0.0);
		break;
	case 3:
		glColor3f(0.0, 0.0, 1.0);
		break;
	default:
		glColor3f(1.0, 1.0, 1.0);
		break;
	}
	glRasterPos2i(6.0, 10.0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)message.c_str());

}
void printScore()
{
	glColor3f(1.0, 1.0, 1.0);
	std::string message = "Puan:" + std::to_string(score / 2);
	glRasterPos2i(16.5, 18.0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)message.c_str());

}
float* generateEnemyYCoords()
{
	static float  ycoords[5];
	for (int i = 0; i < 5; ++i)
	{
		ycoords[i] = 19.5;
	}
	return ycoords;
}
void changeEnemyYCoords()
{
	count = false;
	for (int i = 0; i < 5; i++)
	{
		if (i == 0 || *(enemiesY + i-1) <= 15.0)
		{
			*(show + i) = true;
			*(enemiesY + i) -= 1.0;
		}
	}
}
void changeEnemyXCoords()
{
	for (int i = 0; i < 5; i++)
	{
		int op;
		op = rand() % 10 + 1;
		if (op % 2 == 0)
		{
			if (*(enemiesX + i) + (((20.0*3.0) / 16.0)) >= 19.5)
			{
				*(enemiesX+i) -= (float)(rand()%10)/10.0;
			}
			else
			{
				*(enemiesX + i) += (float)(rand() % 10) / 10.0;
			}
		}
		else if (op % 2 == 1)
		{
			if (*(enemiesX + i) >= 0.5)
			{
				*(enemiesX + i) -= (float)(rand() % 10) / 10.0;
			}
			else
			{
				*(enemiesX + i) += (float)(rand() % 10) / 10.0;
			}
		}
	}
}
bool enemiesFinished()
{
	bool temp = true;
	for (int i = 0; i < 5; i++)
	{
		if (*(enemiesY + i) >= 20.0 /8.0)
		{
			temp = false;
		}
	}
	return temp;
}
void checkCrashed() 
{
	for (int i = 0; i < 5; i++)
	{
		if (((*(enemiesX + i) >= xr && *(enemiesX + i) <= xr + 20.0 / 8.0)) || (*(enemiesX + i) + (20.0*3.0) / 16.0 >= xr && *(enemiesX + i) + ((20.0*3.0) / 16.0) <= xr + 20.0 / 8.0) || (*(enemiesX + i) + (20.0*3.0) / 32.0 >= xr && *(enemiesX + i) + ((20.0*3.0) / 32.0) <= xr + 20.0 / 8.0))
		{
			if ((*(enemiesY + i) - (20.0 / 8.0) >=  (20.0 / 12.0) && *(enemiesY + i) - (20.0 / 8.0) <= 2 * (20.0 / 12.0)) || (*(enemiesY + i) >= (20.0 / 12.0) && *(enemiesY + i) <= 2 * (20.0 / 12.0)))
			{
				float tempX = *(enemiesX + i) + (((20.0*3.0) / 16.0) / 2.0);
				float tempY = *(enemiesY + i) - (20.0 / 8.0) - (20.0 / 12.0);
				if (health <= 0) 
				{
					stop = true;
					changers = 0;
				}
				else 
				{
					announcment_counter = 5;
					printAnnouncment(std::to_string(health-1)+" caniniz kaldi.",1);
					health--;
					generateEnemyXCoords();
					generateEnemyYCoords();
				}
			}
		}
	}
}
void initialize() {
	enemiesX = generateEnemyXCoords();
	enemiesY = generateEnemyYCoords();
}
void drawEnemies()
{
	if (enemiesFinished())
	{
		initialize();
		count = true;
	}
	else
	{
		if(!stop && !pause)
		{
			changeEnemyYCoords();
			changeEnemyXCoords();
		}
	}
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 5; i++)
	{
		if (i % 5 == 1)
		{
			glColor3f(1.0, 0.8, 0.3);
		}
		else if (i % 5 == 0)
		{
			glColor3f(0.5, 0.0, 0.5);
		}
		else if (i % 5 == 2)
		{
			glColor3f(0.2, 0.9, 0.6);
		}
		else if (i % 5 == 3)
		{
			glColor3f(0.6, 0.7, 0.2);
		}
		else if (i % 5 == 4)
		{
			glColor3f(0.3, 0.5, 0.0);
		}
		if (*(show + i))
		{
			glVertex2f(*(enemiesX + i), *(enemiesY + i));
			glVertex2f(*(enemiesX + i) + (((20.0*3.0) / 16.0) / 2.0), *(enemiesY + i) - (20.0 / 8.0));
			glVertex2f(*(enemiesX + i) + (((20.0*3.0) / 16.0)), *(enemiesY + i));
		}
	}
	glEnd();
}

void drawPlayer() {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex2f(xr, 20.0 / 12.0);
	glVertex2f(20.0 / 8.0 +xr, 20.0 / 12.0);
	glVertex2f(20.0 / 8.0 +xr, 2*(20.0 / 12.0));
	glVertex2f(xr, 2*(20.0 / 12.0));
	glEnd();
	glFlush();
}
void timer(int)
{
	glutSwapBuffers();
	glutPostRedisplay();
	glutTimerFunc(10000.0 / 60.0, timer, 0);
}
void drawHealth()
{
	for (int j = 0; j < health; j++)
	{
		int i;
		int triangleAmount = 50;

		GLfloat twicePi = 2.0f * 3.1459;
		glColor3f(1.0,0.0,0.0);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(19 - j * 2,1);
		for (i = 0; i <= triangleAmount; i++) {
			glVertex2f(
				19 - j * 2 + (0.375 * cos(i *  twicePi / triangleAmount)),
				0.75 + (0.25* sin(i * twicePi / triangleAmount))
			);
		}
		glEnd();
	}
}
void drawChangerBar()
{
	for (int j = 0; j < changers; j++)
	{
		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_LINES);
		glVertex2f(1.0+j, 1.0);
		glVertex2f(1.0 + j, 0.25);
		glEnd();
		glFlush();
	}
}
void checkHealth()
{
	if (score%75 == 0 && health < 2)
	{
		announcment_counter = 5;
		printAnnouncment("+1 Yasam Hakki", 2);
		health++;
	}
	drawHealth();
}
void checkChangers() 
{
	if (score % 45 == 0 && changers < 1)
	{
		announcment_counter = 5;
		printAnnouncment("+1 Yer Degistirme", 3);
		changers++;
	}
	drawChangerBar();
}
void drawItems()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0,0.0,0.0);
	glFlush();
	drawEnemies();
	if (!stop && !pause)
	{
		score++;
	}
	else 
	{
		if (stop) 
		{
			printAnnouncment("Oyun Bitti!", 4);
		}
		else if (pause) {
			printAnnouncment("Durduruldu!", 4);
		}
	}
	drawPlayer();
	checkCrashed();
	checkHealth();
	checkChangers();
	printScore();
	if (announcment_counter > 0)
	{
		announcment_counter--;
		printAnnouncment(temp_message, temp_type);
	}
}
void keyUp(int key,int x, int y) {
	if (!stop && !pause)
	{
		switch (key)
		{
		case GLUT_KEY_LEFT:
			if (xr > 0.15)
				xr -= 0.30;
			break;
		case GLUT_KEY_RIGHT:
			if (xr < 20.0 - 20.0 / 8.0 - 0.15)
				xr += 0.30;
			break;
		}
	}
}
void mousePressed(int button, int state,  int x, int y) 
{
	if (changers > 0 && button == GLUT_LEFT_BUTTON && !pause)
	{
		xr = 20.0 * (float)x / (float)width;
		if (xr >= 20.0 - 20.0 / 8.0 - 0.15)
		{
			xr = 20.0 - 20.0 / 8.0 - 0.15;
		}
		changers--;
	}
}
void keyPressed(unsigned char key, int x, int y) {
	if (key==13) {
		if (stop)
		{
			score = 0;
			generateEnemyXCoords();
			generateEnemyYCoords();
		}
		stop = false;
	}
	if (key == 27)
	{
		exit(0);
	}
	if (!stop && !pause)
	{
		switch (key)
		{
		case 'a':
			if (xr > 0.15)
				xr -= 0.30;
			break;
		case 'd':
			if (xr < 20.0 - 20.0 / 8.0 - 0.15)
				xr += 0.30;
			break;
		}
	}
	if (key == 'p')
	{
		pause = !pause;
	}
}


int main(int iArgc, char** cppArgv) {
	initialize();
	glutInit(&iArgc, cppArgv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Utku Erdemir");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	gluOrtho2D(0.0, 20.0, 0.0, 20.0);
	glutSpecialFunc(keyUp);
	glutKeyboardFunc(keyPressed);
	glutMouseFunc(mousePressed);
	glutDisplayFunc(drawItems);
	glutTimerFunc(1000.0 / 60.0, timer, 0);
	glutMainLoop();
	return 0;
}
