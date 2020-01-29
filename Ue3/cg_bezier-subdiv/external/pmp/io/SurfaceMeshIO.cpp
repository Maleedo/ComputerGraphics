//=============================================================================
// Copyright (C) 2011-2018 The pmp-library developers
// Copyright (C) 2001-2005 by Computer Graphics Group, RWTH Aachen
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//=============================================================================

#include <pmp/io/SurfaceMeshIO.h>

#include <clocale>
#include <cfloat>
#include <fstream>
#include <cstring>
#include <cctype>

// helper function
template <typename T>
void tfread(FILE* in, const T& t)
{
    size_t nItems = fread((char*)&t, 1, sizeof(t), in);
    PMP_ASSERT(nItems > 0);
}

// helper function
template <typename T>
void tfwrite(FILE* out, const T& t)
{
    size_t nItems = fwrite((char*)&t, 1, sizeof(t), out);
    PMP_ASSERT(nItems > 0);
}

// check if string contains only spaces
bool hasOnlySpaces(const std::string& str)
{
    for (const char& c : str)
    {
        if (!std::isspace(c))
            return false;
    }
    return true;
}

// function to skip comment and whitespace lines
void skipLines(FILE* in)
{
    char line[200];
    fpos_t pos;

    while (in && !feof(in))
    {
        fgetpos(in, &pos);
        auto c = fgets(line, 200, in);
        if (c == nullptr)
            break;

        std::string s(line);
        if (line[0] == '#')
        {
            continue;
        }
        else if (hasOnlySpaces(s))
        {
            continue;
        }
        else
        {
            fsetpos(in, &pos);
            break;
        }
    }
}

//=============================================================================

