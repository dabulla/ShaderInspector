#include "cameracontroller.h"

#include <QtMath>

CameraController::CameraController(QObject *parent)
    :QObject(parent),
     m_velocity(0.0f,0.1f,0.0f),
     m_acceleration(0.0f,0.0f,0.0f),
     m_rollAcceleration(0.0f),
     m_rollVelocity(0.0f),
     m_inertia(0.75f),
     m_extraSpeed(false),
     m_tiltLeft(false),
     m_tiltRight(false),
     m_movingFore(false),
     m_movingBack(false),
     m_movingUp(false),
     m_movingDown(false),
     m_movingLeft(false),
     m_movingRight(false),
     m_movementDirty(true),
//     m_mouse(0.0,0.0),
//     m_mousePrev(0.0,0.0),
     m_leftButtonPressed(false),
     m_middleButtonPressed(false),
     m_rightButtonPressed(false),
     m_keepUpVector( false ),
     m_translationOption(Camera::DontTranslateViewCenter)
{
}

CameraController::~CameraController()
{
}

void CameraController::setCamera(Camera *camera) { m_camera = camera; }

Camera *CameraController::camera() const { return m_camera; }

void CameraController::setTranslationOption(Camera::CameraTranslationOption translationOption) { m_translationOption = translationOption; }

Camera::CameraTranslationOption CameraController::translationOption() const { return m_translationOption; }

void CameraController::setInertia(const float inertia) { m_inertia = inertia; }

float CameraController::inertia() const { return m_inertia; }

void CameraController::update(const float elapsedSeconds)
{
    if(m_movementDirty)
    {
        const float accel = 25.0f + m_extraSpeed * 40.0f;
        m_acceleration.setX( m_movingRight - m_movingLeft );
        m_acceleration.setY( m_movingUp - m_movingDown );
        m_acceleration.setZ( m_movingFore - m_movingBack );
        m_acceleration.normalize();
        m_acceleration *= accel;

        m_rollAcceleration = m_tiltRight - m_tiltLeft;
        m_rollAcceleration *= 17.7f;

        m_movementDirty = false;
    }

    m_rollVelocity += m_rollAcceleration;
    m_camera->roll(m_rollVelocity * elapsedSeconds);
    m_rollVelocity *= m_inertia;

    //const float invElapsed = 1.0/elapsedSeconds;
    m_velocity += m_acceleration;
    m_camera->translate( m_velocity * elapsedSeconds, m_translationOption);
    m_velocity *= m_inertia; //FIXME: not time corrected

    if(m_keepUpVector)
    {
        m_camera->setUpVector( QVector3D(0.0, 1.0, 0.0) );
    }
}

bool CameraController::keepUpVector() const
{
    return m_keepUpVector;
}

QObject *CameraController::eventSource() const
{
    return m_eventSource;
}

void CameraController::keyPressEvent( QKeyEvent* e )
{
    switch ( e->key() )
    {
    case Qt::Key_Shift:
        m_extraSpeed = true;
        m_movementDirty = true;
        break;
    case Qt::Key_Q:
        m_tiltLeft = true;
        m_movementDirty = true;
        break;
    case Qt::Key_E:
        m_tiltRight = true;
        m_movementDirty = true;
        break;
    case Qt::Key_W:
        m_movingFore = true;
        m_movementDirty = true;
        break;
    case Qt::Key_A:
        m_movingLeft = true;
        m_movementDirty = true;
        break;
    case Qt::Key_S:
        m_movingBack = true;
        m_movementDirty = true;
        break;
    case Qt::Key_D:
        m_movingRight = true;
        m_movementDirty = true;
        break;
    case Qt::Key_PageUp:
        m_movingUp = true;
        m_movementDirty = true;
        break;
    case Qt::Key_PageDown:
        m_movingDown = true;
        m_movementDirty = true;
        break;
    case Qt::Key_V:
        if(m_translationOption == Camera::TranslateViewCenter)
        {
            m_translationOption = Camera::DontTranslateViewCenter;
        }
        else
        {
            m_translationOption = Camera::TranslateViewCenter;
        }
        break;
    }
}

