#include <iostream>
#include <string>
#include <vector>
#include <fstream>

struct Vertex {
	std::string name;
	int mark;
};

struct Edge {
	int v;
	int w;
	int weight;
};

class Graph {
private:
	std::vector<Vertex> vertexList;
	std::vector<Edge> edgeList;

public:
	size_t get_number_of_vertex() {
		return vertexList.size();
	}

	void ADD_V(std::string name) {
		Vertex vertex = { name, 0 };
		vertexList.push_back(vertex);
	}

	void ADD_E(int v, int w) {
		Edge edge = { v, w };
		edgeList.push_back(edge);
	}

	void DEL_V(int v) {
		vertexList.erase(vertexList.begin() + v);
		auto it = edgeList.begin();
		while (it != edgeList.end()) {
			if (it->v == v || it->w == v) {
				it = edgeList.erase(it);
			}
			else {
				it++;
			}
		}

		for (auto& edge : edgeList) {
			if (edge.v > v) edge.v--;
			if (edge.w > v) edge.w--;
		}
	}

	void DEL_E(int v, int w) {
		for (auto it = edgeList.begin(); it != edgeList.end(); it++) {
			if ((it->v == v && it->w == w)) {
				edgeList.erase(it);
				break;
			}
		}
	}

	void EDIT_V(std::string name, int new_mark) {
		for (Vertex vertex : vertexList) {
			if (vertex.name == name) {
				vertex.mark = new_mark;
				break;
			}
		}
	}

	void EDIT_E(int v, int w, int new_weight) {
		for (Edge edge : edgeList) {
			if (edge.v == v && edge.w == w) {
				edge.weight = new_weight;
				break;
			}
		}
	}

	int FIRST(int v) const{
		for (Edge edge : edgeList) {
			if (edge.v == v) {
				return edge.w;
			}
		}
		return -1;
	}

	int NEXT(int v, int current) const {
		bool foundCurrent = false;
		for (const Edge& edge : edgeList) {
			if (edge.v == v) {
				if (foundCurrent) {
					return edge.w;
				}
				if (edge.w == current) {
					foundCurrent = true;
				}
			}
		}
		return -1;
	}


	void visualizeSimple() {
		std::cout << "\nÂÈÇÓÀËÈÇÀÖÈß ÃÐÀÔÀ:\n";
		std::cout << "Âåðøèíû: ";
		for (size_t i = 0; i < vertexList.size(); i++) {
			std::cout << "(" << i << ":" << vertexList[i].name << ") ";
		}
		std::cout << "\n\n";

		for (size_t i = 0; i < vertexList.size(); i++) {
			std::cout << vertexList[i].name << " --> ";
			for (const auto& edge : edgeList) {
				if (edge.v == i) {
					std::cout << vertexList[edge.w].name << " ";
				}
			}
			std::cout << "\n";
		}
	}

	Graph& operator=(const Graph& other) {
		if (this != &other) {
			this->vertexList = other.vertexList;
			this->edgeList = other.edgeList;
		}
		return *this;
	}

	class Iterator {
	private:
		const Graph& graph;
		int vertex;
		int currentEdge;
	
	public:
		Iterator(const Graph& g, int v) : graph(g), vertex(v), 
			currentEdge(g.FIRST(v)) { }

		int operator*() const {
			return currentEdge;
		}

		Iterator& operator++() {
			currentEdge = graph.NEXT(vertex, currentEdge);
			return *this;
		}

		bool operator!=(const Iterator& other) const {
			return currentEdge != other.currentEdge;
		}
	};

	Iterator begin(int v) const {
		return Iterator(*this, v);
	}
	Iterator end() const {
		return Iterator(*this, -1);
	}
};

std::vector<std::vector<int>> find_transitive_clourse(Graph& graph) {
	size_t lvl = graph.get_number_of_vertex();
	std::vector<std::vector<int>> M(lvl, std::vector<int>(lvl, 0));

	for (int i = 0; i < lvl; i++) {
		for (auto it = graph.begin(i); it != graph.end(); ++it) {
			M[i][*it] = 1;
		}
		M[i][i] = 1;
	}

	for (int k = 0; k < lvl; k++) {
		for (int i = 0; i < lvl; i++) {
			for (int j = 0; j < lvl; j++) {
				M[i][j] = M[i][j] | (M[i][k] & M[k][j]);
			}
		}
	}

	std::cout << "Ìàòðèöà òðàíçèòèâíîãî çàìûêàíèÿ:\n";
	for (int i = 0; i < lvl; i++) {
		for (int j = 0; j < lvl; j++) {
			std::cout << M[i][j] << " ";
		}
		std::cout << std::endl;
	}
	return M;
}

bool transitive_clourse_equal(const std::vector<std::vector<int>>& original,
	const std::vector<std::vector<int>>& test) {
	for (int i = 0; i < original.size(); i++) {
		for (int j = 0; j < original[0].size(); j++) {
			if (original[i][j] != test[i][j])
				return false;
		}
	}
	return true;
}

Graph find_min_equal_graph(Graph& graph) {
	size_t lvl = graph.get_number_of_vertex();
	Graph min_equal_graph = graph;
	std::vector<std::vector<int>>original_transitive_matrix = find_transitive_clourse(graph);

	for (int i = 0; i < lvl; i++) {
		for (int j = 0; j < lvl; j++) {
			Graph graph_copy = graph;
			graph_copy.DEL_E(i, j);
			std::vector<std::vector<int>>test_transitive_matrix = find_transitive_clourse(graph_copy);

			if (transitive_clourse_equal(original_transitive_matrix, test_transitive_matrix)) {
				min_equal_graph.DEL_E(i, j);
			}
		}
	}
	return min_equal_graph;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	Graph g;
	g.ADD_V("A");
	g.ADD_V("B");
	g.ADD_V("C");
	g.ADD_V("D");
	g.ADD_V("E");
	g.ADD_E(0, 1);
	g.ADD_E(0, 2);
	g.ADD_E(2, 3);
	g.ADD_E(0, 3);
	g.ADD_E(0, 0);
	g.ADD_E(0, 4);
	g.ADD_E(4, 1);
	Graph g1 = find_min_equal_graph(g);
	g1.visualizeSimple();
}