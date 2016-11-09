#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <GL/glut.h>

#define wWIDTH  260
#define wHEIGHT 260

static GLubyte Image[wWIDTH * wHEIGHT * 4];

static int Xpos, Ypos;
static int SkipPixels, SkipRows;
static int Scissor = 0;
static GLfloat Zpos = -1.0;
static float Xzoom, Yzoom;
static GLboolean DrawFront = GL_FALSE;
static GLboolean Dither = GL_TRUE;
static int win = 0;


static
void
Reset(void) {
    Xpos = Ypos = 0;
    SkipPixels = SkipRows = 0;
    Scissor = 0;
    Zpos = -1.0;
    Xzoom = Yzoom = 2.0;
}


static
GLboolean
ReadData() {
    printf("ReadData\n");

    /* size_t bytes = wWIDTH * wHEIGHT * 4; */
    /* if (fread(Image, 1, bytes, stdin) == bytes) { */
    /*     printf("here %d\n", Image[0]); */
    /*     return GL_TRUE; */
    /* } */
    /* else { */
    /*     perror("read"); */
    /*     return GL_FALSE; */
    /* } */

    for (int i = 0; i < wWIDTH * wHEIGHT * 4; i += 4) {
        Image[i + 0] = 0;
        Image[i + 1] = 0;
        Image[i + 2] = 0;
        Image[i + 3] = 255;
    }

    unsigned char previous = 0;
    /* for (int i = 0; i < wWIDTH * wHEIGHT; ++i) { */
    for (int i = 0; i < wWIDTH * wHEIGHT; i += 4) {
        unsigned char current = 0;
        if (fread(&current, 1, 1, stdin) == 1) {
            if (0 == i) {
                previous = current;
                continue;
            }
            int idx = wWIDTH * previous + current;
            Image[idx + 0] = 1 + Image[idx + 0];
            Image[idx + 1] = 1 + Image[idx + 1];
            Image[idx + 2] = 1 + Image[idx + 2];
            Image[idx + 3] = 255;
            previous = current;
        }
        else {
            perror("read");
            return GL_FALSE;
        }
    }

    return GL_TRUE;
}


static
void
Display(void) {
    printf("Display \n");
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

#if 0
    glRasterPos2i(Xpos, Ypos);
#else
    /* This allows negative raster positions: */
    glRasterPos3f(0, 0, Zpos);
    glBitmap(0, 0, 0, 0, Xpos, Ypos, NULL);
#endif

    glPixelStorei(GL_UNPACK_SKIP_PIXELS, SkipPixels);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, SkipRows);

    glPixelZoom(Xzoom, Yzoom);

    if (Scissor)
        glEnable(GL_SCISSOR_TEST);

    if (GL_TRUE != ReadData())
        printf("!read\n");
    glDrawPixels(wWIDTH, wHEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, Image);
    /* glDrawPixels(wWIDTH, wHEIGHT, GL_LUMINANCE, GL_UNSIGNED_BYTE, Image); */

    glDisable(GL_SCISSOR_TEST);

    if (DrawFront)
        glFinish();
    else
        glutSwapBuffers();
}


static
void
Reshape(int width, int height) {
    printf("Reshape \n");
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, 0.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glScissor(width/4, height/4, width/2, height/2);
}


