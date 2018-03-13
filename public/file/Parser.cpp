/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#include "Parser.h"

namespace
{
    ErrorCode ExportGridPointCloudByGTG(std::string fileName, const IGridPointCloud* pointCloud)
    {
        if (!pointCloud)
        {
            GPPInfo << "ExportGridPointCloudByGTG: pointCloud is NULL" << std::endl;
            return GPP_INVALID_INPUT;
        }
        std::ofstream fout(fileName.c_str());
        if (fout.is_open() == false)
        {
            GPPInfo << "ExportGridPointCloudByGTG: file could not open" << std::endl;
            return GPP_INVALID_INPUT;
        }
        fout.precision(20);
        fout << pointCloud->GetWidth() << " " << pointCloud->GetHeight() << " " 
            << pointCloud->GetPointCount() << " " << pointCloud->HasColor() << std::endl;
        Int pointCount = pointCloud->GetPointCount();
        if (pointCloud->HasColor())
        {
            for (Int pid = 0; pid < pointCount; pid++)
            {
                Int xid, yid;
                pointCloud->GetPointGridId(pid, xid, yid);
                Vector3 coord = pointCloud->GetPointCoord(pid);
                Vector3 color = pointCloud->GetPointColor(pid);
                fout << xid << " " << yid << " " << coord[0] << " " << coord[1] << " " << coord[2] << " " 
                    << color[0] << " " << color[1] << " " << color[2] << "\n";
            }
        }
        else
        {
            for (Int pid = 0; pid < pointCount; pid++)
            {
                Int xid, yid;
                pointCloud->GetPointGridId(pid, xid, yid);
                Vector3 coord = pointCloud->GetPointCoord(pid);
                fout << xid << " " << yid << " " << coord[0] << " " << coord[1] << " " << coord[2] << "\n";
            }
        }
        fout.close();
        return GPP_NO_ERROR;
    }

    ErrorCode ExportGridPointCloudByGBG(std::string fileName, const IGridPointCloud* pointCloud)
    {
        if (!pointCloud)
        {
            GPPInfo << "ExportGridPointCloudByGBG: pointCloud is NULL" << std::endl;
            return GPP_INVALID_INPUT;
        }
        FILE *fp = fopen(fileName.c_str(), "wb");
        double maxCoord = 0;
        int width = pointCloud->GetWidth();
        int height = pointCloud->GetHeight();
        fwrite(&width, sizeof(int), 1, fp);
        fwrite(&height, sizeof(int), 1, fp);
        Vector3 coord;
        int validPointCount = 0;
        for (Int wid = 0; wid < width; wid++)
        {
            for (Int hid = 0; hid < height; hid++)
            {
                if (!pointCloud->IsGridValid(wid, hid))
                {
                    continue;
                }
                validPointCount++;
                coord = pointCloud->GetGridCoord(wid, hid);
                for (Int did = 0; did < 3; did++)
                {
                    if (fabs(coord[did]) > maxCoord)
                    {
                        maxCoord = fabs(coord[did]);
                    }
                }
            }
        }
        fwrite(&validPointCount, sizeof(int), 1, fp);
        maxCoord = maxCoord < REAL_TOL ? REAL_TOL : (maxCoord + REAL_TOL);
        maxCoord = 32768 / maxCoord;
        fwrite(&maxCoord, sizeof(double), 1, fp);
        short xyz[3];
        for (Int wid = 0; wid < width; wid++)
        {
            for (Int hid = 0; hid < height; hid++)
            {
                if (!pointCloud->IsGridValid(wid, hid))
                {
                    continue;
                }
                coord = pointCloud->GetGridCoord(wid, hid);
                xyz[0] = coord[0] * maxCoord;
                xyz[1] = coord[1] * maxCoord;
                xyz[2] = coord[2] * maxCoord;
                fwrite(&xyz, sizeof(short), 3, fp);
            }
        }
        if (pointCloud->HasColor())
        {
            int hasColor = 1;
            fwrite(&hasColor, sizeof(int), 1, fp);
            int color;
            for (Int wid = 0; wid < width; wid++)
            {
                for (Int hid = 0; hid < height; hid++)
                {
                    if (!pointCloud->IsGridValid(wid, hid))
                    {
                        continue;
                    }
                    color = Color4::Color4ToInt(Color4::Vector3ToColor4(pointCloud->GetGridColor(wid, hid)));
                    fwrite(&color, sizeof(int), 1, fp);
                }
            }
        }
        else
        {
            int hasColor = 0;
            fwrite(&hasColor, sizeof(int), 1, fp);
        }
        int bitIndex = 0;
        int bitSize = 32;
        int indexValue = 0;
        for (Int wid = 0; wid < width; wid++)
        {
            for (Int hid = 0; hid < height; hid++)
            {
                indexValue |= ((int(pointCloud->IsGridValid(wid, hid))) << bitIndex);
                bitIndex++;
                if (bitIndex == bitSize)
                {
                    fwrite(&indexValue, sizeof(int), 1, fp);
                    bitIndex = 0;
                    indexValue = 0;
                }
            }
        }
        if (bitIndex != 0)
        {
            fwrite(&indexValue, sizeof(int), 1, fp);
        }
        fclose(fp);
        return GPP_NO_ERROR;
    }
    
