unsigned int NumSrc = 64;  // NumSrc <= 64

class InfluenceMaxVertex: Vertex {
  uint64_t visited = 0;

  void Scatter() {
    if(SuperStep() == 1){
        for(int i=0; i<NumSrc; i++) {
          if(GetVertexId() == SRC[i])
            visited |= 0x01 << i;
        }
    }
    triggerGatherOnNeighbors();
  }

  void Gather(const Vertex src, const GEdge edge) {
    uint64_t new_visited = visited | src.visited;

    if(new_visited != visited)
      ActivateSelf();
  }
};

class InfluenceMaxGVertex: GVertex {
  unsigned int visited_cnt[NumSrc] = {0};

  void Reduce(const Vertex v) {
    for(int i=0; i<NumSrc; i++)
      if(visited & (0x01 << i))
        visited_cnt ++;
  }
};

class InfluenceMaxGlobal: Global {
  unsigned int visited_cnt[NumSrc] = {0};
  unsigned int max_src;

  void Reduce(GVertex gv) {
    for(int i=0; i<NumSrc; i++)
      visited_cnt[i] += gv.visited_cnt[i];
  }

  void ReduceDone() {
    unsigned int max_cnt = visited_cnt[0];
    max_src = 0;

    for(int i=1; i<NumSrc; i++) {
      if( visited_cnt[i] > max_cnt) {
        max_src = i;
        max_cnt = visited_cnt[i];
      }
    }
  }
};
