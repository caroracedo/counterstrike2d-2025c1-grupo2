#ifndef MAIN_MENU_DTO_H
#define MAIN_MENU_DTO_H

#include <string>

enum class Option { CREATE, LIST, JOIN, UNKNOWN };

struct MainMenuDTO {
    Option option;
    std::string game_name;

    // para unknown
    MainMenuDTO(): option(Option::UNKNOWN), game_name() {}

    // para list
    explicit MainMenuDTO(Option option): option(option), game_name() {}

    // para create y join
    MainMenuDTO(Option option, const std::string& game_name):
            option(option), game_name(game_name) {}
};

#endif  // MAIN_MENU_DTO_H
