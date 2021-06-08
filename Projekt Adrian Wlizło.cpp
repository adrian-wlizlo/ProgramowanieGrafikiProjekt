#include "freeglut/include/GL/glut.h"
#include "freeglut/include/GL/freeglut.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

double r, g, b = 1.0;
double kot = 0;
double kot2 = 0;
double speed = 0.5;
double speed2 = 0.01;

int ktory = 1;

int i = 0;
double kierunekX, kierunekY, kierunekZ;
double rotateX, rotateY, ostatniX, ostatniY;
bool lpm_wcisniety;
double scale = 1.0;
double translX, translY, translZ;
int kamera = 0;

void kb(unsigned char key, int x, int y) {
    switch (key) {
    case '+':
        scale += 0.05;
        break;
    case '-':
        if (scale > 0.05) {
            scale -= 0.05;
        }
        break;
    case 'w':
        translY -= 0.05;
        break;
    case 's':
        translY += 0.05;
        break;
    case 'a':
        translX += 0.05;
        break;
    case 'd':
        translX -= 0.05;
        break;
    case 'y':
        kierunekX += 0.01;
        break;
    case 'h':
        kierunekX -= 0.01;
        break;
    case 'u':
        kierunekY += 0.01;
        break;
    case 'j':
        kierunekY -= 0.01;
        break;
    case 'i':
        kierunekZ += 0.01;
        break;
    case 'k':
        kierunekZ -= 0.01;
        break;
    }
}

void sk(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        if (ktory == 1) {
            speed += 0.1;
        }
        else {
            speed2 += 0.005;
        }
        break;
    case GLUT_KEY_DOWN:
        if (ktory == 1) {
            speed -= 0.1;
        }
        else {
            speed2 -= 0.005;
        }
        break;
    case GLUT_KEY_CTRL_L:
        speed = -speed;
        break;
    case GLUT_KEY_ALT_L:
        if (ktory == 1) {
            ktory = 2;
        }
        else {
            ktory = 1;
        }

    }
}

void mouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            kamera += 1;
            if (kamera == 4) {
                kamera = 0;
            }
        }
    }
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            ostatniX = x;
            ostatniY = y;
            lpm_wcisniety = true;
        }
        if (state == GLUT_UP) {
            lpm_wcisniety = false;
        }
    }

}

void mouseButtonAndMove(int x, int y) {
    if (lpm_wcisniety) {
        rotateX += (x - ostatniX);
        rotateY += (y - ostatniY);
        ostatniX = x;
        ostatniY = y;
    }
}

void mouseMove(int x, int y)
{
    int w, h;
    w = glutGet(GLUT_WINDOW_WIDTH);
    h = glutGet(GLUT_WINDOW_HEIGHT);
    r = 0.5 - x / (float)(w);
    b = 0.5 - y / (float)(h);
    g = 0.2;
}

