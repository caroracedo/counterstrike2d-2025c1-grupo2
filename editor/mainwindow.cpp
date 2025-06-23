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
#define IMAGEN_ZONA_A ":/imagenes/zonaA.png"
#define IMAGEN_ZONA_B ":/imagenes/zonaB.png"
#define IMAGEN_AK47 ":/imagenes/arma3.png"
#define IMAGEN_AWP ":/imagenes/arma2.png"
#define IMAGEN_M3 ":/imagenes/arma1.png"
#define IMAGEN_CAJA1 ":/imagenes/bloque1.png"
#define IMAGEN_CAJA2 ":/imagenes/bloque2.png"
#define IMAGEN_CAJA3 ":/imagenes/bloque3.png"
#define IMAGEN_CAJA4 ":/imagenes/bloque4.png"
#define IMAGEN_CAJA5 ":/imagenes/bloque5.png"
#define IMAGEN_AZTECA ":/imagenes/aztec.png"
#define IMAGEN_ENTRENAMIENTO ":/imagenes/office.png"
#define IMAGEN_DESIERTO ":/imagenes/sand1.jpg"

MainWindow::MainWindow(QWidget* parent):
        QMainWindow(parent), ui(new Ui::MainWindow), scene(new QGraphicsScene(this)) {
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    ui->graphicsView->viewport()->setMouseTracking(true);
    ui->graphicsView->viewport()->installEventFilter(this);

    inicializarGrilla();

    ui->terrenos->addItem("Choose terrain...");
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
    conectarBtnClicked(ui->zonaA, IMAGEN_ZONA_A);
    conectarBtnClicked(ui->zonaB, IMAGEN_ZONA_B);

    connect(ui->btnClean, &QPushButton::clicked, this, &MainWindow::limpiarMapa);

    connect(ui->btnGuardar, &QPushButton::clicked, this, [this]() {
        if (!verificarZonaBomba()) {
            return;
        } else if (!verificarZonasDeInicio()) {
            return;
        }
        QString nombreArchivo = QFileDialog::getSaveFileName(
                this, "Guardar mapa", QDir::cleanPath(MAPS_PATH), "Mapas YAML (*.yaml)");
        if (!nombreArchivo.isEmpty()) {
            if (!nombreArchivo.endsWith(".yaml", Qt::CaseInsensitive)) {
                nombreArchivo += ".yaml";
            }
            guardarMapaComoYaml(nombreArchivo);
        }
    });

    connect(ui->btnAbrir, &QPushButton::clicked, this, [this]() {
        QString nombreArchivo =
                QFileDialog::getOpenFileName(this, "Abrir archivo YAML", QDir::cleanPath(MAPS_PATH),
                                             "Archivos YAML (*.yaml *.yml)");
        if (!nombreArchivo.isEmpty()) {
            abrirMapaDesdeYaml(nombreArchivo);
        }
    });
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::conectarBtnClicked(QPushButton* btn, QString imagen) {
    connect(btn, &QPushButton::clicked, this, [this, imagen]() { imagenSeleccionada = imagen; });
}

void MainWindow::inicializarGrilla() {
    dibujarGrilla();

    for (int fila = 0; fila < MATRIX_SIZE; fila++) {
        for (int col = 0; col < MATRIX_SIZE; col++) {
            grilla[fila][col] = Celda();
        }
    }
}

void MainWindow::dibujarGrilla() {
    QPen pen(Qt::black);
    pen.setWidth(1);

    for (int i = 0; i <= MATRIX_SIZE; i++) {
        scene->addLine(0, i * CELL_SIZE, MATRIX_SIZE * CELL_SIZE, i * CELL_SIZE, pen);
    }
    for (int j = 0; j <= MATRIX_SIZE; j++) {
        scene->addLine(j * CELL_SIZE, 0, j * CELL_SIZE, MATRIX_SIZE * CELL_SIZE, pen);
    }
    scene->setSceneRect(0, 0, MATRIX_SIZE * CELL_SIZE, MATRIX_SIZE * CELL_SIZE);
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

bool posicionValida(int fila, int col) {
    return (fila >= 0 && fila < MATRIX_SIZE && col >= 0 && col < MATRIX_SIZE);
}

void MainWindow::mostrarMensajeCeldaOcupada() {
    QMessageBox::warning(this, "Celda ocupada", "Ya hay un objeto en esta celda.");
}

Celda MainWindow::crearCelda(QGraphicsPixmapItem* item) {
    Celda celda;
    celda.imagenElemento = imagenSeleccionada;
    celda.item = item;

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
    if (imagenSeleccionada.contains("zonaA") && !hayZonaA) {
        hayZonaA = true;
        celda.zona = TIPO_ZONA::TERRORISTA;
    }
    if (imagenSeleccionada.contains("zonaB") && !hayZonaB) {
        hayZonaB = true;
        celda.zona = TIPO_ZONA::COUNTERTERRORISTA;
    }
    return celda;
}

void MainWindow::colocarElemento(int fila, int col, bool mostrarWarning) {
    if (!posicionValida(fila, col)) {
        return;
    }

    QPixmap pixmap(imagenSeleccionada);
    QPixmap pixmapEscalado =
            pixmap.scaled(CELL_SIZE, CELL_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    if (grilla[fila][col].tipoElemento != TIPO_ELEMENTO::NINGUNO ||
        grilla[fila][col].zona != TIPO_ZONA::NINGUNA) {
        if (mostrarWarning) {
            mostrarMensajeCeldaOcupada();
            mousePresionado = false;
            return;
        }
    }
    if (imagenSeleccionada.contains("zona_bomba") && !agregarZonaBomba()) {
        return;
    }
    if ((imagenSeleccionada.contains("zonaA") && hayZonaA) ||
        (imagenSeleccionada.contains("zonaB") && hayZonaB)) {
        return;
    }

    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmapEscalado);
    int x = col * CELL_SIZE + (CELL_SIZE - pixmapEscalado.width()) / 2;
    int y = fila * CELL_SIZE + (CELL_SIZE - pixmapEscalado.height()) / 2;
    item->setPos(x, y);
    scene->addItem(item);

    grilla[fila][col] = crearCelda(item);
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (!ui->graphicsView->geometry().contains(event->pos())) {
        QMainWindow::mousePressEvent(event);
        return;
    }

    QPoint posViewport =
            ui->graphicsView->viewport()->mapFromGlobal(event->globalPosition().toPoint());
    QPointF escenaPos = ui->graphicsView->mapToScene(posViewport);

    int fila = int(escenaPos.y()) / CELL_SIZE;
    int col = int(escenaPos.x()) / CELL_SIZE;

    if (event->button() == Qt::RightButton) {
        eliminarElemento(fila, col);
        return;
    }

    mousePresionado = true;
    ultimaFila = fila;
    ultimaCol = col;

    colocarElemento(fila, col, true);
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (obj == ui->graphicsView->viewport()) {
        if (event->type() == QEvent::MouseMove) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

            if (mousePresionado) {
                if (imagenSeleccionada.contains("zona_bomba") ||
                    imagenSeleccionada.contains("zonaA") || imagenSeleccionada.contains("zonaB")) {
                    return true;
                }
                QPoint posViewport = mouseEvent->pos();
                QPointF escenaPos = ui->graphicsView->mapToScene(posViewport);

                int fila = int(escenaPos.y()) / CELL_SIZE;
                int col = int(escenaPos.x()) / CELL_SIZE;

                if (fila != ultimaFila || col != ultimaCol) {
                    if (grilla[fila][col].tipoElemento != TIPO_ELEMENTO::NINGUNO ||
                        grilla[fila][col].zona != TIPO_ZONA::NINGUNA) {
                        mostrarMensajeCeldaOcupada();
                        mousePresionado = false;
                        return true;
                    }
                    colocarElemento(fila, col, false);
                    ultimaFila = fila;
                    ultimaCol = col;
                }
            }
            return true;
        } else if (event->type() == QEvent::MouseButtonRelease) {
            mousePresionado = false;
            ultimaFila = -1;
            ultimaCol = -1;
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

QString MainWindow::seleccionarImagenTerreno() {
    QString imagen;
    if (terreno == "azteca") {
        imagen = IMAGEN_AZTECA;
    } else if (terreno == "entrenamiento") {
        imagen = IMAGEN_ENTRENAMIENTO;
    } else if (terreno == "desierto") {
        imagen = IMAGEN_DESIERTO;
    }
    return imagen;
}

void MainWindow::cargarImagenTerreno() {
    QString imagen = seleccionarImagenTerreno();

    for (int fila = 0; fila < MATRIX_SIZE; fila++) {
        for (int col = 0; col < MATRIX_SIZE; col++) {
            QPixmap pixmap(imagen);
            QPixmap pixmapEscalado = pixmap.scaled(CELL_SIZE, CELL_SIZE, Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation);

            QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmapEscalado);

            int x = col * CELL_SIZE + (CELL_SIZE - pixmapEscalado.width()) / 2;
            int y = fila * CELL_SIZE + (CELL_SIZE - pixmapEscalado.height()) / 2;
            item->setPos(x, y);
            scene->addItem(item);
        }
    }
}

void MainWindow::rellenarGrillaConTerreno(int index) {
    if (index == 0) {
        return;
    }
    terreno = ui->terrenos->itemData(index).toString();

    cargarImagenTerreno();

    dibujarGrilla();
}

void MainWindow::eliminarElemento(int fila, int col) {
    Celda& celda = grilla[fila][col];

    if (celda.item != nullptr) {
        scene->removeItem(celda.item);
        delete celda.item;
        celda.item = nullptr;

        if (celda.zona == TIPO_ZONA::BOMBA) {
            cantZonasBomba--;
        } else if (celda.zona == TIPO_ZONA::TERRORISTA) {
            hayZonaA = false;
        } else if (celda.zona == TIPO_ZONA::COUNTERTERRORISTA) {
            hayZonaB = false;
        }
    }
    celda = Celda();
    QString imagen = seleccionarImagenTerreno();
    QPixmap pixmap(imagen);
    QPixmap pixmapEscalado =
            pixmap.scaled(CELL_SIZE, CELL_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmapEscalado);

    int x = col * CELL_SIZE + (CELL_SIZE - pixmapEscalado.width()) / 2;
    int y = fila * CELL_SIZE + (CELL_SIZE - pixmapEscalado.height()) / 2;
    item->setPos(x, y);
    item->setZValue(-1);
    scene->addItem(item);
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

bool MainWindow::verificarZonasDeInicio() {
    if (!hayZonaA || !hayZonaB) {
        QMessageBox::warning(this, "Error de validación", "Deben estar las zonas de inicio.");
        return false;
    }
    return true;
}

void MainWindow::limpiarMapa() {
    scene->clear();
    terreno.clear();
    imagenSeleccionada.clear();

    cantZonasBomba = 0;
    hayZonaA = false;
    hayZonaB = false;

    inicializarGrilla();
    ui->terrenos->setCurrentIndex(0);
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
    for (int fila = 0; fila < MATRIX_SIZE; fila++) {
        for (int col = 0; col < MATRIX_SIZE; col++) {
            const Celda& celda = grilla[fila][col];
            if (celda.tipoElemento == TIPO_ELEMENTO::CAJA) {
                out << YAML::BeginMap;
                out << YAML::Key << "tipo" << YAML::Value << tipoCajaToString(celda.tipoCaja);
                out << YAML::Key << "width" << YAML::Value << CELL_SIZE;
                out << YAML::Key << "height" << YAML::Value << CELL_SIZE;
                out << YAML::Key << "position" << YAML::Value << YAML::BeginMap;
                out << YAML::Key << "x" << YAML::Value << col * CELL_SIZE;
                out << YAML::Key << "y" << YAML::Value << fila * CELL_SIZE;
                out << YAML::EndMap;
                out << YAML::EndMap;
            }
        }
    }
    out << YAML::EndSeq;
}

void MainWindow::guardarArmas(YAML::Emitter& out) {
    out << YAML::Key << "weaponsMapa" << YAML::Value << YAML::BeginSeq;
    for (int fila = 0; fila < MATRIX_SIZE; fila++) {
        for (int col = 0; col < MATRIX_SIZE; col++) {
            const Celda& celda = grilla[fila][col];
            if (celda.tipoElemento == TIPO_ELEMENTO::ARMA) {
                out << YAML::BeginMap;
                out << YAML::Key << "tipo" << YAML::Value << tipoArmaToString(celda.tipoArma);
                out << YAML::Key << "width" << YAML::Value << CELL_SIZE;
                out << YAML::Key << "height" << YAML::Value << CELL_SIZE;
                out << YAML::Key << "position" << YAML::Value << YAML::BeginMap;
                out << YAML::Key << "x" << YAML::Value << col * CELL_SIZE;
                out << YAML::Key << "y" << YAML::Value << fila * CELL_SIZE;
                out << YAML::EndMap;
                out << YAML::EndMap;
            }
        }
    }
    out << YAML::EndSeq;
}

void MainWindow::guardarZonasBomba(YAML::Emitter& out) {
    out << YAML::Key << "bomb_zones" << YAML::Value << YAML::BeginSeq;
    for (int fila = 0; fila < MATRIX_SIZE; fila++) {
        for (int col = 0; col < MATRIX_SIZE; col++) {
            const Celda& celda = grilla[fila][col];
            if (celda.zona == TIPO_ZONA::BOMBA) {
                out << YAML::BeginMap;
                out << YAML::Key << "position" << YAML::Value << YAML::BeginMap;
                out << YAML::Key << "x" << YAML::Value << col * CELL_SIZE;
                out << YAML::Key << "y" << YAML::Value << fila * CELL_SIZE;
                out << YAML::EndMap;
                out << YAML::Key << "width" << YAML::Value << CELL_SIZE;
                out << YAML::Key << "height" << YAML::Value << CELL_SIZE;
                out << YAML::EndMap;
            }
        }
    }
    out << YAML::EndSeq;
}

void MainWindow::guardarZonasInicio(YAML::Emitter& out) {
    out << YAML::Key << "init_zones" << YAML::Value << YAML::BeginSeq;
    for (int fila = 0; fila < MATRIX_SIZE; fila++) {
        for (int col = 0; col < MATRIX_SIZE; col++) {
            const Celda& celda = grilla[fila][col];
            if (celda.zona == TIPO_ZONA::TERRORISTA) {
                out << YAML::BeginMap;
                out << YAML::Key << "tipo" << YAML::Value << "Terrorist";
                out << YAML::Key << "width" << YAML::Value << CELL_SIZE;
                out << YAML::Key << "height" << YAML::Value << CELL_SIZE;
                out << YAML::Key << "position" << YAML::Value << YAML::BeginMap;
                out << YAML::Key << "x" << YAML::Value << col * CELL_SIZE;
                out << YAML::Key << "y" << YAML::Value << fila * CELL_SIZE;
                out << YAML::EndMap;
                out << YAML::EndMap;
            }
            if (celda.zona == TIPO_ZONA::COUNTERTERRORISTA) {
                out << YAML::BeginMap;
                out << YAML::Key << "tipo" << YAML::Value << "CounterTerrorist";
                out << YAML::Key << "width" << YAML::Value << CELL_SIZE;
                out << YAML::Key << "height" << YAML::Value << CELL_SIZE;
                out << YAML::Key << "position" << YAML::Value << YAML::BeginMap;
                out << YAML::Key << "x" << YAML::Value << col * CELL_SIZE;
                out << YAML::Key << "y" << YAML::Value << fila * CELL_SIZE;
                out << YAML::EndMap;
                out << YAML::EndMap;
            }
        }
    }
    out << YAML::EndSeq;
}

void MainWindow::guardarMapaComoYaml(const QString& nombreArchivo) {
    YAML::Emitter out;

    out << YAML::BeginMap;

    guardarCajas(out);
    guardarArmas(out);
    guardarZonasBomba(out);
    guardarZonasInicio(out);

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

void MainWindow::cargarObstaculosEnElMapa(const YAML::Node& archivo) {
    if (archivo["obstacles"]) {
        for (const auto& caja: archivo["obstacles"]) {
            int x = caja["position"]["x"].as<int>();
            int y = caja["position"]["y"].as<int>();
            TIPO_CAJA tipoCaja = stringToTipoCaja(caja["tipo"].as<std::string>());
            int fila = y / CELL_SIZE;
            int col = x / CELL_SIZE;

            if (fila >= 0 && fila < MATRIX_SIZE && col >= 0 && col < MATRIX_SIZE) {
                grilla[fila][col].tipoElemento = TIPO_ELEMENTO::CAJA;
                grilla[fila][col].tipoCaja = tipoCaja;
                grilla[fila][col].imagenElemento = mapaCajas2[tipoCaja];
            }
        }
    }
}

void MainWindow::cargarArmasEnElMapa(const YAML::Node& archivo) {
    if (archivo["weaponsMapa"]) {
        for (const auto& arma: archivo["weaponsMapa"]) {
            int x = arma["position"]["x"].as<int>();
            int y = arma["position"]["y"].as<int>();
            TIPO_ARMA tipoArma = stringToTipoArma(arma["tipo"].as<std::string>());
            int fila = y / CELL_SIZE;
            int col = x / CELL_SIZE;

            if (fila >= 0 && fila < MATRIX_SIZE && col >= 0 && col < MATRIX_SIZE) {
                grilla[fila][col].tipoElemento = TIPO_ELEMENTO::ARMA;
                grilla[fila][col].tipoArma = tipoArma;
                grilla[fila][col].imagenElemento = mapaArmas2[tipoArma];
            }
        }
    }
}

void MainWindow::cargarZonasBombasEnElMapa(const YAML::Node& archivo) {
    if (archivo["bomb_zones"]) {
        for (const auto& zona: archivo["bomb_zones"]) {
            int x = zona["position"]["x"].as<int>();
            int y = zona["position"]["y"].as<int>();
            int fila = y / CELL_SIZE;
            int col = x / CELL_SIZE;

            if (fila >= 0 && fila < MATRIX_SIZE && col >= 0 && col < MATRIX_SIZE) {
                cantZonasBomba++;
                grilla[fila][col].zona = TIPO_ZONA::BOMBA;
                grilla[fila][col].imagenElemento = IMAGEN_BOMBA;
            }
        }
    }
}

void MainWindow::cargarZonasDeInicioEnElMapa(const YAML::Node& archivo) {
    if (archivo["init_zones"]) {
        for (const auto& zona: archivo["init_zones"]) {
            int x = zona["position"]["x"].as<int>();
            int y = zona["position"]["y"].as<int>();
            int fila = y / CELL_SIZE;
            int col = x / CELL_SIZE;

            if (fila >= 0 && fila < MATRIX_SIZE && col >= 0 && col < MATRIX_SIZE) {
                if (zona["tipo"].as<std::string>() == "Terrorist") {
                    hayZonaA = true;
                    grilla[fila][col].zona = TIPO_ZONA::TERRORISTA;
                    grilla[fila][col].imagenElemento = IMAGEN_ZONA_A;
                }
                if (zona["tipo"].as<std::string>() == "CounterTerrorist") {
                    hayZonaB = true;
                    grilla[fila][col].zona = TIPO_ZONA::COUNTERTERRORISTA;
                    grilla[fila][col].imagenElemento = IMAGEN_ZONA_B;
                }
            }
        }
    }
}

void MainWindow::abrirMapaDesdeYaml(const QString& nombreArchivo) {
    YAML::Node archivo = YAML::LoadFile(nombreArchivo.toStdString());

    if (archivo["terreno"]) {
        terreno = QString::fromStdString(archivo["terreno"].as<std::string>());
    }

    cargarImagenTerreno();
    inicializarGrilla();

    cargarObstaculosEnElMapa(archivo);
    cargarArmasEnElMapa(archivo);
    cargarZonasBombasEnElMapa(archivo);
    cargarZonasDeInicioEnElMapa(archivo);

    QGraphicsScene* localScene = ui->graphicsView->scene();
    for (int fila = 0; fila < MATRIX_SIZE; fila++) {
        for (int col = 0; col < MATRIX_SIZE; col++) {
            Celda& celda = grilla[fila][col];

            if (!celda.imagenElemento.isEmpty()) {
                QPixmap pixmap(celda.imagenElemento);
                if (pixmap.isNull()) {
                    continue;
                }
                QPixmap pixmapEscalado = pixmap.scaled(CELL_SIZE, CELL_SIZE, Qt::KeepAspectRatio,
                                                       Qt::SmoothTransformation);
                QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmapEscalado);
                celda.item = item;
                int x = col * CELL_SIZE + (CELL_SIZE - pixmapEscalado.width()) / 2;
                int y = fila * CELL_SIZE + (CELL_SIZE - pixmapEscalado.height()) / 2;
                item->setPos(x, y);
                localScene->addItem(item);
            }
        }
    }
}
