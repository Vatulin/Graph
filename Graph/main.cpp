#include <iostream>
#include <string>
#include <vector>

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
	int currentVertex = -1;

public:
	size_t get_number_of_vertex() {
		return vertexList.size();
	}

	void setCurrentVertex(int v) { currentVertex = v; }

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
		for (Vertex& vertex : vertexList) {
			if (vertex.name == name) {
				vertex.mark = new_mark;
				break;
			}
		}
	}

	void EDIT_E(int v, int w, int new_weight) {
		for (Edge& edge : edgeList) {
			if (edge.v == v && edge.w == w) {
				edge.weight = new_weight;
				break;
			}
		}
	}

	int FIRST(int v) const {
		for (const auto& edge : edgeList) {
			if (edge.v == v)
				return edge.w; // возвращаем вершину w, а не индекс
		}
		return -1;
	}

	int NEXT(int v, int current) const {
		size_t i_current = edgeList.size();
		for (size_t i = 0; i < edgeList.size(); i++) {
			if (edgeList[i].v == v && edgeList[i].w == current) {
				i_current = i;
				break;
			}
		}

		for (size_t j = i_current + 1; j < edgeList.size(); j++) // ищем первую следующую дугу, инцидентную v
			if (edgeList[j].v == v) return edgeList[j].w;
		return -1; 
	}


	void print() {
		for (size_t i = 0; i < vertexList.size(); i++) {
			std::cout << vertexList[i].name << " --> ";
			for (const auto& edge : edgeList) {
				if (edge.v == i) {
					std::cout << vertexList[edge.w].name << " ";
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
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
		int vertex_smegn;

	public:
		Iterator(const Graph& g) : graph(g), vertex(-1), vertex_smegn(-1) {};
		Iterator(const Graph& g, int v) : graph(g), vertex(v), vertex_smegn(g.FIRST(v)) {};

		int operator*() const {
			return vertex_smegn; 
		}

		Iterator& operator++() {
			vertex_smegn = graph.NEXT(vertex, vertex_smegn);
			return *this;
		}

		bool operator!=(const Iterator& other) const {
			return vertex_smegn != other.vertex_smegn || &graph != &other.graph;
		}
	};

	Iterator begin() const {
		return Iterator(*this, currentVertex);
	}

	Iterator begin(int v) {
		Graph::currentVertex = v;
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
		graph.setCurrentVertex(i);
		for (auto v : graph) {
			M[i][v] = 1;
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
	return M;
}

bool transitive_clourse_equal(const std::vector<std::vector<int>>& original,
	const std::vector<std::vector<int>>& test) {
	for (int i = 0; i < original.size(); i++) {
		for (int j = 0; j < test.size(); j++) {
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

	//Пример 1
	std::cout << "Пример 1: " << std::endl;
	Graph g1;
	g1.ADD_V("A");
	g1.ADD_V("B");
	g1.ADD_V("C");
	g1.ADD_V("D");
	g1.ADD_V("E");
	
	g1.ADD_E(0, 1);
	g1.ADD_E(0, 2);
	g1.ADD_E(0, 3);
	g1.ADD_E(1, 2);
	g1.ADD_E(3, 4);

	Graph g1_min = find_min_equal_graph(g1);
	g1_min.print();

	//Пример 2
	std::cout << "Приvер 2: " << std::endl;
	Graph g2;
	g2.ADD_V("A");
	g2.ADD_V("B");
	g2.ADD_V("C");
	g2.ADD_V("D");
	g2.ADD_V("E");
	g2.ADD_V("F");

	g2.ADD_E(0, 1);
	g2.ADD_E(0, 4);
	g2.ADD_E(0, 5);
	g2.ADD_E(5, 0);
	g2.ADD_E(1, 0);
	g2.ADD_E(1, 2);
	g2.ADD_E(1, 3);
	g2.ADD_E(1, 4);
	g2.ADD_E(2, 3);
	g2.ADD_E(3, 4);
	Graph g2_min = find_min_equal_graph(g2);
	g2_min.print();
}