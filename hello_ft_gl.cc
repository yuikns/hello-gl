#include <math.h>
#include <stdio.h>
#include <string.h>

#include <algorithm>
#include <string>
#include <vector>

#include "argcv/cxx/par/thread_pool.h"
#include "argcv/cxx/str/str.h"

#include <ft2build.h>
#include FT_FREETYPE_H

// Start.. GLFW
#ifdef __APPLE__
#include <GLFW/glfw3.h>
//#elif defined(_WIN32) || defined(_WIN64)
//#include <GLFW/glfw3.h> //I DON'T KNOW... NOT TESTED
#else  // for linux : yum install glfw* :)
#include <GLFW/glfw3.h>
#endif
// End... GLFW

class FontPoints {
 public:
  explicit FontPoints(size_t height, size_t width)
      : height_(height),
        width_(width),
        image(height, std::vector<unsigned char>(width, (unsigned char)0)) {}

  virtual ~FontPoints() {}

  void DrawBitmap(FT_Bitmap* bitmap, FT_Int x, FT_Int y) {
    FT_Int i, j, p, q;
    FT_Int x_max = x + bitmap->width;
    FT_Int y_max = y + bitmap->rows;

    for (i = x, p = 0; i < x_max; i++, p++) {
      for (j = y, q = 0; j < y_max; j++, q++) {
        if (i < 0 || j < 0 || i >= width_ || j >= height_) continue;
        image[j][i] |= bitmap->buffer[q * bitmap->width + p];
      }
    }
  }

  void Draw(FT_Face& face, FT_Matrix& matrix, const std::string& text) {
    FT_GlyphSlot slot = face->glyph;
    FT_Error error;
    /* the pen position in 26.6 cartesian space coordinates; */
    /* start at (300,200) relative to the upper left corner  */
    FT_Vector pen; /* untransformed origin  */
    // pen.x = 300 * 64;
    // pen.y = (height_ - 200) * 64;
    pen.x = 10 * 64;
    pen.y = 15 * 64;

    // printf("Utf8StringDecode starting..");
    // fflush(nullptr);
    std::vector<uint64_t> blocks = argcv::Utf8ToUnicode(text);
    // printf("n-bloocks: %d", blocks.size());
    for (const FT_ULong& block : blocks) {
      FT_Set_Transform(face, &matrix, &pen);

      /* load glyph image into the slot (erase previous one) */
      // error = FT_Load_Char(face, text[n], FT_LOAD_RENDER);
      error = FT_Load_Char(face, block, FT_LOAD_RENDER);

      if (error) continue; /* ignore errors */

      /* now, draw to our target surface (convert position) */
      DrawBitmap(&slot->bitmap, slot->bitmap_left, height_ - slot->bitmap_top);

      /* increment pen position */
      pen.x += slot->advance.x;
      pen.y += slot->advance.y;
    }
  }

  float At(int x, int y) {
    if (OutOfRange(x, y)) {
      return 0.f;
    }
    return image[x][y];
  }

  bool OutOfRange(int x, int y) {
    return x < 0 || x >= height_ || y < 0 || y >= height_;
  }

  float Norm(int x, int y, int range) {
    float norm = 0.f;
    int cnt = 0;
    int xmin = x - range;
    if (xmin < 0) xmin = 0;
    int xmax = x + range;
    if (xmax >= height_) xmax = height_ - 1;
    int ymin = y - range;
    if (ymin < 0) ymin = 0;
    int ymax = y + range;
    if (ymax >= width_) ymax = width_ - 1;
    for (int ix = xmin; ix <= xmax; ix++) {
      for (int iy = ymin; iy <= ymax; iy++) {
        if (abs(ix - x) + abs(iy - y) > (range + 1)) {
          continue;
        }
        norm += image[ix][iy];
        cnt++;
      }
    }
    return cnt == 0 ? 0.f : norm / cnt;
  }

 private:
  size_t height_;
  size_t width_;
  // TODO(yu): to mat2x2
  // mat2x2
  // image[height][width]
  std::vector<std::vector<unsigned char>> image;
};

struct OnePoint {
  float color;
  float ch;
  float cw;
};

struct PointPack {
  std::vector<OnePoint> points;
};

