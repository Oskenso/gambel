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

	GLuint m_posAttr;
	GLuint m_colAttr;
	GLuint m_matrixUniform;
	GLuint frame;
	QTimer *timer;
	QOpenGLTexture *texture;

};

#endif // GLRENDER_H
