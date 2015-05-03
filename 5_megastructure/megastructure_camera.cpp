#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <vector>
#include<GL/glut.h>
#include<glm/glm.hpp>
using namespace std;
#define pi 2*acos(0)
#define ALL 9

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;


struct point
{
	double x,y,z;
	point(double a,double b,double c)
	{
	    x = a/300.0;
	    y = b/300.0;
	    z = c/300.0;
	}
	point() {}
};
vector <point> points_basement_bottom,points_basement_top,points_walkway_top_outer,points_walkway_top_inner;
vector <point> points_walkway_bottom_outer,points_walkway_bottom_inner;
vector <point> points_stairs,points_stairs_railing,points_waterbody,points_pillars,points_railing;
vector <point> points_structure1,points_structure2,points_center_tower,points_structure3;
vector <point> points_structure1_pattern;

class Camera
{
private:
	glm::vec3 eye;
	glm::vec3 u,v,n;
	double viewAngle, aspect, nearDist, farDist;
	void setModelviewMatrix();

public:
	void set(glm::vec3 eye, glm::vec3 look, glm::vec3 up);
	void roll(double angle); // roll it
	void pitch(double angle); // increase pitch
	void yaw(double angle); // yaw it
	void slide(float delU, float delV, float delN); // slide it
};

void Camera::setModelviewMatrix()
{
	float m[16];

	m[0]=(float)u.x; m[4]=(float)u.y; m[8]=(float)u.z; m[12]=(float) -glm::dot(eye,u);
	m[1]=(float)v.x; m[5]=(float)v.y; m[9]=(float)v.z; m[13]=(float) -glm::dot(eye,v);
	m[2]=(float)n.x; m[6]=(float)n.y; m[10]=(float)n.z; m[14]=(float) -glm::dot(eye,n);
	m[3]=0.0f; m[7]=0.0f; m[11]=0.0f; m[15]=1.0f;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m);

}


void Camera::set(glm::vec3 Eye, glm::vec3 look, glm::vec3 up)
{
	eye = Eye;
	n = (eye - look);
	u = glm::cross(up,n);
	n = glm::normalize(n);
	u = glm::normalize(u);
	v = glm::cross(n,u);
	setModelviewMatrix();

}

void Camera::slide(float delU, float delV, float delN)
{
	eye.x += delU * u.x + delV * v.x + delN * n.x;
	eye.y += delU * u.y + delV * v.y + delN * n.y;
	eye.z += delU * u.z + delV * v.z + delN * n.z;
	setModelviewMatrix();
}

void Camera :: roll(double angle)
{

	float cs = cos(angle);
	float sn = sin(angle);
	glm::vec3 t = u;
	u = glm::vec3(cs*t.x - sn*v.x, cs*t.y - sn*v.y, cs*t.z - sn*v.z);
	v = glm::vec3(sn*t.x + cs*v.x, sn*t.y + cs*v.y, sn*t.z + cs*v.z);
	setModelviewMatrix();
}

void Camera :: pitch(double angle)
{

	float cs = cos(angle);
	float sn = sin(angle);
	glm::vec3 t = v;
	v = glm::vec3(cs*t.x - sn*n.x, cs*t.y - sn*n.y, cs*t.z - sn*n.z);
	n = glm::vec3(sn*t.x + cs*n.x, sn*t.y + cs*n.y, sn*t.z + cs*n.z);
	setModelviewMatrix();
}

void Camera :: yaw(double angle)
{

	float cs = cos(angle);
	float sn = sin(angle);
	glm::vec3 t = n;
	n = glm::vec3(cs*t.x - sn*u.x, cs*t.y - sn*u.y, cs*t.z - sn*u.z);
	u = glm::vec3(sn*t.x + cs*u.x, sn*t.y + cs*u.y, sn*t.z + cs*u.z);
	setModelviewMatrix();
}

Camera cam;

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(0, 0, 1);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);
		}glEnd();
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);{
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);
			/*glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);*/
		}glEnd();
		glColor3f(0, 1, 0);
		glBegin(GL_LINES);{
			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

			case '1':
			{
			cam.yaw(-0.05);
			}
			break;

		case '2':
			{
			cam.yaw(0.05);
			}
			break;

			case '3':
			{
			cam.pitch(-0.05);
			}
			break;

		case '4':
			{
			cam.pitch(0.05);
			}
			break;
		case '5':
			{
			cam.roll(0.05);
			}
			break;

		case '6':
			{
			cam.roll(-0.05);
			}
			break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cam.slide(0.0f,0.0f,1.0f);
			break;
		case GLUT_KEY_UP:		// up arrow key
			cam.slide(0.0f,0.0f,-1.0f);
			break;

		case GLUT_KEY_RIGHT:
			cam.slide(1.0f,0.0f,0.0f);
			break;
		case GLUT_KEY_LEFT:
			cam.slide(-1.0f,0.0f,0.0f);
			break;

		case GLUT_KEY_PAGE_UP:
			cam.slide(0.0f,1.0f,0.0f);
			break;
		case GLUT_KEY_PAGE_DOWN:
			cam.slide(0.0f,-1.0f,0.0f);
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN){
                drawgrid = 1 - drawgrid;
			}
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

