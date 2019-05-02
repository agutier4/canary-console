#ifndef VIEWER_WIDGET_H
#define VIEWER_WIDGET_H

#include <QtGui/QKeyEvent>
#include <QtCore/QTimer>
#include <QtCore/QSocketNotifier>
#include <QtOpenGL/QGLWidget>
#include <lcm/lcm-cpp.hpp>
#include "xyzLdr/xyzLidar_t.hpp"
#include "cmnd/command_t.hpp"
#include <vector>

// struct Mouse{
//   int x;
//   int y;
//   int lmb;
//   int rmb;

//   int xpress; //x location of pressing
//   int ypress; //y location of pressing
// };

// typedef struct Mouse Mouse;

// typedef void(*ButtonCallback)();

// struct Button{
//   int   x;							/* top left x coord of the button */
// 	int   y;							/* top left y coord of the button */
// 	int   w;							/* the width of the button */
// 	int   h;							/* the height of the button */
// 	int	  state;						/* the state, 1 if pressed, 0 otherwise */
// 	int	  highlighted;					/* is the mouse cursor over the control? */
// 	char* label;						/* the text label of the button */
// };

// typedef struct Button Button;

//Used to store points to plot
struct Point{
  Point(){};
  Point(xyzLdr::xyzLidar_t _data, double _r = 1.0, double _g = 0.0, double _b = 0.0)
    :data(_data),r(_r), g(_g), b(_b){};
  xyzLdr::xyzLidar_t data;
  double r;
  double g;
  double b;
};

class ViewerWidget : public QGLWidget {
    Q_OBJECT
  public:
    ViewerWidget(QWidget * parent = NULL);
    virtual ~ViewerWidget(){};
    lcm::LCM lcm;
    void handleMessage(const lcm::ReceiveBuffer* rbuf,
  		const std::string &chan,
  		const xyzLdr::xyzLidar_t* msg);
    void theButtonCallBack();
    // Mouse testMouse = {0, 0, 0, 0};
    // Button velUp = {5,5, 100,25, 0,0, "Button"};

  protected slots:
	 void timer_callback(void);
         void socket_notifier_callback(void);

  protected:
    QTimer _timer;
    QSocketNotifier _socket_notifier;
    virtual void initializeGL();
    virtual void resizeGL( int width, int height );
    virtual void paintGL();
    void keyPressEvent(QKeyEvent *);
    std::vector<Point> points; //List of all current collected points
    std::vector< double > _eye;
    int _counter;
    bool orbit = false;
    double zoom = 5;

    //painting
    std::pair<Point, Point> _minMaxPair;
    void findMinMaxPair(std::vector<Point> pointVector);
    double avgZ( void );
    Point paint(Point temp);
    // //buttons
    // void Font( void *font, char *text, int x, int y );
    // // bool ButtonClickTest(Button* b,int x,int y);
    // void ButtonDraw(Button *b);
    // void Draw2D();
  };

#endif /* VIEWER_WIDGET_H */
