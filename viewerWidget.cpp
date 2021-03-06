#include <iostream>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <cmath>

#include <GL/glut.h>
#include <GL/freeglut.h>
#include "viewerWidget.h"

using namespace std;

ViewerWidget::
ViewerWidget( QWidget * parent) : QGLWidget(parent), lcm( "udpm://239.255.76.67:7667?ttl=1" ),
_timer(),
_eye( 3, 5.0 ),
_counter( 0 ),
_socket_notifier( lcm.getFileno(), QSocketNotifier::Read ){
  connect( &_timer, SIGNAL( timeout() ),this, SLOT( timer_callback() ) );
  _timer.start( 16 );
  connect( &_socket_notifier, SIGNAL( activated( int ) ), this, SLOT( socket_notifier_callback() ) );
  setMinimumSize( 320, 240 );
}

void
ViewerWidget::
findMinMaxPair(vector<Point> pointVector){
  if( points[points.size() - 1].data.z > _minMaxPair.first.data.z ){
    _minMaxPair.first = points[points.size() - 1];
  }
  if( points[points.size() - 1].data.z < _minMaxPair.second.data.z ){
    _minMaxPair.second = points[points.size() - 1];
  }
}

double
ViewerWidget::
avgZ( void ){
  double avg_z = (_minMaxPair.first.data.z + _minMaxPair.second.data.z) /2;
  return avg_z;
}

void
ViewerWidget::
handleMessage(const lcm::ReceiveBuffer* rbuf,
  const std::string &chan,
  const xyzLdr::xyzLidar_t* msg)
{
  printf("Received message on channel \"%s\":\n", chan.c_str());
  printf("  timestamp   = %lld\n", (long long)msg->timestamp);
  printf("  x    = %.2f(m)\n",msg->x );
  printf("  y    = %.2f(m)\n",msg->y );
  printf("  z    = %.2f(m)\n",msg->z );
  printf("  quality     = %lld\n",(long long)msg->quality);
  if(points.size() == 0){
    cout << "empty" << endl;
    points.push_back(*(new Point(*msg,0.0,0.0,1.0)));
  }
  Point temp = Point(*msg, 1.0, 0.0, 0.0);
  Point newTemp = paint(temp);
  points.push_back(newTemp);
}

Point
ViewerWidget::
paint(Point temp){
  findMinMaxPair(points);
  double avgValue = avgZ();
  double deltaZ = temp.data.z - avgValue;
  if(deltaZ > 0){
    temp.r = deltaZ/avgValue;
    temp.g = (1 - deltaZ)/avgValue;
    temp.b = 0;
  }else{
    temp.r = 0;
    temp.g = (1 - abs(deltaZ))/avgValue;
    temp.b = abs(deltaZ/avgValue);
  }
  return temp;
}

void
ViewerWidget::
initializeGL(){
  glClearColor( 1.0, 1.0, 1.0, 1.0 );
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_LINE_SMOOTH );
  glEnable( GL_MULTISAMPLE );
  return;
}

void
ViewerWidget::
resizeGL( int width, int height ){
  glViewport( 0, 0, width, height );
  updateGL();
  return;
}

void
ViewerWidget::
paintGL(){
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  // setup window
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective( 45, ( double )( width() ) / ( double )( height() ), 0.01, 100.0 );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  gluLookAt( _eye[ 0 ], _eye[ 1 ], _eye[  2 ], 0.0, 0.0, 0.0, 0.0, 0.0, 1.0 );
  // do drawing
  glBegin(GL_LINES);
  glColor4f( 1.0, 0.0, 0.0, 1.0 );
  glVertex3f( 0.0, 0.0, 0.0 );
  glVertex3f( 1.0, 0.0, 0.0 );
  glColor4f( 0.0, 1.0, 0.0, 1.0 );
  glVertex3f( 0.0, 0.0, 0.0 );
  glVertex3f( 0.0, 1.0, 0.0 );
  glColor4f( 0.0, 0.0, 1.0, 1.0 );
  glVertex3f( 0.0, 0.0, 0.0 );
  glVertex3f( 0.0, 0.0, 1.0 );
  glEnd();

  //starts drawing of points
  glPointSize(2.0); //set point size in pizels
  glColor4f( 1.0, 0.0, 0.0, 1.0 ); //Set point color to red
  glBegin(GL_POINTS); //Start drawing vertecies as points

  //draw points stored in viewerWidget.points
  for (auto it = points.cbegin(); it != points.cend(); it++){
    cout << "it->r" << it->r << endl;
    glColor4f(it->r,it->g,it->b,1.0);
    glVertex3f(it->data.x,it->data.y,it->data.z);
  }
  glEnd();
  return;
}

