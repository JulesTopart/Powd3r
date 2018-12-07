#ifndef RENDERER_H
#define RENDERER_H

#include "vtk/vtkDistanceRepresentation.h"

#include "vtk/QVTKWidget.h"
#include "vtk/vtkActor.h"
#include "vtk/vtkAxesActor.h"
#include "vtk/vtkLegendScaleActor.h"
#include "vtk/vtkOrientationMarkerWidget.h"
#include "vtk/vtkCubeSource.h"

#include "vtk/vtkSTLReader.h"
#include "vtk/vtkOBJReader.h"
#include "vtk/vtkSTLWriter.h"

#include "vtk/vtkRenderer.h"
#include "vtk/vtkRenderWindow.h"
#include "vtk/vtkInteractorStyleSwitch.h"

#include "vtk/vtkPolyDataMapper.h"
#include "vtk/vtkPolyData.h"
#include "vtk/vtkAppendPolyData.h"
#include "vtk/vtkPolyDataConnectivityFilter.h"
#include "vtk/vtkCleanPolyData.h"

#include "vtk/vtkProperty.h"
#include "vtk/vtkCamera.h"


#include <QDebug>
#include <QList>
#include <QResizeEvent>
#include <QWidget>
#include <QIcon>
#include <QMessageBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>
#include <QGroupBox>
#include <QRadioButton>
#include <QTextEdit>
#include <QLabel>
#include <QCoreApplication>
#include <QTranslator>

#include <QTabBar>
#include <QMainWindow>
#include <QMdiArea>
#include <QDockWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QMdiSubWindow>
#include <QToolBar>
#include <QColorDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QTreeView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QDesktopServices>



class fenetre3D : public QWidget
{
    Q_OBJECT

public:
    fenetre3D(QWidget *parent=0);
    ~fenetre3D();
    template<typename nouveauvtkreader>  void afficher_acteur(nouveauvtkreader *reader,QString nom_fichier);
    void choix_lecteur_acteur(QString nom_fichier,QString nom_extension);
    QList<unsigned char> fragmenter(int indexselection);
    void fusionner(QList<unsigned char> indices_pieces);
    void cacheracteur(int i);
    void cacherelementscene3D(int i);
    void montrerelementscene3D(int i);
    void montreracteur(int i);
    void supprimeracteur(int i);
    void bilan(int i,unsigned char& N_mesh, unsigned char& N_points, unsigned char& N_faces);
    void exporteracteur(int i, QString nom, QString typestl);

public slots:
    void setcolorobjet(QColor color);
    void setcolorscene(QColor color);
    void setrotationcamera();
    void setrotationacetur();
    void setActiveActor(int i);
    void setzoneimpression(double l1, double l2, double h);
    void test();

protected:

    void resizeEvent(QResizeEvent *event);

    vtkAxesActor *m_repere;
    vtkActor *m_actor_active;
    vtkOrientationMarkerWidget *m_widgetorientation;
    vtkLegendScaleActor *m_legendScaleActor;
    vtkActor *m_zoneimpression;

    vtkInteractorStyleSwitch *m_style;
    vtkRenderer *m_renderer;
    vtkRenderWindow *m_renderWindow;
    QVTKWidget *m_widget;

    QList<vtkActor*> m_listeactor;

};





#endif // RENDERER_H
