#include<stdlib.h>
#include<GL/glut.h>
#include<stdio.h>
#include<math.h>



void display();
void draw_gwheel();
void draw_wagon();
void draw_seat();
void place_camera(int);
void set_material(int material);

const int SKY_FRONT=0,SKY_RIGHT=1,SKY_LEFT=2,SKY_BACK=3,SKY_UP=4,SKY_DOWN=5,COLUMBUS=1,COLUMBUS_STAND=2, GWHEEL_RING=3,GWHEEL_TROLLEY=4, GWHEEL_TOP=5, ROLLER_BODY=6, ROLLER_FRAME=7;
int ni=0,prevx=0, rcam=1,bezno,camw=0, roll=0, background=0, cswing=0, gw=0,columbus_color=0, columbus_stand_color=0, gwheel_ring_color=0, gwheel_trolley_color=0, roller_body_color=0;
GLint skybox[6], grass,help=0,x_r=0, y_r=0, z_r=0;
GLfloat viewer[3] = {1.0f, 0.0f, 0.0f},camera[3] = {0.0f, 0.0, 0.0};
GLdouble curr=0, prev=0,gw_spin = 0.0,angle=0.0,c_angle=90.0,gw_width = 8.0, gw_radius=45.0,gw_x=-180.0, gw_y=50.0, gw_z=220.0,co_x=180.0, co_y=0.0, co_z=80.0,lx=50.0,ly=50.0,lz=50.0,bez_prog=0.0,roller_speed=0.0150,gy=0,movcord[3]={-150,-10,200};
double bez[][3]={
	{30,10,-200},{30,10,-110},	{30,10,10},	{30,10,50},	{30,70,100}, {60,90,140}, {80,80,100}, {70,80,70}, {0,50,80}, {-20,30,80},{-45, 25, 60}, {-70,20,50}, {-40,10,30}, {-30,30,10}, {-90,30,20}, {-120,130,60}, {-115,160,75}, {-110, 200, 130}, {-100, 160, 200}, {-90,130,250}, {-50,150,260}, {-25,150,260}, {-5,140,260}, {25,160,260}, {40,130,230}, {55,110,210}, {70, 90, 160}, {65, 100,140}, {45, 110, 110}, {25, 120, 90}, {-50, 140, 100}, {-100, 150, 100}, {-150, 130, 100},{-170,120,110}, {-175,120, 180 },{-120,90,150}, {-170, 110, 120}, {-190, 80,  140}, {-170, 60, 150},/**/  {-160, 50, 160},{-100, 25, 170},{-15, 1.5, 170},{50, 0, 170}, {100, 0, 170}, {160, 0, 170},{200, 20, 170}, {260, 50, 170},{290, 50, 150}, {320, 65, 110},{330, 65, 80},{340, 65, 60}, {350, 80, 30}, {350, 130, 0},{350, 100, -60},{330, 70, -100},{310, 100, -170},{280, 80, -210},{250, 60, -250},{230, 30, -280}, {200, 55, -290}, {170, 35, -270}, {150, 22, -250}, {100, 20, -230},{80, 35, -200},{60, 30, -170}, {35, 20, -150}, {30,10,-115}, {30,10,-105},{30,10,-105}, {30,10,-105}
};


void draw_bezier();
void draw_columbus();
void moveToBezier(double);
double bezier(double, double, double, double, double);
void draw_cyl(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions);

void set_material(int m)
{
	if(m==0)
	{
		float materialGrey[]={0.8,0.8,0.8},materialWhite[]={0.2,0.2,0.2};
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialGrey);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialWhite);
	}

	if(m==COLUMBUS)
	{	float materialColours[][3]={{1,0.6,0.3}, {0.2,0.2,0.2}, {0.6,0.6,0.6}, {0.75,0.164,0.164},{0.601,0.19,1.0}, {1,1,0}}, materialLightBr[]={0.2,0.2,0.0};
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialColours[columbus_color]);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialLightBr);
	}
	if(m==COLUMBUS_STAND)
	{

		float materialColours[][3]={{0.33,0.33,0.33}, {0.9,0.9,0.9}, {0.1,0.1,0.1},{0.5450,0.27,0}, {0,0,0.5}},materialLightBr[]={0.5,0.5,0.1};
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialColours[columbus_stand_color]);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialLightBr);
	}

	if(m==GWHEEL_RING)
	{	float materialColours[][3]={{0.8,0.8,0.0},{0.33,0.33,0.33}, {0.9,0.4,0.4} ,{0.5450,0.27,0}, {0.36,0.36,0.36}},materialLightBr[][3]={{0,0,0},{0.3,0.3,0.3},{0,0,0},{0,0,0},{0.0,0.0,0.0}};
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialColours[gwheel_ring_color]);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialLightBr[gwheel_ring_color]);
	}
	if(m==GWHEEL_TROLLEY)
	{	float materialColours[][3]={{0.2,0.2,0.8},{0.8,0.8,0.0},{0.33,0.33,0.33}, {0.9,0.4,0.4} ,{0.5450,0.27,0}},materialLightBr[]={0.1,0.1,0.1};
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialColours[gwheel_trolley_color]);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialLightBr);
	}

	if(m==ROLLER_BODY)
	{	float materialColours[][3]={{0.6,0.6,0.6},{1,0.6,0.3}, {0.2,0.2,0.2},  {0.75,0.164,0.164},{0.601,0.19,1.0}, {1,1,0}},materialLightBr[]={0.2,0.2,0.0};
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialColours[roller_body_color]);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialLightBr);
	}

}

