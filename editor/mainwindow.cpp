#include "mainwindow.h"

#include <QComboBox>
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QStandardItemModel>
#include <QTextStream>
#include <QVBoxLayout>
#include <string>

#include <yaml-cpp/yaml.h>

#include "ui_mainwindow.h"

#define IMAGEN_BOMBA ":/imagenes/zona_bomba.png"
#define IMAGEN_AK47 ":/imagenes/arma3.png"
#define IMAGEN_AWP ":/imagenes/arma2.png"
#define IMAGEN_M3 ":/imagenes/arma1.png"
#define IMAGEN_CAJA1 ":/imagenes/bloque1.png"
#define IMAGEN_CAJA2 ":/imagenes/bloque2.png"
#define IMAGEN_CAJA3 ":/imagenes/bloque3.png"
#define IMAGEN_CAJA4 ":/imagenes/bloque4.png"
#define IMAGEN_CAJA5 ":/imagenes/bloque5.png"
#define IMAGEN_AZTECA ":/imagenes/aztec.png";
#define IMAGEN_ENTRENAMIENTO ":/imagenes/office.png";
#define IMAGEN_DESIERTO ":/imagenes/sand1.jpg";
#define FILAS 21
#define COLUMNAS 21
#define TAM_CELDA 32

MainWindow::MainWindow(QWidget* parent):
        QMainWindow(parent), ui(new Ui::MainWindow), scene(new QGraphicsScene(this)) {
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    inicializarGrilla();

    ui->terrenos->addItem("Seleccionar terreno...");

    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->terrenos->model());
    if (model) {
        QStandardItem* item = model->item(0);
        if (item) {
            item->setEnabled(false);
        }
    }

    ui->terrenos->addItem("Pueblo Azteca", QVariant("azteca"));
    ui->terrenos->addItem("Zona de entrenamiento", QVariant("entrenamiento"));
    ui->terrenos->addItem("Desierto", QVariant("desierto"));

    connect(ui->terrenos, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::rellenarGrillaConTerreno);

    conectarBtnClicked(ui->botonCaja1, IMAGEN_CAJA1);
    conectarBtnClicked(ui->botonCaja2, IMAGEN_CAJA2);
    conectarBtnClicked(ui->botonCaja3, IMAGEN_CAJA3);
    conectarBtnClicked(ui->botonCaja4, IMAGEN_CAJA4);
    conectarBtnClicked(ui->botonCaja5, IMAGEN_CAJA5);

    conectarBtnClicked(ui->arma1, IMAGEN_M3);
    conectarBtnClicked(ui->arma2, IMAGEN_AWP);
    conectarBtnClicked(ui->arma3, IMAGEN_AK47);

    conectarBtnClicked(ui->zona1, IMAGEN_BOMBA);

    conectarBtnPressed(ui->botonCaja1, IMAGEN_CAJA1);
    conectarBtnPressed(ui->botonCaja2, IMAGEN_CAJA2);
    conectarBtnPressed(ui->botonCaja3, IMAGEN_CAJA3);
    conectarBtnPressed(ui->botonCaja4, IMAGEN_CAJA4);
    conectarBtnPressed(ui->botonCaja5, IMAGEN_CAJA5);

    conectarBtnPressed(ui->arma1, IMAGEN_M3);
    conectarBtnPressed(ui->arma2, IMAGEN_AWP);
    conectarBtnPressed(ui->arma3, IMAGEN_AK47);

    conectarBtnPressed(ui->zona1, IMAGEN_BOMBA);

    connect(ui->btnGuardar, &QPushButton::clicked, this, [this]() {
        if (!verificarZonaBomba()) {
            return;
        }
        QString nombreArchivo =
                QFileDialog::getSaveFileName(this, "Guardar mapa", "", "YAML Files (*.yaml)");
        if (!nombreArchivo.isEmpty()) {
            if (!nombreArchivo.endsWith(".yaml", Qt::CaseInsensitive)) {
                nombreArchivo += ".yaml";
            }
            guardarMapaComoYaml(nombreArchivo);
        }
    });

    connect(ui->btnAbrir, &QPushButton::clicked, this, [this]() {
        QString nombreArchivo = QFileDialog::getOpenFileName(this, "Abrir archivo YAML", "",
                                                             "Archivos YAML (*.yaml *.yml)");
        if (!nombreArchivo.isEmpty()) {
            abrirMapaDesdeYaml(nombreArchivo);
        }
    });
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::conectarBtnClicked(QPushButton* btn, QString imagen) {
    connect(btn, &QPushButton::clicked, this, [=, this]() { imagenSeleccionada = imagen; });
}

