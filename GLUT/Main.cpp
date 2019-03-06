#include<GL/glut.h>
#include<iostream>
#include<algorithm>
#include<stdio.h>
#define _USE_MATH_DEFINES
#include<math.h>

using namespace std;

//global variables
int width = 600;
int height = 600;

string git = "My first time with git";

//color
GLubyte R = 0, B = 0, G = 0;
int colorIncrementer = 25;

//state variables
bool isMousePressed = false;

//tool states
bool isPenToolActive = false;
	bool isKaleidoscope = false;
		int symmetryLines = 30;
		bool doubleSymmetry = true;
bool isLineToolActive = false;
bool isRectangleToolActive = false;
bool isCircleToolActive = false;
bool diameterTool = false;

//hold variables
bool isShiftPressed = false;

//Window_display
void InitWindow();
void display(void);
void CreateMyMenu();

//contoller
void keyboard(unsigned char, int, int);
void keyboardUp(unsigned char, int, int);
void specialFunc(int, int, int);
void specialUpFunc(int, int, int);
void mouseFunc(int, int, int, int);
void motionFunc(int, int);
void passiveMotionFunc(int, int);

//tools
void PenTool(int, int, int, int);
void LineTool(int, int, int, int);
void RectangleTool(int, int, int, int);
void CircleTool(int, int, int, int);
void ChangeColorUp();
void ChangeColorDown();

//drawing
void RecordPoints(int, int);
void SetColor(GLubyte, GLubyte, GLubyte);


//Ansilary drawing
void ToggleTools(bool&);
void SnapLine(int, int, int, int);
//void ReflectPoint(int&, int&, int, int, int);
void RotatePoint(int&, int&, int, int, double);

//menu
void MainMenu(int);
void ToolSelectorMenu(int);
void ColorSelectorMenu(int);

//Drawing Algorithms
void DrawLine(int x1, int y1, int x2, int y2);
void DrawCircle(int h, int k, int radius);
double CalculateEpsilon(int r);
//void FillColor(int x, int y);

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	InitWindow();

	glutPassiveMotionFunc(passiveMotionFunc);//capture free moment of the mouse
	glutMotionFunc(motionFunc);//capture hold state of the mouse
	glutMouseFunc(mouseFunc);//capture clicks
	glutKeyboardFunc(keyboard);//capture key pressed
	glutKeyboardUpFunc(keyboardUp);		//capture released keys
	glutSpecialFunc(specialFunc);
	glutSpecialUpFunc(specialUpFunc);			//capture released special keys
	glutDisplayFunc(display);

	CreateMyMenu();

	glutMainLoop();
}

void InitWindow()											//-----INIT_WINDOW-----
{
	//create window
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("PAINT");

	//initialise canvas
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, width, 0, height);		//set the projection area
	glClearColor(1, 1, 1, 1);					//set color of the screen
	glClear(GL_COLOR_BUFFER_BIT);			//clear the buffer of the screen
	glFlush();								//force update of the screen

	//set pencil color
	glColor3ub(255, 0, 0);
}

void display(void)											//-----DISPLAY-----
{
	//glClear(GL_COLOR_BUFFER_BIT);			//clear the buffer of the screen
	glFlush();								//force update of the screen
}

