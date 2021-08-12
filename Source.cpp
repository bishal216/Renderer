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
    /*
    for (int i = 0; i < object->nfaces(); i++)
        faceList.push_back(object->face(i));
    for (int i = 0; i < object->nverts(); i++)
        vertexList.push_back(object->vert(i));
    */
    for (int i = 0; i < object->nfaceData(); i++)
        fDList.push_back(object->returnFaceData(i));
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

//float random = 
//vec3 clr = { std::rand()%255,intensity,intensity };
void update(int value) {
    deltatime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - lastframe).count();
    lastframe = std::chrono::high_resolution_clock::now();
    //std::cout << 1e6 / deltatime<<std::endl;
    glutSetWindowTitle(std::to_string(1e6 / deltatime).c_str());
    //const vec3_T<float>& clr = { (float)(rand() % 255) / 255 ,(float)(rand() % 255) / 255 ,(float)(rand() % 255) / 255 };
    //light_dir.normalize();
    Rotatelight();
    std::cout << light_dir;

    cleargrid();
    for (int i = 0; i < fDList.size(); i++)
    {
        fD = fDList.at(i);
        Drawface(fD);
    }
    /*
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
        //------------------for flat shading
        /*
        vec3 n = vec3::cross((world[2] - world[0]), (world[1] - world[0]));
        //vec3 n = vec3::cross((points[2] - points[0]), (points[1] - points[0]));
        n.normalize();
        float intensity = n * light_dir;
        //intensity = 0.92;
        if (intensity > 0)
        {
            vec3 clr = { intensity,intensity,intensity };
            Triangle({ (int)points[0].x, (int)points[0].y }, { (int)points[1].x, (int)points[1].y }, { (int)points[2].x, (int)points[2].y },clr, false);
        }
        */
        //----------------for Gauraud
        //triangle(points, zBuffer, WHITE, intensity);
    //}

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
    case 'z': scale /= 1.1; break;
    case 'x': scale *= 1.1; break;
    
    //translation
    case 'w': translate.z+=0.01; break;
    case 's': translate.z-=0.01; break;
    
    case 'a': translate.x-=0.01; break;
    case 'd': translate.x+=0.01; break;

    case 'q': translate.y-=0.01; break;
    case 'e': translate.y+=0.01; break;

    //reset
    case 'r': translate = { 0,-1,0 }; rotate = 0; scale = 0.05; break;

    //raster
    case '1': rMode = wireframe; break;
    case '2': rMode = flat; break;
    case '3': rMode = gauraud; break;
    case '4': rMode = phong; break;

    //toggles:
    case 'p': doPers = !doPers; break;
    case 'l': lightRevolve = !lightRevolve; break;
    case 27: exit(1);
    }
}

