#include "match_manager.h"

#include <utility>

/* Constructor */
MatchManager::MatchManager(const char* yaml_path):
        config(yaml_path),
        server_socket(config.get_server_port().c_str()),
        own_recv_queue(std::make_shared<Queue<ActionDTO>>()) {}

/* Limpieza */
void MatchManager::kill_client_handler(ClientHandler* client_handler) {
    client_handler->hard_kill();
    client_handler->join();
    delete client_handler;
}

void MatchManager::clear() {
    for (auto* client_handler: client_handlers_list) {
        kill_client_handler(client_handler);
    }
    client_handlers_list.clear();

    for (auto& match: matches) {
        match.second->stop();
        match.second->join();
    }
    matches.clear();

    shared_recv_queues.clear();
    monitors_client_send_queues.clear();
}

void MatchManager::reap() {
    client_handlers_list.remove_if([this](ClientHandler* client_handler) {
        if (!client_handler->is_alive()) {
            kill_client_handler(client_handler);
            return true;
        }
        return false;
    });

    for (auto it = matches.begin(); it != matches.end();) {
        if (!it->second->is_alive()) {
            it->second->join();
            shared_recv_queues.erase(it->first);
            monitors_client_send_queues.erase(it->first);
            it = matches.erase(it);
        } else {
            ++it;
        }
    }
}

/* Inicialización */
void MatchManager::initialize_match_resources(const std::string& match, const std::string& map) {
    std::shared_ptr<Queue<ActionDTO>> shared_recv_queue = std::make_shared<Queue<ActionDTO>>();
    std::shared_ptr<MonitorClientSendQueues> monitor_client_send_queues =
            std::make_shared<MonitorClientSendQueues>();

    shared_recv_queues[match] = shared_recv_queue;
    monitors_client_send_queues[match] = monitor_client_send_queues;
    matches[match] = std::make_shared<Match>(config, shared_recv_queue, monitor_client_send_queues,
                                             MAPS_PATH + map + YAML_EXTENSION);
}

void MatchManager::initialize_client_handler() {
    auto [client_handler, client_send_queue] = create_and_start_client_handler();

    client_send_queue->push(ActionDTO(ActionType::INFORMATION, get_matches(), get_maps()));

    ActionDTO first_action = own_recv_queue->pop();

    if (should_initialize_match(first_action))
        initialize_match_resources(first_action.match, first_action.map);

    setup_client_handler_in_match(client_handler, client_send_queue, first_action);
}

std::pair<ClientHandler*, std::shared_ptr<Queue<ActionDTO>>>
        MatchManager::create_and_start_client_handler() {
    Socket new_client_socket = server_socket.accept();
    auto client_send_queue = std::make_shared<Queue<ActionDTO>>();
    ClientHandler* handler = new ClientHandler(std::move(new_client_socket), own_recv_queue,
                                               client_send_queue, ++id);
    handler->start();
    client_handlers_list.push_back(handler);
    return {handler, client_send_queue};
}

void MatchManager::setup_client_handler_in_match(ClientHandler* handler,
                                                 std::shared_ptr<Queue<ActionDTO>>& send_queue,
                                                 const ActionDTO& action) {
    auto shared_recv_queue = shared_recv_queues[action.match];
    auto monitor_send_queues = monitors_client_send_queues[action.match];
    auto match = matches[action.match];

    monitor_send_queues->add_queue_to(send_queue, action.id);
    handler->bind_queue(shared_recv_queue);

    if (should_initialize_match(action))
        match->start();

    match->add_player(action);
    send_queue->push(ActionDTO(ActionType::CONFIGURATION, match->get_terrain(), id));
}

/* Validación */
bool MatchManager::is_valid_match(const std::string& match) const {
    return !matches.count(match) == 0;
}

bool MatchManager::should_initialize_match(const ActionDTO& action) const {
    return action.type == ActionType::CREATE ||
           (action.type == ActionType::JOIN && !is_valid_match(action.match));
}

/* Getters */
std::vector<std::string> MatchManager::get_matches() const {
    std::vector<std::string> matches_vector;
    matches_vector.reserve(matches.size());  // opcional pero eficiente
    std::transform(matches.begin(), matches.end(), std::back_inserter(matches_vector),
                   [](const auto& match) { return match.first; });
    return matches_vector;
}

std::vector<std::string> MatchManager::get_maps() const {
    std::vector<std::string> maps_vector;
    for (const auto& entry: std::filesystem::directory_iterator(MAPS_PATH)) {
        if (entry.is_regular_file() && entry.path().extension() == YAML_EXTENSION) {
            maps_vector.push_back(entry.path().stem().string());
        }
    }
    return maps_vector;
}

/* Override */
void MatchManager::run() {
    while (should_keep_running()) {
        try {
            initialize_client_handler();
            reap();
        } catch (...) {}
        // catch (const std::runtime_error& e) {
        //     std::cerr << "Runtime error: " << e.what() << std::endl;
        //     break;
        // } catch (const std::exception& e) {
        //     std::cerr << "Excepción estándar: " << e.what() << std::endl;
        //     break;
        // }
    }
    clear();
}

void MatchManager::stop() {
    Thread::stop();
    server_socket.shutdown(2);
    server_socket.close();
}