void MainWindow::conectarBtnPressed(QPushButton* btn, QString imagen) {
    connect(btn, &QPushButton::pressed, this, [=, this]() { imagenSeleccionada = imagen; });
}

void MainWindow::inicializarGrilla() {
    dibujarGrilla();

    for (int fila = 0; fila < FILAS; fila++) {
        for (int col = 0; col < COLUMNAS; col++) {
            grilla[fila][col] = Celda();
        }
    }
}

void MainWindow::dibujarGrilla() {
    QPen pen(Qt::black);
    pen.setWidth(1);

    for (int i = 0; i <= FILAS; i++) {
        scene->addLine(0, i * TAM_CELDA, COLUMNAS * TAM_CELDA, i * TAM_CELDA, pen);
    }

    for (int j = 0; j <= COLUMNAS; j++) {
        scene->addLine(j * TAM_CELDA, 0, j * TAM_CELDA, FILAS * TAM_CELDA, pen);
    }

    scene->setSceneRect(0, 0, COLUMNAS * TAM_CELDA, FILAS * TAM_CELDA);
}

QMap<QString, TIPO_CAJA> mapaCajas = {
        {IMAGEN_CAJA1, TIPO_CAJA::CAJA1}, {IMAGEN_CAJA2, TIPO_CAJA::CAJA2},
        {IMAGEN_CAJA3, TIPO_CAJA::CAJA3}, {IMAGEN_CAJA4, TIPO_CAJA::CAJA4},
        {IMAGEN_CAJA5, TIPO_CAJA::CAJA5},
};

QMap<TIPO_CAJA, QString> mapaCajas2 = {
        {TIPO_CAJA::CAJA1, IMAGEN_CAJA1}, {TIPO_CAJA::CAJA2, IMAGEN_CAJA2},
        {TIPO_CAJA::CAJA3, IMAGEN_CAJA3}, {TIPO_CAJA::CAJA4, IMAGEN_CAJA4},
        {TIPO_CAJA::CAJA5, IMAGEN_CAJA5},
};

QMap<QString, TIPO_ARMA> mapaArmas = {
        {IMAGEN_M3, TIPO_ARMA::M3},
        {IMAGEN_AWP, TIPO_ARMA::AWP},
        {IMAGEN_AK47, TIPO_ARMA::AK47},
};

QMap<TIPO_ARMA, QString> mapaArmas2 = {
        {TIPO_ARMA::M3, IMAGEN_M3},
        {TIPO_ARMA::AWP, IMAGEN_AWP},
        {TIPO_ARMA::AK47, IMAGEN_AK47},
};

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (!ui->graphicsView->geometry().contains(event->pos())) {
        QMainWindow::mousePressEvent(event);
        return;
    }
    mouseReleaseEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
    QPoint posViewport =
            ui->graphicsView->viewport()->mapFromGlobal(event->globalPosition().toPoint());

    QPointF escenaPos = ui->graphicsView->mapToScene(posViewport);

    QGraphicsScene* scene = ui->graphicsView->scene();

    QPointF esqIzq = scene->sceneRect().topLeft();
    QPointF posRelativa = escenaPos - esqIzq;

    int fila = int(posRelativa.y()) / TAM_CELDA;
    int col = int(posRelativa.x()) / TAM_CELDA;

    if (fila < 0 || fila >= FILAS || col < 0 || col >= COLUMNAS)
        return;

    QPixmap pixmap(imagenSeleccionada);
    QPixmap pixmapEscalado =
            pixmap.scaled(TAM_CELDA, TAM_CELDA, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    if (grilla[fila][col].tipoElemento != TIPO_ELEMENTO::NINGUNO ||
        grilla[fila][col].zona != TIPO_ZONA::NINGUNA) {
        QMessageBox::warning(this, "Celda ocupada", "Ya hay un objeto en esta celda.");
        return;
    }

    if (imagenSeleccionada.contains("zona_bomba") && !agregarZonaBomba()) {
        return;
    }

    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmapEscalado);

    item->setPos(col * TAM_CELDA, fila * TAM_CELDA);
    scene->addItem(item);

    Celda celda;
    celda.imagenElemento = imagenSeleccionada;

    if (imagenSeleccionada.contains("bloque")) {
        celda.tipoElemento = TIPO_ELEMENTO::CAJA;
        celda.tipoCaja = mapaCajas[imagenSeleccionada];
    } else if (mapaArmas.contains(imagenSeleccionada)) {
        celda.tipoElemento = TIPO_ELEMENTO::ARMA;
        celda.tipoArma = mapaArmas[imagenSeleccionada];
    } else {
        celda.tipoElemento = TIPO_ELEMENTO::NINGUNO;
    }

    if (imagenSeleccionada.contains("zona_bomba") && agregarZonaBomba()) {
        cantZonasBomba++;
        celda.zona = TIPO_ZONA::BOMBA;
    }

    grilla[fila][col] = celda;
}