void DRAW_BASEMENT()
{
    int i;
    int n = points_basement_bottom.size() - 1;
    for(i = 1; i<=n; i++)
    {
        point p1 = points_basement_bottom[i];
        point p2 = points_basement_bottom[i-1];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
    }
    point p1 = points_basement_bottom[0];
    point p2 = points_basement_bottom[n];
    glPushMatrix();{
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);{
            glVertex3f(p1.x,p1.y,p1.z);
            glVertex3f(p2.x,p2.y,p2.z);
        }glEnd();
    }glPopMatrix();

    n = points_basement_top.size() - 1;
    for(i = 1; i<=n; i++)
    {
        p1 = points_basement_top[i];
        p2 = points_basement_top[i-1];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
    }
    p1 = points_basement_top[0];
    p2 = points_basement_top[n];
    glPushMatrix();{
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);{
            glVertex3f(p1.x,p1.y,p1.z);
            glVertex3f(p2.x,p2.y,p2.z);
        }glEnd();
    }glPopMatrix();

    for(i=0; i<=n; i++)
    {
        p1 = points_basement_top[i];
        p2 = points_basement_bottom[i];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
        /*
        p1 = points_basement_top[i+1];
        p2 = points_basement_bottom[i+1];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
        */
    }
}
void connect_top_bottom()
{
    int i,n;
    n = points_walkway_bottom_outer.size() - 1;
    point p1,p2;
    for(i=0; i<=n; i++)
    {
        p1 = points_walkway_bottom_outer[i];
        p2 = points_walkway_top_inner[i];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
    }
}
void DRAW_WALKWAY()
{
    int i,n;
    n = points_walkway_top_outer.size() - 1;
    point p1,p2;
    for(i=1; i<=n; i++)
    {
        p1 = points_walkway_top_outer[i];
        p2 = points_walkway_top_outer[i-1];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
    }
    p1 = points_walkway_top_outer[0];
    p2 = points_walkway_top_outer[n];
    glPushMatrix();{
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);{
            glVertex3f(p1.x,p1.y,p1.z);
            glVertex3f(p2.x,p2.y,p2.z);
        }glEnd();
    }glPopMatrix();

    n = points_walkway_top_inner.size() - 1;
    for(i=1; i<=n; i++)
    {
        p1 = points_walkway_top_inner[i];
        p2 = points_walkway_top_inner[i-1];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
    }
    p1 = points_walkway_top_inner[0];
    p2 = points_walkway_top_inner[n];
    glPushMatrix();{
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);{
            glVertex3f(p1.x,p1.y,p1.z);
            glVertex3f(p2.x,p2.y,p2.z);
        }glEnd();
    }glPopMatrix();

    n = points_walkway_bottom_outer.size() - 1;
    for(i=1; i<=n; i++)
    {
        p1 = points_walkway_bottom_outer[i];
        p2 = points_walkway_bottom_outer[i-1];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
    }
    p1 = points_walkway_bottom_outer[0];
    p2 = points_walkway_bottom_outer[n];
    glPushMatrix();{
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);{
            glVertex3f(p1.x,p1.y,p1.z);
            glVertex3f(p2.x,p2.y,p2.z);
        }glEnd();
    }glPopMatrix();

    n = points_walkway_bottom_inner.size() - 1;
    for(i=1; i<=n; i++)
    {
        p1 = points_walkway_bottom_inner[i];
        p2 = points_walkway_bottom_inner[i-1];
        glPushMatrix();{
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);{
                glVertex3f(p1.x,p1.y,p1.z);
                glVertex3f(p2.x,p2.y,p2.z);
            }glEnd();
        }glPopMatrix();
    }
    p1 = points_walkway_bottom_inner[0];
    p2 = points_walkway_bottom_inner[n];
    glPushMatrix();{
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);{
            glVertex3f(p1.x,p1.y,p1.z);
            glVertex3f(p2.x,p2.y,p2.z);
        }glEnd();
    }glPopMatrix();

    connect_top_bottom();
}

