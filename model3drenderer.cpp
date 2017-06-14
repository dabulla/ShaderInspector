#include "model3drenderer.h"
#include "model3d.h"
#include <QVector3D>
#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <QtConcurrent/QtConcurrent>
#include <QOpenGLFunctions_4_0_Core>
#include "shadermodel.h" // for ShaderParameterInfoBackend
#include <QOpenGLVertexArrayObject>

Model3DRenderer::Model3DRenderer()
    : m_window( nullptr )
    , m_vertices( nullptr )
    , m_initializedFromItem( false )
    , m_indices( QOpenGLBuffer::IndexBuffer )
    , m_vertexCount( 0 )
    , m_indexCount( 0 )
    , m_funcs( nullptr )
    , m_camera( nullptr )
    , m_drawPoints( false )
    , m_tesselationEnabled( false )
    , m_isInstanced( false )
    , m_vertexCountPerPrimitive( 0 )
    , m_primitiveCount( 0 )
    , m_reinitializeFromVertexdata( false )
    , m_vertexdataInitialized( false )
{
    connect(&m_objectLoadingWatcher, SIGNAL(finished()), this, SLOT(handleObjectLoaded()));
}
Model3DRenderer::~Model3DRenderer()
{
    if(m_vertices != nullptr )
    {
        delete m_vertices;
    }
}

void Model3DRenderer::synchronize(QQuickFramebufferObject *item)
{
    Model3D *modelItem = static_cast<Model3D *>(item);
    if(!m_window) m_window = modelItem->window();
    if(!m_initializedFromItem)
    {
        connect(modelItem, &Model3D::loadingFile, this, &Model3DRenderer::loadFile);
        if(!modelItem->m_fileLoading.isCanceled())
        {
            loadFile(modelItem->m_fileLoading);
        }
        m_funcs = m_window->openglContext()->versionFunctions<QOpenGLFunctions_4_0_Core>();
        if ( !m_funcs ) {
            qWarning() << "Unsupported Profile: QOpenGLFunctions_4_0_Core";
            return;
        }
        if ( !m_funcs->initializeOpenGLFunctions() ) {
            qWarning() << "Could not initialize gl functions";
            return;
        }
        m_initializedFromItem = true;
    }
    m_drawPoints = modelItem->drawPoints();
    if(modelItem->shader()->shaderProgram())
    {
        m_tmpShader = modelItem->shader();
        if(m_shader != modelItem->shader()->shaderProgram())
        {
            m_shader = modelItem->shader()->shaderProgram();
            recreateVao();
        }
        m_tesselationEnabled = modelItem->shader()->tesselationEnabled();
        m_shader->bind();
        if(modelItem->m_camera)
        {
            m_camera = modelItem->m_camera;
            updateCameraParameters(modelItem->shader());
        }
        while(!modelItem->m_updates.empty())
        {
            QPair<const ShaderParameterInfoBackend *, QVariant> &parameterAndValue = modelItem->m_updates.first();
            qDebug() << "DBG: updated" << parameterAndValue.first->m_name << " = " << parameterAndValue.second;
            updateParameter(parameterAndValue.first, parameterAndValue.second);
            modelItem->m_updates.removeFirst();
        }
        m_shader->release();
    }
    modelItem->update();
}
void Model3DRenderer::updateCameraParameters(Shader *shader)
{
    ShaderParameterInfoBackend *info;
    info = shader->parameterInfo("mvp");
    if(info) updateParameter(info, m_camera->viewProjectionMatrix());
    info = shader->parameterInfo("modelMatrix");
    if(info) updateParameter(info, m_camera->viewMatrix());
    info = shader->parameterInfo("modelViewNormal"); // TODO
    if(info) updateParameter(info, m_camera->viewMatrix());
    info = shader->parameterInfo("projectionMatrix");
    if(info) updateParameter(info, m_camera->projectionMatrix());
//    info = shader->parameterInfo("viewportMatrix");
//    if(info) updateParameter(info, m_camera->view());
}

