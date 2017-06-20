#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include "model3d.h"
#include "shader.h"
#include "shadermodel.h"
#include "shaderparameterinfo.h"
#include "shaderparameter.h"
#include "cameracontroller.h"
#include "camera.h"

void onMessageLogged(const QOpenGLDebugMessage &msg)
{
    qDebug() << msg;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("FH Aachen - University of Applied Sciences");
    app.setOrganizationDomain("fh-aachen.de");
    app.setApplicationName("Shader Inspector");

    qmlRegisterType<Model3D>("fhac", 1, 0, "Model3D");
    qmlRegisterType<Shader>("fhac", 1, 0, "Shader");
    qmlRegisterType<ShaderModel>("fhac", 1, 0, "ShaderModel");
    qmlRegisterType<ShaderParameterInfo>("fhac", 1, 0, "ShaderParameterInfo");
    qmlRegisterType<ShaderParameter>("fhac", 1, 0, "ShaderParameter");
    qmlRegisterType<Camera>("fhac", 1, 0, "Camera");
    qmlRegisterType<CameraController>("fhac", 1, 0, "CameraController");
    QQuickView view;
    view.setWidth(1600);
    view.setHeight(900);
    view.rootContext()->setContextProperty("currentPath", QDir::currentPath());
#ifdef _WIN32
    view.rootContext()->setContextProperty("pathPrefix", "file:///");
#else
    view.rootContext()->setContextProperty("pathPrefix", "file://");
#endif
    QOpenGLDebugLogger logger;
    QObject::connect( &view, &QQuickWindow::sceneGraphInitialized, [&view, &logger]()
    {
        if ( logger.initialize() ) {
            QObject::connect( &logger, &QOpenGLDebugLogger::messageLogged, onMessageLogged);
            logger.startLogging( QOpenGLDebugLogger::SynchronousLogging );
            logger.enableMessages();
            QVector<uint> disabledMessages;
            disabledMessages.push_back(131185); //Buffer Detailed Info
            disabledMessages.push_back(131169); //Buffer Detailed Info
            //disabledMessages.push_back(131204); // Texture has mipmaps, filter is inconsistent with mipmaps (framebuffer?)
            //disabledMessages.push_back(131218);
            //disabledMessages.push_back(131184);
            logger.disableMessages(disabledMessages);
        }
    });
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    QSurfaceFormat format = view.format();
    format.setOption(QSurfaceFormat::DebugContext);
    format.setVersion(4, 0);
    format.setProfile(QSurfaceFormat::CoreProfile);
    view.setFormat(format);

    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.show();
//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//    if (engine.rootObjects().isEmpty())
//        return -1;


    return app.exec();
}
