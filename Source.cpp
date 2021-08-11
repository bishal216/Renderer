#include"Header.h"
#include"GenericFunctions.h"
#include<chrono>

float angleX = 0, angleY = 0, angleZ = 0;
int main(int argc, char** argv)
{
    initcanvas(argc, argv);
    update(0);
    glutMainLoop();

}

void initcanvas(int argc, char** argv)
{
    //Initialize GLUT
    glutInit(&argc, argv);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Renderer");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(myKeyboardFunc);
    // Do drawing here
    grid = new bool[WIDTH * HEIGHT];
    color = new vec3[WIDTH * HEIGHT];
    zBuffer = new float[WIDTH * HEIGHT];
    for (GLint x = 0; x < WIDTH * HEIGHT; x++)
    {
        grid[x] = false;
        color[x] = 0;
        zBuffer[x] = std::numeric_limits<float>::min();
    }
    for (int i = 0; i < object->nfaces(); i++)
        faceList.push_back(object->face(i));
    for (int i = 0; i < object->nverts(); i++)
        vertexList.push_back(object->vert(i));
}
void reshape(int w, int h) {
    auto oldWidth = width;
    auto oldHeight = height;
    width = w;
    height = h;

    bool* newGrid = new bool[width * height];
    vec3* newcolor = new vec3[width * height];
    float* newzBuffer = new float[width * height];
    for (GLint x = 0; x < width; ++x) {
        for (GLint y = 0; y < height; ++y) {
            if (x < oldWidth && x >= 0 && y < oldHeight && y >= 0) {
                newGrid[x + y * width] = grid[x + y * oldWidth];
                newcolor[x + y * width] = color[x + y * oldWidth];
                newzBuffer[x + y * width] = zBuffer[x + y * oldWidth];
            }
            else {
                newGrid[x + y * width] = false;
                newcolor[x + y * width] = 0;
                newzBuffer[x + y * width] = std::numeric_limits<float>::min();
            }
        }
    }

    delete[] grid;
    delete[] color;
    delete[] zBuffer;
    grid = newGrid;
    color = newcolor;
    zBuffer = newzBuffer;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width, 0.0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutPostRedisplay();
}

auto lastframe = std::chrono::high_resolution_clock::now();
float deltatime;

void update(int value) {
    deltatime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - lastframe).count();
    lastframe = std::chrono::high_resolution_clock::now();
    //std::cout << 1e6 / deltatime<<std::endl;
    glutSetWindowTitle(std::to_string(1e6 / deltatime).c_str());
    //const vec3_T<float>& clr = { (float)(rand() % 255) / 255 ,(float)(rand() % 255) / 255 ,(float)(rand() % 255) / 255 };


    cleargrid();

    for (int i = 0; i < faceList.size(); i++)
    {
        face = faceList.at(i);
        //vec2i screen[3];
        for (int j = 0; j < 3; j++)
        {
            v = object->vert(face[j].x);
            v = transform(v);
            points[j] = world2screen(v);
            world[j] = v;
            normal = object->norm(i, j);
            intensity[j] = transformation3D<float>::rotate( normal,rotate)* light_dir;
            if (intensity[j] < 0)
                intensity[j] = 0;
            intensity[j] += 0.2;
        }
        //vec3 n = vec3::cross((world[2] - world[0]), (world[1] - world[0]));
        //vec3 n = vec3::cross((points[2] - points[0]), (points[1] - points[0]));
        //n.normalize();
        //float intensity = n * light_dir;
        //intensity = 0.92;
        //if (intensity > 0)
        //{
        triangle(points, zBuffer, GREEN, intensity);
        //Triangle({ (int)points[0].x, (int)points[0].y }, { (int)points[1].x, (int)points[1].y }, { (int)points[2].x, (int)points[2].y },RED, true);
    //}
    }
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, update, 0);
}
void display() {

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    //     glPointSize(5.5f);

    //auto alpla = sin(((t += 0.1f) + 1) / 2.f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_POINTS);
    for (GLint x = 0; x < width; ++x) {
        for (GLint y = 0; y < height; ++y) {
            if (grid[x + y * width]) {
                //vec3& c = color[x + y * width];
                glColor4f(static_cast<float>(color[x + y * width].x), static_cast<float>(color[x + y * width].y), static_cast<float>(color[x + y * width].z), 1);
                glVertex2i(x, y);
            }
        }
    }
    glEnd();

    glFlush();
    glutSwapBuffers();
}