GLuint LoadBMP(const char *fileName)
{
	FILE *file;
	unsigned char header[54],*data;
	unsigned int dataPos,size,width, height;
	file = fopen(fileName, "rb");
	fread(header, 1, 54, file);				//Windows BMP begin with 54 byte header
	dataPos		= *(int*)&(header[0x0A]);	//dec10, Actual BMP data
	size		= *(int*)&(header[0x22]);	//dec34, BMP Size
	width		= *(int*)&(header[0x12]);	//dec18, Image Width
	height		= *(int*)&(header[0x16]);	//dec22, Image Height
	if (size == NULL)
		size = width * height * 3;
	if (dataPos == NULL)
		dataPos = 54;
	data = new unsigned char[size];
	fread(data, 1, size, file);
	fclose(file);
	GLuint texture;
	glGenTextures(1, &texture);				//Generate (allocate) 1 texture name
	glBindTexture(GL_TEXTURE_2D, texture);	//Bind the 2D texture



	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	//MAG filter
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	//MIN filter

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data); //target, level, internalFormat, width, height,border, format, type, data 
	return texture;
}

void initSky()
{
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);														
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	if(background==0)
	{	skybox[SKY_DOWN] = LoadBMP("BMP11/down.bmp");
		skybox[SKY_FRONT] = LoadBMP("BMP11/front.bmp");
		skybox[SKY_BACK] = LoadBMP("BMP11/back.bmp");
		skybox[SKY_RIGHT] = LoadBMP("BMP11/right.bmp");
		skybox[SKY_LEFT] = LoadBMP("BMP11/left.bmp");
		skybox[SKY_UP] = LoadBMP("BMP11/up.bmp");
		grass=LoadBMP("BMP11/grass_1.bmp");
	}
	else if(background==1)
	{
		skybox[SKY_DOWN] = LoadBMP("BMP12/down.bmp");
		skybox[SKY_FRONT] = LoadBMP("BMP12/front.bmp");
		skybox[SKY_BACK] = LoadBMP("BMP12/back.bmp");
		skybox[SKY_RIGHT] = LoadBMP("BMP12/right.bmp");
		skybox[SKY_LEFT] = LoadBMP("BMP12/left.bmp");
		skybox[SKY_UP] = LoadBMP("BMP12/up.bmp");
		grass=LoadBMP("BMP12/grass_1.bmp");
	}
	else if(background==2)
	{
		skybox[SKY_DOWN] = LoadBMP("BMP13/down.bmp");
		skybox[SKY_FRONT] = LoadBMP("BMP13/front.bmp");
		skybox[SKY_BACK] = LoadBMP("BMP13/back.bmp");
		skybox[SKY_RIGHT] = LoadBMP("BMP13/right.bmp");
		skybox[SKY_LEFT] = LoadBMP("BMP13/left.bmp");
		skybox[SKY_UP] = LoadBMP("BMP13/up.bmp");
		grass=LoadBMP("BMP13/grass_1.bmp");
	}
}

void initLights()
{

	GLfloat whiteSpecularMaterial[] = {1.0,1.0,1.0},light_post0[]={0.0,0.0,10.0,1.0},whiteSpecularLight[] = {1.0, 1.0, 1.0},blackAmbientLight[] = {0.3, 0.3, 0.3},whiteDiffuseLight[] = {1.0, 1.0, 1.0},mShininess[] = {50},twoModel[]={GL_TRUE}; 
	glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
   	glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, light_post0);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, twoModel);
 	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
}

void initLightsforGW(){

	GLfloat light_post1[]={0.0,50.0,0.0,1.0},whiteSpecularLight[] = {0.5, 0.5, 0.5},blackAmbientLight[] = {0.3, 0.3, 0.3},whiteDiffuseLight[] = {0.6, 0.6, 0.6},mShininess[] = {128},twoModel[]={GL_TRUE};
	glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, whiteSpecularLight);
   	glLightfv(GL_LIGHT1, GL_AMBIENT, blackAmbientLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, whiteDiffuseLight);
	glLightfv(GL_LIGHT1, GL_POSITION, light_post1);
}

void Draw_Skybox(float x, float y, float z, float width, float height, float length){

	glDisable(GL_DEPTH_TEST);
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,skybox[SKY_UP]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z); 
	glEnd();

	glBindTexture(GL_TEXTURE_2D,skybox[SKY_FRONT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,	z+length); 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,	z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,skybox[SKY_BACK]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);	
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length); 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);		
	glEnd();

	glBindTexture(GL_TEXTURE_2D,skybox[SKY_RIGHT]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z); 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();
 
	glBindTexture(GL_TEXTURE_2D,skybox[SKY_LEFT]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height, z+length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z+length); 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z+length);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,skybox[SKY_DOWN]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z+length); 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y,		z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y,		z+length);
	glEnd();
 	glDisable(GL_TEXTURE_2D);

}



