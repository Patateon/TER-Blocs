#ifndef PLYFILE_H
#define PLYFILE_H

#include <string>

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMouseEvent>
#include <QFileDialog>


#include "mesh.h"




class PlyFile {
public:
    PlyFile();
    ~PlyFile();

    void loadPlyFile(const std::string& filename, Mesh* mesh);
    void writePlyFile(const std::string& filename, Mesh* mesh);
};


#endif // PLYFILE_H
