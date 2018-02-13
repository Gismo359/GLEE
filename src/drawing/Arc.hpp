//
// Created by braynstorm on 2/13/18.
//

#ifndef GLEE_ARC_HPP
#define GLEE_ARC_HPP

namespace glee {
    class Arc {
        // http://slabode.exofire.net/circle_draw.shtml

//        void DrawCircle(float cx, float cy, float r, int num_segments)
//        {
//            float theta = 2 * 3.1415926 / float(num_segments);
//            float c = cosf(theta);//precalculate the sine and cosine
//            float s = sinf(theta);
//            float t;
//
//            float x = r;//we start at angle = 0
//            float y = 0;
//
//            glBegin(GL_LINE_LOOP);
//            for(int ii = 0; ii < num_segments; ii++)
//            {
//                glVertex2f(x + cx, y + cy);//output vertex
//
//                //apply the rotation matrix
//                t = x;
//                x = c * x - s * y;
//                y = s * t + c * y;
//            }
//            glEnd();
//        }


    };
}

#endif //GLEE_ARC_HPP