void cleargrid() {
    for (GLint x = 0; x < width * height; ++x) {
        grid[x] = false;
        color[x] = 0;
        zBuffer[x] = std::numeric_limits<float>::min();
    }
}

void myKeyboardFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
        //rotation
    case 'i':rotate.x += 0.1; break;
    case 'o':rotate.x -= 0.1; break;

    case 'j':rotate.y += 0.1; break;
    case 'k':rotate.y -= 0.1; break;

    case 'n':rotate.z += 0.1; break;
    case 'm':rotate.z -= 0.1; break;
    //scaling
    case 'z': scale += 0.1; break;
    case 'x': scale -= 0.1; break;
    
    //translation
    case 'w': translate.z+=0.01; break;
    case 's': translate.z-=0.01; break;
    
    case 'a': translate.x-=0.01; break;
    case 'd': translate.x+=0.01; break;

    case 'q': translate.y-=0.01; break;
    case 'e': translate.y+=0.01; break;

    case 27: exit(1);
    }
}

void putpixel(int x, int y, float zBuf, const vec3& col) {

    width = (int)width;
    if (x < width && x >= 0 && y < height && y >= 0) {
        if (zBuffer[x + y * width] <= zBuf)
        {
            color[x + y * width] = col;
            grid[x + y * width] = true;
            zBuffer[x + y * width] = zBuf;
        }

    }
}

void LineBresenham_adjusted(int x1, int y1, int x2, int y2, const vec3_T<float>& color)
{
    int dx, dy;
    int steps, k;
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    //Sets increment/decrement : stepsize
    int lx, ly;

    if (x2 > x1) { lx = 1; }
    else { lx = -1; }

    if (y2 > y1) { ly = 1; }
    else { ly = -1; }
    //initialize
    int x = x1, y = y1;
    //slope<1
    if (dx > dy) {
        int p = 2 * dy - dx;
        for (int k = 0; k <= dx; k++) {
            putpixel(x, y, INT_MAX, color);
            if (p < 0) {
                x += lx;
                p += 2 * dy;
            }
            else {
                x += lx;
                y += ly;
                p += 2 * dy - 2 * dx;
            }
        }
    }
    //if slope>=1
    else {
        int p = 2 * dx - dy;
        for (int k = 0; k <= dy; k++) {
            putpixel(x, y, 1, color);
            if (p < 0) {
                y += ly;
                p += 2 * dx;
            }
            else {
                x += lx;
                y += ly;
                p += 2 * dx - 2 * dy;
            }
        }
    }
    putpixel(x, y, INT_MAX, color);
}
void Triangle(vec2i t0, vec2i t1, vec2i t2, const vec3_T<float>& color, bool wireframe)
{
    if (wireframe == false)
        rasterize(t0, t1, t2, color);
    LineBresenham_adjusted(t0.x, t0.y, t1.x, t1.y, color);
    LineBresenham_adjusted(t0.x, t0.y, t2.x, t2.y, color);
    LineBresenham_adjusted(t2.x, t2.y, t1.x, t1.y, color);

}
void rasterize(vec2i V1, vec2i V2, vec2i V3, const vec3_T<float>& color)
{
    if (V1.y == V2.y && V2.y == V3.y) return;
    //Bubble sort on y-position
    if (V1.y > V2.y) { swap(V1, V2); }
    if (V1.y > V3.y) { swap(V1, V3); }
    if (V2.y > V3.y) { swap(V3, V2); }

    //divide triangle into two halves

    int height = V3.y - V1.y;

    for (int y = V1.y; y <= V2.y; y++)
    {
        int partialHeight = V2.y - V1.y + 1; // +1 because both upper and lower limit is included

        float alpha = (float)(y - V1.y) / height;// be careful with divisions by zero 
        if (partialHeight != 0)
        {
            float beta = (float)(y - V1.y) / partialHeight;
            int Ax = (V1.x + (V3.x - V1.x) * alpha), Ay = V1.y + (V3.y - V1.y) * alpha;
            int Bx = V1.x + (V2.x - V1.x) * beta, By = V1.y + (V2.y - V1.y) * beta;
            if (Ax > Bx) { swap(Ax, Bx); }
            for (int j = Ax; j <= Bx; j++)
                putpixel(j, y, 0, color);
        }

    }

    for (int y = V2.y; y <= V3.y; y++)
    {
        int partialHeight = V3.y - V2.y + 1; // +1 because both upper and lower limit is included

        float alpha = (float)(y - V1.y) / height;
        if (partialHeight != 0)
        {
            float beta = (float)(y - V2.y) / partialHeight; // be careful with divisions by zero 

            int Ax = V1.x + (V3.x - V1.x) * alpha, Ay = V1.y + (V3.y - V1.y) * alpha;
            int Bx = V2.x + (V3.x - V2.x) * beta, By = V2.y + (V3.y - V2.y) * beta;
            if (Ax > Bx) { swap(Ax, Bx); }
            for (int j = Ax; j <= Bx; j++)
                putpixel(j, y, 0, color);
        }

    }
}

