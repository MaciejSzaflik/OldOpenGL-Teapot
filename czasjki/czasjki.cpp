// OpenGl_lab4.cpp : Defines the entry point for the console application.
//
/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj? osi  
//  uk?adu wspó?rz?dnych dla rzutowania perspektywicznego

/*************************************************************************************/

#include "stdafx.h"

#pragma region

#define PI 3.14159265359
typedef float point3[3]; 
int model = 2;  // 1- punkty, 2- siatka, 3 - wype³nione trójk¹ty
int numOfPoints =40;
float valueY = 1.0f;
float valueY2 = 0.0f;
float valueX = 0.0f;
float valueZ = 0.0f;
float range = 1000.0f;
float fly=0;
float flySpeed = 0.001f;


typedef float point3[3]; 

static GLfloat viewer[]= {0.0, 0.0, 10.0}; 
static GLfloat theta = 0.0;   // k?t obrotu obiektu
static GLfloat theta2 = 0.0;
static GLfloat zoom = 1.0;
static GLfloat pix2angle;     // przelicznik pikseli na stopnie
static GLfloat pix2azymut;

static GLfloat R=18;

static GLint status = 0;      // stan klawiszy myszy 
static GLint status2 = 0;                              // 0 - nie naci?ni?to ?adnego klawisza
                              // 1 - naci?ni?ty zosta? lewy klawisz

static int x_pos_old=0;       // poprzednia pozycja kursora myszy
static int y_pos_old=0;
static int y_zoom=0;	


static int delta_x = 0;       // ró?nica pomi?dzy pozycj? bie??c?
                              // i poprzedni? kursora myszy
static int delta_y = 0;    
// inicjalizacja po?o?enia obserwatora 
#pragma endregion A
#pragma region 

// Funkcja rysuj?ca osie uk?adu wspó?rz?dnych 

void Axes(void)
{
point3  x_min = {-100.0, 0.0, 0.0};
point3  x_max = { 100.0, 0.0, 0.0};
// pocz?tek i koniec obrazu osi x

point3  y_min = {0.0, -100.0, 0.0}; 
point3  y_max = {0.0,  100.0, 0.0};
// pocz?tek i koniec obrazu osi y

point3  z_min = {0.0, 0.0, -100.0};
point3  z_max = {0.0, 0.0,  100.0};
//  pocz?tek i koniec obrazu osi y

glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
glBegin(GL_LINES); // rysowanie osi x
glVertex3fv(x_min);
glVertex3fv(x_max);

glEnd();

glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
glBegin(GL_LINES);  // rysowanie osi y
glVertex3fv(y_min);
glVertex3fv(y_max);                            

glEnd();

glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
glBegin(GL_LINES); // rysowanie osi z
glVertex3fv(z_min);
glVertex3fv(z_max); 

glEnd();

}

/*************************************************************************************/

// Funkcja okre?laj?ca co ma by? rysowane (zawsze wywo?ywana, gdy trzeba 
// przerysowa? scen?)
#pragma endregion Rysujace


/*************************************************************************************/
#pragma region

void Mouse(int btn, int state, int x, int y)
{


    if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN)        
    {
        x_pos_old=x;        // przypisanie aktualnie odczytanej pozycji kursora 
        y_pos_old=y;               // jako pozycji poprzedniej
        status = 1;         // wci?ni?ty zosta? lewy klawisz myszy
    }
	else status = 0;
	if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old=x;        // przypisanie aktualnie odczytanej pozycji kursora 
        y_pos_old=y;               // jako pozycji poprzedniej
        status2 = 1;
	}
    else status2 =0;

                 // nie zosta? wci?ni?ty ?aden klawisz 
}

