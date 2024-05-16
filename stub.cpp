
/******************************************************************************|
| CPSC 4050/6050 Computer Garphics Assignment 5, Daljit Singh Dhillon, 2020    |
| Reference:                                                                   |
|                                                                              |
| Some OpenGL setup code here including math_funcs, and gl_utils               |
| are from Angton Gerdelan and "Anton's OpenGL 4 Tutorials."                   |
| http://antongerdelan.net/opengl/                                             |
| Email: anton at antongerdelan dot net                                        |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
|******************************************************************************/
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <assert.h>


#include <math.h>
#include <time.h>

#include "maths_funcs.h"   // Anton's maths functions.
#include "gl_utils.h"      // Anton's opengl functions and small utilities like logs
#include "stb_image.h"     // Sean Barrett's image loader with Anton's load_texture()

#include <iostream>
using namespace std;

#define _USE_MATH_DEFINES
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444

mat4 view_mat;
mat4 proj_mat;
mat4 model_mat;

int size;

float lightX = 0.0;
float lightY = 0.0;
float lightZ = 5.0;
float specEx = 4.0;
int diffFlag = 1;
int specFlag = 1;
int texFlag = 1;


int pointCount;

float roundF(float var)
{
    float value = (int)(var * 1000 + .5);
    return (float)value / 1000;
}



