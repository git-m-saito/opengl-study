#include <stdio.h>
#include <stdbool.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <unistd.h>
#include <math.h>

int display_num = 0;
float vertexZPositionBase = 0.0f;
float vertexZPositionRangeMin = -0.25f;
float vertexZPositionRangeMax = 0.25f;

float rotationH = 0.0f;//水平方向の回転角
float moveSpeed = 0.01f;
int renderIntervalMilliSec = 20;

bool isGoBack = true;

// 描画処理
void display(void)
{
    // カラーバッファを初期化する
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    // 立方体定義
    GLdouble    vertex[][3] = {
         {-0.25, -0.25, -0.25}                //A 0
        ,{0.25, -0.25, -0.25}                //B 1
        ,{0.25, 0.25, -0.25}                //C 2
        ,{-0.25, 0.25, -0.25}                //D 3
        ,{-0.25, -0.25, 0.25}                //E 4
        ,{0.25, -0.25, 0.25}                //F 5
        ,{0.25, 0.25, 0.25}                //G 6
        ,{-0.25, 0.25, 0.25}                //H 7
    };

    int face[][4] = {
         {0, 1, 2, 3}        //A-B-C-D    奥側    赤
        ,{1, 5, 6, 2}        //B-F-G-C    右側    緑
        ,{5, 4, 7, 6}        //E-F-H-G    手前    青
        ,{4, 0, 3, 7}        //E-A-D-H    左側    黄
        ,{4, 5, 1, 0}        //E-F-B-A    下側    水色
        ,{3, 2, 6, 7}        //D-C-G-H    上側    白
    };

    GLdouble color[][3] = {
         {1, 0, 0}            //奥側    赤
        ,{0, 1, 0}            //右側    緑
        ,{0, 0, 1}            //手前    青
        ,{1, 1, 0}            //左側    黄
        ,{0, 1, 1}            //下側    水色
        ,{0, 0, 0}            //上側    白
    };
    
    // z軸水平移動
    glTranslated(0, 0, vertexZPositionBase);
    // y軸回転
    glRotatef(rotationH, 0.0, 1.0, 0.0);
    
    glBegin(GL_QUADS);
    for (int i = 0; i < 6; i++) {
        glColor3dv(color[i]);
        
        for (int j = 0; j < 4; j++) {
            glVertex3dv    (vertex[face[i][j]]);
        }
    }
    glEnd();
    
    // 描画を行う
    glFlush();
    // 打ち消す方向にy軸回転
    glRotatef(-rotationH, 0.0, 1.0, 0.0);

    
    //glRotateを使わない回転行列
    //double m[16];
    // y軸回転行列
//    m[0] = cos(rotationH);
//    m[1] = 0;
//    m[2] = -sin(rotationH);
//    m[3] = 0;
//
//    m[4] = 0;
//    m[5] = 1;
//    m[6] = 0;
//    m[7] = 0;
//
//    m[8] = sin(rotationH);
//    m[9] = 0;
//    m[10] = cos(rotationH);
//    m[11] = vertexZPositionBase;
//
//    m[12] = 0;
//    m[13] = 0;
//    m[14] = 0;
//    m[15] = 1;
//
//    glMultMatrixd(m);
    
//    double n[16];
//    // z軸移動
//    n[0] = 1;
//    n[1] = 0;
//    n[2] = 0;
//    n[3] = 0;
//
//    n[4] = 0;
//    n[5] = 1;
//    n[6] = 0;
//    n[7] = 0;
//
//    n[8] = 0;
//    n[9] = 0;
//    n[10] = 1;
//    n[11] = vertexZPositionBase;
//
//    n[12] = 0;
//    n[13] = 0;
//    n[14] = 0;
//    n[15] = 1;
//
//    glMultMatrixd(n);
    
    
}

// ウィンドウのサイズ変更時に呼ばれる
void reshape(int width, int height) {

    // ① ビューポートの設定
    glViewport(0, 0, width, height);

    // ② 視野領域の決定
    glMatrixMode(GL_PROJECTION);                    // 射影行列を操作する
    glLoadIdentity();                        // 行列を初期化
    gluPerspective(60.0, (double)width / height, 1.0, 100.0);// 垂直方向の画角, アスペクト比, 視体積の手前のz値, 視体積の奥側のz値
    
    // ③ 視点位置の決定
    glMatrixMode(GL_MODELVIEW);                    // モデルビュー行列の設定
    glLoadIdentity();                        // 行列を初期化
    gluLookAt(0.0, 0.0, -5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);// 視点位置、注視点、姿勢
    
}

void initialize(void)
{
    // 画面を白にする
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

// 20ミリ秒ごとに実行される関数
void timer(int value)
{
    // 前後に移動するようにする
    if(isGoBack)
    {
        if(vertexZPositionBase <= vertexZPositionRangeMax)
        {
            vertexZPositionBase += moveSpeed;
        }
        else
        {
            // 端っこに来たら反転
            isGoBack =! isGoBack;
        }
    }
    else
    {
        if(vertexZPositionBase >= vertexZPositionRangeMin)
        {
            vertexZPositionBase -= moveSpeed;
        }
        else
        {
            // 端っこに来たら反転
            isGoBack =! isGoBack;
        }
    }

    rotationH += 1;
    // 画面を再描写
    glutPostRedisplay();

    // 20ミリ秒後に再実行
    glutTimerFunc(renderIntervalMilliSec, timer, 0);
}



int main(int argc, char** argv)
{
    // GLUT初期化
    glutInit(&argc, argv);
    // 画面サイズ
    glutInitWindowSize(800, 600);
    // 画面の初期位置を指定
    glutInitWindowPosition(0, 0);
    // 名前
    glutCreateWindow("Cube");
    
    // 表示モード設定
    glutInitDisplayMode(GLUT_RGBA);
    // 画面の初期化(白で染める)
    initialize();
    
    // 画面を再描写するときに実行される関数を指定 (初期化、ウィンドウサイズ変更時など)
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    // 20ミリ秒後に timer() を実行
    glutTimerFunc(renderIntervalMilliSec, timer, 0);
    
    // 毎フレームのLoop
    glutMainLoop();
    return 0;
}
