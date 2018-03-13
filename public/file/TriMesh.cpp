/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#include "TriMesh.h"
#include <map>
#include "GppInternal.h"
#include "ToolAnn.h"
#include "IPointList.h"

namespace GPP
{
    VertexInfo::VertexInfo() :
        mCoord(0, 0, 0),
        mNormal(0, 0, 0)
    {
    }

    VertexInfo::VertexInfo(const Vector3& coord) :
        mCoord(coord),
        mNormal(0, 0, 0)
    {
    }

    VertexInfo::VertexInfo(const Vector3& coord, const Vector3& normal) :
        mCoord(coord),
        mNormal(normal)
    {
    }

    TriangleInfo::TriangleInfo(Int vertexId0, Int vertexId1, Int vertexId2) :
        mNormal()
    {
        mIndex[0] = vertexId0;
        mIndex[1] = vertexId1;
        mIndex[2] = vertexId2;
    }

    TriMesh::TriMesh() :
        mMeshType(MT_NORMAL),
        mVertexList(),
        mTriangleList(),
        mVertexColorList(),
        mVertexTexCoordList(),
        mTriangleTexCoordList(),
        mHasColor(false),
        mHasVertexTexCoord(false),
        mHasTriangleTexCoord(false),
        mDefaultColor(0.86, 0.86, 0.86)
    {
    }

    TriMesh::TriMesh(bool hasColor, bool hasVertexTexCoord, bool hasTriangleTexCoord) :
        mMeshType(MT_NORMAL),
        mVertexList(),
        mTriangleList(),
        mVertexColorList(),
        mVertexTexCoordList(),
        mTriangleTexCoordList(),
        mHasColor(hasColor),
        mHasVertexTexCoord(hasVertexTexCoord),
        mHasTriangleTexCoord(hasTriangleTexCoord),
        mDefaultColor(0.86, 0.86, 0.86)
    {

    }

    Int TriMesh::GetVertexCount() const
    {
        return mVertexList.size();
    }

    Vector3 TriMesh::GetVertexCoord(Int vid) const
    {
        return mVertexList.at(vid)->mCoord;
    }

    void TriMesh::SetVertexCoord(Int vid, const Vector3& coord)
    {
        mVertexList.at(vid)->mCoord = coord;
    }

    Vector3 TriMesh::GetVertexNormal(Int vid) const
    {
        return mVertexList.at(vid)->mNormal;
    }

    void TriMesh::SetVertexNormal(Int vid, const Vector3& normal)
    {
        mVertexList.at(vid)->mNormal = normal;
    }

    Int TriMesh::GetTriangleCount() const
    {
        return mTriangleList.size();
    }

    void TriMesh::GetTriangleVertexIds(Int fid, Int vertexIds[3]) const
    {
        vertexIds[0] = mTriangleList.at(fid)->mIndex[0];
        vertexIds[1] = mTriangleList.at(fid)->mIndex[1]; 
        vertexIds[2] = mTriangleList.at(fid)->mIndex[2]; 
    }

    void TriMesh::SetTriangleVertexIds(Int fid, Int vertexId0, Int vertexId1, Int vertexId2)
    {
        mTriangleList.at(fid)->mIndex[0] = vertexId0;
        mTriangleList.at(fid)->mIndex[1] = vertexId1;
        mTriangleList.at(fid)->mIndex[2] = vertexId2;
    }

    Vector3 TriMesh::GetTriangleNormal(Int fid) const
    {
        return mTriangleList.at(fid)->mNormal;
    }

    void TriMesh::SetTriangleNormal(Int fid, const Vector3& normal)
    {
        mTriangleList.at(fid)->mNormal = normal;
    }

    void TriMesh::SetMeshType(MeshType meshType)
    {
        mMeshType = meshType;
    }

    MeshType TriMesh::GetMeshType(void) const
    {
        return mMeshType;
    }

    Vector3 TriMesh::GetVertexColor(Int vid) const
    {
        if (mHasColor)
        {
            return mVertexColorList.at(vid);
        }
        else
        {
            return mDefaultColor;
        }
    }

    void TriMesh::SetVertexColor(Int vid, const Vector3& color)
    {
        mVertexColorList.at(vid) = color;
    }

    Vector3 TriMesh::GetVertexTexcoord(Int vid) const
    {
        return mVertexTexCoordList.at(vid);
    }

    void TriMesh::SetVertexTexcoord(Int vid, const Vector3& texcoord)
    {
        mVertexTexCoordList.at(vid) = texcoord;
    }