void CameraController::keyReleaseEvent( QKeyEvent* e )
{
    switch ( e->key() )
    {
    case Qt::Key_Shift:
        m_extraSpeed = false;
        m_movementDirty = true;
        break;
    case Qt::Key_Q:
        m_tiltLeft = false;
        m_movementDirty = true;
        break;
    case Qt::Key_E:
        m_tiltRight = false;
        m_movementDirty = true;
        break;
    case Qt::Key_W:
        m_movingFore = false;
        m_movementDirty = true;
        break;
    case Qt::Key_A:
        m_movingLeft = false;
        m_movementDirty = true;
        break;
    case Qt::Key_S:
        m_movingBack = false;
        m_movementDirty = true;
        break;
    case Qt::Key_D:
        m_movingRight = false;
        m_movementDirty = true;
        break;
    case Qt::Key_PageUp:
        m_movingUp = false;
        m_movementDirty = true;
        break;
    case Qt::Key_PageDown:
        m_movingDown = false;
        m_movementDirty = true;
        break;
    }
}

void CameraController::mousePressEvent( QMouseEvent* e )
{
    if ( e->button() == Qt::LeftButton )
    {
        m_leftButtonPressed = true;
        m_mouse = m_mousePrev = e->pos();
    }
    if ( e->button() == Qt::MidButton )
    {
        m_middleButtonPressed = true;
        m_mouse = m_mousePrev = e->pos();
    }
    if ( e->button() == Qt::RightButton )
    {
        m_rightButtonPressed = true;
        m_mouse = m_mousePrev = e->pos();
    }
}

void CameraController::mouseReleaseEvent( QMouseEvent* e )
{
    if ( e->button() == Qt::LeftButton )
    {
        m_leftButtonPressed = false;
    }
    if ( e->button() == Qt::MidButton )
    {
        m_middleButtonPressed = false;
    }
    if ( e->button() == Qt::RightButton )
    {
        m_rightButtonPressed = false;
    }
}

void CameraController::mouseMoveEvent( QMouseEvent* e )
{
    if ( m_leftButtonPressed )
    {
        m_mouse = e->pos();
        float dx = 0.2f * ( m_mouse.x() - m_mousePrev.x() );
        float dy = 0.2f * ( m_mouse.y() - m_mousePrev.y() );
        m_mousePrev = m_mouse;

        if(m_translationOption == Camera::DontTranslateViewCenter)
        {
            m_camera->panAboutViewCenter( dx );
            float dot = QVector3D::dotProduct(m_camera->viewVector(), QVector3D(0.0,1.0,0.0));
            if(qAbs(dot) > 0.99f)
            {
                dy = dot<0.f?qMax(dy, 0.f):qMin(dy, 0.f);
            }
            m_camera->tiltAboutViewCenter( -dy );
        }
        else
        {
            m_camera->pan( dx );
            m_camera->tilt( -dy );
        }
        if(m_keepUpVector)
        {
            m_camera->setUpVector( QVector3D(0.0, 1.0, 0.0) );
        }
    }
    if ( m_middleButtonPressed || m_rightButtonPressed )
    {
        m_mouse = e->pos();
        float dx = 0.2f * ( m_mouse.x() - m_mousePrev.x() );
        float dy = 0.2f * ( m_mouse.y() - m_mousePrev.y() );
        m_mousePrev = m_mouse;

        if(m_translationOption != Camera::DontTranslateViewCenter)
        {
            m_camera->panAboutViewCenter( dx );
            float dot = QVector3D::dotProduct(m_camera->viewVector(), QVector3D(0.0,1.0,0.0));
            if(qAbs(dot) > 0.99f)
            {
                dy = dot<0.f?qMax(dy, 0.f):qMin(dy, 0.f);
            }
            m_camera->tiltAboutViewCenter( -dy );
        }
        else
        {
            m_camera->pan( dx );
            m_camera->tilt( -dy );
        }
        if(m_keepUpVector)
        {
            m_camera->setUpVector( QVector3D(0.0, 1.0, 0.0) );
        }
    }
}

void CameraController::wheelEvent( QWheelEvent* e )
{
    m_camera->translate(QVector3D(0.0, 0.0, e->delta() * (0.2 + m_extraSpeed * 0.5) * 0.001), m_translationOption);
}

