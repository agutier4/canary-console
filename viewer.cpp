#include <QtGui/QApplication>
#include "viewerWidget.h"

int
main( int argc, char* argv[] ){
	QApplication app( argc, argv );
	ViewerWidget viewerWidget;

	if(!viewerWidget.lcm.good())
		return 1;

	lcm::Subscription* s = viewerWidget.lcm.subscribe("LIDAR_STREAM", &ViewerWidget::handleMessage, &viewerWidget);
	s->setQueueCapacity(1500);
	viewerWidget.show();

  return app.exec();
}
