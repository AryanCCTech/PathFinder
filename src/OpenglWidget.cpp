#include "OpenGlWidget.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>

using namespace std;

OpenGlWidget::OpenGlWidget(QWidget* parent)
    : QOpenGLWidget(parent),
    isInitialized(false),
    maxId(0)
{
}

OpenGlWidget::~OpenGlWidget() {
    makeCurrent();
    for (DrawingObject drawingObject : drawingObjects)
    {
        drawingObject.vao->destroy();
        drawingObject.vbo.destroy();
    }
    doneCurrent();
}

int OpenGlWidget::addObject(Data inData)
{
    int doId = buildDrawingObjects(inData);
    return doId;
}

void OpenGlWidget::removeObject(int id)
{
    makeCurrent();
    
    // Find the index of the drawing object with the given id
    for (int i = 0; i < drawingObjs.size(); i++)
    {
        if (drawingObjs[i] == id)
        {
            DrawingObject drawingObject = drawingObjects.at(i);
            drawingObjects.erase(drawingObjects.begin() + i);
            drawingObjs.remove(i);
            drawingObject.vao->destroy();
            drawingObject.vbo.destroy();
            update();
            break;
        }
    }
    
    doneCurrent();
}

QSize OpenGlWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize OpenGlWidget::sizeHint() const
{
    return QSize(800, 800);
}

void OpenGlWidget::sync(float inZoomLevel, QVector3D inRotation, QVector2D inPanOffset)
{
    zoomLevel = inZoomLevel;
    rotation = inRotation;
    panOffset = inPanOffset;
    update();
}

void OpenGlWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.45f, 0.29f, 0.07f, 1.0f);
    loadShaders();
}

void OpenGlWidget::resizeGL(int w, int h) {
    if (isInitialized == true)
    {
        glViewport(0, 0, w, h);
    }
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / float(h), 0.1f, 100.0f);
}

void OpenGlWidget::paintGL()
{
    if (isInitialized == true)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLineWidth(5.0f);
        updateModelViewMatrix();

        for (DrawingObject drawingObject : drawingObjects)
        {
            QOpenGLShaderProgram& shader = (drawingObject.drawStyle == DrawStyle::TRIANGLES) ? shadedShader : wireShader;

            shader.bind();
            shader.setUniformValue("projection", projection);
            shader.setUniformValue("modelView", modelView);

            QVector3D lightPos(0.5f, 0.5f, 10.0f);
            shader.setUniformValue("lightPos", lightPos);
            shader.setUniformValue("viewPos", QVector3D(0.0f, 0.0f, 5.0f));

            GLenum drawMode = (drawingObject.drawStyle == DrawStyle::TRIANGLES) ? GL_TRIANGLES : GL_LINES;

            drawingObject.vao->bind();
            glDrawArrays(drawMode, 0, drawingObject.numVertices);
            drawingObject.vao->release();

            shader.release();
        }
    }
}

bool OpenGlWidget::loadShaders()
{
    bool shaderLoaded = false;
    bool shaderLinked = false;

    shaderLoaded = shadedShader.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/shadedVert.glsl");
    shaderLoaded = shadedShader.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/shadedFrag.glsl");
    shaderLinked = shadedShader.link();

    shaderLoaded = wireShader.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/wireVert.glsl");
    shaderLoaded = wireShader.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/wireFrag.glsl");
    shaderLinked = wireShader.link();

    if (!shaderLoaded || !shaderLinked)
    {
        qDebug() << "Error in loading or linking the shader";
    }
    return shaderLoaded && shaderLinked;
}

void OpenGlWidget::updateModelViewMatrix() {
    modelView.setToIdentity();
    modelView.translate(panOffset.x(), panOffset.y(), -5.0f);
    modelView.scale(zoomLevel);
    modelView.rotate(rotation.x(), 1.0f, 0.0f, 0.0f);
    modelView.rotate(rotation.y(), 0.0f, 1.0f, 0.0f);
    modelView.rotate(rotation.z(), 0.0f, 0.0f, 1.0f);
}

int OpenGlWidget::buildDrawingObjects(Data data)
{
    makeCurrent();
    DrawingObject drawingObject;

    QOpenGLShaderProgram& shader = (data.drawStyle == DrawStyle::TRIANGLES) ? shadedShader : wireShader;

    std::vector<float> vertexData;
    vertexData.insert(vertexData.end(), data.vertices.begin(), data.vertices.end());
    vertexData.insert(vertexData.end(), data.normals.begin(), data.normals.end());
    vertexData.insert(vertexData.end(), data.colors.begin(), data.colors.end());

    drawingObject.vao = new QOpenGLVertexArrayObject();
    drawingObject.vao->create();
    drawingObject.vao->bind();

    drawingObject.vbo.create();
    drawingObject.vbo.bind();
    drawingObject.vbo.allocate(vertexData.data(), static_cast<int>(vertexData.size() * sizeof(float)));

    shader.bind();
    shader.enableAttributeArray(0); // Position attribute
    shader.setAttributeBuffer(0, GL_FLOAT, 0, 3);

    if (data.normals.size() == data.vertices.size())
    {
        shader.enableAttributeArray(1); // Normal attribute
        shader.setAttributeBuffer(1, GL_FLOAT, data.vertices.size() * sizeof(float), 3);
    }

    if (data.colors.size() == data.vertices.size())
    {
        shader.enableAttributeArray(2); // color attribute
        shader.setAttributeBuffer(2, GL_FLOAT, ((data.vertices.size() * sizeof(float)) + (data.normals.size() * sizeof(float))), 3);
    }

    shader.release();
    drawingObject.vbo.release();

    drawingObject.numVertices = data.vertices.size();
    drawingObject.drawStyle = data.drawStyle;
    drawingObjects.push_back(drawingObject);
    update();
    doneCurrent();
    isInitialized = true;

    return maxId++; // Return the index of the drawing object
}

void OpenGlWidget::wheelEvent(QWheelEvent* event) {
    // Zoom in or out
    if (event->angleDelta().y() > 0)
        zoomLevel *= 1.1f;  // Zoom in
    else
        zoomLevel /= 1.1f;  // Zoom out
    update();
    viewChange(zoomLevel, rotation, panOffset);
}

void OpenGlWidget::mousePressEvent(QMouseEvent* event) {
    lastMousePosition = event->pos();
}

void OpenGlWidget::mouseMoveEvent(QMouseEvent* event) {
    QPoint delta = event->pos() - lastMousePosition;
    lastMousePosition = event->pos();

    if (event->buttons() & Qt::LeftButton) {
        // Rotate model
        rotation.setX(rotation.x() + delta.y() * 0.5f);
        rotation.setY(rotation.y() + delta.x() * 0.5f);
    }
    else if (event->buttons() & Qt::RightButton) {
        // Pan model
        panOffset.setX(panOffset.x() + delta.x() * 0.01f);
        panOffset.setY(panOffset.y() - delta.y() * 0.01f);
    }
    update();
    viewChange(zoomLevel, rotation, panOffset);
}