void Motion( GLsizei x, GLsizei y )
{
    
    delta_x=x-x_pos_old;    // obliczenie ró?nicy po?o?enia kursora myszy
	delta_y=y-y_pos_old;
    x_pos_old=x;            // podstawienie bie?acego po?o?enia jako poprzednie
	y_pos_old=y;
    glutPostRedisplay();    // przerysowanie obrazu sceny
}
#pragma endregion input 



#pragma region
void RenderScene(void)
{
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// Czyszczenie okna aktualnym kolorem czyszcz?cym

glLoadIdentity();
// Czyszczenie macierzy bie??cej

if(status2 == 1)                    // je?li lewy klawisz myszy wci?ni?ty
{
   R+= delta_y*pix2angle*0.1f;							   // modyfikacja k?ta obrotu o k?t proporcjonalny
}	
if(status == 1)                    // je?li lewy klawisz myszy wci?ni?ty
{
    theta += delta_x*pix2angle;
	theta2 +=delta_y*pix2azymut;
} 


gluLookAt(0,0,10,0,0,0, 0, valueY, 0);

Axes();

glRotatef(theta, 0.0, 1.0, 0.0); 
glRotatef(theta2, 1.0, 0.0, 0.0);
glScalef(R,R,R);

glColor3f(1.0f, 1.0f, 1.0f);
glTranslatef(0, 0, 0);
glutWireTeapot(3.0);

glFlush();

glutSwapBuffers();


 }
void MyInit(void)
{

glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
// Kolor czyszcz?cy (wype?nienia okna) ustawiono na czarny

}


void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

    glMatrixMode(GL_PROJECTION);
    // Prze??czenie macierzy bie??cej na macierz projekcji

    glLoadIdentity();
    // Czyszcznie macierzy bie??cej 

    gluPerspective(70, 1.0, 1.0, 100.0);
    // Ustawienie parametrów dla rzutu perspektywicznego
	pix2angle = 360.0/(float)horizontal; // przeliczenie pikseli na stopnie
	pix2azymut = 360.0/(float)vertical;

    if(horizontal <= vertical)
        glViewport(0, (vertical-horizontal)/2, horizontal, horizontal);

    else
        glViewport((horizontal-vertical)/2, 0, vertical, vertical);
    // Ustawienie wielko?ci okna okna widoku (viewport) w zale?no?ci
    // relacji pomi?dzy wysoko?ci? i szeroko?ci? okna

    glMatrixMode(GL_MODELVIEW);
    // Prze??czenie macierzy bie??cej na macierz widoku modelu  

    glLoadIdentity();
    // Czyszczenie macierzy bie??cej 

}

/*************************************************************************************/

// G?ówny punkt wej?cia programu. Program dzia?a w trybie konsoli

 

void main(void)
{           
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);

glutInitWindowSize(300, 300);
srand(time(NULL));
glutCreateWindow("Rzutowanie perspektywiczne");
               
glutDisplayFunc(RenderScene);
// Okre?lenie, ?e funkcja RenderScene b?dzie funkcj? zwrotn?
// (callback function).  B?dzie ona wywo?ywana za ka?dym razem 
// gdy zajdzie potrzeba przerysowania okna

 
glutReshapeFunc(ChangeSize);
// Dla aktualnego okna ustala funkcj? zwrotn? odpowiedzialn?
// za zmiany rozmiaru okna                        
glutMouseFunc(Mouse);
// Ustala funkcj? zwrotn? odpowiedzialn? za badanie stanu myszy
glutMotionFunc(Motion);
// Ustala funkcj? zwrotn? odpowiedzialn? za badanie ruchu myszy
MyInit();
// Funkcja MyInit() (zdefiniowana powy?ej) wykonuje wszelkie
// inicjalizacje konieczne  przed przyst?pieniem do renderowania 
glEnable(GL_DEPTH_TEST);
// W??czenie mechanizmu usuwania niewidocznych elementów sceny

glutMainLoop();
// Funkcja uruchamia szkielet biblioteki GLUT

}
#pragma endregion glut
 

/*************************************************************************************/

 
 