void draw_ground()
{		
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,grass);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(5000,-10,5000);
	glTexCoord2f(800.0f, 0.0f); glVertex3f(5000,-10,-5000);
	glTexCoord2f(800.0f, 800.0f); glVertex3f(-5000,-10,-5000);
	glTexCoord2f(0.0f, 800.0f); glVertex3f(-5000,-10,5000);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glLineWidth(5.0);
	glTranslatef(0.0, -2, 0.0);
	draw_bezier();
	glTranslatef(0.0, 2, 0.0);
}


void draw_gwheel()
{
	int num=12;
	GLUquadricObj *quadric=gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	
	glPushMatrix();
	
	initLightsforGW();
	GLfloat twoModel[]={GL_FALSE};
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, twoModel);
	set_material(GWHEEL_RING);
	glTranslatef(0.0,0.0, gw_width);
	glRotatef(-gw_spin,0,0,1);
	draw_cyl(0,0,0,0,-55,20,1.5,8);
	glTranslatef(0.0,0.0, -gw_width*2);
	draw_cyl(0,0,0,0,-55,-20,1.5,8);
	glTranslatef(0.0,0.0, gw_width*2);
	glRotatef(gw_spin,0,0,1);
	glutSolidTorus(1.0, 35.0, 16, 64);

	twoModel[0]=GL_TRUE;
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, twoModel);
	gluDisk(quadric,0.0, 10.0, 10.0, 1);
	glTranslatef(0.0,0.0, -gw_width*2);
	glutSolidTorus(1.0, 35.0, 16, 64);
	gluDisk(quadric,0.0, 10.0, 10.0, 1);
	glTranslatef(0.0,0.0, gw_width); 

	twoModel[0]=GL_FALSE;
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, twoModel);

	for(int i=0; i<num; i++)
	{
		glEnable(GL_LIGHTING);	
		glPushMatrix();
		glRotatef(360*i/num, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);
		glTranslatef(0.0,0.0, gw_width); 
		draw_cyl(0.0, 45.0, 0.0, 0.0, 45.0, -5.0, 1.0, 12);
		draw_cyl(0.0, 45.0, 0.0, 0.0, 2.0, 0.0, 1.0, 12);
		glTranslatef(0.0,0.0, -gw_width*2); 
		draw_cyl(0.0, 45.0, 0.0, 0.0, 2.0, 0.0, 1.0, 12);
		draw_cyl(0.0, 45.0, 0.0, 0.0, 45.0, 5.0, 1.0, 12);
		glTranslatef(0.0,0.0, gw_width); 
		glDisable(GL_LIGHTING);
		glTranslatef(0.0, -45.0, 0.0);
		glRotatef(-gw_spin-(360*i/num)-sin(gw_spin/10.0)*10.0, 0.0, 0.0, 1.0);
		set_material(GWHEEL_TROLLEY);
		draw_wagon();
		set_material(GWHEEL_RING);
		glTranslatef(0.0, 45.0, 0.0);
		glPopMatrix();

	}
	set_material(0);
	glPopMatrix();
	glDisable(GL_LIGHT1);
}

