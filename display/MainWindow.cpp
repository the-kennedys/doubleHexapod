/*
 * MainWIndow.cpp
 *
 *  Created on: Aug 17, 2012
 *      Author: paul
 */
#include <cstdlib>
#include <iostream>
extern "C" {
#include "support.h"
#include "input.h"
#include "keys.h"
}
#include "MainWindow.h"



extern bool *keys;
extern int *mouse;
extern struct joystick_t *joy1;

MainWindow::MainWindow() {
	// TODO Auto-generated constructor stub

}

MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}

void MainWindow::initialise() {
	float deltaMove;
	GLfloat size, aspectRatio;


	std::cout << "Context init ..." << std::endl;

	// creates a window and GLES context
	if (makeContext() != 0)
		exit(-1);
	std::cout << "Context Ok" << std::endl;



	// all the shaders have at least texture unit 0 active so
	// activate it now and leave it active
	glActiveTexture(GL_TEXTURE0);

	kmMat4Identity(&view);

	pEye.x = 5;
	pEye.y = -5;
	pEye.z = 5;
	pCenter.x = 0;
	pCenter.y = 0;
	pCenter.z = 0;
	pUp.x = 0;
	pUp.y = 0;
	pUp.z = 1;

	kmVec3Subtract(&viewDir, &pEye, &pCenter);
	kmVec3Normalize(&viewDir, &viewDir);

	kmMat4LookAt(&view, &pEye, &pCenter, &pUp);

	// these two matrices are pre combined for use with each model render
	// the view and projection
	kmMat4Assign(&vp, &projection);
	kmMat4Multiply(&vp, &vp, &view);

	// projection matrix, as distance increases
	// the way the model is drawn is effected
	kmMat4Identity(&projection);
	kmMat4PerspectiveProjection(&projection, 45,
			(float) getDisplayWidth() / getDisplayHeight(), 0.1, 20);

	glViewport(0, 0, getDisplayWidth(), getDisplayHeight());
	// initialises glprint's matrix, shader and texture
	initGlPrint(getDisplayWidth(), getDisplayHeight());

	// we don't want to draw the back of triangles
	// the blending is set up for glprint but disabled
	// while not in use
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.7, 0.7, 0.7, 1);
	glClearDepthf(1.0);

	// count each frame
	int num_frames = 0;

	// set to true to leave main loop
	bool quit = false;

}

void MainWindow::render() {

    float rad;		// radians of rotation based on frame counter
    kmMat4 scaleMatrix;

    // clear the colour (drawing) and depth sort back (offscreen) buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // count the frame and base a rotation on it.
    frame++;
    rad = frame * (0.0175f);

    // rotate the light direction depending on lightAng
    lightDir.x=cos(lightAng/10.);
    lightDir.z=sin(lightAng/10.);
    lightDir.y=0;


    pEye.x=cos(camAng/10.)*7.;
    pEye.y=sin(camAng/10.)*7.;
    pEye.z=5;

    // recalculate the view direction vector used by lighting
    kmVec3Subtract(&viewDir,&pEye,&pCenter);
    kmVec3Normalize(&viewDir,&viewDir);

    // update view matrix for new cam position
    kmMat4LookAt(&view, &pEye, &pCenter, &pUp);

    // these two matrices are pre combined for use with each model render
    // the view and projection
    kmMat4Assign(&vp, &projection);
    kmMat4Multiply(&vp, &vp, &view);


    //drawDoublePod(&pod);

    // see printf documentation for the formatting of variables...
    glPrintf(100, 20, "frame=%i", frame);
    glPrintf(100, 40, "mouse %i  %i   %i", mouse[0],mouse[1],mouse[2]);
	glPrintf(100, 60, "joystick %i,%i  %i",joy1->axis[0],joy1->axis[1],joy1->buttons);
	// swap the front (visible) buffer for the back (offscreen) buffer
	swapBuffers();

}

void MainWindow::shutDown() {
	closeContext();
}
