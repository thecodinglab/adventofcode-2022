#include "base.hpp"

constexpr int NUM_RESOURCES = 4;
constexpr int ORE = 0;
constexpr int CLAY = 1;
constexpr int OBSIDIAN = 2;
constexpr int GEODE = 3;

// (ore, clay, obsidian, geode)
using resources_t = array<int64_t, NUM_RESOURCES>;
// (ore robot, clay robot, obsidian robot, geode robot)
using blueprint_t = array<resources_t, NUM_RESOURCES>;
// (time, robots, inventory)
using state_t = tuple<int64_t, resources_t, resources_t>;

constexpr int TIME = 0;
constexpr int ROBOTS = 1;
constexpr int INVENTORY = 2;

vector<blueprint_t> parse_blueprints(istream &stream) {
  vector<blueprint_t> blueprints;

  while (1) {
    string line;
    getline(stream, line);

    if (!stream)
      break;

    int idx;
    resources_t ore_robot = {0, 0, 0, 0}, clay_robot = {0, 0, 0, 0},
                obsidian_robot = {0, 0, 0, 0}, geode_robot = {0, 0, 0, 0};

    int off = sscanf(
        line.c_str(),
        "Blueprint %d: Each ore robot costs %lu ore. Each clay robot "
        "costs %lu ore. Each obsidian robot costs %lu ore and %lu clay. "
        "Each geode robot costs %lu ore and %lu obsidian.",
        &idx, &get<0>(ore_robot), &get<0>(clay_robot), &get<0>(obsidian_robot),
        &get<1>(obsidian_robot), &get<0>(geode_robot), &get<2>(geode_robot));

    blueprints.push_back({
        ore_robot,
        clay_robot,
        obsidian_robot,
        geode_robot,
    });

    assert(idx == blueprints.size());

    if (blueprints.size() == 3)
      break;
  }

  return blueprints;
}

bool can_build(const resources_t &resources, const resources_t &recipe) {
  for (int i = 0; i < NUM_RESOURCES; i++) {
    if (resources[i] < recipe[i])
      return false;
  }
  return true;
}

resources_t add(const resources_t &a, const resources_t &b) {
  resources_t res;
  for (int i = 0; i < NUM_RESOURCES; i++)
    res[i] = a[i] + b[i];
  return res;
}

resources_t sub(const resources_t &a, const resources_t &b) {
  resources_t res;
  for (int i = 0; i < NUM_RESOURCES; i++)
    res[i] = a[i] - b[i];
  return res;
}

resources_t limit(const resources_t &x, const resources_t &l, int64_t m) {
  resources_t res;
  for (int i = 0; i < NUM_RESOURCES; i++)
    res[i] = min(x[i], l[i] * m);
  return res;
}

int64_t calculate_score(unordered_map<state_t, int64_t> &memory,
                        const blueprint_t &blueprint,
                        const resources_t &max_spending, const state_t &state,
                        uint16_t build_mask) {
  if (get<TIME>(state) == 0)
    return 0;

  for (int i = 0; i < NUM_RESOURCES; i++) {
    if (i == GEODE)
      continue;

    // when the production of any resource exceeds the maximum spending, cut
    // branch
    if (get<ROBOTS>(state)[i] > max_spending[i])
      return 0;
  }
  auto increment_state = [&](const resources_t &built_robots,
                             const resources_t &spending) {
    auto resources =
        add(sub(get<INVENTORY>(state), spending), get<ROBOTS>(state));

    return state_t{
        get<TIME>(state) - 1,
        add(get<ROBOTS>(state), built_robots),
        // limit resources to only account for max potential spending in the
        // future to improve cache hits
        limit(resources, max_spending, get<TIME>(state)),
    };
  };

  auto iter = memory.find(state);
  if (iter != memory.end())
    return iter->second;

  int64_t max_score = 0;

  uint16_t current_build_mask = 0;
  for (int i = 0; i < NUM_RESOURCES; i++) {
    uint16_t bit = (1 << i);

    // skip building if it could have been built in previous round
    if (build_mask & bit)
      continue;

    if (can_build(get<INVENTORY>(state), blueprint[i])) {
      current_build_mask |= bit;

      resources_t robots = {0, 0, 0, 0};
      robots[i] += 1;

      int64_t score = calculate_score(memory, blueprint, max_spending,
                                      increment_state(robots, blueprint[i]), 0);

      if (i == GEODE)
        score += get<TIME>(state) - 1;

      max_score = max(max_score, score);
    }
  }

  // dont build anything right now
  int64_t score = calculate_score(memory, blueprint, max_spending,
                                  increment_state({0, 0, 0, 0}, {0, 0, 0, 0}),
                                  current_build_mask);

  max_score = max(max_score, score);

  memory[state] = max_score;
  return max_score;
}

void run(istream &stream) {
  auto blueprints = parse_blueprints(stream);

  int64_t time = 32;
  int64_t total = 1;

  for (int i = 0; i < blueprints.size(); i++) {
    const auto &blueprint = blueprints[i];

    unordered_map<state_t, int64_t> memory;
    state_t state = {time, {1, 0, 0, 0}, {0, 0, 0, 0}};

    resources_t max_spending = {};
    for (const auto &recipe : blueprint) {
      for (int i = 0; i < NUM_RESOURCES; i++)
        max_spending[i] = max(max_spending[i], recipe[i]);
    }

    int64_t score = calculate_score(memory, blueprint, max_spending, state, 0);
    cout << "BLUEPRINT " << i << ": " << score << endl;
    total *= score;
  }

  cout << "SCORE: " << total << endl;
}

#include <fstream>

int main(int argc, char **argv) {
  if (argc == 1) {
    run(cin);
    return 0;
  }

  ifstream stream(argv[1]);
  run(stream);
  stream.close();

  return 0;
}