void MainWindow::cargarImagenTerreno() {
    QString imagen;
    if (terreno == "azteca") {
        imagen = IMAGEN_AZTECA;
    } else if (terreno == "entrenamiento") {
        imagen = IMAGEN_ENTRENAMIENTO;
    } else if (terreno == "desierto") {
        imagen = IMAGEN_DESIERTO;
    }

    for (int fila = 0; fila < FILAS; fila++) {
        for (int col = 0; col < COLUMNAS; col++) {
            QPixmap pixmap(imagen);
            QPixmap pixmapEscalado = pixmap.scaled(TAM_CELDA, TAM_CELDA, Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation);

            QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmapEscalado);

            item->setPos(col * TAM_CELDA, fila * TAM_CELDA);
            scene->addItem(item);
        }
    }
}

void MainWindow::rellenarGrillaConTerreno(int index) {
    terreno = ui->terrenos->itemData(index).toString();

    cargarImagenTerreno();

    dibujarGrilla();
}

bool MainWindow::agregarZonaBomba() {
    if (cantZonasBomba >= 2) {
        QMessageBox::warning(this, "Límite alcanzado",
                             "Como máximo puede haber 2 zonas de bombas.");
        return false;
    }
    return true;
}

bool MainWindow::verificarZonaBomba() {
    if ((cantZonasBomba < 1) || (cantZonasBomba > 2)) {
        QMessageBox::warning(this, "Error de validación", "Debe haber 1 o 2 zonas de bombas.");
        return false;
    }
    return true;
}

std::string tipoCajaToString(TIPO_CAJA tipo) {
    switch (tipo) {
        case TIPO_CAJA::CAJA1:
            return "CAJA1";
        case TIPO_CAJA::CAJA2:
            return "CAJA2";
        case TIPO_CAJA::CAJA3:
            return "CAJA3";
        case TIPO_CAJA::CAJA4:
            return "CAJA4";
        case TIPO_CAJA::CAJA5:
            return "CAJA5";
        default:
            return "UNKNOWN_CAJA";
    }
}

std::string tipoArmaToString(TIPO_ARMA tipo) {
    switch (tipo) {
        case TIPO_ARMA::M3:
            return "M3";
        case TIPO_ARMA::AWP:
            return "AWP";
        case TIPO_ARMA::AK47:
            return "AK47";
        default:
            return "UNKNOWN_ARMA";
    }
}

void MainWindow::guardarCajas(YAML::Emitter& out) {
    out << YAML::Key << "obstacles" << YAML::Value << YAML::BeginSeq;
    for (int fila = 0; fila < FILAS; fila++) {
        for (int col = 0; col < COLUMNAS; col++) {
            Celda& celda = grilla[fila][col];
            if (celda.tipoElemento == TIPO_ELEMENTO::CAJA) {
                out << YAML::BeginMap;
                out << YAML::Key << "tipo" << YAML::Value << tipoCajaToString(celda.tipoCaja);
                out << YAML::Key << "width" << YAML::Value << TAM_CELDA;
                out << YAML::Key << "height" << YAML::Value << TAM_CELDA;
                out << YAML::Key << "position" << YAML::Value << YAML::BeginMap;
                out << YAML::Key << "x" << YAML::Value << col * TAM_CELDA;
                out << YAML::Key << "y" << YAML::Value << fila * TAM_CELDA;
                out << YAML::EndMap;
                out << YAML::EndMap;
            }
        }
    }
    out << YAML::EndSeq;
}

void MainWindow::guardarArmas(YAML::Emitter& out) {
    out << YAML::Key << "weaponsMapa" << YAML::Value << YAML::BeginSeq;
    for (int fila = 0; fila < FILAS; fila++) {
        for (int col = 0; col < COLUMNAS; col++) {
            Celda& celda = grilla[fila][col];
            if (celda.tipoElemento == TIPO_ELEMENTO::ARMA) {
                out << YAML::BeginMap;
                out << YAML::Key << "tipo" << YAML::Value << tipoArmaToString(celda.tipoArma);
                out << YAML::Key << "width" << YAML::Value << TAM_CELDA;
                out << YAML::Key << "height" << YAML::Value << TAM_CELDA;
                out << YAML::Key << "position" << YAML::Value << YAML::BeginMap;
                out << YAML::Key << "x" << YAML::Value << col * TAM_CELDA;
                out << YAML::Key << "y" << YAML::Value << fila * TAM_CELDA;
                out << YAML::EndMap;
                out << YAML::EndMap;
            }
        }
    }
    out << YAML::EndSeq;
}

