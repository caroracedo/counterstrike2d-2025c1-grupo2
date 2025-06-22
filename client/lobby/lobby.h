#ifndef LOBBY_H
#define LOBBY_H

#include <QAudioOutput>
#include <QMainWindow>
#include <QMediaPlayer>
#include <string>
#include <vector>

#include "common/action_DTO.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Lobby;
}
QT_END_NAMESPACE

struct InfoAux {
    ActionType type = ActionType::UNKNOWN;
    std::string mapSelected = "";
    std::string matchName = "";
    int numTerrorist = 0;
    int numCounterTerrorist = 0;
    PlayerType player_type = PlayerType::UNKNOWN;
    PlayerSkin skin = PlayerSkin::UNKNOWN;
};

class Lobby: public QMainWindow {
    Q_OBJECT

public:
    Lobby(const std::vector<std::string>& mapasIngresados,
          const std::vector<std::string>& partidasIngresadas, QWidget* parent = nullptr);
    ~Lobby();
    ActionDTO getInfo();
    void guardarOperacion(int index);
    void guardarEquipo(int index);
    void guardarTipoSkin(int index);
    void guardarMapa(int index);
    void guardarPartidaElegida(int index);
    void guardarNombrePartida();
    void guardarNumJugadores();
    bool validarInfo();
    void saveDTO();
    PlayerSkin skin_str_to_type(const std::string& skin) {
        if (skin == "Pheonix")
            return PlayerSkin::PHEONIX;
        if (skin == "L337 Krew")
            return PlayerSkin::L337KREW;
        if (skin == "Artic avenger")
            return PlayerSkin::ARTICAVENGER;
        if (skin == "Guerrilla")
            return PlayerSkin::GUERRILLA;
        if (skin == "Seal force")
            return PlayerSkin::SEALFORCE;
        if (skin == "German GSG-9")
            return PlayerSkin::GERMANGSG9;
        if (skin == "UK SAS")
            return PlayerSkin::UKSAS;
        if (skin == "French GIGN")
            return PlayerSkin::FRENCHGIGN;
        return PlayerSkin::UNKNOWN;
    }

private:
    Ui::Lobby* ui;
    InfoAux info_aux;
    ActionDTO info;
    const std::vector<std::string>& mapas;
    const std::vector<std::string>& partidas;
    std::vector<std::string> skinsTerrorist = {"Pheonix", "L337 Krew", "Artic avenger",
                                               "Guerrilla"};
    std::vector<std::string> skinsCounterTerrorist = {"Seal force", "German GSG-9", "UK SAS",
                                                      "French GIGN"};
    QMediaPlayer* mediaPlayer;
    QAudioOutput* audioOutput;
};
#endif  // LOBBY_H