void loadSurfaceOfRevolution() 
{
  /*------------------------------CREATE GEOMETRY-------------------------------*/
  
	// VAO -- vertex attribute objects bundle the various things associated with vertices
	GLuint vao;
	glGenVertexArrays (1, &vao);   // generating and binding is common pattern in OpenGL
	glBindVertexArray (vao);       // basically setting up memory and associating it

	// VBO -- vertex buffer object to contain coordinates
	// MODIFY THE FOLLOWING BLOCK OF CODE APPRORIATELY FOR YOUR SURFACE OF REVOLUTION
	double r1 = .2;
	double p1 = 1;

	double r2 = .4;
	double p2 = .6;

	double r3 = .3;
	double p3 = -.2;

	double r4 = .1;
	double p4 = -1;

	int round = 1;
	int y;
	int theta = 7;

	cout << "Enter number of steps along the y dimension: ";
	cin >> y;
	cout << "Enter the theta to rotate by (360 % theta must equal 0): ";
	while (360 % theta != 0) {
	  cin >> theta;
	}
	cout << theta;
	double yStep = 2/(double)y;
	size = (360/theta) * y * 18;

	GLfloat vp[size];
	int count = 0;
  
	for (double j = -1; j < 1; j+= yStep) {
	  double t =  (j+1)/2;
	  double t2 = (j+yStep+1)/2;  //y+1
	  for(int i = 0; i < 360; i+= theta) {
	    // cout << "Round: " << round << endl;
	    //cout << "i: " << i <<endl;
	    round++;
	    double r =  ((1-t)*(1-t)*(1-t)*r4) + (3*(1-t)*(1-t)*t*r3) + (3*(1-t)*t*t*r2)+ (t*t*t*r1); //x
	    double ra2 = ((1-t2)*(1-t2)*(1-t2)*r4) + (3*(1-t2)*(1-t2)*t2*r3) + (3*(1-t2)*t2*t2*r2)+ (t2*t2*t2*r1); //y+1

	    double a;
	    double b;
	    double c;

	    float rad = i * ONE_DEG_IN_RAD;
	    float rad2 = (i + theta) * ONE_DEG_IN_RAD;
      
	    //face 1, vertex 1
	    vp[count] = roundF(r * cos(rad));
	    count++;
	    a = roundF(r * cos(rad));
	    vp[count] = j;
	    count++;
	    b = j;
	    vp[count] = roundF(r * sin(rad));
	    count++;
	    c = roundF(r * sin(rad));
	    //cout << a << ", " << b << ", " << c << endl; 
      
	    //face 1, vertex 2
	    vp[count] = roundF(r * cos(rad2));
	    count++;
	    a = roundF(r*cos(rad2));
	    vp[count] = j;
	    count++;
	    b = j;
	    vp[count] = roundF(r * sin(rad2));
	    count++;
	    c = roundF(r*sin(rad2));
	    //cout << a << ", " << b << ", " << c << endl; 

	    //face 1, vertex 3
	    vp[count] = roundF(ra2 * cos(rad));
	    count++;
	    a = roundF(ra2 * cos(rad));
	    vp[count] = j+yStep;
	    count++;
	    b = j+yStep;
	    vp[count] = roundF(ra2 * sin(rad));
	    count++;
	    c = roundF(ra2*sin(rad));
	    //cout << a << ", " << b << ", " << c << endl; 

	    //face 2, vertex 1
	    vp[count] = roundF(ra2 * cos(rad));
	    count++;
	    a = roundF(ra2 * cos(rad));
	    vp[count] = j+yStep;
	    count++;
	    b = j+yStep;
	    vp[count] = roundF(ra2 * sin(rad));
	    count++;
	    c = roundF(ra2 * sin(rad));
	    //cout << a << ", " << b << ", " << c << endl; 

	    //face 2, vertex 2
	    vp[count] = roundF(r * cos(rad2));
	    count++;
	    a = roundF(r * cos(rad2));
	    vp[count] = j;
	    count++;
	    b = j;
	    vp[count] = roundF(r * sin(rad2));
	    count++;
	    c = roundF(r * sin(rad2));
	    //cout << a << ", " << b << ", " << c << endl; 
     
	    //face 2, vertex 3
	    vp[count] = roundF(ra2 * cos(rad2));
	    count++;
	    a = roundF(ra2*cos(rad2));
	    vp[count] = j+yStep;
	    count++;
	    b=j+yStep;
	    vp[count] = roundF(ra2 * sin(rad2));
	    count++;
	    c = roundF(ra2 * sin(rad2));
	    // cout << a << ", " << b << ", " << c << endl;

	  } //for  
	}//for
	
	GLuint points_vbo;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof (GLfloat), vp, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// VBO -- normals -- needed for shading calcuations
	// ADD CODE TO POPULATE AND LOAD PER-VERTEX SURFACE NORMALS  
	// [HINT] Vertex normals are organized in same order as that for vertex coordinates

		
	GLfloat normals[size];
	count = 0;
	for(int i = 0; i < size; i+= 9) {
	  //U = V2 - V1
	  //V = V3 - V1
	  float Ux = vp[i+3] - vp[i];   //V2(x)-V1(x)
	  float Uy = vp[i+4] - vp[i+1]; //V2(y)-V1(y)
	  float Uz = vp[i+5] - vp[i+2]; //V2(z)-V1(z)
	  float Vx = vp[i+6] - vp[i];   //V3(x)-V1(x) 
	  float Vy = vp[i+7] - vp[i+1]; //V3(y)-V1(y)
	  float Vz = vp[i+8] - vp[i+2]; //V3(z)-V1(z)

	  //calculate normals
	  float Nx = (Uy*Vz) - (Uz*Vy);
	  float Ny = (Uz*Vx) - (Ux*Vz);
	  float Nz = (Ux*Vy) - (Uy*Vx);

	  for (int j = 0; j < 3; ++j) {
	    normals[count] = Nx;
	    count++;
	    normals[count] = Ny;
	    count++;
	    normals[count] = Nz;
	    count++;
	  }
	  
	}
	
	GLuint normals_vbo;
	glGenBuffers(1, &normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	

    // VBO -- vt -- texture coordinates
	// ADD CODE TO POPULATE AND LOAD PER-VERTEX TEXTURE COORDINATES  
	// [HINT] texture coordinates are organized in same order as that for vertex coordinates
	// [HINT] there are two texture coordinates instead of three vertex coordinates for each vertex
	size = (360/theta) * y * 12;
	GLfloat texture[size];
	count = 0;
	for(double i = -1; i < 1; i += yStep) {
	  for(double j = 0; j < 360; j+= theta) {\

	    //face1, vertex 1
	    texture[count] = j / 360; //u
	    count++;
	    texture[count] = (i+1)/2; //v
	    count++;
	    
	    //face 1, vertex 2
	    texture[count] = (j+theta)/360;
	    count++;
	    texture[count] = (i+1)/2;
	    count++;
	    //face 1, vertex 3
	    texture[count] = j / 360;
	    count++;
	    texture[count] = (i+yStep+1)/2;
	    count++;
	    //face 2, vertex 1
	    texture[count] = j / 360;
	    count++;
	    texture[count] = (i+yStep+1)/2;
	    count++;
	    //face 2, vertex 2
	    texture[count] = (j+theta)/360;
	    count++;
	    texture[count] = (i+1)/2;
	    count++;
	    //face 2, vertex 3
	    texture[count] = (j+theta)/360;
	    count++;
	    texture[count] = (i+yStep+1)/2;
	    count++;
	    
	    }
	}

	GLuint texture_vbo;
	glGenBuffers(2, &texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), texture, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);
	
}


	
void loadUniforms(GLuint shader_programme)
{	
/*---------------------------SET RENDERING DEFAULTS---------------------------*/

	// Choose vertex and fragment shaders to use as well as view and proj matrices.
	int model_mat_location = glGetUniformLocation (shader_programme, "model_mat");
	int view_mat_location  = glGetUniformLocation (shader_programme, "view_mat");
	int proj_mat_location  = glGetUniformLocation (shader_programme, "proj_mat");
	
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat.m);
	glUniformMatrix4fv (model_mat_location, 1, GL_FALSE, model_mat.m);
	
	// WRITE CODE TO LOAD OTHER UNIFORM VARIABLES LIKE FLAGS FOR ENABLING OR DISABLING CERTAIN FUNCTIONALITIES

	int lightXLocation = glGetUniformLocation(shader_programme, "lightX");
	glUniform1f(lightXLocation, lightX);
	int lightYLocation = glGetUniformLocation(shader_programme, "lightY");
	glUniform1f(lightYLocation, lightY);
	int lightZLocation = glGetUniformLocation(shader_programme, "lightZ");
	glUniform1f(lightZLocation, lightZ);
	int specExLocation = glGetUniformLocation(shader_programme, "specEx");
	glUniform1f(specExLocation, specEx);
	int diffLocation = glGetUniformLocation(shader_programme, "diffFlag");
	glUniform1i(diffLocation, diffFlag);
	int specLocation = glGetUniformLocation(shader_programme, "specFlag");
	glUniform1i(specLocation, specFlag);
	int texLocation = glGetUniformLocation(shader_programme, "texFlag");
	glUniform1i(texLocation, texFlag);

}

