#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QtGui>
#include <QSharedPointer>
#include "camera.h"

class CameraController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* eventSource READ eventSource WRITE setEventSource NOTIFY eventSourceChanged)
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged)
public:
    CameraController(QObject *parent = nullptr);
    virtual ~CameraController();

    Camera* camera() const;

    void setTranslationOption( Camera::CameraTranslationOption translationOption );
    Camera::CameraTranslationOption translationOption() const;

    Q_PROPERTY( float inertia READ inertia WRITE setInertia )
    Q_PROPERTY(bool keepUpVector READ keepUpVector WRITE setKeepUpVector NOTIFY keepUpVectorChanged)

    void setInertia( const float inertia );
    float inertia() const;

    bool keepUpVector() const;

    QObject* eventSource() const;

public slots:
    void update(const float elapsedSeconds = 0.1f);

    void keyPressEvent( QKeyEvent* e );
    void keyReleaseEvent( QKeyEvent* e );
    void mousePressEvent( QMouseEvent* e );
    void mouseReleaseEvent( QMouseEvent* e );
    void mouseMoveEvent( QMouseEvent* e );
    void wheelEvent( QWheelEvent* e );

    bool eventFilter(QObject *tgt, QEvent *event);
    void setKeepUpVector(bool keepUpVector);

    void setCamera( Camera* camera );
    void setEventSource(QObject* eventSource);

signals:
    void keepUpVectorChanged(bool keepUpVector);

    void eventSourceChanged(QObject* eventSource);

    void cameraChanged(Camera* camera);

private:
    Camera *m_camera;

    QVector3D m_velocity;
    QVector3D m_acceleration;
    float m_rollAcceleration;
    float m_rollVelocity;
    float m_inertia;

    bool m_extraSpeed;
    bool m_tiltLeft;
    bool m_tiltRight;
    bool m_movingFore;
    bool m_movingBack;
    bool m_movingUp;
    bool m_movingDown;
    bool m_movingLeft;
    bool m_movingRight;
    bool m_movementDirty;

    QPointF m_mouse;
    QPointF m_mousePrev;
    bool m_leftButtonPressed;
    bool m_middleButtonPressed;
    bool m_rightButtonPressed;
    Camera::CameraTranslationOption m_translationOption;

    bool m_keepUpVector;
    QObject* m_eventSource;
};

#endif // CAMERACONTROLLER_H