void Model3DRenderer::recreateVao()
{
    if( m_vao.isCreated() )
    {
        m_vao.destroy();
    }
}

void Model3DRenderer::handleObjectLoaded()
{
    QFutureWatcher<Vertices*> *watcher = dynamic_cast< QFutureWatcher<Vertices*>* >(sender());
    if( watcher ){
        QFuture<Vertices*> future = watcher->future();
        if( m_vertices != nullptr )
        {
            delete m_vertices;
        }
        m_vertices = future.result();
        m_reinitializeFromVertexdata = true;
    }
}

void Model3DRenderer::reinitializeFromVertexdata()
{
    m_vertexDeclaration.clear();

    qint32 vertexCount = m_vertices->positions.length();
    qint32 texCoordCount = m_vertices->texcoords.length();
    qint32 indexCount = m_vertices->indices.length();

    qDebug() << "Vertices:" << vertexCount << "\nIndices:" << m_vertices->indices.size() << "\n";

    // Now sizes are known.
    // Create and map all buffers on the Gpu

    VertexAttribute *vaPosition = new VertexAttribute("vertexPosition", VertexAttribute::Vector3);
    vaPosition->m_buffer.create();
    vaPosition->m_buffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    vaPosition->m_buffer.bind(); //Buffers must be bound to do "allocate", "map", "unmap", ...
    vaPosition->m_buffer.allocate(vertexCount * 3 * sizeof(GLfloat));
    m_vertexDeclaration.append( vaPosition );
    //Once "map" was called, pointer can be accessed although other buffers are bound in the future (until unmap is called).
    GLfloat *positionsGpu = static_cast<GLfloat*>(vaPosition->m_buffer.map( QOpenGLBuffer::WriteOnly ));

    //TODO: name of standard texture coordinates
    VertexAttribute *vaTexCoord = new VertexAttribute("vertexTexture1", VertexAttribute::Vector2);
    vaTexCoord->m_buffer.create();
    vaTexCoord->m_buffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    vaTexCoord->m_buffer.bind();
    vaTexCoord->m_buffer.allocate(vertexCount * 2 * sizeof(GLfloat));
    m_vertexDeclaration.append( vaTexCoord );
    GLfloat *texCoordsGpu = static_cast<GLfloat*>(vaTexCoord->m_buffer.map( QOpenGLBuffer::WriteOnly ));

    VertexAttribute *vaNormal = new VertexAttribute("vertexNormal", VertexAttribute::Vector3);
    vaNormal->m_buffer.create();
    vaNormal->m_buffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    vaNormal->m_buffer.bind();
    vaNormal->m_buffer.allocate(vertexCount * 3 * sizeof(GLfloat));
    m_vertexDeclaration.append( vaNormal );
    GLfloat *normalsGpu = static_cast<GLfloat*>(vaNormal->m_buffer.map( QOpenGLBuffer::WriteOnly ));

    VertexAttribute *vaTangent = new VertexAttribute("vertexTangent", VertexAttribute::Vector3);
    vaTangent->m_buffer.create();
    vaTangent->m_buffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    vaTangent->m_buffer.bind();
    vaTangent->m_buffer.allocate(vertexCount * 3 * sizeof(GLfloat));
    m_vertexDeclaration.append( vaTangent );
    GLfloat *tangentsGpu = static_cast<GLfloat*>(vaTangent->m_buffer.map( QOpenGLBuffer::WriteOnly ));

    VertexAttribute *vaBitangent = new VertexAttribute("vertexBitangent", VertexAttribute::Vector3);
    vaBitangent->m_buffer.create();
    vaBitangent->m_buffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    vaBitangent->m_buffer.bind();
    vaBitangent->m_buffer.allocate(vertexCount * 3 * sizeof(GLfloat));
    m_vertexDeclaration.append( vaBitangent );
    GLfloat *bitangentsGpu = static_cast<GLfloat*>(vaBitangent->m_buffer.map( QOpenGLBuffer::WriteOnly ));

    m_indices.create();
    m_indices.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_indices.bind();
    m_indices.allocate( m_vertices->indices.size() * sizeof(GLint));
    GLuint *indicesGpu = static_cast<GLuint*>(m_indices.map( QOpenGLBuffer::WriteOnly ));

    // indices //
    for(int i=0 ; i<m_vertices->indices.size()-1 ; i+=3)
    {
        indicesGpu[i] = m_vertices->indices[i];
        indicesGpu[i+1] = m_vertices->indices[i+1];
        indicesGpu[i+2] = m_vertices->indices[i+2];
    }
    // positions //
    for(int i=vertexCount-1 ; i>=0 ; i--)
    {
        positionsGpu[i*3+0] = m_vertices->positions[i].x;
        positionsGpu[i*3+1] = m_vertices->positions[i].y;
        positionsGpu[i*3+2] = m_vertices->positions[i].z;
    }
    // Texture Coordinates //
    if(texCoordCount != 0)
    {
        for(int i=texCoordCount-1 ; i>=0 ; i--)
        {
            texCoordsGpu[i*2+0] = m_vertices->texcoords[i].u;
            texCoordsGpu[i*2+1] = m_vertices->texcoords[i].v;
        }
    }
    else
    {
        /// Texture Coordinates (spherical)
        // Convert vertex positions to spherical coordinates.
        // see wikipedia for more info http://de.wikipedia.org/wiki/Kugelkoordinaten
        for(int i=vertexCount-1 ; i>=0 ; i--)
        {
            float x = positionsGpu[i*3+0];
            float y = positionsGpu[i*3+1];
            float z = positionsGpu[i*3+2];

            float r = sqrtf(powf(x,2.f)+powf(y,2.f)+powf(z,2.f))+0.01f;
            float phi = atan2f(x, y);
            float theta = acosf(z/r);
            texCoordsGpu[i*2+0] = phi*0.6;
            texCoordsGpu[i*2+1] = -theta;
        }
    }

    ///// Calculate Normals for each vertex /////
    // Algorithm:
    // 1. for each triangle, calculate the normal: use two sides of the tri (a,b) and calculate the cross product
    // 2. normalize the normal (length = 1)
    // 3. Add it to the vertex' normal and count it's accumulated normals
    // 4. divide each vertex' normal by the number of normals that where added in order to normalize it again.
    //
    // Tagent/Bitangent: use texture coordinates (u, v) and bring their base to model-space.
    // Bitangent is either texture 'v' direction or is orthogonal to normal and tangent.
    // everything should be normalized

    // because normals are added, zeros are needed for the beginning
    memset(normalsGpu, 0, vertexCount*3*sizeof(GLfloat));
    memset(tangentsGpu, 0, vertexCount*3*sizeof(GLfloat));
    memset(bitangentsGpu, 0, vertexCount*3*sizeof(GLfloat));
    unsigned short *adjacentFaces = new unsigned short[vertexCount];
    memset(adjacentFaces, 0, vertexCount*sizeof(unsigned short));

    GLfloat a[3], b[3], crossprod[3], tangent[3], bitangent[3], deltaUV1[2], deltaUV2[2];
    GLfloat invlength;
    for(int i=0 ; i<m_vertices->indices.size()-1 ; i+=3)
    {
        GLint i1 = indicesGpu[i];
        GLint i2 = indicesGpu[i+1];
        GLint i3 = indicesGpu[i+2];
        //construct pointer to the three indexed vertices
        GLfloat* pV1 = positionsGpu+i1*3;
        GLfloat* pV2 = positionsGpu+i2*3;
        GLfloat* pV3 = positionsGpu+i3*3;
        //get the difference in position (two edges of the triangle)
        //vec3 a = p2.xyz - p1.xyz;
        //vec3 b = p2.xyz - p3.xyz;
        a[0] = pV1[0]-pV2[0];
        a[1] = pV1[1]-pV2[1];
        a[2] = pV1[2]-pV2[2];
        b[0] = pV2[0]-pV3[0];
        b[1] = pV2[1]-pV3[1];
        b[2] = pV2[2]-pV3[2];
        //cross
        crossprod[0] = a[1]*b[2]-a[2]*b[1];
        crossprod[1] = a[2]*b[0]-a[0]*b[2];
        crossprod[2] = a[0]*b[1]-a[1]*b[0];
        //normalize, switch side (ccw triangles)
        invlength = 1.f/sqrt(pow(crossprod[0],2.f)+pow(crossprod[1],2.f)+pow(crossprod[2],2.f));
        crossprod[0] *= invlength;
        crossprod[1] *= invlength;
        crossprod[2] *= invlength;

        normalsGpu[i1*3] += crossprod[0];
        normalsGpu[i1*3+1] += crossprod[1];
        normalsGpu[i1*3+2] += crossprod[2];
        normalsGpu[i2*3] += crossprod[0];
        normalsGpu[i2*3+1] += crossprod[1];
        normalsGpu[i2*3+2] += crossprod[2];
        normalsGpu[i3*3] += crossprod[0];
        normalsGpu[i3*3+1] += crossprod[1];
        normalsGpu[i3*3+2] += crossprod[2];

        //TODO: This is wrong!
        /// calculate tanget / bitangent ///
        // use uv texture coordinates of the indexed vertices (construct pointers)
        GLfloat* pUV1 = texCoordsGpu+i1*2;
        GLfloat* pUV2 = texCoordsGpu+i2*2;
        GLfloat* pUV3 = texCoordsGpu+i3*2;
        deltaUV1[0] = pUV2[0]-pUV1[0];
        deltaUV1[1] = pUV2[1]-pUV1[1];
        deltaUV2[0] = pUV3[0]-pUV2[0];
        deltaUV2[1] = pUV3[1]-pUV2[1];

        float r = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV1[1] * deltaUV2[0]);
        tangent[0] = (a[0] * deltaUV2[1]   - b[0] * deltaUV1[1])*r;
        tangent[1] = (a[1] * deltaUV2[1]   - b[1] * deltaUV1[1])*r;
        tangent[2] = (a[2] * deltaUV2[1]   - b[2] * deltaUV1[1])*r;
        bitangent[0] = (a[0] * deltaUV2[0]   - b[0] * deltaUV1[0])*r;
        bitangent[1] = (a[1] * deltaUV2[0]   - b[1] * deltaUV1[0])*r;
        bitangent[2] = (a[2] * deltaUV2[0]   - b[2] * deltaUV1[0])*r;

        tangentsGpu[i1*3] += tangent[0];
        tangentsGpu[i1*3+1] += tangent[1];
        tangentsGpu[i1*3+2] += tangent[2];
        tangentsGpu[i2*3] += tangent[0];
        tangentsGpu[i2*3+1] += tangent[1];
        tangentsGpu[i2*3+2] += tangent[2];
        tangentsGpu[i3*3] += tangent[0];
        tangentsGpu[i3*3+1] += tangent[1];
        tangentsGpu[i3*3+2] += tangent[2];

        bitangentsGpu[i1*3] += bitangent[0];
        bitangentsGpu[i1*3+1] += bitangent[1];
        bitangentsGpu[i1*3+2] += bitangent[2];
        bitangentsGpu[i2*3] += bitangent[0];
        bitangentsGpu[i2*3+1] += bitangent[1];
        bitangentsGpu[i2*3+2] += bitangent[2];
        bitangentsGpu[i3*3] += bitangent[0];
        bitangentsGpu[i3*3+1] += bitangent[1];
        bitangentsGpu[i3*3+2] += bitangent[2];

        adjacentFaces[i1]++;
        adjacentFaces[i2]++;
        adjacentFaces[i3]++;
    }
    //normalize
    for(int i=0 ; i<vertexCount ; i++)
    {
        GLfloat invlength = 1.f/adjacentFaces[i];
        normalsGpu[i*3] *= invlength;
        normalsGpu[i*3+1] *= invlength;
        normalsGpu[i*3+2] *= invlength;

        tangentsGpu[i*3] *= invlength;
        tangentsGpu[i*3+1] *= invlength;
        tangentsGpu[i*3+2] *= invlength;

        bitangentsGpu[i*3] *= invlength;
        bitangentsGpu[i*3+1] *= invlength;
        bitangentsGpu[i*3+2] *= invlength;
    }

    delete [] adjacentFaces;

    // unmap all buffers, so driver can access data (send to gpu, render from it)
    // to call unmap, buffer must be bound again. Order of buffers is not important.
    m_indices.bind();
    m_indices.unmap();
    vaTexCoord->m_buffer.bind();
    vaTexCoord->m_buffer.unmap();
    vaBitangent->m_buffer.bind();
    vaBitangent->m_buffer.unmap();
    vaTangent->m_buffer.bind();
    vaTangent->m_buffer.unmap();
    vaNormal->m_buffer.bind();
    vaNormal->m_buffer.unmap();
    vaPosition->m_buffer.bind();
    vaPosition->m_buffer.unmap();

    m_vertexCount = vertexCount;
    m_indexCount = indexCount;
    m_vertexdataInitialized = true;
}

