#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <vector>

#include "common/action_DTO.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct InfoAux {
    ActionType type = ActionType::UNKNOWN;
    std::string mapSelected = "";
    std::string matchName = "";
    PlayerType player_type = PlayerType::UNKNOWN;
};

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(const std::vector<std::string>& mapasIngresados,
               const std::vector<std::string>& partidasIngresadas, QWidget* parent = nullptr);
    ~MainWindow();
    ActionDTO getInfo();
    void guardarOperacion(int index);
    void guardarEquipo(int index);
    void guardarMapa(int index);
    void guardarPartidaElegida(int index);
    void guardarNombrePartida();
    bool validarInfo();
    void saveDTO();

private:
    Ui::MainWindow* ui;
    InfoAux info_aux;
    ActionDTO info;
    const std::vector<std::string>& mapas;
    const std::vector<std::string>& partidas;
};
#endif  // MAINWINDOW_H