void
ViewerWidget::
timer_callback(){
  updateGL(); //Necessary for live data visualization
  _eye[ 0 ] = zoom * cos( _counter * 2.0 * M_PI / 360.0 );
  _eye[ 1 ] = zoom * sin( _counter * 2.0 * M_PI / 360.0 );
  _eye[ 2 ] = zoom;
  if(orbit){
    _counter++;
  }
}

void
ViewerWidget::
socket_notifier_callback(){
  lcm.handle();
}

void
ViewerWidget::
keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Left){
      if(!orbit)
        _counter-=10;
    }else if(event->key() == Qt::Key_Right){
      if(!orbit)
        _counter+=10;
    }else if(event->key() == Qt::Key_Up){
      if(zoom>5)
        zoom-=5;
    }else if(event->key() == Qt::Key_Down){
      if(zoom<50)
        zoom+=5;
    }else if(event->key() == Qt::Key_O){
      orbit = !orbit;
    }else if(event->key() == Qt::Key_K){
      points.clear();
    }else if(event->key() == Qt::Key_B){
      cmnd::command_t command;
      command.state = 0x01;
      command.motor = true;
      command.speed = 4.2;
      command.scan = true;
      command.stop = false;
      lcm.publish("COMMAND_STREAM", &command);
    }else if(event->key() == Qt::Key_E){
      cmnd::command_t command;
      command.state = 0x02;
      command.motor = true;
      command.scan = true;
      command.stop = true;
      lcm.publish("COMMAND_STREAM", &command);
    }else if(event->key() == Qt::Key_R){
      cmnd::command_t command;
      command.state = 0x01;
      command.motor = true;
      command.speed = -4.2;
      command.scan = true;
      command.stop = false;
      lcm.publish("COMMAND_STREAM", &command);
    }else if(event->key() == Qt::Key_P){
      cmnd::command_t command;
      command.state = 0x02;
      command.motor = true;
      command.speed = 0.0;
      command.scan = true;
      command.stop = false;
    }
}

// void
// ViewerWidget::
// theButtonCallBack(){
//   cout << "Button pressed" <<endl;
// }

// void
// ViewerWidget::
// Font(void *font,char *text,int x,int y)
// {
// 	glRasterPos2i(x, y);

// 	while( *text != '\0' )
// 	{
// 		glutBitmapCharacter( font, *text );
// 		++text;
// 	}
// }

// void
// ViewerWidget::
// ButtonDraw(Button *b){
//   int fontx;
//   int fonty;

//   if(b){
//     if(b->highlighted){
//       glColor3f(0.7f,0.7f,0.8f);
//     }else{
//       glColor3f(0.6f,0.6f,0.6f);
//     }

//     glBegin(GL_QUADS); //draw button
// 			glVertex2i( b->x, b->y );
// 			glVertex2i( b->x, b->y+b->h );
// 			glVertex2i( b->x+b->w, b->y+b->h );
// 			glVertex2i( b->x+b->w, b->y );
// 		glEnd();

//     glLineWidth(3.0);

//       /*
// 		 *	Calculate the x and y coords for the text string in order to center it.
// 		 */
// 		fontx = b->x + (b->w /*- glutBitmapLength(GLUT_BITMAP_HELVETICA_10, b->label)*/) / 2 ;
// 		fonty = b->y + (b->h+10)/2;

// 		glColor3f(1,1,1);
// 		Font(GLUT_BITMAP_HELVETICA_10,b->label,fontx,fonty);
//   }
// }