namespace pmp {

//=============================================================================

bool SurfaceMeshIO::read(SurfaceMesh& mesh, const std::string& filename)
{
    std::setlocale(LC_NUMERIC, "C");

    // clear mesh before reading from file
    mesh.clear();

    // extract file extension
    std::string::size_type dot(filename.rfind("."));
    if (dot == std::string::npos)
        return false;
    std::string ext = filename.substr(dot + 1, filename.length() - dot - 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);

    // extension determines reader
    if (ext == "off")
    {
        return readOFF(mesh, filename);
    }
    else if (ext == "obj")
    {
        return readOBJ(mesh, filename);
    }
    else if (ext == "stl")
    {
        return readSTL(mesh, filename);
    }
    else if (ext == "pmp")
    {
        return readPMP(mesh, filename);
    }

    // we didn't find a reader module
    return false;
}

//-----------------------------------------------------------------------------

bool SurfaceMeshIO::write(const SurfaceMesh& mesh, const std::string& filename)
{
    // extract file extension
    std::string::size_type dot(filename.rfind("."));
    if (dot == std::string::npos)
        return false;
    std::string ext = filename.substr(dot + 1, filename.length() - dot - 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);

    // extension determines reader
    if (ext == "off")
    {
        return writeOFF(mesh, filename);
    }
    else if (ext == "obj")
    {
        return writeOBJ(mesh, filename);
    }
    else if (ext == "stl")
    {
        return writeSTL(mesh, filename);
    }
    else if (ext == "pmp")
    {
        return writePMP(mesh, filename);
    }

    // we didn't find a writer module
    return false;
}

//-----------------------------------------------------------------------------

bool SurfaceMeshIO::readOBJ(SurfaceMesh& mesh, const std::string& filename)
{
    char s[200];
    float x, y, z;
    std::vector<SurfaceMesh::Vertex> vertices;
    std::vector<TextureCoordinate>
        allTexCoords;                //individual texture coordinates
    std::vector<int> halfedgeTexIdx; //texture coordinates sorted for halfedges
    SurfaceMesh::HalfedgeProperty<TextureCoordinate> texCoords =
        mesh.halfedgeProperty<TextureCoordinate>("h:tex");
    bool withTexCoord = false;

    // clear mesh
    mesh.clear();

    // open file (in ASCII mode)
    FILE* in = fopen(filename.c_str(), "r");
    if (!in)
        return false;

    // clear line once
    memset(&s, 0, 200);

    // parse line by line (currently only supports vertex positions & faces
    while (in && !feof(in) && fgets(s, 200, in))
    {
        // comment
        if (s[0] == '#' || isspace(s[0]))
            continue;

        // vertex
        else if (strncmp(s, "v ", 2) == 0)
        {
            if (sscanf(s, "v %f %f %f", &x, &y, &z))
            {
                mesh.addVertex(Point(x, y, z));
            }
        }

        // normal
        else if (strncmp(s, "vn ", 3) == 0)
        {
            if (sscanf(s, "vn %f %f %f", &x, &y, &z))
            {
                // problematic as it can be either a vertex property when interpolated
                // or a halfedge property for hard edges
            }
        }

        // texture coordinate
        else if (strncmp(s, "vt ", 3) == 0)
        {
            if (sscanf(s, "vt %f %f", &x, &y))
            {
                allTexCoords.emplace_back(x, y);
            }
        }

        // face
        else if (strncmp(s, "f ", 2) == 0)
        {
            int component(0), nV(0);
            bool endOfVertex(false);
            char *p0, *p1(s + 1);

            vertices.clear();
            halfedgeTexIdx.clear();

            // skip white-spaces
            while (*p1 == ' ')
                ++p1;

            while (p1)
            {
                p0 = p1;

                // overwrite next separator

                // skip '/', '\n', ' ', '\0', '\r' <-- don't forget Windows
                while (*p1 != '/' && *p1 != '\r' && *p1 != '\n' && *p1 != ' ' &&
                       *p1 != '\0')
                    ++p1;

                // detect end of vertex
                if (*p1 != '/')
                {
                    endOfVertex = true;
                }

                // replace separator by '\0'
                if (*p1 != '\0')
                {
                    *p1 = '\0';
                    p1++; // point to next token
                }

                // detect end of line and break
                if (*p1 == '\0' || *p1 == '\n')
                {
                    p1 = nullptr;
                }

                // read next vertex component
                if (*p0 != '\0')
                {
                    switch (component)
                    {
                        case 0: // vertex
                        {
                            vertices.emplace_back(atoi(p0) - 1);
                            break;
                        }
                        case 1: // texture coord
                        {
                            int idx = atoi(p0) - 1;
                            halfedgeTexIdx.push_back(idx);
                            withTexCoord = true;
                            break;
                        }
                        case 2: // normal
                            break;
                    }
                }

                ++component;

                if (endOfVertex)
                {
                    component = 0;
                    nV++;
                    endOfVertex = false;
                }
            }

            SurfaceMesh::Face f = mesh.addFace(vertices);

            // add texture coordinates
            if (withTexCoord)
            {
                SurfaceMesh::HalfedgeAroundFaceCirculator h_fit =
                    mesh.halfedges(f);
                SurfaceMesh::HalfedgeAroundFaceCirculator h_end = h_fit;
                unsigned v_idx = 0;
                do
                {
                    texCoords[*h_fit] =
                        allTexCoords.at(halfedgeTexIdx.at(v_idx));
                    ++v_idx;
                    ++h_fit;
                } while (h_fit != h_end);
            }
        }
        // clear line
        memset(&s, 0, 200);
    }

    // if there are no textures, delete texture property!
    if (!withTexCoord)
    {
        mesh.removeHalfedgeProperty(texCoords);
    }

    fclose(in);
    return true;
}

//-----------------------------------------------------------------------------

bool SurfaceMeshIO::writeOBJ(const SurfaceMesh& mesh,
                             const std::string& filename)
{
    FILE* out = fopen(filename.c_str(), "w");
    if (!out)
        return false;

    // comment
    fprintf(out, "# OBJ export from SurfaceMesh\n");

    //vertices
    SurfaceMesh::VertexProperty<Point> points =
        mesh.getVertexProperty<Point>("v:point");
    for (SurfaceMesh::VertexIterator vit = mesh.verticesBegin();
         vit != mesh.verticesEnd(); ++vit)
    {
        const Point& p = points[*vit];
        fprintf(out, "v %.10f %.10f %.10f\n", p[0], p[1], p[2]);
    }

    //normals
    SurfaceMesh::VertexProperty<Point> normals =
        mesh.getVertexProperty<Point>("v:normal");
    if (normals)
    {
        for (SurfaceMesh::VertexIterator vit = mesh.verticesBegin();
             vit != mesh.verticesEnd(); ++vit)
        {
            const Point& p = normals[*vit];
            fprintf(out, "vn %.10f %.10f %.10f\n", p[0], p[1], p[2]);
        }
    }

    // optional texture coordinates
    // do we have them?
    std::vector<std::string> hprops = mesh.halfedgeProperties();
    bool withTexCoord = false;
    auto hpropEnd = hprops.end();
    auto hpropStart = hprops.begin();
    while (hpropStart != hpropEnd)
    {
        if (0 == (*hpropStart).compare("h:tex"))
        {
            withTexCoord = true;
        }
        ++hpropStart;
    }

    //if so then add
    if (withTexCoord)
    {
        SurfaceMesh::HalfedgeProperty<TextureCoordinate> texCoord =
            mesh.getHalfedgeProperty<TextureCoordinate>("h:tex");
        for (SurfaceMesh::HalfedgeIterator hit = mesh.halfedgesBegin();
             hit != mesh.halfedgesEnd(); ++hit)
        {
            const TextureCoordinate& pt = texCoord[*hit];
            fprintf(out, "vt %.10f %.10f \n", pt[0], pt[1]);
        }
    }

    //faces
    for (SurfaceMesh::FaceIterator fit = mesh.facesBegin();
         fit != mesh.facesEnd(); ++fit)
    {
        fprintf(out, "f");
        SurfaceMesh::VertexAroundFaceCirculator fvit = mesh.vertices(*fit),
                                                fvend = fvit;
        SurfaceMesh::HalfedgeAroundFaceCirculator fhit = mesh.halfedges(*fit);
        do
        {
            if (withTexCoord)
            {
                // write vertex index, texCoord index and normal index
                fprintf(out, " %d/%d/%d", (*fvit).idx() + 1, (*fhit).idx() + 1,
                        (*fvit).idx() + 1);
                ++fhit;
            }
            else
            {
                // write vertex index and normal index
                fprintf(out, " %d//%d", (*fvit).idx() + 1, (*fvit).idx() + 1);
            }
        } while (++fvit != fvend);
        fprintf(out, "\n");
    }

    fclose(out);
    return true;
}

//-----------------------------------------------------------------------------

bool readOFFAscii(SurfaceMesh& mesh, FILE* in, const bool hasNormals,
                  const bool hasTexcoords, const bool hasColors)
{
    char line[200], *lp;
    int nc;
    unsigned int i, j, items, idx;
    unsigned int nV, nF, nE;
    vec3 p, n, c;
    vec3 t;
    SurfaceMesh::Vertex v;

    // properties
    SurfaceMesh::VertexProperty<Normal> normals;
    SurfaceMesh::VertexProperty<TextureCoordinate> texcoords;
    SurfaceMesh::VertexProperty<Color> colors;
    if (hasNormals)
        normals = mesh.vertexProperty<Normal>("v:normal");
    if (hasTexcoords)
        texcoords = mesh.vertexProperty<TextureCoordinate>("v:tex");
    if (hasColors)
        colors = mesh.vertexProperty<Color>("v:color");

    // #Vertice, #Faces, #Edges
    items = fscanf(in, "%d %d %d\n", (int*)&nV, (int*)&nF, (int*)&nE);
    PMP_ASSERT(items);

    // skip comments and empty lines
    //skipLines(in);

    mesh.clear();
    mesh.reserve(nV, std::max(3 * nV, nE), nF);

    // read vertices: pos [normal] [color] [texcoord]
    for (i = 0; i < nV && !feof(in); ++i)
    {
        // read line
        lp = fgets(line, 200, in);
        lp = line;

        // position
        items = sscanf(lp, "%f %f %f%n", &p[0], &p[1], &p[2], &nc);
        assert(items == 3);
        v = mesh.addVertex((Point)p);
        lp += nc;

        // normal
        if (hasNormals)
        {
            if (sscanf(lp, "%f %f %f%n", &n[0], &n[1], &n[2], &nc) == 3)
            {
                normals[v] = n;
            }
            lp += nc;
        }

        // color
        if (hasColors)
        {
            if (sscanf(lp, "%f %f %f%n", &c[0], &c[1], &c[2], &nc) == 3)
            {
                if (c[0] > 1.0f || c[1] > 1.0f || c[2] > 1.0f)
                    c *= (1.0 / 255.0);
                colors[v] = c;
            }
            lp += nc;
        }

        // tex coord
        if (hasTexcoords)
        {
            items = sscanf(lp, "%f %f%n", &t[0], &t[1], &nc);
            assert(items == 2);
            texcoords[v][0] = t[0];
            texcoords[v][1] = t[1];
            lp += nc;
        }
    }

    // skip comments and empty lines
    //skipLines(in);

    // read faces: #N v[1] v[2] ... v[n-1]
    std::vector<SurfaceMesh::Vertex> vertices;
    for (i = 0; i < nF; ++i)
    {
        // read line
        lp = fgets(line, 200, in);
        lp = line;

        // #vertices
        items = sscanf(lp, "%d%n", (int*)&nV, &nc);
        assert(items == 1);
        vertices.resize(nV);
        lp += nc;

        // indices
        for (j = 0; j < nV; ++j)
        {
            items = sscanf(lp, "%d%n", (int*)&idx, &nc);
            assert(items == 1);
            vertices[j] = SurfaceMesh::Vertex(idx);
            lp += nc;
        }
        mesh.addFace(vertices);
    }

    return true;
}

//-----------------------------------------------------------------------------

bool readOFFBinary(SurfaceMesh& mesh, FILE* in, const bool hasNormals,
                   const bool hasTexcoords, const bool hasColors)
{
    unsigned int i, j, idx(0);
    unsigned int nV(0), nF(0), nE(0);
    vec3 p, n, c;
    vec2 t;
    SurfaceMesh::Vertex v;

    // binary cannot (yet) read colors
    if (hasColors)
        return false;

    // properties
    SurfaceMesh::VertexProperty<Normal> normals;
    SurfaceMesh::VertexProperty<TextureCoordinate> texcoords;
    if (hasNormals)
        normals = mesh.vertexProperty<Normal>("v:normal");
    if (hasTexcoords)
        texcoords = mesh.vertexProperty<TextureCoordinate>("v:tex");

    // #Vertice, #Faces, #Edges
    tfread(in, nV);
    tfread(in, nF);
    tfread(in, nE);
    mesh.clear();
    mesh.reserve(nV, std::max(3 * nV, nE), nF);

    // read vertices: pos [normal] [color] [texcoord]
    for (i = 0; i < nV && !feof(in); ++i)
    {
        // position
        tfread(in, p);
        v = mesh.addVertex((Point)p);

        // normal
        if (hasNormals)
        {
            tfread(in, n);
            normals[v] = n;
        }

        // tex coord
        if (hasTexcoords)
        {
            tfread(in, t);
            texcoords[v][0] = t[0];
            texcoords[v][1] = t[1];
        }
    }

    // read faces: #N v[1] v[2] ... v[n-1]
    std::vector<SurfaceMesh::Vertex> vertices;
    for (i = 0; i < nF; ++i)
    {
        tfread(in, nV);
        vertices.resize(nV);
        for (j = 0; j < nV; ++j)
        {
            tfread(in, idx);
            vertices[j] = SurfaceMesh::Vertex(idx);
        }
        mesh.addFace(vertices);
    }

    return true;
}

//-----------------------------------------------------------------------------

bool SurfaceMeshIO::writeOFFBinary(const SurfaceMesh& mesh,
                                   const std::string& filename)
{
    FILE* out = fopen(filename.c_str(), "w");
    if (!out)
        return false;

    fprintf(out, "OFF BINARY\n");
    fclose(out);

    out = fopen(filename.c_str(), "ab");
    tfwrite(out, (unsigned int)mesh.nVertices());
    tfwrite(out, (unsigned int)mesh.nFaces());
    tfwrite(out, (unsigned int)0);
    auto points = mesh.getVertexProperty<Point>("v:point");
    for (auto v : mesh.vertices())
    {
        const Point& p = points[v];
        tfwrite(out, p);
    }

    for (auto f : mesh.faces())
    {
        unsigned int nV = mesh.valence(f);
        tfwrite(out, nV);
        for (auto fv : mesh.vertices(f))
            tfwrite(out, (unsigned int)fv.idx());
    }
    fclose(out);
    return true;
}

//-----------------------------------------------------------------------------

bool SurfaceMeshIO::readOFF(SurfaceMesh& mesh, const std::string& filename)
{
    char line[200];
    bool hasTexcoords = false;
    bool hasNormals = false;
    bool hasColors = false;
    bool hasHcoords = false;
    bool hasDim = false;
    bool isBinary = false;

    // open file (in ASCII mode)
    FILE* in = fopen(filename.c_str(), "r");
    if (!in)
        return false;

    // skip comments and empty lines
    //skipLines(in);

    // read header: [ST][C][N][4][n]OFF BINARY
    char* c = fgets(line, 200, in);
    assert(c != nullptr);
    c = line;
    if (c[0] == 'S' && c[1] == 'T')
    {
        hasTexcoords = true;
        c += 2;
    }
    if (c[0] == 'C')
    {
        hasColors = true;
        ++c;
    }
    if (c[0] == 'N')
    {
        hasNormals = true;
        ++c;
    }
    if (c[0] == '4')
    {
        hasHcoords = true;
        ++c;
    }
    if (c[0] == 'n')
    {
        hasDim = true;
        ++c;
    }
    if (strncmp(c, "OFF", 3) != 0)
    {
        fclose(in);
        return false;
    } // no OFF
    if (strncmp(c + 4, "BINARY", 6) == 0)
        isBinary = true;

    // homogeneous coords, and vertex dimension != 3 are not supported
    if (hasHcoords || hasDim)
    {
        fclose(in);
        return false;
    }

    // if binary: reopen file in binary mode
    if (isBinary)
    {
        fclose(in);
        in = fopen(filename.c_str(), "rb");
        c = fgets(line, 200, in);
        assert(c != nullptr);
    }

    // read as ASCII or binary
    bool ok =
        (isBinary
             ? readOFFBinary(mesh, in, hasNormals, hasTexcoords, hasColors)
             : readOFFAscii(mesh, in, hasNormals, hasTexcoords, hasColors));

    fclose(in);
    return ok;
}

//-----------------------------------------------------------------------------

bool SurfaceMeshIO::writeOFF(const SurfaceMesh& mesh,
                             const std::string& filename)
{
    if (m_options.doBinary())
        return writeOFFBinary(mesh, filename);

    FILE* out = fopen(filename.c_str(), "w");
    if (!out)
        return false;

    bool hasNormals = false;
    bool hasTexcoords = false;
    bool hasColors = false;

    auto normals = mesh.getVertexProperty<Normal>("v:normal");
    auto texcoords = mesh.getVertexProperty<TextureCoordinate>("v:tex");
    auto colors = mesh.getVertexProperty<Color>("v:color");

    if (normals && m_options.doNormals())
        hasNormals = true;
    if (texcoords && m_options.doTexcoords())
        hasTexcoords = true;
    if (colors && m_options.doColors())
        hasColors = true;

    // header
    if (hasTexcoords)
        fprintf(out, "ST");
    if (hasColors)
        fprintf(out, "C");
    if (hasNormals)
        fprintf(out, "N");
    fprintf(out, "OFF\n%zu %zu 0\n", mesh.nVertices(), mesh.nFaces());

    // vertices, and optionally normals and texture coordinates
    SurfaceMesh::VertexProperty<Point> points =
        mesh.getVertexProperty<Point>("v:point");
    for (SurfaceMesh::VertexIterator vit = mesh.verticesBegin();
         vit != mesh.verticesEnd(); ++vit)
    {
        const Point& p = points[*vit];
        fprintf(out, "%.10f %.10f %.10f", p[0], p[1], p[2]);

        if (hasNormals)
        {
            const Normal& n = normals[*vit];
            fprintf(out, " %.10f %.10f %.10f", n[0], n[1], n[2]);
        }

        if (hasColors)
        {
            const Color& c = colors[*vit];
            fprintf(out, " %.10f %.10f %.10f", c[0], c[1], c[2]);
        }

        if (hasTexcoords)
        {
            const TextureCoordinate& t = texcoords[*vit];
            fprintf(out, " %.10f %.10f", t[0], t[1]);
        }

        fprintf(out, "\n");
    }

    // faces
    for (SurfaceMesh::FaceIterator fit = mesh.facesBegin();
         fit != mesh.facesEnd(); ++fit)
    {
        int nV = mesh.valence(*fit);
        fprintf(out, "%d", nV);
        SurfaceMesh::VertexAroundFaceCirculator fvit = mesh.vertices(*fit),
                                                fvend = fvit;
        do
        {
            fprintf(out, " %d", (*fvit).idx());
        } while (++fvit != fvend);
        fprintf(out, "\n");
    }

    fclose(out);
    return true;
}

//=============================================================================

template <typename T>
using VertexProperty = SurfaceMesh::VertexProperty<T>;
template <typename T>
using HalfedgeProperty = SurfaceMesh::HalfedgeProperty<T>;
template <typename T>
using FaceProperty = SurfaceMesh::FaceProperty<T>;
using VertexConnectivity = SurfaceMesh::VertexConnectivity;
using HalfedgeConnectivity = SurfaceMesh::HalfedgeConnectivity;
using HalfedgeFaceConnectivity = SurfaceMesh::HalfedgeFaceConnectivity;
using FaceConnectivity = SurfaceMesh::FaceConnectivity;

//== IMPLEMENTATION ===========================================================

//-----------------------------------------------------------------------------

bool SurfaceMeshIO::readPMP(SurfaceMesh& mesh, const std::string& filename)
{
    // open file (in binary mode)
    FILE* in = fopen(filename.c_str(), "rb");
    if (!in)
        return false;

    // clear mesh
    mesh.clear();

    // how many elements?
    unsigned int nv(0), ne(0), nh(0), nf(0);
    tfread(in, nv);
    tfread(in, ne);
    tfread(in, nf);
    nh = 2 * ne;

    // texture coordinates?
    bool has_htex(false);
    tfread(in, has_htex);

    // resize containers
    mesh.m_vprops.resize(nv);
    mesh.m_hprops.resize(nh);
    mesh.m_eprops.resize(ne);
    mesh.m_fprops.resize(nf);

    // get properties
    auto vconn = mesh.vertexProperty<VertexConnectivity>("v:connectivity");
    auto hconn = mesh.halfedgeProperty<HalfedgeConnectivity>("h:connectivity");
    auto hfconn =
        mesh.halfedgeProperty<HalfedgeFaceConnectivity>("hf:connectivity");
    auto fconn = mesh.faceProperty<FaceConnectivity>("f:connectivity");
    auto point = mesh.vertexProperty<Point>("v:point");

    // read properties from file
    size_t nvc = fread((char*)vconn.data(), sizeof(VertexConnectivity), nv, in);
    size_t nhc =
        fread((char*)hconn.data(), sizeof(HalfedgeConnectivity), nh, in);
    size_t nhfc =
        fread((char*)hfconn.data(), sizeof(HalfedgeFaceConnectivity), nh, in);
    size_t nfc = fread((char*)fconn.data(), sizeof(FaceConnectivity), nf, in);
    size_t np = fread((char*)point.data(), sizeof(Point), nv, in);
    PMP_ASSERT(nvc == nv);
    PMP_ASSERT(nhc == nh);
    PMP_ASSERT(nhfc == nh);
    PMP_ASSERT(nfc == nf);
    PMP_ASSERT(np == nv);

    // read texture coordiantes
    if (has_htex)
    {
        auto htex = mesh.halfedgeProperty<TextureCoordinate>("h:tex");
        size_t nhtc =
            fread((char*)htex.data(), sizeof(TextureCoordinate), nh, in);
        PMP_ASSERT(nhtc == nh);
    }

    fclose(in);
    return true;
}

//-----------------------------------------------------------------------------

bool SurfaceMeshIO::writePMP(const SurfaceMesh& mesh,
                             const std::string& filename)
{
    // open file (in binary mode)
    FILE* out = fopen(filename.c_str(), "wb");
    if (!out)
        return false;

    // get properties
    auto vconn = mesh.getVertexProperty<VertexConnectivity>("v:connectivity");
    auto hconn =
        mesh.getHalfedgeProperty<HalfedgeConnectivity>("h:connectivity");
    auto hfconn =
        mesh.getHalfedgeProperty<HalfedgeFaceConnectivity>("hf:connectivity");
    auto fconn = mesh.getFaceProperty<FaceConnectivity>("f:connectivity");
    auto point = mesh.getVertexProperty<Point>("v:point");
    auto htex = mesh.getHalfedgeProperty<TextureCoordinate>("h:tex");

    // how many elements?
    unsigned int nv, ne, nh, nf;
    nv = mesh.nVertices();
    ne = mesh.nEdges();
    nh = mesh.nHalfedges();
    nf = mesh.nFaces();

    // write header
    tfwrite(out, nv);
    tfwrite(out, ne);
    tfwrite(out, nf);
    tfwrite(out, (bool)htex);

    // write properties to file
    fwrite((char*)vconn.data(), sizeof(VertexConnectivity), nv, out);
    fwrite((char*)hconn.data(), sizeof(HalfedgeConnectivity), nh, out);
    fwrite((char*)hfconn.data(), sizeof(HalfedgeFaceConnectivity), nh, out);
    fwrite((char*)fconn.data(), sizeof(FaceConnectivity), nf, out);
    fwrite((char*)point.data(), sizeof(Point), nv, out);

    // texture coordinates
    if (htex)
        fwrite((char*)htex.data(), sizeof(TextureCoordinate), nh, out);

    fclose(out);
    return true;
}

//-----------------------------------------------------------------------------

// helper class for STL reader
class CmpVec
{
public:
    CmpVec(float eps = FLT_MIN) : m_eps(eps) {}

