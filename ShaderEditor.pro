TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    model3d.cpp \
    minimalobjreader.cpp \
    vertexattribute.cpp \
    model3drenderer.cpp \
    shader.cpp \
    shaderparameterinfo.cpp \
    shadermodel.cpp \
    shaderparameter.cpp \
    camera.cpp \
    cameracontroller.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    model3d.h \
    minimalobjreader.h \
    vertexattribute.h \
    model3drenderer.h \
    shader.h \
    shaderparameterinfo.h \
    shadermodel.h \
    shaderparameter.h \
    camera_p.h \
    camera.h \
    cameracontroller.h

DISTFILES += \
    shader/pointcloud.frag \
    shader/pointcloud.vert \
    shader/shader.fs \
    shader/shader.gs \
    shader/shader.tcs \
    shader/shader.tes \
    shader/shader.vs \
    shader/lessons/1_phong/shader.frag \
    shader/lessons/1_phong/shader.vert \
    shader/lessons/3_tessellation/settings.qmlc \
    shader/lessons/3_tessellation/SettingsShader.qmlc \
    shader/lessons/3_tessellation/shader_tcs.glsl \
    shader/lessons/3_tessellation/shader_tes.glsl \
    shader/lessons/3_tessellation_solution/shader_tcs.glsl \
    shader/lessons/3_tessellation_solution/shader_tes.glsl \
    shader/lessons/shader_tcs.glsl \
    shader/lessons/shader_tes.glsl \
    shader/lessons/2_tangentspace/shader.geom \
    shader/lessons/2_tangentspace_solution/shader.geom \
    shader/lessons/3_tessellation/shader.geom \
    shader/lessons/3_tessellation_solution/shader.geom \
    shader/lessons/shader.geom \
    shader/lessons/1_phong/shader.frag \
    shader/lessons/1_phong_solution/shader.frag \
    shader/lessons/2_tangentspace/shader.frag \
    shader/lessons/3_tessellation/shader.frag \
    shader/lessons/3_tessellation_solution/shader.frag \
    shader/lessons/shader.frag \
    shader/lessons/1_phong/shader.vert \
    shader/lessons/1_phong_solution/shader.vert \
    shader/lessons/2_tangentspace/shader.vert \
    shader/lessons/3_tessellation/shader.vert \
    shader/lessons/3_tessellation_solution/shader.vert \
    shader/lessons/shader.vert \
    shader/lessons/3_tessellation/settings.qml \
    shader/lessons/1_phong/settings.qml \
    shader/lessons/2_tangentspace_solution/settings.qml \
    shader/lessons/3_tessellation/shader_tcs.glsl \
    shader/lessons/3_tessellation/shader_tes.glsl \
    shader/lessons/3_tessellation_solution/shader_tcs.glsl \
    shader/lessons/3_tessellation_solution/shader_tes.glsl \
    shader/lessons/shader_tcs.glsl \
    shader/lessons/shader_tes.glsl \
    shader/lessons/2_tangentspace/shader.geom \
    shader/lessons/2_tangentspace_solution/shader.geom \
    shader/lessons/3_tessellation/shader.geom \
    shader/lessons/3_tessellation_solution/shader.geom \
    shader/lessons/shader.geom \
    shader/lessons/1_phong/shader.frag \
    shader/lessons/1_phong_solution/shader.frag \
    shader/lessons/2_tangentspace/shader.frag \
    shader/lessons/3_tessellation/shader.frag \
    shader/lessons/3_tessellation_solution/shader.frag \
    shader/lessons/shader.frag \
    shader/lessons/1_phong/shader.vert \
    shader/lessons/1_phong_solution/shader.vert \
    shader/lessons/2_tangentspace/shader.vert \
    shader/lessons/3_tessellation/shader.vert \
    shader/lessons/3_tessellation_solution/shader.vert \
    shader/lessons/shader.vert \
    shader/lessons/1_phong/settings.qml \
    shader/lessons/1_phong_solution/settings.qml \
    shader/lessons/2_tangentspace/settings.qml \
    shader/lessons/2_tangentspace_solution/settings.qml \
    shader/lessons/3_tessellation/settings.qml \
    shader/lessons/3_tessellation_solution/settings.qml \
    shader/lessons/1_phong/settings.qml \
    shader/lessons/2_tangentspace_solution/shader.frag \
    shader/lessons/2_tangentspace_solution/shader.vert
