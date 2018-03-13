/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#include "PointCloud.h"

namespace GPP
{
////////////////////////////////////////////////// PointCloud //////////////////////////////////////////////////
    PointCloud::PointCloud() :
        mHasNormal(false),
        mHasColor(false),
        mIsGrid(false),
        mCoordList(),
        mNormalList(),
        mColorList(),
        mGrid2PointMap(),
        mPoint2GridMap(),
        mGridWidth(0),
        mGridHeight(0),
        mDefaultColor(0.09, 0.48627, 0.69)
    {
    }

    PointCloud::PointCloud(bool hasNormal, bool hasColor, bool isGrid) :
        mHasNormal(hasNormal),
        mHasColor(hasColor),
        mIsGrid(isGrid),
        mCoordList(),
        mNormalList(),
        mColorList(),
        mGrid2PointMap(),
        mPoint2GridMap(),
        mGridWidth(0),
        mGridHeight(0),
        mDefaultColor(0.09, 0.48627, 0.69)
    {
    }

    Int PointCloud::GetPointCount() const
    {
        return mCoordList.size();
    }

    Vector3 PointCloud::GetPointCoord(Int pid) const
    {
        return mCoordList.at(pid);
    }

    void PointCloud::SetPointCoord(Int pid, const Vector3& coord)
    {
        mCoordList.at(pid) = coord;
    }

    Vector3 PointCloud::GetPointNormal(Int pid) const
    {
        if (mHasNormal)
        {
            return mNormalList.at(pid);
        }
        else
        {
            return Vector3(0, 0, 0);
        }
    }

    void PointCloud::SetPointNormal(Int pid, const Vector3& normal)
    {
        mNormalList.at(pid) = normal;
    }

    bool PointCloud::HasNormal() const
    {
        return mHasNormal;
    }
    
    void PointCloud::SetHasNormal(bool hasNormal)
    {
        mHasNormal = hasNormal;
        if (mHasNormal && mNormalList.size() < mCoordList.size())
        {
            mNormalList.resize(mCoordList.size());
        }
    }

    void PointCloud::SetHasColor(bool has)
    {
        mHasColor = has;
        if (mHasColor && mColorList.size() < mCoordList.size())
        {
            mColorList.resize(mCoordList.size());
        }
    }

    bool PointCloud::HasColor(void) const
    {
        return mHasColor;
    }

    Vector3 PointCloud::GetPointColor(Int pid) const
    {
        if (mHasColor)
        {
            return mColorList.at(pid);
        }
        else
        {
            return mDefaultColor;
        }
    }

    void PointCloud::SetPointColor(Int pid, const Vector3& color)
    {
        mColorList.at(pid) = color;
    }

    void PointCloud::ReservePoint(Int pointCount)
    {
        mCoordList.reserve(pointCount);
        if (mHasNormal)
        {
            mNormalList.reserve(pointCount);
        }
        if (mHasColor)
        {
            mColorList.reserve(pointCount);
        }
    }

    Int PointCloud::InsertPoint(const Vector3& coord)
    {
        mCoordList.push_back(coord);
        if (mHasNormal)
        {
            mNormalList.resize(mCoordList.size());
        }
        if (mHasColor)
        {
            mColorList.resize(mCoordList.size());
        }
        if (mIsGrid)
        {
            ClearGridTopologyInfo();
        }
        return mCoordList.size() - 1;
    }

    Int PointCloud::InsertPoint(const Vector3& coord, const Vector3& normal)
    {
        mCoordList.push_back(coord);
        mNormalList.push_back(normal);
        if (mHasColor)
        {
            mColorList.resize(mCoordList.size());
        }
        if (mIsGrid)
        {
            ClearGridTopologyInfo();
        }
        return mCoordList.size() - 1;
    }

    void PointCloud::SwapPoint(Int pointId0, Int pointId1)
    {
        Vector3 tempCoord = mCoordList.at(pointId0);
        mCoordList.at(pointId0) = mCoordList.at(pointId1);
        mCoordList.at(pointId1) = tempCoord;
        if (mHasNormal)
        {
            Vector3 tempNormal = mNormalList.at(pointId0);
            mNormalList.at(pointId0) = mNormalList.at(pointId1);
            mNormalList.at(pointId1) = tempNormal;
        }
        if (mHasColor)
        {
            Vector3 tempColor = mColorList.at(pointId0);
            mColorList.at(pointId0) = mColorList.at(pointId1);
            mColorList.at(pointId1) = tempColor;
        }
        if (mIsGrid)
        {
            Int gridIndex0 = mPoint2GridMap.at(pointId0);
            Int gridIndex1 = mPoint2GridMap.at(pointId1);
            mGrid2PointMap.at(gridIndex0) = pointId1;
            mGrid2PointMap.at(gridIndex1) = pointId0;
            mPoint2GridMap.at(pointId0) = gridIndex1;
            mPoint2GridMap.at(pointId1) = gridIndex0;
        }
    }

    void PointCloud::PopbackPoints(Int popCount)
    {
        Int pointCount = mCoordList.size();
        mCoordList.erase(mCoordList.begin() + pointCount - popCount, mCoordList.end());
        if (mHasNormal)
        {
            mNormalList.erase(mNormalList.begin() + pointCount - popCount, mNormalList.end());
        }
        if (mHasColor)
        {
            mColorList.erase(mColorList.begin() + pointCount - popCount, mColorList.end());
        }
        if (mIsGrid)
        {
            for (Int pid = 1; pid <= popCount; pid++)
            {
                mGrid2PointMap.at(mPoint2GridMap.at(pointCount - pid)) = -1;
            }
            mPoint2GridMap.erase(mPoint2GridMap.begin() + pointCount - popCount, mPoint2GridMap.end());
        }
    }