void CreateMyMenu()											//-----MENU-----
{
	//Creating submenus
	int subMenuTool=glutCreateMenu(ToolSelectorMenu);	    //Tool_selctor_tool
		glutAddMenuEntry("PEN TOOL",1);
		glutAddMenuEntry("LINE TOOL",2);
		glutAddMenuEntry("RECTANGLE TOOL",3);
		glutAddMenuEntry("CIRCLE TOOL",4);
	
	int subMenuColor = glutCreateMenu(ColorSelectorMenu);	//Color_Menu
		glutAddMenuEntry("RED",1);
		glutAddMenuEntry("BLUE",2);
		glutAddMenuEntry("BLACK",3);
		glutAddMenuEntry("WHITE",4);
		glutAddMenuEntry("YELLOW",5);
		glutAddMenuEntry("GREEN",6);
		glutAddMenuEntry("SPRING_GREEN",7);
		glutAddMenuEntry("DEEP_PINK",8);
		glutAddMenuEntry("LAWNGREEN",9);
		glutAddMenuEntry("AQUAMARINE",10);
		

	//Creating Main Menu
		glutCreateMenu(MainMenu);							//Tool_selctor_tool
		glutAddSubMenu("TOOLS",subMenuTool);
		glutAddSubMenu("COLORS",subMenuColor);
		glutAddMenuEntry("EXIT",1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	

	
}

//=======================================CONTROLLER====================================

void keyboard(unsigned char key, int x, int y)				//-----KEYBOARD-----
{
	switch (key)
	{
	case 'd'://clear the screen
		glClear(GL_COLOR_BUFFER_BIT);						//clear the buffer of the screen
		glFlush();
		break;
	case 's'://set the color
		cout << "ENTER COLOR" << endl;
		double r, b, g;
		cin >> r >> b >> g;
		SetColor(r, b, g);
		break;

	case 'p'://toggle PencilTool on/off
		ToggleTools(isPenToolActive);
		break;

	case 'l'://toggle LineTool on/off
		ToggleTools(isLineToolActive);
		break;

	case 'r'://toggle Rectangle tool
		ToggleTools(isRectangleToolActive);
		break;

	case 'c'://toggle Circle tool
		ToggleTools(isCircleToolActive);
		break;
	case '+'://increase symmetry
		if (isKaleidoscope && symmetryLines<360)
		{
			symmetryLines++;
			cout << "SYMMETRY: " << symmetryLines << endl;
		}
		break;
	case '-'://decrease symmetry
		if (isKaleidoscope && symmetryLines>2)
		{
			symmetryLines--;
			cout << "SYMMETRY: " << symmetryLines << endl;

		}
		break;

	case '1'://multifunction
		if (isPenToolActive)//toggle kaleidoScope if pen is selected
		{
			isKaleidoscope = !isKaleidoscope;
			cout << "KALEIIDOSCOPE: " << isKaleidoscope << endl;
		}
		else if (isCircleToolActive)
		{
			diameterTool = !diameterTool;
			cout << "DIAMETER CIRCLE: " << diameterTool << endl;
		}
		break;
	case '2':
		if (isKaleidoscope)
		{
			doubleSymmetry = !doubleSymmetry;
			cout << "DOUBLE SYMMETRY: " << doubleSymmetry << endl;
		}
	}
}

void mouseFunc(int button, int state, int x, int y)			//-----mouseFunc-----
{
	if (state == GLUT_DOWN)
	{

		isMousePressed = true;
		if (isLineToolActive || isRectangleToolActive || isCircleToolActive)
			RecordPoints(x, -y + height);

	}
	else if (state == GLUT_UP)
	{
		//cout << "MOUSE IS RELEASED" << endl;
		isMousePressed = false;
		if (isPenToolActive)
		{
			RecordPoints(0, 0);//to reset penTool so i can lift up my pen
		}
	}
	if (button == 3)//Scroll Up
	{
		ChangeColorUp();
	}
	else if (button == 4)//Scroll Down
	{
		ChangeColorDown();
	}
}

void motionFunc(int x, int y)								//-----motionFunc-----
{
	if (isMousePressed && isPenToolActive)//only draw on mouse if pentool is active
	{
		RecordPoints(x, -y + height);
	}
}

void passiveMotionFunc(int x, int y)						//-----passiveMotionFunc-----
{
	//cout << "*";
}

void keyboardUp(unsigned char key, int x, int y)			//-----relasing of keyboard_inputs-----
{
	//cout << "key released" << key << endl;
}

void specialFunc(int key, int x, int y)						//-----pressing of special keys-----
{
	switch (key)
	{
	case 112://shift is pressed
		isShiftPressed = true;
		break;
	}
	//cout << key << endl;
}

void specialUpFunc(int key, int x, int y)					//-----releasing of special keys-----
{
	switch (key)
	{
	case 112://shift is released
		isShiftPressed = false;
		break;
	}
}


//=============================CONTROLLER_END======================================

void RecordPoints(int x, int y)							 	//-----RECORD_POINTS-----
{
	static int count = 0;
	static int x1, y1;

	if (!isMousePressed)	//so  i can lift my pen up
	{
		count = 0;
		return;
	}

	if (count == 0)//record first point
	{
		x1 = x;
		y1 = y;
		count++;
	}
	else//record second point
	{
		if (isPenToolActive)
		{
			PenTool(x1, y1, x, y);
		}
		else if (isLineToolActive)
		{
			LineTool(x1, y1, x, y);
		}
		else if (isRectangleToolActive)//draw rectangle
		{
			RectangleTool(x1, y1, x, y);
		}
		else if (isCircleToolActive)
		{
			//cerr << "circle" << endl;
			CircleTool(x1, y1, x, y);
		}


		if (!isPenToolActive)//reset points
		{
			count = 0;
		}
		else//pen tool is active or recording is on
		{
			x1 = x;
			y1 = y;
		}
	}

	glFlush();
}

//===========================ANSILARRY_DRAWING==============================

void SetColor(GLubyte r, GLubyte b, GLubyte g)			    //SetColor
{
	glColor3ub(r, b, g);
}

void ToggleTools(bool &current)							    //ToggleTools
{
	system("CLS");
	bool temp = current;
	isCircleToolActive = isRectangleToolActive = isKaleidoscope = isPenToolActive = isLineToolActive = false;
	current = !temp;

	cout << "LINETOOL : " << isLineToolActive << endl;
	cout << "PENTOOL  : " << isPenToolActive << endl;
	cout << "RECTANGLE: " << isRectangleToolActive << endl;
	cout << "CIRCLE   : " << isCircleToolActive << endl;

}

void SnapLine(int x1, int y1, int x2, int y2)
{
	int dy = abs(y2 - y1);
	int dx = abs(x2 - x1);

	if (dx > dy)//snap horizontally
	{
		DrawLine(x1, y1, x2, y1);
	}
	else       //snap vertically
	{
		DrawLine(x1, y1, x1, y2);
	}
}

//void ReflectPoint(int &X_new, int &Y_new, int x, int y, int angle)//reflect point around centre
//{
//	double rad = (M_PI / (double)180) * 2 * angle;
//	cout << rad << endl;
//	int Tx = width / 2;
//	int Ty = height / 2;
//	X_new = x * cos(rad) - y * sin(rad) - Tx * cos(rad) + Ty * sin(rad) + Tx;
//	Y_new = -x * sin(rad) - y * cos(rad) + Tx * sin(rad) + Ty * cos(rad) + Ty;
//}

void RotatePoint(int &X_new, int &Y_new, int x, int y, double angle)		//Rotate Point Around Centre
{
	double rad = (M_PI / (double)180) * angle;
	//cerr << rad << endl;
	int Tx = width / 2;
	int Ty = height / 2;

	X_new =  x * cos(rad) + y * sin(rad) - Tx * cos(rad) - Ty * sin(rad) + Tx;
    Y_new = -x * sin(rad) + y * cos(rad) + Tx * sin(rad) - Ty * cos(rad) + Ty;
}
//==============================ANISILARY_DRAWING_END===========================

//====================================TOOLS=====================================

void PenTool(int x1, int y1, int x2, int y2)				//<+ PEN_TOOL +>
{
	int git = 99;
	DrawLine(x1, y1, x2, y2);
	//reafactor this
	if (isKaleidoscope)//4 symmetry
	{
		if (doubleSymmetry)
		{
			DrawLine(-x1+width,y1,-x2+width,y2);
		}
		int x3, y3, x4, y4;
		double angle=(double)360/symmetryLines;
		double angleInc = angle;
		for (int i = 0; i < symmetryLines-1; i++)
		{

			RotatePoint(x3,y3,x1,y1,angle);
			RotatePoint(x4,y4,x2,y2,angle);
			DrawLine(x3,y3,x4,y4);

			if (doubleSymmetry)
			{
				DrawLine(-x3 + width, y3, -x4 + width, y4);
			}
			angle += angleInc;
		}
	}
}

void LineTool(int x1, int y1, int x2, int y2)				//<+- LINE_TOOL -+>
{
	if (isShiftPressed)
	{
		SnapLine(x1, y1, x2, y2);
	}
	else
	{
		DrawLine(x1, y1, x2, y2);
	}
}

void RectangleTool(int x1, int y1, int x2, int y2)			//<+- RECTANGLE_TOOL -+>
{
	DrawLine(x1, y1, x1, y2);
	DrawLine(x1, y2, x2, y2);
	DrawLine(x2, y2, x2, y1);
	DrawLine(x2, y1, x1, y1);
}

void CircleTool(int x1, int y1, int x2, int y2)
{
	double radius = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));

	if (diameterTool)
	{
		double h = (x1 + x2) / 2;
		double k = (y1 + y2) / 2;
		DrawCircle(h, k, radius / 2);
	}
	else
	{
		DrawCircle(x1, y1, radius);
	}
}

