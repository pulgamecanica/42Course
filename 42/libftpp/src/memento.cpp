#include "memento.hpp"

Memento::Snapshot Memento::save() {
  Snapshot snapshot;
  _saveToSnapshot(snapshot);
  return snapshot;
}

void Memento::load(const Snapshot& snapshot) {
  Snapshot temp_snapshot = snapshot;
  _loadFromSnapshot(temp_snapshot);
}