    Vector3 TriMesh::GetTriangleTexcoord(Int fid, Int localVid) const
    {
        return mTriangleTexCoordList.at(fid * 3 + localVid);
    }

    void TriMesh::SetTriangleTexcoord(Int fid, Int localVid, const Vector3& texcoord)
    {
        mTriangleTexCoordList.at(fid * 3 + localVid) = texcoord;
    }

    Int TriMesh::InsertVertex(const Vector3& coord)
    {
        VertexInfo* vertex = new VertexInfo(coord);
        mVertexList.push_back(vertex);
        if (mHasColor)
        {
            mVertexColorList.push_back(GPP::Vector3(0, 0, 0));
        }
        if (mHasVertexTexCoord)
        {
            mVertexTexCoordList.push_back(GPP::Vector3(0, 0, 0));
        }
        return (mVertexList.size() - 1);
    }
    
    Int TriMesh::InsertVertex(const Vector3& coord, const Vector3& normal)
    {
        VertexInfo* vertex = new VertexInfo(coord, normal);
        mVertexList.push_back(vertex);
        if (mHasColor)
        {
            mVertexColorList.push_back(GPP::Vector3(0, 0, 0));
        }
        if (mHasVertexTexCoord)
        {
            mVertexTexCoordList.push_back(GPP::Vector3(0, 0, 0));
        }
        return (mVertexList.size() - 1);
    }

    ErrorCode TriMesh::FuseVertex()
    {
        Real functionStartTime = Profiler::GetTime();
        Int vertexCount = mVertexList.size();
        if (vertexCount < 2)
        {
            GPPInfo << "TriMesh::FuseVertex: vertexCount < 3" << std::endl;
            return GPP_INVALID_INPUT;
        }
        std::vector<Int> vertex2FusedMap(vertexCount);
        std::vector<bool> vertexValidFlag(vertexCount, 0);
        std::map<Vector3, Int> vertexCoordMap;
        Int fusedId = 0;
        for (Int vid = 0; vid < vertexCount; vid++)
        {
            std::pair<std::map<Vector3, Int>::iterator, bool> insertRes = vertexCoordMap.insert(std::pair<Vector3, Int>(mVertexList.at(vid)->mCoord, fusedId));
            if (insertRes.second)
            {
                vertex2FusedMap.at(vid) = fusedId;
                fusedId++;
                vertexValidFlag.at(vid) = 1;
            }
            else
            {
                vertex2FusedMap.at(vid) = vertexCoordMap[mVertexList.at(vid)->mCoord];
            }
        }
        std::vector<VertexInfo*> newVertexList;
        mVertexList.swap(newVertexList);
        mVertexList.reserve(fusedId);
        for (Int vid = 0; vid < vertexCount; vid++)
        {
            if (!vertexValidFlag.at(vid))
            {
                GPPFREEPOINTER(newVertexList.at(vid));
            }
            else
            {
                mVertexList.push_back(newVertexList.at(vid));
            }
        }
        newVertexList.clear();
        Int faceCount = mTriangleList.size();
        for (Int fid = 0; fid < faceCount; fid++)
        {
            mTriangleList.at(fid)->mIndex[0] = vertex2FusedMap.at(mTriangleList.at(fid)->mIndex[0]);
            mTriangleList.at(fid)->mIndex[1] = vertex2FusedMap.at(mTriangleList.at(fid)->mIndex[1]);
            mTriangleList.at(fid)->mIndex[2] = vertex2FusedMap.at(mTriangleList.at(fid)->mIndex[2]);
        }
        GPPDebug << "TriMesh::FuseVertex time: " << Profiler::GetTime() - functionStartTime << std::endl;
        mMeshType = MT_NORMAL;
        return GPP_NO_ERROR;
    }

    void TriMesh::Clear()
    {
        for (std::vector<VertexInfo* >::iterator vertexItr = mVertexList.begin(); vertexItr != mVertexList.end(); ++vertexItr)
        {
            GPPFREEPOINTER((*vertexItr));
        }
        mVertexList.clear();
        for (std::vector<TriangleInfo* >::iterator faceItr = mTriangleList.begin(); faceItr != mTriangleList.end(); ++faceItr)
        {
            GPPFREEPOINTER(*faceItr);
        }
        mTriangleList.clear();
        std::vector<Vector3>().swap(mVertexColorList);
        std::vector<Vector3>().swap(mVertexTexCoordList);
        std::vector<Vector3>().swap(mTriangleTexCoordList);
    }