void ChangeColorUp()										//<+- COLOR_CHANGER_TOOL -+>
{
	R += colorIncrementer;
	if (R > 240)
	{
		B += colorIncrementer;
		if (B > 240)
			G += colorIncrementer;
	}
	//cout << (int)R << "," << (int)B << "," << (int)G << endl;
	SetColor(R, B, G);
}
void ChangeColorDown()										//<+- COLOR_CHANGER_TOOL -+>
{
	if (G < 10)
	{
		if (B < 10)
			R -= colorIncrementer;
		B -= colorIncrementer;
	}

	G -= colorIncrementer;
	//cout << (int)R << "," << (int)B << "," << (int)G << endl;

	SetColor(R, B, G);
}
//===============================TOOLS_END=======================================

//==================================MENU=========================================
void MainMenu(int  choice)					    //-----Simple_MENU-----
{
	switch (choice)
	{
	case 1://EXIT
		exit(0);
		break;
	}
}
void ToolSelectorMenu(int choice)				//-----Tool_Selector_Menu-----
{
	switch (choice)
	{
	case 1:
		ToggleTools(isPenToolActive);
		break;
	case 2:
		ToggleTools(isLineToolActive);
		break;
	case 3:
		ToggleTools(isRectangleToolActive);
		break;
	case 4:
		ToggleTools(isCircleToolActive);
		break;
	}
}

