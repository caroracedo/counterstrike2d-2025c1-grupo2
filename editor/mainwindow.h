#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPushButton>

#include <yaml-cpp/yaml.h>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum class TIPO_ZONA { NINGUNA, TERRORISTA, COUNTERTERRORISTA, BOMBA };

enum class TIPO_ELEMENTO { NINGUNO, CAJA, ARMA };

enum class TIPO_CAJA { CAJA1, CAJA2, CAJA3, CAJA4, CAJA5, NINGUNO };

enum class TIPO_ARMA { M3, AWP, AK47, NINGUNO };

struct Celda {
    TIPO_ZONA zona = TIPO_ZONA::NINGUNA;
    TIPO_ELEMENTO tipoElemento = TIPO_ELEMENTO::NINGUNO;
    TIPO_CAJA tipoCaja = TIPO_CAJA::NINGUNO;
    TIPO_ARMA tipoArma = TIPO_ARMA::NINGUNO;
    QString imagenElemento;
};

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent*) override;

private:
    Ui::MainWindow* ui;
    QGraphicsScene* scene;
    QString imagenSeleccionada;
    QString terreno;
    int cantZonasBomba;
    Celda grilla[21][21];

    void dibujarGrilla();
    void inicializarGrilla();
    void cargarImagenTerreno();
    void rellenarGrillaConTerreno(int index);
    bool verificarZonaBomba();
    bool agregarZonaBomba();

    void guardarCajas(YAML::Emitter& out);
    void guardarArmas(YAML::Emitter& out);
    void guardarZonasBomba(YAML::Emitter& out);
    void guardarMapaComoYaml(const QString&);
    void abrirMapaDesdeYaml(const QString&);

    void conectarBtnClicked(QPushButton* btn, QString imagen);
    void conectarBtnPressed(QPushButton* btn, QString imagen);
};
#endif  // MAINWINDOW_H
