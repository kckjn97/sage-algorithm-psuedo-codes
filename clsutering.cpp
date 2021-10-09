unsigned int NUM_CLUSTER = 4;
unsigned int CURRENT_CLUSTER_STAGE = 0;

class ClusterVertex:Vertex{
  bool visited = false;

  void Scatter() {
    if(visited == false){
      visited = true;
      TriggerGatherOnNeighbors();
    }
  }
  
  void Gather(const Vertex src, const GEdge edge) {
    ActivateSelf();
  }
};

class ClusterGVertex: GVertex {
  unsigned int reliability[NUM_CLUSTER];
  unsigned int cluster = INVALID;

  void Reduce(const Vertex v) {
    if(v.visited)
      reliablity[CURRENT_CLUSTER_STAGE]++;
  }

  void ReduceDone(){
    cluster = GetMaxReliableCluster(reliability);
  }
};

class ClusterGlobal: Global {
  unsigned int farthest_vertex;
  unsigned int min_reliability = UINT_MAX;

  void Reduce(GVertex gv) {
    switch(CURRENT_CLUSTER_STAGE){
       case 0: k = 0;
       case 1: k = 1;
       case 2: k = 0;
       case 3: k = 1;
    }

    if(min_reliability > gv.reliability[CURRENT_CLUSTER_STAGE]){
      if(gv.cluster == k){
         min_reliability = gv.reliability[CURRENT_CLUSTER_STAGE];
         farthest_vertex = gv.GetVertexId();
      }
    }
  }

  void ReduceDone() {
    if(CURRENT_CLUSTER_STAGE < 3){
      CURRENT_CLUSTER_STAGE++;
      Activate(farthest_vertex);
    }
  }
}
