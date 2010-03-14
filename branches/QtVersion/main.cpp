#include "stable.h"
#include "azApplication.h"
#include "utils/azIconData.h"
#include "utils/azIconUtil.h"


int main(int argc, char *argv[]) {
  azApplication app(argc, argv);
  app.setOrganizationName("Trolltech");
  app.setApplicationName("Application Example");

  QGraphicsScene scene;

  scene.addText("Hello, world!");

  azIconData iconData = azIconUtil::getFolderItemIcon("C:\\Program Files\\Adobe\\Adobe Photoshop CS4\\Photoshop.exe", 256);
  scene.addPixmap(iconData.pixmap);

  iconData = azIconUtil::getFolderItemIcon("C:\\Program Files\\Adobe\\Adobe Illustrator CS4\\Support Files\\Contents\\Windows\\AI_Application_Icon.ico", 48);
  scene.addPixmap(iconData.pixmap);

  QGraphicsView view(&scene); 
  view.show();

  return app.exec();
}