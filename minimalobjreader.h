#ifndef MINIMALOBJREADER_H
#define MINIMALOBJREADER_H

#include <QString>
#include <QList>

//Two helper structs, because float arrays can't be used in templates
struct VertexPos
{
    float x;
    float y;
    float z;
};

struct VertexTex
{
    float u;
    float v;
};

struct Vertices
{
    QList<VertexPos> positions;
    QList<VertexTex> texcoords;
    QList<int> indices;
};

class MinimalObjReader
{
public:
    MinimalObjReader();
    static Vertices *readFile(QString uri, Vertices *vertices);
};

#endif // MINIMALOBJREADER_H
