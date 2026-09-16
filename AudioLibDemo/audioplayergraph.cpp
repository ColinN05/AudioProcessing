#include "audioplayergraph.h"

#include <QPainter>

AudioPlayerGraph::AudioPlayerGraph(QWidget *parent)
    : QOpenGLWidget(parent) {}

void AudioPlayerGraph::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, QOverload<>::of(&QWidget::update));
    m_updateTimer->start(16);
}

void AudioPlayerGraph::resizeGL(int w, int h)
{
    if (h == 0)
    {
        h = 1;
    }

    glViewport(0,0,w,h);
}

void AudioPlayerGraph::paintGL()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(25, 25, 30));

    int width = this->width();
    int height = this->height();
    int centerY = height / 2;

    painter.setPen(QPen(QColor(50, 50, 90), 1, Qt::DashLine));
    painter.drawLine(0, centerY, width, centerY);
    painter.setPen(QPen(QColor(0, 130, 220), 1.5));

    if (m_File)
    {
        const auto& samples = m_File->GetSamples();
        unsigned int channels = m_File->GetChannels();
        unsigned int frameCount = m_File->GetFrameCount();
        float a = static_cast<float>(frameCount) / width;

        for (int c = 0; c < channels; ++c)
        {
            QPolygonF graphPoints;
            for (int x = 0; x < width; ++x)
            {
                int i = static_cast<int>(x * a);
                int y = static_cast<int>(((samples[channels*i+c] + 1) * 0.5f + c)/channels*height);
                graphPoints << QPointF(x,y); 
            }

            painter.drawPolyline(graphPoints);

            int rectWidth = static_cast<int>(m_Progress*width);
            QColor regionColor(0, 120, 255, 60); 
            painter.setPen(QPen(QColor(0, 180, 255, 180), 1)); 
            painter.setBrush(regionColor);
            painter.drawRect(0, 0, rectWidth, height);
        }
    }
}




