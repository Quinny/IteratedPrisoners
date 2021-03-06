#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <functional>

namespace ipd {

namespace config {
    int rounds;         // Number of rounds used in the 'play' function
    int mem_first;      // Lowest possible mem_size for genetic population
    int mem_last;       // Highest possible mem_size for genetic population
    int pop_size;       // Population size used in the genetic algorithm
    int mutation_rate;  // Probability of their being a mutation (in %age)
    int generations;    // Number of generations used in the genetic algorithm
    int hill_pop;       // Number of candidates processes in hill-climbing

    // Returns a function which sets the specified setting
    using setter_fn = std::function<void(int)>;
    setter_fn setter_for(int& x) {
        return [&] (int y) {
            x = y;
        };
    }

    // This maps strings or argument names to functions which update
    // the appropriate constants in the config.
    std::unordered_map<std::string, setter_fn> config_map{
        // Mappings for conf file
        {"rounds",        setter_for(rounds) },
        {"mem_first",     setter_for(mem_first) },
        {"mem_last",      setter_for(mem_last) },
        {"pop_size",      setter_for(pop_size) },
        {"mutation_rate", setter_for(mutation_rate) },
        {"generations",   setter_for(generations) },
        {"hill_pop",      setter_for(hill_pop) },

        {"-r",  setter_for(rounds) },
        {"-mf", setter_for(mem_first) },
        {"-ml", setter_for(mem_last) },
        {"-p",  setter_for(pop_size) },
        {"-m",  setter_for(mutation_rate) },
        {"-g",  setter_for(generations) },
        {"-h",  setter_for(hill_pop) }
    };

    // Given a line from the config file, return the name value pair
    std::pair<std::string, int> parse_line(std::string line) {
        std::stringstream sstream(line);
        std::string name;
        int val;

        sstream >> name >> val;
        return {name, val};
    }

    // This function takes in a name value pair and updates the appropriate
    // constant using the config_map
    void assign_arg(std::string name, int val) {
        auto iter = config_map.find(name);
        if (iter != config_map.end())
            iter->second(val);
    }

    // This function reads in data from the config file, and updates all
    // the constants using the config_map and its functions.
    void load_config() {
        std::ifstream in("ipd.conf");
        std::string line;
        while (std::getline(in, line)) {
            auto p = parse_line(line);
            assign_arg(p.first, p.second);
        }
    }

    // Given argments from the command line, updates the appropriate constants
    void load_cmd_args(int argc, char* argv[]) {
        for (int i = 1; i < argc; i += 2) {
            assign_arg(argv[i], std::stoi(argv[i+1]));
        }
    }

} // config namespace

} // ipd namespace

#endif /* CONFIG_H */