void DRAW_STAIRS()
{
    int i,n,j,m;
    n = points_stairs.size() - 1;
    m = points_stairs_railing.size() - 1;
    point p1,p2,p3,p4;
    for(j=0; j<ALL; j++)
    {
        glPushMatrix();{
            glRotatef(40*j,0,0,1);
            for(i=0; i<=n; i+=4)
            {
                p1 = points_stairs[i];
                p2 = points_stairs[i+1];
                p3 = points_stairs[i+2];
                p4 = points_stairs[i+3];
                glPushMatrix();{
                    glBegin(GL_LINE_LOOP);{
                        glVertex3f(p1.x,p1.y,p1.z);
                        glVertex3f(p2.x,p2.y,p2.z);
                        glVertex3f(p3.x,p3.y,p3.z);
                        glVertex3f(p4.x,p4.y,p4.z);
                    }glEnd();
                }glPopMatrix();
            }
            for(i=0; i<=m; i+=4)
            {
                p1 = points_stairs_railing[i];
                p2 = points_stairs_railing[i+1];
                p3 = points_stairs_railing[i+2];
                p4 = points_stairs_railing[i+3];
                glPushMatrix();{
                    glBegin(GL_LINE_LOOP);{
                        glVertex3f(p1.x,p1.y,p1.z);
                        glVertex3f(p2.x,p2.y,p2.z);
                        glVertex3f(p3.x,p3.y,p3.z);
                        glVertex3f(p4.x,p4.y,p4.z);
                    }glEnd();
                }glPopMatrix();
            }
        } glPopMatrix();
    }
}

void DRAW_WATERBODY()
{
    int i,n,j;
    n = points_waterbody.size() - 1;
    point p1,p2;
    for(j=0; j<ALL; j++)
    {
        glPushMatrix();{
            glRotatef(40*j,0,0,1);
            for(i=1; i<=n; i++)
            {
                p1 = points_waterbody[i];
                p2 = points_waterbody[i-1];
                glPushMatrix();{
                    glColor3f(0, 0, 0);
                    glBegin(GL_LINES);{
                        glVertex3f(p1.x,p1.y,p1.z);
                        glVertex3f(p2.x,p2.y,p2.z);
                    }glEnd();
                } glPopMatrix();
            }
        }glPopMatrix();
    }
}