    void TriMesh::SetHasColor(bool has)
    {
        mHasColor = has;
        if (mHasColor && mVertexColorList.size() < mVertexList.size())
        {
            mVertexColorList.resize(mVertexList.size());
        }
    }

    bool TriMesh::HasColor(void) const
    {
        return mHasColor;
    }

    void TriMesh::SetHasVertexTexCoord(bool has)
    {
        mHasVertexTexCoord = has;
        if (mHasVertexTexCoord && mVertexTexCoordList.size() < mVertexList.size())
        {
            mVertexTexCoordList.resize(mVertexList.size());
        }
    }

    bool TriMesh::HasVertexTexCoord(void) const
    {
        return mHasVertexTexCoord;
    }

    void TriMesh::SetHasTriangleTexCoord(bool has)
    {
        mHasTriangleTexCoord = has;
        if (mHasTriangleTexCoord && mTriangleTexCoordList.size() < mTriangleList.size() * 3)
        {
            mTriangleTexCoordList.resize(mTriangleList.size() * 3);
        }
    }

    bool TriMesh::HasTriangleTexCoord(void) const
    {
        return mHasTriangleTexCoord;
    }

    Int TriMesh::InsertTriangle(Int vertexId0, Int vertexId1, Int vertexId2)
    {
        TriangleInfo* triangleInfo = new TriangleInfo(vertexId0, vertexId1, vertexId2);
        mTriangleList.push_back(triangleInfo);
        if (mHasTriangleTexCoord)
        {
            mTriangleTexCoordList.resize(mTriangleTexCoordList.size() + 3);
        }
        return (mTriangleList.size() - 1);
    }

    void TriMesh::UnifyCoords(Real bboxSize, Real* scaleValue, Vector3* objCenterCoord)
    {
        Vector3 coordMin(REAL_LARGE, REAL_LARGE, REAL_LARGE);
        Vector3 coordMax(-REAL_LARGE, -REAL_LARGE, -REAL_LARGE);
        for (std::vector<VertexInfo* >::iterator itr = mVertexList.begin(); itr != mVertexList.end(); ++itr)
        {
            Vector3 coord = (*itr)->mCoord;
            for (Int cid = 0; cid < 3; cid++)
            {
                if (coord[cid] < coordMin[cid])
                {
                    coordMin[cid] = coord[cid];
                }
                else if (coord[cid] > coordMax[cid])
                {
                    coordMax[cid] = coord[cid];
                }
            }
        }
        Vector3 scale3 = coordMax - coordMin;
        GPP::Real scaleMax = scale3[0];
        if (scaleMax < scale3[1])
        {
            scaleMax = scale3[1];
        }
        if (scaleMax < scale3[2])
        {
            scaleMax = scale3[2];
        }
        if (scaleMax > REAL_TOL)
        {
            GPP::Real scaleV = bboxSize / scaleMax;
            if (scaleValue != NULL)
            {
                *scaleValue = scaleV;
            }
            Vector3 centerPos = (coordMin + coordMax) / 2.0;
            if (objCenterCoord != NULL)
            {
                *objCenterCoord = centerPos;
            }
            for (std::vector<VertexInfo* >::iterator itr = mVertexList.begin(); itr != mVertexList.end(); ++itr)
            {
                (*itr)->mCoord = ((*itr)->mCoord - centerPos) * scaleV;
            }
        }
    }

    void TriMesh::UnifyCoords(Real scaleValue, const Vector3& objCenterCoord)
    {
        if (scaleValue > REAL_TOL)
        {
            for (std::vector<VertexInfo* >::iterator itr = mVertexList.begin(); itr != mVertexList.end(); ++itr)
            {
                (*itr)->mCoord = ((*itr)->mCoord - objCenterCoord) * scaleValue;
            }
        }
    }

    void TriMesh::SetDefaultColor(Vector3 color)
    {
        mDefaultColor = color;
    }

    void TriMesh::SwapVertex(Int vertexId0, Int vertexId1)
    {
        VertexInfo* temp = mVertexList.at(vertexId0);
        mVertexList.at(vertexId0) = mVertexList.at(vertexId1);
        mVertexList.at(vertexId1) = temp;
        if (mHasColor)
        {
            Vector3 tempColor = mVertexColorList.at(vertexId0);
            mVertexColorList.at(vertexId0) = mVertexColorList.at(vertexId1);
            mVertexColorList.at(vertexId1) = tempColor;
        }
        if (mHasVertexTexCoord)
        {
            Vector3 tempCoord = mVertexTexCoordList.at(vertexId0);
            mVertexTexCoordList.at(vertexId0) = mVertexTexCoordList.at(vertexId1);
            mVertexTexCoordList.at(vertexId1) = tempCoord;
        }
    }

