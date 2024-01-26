#ifndef _MY_CLOSEST_PAIR_POINTS_H_
#define _MY_CLOSEST_PAIR_POINTS_H_
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <limits>
#include <vector>
#include <tuple>
typedef struct
{
    unsigned int ID; // the ID of the point
    float x; // the x-coordinate of the point
    float y; // the y-coordinate of the point
} PointType;
float ClosestPairStrip(const std::vector<PointType> &strip, float minDistance,
PointType &closestPoint1, PointType &closestPoint2)
{
    float min = minDistance;
    for (auto point1 = strip.begin(); point1 != strip.end(); ++point1)
    {
        for (auto point2 = point1 + 1; point2 != strip.end() && (point2->y -
        point1->y) < min; ++point2)
        {
            float dist = std::hypot(point1->x - point2->x, point1->y - point2->y);
            if (dist < min)
            {
                min = dist;
                closestPoint1 = point1->ID < point2->ID ? *point1 : *point2;
                closestPoint2 = point1->ID < point2->ID ? *point2 : *point1;
            }
        }
    }
    return min;
}
float ClosestPairRecursive(std::vector<PointType> &sortedByX,
std::vector<PointType> &sortedByY, int start, int end, PointType &closestPoint1,
PointType &closestPoint2)
{
    if (end - start <= 3)
    {
        return std::numeric_limits<float>::max();
    }
    int mid = (start + end) / 2;
    PointType midPoint = sortedByX[mid];
    std::vector<PointType> leftSortedByY, rightSortedByY;
    for (auto &point : sortedByY)
    {
        if (point.x <= midPoint.x)
            leftSortedByY.push_back(point);
        else
            rightSortedByY.push_back(point);
    }
    PointType leftClosestPoint1, leftClosestPoint2, rightClosestPoint1,
    rightClosestPoint2;
    float leftMin = ClosestPairRecursive(sortedByX, leftSortedByY, start, mid,
    leftClosestPoint1, leftClosestPoint2);
    float rightMin = ClosestPairRecursive(sortedByX, rightSortedByY, mid + 1, end,
    rightClosestPoint1, rightClosestPoint2);
    float minDistance = std::min(leftMin, rightMin);
    if (minDistance == leftMin)
    {
        closestPoint1 = leftClosestPoint1;
        closestPoint2 = leftClosestPoint2;
    }
    else
    {
        closestPoint1 = rightClosestPoint1;
        closestPoint2 = rightClosestPoint2;
    }
    std::vector<PointType> strip;
    strip.reserve(sortedByY.size());
    for (auto &point : sortedByY)
    {
        if (std::abs(point.x - midPoint.x) < minDistance)
        {
            strip.push_back(point);
        }
    }
    float stripMin = ClosestPairStrip(strip, minDistance, leftClosestPoint1,
    leftClosestPoint2);
    if (stripMin < minDistance)
    {
        minDistance = stripMin;
        closestPoint1 = leftClosestPoint1;
        closestPoint2 = leftClosestPoint2;
    }
    return minDistance;
}
float ClosestPairOfPoints(std::vector<PointType> &points, PointType &closestPoint1,
PointType &closestPoint2)
{
    std::vector<PointType> sortedByX = points;
    std::sort(sortedByX.begin(), sortedByX.end(), [](const PointType &p1, const
    PointType &p2) {
        return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
    });
    std::vector<PointType> sortedByY = sortedByX;
    std::sort(sortedByY.begin(), sortedByY.end(), [](const PointType &p1, const
    PointType &p2) {
        return p1.y < p2.y || (p1.y == p2.y && p1.x < p2.x);
    });
    float minDistance = ClosestPairRecursive(sortedByX, sortedByY, 0, points.size()
    - 1, closestPoint1, closestPoint2);
    float roundedNumber = std::round(minDistance * 1000.0f) / 1000.0f;
    return roundedNumber;
}
#endif