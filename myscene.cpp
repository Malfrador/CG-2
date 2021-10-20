#include "opengl.h"
#include "openglwidget.h"
#include "screenrenderer.h"
#include "scenemanager.h"
#include "transformation.h"
#include "keyboardtransformation.h"
#include "mousekeyboardcameracontroller.h"
#include "planet.h"
#include "color.h"

#include "ui_dockwidget.h"

Node *initScene1();

void SceneManager::initScenes()
{
    Ui_FPSWidget *lDock;
    QDockWidget *lDockWidget = new QDockWidget(QString("FPS"), SceneManager::getMainWindow());

    Camera* cam = new Camera();
    CameraController* camController = new MouseKeyboardCameraController(cam);
    Q_UNUSED(camController);
    RenderingContext *myContext=new RenderingContext(cam);
    unsigned int myContextNr = SceneManager::instance()->addContext(myContext);
    unsigned int myScene = SceneManager::instance()->addScene(initScene1());
    ScreenRenderer *myRenderer = new ScreenRenderer(myContextNr, myScene);

    //Vorsicht: Die Szene muss initialisiert sein, bevor das Fenster verändert wird (Fullscreen)
    SceneManager::instance()->setActiveScene(myScene);
    SceneManager::instance()->setActiveContext(myContextNr);
//    SceneManager::instance()->setFullScreen();

    lDock = new Ui_FPSWidget();
    lDock->setupUi(lDockWidget);
    lDockWidget->resize(200,100);
    SceneManager::getMainWindow()->addDockWidget(Qt::RightDockWidgetArea, lDockWidget);
    lDockWidget->show();
    QObject::connect(OpenGLWidget::getInstance(), SIGNAL(sigFPS(int)), lDock->lcdNumber, SLOT(display(int)));
}

Node *initScene1()
{
//    Objekte anlegen
    Planet *sonne = new Planet(1.0);
    KeyboardTransformation *sonnensystem = new KeyboardTransformation();
    KeyboardTransformation *earthRotation = new KeyboardTransformation();
    KeyboardTransformation *moonRotation = new KeyboardTransformation();
    Transformation *earthOrbit = new Transformation();
    Transformation *moonOrbit = new Transformation();
    Planet *earth = new Planet(0.2F);
    Planet *moon = new Planet(0.05F);
    Color *c;
    float umlaufspeed=2.0f;
    float moonSpeed = 10.0f;
    float earthSpeed = 100.0f;

    //Farben
    c = sonne->getProperty<Color>();
    c->setValue(1.0,1.0,0.0,1.0);
    c = earth->getProperty<Color>();
    c->setValue(0.0,0.0,1.0,1.0);
    c = moon->getProperty<Color>();
    c->setValue(0.5,0.5,0.5,1.0F);

    //Damit man die Drehungen sieht, Gitternetz aktivieren
    sonne->deactivateFill();
    earth->deactivateFill();
    moon->deactivateFill();

    //Keys belegen
    sonnensystem->setRotKeys(KeyboardTransformation::NoKey,
                             KeyboardTransformation::NoKey,
                             'r', 'R',
                             KeyboardTransformation::NoKey,
                             KeyboardTransformation::NoKey);
    sonnensystem->setRotspeed(umlaufspeed);

    earthRotation->setRotKeys(KeyboardTransformation::NoKey,
                              KeyboardTransformation::NoKey,
                              'r', 'R',
                              KeyboardTransformation::NoKey,
                              KeyboardTransformation::NoKey);
    earthRotation->setRotspeed(earthSpeed);
    moonRotation->setRotKeys(KeyboardTransformation::NoKey,
                             KeyboardTransformation::NoKey,
                             'r', 'R',
                             KeyboardTransformation::NoKey,
                             KeyboardTransformation::NoKey);
    moonRotation->setRotspeed(moonSpeed);

    //Vorsicht beim ändern von Rot. und Trans. derselben Transformation:
    //Die Reihenfolge ist wichtig!
    sonnensystem->rotate(45.0,1.0,0.0,0.0); //Neigen, damit man die Umlaufbahn besser sieht
    earthOrbit->translate(4.0, 0.0, 0.0);
    moonOrbit->translate(0.5, 0.0, 0.0);

    //Szenengraph aufbauen
    Node *sonnensystemNode = new Node(sonnensystem);
    Node *earthOrbitNode = new Node(earthOrbit);
    Node *earthRotationNode = new Node(earthRotation);
    Node *moonOrbitNode = new Node(moonOrbit);
    Node *moonRotationNode = new Node(moonRotation);

    earthOrbitNode->addChild(moonOrbitNode);


    sonnensystemNode->addChild(new Node(sonne));

    sonnensystemNode->addChild(earthOrbitNode);

    return(sonnensystemNode);
}
