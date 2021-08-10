#include"Header.h"



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
    zBuffer = new float[WIDTH * HEIGHT];
    for (GLint x = 0; x < WIDTH * HEIGHT; x++)
    {
        grid[x] = false;
        color[x] = 0;
        zBuffer[x] = std::numeric_limits<float>::min();
    }
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
    glutSetWindowTitle(std::to_string(1e6 / deltatime).c_str());
    cleargrid();
    for (int i = 0; i < object->nfaces(); i++)
    {
        std::vector<vec3i> face = object->face(i);
        vec3 points[3], world[3];   //Screen and World co-ords
        float intensity[3];         //intensity values for Gouraud shading
        vec3 light[3];
        vec3 normal[3];             //normal values for Phong shading
        vec3 view[3];             //view vector
        vec3 reflection[3];             //reflection vector
        float intnsity;
        for (int j = 0; j < 3; j++)
        {
            vec3 v = object->vert(face[j].x);       //normalized
            
            points[j] = reverseNormalization(v);    //in Int
            vec3 n = vec3::cross((points[2] - points[0]), (points[1] - points[0]));
            n.normalize();
            //For phong
            light[j] = (light_dir-points[j]).normalize();
            normal[j] = object->norm(i, j);
            view[j] = (prp).normalize();
            reflection[j] = -light[j] + normal[j] * 2 * (light[j] * normal[j]);
            //for gauraud
            intensity[j] = object->norm(i, j) * light_dir.normalize() +0.4;
            points[j] = project(points[j]);
            intnsity = n * prp + 0.4;
        }
        //if(intnsity>0)
            triangle(points, {135.f/255,206.f/255,235.f/255},light, normal,view,reflection,intensity,0);//0:Gauraud,1:Phong

    }
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, update, 0);
}
void display() 
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
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

void cleargrid() 
{
    for (GLint x = 0; x < width * height; ++x) {
        grid[x] = false;
        color[x] = 0;
        zBuffer[x] = std::numeric_limits<float>::min();
    }
}

void putpixel(int x, int y, float zBuf, const vec3& col) 
{
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

vec3 reverseNormalization(vec3 v)
{
    float tempx = (float)((int)(((v.x + 1) * width / 2. + .5)));
    float tempy = (float)((int)(((v.y + 1) * height / 2. + .5)));
    float tempz = (float)((v.z+1));
    vec3 temp = { tempx,tempy,tempz };
    return temp;
}

vec3 project(vec3 point) {

    float foV = 1;
    float zFar = -1000;
    float zNear = 1;
    float aspect = 1.0f * width / height;
    mat4 Result;
    Result(0,0) = (1) / (aspect * foV);
    Result(1,1) = foV;
    Result(2,2) = zFar / (zFar - zNear);
    Result(2,3) = 1;
    Result(3,2) = -(zFar * zNear) / (zFar - zNear);
    return Result * point;
   

}

//-------------------------------------EXPERIMENTAL---------------------------------//
/*
This deals with cross products and centre of masses. I don't exactly know the physics behind it 
//but its better than our original resterizer.
AFAIK, barycentric deals with its own Barycentric co-ordinates.
The vertices of triangle form a simplex and their masses are positive iff
*/
vec3 barycentric(vec3 A, vec3 B, vec3 C, vec3 P) {
    vec3 s[2];
    s[0] = { C.x - A.x , B.x - A.x , A.x - P.x };
    s[1] = { C.y - A.y , B.y - A.y , A.y - P.y };
    vec3 u = vec3::cross(s[0], s[1]);
    if (std::abs(u.z) > 0/*1e-2*/) // dont forget that u.z is integer. If it is zero then triangle ABC is degenerate
        return vec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    return vec3(-1, 1, 1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}
void triangle(vec3* pts, const vec3_T<float>& color, vec3* L, vec3* N, vec3* V, vec3* R,float* intensity,bool mode)
{

    vec2 bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    vec2 bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    for (int i = 0; i < 3; i++) {

        //kinda redundant but cannot loop this
        bboxmin.x = std::min(bboxmin.x, pts[i].x);
        bboxmax.x = std::max(bboxmax.x, pts[i].x);

        bboxmin.y = std::min(bboxmin.y, pts[i].y);
        bboxmax.y = std::max(bboxmax.y, pts[i].y);
    }

    vec3 P;
    vec3 clr = color;
    
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
    {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
        {
            vec3 bc_screen = barycentric(pts[0], pts[1], pts[2], P);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
            P.z = pts[0].z * bc_screen.x + pts[1].z * bc_screen.y + pts[2].z * bc_screen.z + 1;
            if (mode == 0)
            {
                clr = color * (bc_screen.x) * intensity[0] + color * (bc_screen.y) * intensity[1] + color * (bc_screen.z) * intensity[2];
                clr *= kd;
                clr = { clamp(clr.x),clamp(clr.y),clamp(clr.z) };
                clr += ka;
                P.z = pts[0].z * bc_screen.x + pts[1].z * bc_screen.y + pts[2].z * bc_screen.z + 1;
            }
            if (mode == 1)
            {
                vec3 n = N[0] * bc_screen.x + N[1] * bc_screen.y + N[2] * bc_screen.z;
                vec3 l = L[0] * bc_screen.x + L[1] * bc_screen.y + L[2] * bc_screen.z;
                vec3 v = V[0] * bc_screen.x + V[1] * bc_screen.y + V[2] * bc_screen.z;
                vec3 r = R[0] * bc_screen.x + R[1] * bc_screen.y + R[2] * bc_screen.z;


                float ambient = ka * Ia;
                float diffuse = kd * Il * (n * l);
                float specular = ks * Il * pow(v * r, alpha);   //specular? alpha = 2 for now

                float Totalintensity = ambient + diffuse + specular;
                //float Totalintensity = ambient;
                //float Totalintensity = diffuse;
                //float Totalintensity = specular;
                //std::cout << Totalintensity << std::endl;
                clr = color * Totalintensity + color * Totalintensity + color * Totalintensity;
                clr /= 3;
            }
           
                putpixel(P.x, P.y, P.z, clr);

        }
    }
}
