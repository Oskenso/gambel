#include "glrender.h"

GLRender::GLRender(QWidget *parent) : QOpenGLWidget(parent)
{
	frame = 0;
}

void GLRender::initializeGL()
{

	QOpenGLFunctions *gl =  QOpenGLContext::currentContext()->functions();
	gl->glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	gl->glEnable(GL_DEPTH_TEST );
	   glShadeModel(GL_SMOOTH);
	   glEnable(GL_LIGHTING);
	   glEnable(GL_LIGHT0);
	   glEnable(GL_MULTISAMPLE);
	gl->glEnable(GL_TEXTURE_2D);

	uchar *imgData = (uchar*) malloc(sizeof(uchar) * 3 * 160 * 144);
	srand(100);
	for (int i = 0; i < 160 * 144; i+= 3)
	{
		imgData[i+0] = (uchar) rand() % 255;
		imgData[i+1] = (uchar) rand() % 255;
		imgData[i+2] = 255;
	}
	texture = new QOpenGLTexture(QImage(imgData, 160, 144, QImage::Format_RGB888));
	texture->allocateStorage();

	shaderProgram = new QOpenGLShaderProgram();
	shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,
										   "attribute highp vec4 posAttr;\n"
										   "attribute lowp vec4 colAttr;\n"
										   "varying lowp vec4 col;\n"
										   "uniform highp mat4 matrix;\n"
										   "void main() {\n"
										   "   col = colAttr;\n"
										   "   gl_Position = matrix * posAttr;\n"
										   "}\n"
	);
	shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,
										   "uniform sampler2D texture;\n"
										   "varying lowp vec4 col;\n"
										   "void main() {\n"
										   "   gl_FragColor = texture2D(texture, vec2(1.0, 1.0));\n"
										   "}\n"
	);


	shaderProgram->link();

	m_posAttr = shaderProgram->attributeLocation("posAttr");
	m_colAttr = shaderProgram->attributeLocation("colAttr");
	m_matrixUniform = shaderProgram->uniformLocation("matrix");


}


void GLRender::paintGL()
{
	QOpenGLFunctions *gl = QOpenGLContext::currentContext()->functions();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	frame++;
	shaderProgram->bind();
	shaderProgram->setUniformValue("texture", texture->textureId());

	QMatrix4x4 matrix;
	matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
	matrix.translate(0, 0, -2);
	matrix.rotate(100.0f * frame / 60, QVector3D(0, 1, 0));

	shaderProgram->setUniformValue(m_matrixUniform, matrix);

	GLfloat vertices[] = {
		0.0f, 0.707f,
		-0.5f, -0.5f,
		0.5f, -0.5f
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	gl->glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	gl->glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

	gl->glEnableVertexAttribArray(0);
	gl->glEnableVertexAttribArray(1);

	texture->bind();
	gl->glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
	texture->release();

	gl->glDisableVertexAttribArray(1);
	gl->glDisableVertexAttribArray(0);

	shaderProgram->release();


}

void GLRender::resizeGL(int w, int h)
{
	//QOpenGLFunctions *gl =  QOpenGLContext::currentContext()->functions();
	//gl->glClear(GL_COLOR_BUFFER_BIT);
}