void MainWindow::guardarZonasBomba(YAML::Emitter& out) {
    out << YAML::Key << "bomb_zones" << YAML::Value << YAML::BeginSeq;
    for (int fila = 0; fila < FILAS; fila++) {
        for (int col = 0; col < COLUMNAS; col++) {
            Celda& celda = grilla[fila][col];
            if (celda.zona == TIPO_ZONA::BOMBA) {
                out << YAML::BeginMap;
                out << YAML::Key << "position" << YAML::Value << YAML::BeginMap;
                out << YAML::Key << "x" << YAML::Value << col * TAM_CELDA;
                out << YAML::Key << "y" << YAML::Value << fila * TAM_CELDA;
                out << YAML::EndMap;
                out << YAML::Key << "width" << YAML::Value << TAM_CELDA;
                out << YAML::Key << "height" << YAML::Value << TAM_CELDA;
                out << YAML::EndMap;
            }
        }
    }
    out << YAML::EndSeq;
}

void MainWindow::guardarMapaComoYaml(const QString& nombreArchivo) {
    YAML::Emitter out;

    out << YAML::BeginMap;

    out << YAML::Key << "server" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "port" << YAML::Value << 1500;
    out << YAML::EndMap;

    out << YAML::Key << "player" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "health" << YAML::Value << 100;
    out << YAML::Key << "money" << YAML::Value << 500;
    out << YAML::EndMap;

    out << YAML::Key << "weapons" << YAML::Value << YAML::BeginMap;

    out << YAML::Key << "knife" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "price" << YAML::Value << 0;
    out << YAML::Key << "range" << YAML::Value << 45;
    out << YAML::Key << "min_damage" << YAML::Value << 10;
    out << YAML::Key << "max_damage" << YAML::Value << 20;
    out << YAML::Key << "precision" << YAML::Value << 1;
    out << YAML::EndMap;

    out << YAML::Key << "glock" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "price" << YAML::Value << 0;
    out << YAML::Key << "range" << YAML::Value << 100;
    out << YAML::Key << "min_damage" << YAML::Value << 10;
    out << YAML::Key << "max_damage" << YAML::Value << 20;
    out << YAML::Key << "precision" << YAML::Value << 0.8;
    out << YAML::EndMap;

    out << YAML::Key << "bomb" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "price" << YAML::Value << 0;
    out << YAML::Key << "range" << YAML::Value << 0;
    out << YAML::Key << "min_damage" << YAML::Value << 500;
    out << YAML::Key << "max_damage" << YAML::Value << 500;
    out << YAML::Key << "precision" << YAML::Value << 1;
    out << YAML::EndMap;

    out << YAML::Key << "ak47" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "price" << YAML::Value << 100;
    out << YAML::Key << "range" << YAML::Value << 150;
    out << YAML::Key << "min_damage" << YAML::Value << 20;
    out << YAML::Key << "max_damage" << YAML::Value << 30;
    out << YAML::Key << "precision" << YAML::Value << 0.8;
    out << YAML::EndMap;

    out << YAML::Key << "m3" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "price" << YAML::Value << 200;
    out << YAML::Key << "range" << YAML::Value << 125;
    out << YAML::Key << "min_damage" << YAML::Value << 15;
    out << YAML::Key << "max_damage" << YAML::Value << 25;
    out << YAML::Key << "precision" << YAML::Value << 0.9;
    out << YAML::EndMap;

    out << YAML::Key << "awp" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "price" << YAML::Value << 300;
    out << YAML::Key << "range" << YAML::Value << 200;
    out << YAML::Key << "min_damage" << YAML::Value << 30;
    out << YAML::Key << "max_damage" << YAML::Value << 40;
    out << YAML::Key << "precision" << YAML::Value << 0.95;
    out << YAML::EndMap;

    out << YAML::EndMap;

    out << YAML::Key << "rounds" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "total" << YAML::Value << 10;
    out << YAML::Key << "terrorists" << YAML::Value << 1;
    out << YAML::Key << "counterterrorists" << YAML::Value << 1;
    out << YAML::EndMap;

    // EDITOR
    guardarCajas(out);
    guardarArmas(out);
    guardarZonasBomba(out);

    out << YAML::Key << "terreno" << YAML::Value << terreno.toStdString();

    out << YAML::EndMap;

    QFile file(nombreArchivo);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo.");
        return;
    }

    QTextStream outStream(&file);
    outStream << QString::fromStdString(out.c_str());
    file.close();

    QMessageBox::information(this, "Guardado", "El mapa fue guardado correctamente.");
}

