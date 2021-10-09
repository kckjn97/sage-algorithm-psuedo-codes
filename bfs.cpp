class BfsVertex: Vertex {
  uint8_t hop = INFINITY;

  void Scatter() {
    if(SuperStep() == 1)
      hop = 0;
    triggerGatherOnNeighbors();
  }

  void Gather(const Vertex src, const GEdge edge) {
    if( hop == INFINITY ){
      hop = src.hop + 1;
      ActivateSelf();
    }
  }
};

class BfsGVertex: GVertex {
  vector<uint8_t> hops;
  uint8_t median_hop;

  void Reduce(const Vertex v) {
    hops.push_back(v.hop);
  }

  void ReduceDone() {
    median_distance = get_median(hops);
    distances.clear();
  }
};