void draw_columbus()
{
	double eqn[]={0.0, -1.0, 0.0, 0.5};
	glPushMatrix();
	glEnable(GL_LIGHTING);
	GLfloat twoModel[]={GL_FALSE};
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, twoModel);
	set_material(COLUMBUS_STAND);
	draw_cyl(0.0, 105.0,30.0,0.0,105.0,-30.0 ,1.0,10);
	draw_cyl(0.0, 105.0,30.0,30.0,-10.0,30.0 ,1.0,10);
	draw_cyl(0.0, 105.0,30.0,-30.0,-10.0,30.0 ,1.0,10);
	draw_cyl(0.0, 105.0,-30.0,30.0,-10.0,-30.0 ,1.0,10);
	draw_cyl(0.0, 105.0,-30.0,-30.0,-10.0,-30.0 ,1.0,10);
	glTranslatef(0.0, 105.0, 0.0);
	glRotatef(cos(c_angle*3.14/180.0)*50.0, 0.0, 0.0, 1.0);
	draw_cyl(0.0, -70.0,-15.0,0.0,-2.0,-1.0 ,0.3,10);
	draw_cyl(0.0, -70.0,15.0,0.0,-2.0,1.0 ,0.3,10);
	glutSolidTorus(1.0, 3.0, 5, 8);
	twoModel[0]=GL_TRUE;
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, twoModel);
	glTranslatef(0.0, -70.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glEnable(GL_CLIP_PLANE0);
	glScalef(3.0,1.0, 1.0);
	set_material(COLUMBUS);
	glutSolidSphere(15.0, 64.0, 64.0);
	set_material(0);
	glScalef( 1/3.0,1.0, 1.0);
	glDisable(GL_CLIP_PLANE0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glScalef(3.0, 5.0, 3.0);
	glTranslatef(11.5,0.0,0.0);
	for(int c=0;c<6;c++)
	{
		glTranslatef(-3.2, 0.0, 0.0);
		draw_seat();
	}	
	glPopMatrix();
	
}


void draw_wagon(){
	

	GLdouble wagon_size=5.0;
	double eqn[]={0.0, -1.0, 0.0, -3.5};
	glPushMatrix();
	glTranslatef(0.0, -5.0, 0.0);
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glDisable(GL_CLIP_PLANE0);
	for(int c=0;c<4;c++)
	{
		glRotatef((90/4.0), 0.0, 1.0, 0.0);
		glNormal3f(1.0,0.0,0.0);
		glutWireCube(wagon_size*2.0);
	}
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glScalef(2.0, 3.0, 2.0);
	set_material(0);
	draw_seat();
	set_material(GWHEEL_TROLLEY);
	glScalef(1/2.0,1/3.0,1/2.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glPopMatrix();
	glTranslatef(0.0, wagon_size, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(5.0, 5.0, 6,6);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -2*wagon_size, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	GLUquadricObj *quadric=gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluDisk(quadric, 0.0, 6.0, 16.0, 1.0);
	gluDeleteQuadric(quadric);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
	glPopMatrix();

}






void idle()
{
	double bez_offset=0.000;
	if(cswing)
	{
		c_angle++;
		if(camw==2)
		{
			movcord[0]=-co_x-cos(c_angle*3.14/180.0)*50.0;
			movcord[1]=-co_y-35-fabs(cos(c_angle*3.14/180.0))*35;
			movcord[2]=co_z;
		}
}

	if(roll)
	{
		if(ni==bezno-2)
		{
			roll=0;
			bez_prog=0.0;
			ni=0.0;
			viewer[0]=1.0;
			viewer[1]=viewer[2]=camera[0]=camera[1]=camera[2]=x_r=0.0;
			return;
		}
		if(bez_prog>=1.0)
		{
			ni++;
			bez_prog=0.0;
		}	
		bez_prog+=roller_speed;
		moveToBezier(bez_prog+bez_offset);
	}

	if(gw)
	{
		gw_spin+=0.25;
		if(camw==1)
		{
		movcord[0]=-gw_x +(gw_radius*sin(gw_spin*3.14/180.0)) + sin(gw_spin/10.0)-2;
		movcord[2]=gw_z;
		
		movcord[1]=-gw_y-(gw_radius*cos(gw_spin*3.14/180.0))+6;
		}
	}

	display();

}




void draw_seat()
{

glPushMatrix();
glEnable(GL_LIGHTING);
glTranslatef(1.0, 0.0, -1.0);
glRotatef(90.0, 0.0, 0.0, 1.0);
glTranslatef(0.0, 0.5, 0.0);
draw_cyl(-2.5,0.0,0.0,2.5,0.0,0.0,0.05,6);
glTranslatef(0.0,-0.5,0.0);
glNormal3f(0.0,1.0,0.0);
glScalef(4.0, 0.1, 1.0);
glutSolidCube(1.0);
glScalef(1/4.0, 1/0.1, 1.0);
glTranslatef(0.0, 0.5, -1.0);
glRotatef(80.0, 1.0, 0.0, 0.0);
glScalef(4.0, 0.1, 1.0);
glNormal3f(0.0,1.0,0.0);
glutSolidCube(1.0);
glScalef(1/4.0, 1/0.1, 1.0);
glPopMatrix();


}


void draw_wheels()
{
	int i=0;
	glPushMatrix();
		glRotatef(90.0,1.0,0.0,0.0);
		for(i=0;i<4;i++)
		{
	glutSolidTorus(0.2,0.3,4,16);
	glTranslatef(0.0,2.0,0.0);
		}
	
	glPopMatrix();

}


void draw_loco()
{
	int i;
	double eqn[]={1.0, 0.0, 0.0, -0.5};
	double eqnt[]={-1.0, 0.0, 0.0, 1.6};
	glPushMatrix();
	set_material(ROLLER_BODY);
	glEnable (GL_LIGHTING);
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glScalef(1.3, 2.0, 1.5);
	glutSolidCube(3.0);
	glScalef(1/1.3, 1/2.0, 1/1.5);
	glDisable(GL_CLIP_PLANE0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_CLIP_PLANE1);
	glClipPlane(GL_CLIP_PLANE1, eqnt);
	glTranslatef(0.0, 0.0, 3.0);
	glutSolidTorus(0.2, 3.0, 10, 10);
	draw_seat();
	glTranslatef(0.0, 0.0, -1.5);
	glutSolidTorus(0.2, 3.0, 10, 10);
	glTranslatef(0.0, 0.0, -1.5);
	glutSolidTorus(0.2, 3.0, 10, 10);
	draw_seat();
	glTranslatef(0.0, 0.0, -1.5);
	glutSolidTorus(0.2, 3.0, 10, 10);
	glTranslatef(0.0, 0.0, -1.5);
	glutSolidTorus(0.2, 3.0, 10, 10);
	glDisable(GL_CLIP_PLANE1);
	set_material(0);
	glTranslatef(2.5,1.0,0);
	draw_wheels();
	glTranslatef(-2.5,-1.0,0);
	glTranslatef(2.5,-1.0,0);
	draw_wheels();
	for(i=0;i<4;i++)
	{
	draw_cyl(0,2.1,0,0,-0.1,0,0.1,12);
	glTranslatef(0.0,0.0,2.0);
	}

	 glPopMatrix();

	
}


void draw_tank()
{
	GLdouble cuteqn[]={0.0,-1.0,0.0,0.0};
	glPushMatrix();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_CLIP_PLANE1);
	glClipPlane(GL_CLIP_PLANE1, cuteqn);
	glScalef(10.0,1.0,2.0);
	glutSolidCube(20.0);
	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_LIGHTING);
	glColor3f(0.7921,0.8824,0.9);
	glBegin(GL_POLYGON);
	glVertex3f(10,-3,10);
	glVertex3f(10,-3,-10);
	glVertex3f(-10,-3,-10);
	glVertex3f(-10,-3,10);
	glEnd();

	glPopMatrix();


}

void getCurveAt(GLdouble *ax,GLdouble *ay,GLdouble *az,int index,GLdouble atpoint)
{
	if(ax) *ax=-bezier(bez[0+index][0],bez[1+index][0],bez[2+index][0],bez[3+index][0],atpoint);
	if(ay) *ay=-bezier(bez[0+index][1],bez[1+index][1],bez[2+index][1],bez[3+index][1],atpoint);
	if(az) *az=-bezier(bez[0+index][2],bez[1+index][2],bez[2+index][2],bez[3+index][2],atpoint);

}

void drawText(char *string,float x,float y,float z) 
{  
char *c;
glRasterPos3f(x, y,z);
for (c=string; *c != '\0'; c++) 
{
	if(*c=='\n')
	glRasterPos3f(x, y-0.05,z);
	else
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
}
}


void display(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(1.0,1.0,1.0,1.0);
	glLoadIdentity();	
	gluLookAt(viewer[0], viewer[1], viewer[2],camera[0], camera[1], camera[2],0, 1, 0);
	if(help)
	{
		glPushMatrix();
		glColor4f(1.0,1.0,0.0,1.0);
		glDisable(GL_LIGHTING);
		drawText("Amusement Park", 0, 0.55, 0.15);
		drawText("Welcome to the amusement park. \nUse the following keys for movement and action controls:", 0,0.4,0.82);
		drawText("* LEFT and RIGHT arrow: Look Around 360 degrees", 0,0.30,0.82);
		drawText("* UP and DOWN arrow: Move forward and backward", 0,0.25,0.82);
		drawText("* b : Change background", 0,0.2,0.82);
		drawText("* c : Start/stop Columbus Ship", 0,0.15,0.82);
		drawText("* h : Show/hide help menu", 0,0.1,0.82);
		drawText("* g : Start/stop Giant Wheel", 0,0.05,0.82);
		drawText("* r : Start/stop Roller Coaster", 0,0.0,0.82);
		drawText("* w : Change camera position", 0,-0.05,0.82);
		drawText("* + : Move up", 0,-0.10,0.82);
		drawText("* - : Move down", 0,-0.15,0.82);
		drawText("* Drag mouse on the window in left or right direction: Look around 360 degrees", 0,-0.20,0.82);
		drawText("Above actions can also be performed from the right-click context menu", 0,-0.25,0.82);
		drawText("Developed by Karthik A (1BI10CS040)", 0,-0.45,0.82);
		glEnable(GL_LIGHTING);
		glPopMatrix();
	}
	else
	{
	glRotatef(x_r, 0, 1, 0);
	Draw_Skybox(viewer[0]+(0.05*movcord[0]),viewer[1]+(0.05*movcord[1]),viewer[2]+(0.05*movcord[2]),250,250,250);

	glTranslatef(movcord[0],movcord[1],movcord[2]);
	draw_ground();  
	glPushMatrix();
	glTranslatef(80,0,165);
	draw_tank();
	glPopMatrix();
	double tx,ty,tz,nx,ny,nz;
	getCurveAt(&tx,&ty,&tz,ni,bez_prog+0.058);
	getCurveAt(&nx,&ny,&nz,ni,bez_prog+0.070);
	gy=ny;
	float bz1=bezier(bez[0+ni][2],bez[1+ni][2],bez[2+ni][2],bez[3+ni][2],bez_prog+0.02)-1*fabs(cos(angle*3.14/180.0));
	float bx1=bezier(bez[0+ni][0],bez[1+ni][0],bez[2+ni][0],bez[3+ni][0],bez_prog+0.02)-1*fabs(sin(angle*3.14/180.0));
	float bz2=bezier(bez[0+ni][2],bez[1+ni][2],bez[2+ni][2],bez[3+ni][2],bez_prog+0.02)+1*fabs(cos(angle*3.14/180.0));
	float bx2=bezier(bez[0+ni][0],bez[1+ni][0],bez[2+ni][0],bez[3+ni][0],bez_prog+0.02)+1*fabs(sin(angle*3.14/180.0));
	double degreer = atan2(1,bx2-bx1)*fabs(sin(angle*3.14/180.0))* 180 / 3.14+fabs(cos(angle*3.14/180.0))*atan2(1,bz2-bz1)* 180 / 3.14;
	double angler = degreer ;
	double degree= atan2(nz-tz, nx-tx);
	angle = degree * 180 / 3.14;
	double degreey= atan2(ny-ty,1);
	double angley = degreey * 180 / 3.14;
	glPushMatrix();
	glTranslatef(-nx,-ny,-nz);
	glRotatef(-angle, 0.0, 1.0, 0.0);
	glRotatef(angley-90, 0, 0, 1.0);
	glRotatef(angler-45, 0.0, 1.0, 0.0);
	if(camw==0) angle=90.0;
	glTranslatef(-2.5, 3.0, 0.0);
	draw_loco();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(co_x, co_y, -co_z);
	draw_columbus();
	glPopMatrix();
	glPushMatrix();
	
	glTranslatef(gw_x, gw_y, -gw_z);
	glRotatef(gw_spin, 0.0, 0.0, 1.0);
	draw_gwheel();
	glPopMatrix();

	}
	glutSwapBuffers();

}





void displayReshape(int width,int height)
{
	glViewport(0,0,width,height);						
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();
	gluPerspective(65,(GLfloat)width/(GLfloat)height,0.01f,1000.0f);
	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();			
}

double bezier(double x0, double x1, double x2, double x3, double t)
{
	return	0.5*((2.0f*x1)+(-x0+x2)*t+(2.0f*x0-5.0f*x1+4*x2-x3)*t*t+(-x0+3.0f*x1-3.0f*x2+x3)*t*t*t);
}

void moveToBezier( double t){
  
	
    int n=0.0;   
			viewer[0]=1.0;
			viewer[1]=0.0;
			viewer[2]=0.0;
			
			if(camw==3)
			{
				

				getCurveAt(&movcord[0],&movcord[1], &movcord[2], ni, t);
				movcord[0]+=1.0;
				movcord[1]-=3.5;

				camera[0]=bezier(bez[0+ni][0],bez[1+ni][0],bez[2+ni][0],bez[3+ni][0],t+0.1)-bezier(bez[0+ni][0],bez[1+ni][0],bez[2+ni][0],bez[3+ni][0],t);
	            camera[1]=bezier(bez[0+ni][1],bez[1+ni][1],bez[2+ni][1],bez[3+ni][1],t+0.1)-bezier(bez[0+ni][1],bez[1+ni][1],bez[2+ni][1],bez[3+ni][1],t);
	            camera[2]=bezier(bez[0+ni][2],bez[1+ni][2],bez[2+ni][2],bez[3+ni][2],t+0.1)-bezier(bez[0+ni][2],bez[1+ni][2],bez[2+ni][2],bez[3+ni][2],t);
				


			if(gy<movcord[1]+2.5)
				 movcord[1]=gy-2.5;
			display();
			}
}



void renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric)
{

float vx = x2-x1,vy = y2-y1,vz = z2-z1;
if(vz == 0) vz = .0001;
float v = sqrt( vx*vx + vy*vy + vz*vz );
float ax = 57.2957795*acos( vz/v );
if ( vz < 0.0 )
    ax = -ax;
float rx = -vy*vz;
float ry = vx*vz;
glPushMatrix();
glTranslatef( x1,y1,z1 );
glRotatef(ax, rx, ry, 0.0);
gluQuadricOrientation(quadric,GLU_OUTSIDE);
gluCylinder(quadric, radius, radius, v, subdivisions, 1);
gluQuadricOrientation(quadric,GLU_INSIDE);
gluDisk( quadric, 0.0, radius, subdivisions, 1);
glTranslatef( 0,0,v );
gluQuadricOrientation(quadric,GLU_OUTSIDE);
gluDisk( quadric, 0.0, radius, subdivisions, 1);
glPopMatrix();

}
void draw_cyl(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions)
{
GLUquadricObj *quadric=gluNewQuadric();
gluQuadricNormals(quadric, GLU_SMOOTH);
renderCylinder(x1,y1,z1,x2,y2,z2,radius,subdivisions,quadric);
gluDeleteQuadric(quadric);
}



