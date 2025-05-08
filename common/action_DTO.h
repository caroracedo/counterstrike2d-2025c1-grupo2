#ifndef ACTION_DTO_H
#define ACTION_DTO_H

#include <string>

enum class ActionType {CREATE, LIST, JOIN, UNKNOWN};

struct ActionDTO {
    ActionType type;
    std::string game_name;

    ActionDTO() : type(ActionType::UNKNOWN), game_name() {}

    explicit ActionDTO(ActionType action_type) : 
        type(action_type), game_name() {}
    
    ActionDTO(ActionType action_type, const std::string& game_name) : 
        type(action_type), game_name(game_name) {}

};

#endif // ACTION_DTO_H