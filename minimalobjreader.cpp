#include "minimalobjreader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

#ifdef _MSC_VER
    #define SSCANF sscanf_s
#else
    #define SSCANF sscanf
#endif

MinimalObjReader::MinimalObjReader()
{

}

Vertices * MinimalObjReader::readFile(QString uri, Vertices *vertices)
{
    QFile file(uri);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << file.errorString();
        return vertices;
    }
    QTextStream stream(&file);
    QString allLines = stream.readAll();
    QStringList all = allLines.split('\n', QString::SkipEmptyParts);
    QString line;

    int curLine = 0;
    int numLines = all.size();
    vertices->positions.reserve( numLines / 2);
    vertices->texcoords.reserve( numLines / 2);

    // Model can not be copied to Gpu directly because size is unknown.
    // 1. Create Lists (Mesh::readFile(...))
    // 2. copy lists to buffers

    while(curLine < numLines)
    {
        if( curLine%10000 == 0 )
        {
            qDebug() << "line: " << curLine << " of " << numLines << " (" << ((float)curLine/(float)numLines)*100.f << "%)";
        }
        VertexPos vertex;
        VertexTex texcoord;
        QStringList vals;
        QByteArray ba = all[curLine].toLatin1();
        line = all[curLine];
        //const char* pszBuff = line.toStdString().c_str();
        const char* pszBuff = ba.constData();
        QTextStream lineStream(&pszBuff[2]);
        switch(pszBuff[0])
        {
        case 'v':
            switch(pszBuff[1]){
            case ' ':
                //Create new vertex
                //SSCANF(pszBuff, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z); //< broken on linux?
                lineStream >> vertex.x >> vertex.y >> vertex.z;
                vertices->positions.push_back(vertex);
                //++vertexCount;
                break;
            case 'n':
                // Create new vertex normal
                // Commented out because it's often not contained in model. Calculated later on the fly.
                //sscanf(pszBuff, "vn %f %f %f", &x, &y, &z);
                //normals.push_back(vertex);
                //++m_normalsCount;
                break;
            case 't':
                //Create new UV
                SSCANF(pszBuff, "vt %f %f", &texcoord.u, &texcoord.v);
                vertices->texcoords.push_back(texcoord);
                //++texCoordCount;
                break;
            }
            break;

        case 'f':
            vals = line.split(' ', QString::SkipEmptyParts);
            vals.pop_front(); // get rid of 'f'
            if(vals.size() == 3)
            {
                if(vals[0].contains("/")) // we have texture coordinate indices.
                {
                    vals[0] = vals[0].split("/")[0];
                    vals[1] = vals[1].split("/")[0];
                    vals[2] = vals[2].split("/")[0];
                }
                //1 based index in file. 0 based index in our datastructure
                int a = vals[0].toInt()-1;
                int b = vals[1].toInt()-1;
                int c = vals[2].toInt()-1;
                vertices->indices.push_back(a);
                vertices->indices.push_back(b);
                vertices->indices.push_back(c);
                //indexCount += 3;
            }
            else if(vals.size() == 4)
            {
                if(vals[0].contains("/")) // we have texture coordinate indices.
                {
                    vals[0] = vals[0].split("/")[0];
                    vals[1] = vals[1].split("/")[0];
                    vals[2] = vals[2].split("/")[0];
                    vals[3] = vals[3].split("/")[0];
                }
                int a = vals[0].toInt()-1;
                int b = vals[1].toInt()-1;
                int c = vals[2].toInt()-1;
                int d = vals[3].toInt()-1;
                vertices->indices.push_back(a);
                vertices->indices.push_back(b);
                vertices->indices.push_back(c);
                vertices->indices.push_back(a);
                vertices->indices.push_back(c);
                vertices->indices.push_back(d);
                //indexCount += 6;
            }
            else
            {
                qCritical("Model has faces with more than 4 or less than 3 vertices!");
            }
            break;
        default:
            ;//just go on
        }
        curLine++;
    }
    return vertices;
}
