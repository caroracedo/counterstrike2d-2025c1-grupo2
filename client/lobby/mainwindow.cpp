#include "mainwindow.h"

#include <QComboBox>
#include <QDebug>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QString>

#include "ui_mainwindow.h"

MainWindow::MainWindow(const std::vector<std::string>& mapasIngresados,
                       const std::vector<std::string>& partidasIngresadas, QWidget* parent):
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        mapas(mapasIngresados),
        partidas(partidasIngresadas) {
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

    ui->create->hide();
    ui->join->hide();
    ui->skin->hide();

    connect(ui->operacion, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::guardarOperacion);

    // Ingresar nombre partida
    // connect(ui->saveBtn, &QPushButton::clicked, this, &MainWindow::guardarNombrePartida);

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
        ui->maps->clear();
        info_aux.type = ActionType::CREATE;
        ui->create->show();
        ui->join->hide();
        ui->maps->addItem("Select map...");
        for (const std::string& mapa: mapas) {
            QString m = QString::fromStdString(mapa);
            ui->maps->addItem(m, QVariant(m));
        }
        QStandardItemModel* modelMap = qobject_cast<QStandardItemModel*>(ui->maps->model());
        if (modelMap) {
            QStandardItem* item = modelMap->item(0);
            if (item) {
                item->setEnabled(false);
            }
        }
    } else {
        ui->partidas->clear();
        info_aux.type = ActionType::JOIN;
        ui->create->hide();
        ui->join->show();
        ui->partidas->addItem("Select match...");
        for (const std::string& partida: partidas) {
            QString p = QString::fromStdString(partida);
            ui->partidas->addItem(p, QVariant(p));
        }
        QStandardItemModel* modelPartidas =
                qobject_cast<QStandardItemModel*>(ui->partidas->model());
        if (modelPartidas) {
            QStandardItem* item = modelPartidas->item(0);
            if (item) {
                item->setEnabled(false);
            }
        }
    }
}

void MainWindow::guardarMapa(int index) {
    std::string mapa = ui->maps->itemData(index).toString().toStdString();
    info_aux.mapSelected = mapa;
}

void MainWindow::guardarNombrePartida() {
    QString name = ui->matchName->text();

    info_aux.matchName = name.toStdString();
}

void MainWindow::guardarNumJugadores() {
    QString nTerrorist = ui->numTerrorist->text();
    info_aux.numTerrorist = nTerrorist.toInt();

    QString nCounter = ui->numCounter->text();
    info_aux.numCounterTerrorist = nCounter.toInt();
}

void MainWindow::guardarPartidaElegida(int index) {
    std::string partidaElegida = ui->partidas->itemData(index).toString().toStdString();
    info_aux.matchName = partidaElegida;
}

void MainWindow::guardarEquipo(int index) {
    QString equipo = ui->equipo->itemData(index).toString();

    ui->typeSkin->clear();
    if (equipo == "terrorist") {
        info_aux.player_type = PlayerType::TERRORIST;
        ui->skin->show();
        // Skin
        ui->typeSkin->addItem("Select operation...");
        QStandardItemModel* model1 = qobject_cast<QStandardItemModel*>(ui->typeSkin->model());
        if (model1) {
            QStandardItem* item = model1->item(0);
            if (item) {
                item->setEnabled(false);
            }
        }
        for (const std::string& skin: skinsTerrorist) {
            QString s = QString::fromStdString(skin);
            ui->typeSkin->addItem(s, QVariant(s));
        }
    } else {
        info_aux.player_type = PlayerType::COUNTERTERRORIST;
        ui->skin->show();
        // Skin
        ui->typeSkin->addItem("Select operation...");
        QStandardItemModel* model1 = qobject_cast<QStandardItemModel*>(ui->typeSkin->model());
        if (model1) {
            QStandardItem* item = model1->item(0);
            if (item) {
                item->setEnabled(false);
            }
        }
        for (const std::string& skin: skinsCounterTerrorist) {
            QString s = QString::fromStdString(skin);
            ui->typeSkin->addItem(s, QVariant(s));
        }
    }
}

void MainWindow::guardarTipoSkin(int index) {
    QString skin = ui->typeSkin->itemData(index).toString();

    info_aux.skin = skin_str_to_type(skin.toStdString());
}

bool MainWindow::validarInfo() {
    if (info_aux.type == ActionType::UNKNOWN) {
        QMessageBox::warning(this, "Error", "Select an option");
        return false;
    }
    if (info_aux.type == ActionType::CREATE &&
        (info_aux.mapSelected.empty() || info_aux.matchName.empty() || info_aux.numTerrorist == 0 ||
         info_aux.numCounterTerrorist == 0)) {
        QMessageBox::warning(this, "Error", "Empty fields");
        return false;
    }
    if (info_aux.type == ActionType::JOIN && info_aux.matchName.empty()) {
        QMessageBox::warning(this, "Error", "Select a match");
        return false;
    }
    if (info_aux.player_type == PlayerType::UNKNOWN || info_aux.skin == PlayerSkin::UNKNOWN) {
        QMessageBox::warning(this, "Error", "Select an option");
        return false;
    }
    return true;
}

void MainWindow::saveDTO() {
    if (info_aux.type == ActionType::CREATE) {
        int indexMap = ui->maps->currentIndex();
        guardarMapa(indexMap);
        guardarNombrePartida();
        guardarNumJugadores();
    } else if (info_aux.type == ActionType::JOIN) {
        int indexPartida = ui->partidas->currentIndex();
        guardarPartidaElegida(indexPartida);
    }
    int indexSkin = ui->typeSkin->currentIndex();
    guardarTipoSkin(indexSkin);

    if (!validarInfo()) {
        return;
    }
    if (info_aux.type == ActionType::CREATE) {
        info = ActionDTO(info_aux.type, info_aux.matchName, info_aux.mapSelected,
                         info_aux.numTerrorist, info_aux.numCounterTerrorist, info_aux.player_type,
                         info_aux.skin);
    } else if (info_aux.type == ActionType::JOIN) {
        info = ActionDTO(info_aux.type, info_aux.matchName, info_aux.player_type, info_aux.skin);
    }

    // ui->operacion->setCurrentIndex(0);
    // ui->maps->setCurrentIndex(0);
    // ui->matchName->clear();
    // ui->partidas->setCurrentIndex(0);
    // ui->equipo->setCurrentIndex(0);
    // ui->create->hide();
    // ui->join->hide();
    // ui->typeSkin->setCurrentIndex(0);
    // ui->skin->hide();

    QApplication::quit();
}

ActionDTO MainWindow::getInfo() { return info; }