void draw_bezier()
{

	glPushMatrix();
      GLfloat bx,by,bz,bx1,by1,bz1,bx2,by2,bz2,i=0.0;
	  int n=0;
      glColor3f(0.0,0.0,0.0);
	  glColor3f(1.0,1.0,0.0);
	  glDisable(GL_LIGHTING);
	  glEnable(GL_DEPTH_TEST);
	   for(n=0; n<bezno-2; n++)
	   {
		
        for(i=0.0;i<=1.0;i+=0.015)
       {	glBegin(GL_LINES);
			bx1=bezier(bez[0+n][0],bez[1+n][0],bez[2+n][0],bez[3+n][0],i)-1.3*fabs(sin(angle*3.14/180.0));
            by1=bezier(bez[0+n][1],bez[1+n][1],bez[2+n][1],bez[3+n][1],i);
            bz1=bezier(bez[0+n][2],bez[1+n][2],bez[2+n][2],bez[3+n][2],i)-1.3*fabs(cos(angle*3.14/180.0));
            glVertex3f(bx1,by1,bz1);
			bx2=bezier(bez[0+n][0],bez[1+n][0],bez[2+n][0],bez[3+n][0],i)+1.3*fabs(sin(angle*3.14/180.0));
            by2=bezier(bez[0+n][1],bez[1+n][1],bez[2+n][1],bez[3+n][1],i);
            bz2=bezier(bez[0+n][2],bez[1+n][2],bez[2+n][2],bez[3+n][2],i)+1.3*fabs(cos(angle*3.14/180.0));
			glVertex3f(bx2,by2,bz2);
			glEnd();
		}
		glBegin(GL_LINE_STRIP);
		for(i=0.0;i<=1.00;i+=0.02)
       {
       		bz1=bezier(bez[0+n][2],bez[1+n][2],bez[2+n][2],bez[3+n][2],i+0.02)-1.0*fabs(cos(angle*3.14/180.0));
			bx1=bezier(bez[0+n][0],bez[1+n][0],bez[2+n][0],bez[3+n][0],i+0.02)-1.0*fabs(sin(angle*3.14/180.0));
			by1=bezier(bez[0+n][1],bez[1+n][1],bez[2+n][1],bez[3+n][1],i+0.02);
			glVertex3f(bx1,by1,bz1);
		}
		glEnd();
		glBegin(GL_LINE_STRIP);
        for(i=0.0;i<=1.0;i+=0.02)
       {
            bz1=bezier(bez[0+n][2],bez[1+n][2],bez[2+n][2],bez[3+n][2],i+0.02)+1.0*fabs(cos(angle*3.14/180.0));
			bx1=bezier(bez[0+n][0],bez[1+n][0],bez[2+n][0],bez[3+n][0],i+0.02)+1.0*fabs(sin(angle*3.14/180.0));
		    by1=bezier(bez[0+n][1],bez[1+n][1],bez[2+n][1],bez[3+n][1],i+0.02);
			glVertex3f(bx1,by1,bz1);
		}
		glEnd();
		glLineWidth(2.0);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		for(i=0.0;i<=1.0;i+=1.00)
       {	  bx=bezier(bez[0+n][0],bez[1+n][0],bez[2+n][0],bez[3+n][0],i);
              by=bezier(bez[0+n][1],bez[1+n][1],bez[2+n][1],bez[3+n][1],i);
              bz=bezier(bez[0+n][2],bez[1+n][2],bez[2+n][2],bez[3+n][2],i);
			  draw_cyl(bx,by-0.5,bz,bx,-10.0,bz,0.5,12);
		}
		glDisable(GL_LIGHTING);
	   	glLineWidth(2.0);

	   }
        glFlush();
		glPopMatrix();
}

