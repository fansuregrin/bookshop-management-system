#include "config.h"

Config::Config() {
    if (!read_from_file()) {
        read_from_std_input();
        save_to_file();
    }
}

void Config::read_from_std_input() {
    std::string menu = 
        "+----------------------------------------------+\n"
        "|              CONFIGING DATABASE              |\n"
        "+----------------------------------------------+\n"
        "| You must configure the following information |\n"
        "| to use this system normally:                 |\n"
        "| 1. HostName                                  |\n"
        "| 2. Port                                      |\n"
        "| 3. UserName                                  |\n"
        "| 4. Password                                  |\n"
        "| 5. Database Name                             |\n"
        "+----------------------------------------------+\n";
    std::cout << menu << "Enter HostName:" << std::flush;
    while (!getline(std::cin, host) || !check_host()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << output_styles::YELLOW << "Invalid HostName, please re-enter:"
                  << output_styles::RESET_ALL;
    }
    std::cout << "Enter Port:";
    while (!(std::cin >> port) || !check_port()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << output_styles::YELLOW << "Invalid Port, please re-enter:"
                  << output_styles::RESET_ALL;
    }
    std::cout << "Enter UserName:";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(std::cin, user);
    // TODO: hide password
    passwd = take_passwd();
    std::cout << "Enter Database Name:";
    getline(std::cin, db);
}

bool Config::read_from_file() {
    std::ifstream is("./CONFIG");
    if (!is.is_open()) {
        return false;
    }
    if (!getline(is, host) || !check_host()) return false;
    if (!(is >> port) || !check_port()) return false;
    is.get();  // skip '\n'
    if (!getline(is, user)) return false;
    if (!getline(is, passwd)) return false;
    if (!getline(is, db)) return false;
    is.close();
    return true;
}

void Config::save_to_file() {
    std::ofstream os("./CONFIG");
    if (!os.is_open()) {
        return;
    }
    os << host << '\n' << port << '\n' << user << '\n' << passwd << '\n' << db << '\n';
    os.close();
}

bool Config::check_host() {
    // check whitespace
    for (auto it=host.begin(); it!=host.end(); ++it) {
        if (isspace(*it)) return false;
    }
    return true;
}

inline bool Config::check_port() {
    return (port > 0 && port <= 65535);
}