void init(void) {
    glEnable(GL_DEPTH_TEST);

    GLfloat mat_ambient[] = { 0.329412, 0.223529, 0.027451, 1.000000 };
    GLfloat mat_diffuse[] = { 0.780392, 0.568627, 0.113725, 1.000000 };
    GLfloat mat_specular[] = { 0.992157, 0.941176, 0.807843, 1.000000 };
    GLfloat mat_shininess = { 27.8974 };
    GLfloat light_position[] = { 2.0, 2.0, 4.5, 0.0 };
    GLfloat light_ambient[] = { 1, 1, 1, 1.0 };
    GLfloat light_diffuse[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_specular[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat att_constant = { 0.2 };
    GLfloat att_linear = { 0.05 };
    GLfloat att_quadratic = { 0.001 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

GLuint tex;
GLbyte* loadTexture(const char* FileName, GLint* w, GLint* h, GLint* ImComponents, GLenum* ImFormat) {
    GLbyte* image;
    int ch;

    *ImComponents = GL_RGB8;
    *ImFormat = GL_BGR_EXT;
    GLbyte* pbitsperpixel = NULL;

    image = (GLbyte*)stbi_load(FileName, w, h, &ch, 0);
    if (image == NULL) {
        std::cout << "error";
        exit(1);
    }

    switch (ch)
    {
    case 3:
        *ImFormat = GL_BGR_EXT;
        *ImComponents = GL_RGB8;
        break;
    case 4:
        *ImFormat = GL_BGRA_EXT;
        *ImComponents = GL_RGBA8;
        break;
    case 1:
        *ImFormat = GL_LUMINANCE;
        *ImComponents = GL_LUMINANCE8;
        break;
    };
    return image;
}

void InitText(const char* FileName) {
    GLbyte* pBytes;
    GLint ImWidth, ImHeight, ImComponents;
    GLenum ImFormat;

    glEnable(GL_CULL_FACE);
    pBytes = loadTexture(FileName, &ImWidth, &ImHeight, &ImComponents, &ImFormat);
    glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);
    free(pBytes);

    glEnable(GL_TEXTURE_2D);
    glCullFace(GL_FRONT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

GLfloat Z = 0.39, z = 0.0;


GLUquadricObj* kwadryka = gluNewQuadric();

void kolo(void) {
    glPushMatrix();

    glRotated(90, 1, 0, 0);
    glutSolidTorus(0.008, 0.05, 20, 30);

    glPushMatrix();
    glScaled(0.02, 0.1, 0.01);
    glutSolidCube(1);
    glPopMatrix();

    glRotated(66, 0, 0, 1);

    glPushMatrix();
    glScaled(0.02, 0.1, 0.01);
    glutSolidCube(1);
    glPopMatrix();

    glRotated(66, 0, 0, 1);

    glPushMatrix();
    glScaled(0.02, 0.1, 0.01);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
}
void kolo_samochod(void) {
    glPushMatrix();
    glColor4d(0, 0, 0, 0.5);
    glRotated(90, 1, 0, 0);
    glutSolidTorus(0.02, 0.05, 20, 30);

    glPushMatrix();
    glColor4d(0.5, 0.5, 0.5, 0.5);
    glutSolidTorus(0.015, 0.02, 20, 30);
    glPopMatrix();

    glPushMatrix();
    glColor4d(0, 0, 0, 0.5);
    glutSolidTorus(0.008, 0.008, 20, 30);
    glPopMatrix();

    glPushMatrix();
    glColor4d(0, 0, 0, 0.5);
    glScaled(0.01, 0.01, 0.01);
    glutSolidCube(1);
    glPopMatrix();

    //---------------sruby

    glPushMatrix();
    glTranslated(0.02, 0, 0);
    glColor4d(0, 0, 0, 0.5);
    glScaled(0.01, 0.01, 0.03);
    glutSolidCube(1);
    glPopMatrix();

    glRotated(70, 0, 0, 1);

    glPushMatrix();
    glTranslated(0.02, 0, 0);
    glColor4d(0, 0, 0, 0.5);
    glScaled(0.01, 0.01, 0.03);
    glutSolidCube(1);
    glPopMatrix();

    glRotated(70, 0, 0, 1);

    glPushMatrix();
    glTranslated(0.02, 0.0, 0);
    glColor4d(0, 0, 0, 0.5);
    glScaled(0.01, 0.01, 0.03);
    glutSolidCube(1);
    glPopMatrix();

    glRotated(70, 0, 0, 1);

    glPushMatrix();
    glTranslated(0.02, 0.0, 0);
    glColor4d(0, 0, 0, 0.5);
    glScaled(0.01, 0.01, 0.03);
    glutSolidCube(1);
    glPopMatrix();

    glRotated(70, 0, 0, 1);

    glPushMatrix();
    glTranslated(0.02, 0.0, 0);
    glColor4d(0, 0, 0, 0.5);
    glScaled(0.01, 0.01, 0.03);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
}

//-------------------------------------------tory
void tory(void) {
    //-----------------szyna
    glPushMatrix();
    glColor3d(1, 1, 1);
    glTranslated(-0.05, 0, 0.01);
    glScaled(0.02, 6, 0.02);
    glutSolidCube(1);
    glPopMatrix();
    //-----------------szyna
    glPushMatrix();
    glColor3d(1, 1, 1);
    glTranslated(0.05, 0, 0.01);
    glScaled(0.02, 6, 0.02);
    glutSolidCube(1);
    glPopMatrix();
    //-----------------podklady
    double i = -3;
    while (i <= 3) {
        glColor3d(0.7, 0.4, 0.01);
        glPushMatrix();
        glTranslated(0, i, 0.0);
        glScaled(0.14, 0.02, 0.02);
        glutSolidCube(1);
        glPopMatrix();
        i = i + 0.05;

    }
}
//-------------------------------------------ulica
void ulica() {
    int i;
    for (i = 0; i <= 360; i = i + 10)
    {
        glPushMatrix();
        glColor3f(0, 0, 0);
        glRotatef(i, 0, 0, 1);

        glTranslatef(1, 0, 0);
        glScalef(0.2, 0.2, 0.01);
        glutSolidCube(1);
        glPopMatrix();
        //----------------pasy
        glPushMatrix();
        glColor3f(1, 1, 1);
        glRotatef(i, 0, 0, 1);

        glTranslatef(1, 0, 0);
        glScalef(0.02, 0.06, 0.015);
        glutSolidCube(1);
        glPopMatrix();
    }
}
//-------------------------------------------podstawa
void podstawa() {
    glColor3f(0, 1, 0);
    glPushMatrix();
    
    InitText("trawa.jpg");
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(3.0, 3.0, 0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(3.0, -3.0, 0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-3.0, -3.0, 0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-3.0, 3.0, 0);
    glEnd();
    glPopMatrix();
}
//-------------------------------------------billboard
void billboard() {
    glColor3f(r, g, b);
    glTranslatef(kierunekX, kierunekY, kierunekZ);
    glPushMatrix;
    glTranslatef(0, 0, 0.1);
    glScalef(0.7, 0.7, 0.7);
    glPushMatrix();
    InitText("logo.png");
    //------------------------przod
    glBegin(GL_QUADS);
    glTexCoord2f(-0.3, 1.0);
    glVertex3f(-0.5, -0.5, 0.5);
    glTexCoord2f(0.7, 1.0);
    glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glTexCoord2f(-0.1, 0.0);
    glVertex3f(-0.5, -0.5, 0.0);
    glEnd();
    //------------------------lewo
    glBegin(GL_QUADS);   
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-0.5, -0.5, 0.0);
    glTexCoord2f(-0.1, 0.0);
    glVertex3f(0.0, 0.5, 0.0);
    glTexCoord2f(-0.3, 1.0);
    glVertex3f(0.0, 0.5, 0.5);
    glTexCoord2f(0.7, 1.0);
    glVertex3f(-0.5, -0.5, 0.5);
    glEnd();
    //------------------------prawo
    glBegin(GL_QUADS);
    glTexCoord2f(-0.3, 1.0);
    glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(0.7, 1.0);
    glVertex3f(0.0, 0.5, 0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.0, 0.5, 0.0);
    glTexCoord2f(-0.1, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0,0,0);
    //------------------------przod
    glBegin(GL_TRIANGLES);;
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0, 0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glEnd();
    //------------------------
    glBegin(GL_TRIANGLES);  
    glVertex3f(-0.5, -0.5, 0.0);
    glVertex3f(0, 0.5, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glEnd();
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(kierunekX, kierunekY, kierunekZ);
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(0.2, -0.2, 0.1);
    glScalef(0.1, 0.1, 0.2);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(-0.2, -0.2, 0.1);
    glScalef(0.1, 0.1, 0.2);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(0.0, 0.2, 0.1);
    glScalef(0.1, 0.1, 0.2);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
}
//-------------------------------------------samochod
void samochod() {
    //-----------------------------------kabina
    //-----------------------------kolo
    glPushMatrix();
    glTranslated(0, 0, 0.068);
    glRotated(kot * 6.28, 0, 1, 0);
    kolo_samochod();
    glTranslated(0, 0.3, 0);
    kolo_samochod();
    glPopMatrix();
    //-----------------------------belka
    glPushMatrix();
    glColor4d(0, 0, 0, 0.5);
    glTranslated(0, 0.16, 0.068);
    glScaled(0.02, 0.30, 0.02);
    glutSolidCube(1);
    glPopMatrix();

    //---------------------   
    glRotated(5, 0, 0, 1);
    //-----------------------------bok
    glPushMatrix();
    glPushMatrix();
    glScaled(0.5, 0.5, 0.5);
    glTranslated(-0.25, 0, -0.025);
    glColor4d(0.9, 0.8, 0.0, 0.5);
    glBegin(GL_POLYGON);
    glVertex3f(0, 0, 1);
    glVertex3f(0.7, 0, 1);
    glVertex3f(0.9, 0, 0.7);
    glVertex3f(0.9, 0, 0.3);
    glVertex3f(0.6, 0, 0.3);
    glVertex3f(0.5, 0, 0.4);
    glVertex3f(0.0, 0, 0.4);
    glEnd();
    glColor4d(0.08, 0.08, 0.08, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(0.0, 0, 0.4);
    glVertex3f(0.1, 0, 0.4);
    glVertex3f(0.1, 0, 0.2);
    glVertex3f(0, 0, 0.2);
    glEnd();
    glColor4d(0.08, 0.08, 0.08, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(0.1, 0, 0.4);
    glVertex3f(0.5, 0, 0.4);
    glVertex3f(0.6, 0, 0.3);
    glVertex3f(0.1, 0, 0.3);
    glEnd();
    glColor4d(0.08, 0.08, 0.08, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(0.4, 0, 0.3);
    glVertex3f(0.9, 0, 0.3);
    glVertex3f(0.9, 0, 0.2);
    glVertex3f(0.4, 0, 0.2);
    glEnd();
    glPopMatrix();

    glTranslated(0, 0.3, 0);

    glPushMatrix();
    glScaled(0.5, 0.5, 0.5);
    glTranslated(-0.25, 0, -0.025);
    glColor4d(0.9, 0.8, 0.0, 0.5);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0, 0.4);
    glVertex3f(0.5, 0, 0.4);
    glVertex3f(0.6, 0, 0.3);
    glVertex3f(0.9, 0, 0.3);
    glVertex3f(0.9, 0, 0.7);
    glVertex3f(0.7, 0, 1);
    glVertex3f(0, 0, 1);
    glEnd();
    glColor4d(0.08, 0.08, 0.08, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0.2);
    glVertex3f(0.1, 0, 0.2);
    glVertex3f(0.1, 0, 0.4);
    glVertex3f(0.0, 0, 0.4);
    glEnd();
    glColor4d(0.08, 0.08, 0.08, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(0.1, 0, 0.3);
    glVertex3f(0.6, 0, 0.3);
    glVertex3f(0.5, 0, 0.4);
    glVertex3f(0.1, 0, 0.4);
    glEnd();
    glColor4d(0.08, 0.08, 0.08, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(0.4, 0, 0.2);
    glVertex3f(0.9, 0, 0.2);
    glVertex3f(0.9, 0, 0.3);
    glVertex3f(0.4, 0, 0.3);
    glEnd();
    glPopMatrix();
    glPopMatrix();
    //-----------------szyba
    glPushMatrix();
    glColor4d(0, 0.4, 0.8, 0.5);
    glTranslated(0.18, 0.152, 0.4);
    glScaled(0.1, 0.33, 0.1);
    glutSolidCube(1);
    glPopMatrix();

    //------------------------------przod
    glPushMatrix();
    glPushMatrix();
    glScaled(0.5, 0.5, 0.5);
    glTranslated(-0.25, 0, -0.025);
    glColor4d(0.9, 0.8, 0.0, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(0.9, 0, 0.7);
    glVertex3f(0.9, 0.6, 0.7);
    glVertex3f(0.9, 0.6, 0.3);
    glVertex3f(0.9, 0, 0.3);
    glEnd();
    glColor4d(0.08, 0.08, 0.08, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(0.9, 0, 0.3);
    glVertex3f(0.9, 0.6, 0.3);
    glVertex3f(0.9, 0.6, 0.2);
    glVertex3f(0.9, 0, 0.2);
    glEnd();
    glColor4d(0, 0.4, 0.8, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(0.7, 0, 1);
    glVertex3f(0.7, 0.6, 1);
    glVertex3f(0.9, 0.6, 0.7);
    glVertex3f(0.9, 0, 0.7);
    glEnd();
    glPopMatrix();
    //-----------------grill
    glPushMatrix();
    glColor4d(0, 0, 0, 0.5);
    glTranslated(0.325, 0.152, 0.250);
    glScaled(0.01, 0.2, 0.1);
    glutSolidCube(1);
    glPopMatrix();
    //-----------------swiatla
    glPushMatrix();
    glColor4d(1, 0, 0, 0.5);
    glTranslated(0.325, 0.030, 0.15);
    glScaled(0.01, 0.04, 0.02);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glColor4d(1, 1, 1, 0.5);
    glTranslated(0.325, 0.030, 0.175);
    glScaled(0.01, 0.04, 0.02);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor4d(1, 0, 0, 0.5);
    glTranslated(0.325, 0.27, 0.15);
    glScaled(0.01, 0.04, 0.02);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glColor4d(1, 1, 1, 0.5);
    glTranslated(0.325, 0.27, 0.175);
    glScaled(0.01, 0.04, 0.02);
    glutSolidCube(1);
    glPopMatrix();

    glPopMatrix();

    //------------------------------gora
    glPushMatrix();
    glPushMatrix();
    glScaled(0.5, 0.5, 0.5);
    glTranslated(-0.25, 0, -0.025);
    glColor4d(0.9, 0.8, 0.0, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 1);
    glVertex3f(0, 0.6, 1);
    glVertex3f(0.7, 0.6, 1);
    glVertex3f(0.7, 0, 1);
    glEnd();
    glPopMatrix();
    //-----------------szyba
    glPushMatrix();
    glColor4d(0, 0.4, 0.8, 0.5);
    glTranslated(0.1, 0.16, 0.489);
    glScaled(0.1, 0.2, 0.01);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    //------------------------------tyl i dol
    glPushMatrix();
    glPushMatrix();
    glScaled(0.5, 0.5, 0.5);
    glTranslated(-0.25, 0, -0.025);
    glColor4d(0.9, 0.8, 0.0, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0.8);
    glVertex3f(0, 0.6, 0.8);
    glVertex3f(0, 0.6, 1);
    glVertex3f(0, 0, 1);
    glEnd();
    glColor4d(0.08, 0.08, 0.08, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0.2);
    glVertex3f(0, 0.6, 0.2);
    glVertex3f(0, 0.6, 0.8);
    glVertex3f(0, 0, 0.8);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glScaled(0.5, 0.5, 0.5);
    glTranslated(-0.25, 0, -0.025);
    glColor4d(0.08, 0.08, 0.08, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(0, 0.6, 0.2);
    glVertex3f(0, 0, 0.2);
    glVertex3f(0.1, 0, 0.2);
    glVertex3f(0.1, 0.6, 0.2);
    glEnd();
    glColor4d(0.08, 0.08, 0.08, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(0.1, 0, 0.2);
    glVertex3f(0.1, 0.6, 0.2);
    glVertex3f(0.1, 0.6, 0.3);
    glVertex3f(0.1, 0, 0.3);
    glEnd();
    glColor4d(0.08, 0.08, 0.08, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(0.1, 0, 0.3);
    glVertex3f(0.1, 0.6, 0.3);
    glVertex3f(0.4, 0.6, 0.3);
    glVertex3f(0.4, 0, 0.3);
    glEnd();
    glColor4d(0.08, 0.08, 0.08, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(0.4, 0.6, 0.3);
    glVertex3f(0.4, 0, 0.3);
    glVertex3f(0.4, 0, 0.2);
    glVertex3f(0.4, 0.6, 0.2);
    glEnd();
    glColor4d(0.08, 0.08, 0.08, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(0.4, 0, 0.2);
    glVertex3f(0.4, 0.6, 0.2);
    glVertex3f(0.9, 0.6, 0.2);
    glVertex3f(0.9, 0, 0.2);
    glEnd();
    glPopMatrix();
    glPopMatrix();
    //-----------------------------------naczepa
    //-----------------------------kolo
    glPushMatrix();
    glPushMatrix();
    glTranslated(-0.5, 0, 0.068);
    glRotated(kot * 6.28, 0, 1, 0);
    kolo_samochod();
    glTranslated(0, 0.3, 0);
    kolo_samochod();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.65, 0, 0.068);
    glRotated(kot * 6.28, 0, 1, 0);
    kolo_samochod();
    glTranslated(0, 0.3, 0);
    kolo_samochod();
    glPopMatrix();
    glPopMatrix();

    //-----------------------------podstawa
    glPushMatrix();
    glColor4d(0.08, 0.08, 0.08, 0.5);
    glTranslated(-0.45, 0.15, 0.15);
    glScaled(0.65, 0.24, 0.15);
    glutSolidCube(1);
    glPopMatrix();
    //-----------------------------podpora
    glPushMatrix();
    glColor4d(0.0, 0.0, 0.0, 0.5);
    glTranslated(-0.65, 0.15, 0.24);
    glScaled(0.08, 0.24, 0.06);
    glutSolidCube(1);
    glPopMatrix();
    //----------------------------panumatyk
    glPushMatrix();
    glTranslated(-0.35, 0.15, 0.2);
    glScaled(0.1, 0.1, 0.1);
    glColor4d(0.5, 0.5, 0.5, 0.5);
    glShadeModel(GL_SMOOTH);
    gluQuadricDrawStyle(kwadryka, GLU_FILL);
    gluQuadricNormals(kwadryka, GLU_SMOOTH);
    gluCylinder(kwadryka, 0.2, 0.2, 0.7, 10, 10);
    glPopMatrix();
    //----------------------------zbiornik
    glPushMatrix();
    glColor4d(0, 0, 0.0, 0.5);
    glTranslated(-0.16, 0.15, 0.3);
    glScaled(0.1, 0.2, 0.2);
    glutSolidCube(1);
    glPopMatrix();
    //----------------------------wydech
    glPushMatrix();
    glTranslated(-0.15, 0.12, 0.38);
    glScaled(0.1, 0.1, 0.1);
    glColor4d(0.5, 0.5, 0.5, 0.5);
    glShadeModel(GL_SMOOTH);
    gluQuadricDrawStyle(kwadryka, GLU_FILL);
    gluQuadricNormals(kwadryka, GLU_SMOOTH);
    gluCylinder(kwadryka, 0.2, 0.2, 1.5, 10, 10);
    glPopMatrix();
    //---------------------------zderzak
    glPushMatrix();
    glColor4d(0, 0, 0.0, 0.5);
    glTranslated(-0.76, 0.08, 0.15);
    glScaled(0.1, 0.05, 0.05);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glColor4d(0, 0, 0.0, 0.5);
    glTranslated(-0.76, 0.21, 0.15);
    glScaled(0.1, 0.05, 0.05);
    glutSolidCube(1);
    glPopMatrix();
    //---------------------------swiatla
    glPushMatrix();
    glColor4d(1, 0, 0, 0.5);
    glTranslated(-0.775, 0.06, 0.21);
    glScaled(0.01, 0.04, 0.02);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor4d(1, 0, 0, 0.5);
    glTranslated(-0.775, 0.24, 0.21);
    glScaled(0.01, 0.04, 0.02);
    glutSolidCube(1);
    glPopMatrix();

    //----------------------------wstawki boczne
    glPushMatrix();
    glColor4d(0.9, 0.8, 0.0, 0.5);
    glTranslated(-0.56, 0.03, 0.2);
    glScaled(0.3, 0.02, 0.05);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor4d(0.9, 0.8, 0.0, 0.5);
    glTranslated(-0.385, 0.03, 0.175);
    glRotated(40, 0, 1, 0);
    glScaled(0.1, 0.02, 0.05);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 0.24, 0);
    glPushMatrix();
    glColor4d(0.9, 0.8, 0.0, 0.5);
    glTranslated(-0.56, 0.03, 0.2);
    glScaled(0.3, 0.02, 0.05);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor4d(0.9, 0.8, 0.0, 0.5);
    glTranslated(-0.385, 0.03, 0.175);
    glRotated(40, 0, 1, 0);
    glScaled(0.1, 0.02, 0.05);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    //----------------------------paka
    glPushMatrix();
    glPushMatrix();
    glColor4d(0.9, 0.8, 0.0, 0.5);
    glTranslated(-0.52, 0.04, 0.395);
    glScaled(0.5, 0.02, 0.25);
    glutSolidCube(1);
    glPopMatrix();
    glTranslated(0, 0.22, 0);
    glPushMatrix();
    glColor4d(0.9, 0.8, 0.0, 0.5);
    glTranslated(-0.52, 0.04, 0.395);
    glScaled(0.5, 0.02, 0.25);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
    //---------------------dol
    glPushMatrix();
    glColor4d(0.9, 0.8, 0.0, 0.5);
    glTranslated(-0.52, 0.15, 0.275);
    glScaled(0.5, 0.22, 0.02);
    glutSolidCube(1);
    glPopMatrix();
    //--------------------przod
    glPushMatrix();
    glColor4d(0.9, 0.8, 0.0, 0.5);
    glTranslated(-0.28, 0.15, 0.4);
    glScaled(0.02, 0.22, 0.255);
    glutSolidCube(1);
    glPopMatrix();
    //--------------------tyl
    glPushMatrix();
    glColor4d(0.9, 0.8, 0.0, 0.5);
    glTranslated(-0.75, 0.15, 0.4);
    glScaled(0.02, 0.22, 0.255);
    glutSolidCube(1);
    glPopMatrix();


}
//-------------------------------------------pociag
void pociag() {
    //---------------------kolo
    glPushMatrix();
    glColor3d(0.5, 0.1, 0.5);
    glTranslated(0, -0.05, 0.062);

    glRotated(kot * 6.28, 0, 1, 0);
    kolo();
    glTranslated(0, 0.1, 0);
    kolo();
    glPopMatrix();

    //---------------------   
    glTranslated(0.20, 0, 0);

    glPushMatrix();
    glColor3d(0.5, 0.1, 0.5);
    glTranslated(0, -0.05, 0.062);

    glRotated(kot * 6.28, 0, 1, 0);
    kolo();
    glTranslated(0, 0.1, 0);
    kolo();
    glPopMatrix();
    glTranslated(-0.1, 0, 0);
    //---------------------podstawa wozka
    glPushMatrix();
    glColor3d(0.1, 0.1, 0.5);
    glTranslated(0, 0, 0.062);
    glScaled(0.3, 0.09, 0.01);
    glutSolidCube(1);
    glPopMatrix();
    //---------------kabina
    glPushMatrix();
    glColor3d(0.1, 0.1, 0.5);
    glTranslated(-0.1, 0, 0.11);
    glScaled(0.09, 0.09, 0.09);
    glutSolidCube(1);
    glPopMatrix();
    //----------ramie kabiny
    glPushMatrix();
    glColor3d(0.1, 0.1, 0.5);
    glTranslated(-0.13, 0.03, 0.2);
    glScaled(0.02, 0.02, 0.09);
    glutSolidCube(1);
    glPopMatrix();
    //----------ramie kabiny
    glPushMatrix();
    glColor3d(0.1, 0.1, 0.5);
    glTranslated(-0.13, -0.03, 0.2);
    glScaled(0.02, 0.02, 0.09);
    glutSolidCube(1);
    glPopMatrix();
    //----------ramie kabiny
    glPushMatrix();
    glColor3d(0.1, 0.1, 0.5);
    glTranslated(-0.07, 0.03, 0.2);
    glScaled(0.02, 0.02, 0.09);
    glutSolidCube(1);
    glPopMatrix();
    //----------ramie kabiny
    glPushMatrix();
    glColor3d(0.1, 0.1, 0.5);
    glTranslated(-0.07, -0.03, 0.2);
    glScaled(0.02, 0.02, 0.09);
    glutSolidCube(1);
    glPopMatrix();
    //----------dach kabiny
    glPushMatrix();
    glColor3d(0.1, 0.1, 0.5);
    glTranslated(-0.1, 0, 0.245);
    glScaled(0.04, 0.04, 0.02);
    glutSolidCone(2, 2, 10, 10);
    glPopMatrix();
    //---------------przod
    glPushMatrix();
    glColor3d(0.1, 0.1, 0.5);
    glTranslated(0.04, 0, 0.09);
    glScaled(0.19, 0.05, 0.05);
    glutSolidCube(1);
    glPopMatrix();
    glRotated(5, 0, 0, 1);
    //---------------komin
    glPushMatrix();
    glColor3d(0.1, 0.1, 0.5);
    glTranslated(0.05, 0, 0.11);
    glScaled(0.1, 0.1, 0.2);
    glShadeModel(GL_SMOOTH);
    gluQuadricDrawStyle(kwadryka, GLU_FILL);
    gluQuadricNormals(kwadryka, GLU_SMOOTH);
    gluCylinder(kwadryka, 0.2, 0.3, 0.5, 10, 10);
    glPopMatrix();


}

void draw(void) {
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (kamera == 0) {
       gluLookAt(0, -1.5, 0.8, 0, 0, 0, 0, 1, 0);//glowna
    }
    else if (kamera == 1) {
        gluLookAt(-1.2, -1.2, 0.01, 0, 0, 0, 0, 0, 1);//z lewej
    }
    else if (kamera == 2) {
        gluLookAt(0, 0, 1.5, 0, 0, 0, 0, 1, 0);//z gory
    }
    else if (kamera == 3) {
        gluLookAt(-1, -1, 1.2, 0, 0, 0, 0, 0, 1);//z gory z lewej
    }

    glTranslated(translX, translY, translZ);
    glScaled(scale, scale, scale);
    glRotated(rotateY, 1.0, 0.0, 0.0);
    glRotated(rotateX, 0.0, 1.0, 0.0);

    glPushMatrix();
    podstawa();
    glPopMatrix();

    glPushMatrix();  
    billboard();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 0, 0.008);
    ulica();
    glPopMatrix();

    glPushMatrix();
    glTranslated(1.5, 0, 0);
    tory();
    glPopMatrix();

    glPushMatrix();
    glRotated(90, 0, 0, 1);
    glTranslated(0, -1.5, 0);
    glTranslated(kot2, 0, 0);
    pociag();
    glPopMatrix();

    glRotated(kot, 0, 0, 1);

    glPushMatrix();
    glTranslated(0, -1.09, 0.0);
    glRotated(-10, 0, 0, 1);
    glScaled(0.3, 0.3, 0.3);   
    samochod();
    glPopMatrix();

    
    kot = kot + speed;
    kot2 = kot2 + speed2;
    if (kot2 > 3) {
        kot2 = -3;
    }
    if (kot2 < -3) {
        kot2 = 3;
    }
    glutSwapBuffers();
}

void reShape(int w, int h) {
    if (w == 0)
        w = 1;
    if (h == 0)
        h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w < h)
        glFrustum(-1.0f, 1.0f, -1.0f * h / w, 1.0f * h / w, 1.0f, 10.0f);
    else
        glFrustum(-1.0f * w / h, 1.0f * w / h, -1.0f, 1.0f, 0.65, 4);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



int main(int argc, char** argv)
{
    if (i == 0) {
        std::cout << "----------------------------Opis funkcji---------------------------" << std::endl;
        std::cout << "+/- - przyblizenie/oddalenie sceny" << std::endl;
        std::cout << "w/s - przesuniecie sceny do przodu/tylu" << std::endl;
        std::cout << "a/d - przesuniecie sceny w lewo/prawo" << std::endl;
        std::cout << "strzalka w gore - zwiekszenie predkosci" << std::endl;
        std::cout << "strzalka w dol - zmniejszenie predkosci" << std::endl;
        std::cout << "lewy ctrl - zmiana kierunku ruchu" << std::endl;
        std::cout << "lewy alt - zmiana obiektu do sterowania na scenie (samochod, pociag)" << std::endl;
        std::cout << "ppm - zmiana kamery" << std::endl;
        std::cout << "lpm + ruch myszy - obrot sceny" << std::endl;
        std::cout << "ruch myszy - zmiana koloru billboardu" << std::endl;
        std::cout << "y/h - ruch baneru w prawo/lewo" << std::endl;
        std::cout << "u/j - ruch baneru do przodu/tylu" << std::endl;
        std::cout << "i/k - ruch baneru do gory/dolu" << std::endl;
        i = 1;
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Projekt zaliczeniowy");
    init();
    glutDisplayFunc(draw);
    glutIdleFunc(draw);
    glutReshapeFunc(reShape);
    glutKeyboardFunc(kb);
    glutSpecialFunc(sk);
    glutMouseFunc(mouseButton);
    glutPassiveMotionFunc(mouseMove);
    glutMotionFunc(mouseButtonAndMove);
    glutMainLoop();
}