void windowSpecial(int key,int x,int y){
	
	if(key==GLUT_KEY_UP)
	{
		movcord[0]+=5*cos(-1*x_r*3.14/180.0);
		movcord[2]+=5*sin(1*x_r*3.14/180.0);
	}
	if(key== GLUT_KEY_DOWN)
	{
		movcord[0]-=5*cos(-1*x_r*3.14/180.0);
		movcord[2]-=5*sin(1*x_r*3.14/180.0);
	}
	if(key==GLUT_KEY_RIGHT) x_r+=3;
	if(key==GLUT_KEY_LEFT) x_r-=3;
	display();

}


void  kb(unsigned char key, int x, int y)
{   if(key=='+') movcord[1]--;
	if(key=='-') movcord[1]++;
	if(key=='b') {
		background=(background+1)%3;
		initSky();
	}
	if(key=='w') place_camera(camw=(++camw)%4);
	
	if(key=='h')	help==0?(help=1):(help=0);
	if(key=='g')	gw==0?(gw=1):(gw=0);
	if(key=='c')	cswing==0?(cswing=1):(cswing=0);
	if(key=='r')
	{	roll==0?(roll=1):(roll=0);
		viewer[0]=1.0;
		viewer[1]=viewer[2]=camera[0]=camera[1]=camera[2]=x_r=0.0;
	}
	display();
}

