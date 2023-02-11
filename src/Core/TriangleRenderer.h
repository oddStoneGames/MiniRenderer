/// Draws a triangle using bounds.
#pragma once

#include "Maths/Maths.h"
#include "Framebuffer.h"
#include <cmath>

namespace MiniRenderer
{
    Vec3f Barycentric(Vec2i* pts, Vec2i P)
    {
        Vec3f u = Cross(Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - P.x), Vec3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - P.y));
        /* `pts` and `P` has integer value as coordinates
           so `abs(u[2])` < 1 means `u[2]` is 0, that means
           triangle is degenerate, in this case return something with negative coordinates */
        if (Abs(u.z) < 1) return Vec3f(-1, 1, 1);
        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    }

    void DrawTriangle(Vec2i* pts, uint32_t color, Framebuffer& buffer)
    {
        int width = buffer.GetFramebufferWidth();
        int height = buffer.GetFramebufferHeight();
        Vec2i bboxmin(width - 1, height - 1);
        Vec2i bboxmax(0, 0);
        Vec2i clamp(width - 1, height - 1);
        for (int i = 0; i < 3; i++)
        {
            bboxmin.x = Max(0, Min(bboxmin.x, pts[i].x));
            bboxmin.y = Max(0, Min(bboxmin.y, pts[i].y));

            bboxmax.x = Min(clamp.x, Max(bboxmax.x, pts[i].x));
            bboxmax.y = Min(clamp.y, Max(bboxmax.y, pts[i].y));
        }
        Vec2i P;
        for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
        {
            for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
            {
                Vec3f bc_screen = Barycentric(pts, P);
                if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
                buffer.SetPixelColor(P.x, P.y, color);
            }
        }
    }

}