    void PointCloud::Clear()
    {
        std::vector<Vector3>().swap(mCoordList);
        std::vector<Vector3>().swap(mNormalList);
        std::vector<Vector3>().swap(mColorList);
        if (mIsGrid)
        {
            ClearGridTopologyInfo();
        }
    }

    void PointCloud::InitGrid(Int width, Int height)
    {
        mGridWidth = width;
        mGridHeight = height;
        std::vector<Int>().swap(mPoint2GridMap);
        std::vector<Int>().swap(mGrid2PointMap);
        mGrid2PointMap.resize(width * height, -1);
        mIsGrid = true;
        std::vector<Vector3>().swap(mCoordList);
        std::vector<Vector3>().swap(mNormalList);
        std::vector<Vector3>().swap(mColorList);
    }

    Int PointCloud::GetWidth(void) const
    {
        return mGridWidth;
    }

    Int PointCloud::GetHeight(void) const
    {
        return mGridHeight;
    }

    Int PointCloud::GetValidGridCount(void) const
    {
        return mCoordList.size();
    }
    
    bool PointCloud::IsGridValid(Int wid, Int hid) const
    {
        Int gridIndex = hid * mGridWidth + wid;
        if (gridIndex < 0 || gridIndex >= mGrid2PointMap.size())
        {
            return false;
        }
        return (mGrid2PointMap.at(gridIndex) != -1);
    }

    void PointCloud::InvalidateGrid(Int wid, Int hid)
    {
        Int gridIndex = hid * mGridWidth + wid;
        if (mGrid2PointMap.at(gridIndex) != -1)
        {
            SwapPoint(mGrid2PointMap.at(gridIndex), mPoint2GridMap.size() - 1);
            PopbackPoints(1);
        }
    }

    Vector3 PointCloud::GetGridCoord(Int wid, Int hid) const
    {
        return mCoordList.at(mGrid2PointMap.at(hid * mGridWidth + wid));
    }

    void PointCloud::SetGridCoord(Int wid, Int hid, const Vector3& coord)
    {
        Int gridIndex = hid * mGridWidth + wid;
        if (mGrid2PointMap.at(gridIndex) == -1)
        {
            mGrid2PointMap.at(gridIndex) = mPoint2GridMap.size();
            mPoint2GridMap.push_back(gridIndex);
            mCoordList.push_back(coord);
            if (mHasNormal)
            {
                mNormalList.resize(mCoordList.size());
            }
            if (mHasColor)
            {
                mColorList.resize(mCoordList.size());
            }
        }
        else
        {
            mCoordList.at(mGrid2PointMap.at(gridIndex)) = coord;
        }
    }

    bool PointCloud::HasGridNormal(void) const 
    {
        return mHasNormal;
    }
    
    void PointCloud::SetHasGridNormal(bool has)
    {
        SetHasNormal(has);
    }
    
    Vector3 PointCloud::GetGridNormal(Int wid, Int hid) const
    {
        return mNormalList.at(mGrid2PointMap.at(hid * mGridWidth + wid));
    }

    void PointCloud::SetGridNormal(Int wid, Int hid, const Vector3& normal)
    {
        mNormalList.at(mGrid2PointMap.at(hid * mGridWidth + wid)) = normal;
    }

    bool PointCloud::HasGridTopology(void) const
    {
        return mIsGrid;
    }

    Int PointCloud::GetGridPointId(Int wid, Int hid) const
    {
        return mGrid2PointMap.at(hid * mGridWidth + wid);
    }

    void PointCloud::UnifyCoords(Real bboxSize, Real* scaleValue, Vector3* objCenterCoord)
    {
        if (mCoordList.empty())
        {
            return;
        }
        Vector3 coordMin = mCoordList.at(0);
        Vector3 coordMax = mCoordList.at(0);
        for (std::vector<Vector3>::iterator itr = mCoordList.begin(); itr != mCoordList.end(); ++itr)
        {
            for (Int cid = 0; cid < 3; cid++)
            {
                if ((*itr)[cid] < coordMin[cid])
                {
                    coordMin[cid] = (*itr)[cid];
                }
                else if ((*itr)[cid] > coordMax[cid])
                {
                    coordMax[cid] = (*itr)[cid];
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
            for (std::vector<Vector3>::iterator itr = mCoordList.begin(); itr != mCoordList.end(); ++itr)
            {
                (*itr) = ((*itr) - centerPos) * scaleV;
            }
        }
    }

    void PointCloud::UnifyCoords(Real scaleValue, const Vector3& objCenterCoord)
    {
        if (scaleValue > REAL_TOL)
        {
            for (std::vector<Vector3>::iterator itr = mCoordList.begin(); itr != mCoordList.end(); ++itr)
            {
                (*itr) = ((*itr) - objCenterCoord) * scaleValue;
            }
        }
    }

    void PointCloud::SetDefaultColor(const Vector3& color)
    {
        mDefaultColor = color;
    }

    PointCloud::~PointCloud()
    {
    }

    void PointCloud::ClearGridTopologyInfo()
    {
        mIsGrid = false;
        std::vector<Int>().swap(mGrid2PointMap);
        std::vector<Int>().swap(mPoint2GridMap);
        mGridWidth = 0;
        mGridHeight = 0;
    }
}