void handleMouse(int x,int y)
{
	if((x-prevx)>=0)
	x_r+=1;
	else
	x_r-=1;
	prevx=x;
}

void passiveMouse(int x, int y)
{
	prevx=x;
}


void place_camera(int action)
{

	if(camw==3)
		x_r=0;
	camw=action;
	if(camw==2)
	{	movcord[0]=-co_x+cos(c_angle*3.14/180.0)*50.0;
		movcord[1]=-co_y-38;
		movcord[2]=co_z;
	}
	if(camw==3)
	{	moveToBezier(bez_prog);
		viewer[0]=1.0;
		viewer[1]=viewer[2]=camera[0]=camera[1]=camera[2]=0.0;
		x_r=-45;
	}
	if(camw==1)
	{	

		movcord[0]=-gw_x +(gw_radius*sin(gw_spin*3.14/180.0)) + sin(gw_spin/10.0);
		movcord[2]=gw_z;
		movcord[1]=-gw_y-(gw_radius*cos(gw_spin*3.14/180.0))+6;
	}
	if(camw==0)
		viewer[1]=viewer[2]=camera[0]=camera[1]=camera[2]=0.0;
}

void handle_gwheel(int action)
{
	if(action==0) gw==0?(gw=1):(gw=0);
}

void handle_columbus(int action)
{
	if(action==0) cswing==0?(cswing=1):(cswing=0);
}
void handle_roller(int action)
{
	if(action==0) {
		roll==0?(roll=1):(roll=0);
		viewer[0]=1.0;
		viewer[1]=viewer[2]=camera[0]=camera[1]=camera[2]=x_r=0.0;
	}
}