bool CameraController::eventFilter(QObject *tgt, QEvent *event)
{
    Q_UNUSED(tgt)
    if (event->type() == QEvent::UpdateRequest) {
        update(0.13f);
    } else if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        keyPressEvent( keyEvent );
        return keyEvent->isAccepted();
    } else if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        keyReleaseEvent( keyEvent );
        return keyEvent->isAccepted();
    } else if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *keyEvent = static_cast<QMouseEvent *>(event);
        mousePressEvent( keyEvent );
        return keyEvent->isAccepted();
    } else if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *keyEvent = static_cast<QMouseEvent *>(event);
        mouseReleaseEvent( keyEvent );
        return keyEvent->isAccepted();
    } else if (event->type() == QEvent::MouseMove) {
        QMouseEvent *keyEvent = static_cast<QMouseEvent *>(event);
        mouseMoveEvent( keyEvent );
        return keyEvent->isAccepted();
    } else if (event->type() == QEvent::Wheel) {
        QWheelEvent *keyEvent = static_cast<QWheelEvent *>(event);
        wheelEvent( keyEvent );
        return keyEvent->isAccepted();
    }
    return false;//event->isAccepted();
}

void CameraController::setKeepUpVector(bool keepUpVector)
{
    if (m_keepUpVector == keepUpVector)
        return;

    m_keepUpVector = keepUpVector;
    emit keepUpVectorChanged(keepUpVector);
}

void CameraController::setEventSource(QObject *eventSource)
{
    if (m_eventSource == eventSource)
        return;

    if(m_eventSource)
    {
        m_eventSource->removeEventFilter( this );
    }
    m_eventSource = eventSource;
    m_eventSource->installEventFilter( this );
    emit eventSourceChanged(m_eventSource);
}

//void CameraController::translate( const QVector3D& vLocal)
//{
//    QVector3D vWorld;
//    //Transform local movement to worldspace.
//    vWorld += vLocal.z() * m_cameraToCenter.normalized();
//    if(m_cameraMode == CAMERMODE_WALKTHROUGH)
//    {
//        // Use crossproduct of camera front direction and up vector for sideway movement
//        QVector3D x = QVector3D::crossProduct( m_cameraToCenter, m_upVector ).normalized();
//        vWorld += vLocal.x() * x;
//        // Use upvector for upward movement
//        vWorld += vLocal.y() * m_upVector;
//        // update the view center coordinates when using a walkthrough camera
//        m_viewCenter += vWorld;
//    } else {
//        // update the cameraToCenter vector (only in z direction) when using objectInspection
//        m_cameraToCenter -= vWorld;
//    }
//    // Update the camera position
//    m_position += vWorld;
//}

//// pan (point camera right or left)
//void CameraController::pan( const float &angle )
//{
//    //rotate the up- and cameraToCenter-vector using quaternion-math.
//    QQuaternion q = QQuaternion::fromAxisAndAngle( QVector3D(0.0f,1.0f,0.0f), -angle );
//    //If the camera looks up or down, the upvector rotates while panning
//    m_upVector = q.rotatedVector( m_upVector );
//    m_cameraToCenter = q.rotatedVector( m_cameraToCenter );
//    if(m_cameraMode == CAMERMODE_WALKTHROUGH)
//    {
//        m_viewCenter = m_position + m_cameraToCenter;
//    } else {
//        m_position = m_viewCenter - m_cameraToCenter;
//    }
//}

////tilt (point camera up and down)
//void CameraController::tilt( const float &angle )
//{
//    QVector3D xBasis = QVector3D::crossProduct( m_upVector, m_cameraToCenter.normalized() ).normalized();
//    QQuaternion q = QQuaternion::fromAxisAndAngle( xBasis, angle );
//    m_upVector = q.rotatedVector( m_upVector );
//    m_cameraToCenter = q.rotatedVector( m_cameraToCenter );
//    if(m_cameraMode == CAMERMODE_WALKTHROUGH)
//    {
//        m_viewCenter = m_position + m_cameraToCenter;
//    } else {
//        m_position = m_viewCenter - m_cameraToCenter;
//    }
//}