    bool operator()(const vec3& v0, const vec3& v1) const
    {
        if (fabs(v0[0] - v1[0]) <= m_eps)
        {
            if (fabs(v0[1] - v1[1]) <= m_eps)
            {
                return (v0[2] < v1[2] - m_eps);
            }
            else
                return (v0[1] < v1[1] - m_eps);
        }
        else
            return (v0[0] < v1[0] - m_eps);
    }

private:
    float m_eps;
};

//-----------------------------------------------------------------------------

bool SurfaceMeshIO::readSTL(SurfaceMesh& mesh, const std::string& filename)
{
    char line[100], *c;
    unsigned int i, nT(0);
    vec3 p;
    SurfaceMesh::Vertex v;
    std::vector<SurfaceMesh::Vertex> vertices(3);
    size_t nItems(0);

    CmpVec comp(FLT_MIN);
    std::map<vec3, SurfaceMesh::Vertex, CmpVec> vMap(comp);
    std::map<vec3, SurfaceMesh::Vertex, CmpVec>::iterator vMapIt;

    // clear mesh
    mesh.clear();

    // open file (in ASCII mode)
    FILE* in = fopen(filename.c_str(), "r");
    if (!in)
        return false;

    // ASCII or binary STL?
    c = fgets(line, 6, in);
    PMP_ASSERT(c != nullptr);
    const bool binary =
        ((strncmp(line, "SOLID", 5) != 0) && (strncmp(line, "solid", 5) != 0));

    // parse binary STL
    if (binary)
    {
        // re-open file in binary mode
        fclose(in);
        in = fopen(filename.c_str(), "rb");
        if (!in)
            return false;

        // skip dummy header
        nItems = fread(line, 1, 80, in);
        PMP_ASSERT(nItems > 0);

        // read number of triangles
        tfread(in, nT);

        // read triangles
        while (nT)
        {
            // skip triangle normal
            nItems = fread(line, 1, 12, in);
            PMP_ASSERT(nItems > 0);
            // triangle's vertices
            for (i = 0; i < 3; ++i)
            {
                tfread(in, p);

                // has vector been referenced before?
                if ((vMapIt = vMap.find(p)) == vMap.end())
                {
                    // No : add vertex and remember idx/vector mapping
                    v = mesh.addVertex((Point)p);
                    vertices[i] = v;
                    vMap[p] = v;
                }
                else
                {
                    // Yes : get index from map
                    vertices[i] = vMapIt->second;
                }
            }

            // Add face only if it is not degenerated
            if ((vertices[0] != vertices[1]) && (vertices[0] != vertices[2]) &&
                (vertices[1] != vertices[2]))
                mesh.addFace(vertices);

            nItems = fread(line, 1, 2, in);
            PMP_ASSERT(nItems > 0);
            --nT;
        }
    }

    // parse ASCII STL
    else
    {
        // parse line by line
        while (in && !feof(in) && fgets(line, 100, in))
        {
            // skip white-space
            for (c = line; isspace(*c) && *c != '\0'; ++c)
            {
            };

            // face begins
            if ((strncmp(c, "outer", 5) == 0) || (strncmp(c, "OUTER", 5) == 0))
            {
                // read three vertices
                for (i = 0; i < 3; ++i)
                {
                    // read line
                    c = fgets(line, 100, in);
                    PMP_ASSERT(c != nullptr);

                    // skip white-space
                    for (c = line; isspace(*c) && *c != '\0'; ++c)
                    {
                    };

                    // read x, y, z
                    sscanf(c + 6, "%f %f %f", &p[0], &p[1], &p[2]);

                    // has vector been referenced before?
                    if ((vMapIt = vMap.find(p)) == vMap.end())
                    {
                        // No : add vertex and remember idx/vector mapping
                        v = mesh.addVertex((Point)p);
                        vertices[i] = v;
                        vMap[p] = v;
                    }
                    else
                    {
                        // Yes : get index from map
                        vertices[i] = vMapIt->second;
                    }
                }

                // Add face only if it is not degenerated
                if ((vertices[0] != vertices[1]) &&
                    (vertices[0] != vertices[2]) &&
                    (vertices[1] != vertices[2]))
                    mesh.addFace(vertices);
            }
        }
    }

    fclose(in);
    return true;
}

//-----------------------------------------------------------------------------

bool SurfaceMeshIO::writeSTL(const SurfaceMesh& mesh,
                             const std::string& filename)
{
    if (!mesh.isTriangleMesh())
    {
        std::cerr << "writeSTL: not a triangle mesh!" << std::endl;
        return false;
    }

    auto fnormals = mesh.getFaceProperty<Normal>("f:normal");
    if (!fnormals)
    {
        std::cerr << "writeSTL: no face normals present!" << std::endl;
        return false;
    }

    std::ofstream ofs(filename.c_str());
    auto points = mesh.getVertexProperty<Point>("v:point");

    ofs << "solid stl" << std::endl;
    Normal n;
    Point p;

    for (auto f : mesh.faces())
    {
        n = fnormals[f];
        ofs << "  facet normal ";
        ofs << n[0] << " " << n[1] << " " << n[2] << std::endl;
        ofs << "    outer loop" << std::endl;
        for (auto v : mesh.vertices(f))
        {
            p = points[v];
            ofs << "      vertex ";
            ofs << p[0] << " " << p[1] << " " << p[2] << std::endl;
        }
        ofs << "    endloop" << std::endl;
        ofs << "  endfacet" << std::endl;
    }
    ofs << "endsolid" << std::endl;
    ofs.close();
    return true;
}

//=============================================================================
} // namespace pmp
//=============================================================================