void DRAW_PILLARS()
{
    int i,n,j;
    n = points_pillars.size() - 1;
    point p1,p2,p3,p4;
    for(j=0; j<ALL; j++)
    {
        glPushMatrix();{
            glRotatef(40*j,0,0,1);
            for(i=1; i<=16; i++)
            {
                p1 = points_pillars[i];
                p2 = points_pillars[i-1];
                glPushMatrix();{
                    glColor3f(0, 0, 0);
                    glBegin(GL_LINES);{
                        glVertex3f(p1.x,p1.y,p1.z);
                        glVertex3f(p2.x,p2.y,p2.z);
                    }glEnd();
                } glPopMatrix();
            }
            for(i=17; i<=29;i+=4)
            {
                p1 = points_pillars[i];
                p2 = points_pillars[i+1];
                p3 = points_pillars[i+2];
                p4 = points_pillars[i+3];
                glPushMatrix();{
                    glColor3f(0, 0, 0);
                    glBegin(GL_LINE_LOOP);{
                        glVertex3f(p1.x,p1.y,p1.z);
                        glVertex3f(p2.x,p2.y,p2.z);
                        glVertex3f(p3.x,p3.y,p3.z);
                        glVertex3f(p4.x,p4.y,p4.z);
                    }glEnd();
                } glPopMatrix();
            }
        }glPopMatrix();
    }
}
void DRAW_RAILING()
{
    int i,n,j;
    n = points_railing.size() - 1;
    point p1,p2;
    for(j=0; j<ALL; j++)
    {
        glPushMatrix();{
            glRotatef(40*j,0,0,1);
            for(i=1; i<=n; i++)
            {
                if(i%13==0) continue;
                p1 = points_railing[i];
                p2 = points_railing[i-1];
                glPushMatrix();{
                    glColor3f(0, 0, 0);
                    glBegin(GL_LINES);{
                        glVertex3f(p1.x,p1.y,p1.z);
                        glVertex3f(p2.x,p2.y,p2.z);
                    }glEnd();
                } glPopMatrix();
            }
        }glPopMatrix();
    }
}
void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius){
	int i;
	int lineAmount = 100; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * pi;
	glBegin(GL_LINE_LOOP);
		for(i = 0; i <= lineAmount;i++) {
			glVertex2f(
			    x + (radius * cos(i *  twicePi / lineAmount)),
			    y + (radius* sin(i * twicePi / lineAmount))
			);
		}
	glEnd();
}
void DRAW_CENTER()
{
    glPushMatrix();{
        glTranslatef(0,0,2.792);
        drawHollowCircle(0,0,27.466);
    }glPopMatrix();
    glPushMatrix();{
        glTranslatef(0,0,3.8607+2.792);
        drawHollowCircle(0,0,27.466);
    }glPopMatrix();
}
void DRAW_STRUCTURE_1()
{
    int i,n,j;
    n = points_structure1.size() - 1;
    point p1,p2;
    for(j=0; j<ALL; j++)
    {
        glPushMatrix();{
            glRotatef(40*j,0,0,1);
            for(i=1; i<=n; i++)
            {
                p1 = points_structure1[i];
                p2 = points_structure1[i-1];
                glPushMatrix();{
                    glColor3f(0, 0, 0);
                    glBegin(GL_LINES);{
                        glVertex3f(p1.x,p1.y,p1.z);
                        glVertex3f(p2.x,p2.y,p2.z);
                    }glEnd();
                } glPopMatrix();
            }
        }glPopMatrix();
    }
}
void DRAW_STRUCTURE_2()
{
    int i,n,j;
    n = points_structure2.size() - 1;
    point p1,p2;
    for(j=0; j<ALL; j++)
    {
        glPushMatrix();{
            glRotatef(40*j,0,0,1);
            for(i=1; i<=n; i++)
            {
                p1 = points_structure2[i];
                p2 = points_structure2[i-1];
                glPushMatrix();{
                    glColor3f(0, 0, 0);
                    glBegin(GL_LINES);{
                        glVertex3f(p1.x,p1.y,p1.z);
                        glVertex3f(p2.x,p2.y,p2.z);
                    }glEnd();
                } glPopMatrix();
            }
        }glPopMatrix();
    }
}
void DRAW_CENTER_TOWER()
{
    int i,n,j;
    n = points_center_tower.size() - 1;
    point p1,p2;
    for(j=0; j<ALL; j++)
    {
        glPushMatrix();{
            glRotatef(40*j,0,0,1);
            for(i=1; i<=n; i++)
            {
                p1 = points_center_tower[i];
                p2 = points_center_tower[i-1];
                glPushMatrix();{
                    glColor3f(0, 0, 0);
                    glBegin(GL_LINES);{
                        glVertex3f(p1.x,p1.y,p1.z);
                        glVertex3f(p2.x,p2.y,p2.z);
                    }glEnd();
                } glPopMatrix();
            }
            p1 = points_center_tower[2];
            p2 = points_center_tower[4];
            glPushMatrix();{
                glColor3f(0, 0, 0);
                glBegin(GL_LINES);{
                    glVertex3f(p1.x,p1.y,p1.z);
                    glVertex3f(p2.x,p2.y,p2.z);
                }glEnd();
            } glPopMatrix();
        }glPopMatrix();
    }
}
void DRAW_STRUCTURE_3()
{
    int i,n,j;
    n = points_structure3.size() - 1;
    point p1,p2;
    for(j=0; j<ALL; j++)
    {
        glPushMatrix();{
            glRotatef(40*j,0,0,1);
            for(i=1; i<=12; i++)
            {
                p1 = points_structure3[i];
                p2 = points_structure3[i-1];
                glPushMatrix();{
                    glColor3f(0, 0, 0);
                    glBegin(GL_LINES);{
                        glVertex3f(p1.x,p1.y,p1.z);
                        glVertex3f(p2.x,p2.y,p2.z);
                    }glEnd();
                } glPopMatrix();
            }
            for(i=14; i<=n; i++)
            {
                p1 = points_structure3[i];
                p2 = points_structure3[i-1];
                glPushMatrix();{
                    glColor3f(0, 0, 0);
                    glBegin(GL_LINES);{
                        glVertex3f(p1.x,p1.y,p1.z);
                        glVertex3f(p2.x,p2.y,p2.z);
                    }glEnd();
                } glPopMatrix();
            }
        }glPopMatrix();
    }
}
void DRAW_STRUCTURE_1_PATTERN()
{
    int i,j,n;
    n = points_structure1_pattern.size();
    point p1,p2,p3,p4;
    for(j=0; j<ALL; j++)
    {
        glPushMatrix();{
            glRotatef(40*j,0,0,1);
            for(i=0; i<=n; i+=4)
            {
                p1 = points_structure1_pattern[i];
                p2 = points_structure1_pattern[i+1];
                p3 = points_structure1_pattern[i+2];
                p4 = points_structure1_pattern[i+3];
                glPushMatrix();{
                    glColor3f(0, 0, 0);
                    glBegin(GL_LINE_LOOP);{
                        glVertex3f(p1.x,p1.y,p1.z);
                        glVertex3f(p2.x,p2.y,p2.z);
                        glVertex3f(p3.x,p3.y,p3.z);
                        glVertex3f(p4.x,p4.y,p4.z);
                    }glEnd();
                } glPopMatrix();
            }
        }glPopMatrix();
    }
}
void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1,1,1,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix

	//initialize the matrix


	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(0,-1,150,	0,0,0,	0,0,1);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

    DRAW_BASEMENT();
    DRAW_WALKWAY();
    DRAW_STAIRS();
    DRAW_WATERBODY();
    DRAW_PILLARS();
    DRAW_RAILING();
    DRAW_CENTER();
    DRAW_STRUCTURE_1();
    //DRAW_STRUCTURE_2();
    //DRAW_CENTER_TOWER();
    //DRAW_STRUCTURE_3();

    DRAW_STRUCTURE_1_PATTERN();
	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate(){
	//angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
    drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	//codes for initialization
	//clear the screen
	glClearColor(0,0,0,0);
	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(100,	1,	1,	10000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

void input()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_basement_bottom.push_back(p);
    }
}
void input2()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_basement_top.push_back(p);
    }
}