void putpixel(vec3 P, const vec3& col) {
    //P = { (int)P.x,(int)P.y,(int)P.z };
    width = (int)width, height = (int)height;
    if (rMode == wireframe)
        P.z = 1;
    if (P.x < width && P.x >= 0 && P.y < height && P.y >= 0) 
    {
        if (zBuffer[(int)(P.x + P.y * width)] <= P.z)
        {
            color[(int)(P.x + P.y * width)] = col;
            grid[(int)(P.x + P.y * width)] = true;
            zBuffer[(int)(P.x + P.y * width)] = P.z;
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
    if (doPers)
    {
        float r = -1 / eye.z;
        tempx = (float)((int)(temp.x / (1 + r * temp.z) + 0.5));
        tempy = (float)((int)(temp.y / (1 + r * temp.z) + 0.5));
        tempz = temp.z / (1 + r * temp.z);
    }
    
    
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

    bboxmin = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
    bboxmax = { -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() };

    for (int i = 0; i < 3; i++) {
        //kinda redundant but cannot loop this
        //takes x and y from lowest to highest value of triangle
        bboxmin.x = std::min(bboxmin.x, pts[i].x);
        bboxmax.x = std::max(bboxmax.x, pts[i].x);

        bboxmin.y = std::min(bboxmin.y, pts[i].y);
        bboxmax.y = std::max(bboxmax.y, pts[i].y);

        bboxmax.x = std::min(bboxmax.x, (float)width);
        bboxmax.y = std::min(bboxmax.y, (float)height);

        bboxmin.x = std::max(bboxmin.x, 0.f);
        bboxmin.y = std::max(bboxmin.y, 0.f);

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
            putpixel(P, clr);

        }
    }
}

void Drawface(FaceData face)
{
    for (int j = 0; j < 3; j++)
    {
        
        v = face.vertices[j];           //reads 3 vertices
        v = transform(v);               //transforms said vertices
        points[j] = world2screen(v);    //projects to screen
        world[j] = v;                   //real world co-ords
        n[j] = transformation3D<float>::rotate(face.normal[j],rotate);
        intensity[j] = ((n[j] * light_dir));
    }
    surfaceNormal = vec3::cross((world[2] - world[0]), (world[1] - world[0])).normalize();
    switch (rMode)
    {
    case(wireframe):
        if (surfaceNormal * eye <0)
        {
            LineBresenham_adjusted(points[0], points[1], fD.mtl.Kd);
            LineBresenham_adjusted(points[0],points[2], fD.mtl.Kd);
            LineBresenham_adjusted(points[2],points[1], fD.mtl.Kd);
            
        }break;
    case(flat):
    {   if (surfaceNormal * eye < 0)
        if (vec3(0,0,-1) *surfaceNormal > 0)
                Flatrasterize(points[0], points[1], points[2], fD.mtl.Kd);
    }break;
    case(gauraud):
    {
        triangle(points, intensity, face.mtl.Ka, face.mtl.Kd);
    }break;
    }
}
//For Wireframe
void LineBresenham_adjusted(vec3 v1, vec3 v2, const vec3_T<float>& color)
{
    int x1 = v1.x, x2 = v2.x, y1 = v1.y, y2 = v2.y;
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
            putpixel({ (float)x, (float)y, (float)INT_MAX }, color);
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
            putpixel({ (float)x, (float)y, (float)INT_MAX }, color);
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
    putpixel({ (float)x, (float)y, (float)INT_MAX }, color);
}
//For Flat Shading
void Flatrasterize(vec3 V1, vec3 V2, vec3 V3, const vec3_T<float>& color)
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
            int Ax = (V1.x + (V3.x - V1.x) * alpha), Ay = V1.y + (V3.y - V1.y) * alpha, Az = V1.z + (V3.z - V1.z) * alpha;
            int Bx = V1.x + (V2.x - V1.x) * beta, By = V1.y + (V2.y - V1.y) * beta, Bz = V1.z + (V3.z - V1.z) * beta;

            if (Ax > Bx) { swap(Ax, Bx); }
            for (int j = Ax; j <= Bx; j++)
                putpixel({ (float)j, (float)y, (float)INT_MAX }, color);
        }

    }

    for (int y = V2.y; y <= V3.y; y++)
    {
        int partialHeight = V3.y - V2.y + 1; // +1 because both upper and lower limit is included

        float alpha = (float)(y - V1.y) / height;
        if (partialHeight != 0)
        {
            float beta = (float)(y - V2.y) / partialHeight; // be careful with divisions by zero 

            int Ax = V1.x + (V3.x - V1.x) * alpha, Ay = V1.y + (V3.y - V1.y) * alpha, Az = V1.z + (V3.z - V1.z) * alpha;
            int Bx = V2.x + (V3.x - V2.x) * beta, By = V2.y + (V3.y - V2.y) * beta, Bz = V2.z + (V3.z - V2.z) * beta;
            if (Ax > Bx) { swap(Ax, Bx); }
            for (int j = Ax; j <= Bx; j++)
                putpixel({ (float)j, (float)y, (float)INT_MAX }, color);
        }

    }
}
//For Gauraud Shading
void triangle(vec3* pts,float* intensity,vec3 ka,vec3 kd)
{

    bboxmin = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
    bboxmax = { -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() };

    for (int i = 0; i < 3; i++) {
        //kinda redundant but cannot loop this
        //takes x and y from lowest to highest value of triangle
        bboxmin.x = std::min(bboxmin.x, pts[i].x);
        bboxmax.x = std::max(bboxmax.x, pts[i].x);

        bboxmin.y = std::min(bboxmin.y, pts[i].y);
        bboxmax.y = std::max(bboxmax.y, pts[i].y);

        bboxmax.x = std::min(bboxmax.x, (float)width);
        bboxmax.y = std::min(bboxmax.y, (float)height);

        bboxmin.x = std::max(bboxmin.x, 0.f);
        bboxmin.y = std::max(bboxmin.y, 0.f);

    }
    vec3 P;
    vec3 clr;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
    {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
        {
            vec3 bc_screen = barycentric(pts[0], pts[1], pts[2], P);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;

            float it = (bc_screen.x) * intensity[0] + (bc_screen.y) * intensity[1] + (bc_screen.z) * intensity[2];
            clr = ka + kd * it;
            clr /= 2;
            P.z = pts[0].z * bc_screen.x + pts[1].z * bc_screen.y + pts[2].z * bc_screen.z + 1;
            putpixel(P, clr);
        }
    }
}
//For Phong Shading
void triangle(vec3* pts, vec3* normal, vec3 ka, vec3 kd,vec3 ks)
{

    bboxmin = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
    bboxmax = { -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() };

    for (int i = 0; i < 3; i++) {
        //kinda redundant but cannot loop this
        //takes x and y from lowest to highest value of triangle
        bboxmin.x = std::min(bboxmin.x, pts[i].x);
        bboxmax.x = std::max(bboxmax.x, pts[i].x);

        bboxmin.y = std::min(bboxmin.y, pts[i].y);
        bboxmax.y = std::max(bboxmax.y, pts[i].y);

        bboxmax.x = std::min(bboxmax.x, (float)width);
        bboxmax.y = std::min(bboxmax.y, (float)height);

        bboxmin.x = std::max(bboxmin.x, 0.f);
        bboxmin.y = std::max(bboxmin.y, 0.f);

    }
    vec3 P;
    vec3 clr;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
    {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
        {
            vec3 bc_screen = barycentric(pts[0], pts[1], pts[2], P);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;

            float it = (bc_screen.x) * intensity[0] + (bc_screen.y) * intensity[1] + (bc_screen.z) * intensity[2];
            clr = ka + kd * it;
            clr /= 2;
            P.z = pts[0].z * bc_screen.x + pts[1].z * bc_screen.y + pts[2].z * bc_screen.z + 1;
            putpixel(P, clr);
        }
    }
}
void Rotatelight()
{
    if (lightRevolve)
    {
        light_dir.x = cos(theta  * 3.1415f / 180);
        light_dir.y = sin(theta  * 3.1415f / 180);
        theta+=stepsize;
        if (theta > 180 || theta < 0)
            stepsize = -stepsize;
        
    }
}