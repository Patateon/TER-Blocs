// **************************************************
// GLRK : the OpenGL Refinement Kernel.
// Camera.
// This file is a part of the GLRK library.
// Author : Tamy Boubekeur (Tamy.Boubekeur@labri.fr).
// Copyright (C) 2006 Tamy Boubekeur.
// All rights reserved.
// **************************************************

#include "../headers/Camera.h"
#include <GL/gl.h>

#include <iostream>

using namespace std;

static int _spinning, _moving;
static int _beginu, _beginv;
static float _curquat[4];
static float _x, _y, _z;
static float __zoom;
static bool ini = false;

Camera::Camera () {
  fovAngle = 45.0;
  aspectRatio = 1.0;
  nearPlane = 0.1;
  farPlane = 10000.0;
  
  spinning = 0;
  moving = 0;
  beginu = 0;
  beginv = 0;
  
  trackball (curquat, 0.0, 0.0, 0.0, 0.0);
  x = 0.0; // Set the initial x position
  y = 0.0; // Set the initial y position
  z = 8.0; // Set the initial z position
  _zoom = 0.0;
}

void Camera::lookAt(const QVector3D &target) {
    QVector3D direction = target - QVector3D(x, y, z);
    qDebug()<<direction;
    direction.normalize();

    // Calculer la rotation à partir de la direction désirée
    QQuaternion rotation = QQuaternion::fromDirection(QVector3D(0, 0, -1), direction);

    // Mettre à jour le quaternion de rotation sans modifier la position de la caméra
    QQuaternion newRotation = rotation * QQuaternion(curquat[0], curquat[1], curquat[2], curquat[3]);
    curquat[0] = newRotation.scalar();
    curquat[1] = newRotation.x();
    curquat[2] = newRotation.y();
    curquat[3] = newRotation.z();
}







QMatrix4x4 Camera::getProjectionMatrix() const {
    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(fovAngle, aspectRatio, nearPlane, farPlane);
    return projectionMatrix;
}

void Camera::resize (int _W, int _H) {
  H = _H;
  W = _W;
  glViewport (0, 0, (GLint)W, (GLint)H);
  // Calcul du nouveau rapport d'aspect
  aspectRatio = static_cast<float>(W) / static_cast<float>(H);

  // Mise à jour de la matrice de projection avec le nouveau rapport d'aspect
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  QMatrix4x4 projectionMatrix = getProjectionMatrix();
  const float* data = projectionMatrix.constData();
  glLoadMatrixf(data);
  glMatrixMode (GL_MODELVIEW);
}


void Camera::initPos () {
  if (!ini) {
  _spinning = spinning;
  _moving = moving;;
  _beginu = beginu;
  _beginv = beginv;
  _curquat[0] = curquat[0];
  _curquat[1] = curquat[1];
  _curquat[2] = curquat[2];
  _curquat[3] = curquat[3];
  _x = x;
  _y = y;
  _z = z;;
  __zoom = _zoom;
  ini = true;
  } else {
    spinning = _spinning;
    moving = _moving;;
    beginu = _beginu;
    beginv = _beginv;
    curquat[0] = _curquat[0];
    curquat[1] = _curquat[1];
    curquat[2] = _curquat[2];
    curquat[3] = _curquat[3];
    x = _x;
    y = _y;
    z = _z;;
    _zoom = __zoom;
  } 
}


void Camera::move (float dx, float dy, float dz) {
  x += dx;
  y += dy;
  z += dz;
}


void Camera::beginRotate (int u, int v) {
  beginu = u; 
  beginv = v;
  moving = 1;
  spinning = 0;
}


void Camera::rotate (int u, int v) {
  if (moving) {
    trackball(lastquat,
	      (2.0 * beginu - W) / W,
	      (H - 2.0 * beginv) / H,
	      (2.0 * u - W) / W,
	      (H - 2.0 * v) / H);
    beginu = u;
    beginv = v;
    spinning = 1;
    add_quats (lastquat, curquat, curquat);
  }
}


void Camera::endRotate () {
  moving = false;
}


void Camera::zoom (float z) {
  _zoom += z;
}


void Camera::apply () {
  glLoadIdentity();
  glTranslatef (x, y, z);
  GLfloat m[4][4];
  build_rotmatrix(m, curquat);
  glTranslatef (0.0, 0.0, -_zoom);
  glMultMatrixf(&m[0][0]);
}


void Camera::getPos (float & X, float & Y, float & Z) {
  GLfloat m[4][4]; 
  build_rotmatrix(m, curquat);
  float _x = -x;
  float _y = -y;
  float _z = -z + _zoom;
  X = m[0][0] * _x +  m[0][1] * _y +  m[0][2] * _z;
  Y = m[1][0] * _x +  m[1][1] * _y +  m[1][2] * _z;
  Z = m[2][0] * _x +  m[2][1] * _y +  m[2][2] * _z;

}

