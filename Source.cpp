#include"Header.h"
#include"GenericFunctions.h"


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

    // Do drawing here
    grid = new bool[WIDTH * HEIGHT];
    color = new vec3[WIDTH * HEIGHT];
    for (GLint x = 0; x < WIDTH * HEIGHT; x++) 
    {
        grid[x] = false;
        color[x] = 0;
    }
}
void reshape(int w, int h) {
    auto oldWidth = width;
    auto oldHeight = height;
    width = w;
    height = h;

    bool* newGrid = new bool[width * height];
    vec3* newcolor = new vec3[width * height];

    for (GLint x = 0; x < width; ++x) {
        for (GLint y = 0; y < height; ++y) {
            if (x < oldWidth && x >= 0 && y < oldHeight && y >= 0) {
                newGrid[x + y * width] = grid[x + y * oldWidth];
                newcolor[x + y * width] = color[x + y * oldWidth];
            }
            else {
                newGrid[x + y * width] = false;
                newcolor[x + y * width] = 0;
            }
        }
    }

    delete[] grid;
    delete[] color;
    grid = newGrid;
    color = newcolor;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width, 0.0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutPostRedisplay();
}
void update(int value) {

    cleargrid();
    
    for (int i = 0; i < object->nfaces(); i++) 
    {
        std::vector<int> face = object->face(i);
        vec2i screen_coords[3];
        vec3 world_coords[3];
        for (int j = 0; j < 3; j++) {
            vec3 v = object->vert(face[j]);
            screen_coords[j] = vec2i((v.x ) * width / 2., (v.y) * height / 2.);
            world_coords[j] = v;
        }
        vec3 n = vec3::cross( (world_coords[2] - world_coords[0]) , (world_coords[1] - world_coords[0]));
        n.normalize();
        float intensity = n * light_dir;
        if(intensity>0)
            Triangle(screen_coords[0], screen_coords[1], screen_coords[2], { intensity,intensity,intensity }, false);
    }
    
    //Triangle({ 0,0 }, { 100,100 }, { 300,-150 }, { 0.50,0.75,0 }, false);
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
                vec3& c = color[x + y * width];
                glColor4f(static_cast<float>(c.x), static_cast<float>(c.y), static_cast<float>(c.z), 1);
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
    }
}

void putpixel(int x, int y, const vec3& col) {
    if (x < width && x >= 0 && y < height && y >= 0) {
        color[x + y * width] = col;
        grid[x + y * width] = true;
    }
}

void putpixel_adjusted(int x, int y, const vec3_T<float>& col) {
    putpixel(x + width / 2, y + height / 2, col);
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
            putpixel_adjusted(x, y, color);
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
            putpixel_adjusted(x, y, color);
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
    putpixel_adjusted(x, y, color);
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
                putpixel_adjusted(j, y, color);
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
                putpixel_adjusted(j, y, color);
        }
        
    }
}