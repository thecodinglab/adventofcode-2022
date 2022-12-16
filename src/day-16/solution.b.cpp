#include "base.hpp"

#include <cstring>
#include <memory>

#define MAX_WEIGHT 99
#define ROOT "AA"

struct valve {
  string identifier;
  int flow_rate;

  vector<string> adj;
};

static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
}

ostream &operator<<(ostream &o, const valve &v) {
  return o << "{" << v.identifier << "," << v.flow_rate << "," << v.adj << "}";
}

void run(istream &stream) {
  vector<valve> valves;
  unordered_map<string, int> lookup;
  vector<valve> relevant_valves;

  while (1) {
    string line;
    getline(stream, line);

    if (!stream)
      break;

    char identifier[3];
    int flow_rate;
    sscanf(line.c_str(), "Valve %s has flow rate=%d", identifier, &flow_rate);

    int next_idx = line.find(';');
    string next = line.substr(next_idx + 24);
    ltrim(next);

    int count = 1 + next.size() / 4;
    vector<string> adj(count, "");

    for (int i = 0; i < count; i++) {
      adj[i] = next.substr(4 * i, 2);
    }

    valve v = {
        .identifier = identifier,
        .flow_rate = flow_rate,
        .adj = adj,
    };

    int idx = valves.size();
    valves.push_back(v);
    lookup[identifier] = idx;

    if (v.identifier == ROOT || v.flow_rate > 0)
      relevant_valves.push_back(v);
  }

  // compute all weights
  vector<vector<int>> weights(valves.size(),
                              vector<int>(valves.size(), MAX_WEIGHT));
  for (int i = 0; i < valves.size(); i++) {
    const auto &v = valves[i];
    for (const auto &adj : v.adj) {
      int o = lookup[adj];
      weights[i][o] = 1;
    }

    weights[i][i] = 0;
  }

  for (int k = 0; k < valves.size(); k++) {
    for (int i = 0; i < valves.size(); i++) {
      for (int j = 0; j < valves.size(); j++) {
        weights[i][j] = min(weights[i][j], weights[i][k] + weights[k][j]);
      }
    }
  }

  // erase valves which cannot be reached from AA
  vector<int> &a_weights = weights[lookup[ROOT]];
  auto iter = relevant_valves.begin();
  while (iter != relevant_valves.end()) {
    if (a_weights[lookup[iter->identifier]] == MAX_WEIGHT)
      relevant_valves.erase(iter);
    else
      ++iter;
  }

  // compute relevant weights
  vector<vector<int>> relevant_weights(
      relevant_valves.size(), vector<int>(relevant_valves.size(), MAX_WEIGHT));

  for (int i = 0; i < relevant_valves.size(); i++) {
    const auto &from = relevant_valves[i];

    for (int j = 0; j < relevant_valves.size(); j++) {
      const auto &to = relevant_valves[j];

      relevant_weights[i][j] =
          weights[lookup[from.identifier]][lookup[to.identifier]];
    }
  }

  cout << "relevant: " << relevant_valves << endl;
  cout << "weights: " << relevant_weights << endl;

  // solve
  using state = tuple<int, int, int>; // (current_pos, time_left, valve_states)
  using entry = tuple<state, int>;
  unordered_map<state, int> best;

  entry maximum{};

  int start_idx = find_if(relevant_valves.begin(), relevant_valves.end(),
                          [](const valve &v) { return v.identifier == "AA"; }) -
                  relevant_valves.begin();

  // (current_pos, time_left, valve_states, score)
  queue<entry> q;
  q.push({{start_idx, 26, 0}, 0});

  while (!q.empty()) {
    auto current = q.front();
    q.pop();

    if (get<1>(current) > get<1>(maximum))
      maximum = current;

    auto current_state = get<0>(current);

    for (int i = 0; i < relevant_valves.size(); i++) {
      int bit = 1 << i;

      int open = get<2>(current_state) | bit;
      int weight = relevant_weights[get<0>(current_state)][i];
      int time = get<1>(current_state) - weight - 1;
      int score = get<1>(current);

      if ((get<2>(current_state) & bit) == 0)
        score += time * relevant_valves[i].flow_rate;

      if (time < 1)
        continue;

      state s{i, time, open};

      if (best[s] < score) {
        best[s] = score;
        q.push({s, score});
      }
    }
  }

  unordered_map<int, int> max_valve_states;
  for (const auto &entry : best) {
    int open = get<2>(entry.first);
    max_valve_states[open] = max(max_valve_states[open], entry.second);
  }

  int all_open = ((1 << relevant_valves.size()) - 1);
  int total = 0;

  for (int mask = 0; mask < (1 << relevant_valves.size()); mask++) {
    int o = all_open ^ mask;
    total = max(total, max_valve_states[o]);

    int iter = mask;
    while (iter > 0) {
      total = max(total, max_valve_states[o] + max_valve_states[iter]);
      iter = (iter - 1) & mask;
    }
  }

  cout << total << endl;
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