vec3 world2screen(vec3 v) {

    /*
    mat4 ModelView = lookat(eye, center, vec3(0, 1, 0));
    mat4 Projection;
    mat4 ViewPort = viewport(width*0.1,height*0.1, width * 0.8, height * 0.8);
    Projection(3, 2) = -1.0f / ((eye - center).normalize()).z;
    return (ViewPort * Projection * ModelView * (v));
    */

    float tempx = (float)((int)(((v.x + 1) * width / 2. + .5)));
    float tempy = (float)((int)(((v.y + 1) * height / 2. + .5)));
    float tempz = (float)((v.z));
    vec3 temp = { tempx,tempy,tempz };

    //perspective;
    float r = -1 / eye.z;
    tempx = (float)((int)(temp.x / (1 + r * temp.z) + 0.5));
    tempy = (float)((int)(temp.y / (1 + r * temp.z) + 0.5));
    tempz = temp.z / (1 + r * temp.z);

     temp = { tempx,tempy,tempz };
    return temp;

}

vec3 transform(vec3 pts)
{
    //transformation
    vec3 temp = transformation3D<float>::rotate(pts, rotate);
    temp = transformation3D<float>::scale(temp, scale);
    temp = transformation3D<float>::translate(temp, translate);
    return temp;
}

//-------------------------------------EXPERIMENTAL---------------------------------//

/*
This deals with cross products and centre of masses. I don't exactly know the physics behind it but its better than our original resterizer.
AFAIK, barycentric deals with its own Barycentric co-ordinates.
The vertices of triangle form a simplex and their masses are positive iff
*/
vec3 barycentric(vec3& A, vec3& B, vec3& C, vec3& P) {
    //vec3 s[2];
    //s[0] = { C.x - A.x , B.x - A.x , A.x - P.x };
    //s[1] = { C.y - A.y , B.y - A.y , A.y - P.y };
    
    CoG= vec3::cross({ C.x - A.x , B.x - A.x , A.x - P.x }, { C.y - A.y , B.y - A.y , A.y - P.y });
    if (std::abs(CoG.z) > 0/*1e-2*/) // dont forget that u.z is integer. If it is zero then triangle ABC is degenerate
        return vec3(1.f - (CoG.x + CoG.y) / CoG.z, CoG.y / CoG.z, CoG.x / CoG.z);
    return vec3(-1, 1, 1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

void triangle(vec3* pts, float* zbuffer, const vec3_T<float>& color, float* intensity)
{

    bboxmin = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
    bboxmax = { -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() };

    for (int i = 0; i < 3; i++) {
        //kinda redundant but cannot loop this
        bboxmin.x = std::min(bboxmin.x, pts[i].x);
        bboxmax.x = std::max(bboxmax.x, pts[i].x);

        bboxmin.y = std::min(bboxmin.y, pts[i].y);
        bboxmax.y = std::max(bboxmax.y, pts[i].y);
    }
    vec3 P;
    vec3 clr;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
    {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
        {
            vec3 bc_screen = barycentric(pts[0], pts[1], pts[2], P);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
            clr = color * (bc_screen.x) * intensity[0] + color * (bc_screen.y) * intensity[1] + color * (bc_screen.z) * intensity[2];
            //std::cout << clr << std::endl;
           // clr /= 3;
            P.z = pts[0].z * bc_screen.x + pts[1].z * bc_screen.y + pts[2].z * bc_screen.z + 1;
            putpixel(P.x, P.y, P.z, clr);

        }
    }
}