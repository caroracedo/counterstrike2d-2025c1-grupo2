#include "prelobby.h"

#include <QMessageBox>

#include "ui_prelobby.h"

PreLobby::PreLobby(QWidget* parent): QMainWindow(parent), ui(new Ui::PreLobby) {
    ui->setupUi(this);


    // Aceptar
    connect(ui->aceptar, &QPushButton::clicked, this, &PreLobby::saveDTO);
}

PreLobby::~PreLobby() { delete ui; }

void PreLobby::guardarCampos() {
    QString hostname = ui->hostname->text();
    host = hostname.toStdString();

    QString servname = ui->servname->text();
    port = servname.toStdString();
}

bool PreLobby::validarInfo() {
    if (host.empty() || port.empty()) {
        QMessageBox::warning(this, "Error", "Empty fields");
        return false;
    }
    return true;
}

void PreLobby::saveDTO() {
    guardarCampos();
    if (!validarInfo()) {
        return;
    }

    info = ActionDTO(ActionType::INIT, host, port);

    // ui->hostname->clear();
    // ui->servname->clear();

    QApplication::quit();
}

ActionDTO PreLobby::getInfo() { return info; }