void StartShow(FT_Face& face, FT_Matrix& matrix, const std::string& text) {
  printf("Starting GL Window...\n");
  printf("Compiled against GLFW %i.%i.%i\n", GLFW_VERSION_MAJOR,
         GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
  int major, minor, revision;
  glfwGetVersion(&major, &minor, &revision);
  printf("Running against GLFW %i.%i.%i\n", major, minor, revision);

  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit()) {
    fprintf(stderr, "GLFW init failed\n");
    return;
  }

  // int fixed_width = 1908;
  // int fixed_height = 270;

  int fixed_width = 476 * 2;
  int fixed_height = 116 * 2;

  /* Create a windowed mode window and its OpenGL context */
  // window = glfwCreateWindow(640, 480, "Hello, OpenGL!", NULL, NULL);
  window = glfwCreateWindow(fixed_width, fixed_height, "Hello, OpenGL!",
                            nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return;
  }

  // Make the window's context current
  // http://www.glfw.org/docs/latest/group__context.html#ga1c04dc242268f827290fe40aa1c91157
  glfwMakeContextCurrent(window);

  // http://www.glfw.org/docs/latest/group__context.html#ga6d4e0cdf151b5e579bd67f13202994ed
  glfwSwapInterval(1);

  int csec = 0;
  int scnt = 0;

  int ow = 0;
  int oh = 0;

  float point_size = 1.f;

  argcv::ThreadPool pool(2, 8);

  // Loop until the user closes the window
  // http://www.glfw.org/docs/latest/group__window.html#ga24e02fbfefbb81fc45320989f8140ab5
  while (!glfwWindowShouldClose(window)) {
    int rsec = (int)glfwGetTime();
    scnt++;
    if (rsec - csec > 3) {
      float rc = (float)scnt / (rsec - csec);
      fprintf(stdout, "FPS: %.2f (%d/%d)\n", rc, scnt, (rsec - csec));
      fflush(stdout);
      csec = rsec;
      scnt = 0;
    }

    /* Render here */
    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // check width and height
    if (ow != width || oh != height) {
      printf("width or height was changed: %d, %d => %d, %d\n", ow, oh, width,
             height);
      ow = width;
      oh = height;

      float curr_point_size = 1.f;

      if (fixed_width > 0 && ((float)width / fixed_width) > curr_point_size) {
        curr_point_size = (float)width / fixed_width;
      }

      if (fixed_height > 0 && (height / fixed_height) > curr_point_size) {
        curr_point_size = (float)height / fixed_height;
      }

      curr_point_size *= 1.f;

      if (curr_point_size != point_size) {
        printf("update point size: %.3f => %.3f\n", point_size,
               curr_point_size);
        point_size = curr_point_size;
      }

    } else {
      // printf("width or height was NOT changed: %d, %d => %d, %d\n", ow, oh,
      // width, height);
    }

    // fixed_width = width;
    // fixed_height = height;

    ratio = width / (float)height;
    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT);
    // glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    // glMatrixMode ref:
    // https://www.opengl.org/sdk/docs/man2/xhtml/glMatrixMode.xml
    glMatrixMode(GL_PROJECTION);  // Applies subsequent matrix operations to the
                                  // projection matrix stack.
    glLoadIdentity();
    // glOrtho
    // void glOrtho(GLdouble left,
    //     GLdouble right,
    //     GLdouble bottom,
    //     GLdouble top,
    //     GLdouble nearVal,
    //     GLdouble farVal);
    // glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glOrtho(0.f, width * 1.f, 0.f, height * 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);  // Applies subsequent matrix operations to the
                                 // modelview matrix stack.
    glLoadIdentity();
    // glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
    glRotatef(0.f, 0.f, 0.f, 1.f);

    // glBegin(GL_TRIANGLES);
    // glColor3f(0.1f, 0.f, 0.f); // x, y, z
    // glVertex3f(-1.f * ratio, -0.99f, 0.f);
    // glColor3f(0.f, 0.1f, 0.f);
    // glVertex3f(1.f * ratio, -0.99f, 0.f);
    // glColor3f(0.f, 0.f, 0.1f);
    // glVertex3f(0.f, 1.f, 0.f);
    // glEnd();

    // int fixed_width = 1908;
    // int fixed_height = 270;
    FontPoints fp(fixed_height, fixed_width);

    fp.Draw(face, matrix, text);

    // glVertex3f(-0.6f, -0.4f, 0.f);
    glPointSize(point_size);
    // float accu = 1.0f / height;
    glBegin(GL_POINTS);
    std::vector<std::future<PointPack>> points;
    // printf("start....\n");
    for (int i = 0; i < height; i++) {
      points.emplace_back(pool.Enqueue(
          [i, &fixed_height, &fixed_width, &width, &height, &fp] {
            PointPack p;

            for (int j = 0; j < width; j++) {

              // float px = fp.At(i,j);
              float px = fp.Norm((float)i / height * fixed_height,
                                 (float)j / width * fixed_width, 4);
              if (px > 0) {
                OnePoint p_curr;
                // printf("h: %d, w: %d... v: %.3f\n", i, j, px);
                float pxc = px / 256;
                // glColor3f(pxc, pxc, pxc);
                // float ch = -(float)i * hd / height + (hd / 2);
                //           float cw = (float)j * wd / width - (wd / 2);
                float ch = height - (float)i;
                float cw = (float)j;
                // glVertex3f(cw, ch, 0.f);

                p_curr.color = pxc;
                p_curr.ch = ch;
                p_curr.cw = cw;
                // printf("get... %d %d =? %f\n", i, j, pxc);
                p.points.push_back(p_curr);
              }
            }
            return p;
          }));
    }
    for (auto&& p : points) {
      PointPack pp = p.get();
      for(auto &&p_curr : pp.points) {
        float color = p_curr.color;
        glColor3f(color, color, color);
        glVertex3f(p_curr.cw, p_curr.ch, 0.f);
      }
    }
    // printf("end....\n");
    glEnd();

    // // glVertex3f(-0.6f, -0.4f, 0.f);
    // glPointSize(1.5f);
    // // float accu = 1.0f / height;
    // glBegin(GL_POINTS);
    // // for (float i = -0.6f; i < 0.6f; i += accu) {
    // //   glColor3f(1.f, 0.f, 0.f);
    // //   glVertex3f(i, -0.4f, 0.f);
    // // }
    // float wd = 2.f * ratio;
    // float hd = 2.f;
    // for (int i = 0; i < height; i++) {
    //   for (int j = 0; j < width; j++) {
    //     // float px = fp.At(i,j);
    //     float px = fp.At((float)i / height * fixed_height,
    //                      (float)j / width * fixed_width);
    //     if (px > 0) {
    //       // printf("h: %d, w: %d... v: %.3f\n", i, j, px);
    //       float pxc = px / 256;
    //       glColor3f(pxc, pxc, pxc);
    //       float ch = -(float)i * hd / height + (hd / 2);
    //       float cw = (float)j * wd / width - (wd / 2);
    //       glVertex3f(cw, ch, 0.f);
    //       // printf("h: %.3f, w: %.3f, v: %.3f\n", ch, cw, pxc);
    //     }
    //   }
    // }
    // glEnd();

    // Swap front and back buffers
    // http://www.glfw.org/docs/latest/group__window.html#ga15a5a1ee5b3c2ca6b15ca209a12efd14
    glfwSwapBuffers(window);

    // Poll for and process events
    // http://www.glfw.org/docs/latest/group__window.html#ga37bd57223967b4211d60ca1a0bf3c832
    glfwPollEvents();
  }
  printf("jump out of the loop\n");

  // http://www.glfw.org/docs/latest/group__window.html#gacdf43e51376051d2c091662e9fe3d7b2
  glfwDestroyWindow(window);

  // http://www.glfw.org/docs/latest/group__init.html#gaaae48c0a18607ea4a4ba951d939f0901
  glfwTerminate();
}

