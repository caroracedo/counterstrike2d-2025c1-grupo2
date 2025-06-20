#ifndef PRELOBBY_H
#define PRELOBBY_H

#include <QMainWindow>
#include <string>

#include "common/action_DTO.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class PreLobby;
}
QT_END_NAMESPACE

class PreLobby: public QMainWindow {
    Q_OBJECT

public:
    explicit PreLobby(QWidget* parent = nullptr);
    void saveDTO();
    void guardarCampos();
    bool validarInfo();
    ActionDTO getInfo();
    ~PreLobby();

private:
    Ui::PreLobby* ui;
    std::string host;
    std::string port;
    ActionDTO info;
};
#endif  // PRELOBBY_H