void Model3DRenderer::updateParameter(const ShaderParameterInfoBackend *uniformInfo, const QVariant &value)
{
    if( uniformInfo->m_isSubroutine )
    {
        QVariant valueCopy(value);
        //Every frame all subroutines must be set. Thus, subroutines are saved in lists
        if(! value.isValid() )
        {
            if(uniformInfo->m_subroutineValues.empty())
            {
                qDebug() << "No values possible for Uniform subroutine" << uniformInfo->m_name;
                return;
            }
            valueCopy = QVariant::fromValue(0); //TODO: this must be a string? (see lines below)
        }
        GLuint subroutineIndex = m_funcs->glGetSubroutineIndex( m_shader->programId(),
                                       uniformInfo->m__subroutineShaderType,
                                       valueCopy.toString().toStdString().c_str() );
        m_subroutines[ uniformInfo->m__subroutineShaderType ].replace(uniformInfo->m_uniformLocation, subroutineIndex);
    }
    else
    {
        QVariant::Type type = ShaderParameterInfo::fromGLDatatype(uniformInfo->m_datatype);
        switch (type) {
        case QVariant::Double:
            m_shader->setUniformValue( uniformInfo->m_uniformLocation, value.toFloat() );
            break;
        case QVariant::Vector2D:
            m_shader->setUniformValue( uniformInfo->m_uniformLocation, qvariant_cast<QVector2D>( value ));
            break;
        case QVariant::Vector3D:
            m_shader->setUniformValue( uniformInfo->m_uniformLocation, qvariant_cast<QVector3D>( value ));
            break;
        case QVariant::Vector4D:
            m_shader->setUniformValue( uniformInfo->m_uniformLocation, qvariant_cast<QVector4D>( value ));
            break;
        case QVariant::Int:
            m_shader->setUniformValue( uniformInfo->m_uniformLocation, value.toInt());
            break;
        case QVariant::Matrix4x4:
            m_shader->setUniformValue( uniformInfo->m_uniformLocation, qvariant_cast<QMatrix4x4>( value ));
            break;
        case QVariant::Matrix: // 3x3 does not exist, lets try, it's most common type in GLSL:
            m_shader->setUniformValue( uniformInfo->m_uniformLocation, qvariant_cast<QMatrix3x3>( value ));
            break;
        case QVariant::Bool:
            m_shader->setUniformValue( uniformInfo->m_uniformLocation, value.toBool());
            break;
        case QVariant::Image:
            //not yet supported
            break;
        default:
            Q_ASSERT( false );
            break;
        }
    }
}