    void TriMesh::PopbackVertices(Int popCount)
    {
        Int vertexCount = mVertexList.size();
        for (Int vid = 0; vid < popCount; vid++)
        {
            GPPFREEPOINTER(mVertexList.at(vertexCount - 1 - vid));
        }
        mVertexList.erase(mVertexList.begin() + vertexCount - popCount, mVertexList.end());
        if (mHasColor)
        {
            mVertexColorList.erase(mVertexColorList.begin() + vertexCount - popCount, mVertexColorList.end());
        }
        if (mHasVertexTexCoord)
        {
            mVertexTexCoordList.erase(mVertexTexCoordList.begin() + vertexCount - popCount, mVertexTexCoordList.end());
        }
    }

    void TriMesh::SwapTriangles(Int fid0, Int fid1)
    {
        TriangleInfo* temp = mTriangleList.at(fid0);
        mTriangleList.at(fid0) = mTriangleList.at(fid1);
        mTriangleList.at(fid1) = temp;
        if (mHasTriangleTexCoord)
        {
            Int baseIndex0 = fid0 * 3;
            Int baseIndex1 = fid1 * 3;
            for (Int localId = 0; localId < 3; localId++)
            {
                Vector3 tempCoord = mTriangleTexCoordList.at(baseIndex0 + localId);
                mTriangleTexCoordList.at(baseIndex0 + localId) = mTriangleTexCoordList.at(baseIndex1 + localId);
                mTriangleTexCoordList.at(baseIndex1 + localId) = tempCoord;
            }
        }
    }

    void TriMesh::PopbackTriangles(Int popCount)
    {
        Int faceCount = mTriangleList.size();
        for (Int fid = 0; fid < popCount; fid++)
        {
            GPPFREEPOINTER(mTriangleList.at(faceCount - 1 - fid));
        }
        mTriangleList.erase(mTriangleList.begin() + mTriangleList.size() - popCount, mTriangleList.end());
        if (mHasTriangleTexCoord)
        {
            mTriangleTexCoordList.erase(mTriangleTexCoordList.begin() + mTriangleTexCoordList.size() - popCount * 3, mTriangleTexCoordList.end());
        }
    }

    void TriMesh::UpdateNormal()
    {
        Real functionStartTime = Profiler::GetTime();
        Int vertexCount = mVertexList.size();
        std::vector<Vector3> normalList(vertexCount, Vector3(0, 0, 0));
        Int triangleCount = mTriangleList.size();
        for (Int fid = 0; fid < triangleCount; fid++)
        {
            TriangleInfo* triangleInfo = mTriangleList.at(fid);
            Vector3 coord0 = mVertexList.at(triangleInfo->mIndex[0])->mCoord;
            Vector3 coord1 = mVertexList.at(triangleInfo->mIndex[1])->mCoord;
            Vector3 coord2 = mVertexList.at(triangleInfo->mIndex[2])->mCoord;
            triangleInfo->mNormal = (coord1 - coord0).CrossProduct(coord2 - coord0);
            normalList.at(triangleInfo->mIndex[0]) += triangleInfo->mNormal;
            normalList.at(triangleInfo->mIndex[1]) += triangleInfo->mNormal;
            normalList.at(triangleInfo->mIndex[2]) += triangleInfo->mNormal;
            triangleInfo->mNormal.Normalise();
        }
        for (Int vid = 0; vid < vertexCount; vid++)
        {
            GPP::Real normalLen = normalList.at(vid).Normalise();
            if (normalLen < REAL_TOL)
            {
                normalList.at(vid)[0] = 1.0;
            }
            mVertexList.at(vid)->mNormal = normalList.at(vid);
        }
    }

    TriMesh::~TriMesh()
    {
        for (std::vector<VertexInfo* >::iterator vertexItr = mVertexList.begin(); vertexItr != mVertexList.end(); ++vertexItr)
        {
            GPPFREEPOINTER(*vertexItr);
        }
        mVertexList.clear();
        for (std::vector<TriangleInfo* >::iterator faceItr = mTriangleList.begin(); faceItr != mTriangleList.end(); ++faceItr)
        {
            GPPFREEPOINTER(*faceItr);
        }
        mTriangleList.clear();
    }
}