void input3()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_walkway_top_outer.push_back(p);
    }
}
void input4()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_walkway_top_inner.push_back(p);
    }
}
void input5()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_walkway_bottom_outer.push_back(p);
    }
}
void input6()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_walkway_bottom_inner.push_back(p);
    }
}
void input7()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_stairs.push_back(p);
    }
}
void input8()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_waterbody.push_back(p);
    }
}
void input9()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_pillars.push_back(p);
    }
}
void input10()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_railing.push_back(p);
    }
}
void input11()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_structure1.push_back(p);
    }
}
void input12()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_structure2.push_back(p);
    }
}
void input13()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_center_tower.push_back(p);
    }
}

void input14()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_structure3.push_back(p);
    }
}
void input15()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_structure1_pattern.push_back(p);
    }
}
void input16()
{
    double a,b,c;
    while(scanf("%lf %lf %lf",&a,&b,&c)!=EOF)
    {
        point p(a,b,c);
        //printf("%f %f %f\n",p.x,p.y,p.z);
        points_stairs_railing.push_back(p);
    }
}
int main(int argc, char **argv){
    freopen("coordinates_basement_bottom.txt","r",stdin);
    input();
    fclose(stdin);
    freopen("coordinates_basement_top.txt","r",stdin);
    input2();
    fclose(stdin);
    freopen("coordinates_walkway_top_outer.txt","r",stdin);
    input3();
    fclose(stdin);
    freopen("coordinates_walkway_top_inner.txt","r",stdin);
    input4();
    fclose(stdin);
    freopen("coordinates_walkway_bottom_outer.txt","r",stdin);
    input5();
    fclose(stdin);
    freopen("coordinates_walkway_bottom_inner.txt","r",stdin);
    input6();
    fclose(stdin);
    freopen("coordinates_stairs.txt","r",stdin);
    input7();
    fclose(stdin);
    freopen("coordinates_waterbody.txt","r",stdin);
    input8();
    fclose(stdin);
    freopen("coordinates_pillars.txt","r",stdin);
    input9();
    fclose(stdin);
    freopen("coordinates_railing.txt","r",stdin);
    input10();
    fclose(stdin);
    freopen("coordinates_structure_1.txt","r",stdin);
    input11();
    fclose(stdin);
    freopen("coordinates_structure_2.txt","r",stdin);
    input12();
    fclose(stdin);
    freopen("coordinates_center_tower.txt","r",stdin);
    input13();
    fclose(stdin);
    freopen("coordinates_structure_3.txt","r",stdin);
    input14();
    fclose(stdin);
    freopen("coordinates_structure_1_pattern.txt","r",stdin);
    input15();
    fclose(stdin);
    freopen("coordinates_stairs_railing.txt","r",stdin);
    input16();
    fclose(stdin);

	glutInit(&argc,argv);
	glutInitWindowSize(850, 700);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Lotus Temple");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

		//setting camera
	glm::vec3 Eye = glm::vec3 (100.0f,100.0f,50.0f);
	glm::vec3 look = glm::vec3 (0.0f,0.0f,0.0f);
	glm::vec3 up = glm::vec3 (0.0f,0.0f,1.0f);
	cam.set(Eye,look,up);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

