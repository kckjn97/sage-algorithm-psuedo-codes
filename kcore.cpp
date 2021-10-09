unsigned int K=5;
float ETA = 0.5;

class KcoreVertex:Vertex {
  unsigned int degree = 0;

  void Init() {
    degree = GetDegree();
  }
  
  void Scatter() {
    TriggerGatherOnNeighbors();
  }
  
  void Gather(const Vertex src, const GEdge edge) {
    degree--;
  }
};

class KcoreGVertex: GVertex {
  bool deleted = false;
  vector<unsigned int> degrees;

  void Reduce(const Vertex v) {
    degrees.push_back(v.degree);
  }

  void ReduceDone() {
    unsigned int eta_degee = GetPercentile(degrees, ETA);

    if(eta_degee <= SuperStep()) {
      deleted = true;
      
      if(SuperStep() > K)
        return;

      ActivateSelf();
    }
  }
};

class KcoreGlobal: Global {
  Vector<VertexId> coreVertices;
  
  void Reduce(GVertex gv) {
    if (gv.deleted == false)
      coreVerties.push_back(gv.GetVertexId());
  }
}
