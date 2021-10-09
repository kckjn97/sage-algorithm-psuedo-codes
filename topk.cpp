class TopkVertex: Vertex {
  bool visited = false;
    
  void Scatter() {
    if (visited==false) {
      visited = true; triggerGatherOnNeighbors();
    }
  }

  void Gather(const Vertex src, const GEdge edge) {
    ActivateSelf();
  }
};

class TopkGVertex: GVertex {
  int pathcount;

  void Reduce(const Vertex v) {
    if (v.visited) pathcount++;
  }
};

class TopkGlobal: Global {
  pair<VertexId, float> topkVertices[K];

  void Reduce(GVertex gv) {
    InsertWithPriority(topkVertices, gv);
  }
};
