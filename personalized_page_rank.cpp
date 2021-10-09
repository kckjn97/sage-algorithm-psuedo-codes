float DAMPING_FACTOR = 0.85;
unsigned int MAX_ITERATION = 10;

class PPagerankVertex: Vertex {
  unsigned int degree = 0;
  float current_rank = 0;
  float new_rank = 0;
  float delta = 0;

  void Init() {
    degree = GetDegree();
  }

  void Scatter() {
    if( SuperStep() > MAX_ITERATION)
        return;

    if( SuperStep() == 1) {
      current_rank = 1 - DAMPING_FACTOR;     
      delta = current_rank / degree * DAMPING_FACTOR;
    } else{
      delta = (new_rank - current_rank) / degree * DAMPING_FACTOR;
      current_rank = new_rank;
    }

    triggerGatherOnNeighbors();
  }

  void Gather(const Vertex src, const GEdge edge) {
    new_rank += src.delta;
  }
};

class PPagerankGVertex: GVertex {
  float rank = 0;

  void Reduce(const Vertex v) {
    rank += v.current_rank;
  }

  void ReduceDone() {
    rank = rank / GetSampleNum();
  }
};