void drawSurfaceOfRevolution()
{
	// MODIFY THIS LINE OF CODE APPRORIATELY FOR YOUR SURFACE OF REVOLUTION
	glDrawArrays(GL_TRIANGLES, 0, size);
}
	
void keyboardFunction(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// MODIFY THIS FUNCTION FOR KEYBOARD INTERACTIVITY
	//GLFW Reference Links:
	// Callback Example: https://www.glfw.org/docs/3.3/input_guide.html#input_key
	// List of Keys: https://www.glfw.org/docs/3.3/group__keys.html
	    
  if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE)) {
    // Close window when esacape is pressed
    glfwSetWindowShouldClose (g_window, 1);
  }

  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    lightX += 0.5;
  }

   if (key == GLFW_KEY_A && action == GLFW_PRESS) {
    lightX -= 0.5;
  }

   if (key == GLFW_KEY_W && action == GLFW_PRESS) {
    lightY += 0.5;
  }

   if (key == GLFW_KEY_S && action == GLFW_PRESS) {
    lightY -= 0.5;
  }

   if (key == GLFW_KEY_E && action == GLFW_PRESS) {
    lightZ += 0.5;
  }

   if (key == GLFW_KEY_D && action == GLFW_PRESS) {
    lightZ -= 0.5;
  }

   if (key == GLFW_KEY_R && action == GLFW_PRESS) {
     specEx += 0.5;
   }

   if (key == GLFW_KEY_F && action == GLFW_PRESS) {
     specEx -= 0.5;
   }

   if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
     if (diffFlag == 1) {
       diffFlag = 0;
     } else {
       diffFlag = 1;
     }
   }

   if (key == GLFW_KEY_X && action == GLFW_PRESS) {
     if (specFlag == 1) {
       specFlag = 0;
     } else {
       specFlag = 1;
     }
   }


   if (key == GLFW_KEY_C && action == GLFW_PRESS) {
      if (texFlag == 1) {
       texFlag = 0;
     } else {
       texFlag = 1;
     }
   }
}


 /*
        size = 18;
	GLfloat vp[18];    // array of vertex points
	
	//face 1, vertex 1
	vp[0] = -1; //x
	vp[1] = -1; //y
	vp[2] = 0; //z
	//face 1, vertex 2
	vp[3] = 1; //x
	vp[4] = -1; //y
	vp[5] = 0; //z
	//face 1, vertex 3
	vp[6] = -1; //x
	vp[7] =  1; //y
	vp[8] =  0; //z
	
	//face 2, vertex 1
	vp[ 9] = -1; //x
	vp[10] =  1; //y
	vp[11] = 0; //z
	//face 2, vertex 2
	vp[12] =  1; //x
	vp[13] = -1; //y
	vp[14] = 0; //z
	//face 2, vertex 3
	vp[15] =  1; //x
	vp[16] =  1; //y
	vp[17] =  0; //z
  */
