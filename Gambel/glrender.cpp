#include "glrender.h"

GLRender::GLRender(QWidget *parent) : QOpenGLWidget(parent)
{

}

void GLRender::initializeGL()
{

	QOpenGLFunctions *gl =  QOpenGLContext::currentContext()->functions();
	//QOpenGLFunctions *gl = QOpenGLContext::currentContext()->functions();
	gl->glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	//shaderProgram = new QOpenGLShaderProgram(this);

	GLfloat vertices[] = {
		0.0f, 0.707f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};


	shaderProgram = new QOpenGLShaderProgram(this);
	shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vert);
	shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, frag);
	shaderProgram->link();

	vbo.create();
	vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo.bind();
	vbo.allocate(&vertices, sizeof(vertices));
	shaderProgram->bind();
	vao.create();
	vao.bind();



	//shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vert);

	//shaderProgram->bind();
	//positionAttr= shaderProgram->attributeLocation("position");
	//color_uniform = shaderProgram->uniformLocation("color");

	//shaderProgram->setAttributeBuffer("vertex", GL_FLOAT, 0, 3);
	//shaderProgram->enableAttributeArray("vertex");
	//shaderProgram->setAttributeBuffer("position", GL_FLOAT_VEC3, 0, 3);
	vao.release();
	vbo.release();
	shaderProgram->release();



/*
	vao.create();
	vao.bind();
	shaderProgram = new QOpenGLShaderProgram(this);
	//shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vert);
	shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vert);
	shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, frag);
	shaderProgram->link();
	shaderProgram->bind();
	positionAttr= shaderProgram->attributeLocation("position");
	color_uniform = shaderProgram->uniformLocation("color");
	vbo.create();
	vbo.bind();
	vbo.allocate(&vertices, 9*sizeof(float));
	shaderProgram->enableAttributeArray(positionAttr);
	shaderProgram->setAttributeArray(positionAttr, vertices, 3);
	shaderProgram->setUniformValue(color_uniform, 1.0f, 1.0f, 1.0f);

	*/
}


void GLRender::paintGL()
{
	QOpenGLFunctions *gl = QOpenGLContext::currentContext()->functions();
	gl->glClear(GL_COLOR_BUFFER_BIT);
	//vbo.bind();
	//vbo.allocate(vertices, 9 * sizeof(float));
	//shaderProgram->enableAttributeArray(positionAttr);
	//shaderProgram->setAttributeArray(positionAttr, vertices, 3, 0);
	//shaderProgram->setUniformValue(color_uniform, 1.0f, 1.0f, 1.0f);

	shaderProgram->bind();
	vao.bind();
	gl->glDrawArrays(GL_TRIANGLES, 0, 3);
	vao.release();
	shaderProgram->release();

}

void GLRender::resizeGL(int w, int h)
{
	//QOpenGLFunctions *gl =  QOpenGLContext::currentContext()->functions();
	//gl->glClear(GL_COLOR_BUFFER_BIT);
}
