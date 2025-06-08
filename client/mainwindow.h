#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

#include "../common/action_DTO.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct InfoAux {
    ActionType type = ActionType::UNKNOWN;
    std::string match = "";
    PlayerType player_type = PlayerType::UNKNOWN;
};

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    ActionDTO getInfo();
    void guardarOperacion(int index);
    void guardarEquipo(int index);
    void guardarNombrePartida();
    bool validarInfo();
    void saveDTO();

private:
    Ui::MainWindow* ui;
    InfoAux info_aux;
    ActionDTO info;
};

#endif  // MAINWINDOW_H
