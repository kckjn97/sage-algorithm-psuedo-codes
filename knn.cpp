const unsigned int dist_limit = DIST_LIMIT;

class KnnVertex: Vertex {
  unsigned int distance = INFINITY;
  bool frontier = false;

  void Init() {
    if (GetVertexId() == SRC){
      frontier = true; distance = 0;
    }
  }

  void Scatter() {
    if (frontier) {
      TriggerGatherOnNeighbors();
      frontier = false;
    }
  }

  void Gather(const Vertex src, const GEdge edge) {
    unsigned int d = src.distance + edge.length;
    if(d < distance){
      distance = d;
      if(distance > dist_limit)
        frontier = true;
      else ActivateSelf();
    }
  }
};

class KnnGVertex: GVertex {
  vector<unsigned int> distances;
  unsigned int median_distance;
  bool frontier;

  void Reduce(const Vertex v) {
    distances.push_back(v.distance);
    if (v.frontier)
      frontier = true;
  }

  void ReduceDone() {
    median_distance = get_median(distances);
    distances.clear();
    if (median_distance < dist_limit)
      KnnGlobal::nearestNeighborCnt++;
  }
};

class KnnGlobal: Global {
  static atomic_int nearestNeighborCnt = 0;
  pair<VertexId, distance> knnVertices[K];

  void Reduce(GVertex gv) {
    if (neighborCnt >= K) {
      InsertWithPriority(knnVertices, gv);
    } else if (gv.frontier) {
      gv.frontier = false;    Activate(gv);
    }
  }

  void ReduceDone() {
    nearestNeighborCnt = 0;    dist_limit += gamma;
  }
};
