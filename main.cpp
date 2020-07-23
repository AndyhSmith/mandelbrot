#include <GL/glut.h>
#include <iostream>
#include <complex> 
#include <vector>
using namespace std;

double zoomAmount = 2;
double decrementValueX = 1;
double decrementValueY = 1;
int iterations = 300;
int progress = 0;

bool showMenu = true;
bool showControls = false;
bool showInfo = false;

// Image Size
double width = 700;
double height = 600;

// Plot Window
double xLowerRange = -2;
double xUpperRange = 1;
double yLowerRange = -1;
double yUpperRange = 1;





class Pixel {
    public:
    int xPos;
    int yPos;
    float color;
    Pixel(int x, int y, float newColor) {
        xPos = x;
        yPos = y;
        color = newColor;
    };
};

vector<vector<Pixel>> pixels;

void render (void);

void keyboard(unsigned char c, int x, int y);

void mouse (int button, int state, int x, int y);
void buildImage(); 

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world!");

    // Used to draw pixels
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, width, height,0.0 );

    buildImage();
    glutDisplayFunc(render);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}


void keyboard(unsigned char c, int x, int y) {
    if (c == 27) { // ESC
        exit(0);
    }
    else if (c == 45) { // -
        iterations -= 25;
        glutPostRedisplay();
    }
    else if (c == 43) { // +
        iterations += 25;
        glutPostRedisplay();
    }
};

double map(int OldValueInt, double oldMin, double oldMax, double newMin, double newMax) {
    double oldValue = (double) OldValueInt;
    return (((oldValue - oldMin) * (newMax - newMin)) / (oldMax - oldMin)) + newMin;


}

void mouse (int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON) {
        exit(0);
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        double xPos = map(x,0,width,xLowerRange, xUpperRange);
        double yPos = map(y,0,height,yLowerRange, yUpperRange);
        decrementValueX = decrementValueX / zoomAmount;
        decrementValueY = decrementValueX * (height/ width);
        xUpperRange = xPos + decrementValueX;
        xLowerRange = xPos - decrementValueX;
        yUpperRange = yPos + decrementValueY;
        yLowerRange = yPos - decrementValueY;
        buildImage();
        glutPostRedisplay();
    }
};

void drawPixel(int x, int y, float color){
    
    
        glColor3f(color, color, color);
        glVertex2i(x,y);
    
}

double mandelbrot(std::complex<double> c) {
    std::complex<double> z = 0;
    double n = 0;
    while ((abs(z) <= 4) && (n < iterations)) {
        z = z * z + c;
        
        n += 1;

    }
    
    return n;
}

void draw() {
    
}

void buildImage() {
    cout << "StartCalc\n";
    pixels.clear();
    for (int i = 0; i < width; i++) {
        vector<Pixel> tempPixels;
        for (int j = 0; j < height; j++) {
            std::complex<double> c(xLowerRange + (i / width) * (xUpperRange - xLowerRange), yLowerRange + (j/ height) * (yUpperRange - yLowerRange));
            double m = mandelbrot(c);
            float color = 1 - (m / 200);
            //std::cout << color << std::endl;
            Pixel pixel(i, j, color);
            tempPixels.push_back(pixel);
            //makedrawPixel(i,j, color);

        }
        pixels.push_back(tempPixels);
        
        

    }
    cout << "EndCalc\n";
}

void render (void) {
    
    

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBegin(GL_POINTS);
        for (int i = 0; i < pixels.size(); i++) {
            for (int j = 0; j < pixels.at(i).size(); j++) {
                Pixel pixel = pixels.at(i).at(j);
                drawPixel(pixel.xPos, pixel.yPos, pixel.color);
            }
        }
    
    glEnd();



    void * font = GLUT_BITMAP_9_BY_15;
    // MENU
    if (showMenu) {
        glColor4f(.23,.78,.32,0.5);
        glRecti(0,0,width,20);

        glColor3f(0.0, 0.0, 0.0);
        glRasterPos2i(5, 15);
        string s = "MENU: Hide Menu [H], Controlls [C] Info [I], Exit [ESC]";
        
        for (string::iterator i = s.begin(); i != s.end(); ++i)
        {
            char c = *i;
            glutBitmapCharacter(font, c);
        }
    }

    showControls = true;
    if (showControls) {
        glColor4f(1,.78,.32,0.5);
        glRecti(0,20,width,40);

        glColor3f(0.0, 0.0, 0.0);
        glRasterPos2i(5, 35);
        string hide = "Increase Iterations [+], Decrease Iterations [-]";
        for (string::iterator i = hide.begin(); i != hide.end(); ++i)
        {
            char c = *i;
            glutBitmapCharacter(font, c);
        }
    }

    showInfo = true;
    if (showInfo) {
        glColor4f(1,.78,1,0.5);
        glRecti(0,40,width,60);

        glColor3f(0.0, 0.0, 0.0);
        glRasterPos2i(5, 55);
        string hide = "Iterations: " + to_string(iterations) + ", xMin: " + to_string(xLowerRange) 
        + ", xMax: " + to_string(xUpperRange) + ", yMin: " + to_string(yLowerRange) + ", yMax: " + to_string(yLowerRange);
        for (string::iterator i = hide.begin(); i != hide.end(); ++i)
        {
            char c = *i;
            glutBitmapCharacter(font, c);
        }
    }

    glutSwapBuffers();
}