void Model3DRenderer::loadFile(QFuture<Vertices *> future)
{
    m_objectLoadingWatcher.setFuture(future);
}

QOpenGLFramebufferObject *Model3DRenderer::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    // optionally enable multisampling by doing format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
}

void Model3DRenderer::render()
{
    if(!m_funcs) return;
    if(!m_shader) return;
    if(m_reinitializeFromVertexdata)
    {
        reinitializeFromVertexdata();
        m_reinitializeFromVertexdata = false;
    }
    if(!m_vertexdataInitialized) return;
    if(!m_vao.isCreated())
    {

        // create vao:
        // Vao saves vertexbuffer/shader state.
        // this is done once on initialisation, later only the vao needs to be "bound".
        m_vao.create();
        // binder calls m_vao->bind() now and m_vao->unbind() at the end of the scope
        QOpenGLVertexArrayObject::Binder binder( &m_vao );
        m_shader->bind();
        m_isInstanced = false;
        foreach( VertexAttribute *va, m_vertexDeclaration )
        {
            int loc = m_shader->attributeLocation(va->name().toStdString().c_str());
            // make sure attribute is used in shader (Note: Some shader compilers kick out unused attributes/uniforms)
            if(loc != -1)
            {
                // bind the buffer (e.g. position)
                m_isInstanced |= va->divisor(); // if divisor is not always 0, instancing is active
                va->m_buffer.bind();
                int size;
                switch( va->dataType() )
                {
                case VertexAttribute::Float:
                    size = 1;
                    break;
                case VertexAttribute::Vector2:
                    size = 2;
                    break;
                case VertexAttribute::Vector3:
                    size = 3;
                    break;
                case VertexAttribute::Vector4:
                    size = 4;
                    break;
                case VertexAttribute::Matrix4x4:
                    size = 16;
                    break;
                case VertexAttribute::Int:
                    size = 1;
                    break;
                default:
                    Q_ASSERT(false);
                }
                // make currently bound buffer avaliable in the shader under attribute name e.g. "position".
                m_shader->setAttributeBuffer( loc, GL_FLOAT, 0, size, size * sizeof(GLfloat));
                if( va->divisor() != 0)
                {
                    //instancing
                    m_funcs->glVertexAttribDivisor( loc, va->divisor() );
                }
                // and activate the attribute
                m_shader->enableAttributeArray( loc );
                qDebug() << "Shader is using Attribute: " << va->name();
            }
            else
            {
                qDebug() << "Shader does not use Attribute: " << va->name();
            }
        }
        m_indices.bind();
    }

    m_funcs->glDepthMask(true);

    m_funcs->glClearColor(0.0,0.5,0.5,0.8);
    m_funcs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Called with the FBO bound and the viewport set.

    m_shader->bind();
//    updateCameraParameters(m_tmpShader);
//    foreach( QmlShaderUniform *unif, m_updates)
//    {
//        if( unif->isSubroutine() )
//        {
//            //Every frame all subroutines must be set. Thus, subroutines are saved in lists
//            if(! unif->value().isValid() )
//            {
//                if(unif->subroutineValues().empty())
//                {
//                    continue;
//                }
//                unif->setValue( unif->subroutineValues().at(0) );
//            }
//            GLuint subroutineIndex = m_funcs->glGetSubroutineIndex( m_shader->programId(),
//                                           unif->_subroutineShaderType(),
//                                           unif->value().toString().toStdString().c_str() );
//            m_qmlShaderProperties.m_subroutines[ unif->_subroutineShaderType() ].replace(unif->m_uniformLocation, subroutineIndex);
//        }
//        else
//        {
//            switch (QmlShaderUniform::fromGLType(unif->type())) {
//            case QVariant::Double:
//                m_shader->setUniformValue( unif->m_uniformLocation, unif->value().toFloat() );
//                break;
//            case QVariant::Vector2D:
//                m_shader->setUniformValue( unif->m_uniformLocation, qvariant_cast<QVector2D>( unif->value() ));
//                break;
//            case QVariant::Vector3D:
//                m_shader->setUniformValue( unif->m_uniformLocation, qvariant_cast<QVector3D>( unif->value() ));
//                break;
//            case QVariant::Vector4D:
//                m_shader->setUniformValue( unif->m_uniformLocation, qvariant_cast<QVector4D>( unif->value() ));
//                break;
//            case QVariant::Int:
//                m_shader->setUniformValue( unif->m_uniformLocation, unif->value().toInt());
//                break;
//            case QVariant::Matrix4x4:
//                m_shader->setUniformValue( unif->m_uniformLocation, qvariant_cast<QMatrix4x4>( unif->value() ));
//                break;
//            case QVariant::Matrix: // 3x3 does not exist, lets try, it's most common type in GLSL:
//                m_shader->setUniformValue( unif->m_uniformLocation, qvariant_cast<QMatrix3x3>( unif->value() ));
//                break;
//            case QVariant::Bool:
//                m_shader->setUniformValue( unif->m_uniformLocation, unif->value().toBool());
//                break;
//            case QVariant::Image:
//                //not yet supported
//                break;
//            default:
//                Q_ASSERT( false );
//                break;
//            }
//        }
//    }



//    foreach ( const GLuint unit, m_unitConfigs.keys() )
//    {
//        const TextureUnitConfiguration& config = m_unitConfigs.value( unit );

//        if(config.sampler().isNull())
//        {
//            config.texture()->bind( unit );
//        }
//        else
//        {
//            // Bind the texture
//            m_funcs->glActiveTexture( GL_TEXTURE0 + unit );
//            config.texture()->bind();

//            // Bind the sampler
//            config.sampler()->bind( unit );
//        }

//        // Associate with sampler uniform in shader (if we know the name or location)
//        if ( m_samplerUniforms.contains( unit ) )
//            m_shader->setUniformValue( m_samplerUniforms.value( unit ).constData(), unit );
//    }

    QMap<int, QList<int> >::const_iterator shaderStageIterator( m_subroutines.begin() );
    GLuint subroutines[100];
    while( shaderStageIterator != m_subroutines.end() )
    {
        const QList<int> &subroutineList = shaderStageIterator.value();
        if(!subroutineList.empty())
        {
            for(int i=0 ; i<subroutineList.length() ; ++i )
            {
                subroutines[ i ] = subroutineList.at( i );
            }
            m_funcs->glUniformSubroutinesuiv( shaderStageIterator.key(), subroutineList.length(), subroutines );
        }
        shaderStageIterator++;
    }

    m_funcs->glDepthMask(GL_TRUE);
    m_funcs->glEnable( GL_DEPTH_TEST );
    m_funcs->glDepthFunc(GL_LESS);
    m_funcs->glEnable( GL_CULL_FACE );
    m_funcs->glCullFace( GL_BACK );

    QOpenGLVertexArrayObject::Binder binder( &m_vao );
    if( m_drawPoints )
    {
        m_funcs->glDrawArrays(GL_POINTS, 0, m_vertexCount);
    }
    else
    {
        m_shader->setPatchVertexCount(3);
        if( m_isInstanced )
        {
            GLenum primitiveType = m_tesselationEnabled ? GL_PATCHES : GL_TRIANGLE_STRIP;
            m_funcs->glDrawArraysInstanced( primitiveType, 0, m_vertexCountPerPrimitive, m_primitiveCount );
        }
        else
        {
            GLenum primitiveType = m_tesselationEnabled ? GL_PATCHES : GL_TRIANGLES;
            //TODO: indexbuffer is somehow not recognized by vao
            m_indices.bind();
            m_funcs->glDrawElements( primitiveType, m_indexCount, GL_UNSIGNED_INT, 0 );
            m_indices.release();
            //m_funcs->glDrawArrays( primitiveType, 0, m_indexCount);//, GL_UNSIGNED_INT, 0 );
        }
    }
    m_shader->release();

    update();
    if (m_window) m_window->resetOpenGLState();
}