TIPO_CAJA stringToTipoCaja(const std::string& str) {
    if (str == "CAJA1")
        return TIPO_CAJA::CAJA1;
    else if (str == "CAJA2")
        return TIPO_CAJA::CAJA2;
    else if (str == "CAJA3")
        return TIPO_CAJA::CAJA3;
    else if (str == "CAJA4")
        return TIPO_CAJA::CAJA4;
    else if (str == "CAJA5")
        return TIPO_CAJA::CAJA5;
    else
        return TIPO_CAJA::CAJA1;
}

TIPO_ARMA stringToTipoArma(const std::string& str) {
    if (str == "M3")
        return TIPO_ARMA::M3;
    else if (str == "AWP")
        return TIPO_ARMA::AWP;
    else if (str == "AK47")
        return TIPO_ARMA::AK47;
    else
        return TIPO_ARMA::M3;
}

void MainWindow::abrirMapaDesdeYaml(const QString& nombreArchivo) {
    YAML::Node archivo = YAML::LoadFile(nombreArchivo.toStdString());

    if (archivo["terreno"]) {
        terreno = QString::fromStdString(archivo["terreno"].as<std::string>());
    }

    cargarImagenTerreno();

    inicializarGrilla();

    if (archivo["obstacles"]) {
        for (const auto& caja: archivo["obstacles"]) {
            int x = caja["position"]["x"].as<int>();
            int y = caja["position"]["y"].as<int>();
            TIPO_CAJA tipoCaja = stringToTipoCaja(caja["tipo"].as<std::string>());
            int fila = y / TAM_CELDA;
            int col = x / TAM_CELDA;

            if (fila >= 0 && fila < FILAS && col >= 0 && col < COLUMNAS) {
                grilla[fila][col].tipoElemento = TIPO_ELEMENTO::CAJA;
                grilla[fila][col].tipoCaja = tipoCaja;
                grilla[fila][col].imagenElemento = mapaCajas2[tipoCaja];
            }
        }
    }

    if (archivo["weaponsMapa"]) {
        for (const auto& arma: archivo["weaponsMapa"]) {
            int x = arma["position"]["x"].as<int>();
            int y = arma["position"]["y"].as<int>();
            TIPO_ARMA tipoArma = stringToTipoArma(arma["tipo"].as<std::string>());
            int fila = y / TAM_CELDA;
            int col = x / TAM_CELDA;

            if (fila >= 0 && fila < FILAS && col >= 0 && col < COLUMNAS) {
                grilla[fila][col].tipoElemento = TIPO_ELEMENTO::ARMA;
                grilla[fila][col].tipoArma = tipoArma;
                grilla[fila][col].imagenElemento = mapaArmas2[tipoArma];
            }
        }
    }

    if (archivo["bomb_zones"]) {
        for (const auto& zona: archivo["bomb_zones"]) {
            int x = zona["position"]["x"].as<int>();
            int y = zona["position"]["y"].as<int>();
            int fila = y / TAM_CELDA;
            int col = x / TAM_CELDA;

            if (fila >= 0 && fila < FILAS && col >= 0 && col < COLUMNAS) {
                grilla[fila][col].zona = TIPO_ZONA::BOMBA;
                grilla[fila][col].imagenElemento = IMAGEN_BOMBA;
            }
        }
    }

    QGraphicsScene* scene = ui->graphicsView->scene();
    for (int fila = 0; fila < FILAS; fila++) {
        for (int col = 0; col < COLUMNAS; col++) {
            Celda& celda = grilla[fila][col];
            int x = col * TAM_CELDA;
            int y = fila * TAM_CELDA;

            if (!celda.imagenElemento.isEmpty()) {
                QPixmap pixmap(celda.imagenElemento);
                if (pixmap.isNull()) {
                    continue;
                }
                QPixmap pixmapEscalado = pixmap.scaled(TAM_CELDA, TAM_CELDA, Qt::KeepAspectRatio,
                                                       Qt::SmoothTransformation);
                QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmapEscalado);
                item->setPos(x, y);
                scene->addItem(item);
            }
        }
    }
}