void ColorSelectorMenu(int choice)					//-----ColorSelector-----
{
	switch (choice)
	{
	case 1://RED
		SetColor(255,0,0);
		break;
	case 2://BLUE
		SetColor(0, 0, 255);
		break;
	case 3://BLACK
		SetColor(0, 0, 0);
		break;
	case 4://WHITE
		SetColor(255, 255, 255);
		break;
	case 5://YELLOW
		SetColor(255, 255, 0);
		break;
	case 6://GREEN
		SetColor(0, 128, 0);
		break;
	case 7://SPRINGGREEN
		SetColor(0, 255, 127);
		break;
	case 8://DEEPPINK
		SetColor(255, 20, 147);
		break;
	case 9://LAWNGREEN
		SetColor(124, 252, 0);
		break;
	case 10://AQUAMARINE
		SetColor(127, 255, 212);
		break;
	}
}



//================================DRAWING_ALGORITHMS==============================

void DrawLine(int x1, int y1, int x2, int y2)				//-----DDA-----
{
	double dx = x2 - x1,
		dy = y2 - y1;
	int step = max(abs(dx), abs(dy));
	double xinc = dx / step,
		yinc = dy / step;
	double x = x1,
		y = y1;

	while (step--)
	{
		x += xinc;
		y += yinc;
		glBegin(GL_POINTS);
		glVertex2f(x, y);
		glEnd();
	}
}


void DrawCircle(int h, int k, int radius)					//-----DDA_CIRCLE-----
{
	double x = radius,
		y = 0;
	double eps = CalculateEpsilon(radius);
	//cerr << "eps: " << eps << endl;

	while ((y - 0) < eps || (radius - x) > eps)
	{
		glBegin(GL_POINTS);
		glVertex2f(x + h, y + k);
		glEnd();
		//putpixel(x + h, y + k, color);
		x += y * eps;
		y -= x * eps;
	}
}


double CalculateEpsilon(int r)							   //CALCULATE_EPSILON
{
	int n;
	for (n = 0; pow(2, n) < r; n++);
	return (double)1 / pow(2, n);
}

//Rotation around centre of screen
// X_new =  x*cos(0) + y*sin(0) - Tx*cos(0) - Ty*sin(0) + Tx;
// Y_new = -x*sin(0) + y*cos(0) + Tx*sin(0) - Ty*cos(0) + Ty;
//


//Reflection around center equation
// X_new =  x*cos(20) - y*sin(20) -Tx*cos(20) + Ty*sin(20) + Tx;
// Y_new = -x*sin(20) - y*cos(20) +Tx*sin(20) + Ty*cos(20) + Ty;
//
//where 0 is the slope of the line

//Debug Buffer
/*cerr << "(" << x1 << "," << y1 << ")";
	cerr << " = ";
	cerr << "(" << x3 << "," << y3 << ")";
	cerr << " = ";
	cerr << "(" << x1 << "," << -y1 + height << ")"<<endl;*/


	//Trash 


				/*ReflectPoint(x3, y3, x1, y1, 90);
				ReflectPoint(x4, y4, x2, y2, 90);
				DrawLine(x3, y3, x4, y4);

				int x5, y5, x6, y6;
				ReflectPoint(x5, y5, x3, y3, 0);
				ReflectPoint(x6, y6, x4, y4, 0);
				DrawLine(x5, y5, x6, y6);

				int x7, y7, x8, y8;
				ReflectPoint(x7, y7, x5, y5, 90);
				ReflectPoint(x8, y8, x6, y6, 90);
				DrawLine(x7, y7, x8, y8);
	*/
