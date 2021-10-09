class ShortestPathVertex: Vertex {
  unsigned int distance = INFINITY;

  void Scatter() {
    if(SuperStep() == 1)
      distance = 0;
    TriggerGatherOnNeighbors();
  }

  void Gather(const Vertex src, const GEdge edge) {
    unsigned int d = src.distance + edge.length;

    if(d < distance) {
      distance = d;
      ActivateSelf();
    }
  }
};

class ShortestPathGVertex: GVertex {
  vector<unsigned int> distances;
  unsigned int median_distance;
  
  void Reduce(const Vertex v) {
    distances.push_back(v.distance);
  }

  void ReduceDone() {
    median_distance = get_median(distances);
    distances.clear();
  }
};
