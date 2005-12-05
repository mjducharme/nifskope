/****************************************************************************
**
** nifscope: a tool for analyzing and editing NetImmerse files (.nif & .kf)
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#ifndef GLVIEW
#define GLVIEW

class NifModel;

#ifdef QT_OPENGL_LIB

#include <QGLWidget>
#include <QCache>
#include <QDateTime>
#include <QFile>
#include <QStack>
#include <QTimer>

#include "nifmodel.h"

#include "glmath.h"

class GLTex;

class GLView : public QGLWidget
{
	Q_OBJECT

public:
	GLView();
	~GLView();
	
	int xRotation() const { return xRot; }
	int yRotation() const { return yRot; }
	int zRotation() const { return zRot; }
	
	bool texturing() const { return texturesOn; }
	bool lighting() const { return lightsOn; }
	bool drawAxis() const { return drawaxis; }
	bool rotate() const { return timer->isActive(); }
	
	QString textureFolder() const { return texfolder; }
	
	QSize minimumSizeHint() const { return QSize( 50, 50 ); }
	QSize sizeHint() const { return QSize( 400, 400 ); }

public slots:
	void setNif( NifModel * );

	void compile( bool center );

	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);
	void setZoom( int zoom );
	void setXTrans( int );
	void setYTrans( int );
	
	void setRotate( bool );
	
	void setDrawAxis( bool );
	void setLighting( bool );
	void setTexturing( bool );
	
	void setTextureFolder( const QString & );
	void flushTextureCache();

signals: 
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);
	void zoomChanged( int zoom );

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent( QMouseEvent * );
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent( QWheelEvent * event );

private slots:
	void advanceGears();
	
	void dataChanged();

private:
	void normalizeAngle(int *angle);
	
	void updateWorldTrans( int b );
	bool compileNode( int b, bool alphatoggle );
	GLuint compileTexture( QString filename );

	GLuint nif;
	
	GLuint click_tex;
	
	int xRot;
	int yRot;
	int zRot;
	int zoom;
	
	int zInc;
	
	int xTrans;
	int yTrans;
	
	bool updated;
	bool doCompile;
	bool doCenter;
	
	QPoint lastPos;
	
	NifModel * model;
	
	QString texfolder;
	
	bool texturesOn;
	bool lightsOn;
	bool drawaxis;
	
	QStack<int> nodestack;
	QStack<Matrix> matrixstack;
	QHash<int,Matrix> worldtrans;
	
	Vector boundMin, boundMax;
	
	QCache<QString,GLTex> textures;
	
	QTimer * timer;
};

class GLTex
{
public:
	static GLTex * create( const QString & filepath, const QGLContext * context );
	
	~GLTex();

	GLuint		id;
	QString		filepath;
	QDateTime	loaded;
	
protected:
	GLTex();
};


#else
class GLView {};
#endif


#endif