static
void
Key(unsigned char key, int x, int y) {
    (void) x;
    (void) y;
    switch (key) {
    case ' ':
        Reset();
        break;
    case 'd':
        Dither = !Dither;
        if (Dither)
            glEnable(GL_DITHER);
        else
            glDisable(GL_DITHER);
        break;
    case 'p':
        if (SkipPixels > 0)
            SkipPixels--;
        break;
    case 'P':
        SkipPixels++;
        break;
    case 'r':
        if (SkipRows > 0)
            SkipRows--;
        break;
    case 'R':
        SkipRows++;
        break;
    case 's':
        Scissor = !Scissor;
        break;
    case 'x':
        Xzoom -= 0.1;
        printf("Xzoom %f\n", Xzoom);
        break;
    case 'X':
        Xzoom += 0.1;
        printf("Xzoom %f\n", Xzoom);
        break;
    case 'y':
        Yzoom -= 0.1;
        printf("Yzoom %f\n", Yzoom);
        break;
    case 'Y':
        Yzoom += 0.1;
        printf("Yzoom %f\n", Yzoom);
        break;
    case 'z':
        Zpos -= 0.1;
        printf("RasterPos Z = %g\n", Zpos);
        break;
    case 'Z':
        Zpos += 0.1;
        printf("RasterPos Z = %g\n", Zpos);
        break;
    case 'f':
        DrawFront = !DrawFront;
        if (DrawFront)
            glDrawBuffer(GL_FRONT);
        else
            glDrawBuffer(GL_BACK);
        printf("glDrawBuffer(%s)\n", DrawFront ? "GL_FRONT" : "GL_BACK");
        break;
    case 'q':
    case 'Q':
    case 27:
        glutDestroyWindow(win);
        exit(0);
        break;
    }
    glutPostRedisplay();
}


static
void
SpecialKey(int key, int x, int y) {
    (void) x;
    (void) y;
    switch (key) {
    case GLUT_KEY_UP:
        Ypos += 1;
        break;
    case GLUT_KEY_DOWN:
        Ypos -= 1;
        break;
    case GLUT_KEY_LEFT:
        Xpos -= 1;
        break;
    case GLUT_KEY_RIGHT:
        Xpos += 1;
         break;
    }
    glutPostRedisplay();
}


static
void
Init(GLboolean ciMode) {
   printf("GL_VERSION = %s\n", (char *) glGetString(GL_VERSION));
   printf("GL_RENDERER = %s\n", (char *) glGetString(GL_RENDERER));

#if 0
   if (ciMode) {
      /* Convert RGB image to grayscale */
      GLubyte *indexImage = (GLubyte *) malloc(wWIDTH * wHEIGHT);
      GLint i;
      for (i = 0; i < wWIDTH*wHEIGHT; i++) {
         int gray = Image[i*3] + Image[i*3+1] + Image[i*3+2];
         indexImage[i] = gray / 3;
      }
      Image = indexImage;

      for (i = 0; i < 255; i++) {
         float g = i / 255.0;
         glutSetColor(i, g, g, g);
      }
   }
#endif

   printf("%dx%d\n", wWIDTH, wHEIGHT);

   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glPixelStorei(GL_UNPACK_ROW_LENGTH, wWIDTH);

   Reset();
}


static
void
Usage(void) {
    printf("Keys:\n"
           "       SPACE  Reset Parameters\n"
           "     Up/Down  Move image up/down\n"
           "  Left/Right  Move image left/right\n"
           "           x  Decrease X-axis PixelZoom\n"
           "           X  Increase X-axis PixelZoom\n"
           "           y  Decrease Y-axis PixelZoom\n"
           "           Y  Increase Y-axis PixelZoom\n"
           "           p  Decrease GL_UNPACK_SKIP_PIXELS*\n"
           "           P  Increase GL_UNPACK_SKIP_PIXELS*\n"
           "           r  Decrease GL_UNPACK_SKIP_ROWS*\n"
           "           R  Increase GL_UNPACK_SKIP_ROWS*\n"
           "           s  Toggle GL_SCISSOR_TEST\n"
           "           z  Decrease RasterPos Z\n"
           "           Z  Increase RasterPos Z\n"

           "           f  Toggle front/back buffer drawing\n"
           "         ESC  Exit\n"
        );
}


int
main(int argc, char *argv[]) {
    GLboolean ciMode = GL_FALSE;
    int i = 1;

    glutInitWindowSize(2*wWIDTH, 2*wHEIGHT);
    glutInit(&argc, argv);

    if (argc > i && strcmp(argv[i], "-ci") == 0) {
        ciMode = GL_TRUE;
        i++;
    }

    if (ciMode)
        glutInitDisplayMode(GLUT_INDEX | GLUT_DOUBLE);
    else
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    win = glutCreateWindow(argv[0]);

    Init(ciMode);
    Usage();

    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Key);
    glutSpecialFunc(SpecialKey);
    glutDisplayFunc(Display);

    glutMainLoop();
    return 0;
}