// bazel run :hello_ft_gl  $(pwd)/fonts/FantasqueSansMono-Regular.ttf 'Hello,
// World!'
int main(int argc, char** argv) {
  FT_Library library;
  FT_Face face;

  // FT_GlyphSlot slot;
  FT_Matrix matrix; /* transformation matrix */
  FT_Error error;

  char* filename;
  char* text;

  double angle;

  // int n, num_chars;

  if (argc < 3) {
    fprintf(stderr, "usage: %s font sample-text\n", argv[0]);
    fflush(nullptr);
    return -1;
  }

  filename = argv[1]; /* first argument     */
  text = argv[2];     /* second argument    */
  // num_chars = strlen(text);
  // angle = (25.0 / 360) * 3.14159 * 2; /* use 25 degrees     */
  // angle = (5.0 / 360) * 3.14159 * 2; /* use 25 degrees     */
  angle = (0 / 360) * 3.14159 * 2; /* use 0 degrees     */

  error = FT_Init_FreeType(&library); /* initialize library */
  /* error handling omitted */

  error = FT_New_Face(library, filename, 0, &face); /* create face object */
  /* error handling omitted */

  /* use 50pt at 100dpi */
  // error = FT_Set_Char_Size(face, 50 * 64, 0, 100, 0); /* set character size
  // */
  // error = FT_Set_Char_Size(face, 8 * 64, 0, 150, 0); /* set character size */
  // FT_Set_Char_Size( FT_Face     face,
  //                   FT_F26Dot6  char_width,
  //                   FT_F26Dot6  char_height,
  //                   FT_UInt     horz_resolution,
  //                   FT_UInt     vert_resolution );

  error = FT_Set_Char_Size(face, 16 * 64, 0, 600, 0);  // set character size
  // error handling omitted

  // slot = face->glyph;

  // set up matrix
  matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
  matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
  matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
  matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);

  StartShow(face, matrix, text);

  FT_Done_Face(face);
  FT_Done_FreeType(library);

  return 0;
}
