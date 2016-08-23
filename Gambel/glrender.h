#ifndef GLRENDER_H
#define GLRENDER_H

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>

#include <QOpenGLFunctions>
//#include <QOpenGLFunctions_4_2_Core>

class GLRender : public QOpenGLWidget
{

public:
	GLRender(QWidget *parent);

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);

	QOpenGLShaderProgram *shaderProgram;
	QOpenGLVertexArrayObject vao;
	QOpenGLBuffer vbo;
	int positionAttr;
	int color_uniform;
	//float vertices[9];
	const char* vert = "#version 130 \n"
					   "in vec3 position; \n"
					   "void main() {\n"
					   "gl_Position = vec4(vec3(0.0f), 1.0); \n"
					   "}";

	const char* frag = "#version 130 \n"
					   "out vec4 color; \n"
					   "void main() \n"
					   "{\n"
					   "color = vec4(vec3(1.0), 1.0);\n"
					   "}";

};

#endif // GLRENDER_H
