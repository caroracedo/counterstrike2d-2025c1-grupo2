#include "mainwindow.h"

#include <QComboBox>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QString>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Crear Unirse partida
    ui->operacion->addItem("Select operation...");

    QStandardItemModel* model1 = qobject_cast<QStandardItemModel*>(ui->operacion->model());
    if (model1) {
        QStandardItem* item = model1->item(0);
        if (item) {
            item->setEnabled(false);
        }
    }

    ui->operacion->addItem("Create match", QVariant("crear"));
    ui->operacion->addItem("Join match", QVariant("unirse"));

    connect(ui->operacion, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::guardarOperacion);

    // Ingresar nombre partida
    connect(ui->saveBtn, &QPushButton::clicked, this, &MainWindow::guardarNombrePartida);

    // Elegir equipo
    ui->equipo->addItem("Select team...");

    QStandardItemModel* model2 = qobject_cast<QStandardItemModel*>(ui->equipo->model());
    if (model2) {
        QStandardItem* item = model2->item(0);
        if (item) {
            item->setEnabled(false);
        }
    }

    ui->equipo->addItem("Terrorist", QVariant("terrorist"));
    ui->equipo->addItem("Counterterrorist", QVariant("counterTerrorist"));

    connect(ui->equipo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::guardarEquipo);

    // Aceptar
    connect(ui->aceptar, &QPushButton::clicked, this, &MainWindow::saveDTO);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::guardarOperacion(int index) {
    QString action = ui->operacion->itemData(index).toString();

    if (action == "crear") {
        info_aux.type = ActionType::CREATE;
    } else {
        info_aux.type = ActionType::JOIN;
    }
}

void MainWindow::guardarNombrePartida() {
    QString name = ui->matchName->text();

    info_aux.match = name.toStdString();
}

void MainWindow::guardarEquipo(int index) {
    QString equipo = ui->equipo->itemData(index).toString();

    if (equipo == "terrorist") {
        info_aux.player_type = PlayerType::TERRORIST;
    } else {
        info_aux.player_type = PlayerType::COUNTERTERRORIST;
    }
}

bool MainWindow::validarInfo() {
    if (info_aux.type == ActionType::UNKNOWN) {
        QMessageBox::warning(this, "Error", "Select an option");
        return false;
    }
    if (info_aux.match.empty()) {
        QMessageBox::warning(this, "Error", "Enter a match name");
        return false;
    }
    if (info_aux.player_type == PlayerType::UNKNOWN) {
        QMessageBox::warning(this, "Error", "Select an option");
        return false;
    }
    return true;
}

void MainWindow::saveDTO() {
    if (!validarInfo())
        return;
    info = ActionDTO(info_aux.type, info_aux.match, info_aux.player_type);
    QApplication::quit();
}

ActionDTO MainWindow::getInfo() { return info; }