void menu(int action)
{
	if(action==0) {
		background=(background+1)%3;
		initSky();}
	if(action==1) help==1?help=0:help=1;
	if(action==2)	exit(0);
}

void handle_columbus_body(int action)
{
	columbus_color=action;
}

void handle_columbus_stand(int action)
{
	columbus_stand_color=action;
}

void handle_gwheel_ring(int action)
{
	gwheel_ring_color=action;
}

void handle_gwheel_trolley(int action)
{
	gwheel_trolley_color=action;
}
void handle_roller_color(int action)
{

	roller_body_color=action;
}

void addMenu()
{
	int submenu1,submenu2,submenu21, submenu22,submenu3, submenu31, submenu32, submenu4, submenu41;
	submenu1 = glutCreateMenu(place_camera);
	glutAddMenuEntry("Free Movement",0);
	glutAddMenuEntry("Inside Giant Wheel",1);
	glutAddMenuEntry("On Columbus ship",2);
	glutAddMenuEntry("On Roller Coaster",3);
	submenu21 = glutCreateMenu(handle_gwheel_ring);
	glutAddMenuEntry("Yellow",0);
	glutAddMenuEntry("Silver",1);
	glutAddMenuEntry("Pale red",2);
	glutAddMenuEntry("Brown",3);
	glutAddMenuEntry("Grey",4);
	submenu22 = glutCreateMenu(handle_gwheel_trolley);
	glutAddMenuEntry("Blue",0);
	glutAddMenuEntry("Yellow",1);
	glutAddMenuEntry("Grey",2);
	glutAddMenuEntry("Pale red",3);
	glutAddMenuEntry("Brown",4);
	submenu2 = glutCreateMenu(handle_gwheel);
	glutAddMenuEntry("Stop/Start Giant Wheel",0);
	glutAddSubMenu("Giant Wheel frame colour",submenu21);
	glutAddSubMenu("Giant Wheel trolley colour",submenu22);
	submenu31 = glutCreateMenu(handle_columbus_body);
	glutAddMenuEntry("Light Brown",0);
	glutAddMenuEntry("Black",1);
	glutAddMenuEntry("Grey",2);
	glutAddMenuEntry("Dark Brown",3);
	glutAddMenuEntry("Purple",4);
	submenu32 = glutCreateMenu(handle_columbus_stand);
	glutAddMenuEntry("Light Grey",0);
	glutAddMenuEntry("White",1);
	glutAddMenuEntry("Dark Grey",2);
	glutAddMenuEntry("Brown",3);
	glutAddMenuEntry("Dark Blue",4);
	submenu3 = glutCreateMenu(handle_columbus);
	glutAddMenuEntry("Stop/Start Columbus ship",0);
	glutAddSubMenu("Columbus body colour",submenu31);
	glutAddSubMenu("Columbus stand colour",submenu32);
	submenu41 = glutCreateMenu(handle_roller_color);
	glutAddMenuEntry("Silver",0);
	glutAddMenuEntry("Gold",1);
	glutAddMenuEntry("Dark Grey",2);
	glutAddMenuEntry("Brown",3);
	glutAddMenuEntry("Purple",4);
	submenu4 = glutCreateMenu(handle_roller);
	glutAddMenuEntry("Stop/Start Roller Coaster",0);
	glutAddSubMenu("Roller Coaster colour",submenu41);
	glutCreateMenu(menu);
	glutAddSubMenu("Camera Position",submenu1);
	glutAddSubMenu("Giant Wheel",submenu2);
	glutAddSubMenu("Columbus ship",submenu3);
	glutAddSubMenu("Roller Coaster",submenu4);
	glutAddMenuEntry("Change Background",0);
	glutAddMenuEntry("Show/hide Help",1);
	glutAddMenuEntry("Quit",2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

int main(int argc, char** argv)
{
		bezno=(sizeof(bez)/24)-2;
		glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
		glutInitWindowSize(800,600);
		glutCreateWindow("Amusement Park");
		initLights();
		initSky();
  		glutDisplayFunc(display);
	 	glutReshapeFunc(displayReshape);
	 	glutKeyboardFunc(kb);
		glutMotionFunc(handleMouse);
		glutPassiveMotionFunc(passiveMouse);
		glutIdleFunc(idle);
		glutSpecialFunc(windowSpecial);
		addMenu();
		glutMainLoop();
		return 0;
}


