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

//Used to store points to plot
struct Point{
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

  };

#endif /* VIEWER_WIDGET_H */
