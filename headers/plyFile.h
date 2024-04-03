#ifndef PLYFILE_H
#define PLYFILE_H

#include <string>

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMouseEvent>
#include <QFileDialog>


#include "nuageDePoint.h"




class PlyFile {
public:
    PlyFile();
    ~PlyFile();

    void loadPlyFile(const std::string& filename, NuageDePoint* ndp);
    void writePlyFile(const std::string& filename, NuageDePoint* ndp);
};


#endif // PLYFILE_H