    static PointCloud* ImportPointCloudByGTG(std::string fileName)
    {
        std::ifstream fin(fileName.c_str());
        if (!fin)
        {
            GPPInfo << "Failed to open file: " << fileName << std::endl;
            return NULL;
        }
        PointCloud* pointCloud = new PointCloud;
        Int width, height, pointCount, hasColor;
        fin >> width >> height >> pointCount >> hasColor;
        pointCloud->InitGrid(width, height);
        Int xid, yid;
        Real x, y, z;
        if (hasColor)
        {
            pointCloud->SetHasColor(true);
            Real r, g, b;
            for (Int pid = 0; pid < pointCount; pid++)
            {
                fin >> xid >> yid >> x >> y >> z >> r >> g >> b;
                pointCloud->SetGridCoord(xid, yid, Vector3(x, y, z));
                pointCloud->SetGridColor(xid, yid, Vector3(r, g, b));
            }
        }
        else
        {
            for (Int pid = 0; pid < pointCount; pid++)
            {
                fin >> xid >> yid >> x >> y >> z;
                pointCloud->SetGridCoord(xid, yid, Vector3(x, y, z));
            }
        }
        fin.close();
        return pointCloud;
    }
    
    static GPP::PointCloud* ImportPointCloudByGBG(std::string fileName)
    {
        FILE *fp = fopen(fileName.c_str(), "rb");
        if (fp == NULL)
        {
            GPPInfo << "ImportPointCloudByGBG:: open failed in binary" << std::endl;
            return NULL;
        }

        int width, height;
        fread(&width, sizeof(int), 1, fp);
        fread(&height, sizeof(int), 1, fp);
        PointCloud* pointCloud = new PointCloud;
        pointCloud->InitGrid(width, height);

        int validPointCount = 0;
        fread(&validPointCount, sizeof(int), 1, fp);
        std::vector<Vector3> coordList;
        coordList.reserve(validPointCount);
        double maxCoord = 0;
        fread(&maxCoord, sizeof(double), 1, fp);
        maxCoord += REAL_TOL;
        const Int patchSize = 100000;
        short xyz[patchSize * 3];
        Int validColorCount = validPointCount;
        while (validPointCount > 0)
        {
            Int readSize = patchSize > validPointCount ? validPointCount : patchSize;
            fread(&xyz, sizeof(short), readSize * 3, fp);
            for (Int pid = 0; pid < readSize; pid++)
            {
                Int baseIndex = pid * 3;
                coordList.push_back(Vector3(double(xyz[baseIndex]) / maxCoord, double(xyz[baseIndex + 1]) / maxCoord, 
                    double(xyz[baseIndex + 2]) / maxCoord));
            }
            validPointCount -= readSize;
        }
        int hasColor = 0;
        fread(&hasColor, sizeof(int), 1, fp);
        std::vector<Int> colorList;
        if (hasColor)
        {
            int colorPatch[patchSize];
            while (validColorCount > 0)
            {
                Int readSize = patchSize > validColorCount ? validColorCount : patchSize;
                fread(&colorPatch, sizeof(int), readSize, fp);
                for (Int pid = 0; pid < readSize; pid++)
                {
                    colorList.push_back(colorPatch[pid]);
                }
                validColorCount -= readSize;
            }
            pointCloud->SetHasColor(true);
        }
        int bitIndex = 0;
        int bitSize = 32;
        int indexValue = 0;
        int gridIndex = 0;
        for (Int wid = 0; wid < width; wid++)
        {
            for (Int hid = 0; hid < height; hid++)
            {
                if (bitIndex == 0)
                {
                    fread(&indexValue, sizeof(int), 1, fp);
                }
                bitIndex = (bitIndex + 1) % bitSize;
                if ((indexValue & (1 << bitIndex)) >> bitIndex)
                {
                    pointCloud->SetGridCoord(wid, hid, coordList.at(gridIndex));
                    if (hasColor)
                    {
                        pointCloud->SetGridColor(wid, hid, Color4::Color4ToVector3(Color4::IntToColor4(colorList.at(gridIndex))));
                    }
                    gridIndex++;
                }
            }
        }
        fclose(fp);
        pointCloud->RemoveOuterBlankGrids();
        return pointCloud;
    }
}
