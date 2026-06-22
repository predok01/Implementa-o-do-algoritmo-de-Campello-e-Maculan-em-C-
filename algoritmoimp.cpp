#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

// Estrutura para armazenar informações do vértice

struct Vertice {
    int id;
    int grau;
};

// Leitura manual do grafo

vector<vector<int>> lerGrafoManual() {

    int n, m;

    cout << "\nNumero de vertices: ";
    cin >> n;

    cout << "Numero de arestas: ";
    cin >> m;

    vector<vector<int>> adj(n);

    cout << "\nDigite as arestas (u v):\n";

    for (int i = 0; i < m; i++) {

        int u, v;

        cin >> u >> v;

        if (u < 0 || u >= n || v < 0 || v >= n) {
            cout << "Aresta invalida!\n";
            i--;
            continue;
        }

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    return adj;
}

// Formato:
// n m
// u v
// u v
// ...

vector<vector<int>> lerGrafoArquivo(const string& nomeArquivo) {

    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {

        cerr << "\nErro ao abrir o arquivo: "
             << nomeArquivo << endl;

        exit(1);
    }

    int n, m;

    arquivo >> n >> m;

    vector<vector<int>> adj(n);

    for (int i = 0; i < m; i++) {

        int u, v;

        arquivo >> u >> v;

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    arquivo.close();

    return adj;
}

// Calcula os graus dos vértices

vector<int> calcularGraus(
    const vector<vector<int>>& adj)
{
    vector<int> graus(adj.size());

    for (int i = 0; i < adj.size(); i++) {
        graus[i] = adj[i].size();
    }

    return graus;
}

// Ordenação decrescente por grau

vector<Vertice> ordenarVertices(
    const vector<int>& graus)
{
    vector<Vertice> vertices;

    for (int i = 0; i < graus.size(); i++) {
        vertices.push_back({i, graus[i]});
    }

    sort(
        vertices.begin(),
        vertices.end(),
        [](const Vertice& a,
           const Vertice& b)
        {
            return a.grau > b.grau;
        });

    return vertices;
}

// Verifica se um vértice pode receber uma cor

bool podeReceberCor(
    int vertice,
    int corAtual,
    const vector<vector<int>>& adj,
    const vector<int>& cores)
{
    for (int vizinho : adj[vertice]) {

        if (cores[vizinho] == corAtual)
            return false;
    }

    return true;
}

// Algoritmo de Campello e Maculan

vector<int> coloracaoCampelloMaculan(
    const vector<vector<int>>& adj)
{
    vector<int> graus =
        calcularGraus(adj);

    vector<Vertice> vertices =
        ordenarVertices(graus);

    vector<int> cores(adj.size(), -1);

    int corAtual = 0;

    cout << "\n====================================";
    cout << "\nORDEM DOS VERTICES";
    cout << "\n====================================\n";

    for (const auto& v : vertices) {

        cout << "Vertice "
             << v.id
             << " (grau "
             << v.grau
             << ")\n";
    }

    cout << "\n====================================";
    cout << "\nPROCESSO DE COLORACAO";
    cout << "\n====================================\n";

    for (const auto& v : vertices) {

        if (cores[v.id] != -1)
            continue;

        cout << "\nCriando cor "
             << corAtual
             << " usando vertice "
             << v.id << "\n\n";

        cores[v.id] = corAtual;

        for (const auto& u : vertices) {

            if (cores[u.id] != -1)
                continue;

            if (podeReceberCor(
                    u.id,
                    corAtual,
                    adj,
                    cores))
            {
                cores[u.id] = corAtual;

                cout << "Vertice "
                     << u.id
                     << " recebe cor "
                     << corAtual
                     << "\n";
            }
        }

        corAtual++;
    }

    return cores;
}

// Exibe resultado final

void exibirResultado(
    const vector<int>& cores)
{
    cout << "\n====================================";
    cout << "\nCOLORACAO FINAL";
    cout << "\n====================================\n";

    int maiorCor = -1;

    for (int i = 0; i < cores.size(); i++) {

        cout << "Vertice "
             << i
             << " -> Cor "
             << cores[i]
             << "\n";

        maiorCor =
            max(maiorCor, cores[i]);
    }

    cout << "\nNumero total de cores utilizadas: "
         << maiorCor + 1
         << "\n";
}

// Exibe estatísticas do grafo

void exibirEstatisticas(
    const vector<vector<int>>& adj,
    const vector<int>& cores)
{
    int totalArestas = 0;

    for (const auto& lista : adj)
        totalArestas += lista.size();

    totalArestas /= 2;

    int maiorCor = -1;

    for (int cor : cores)
        maiorCor = max(maiorCor, cor);

    cout << "\n====================================";
    cout << "\nESTATISTICAS";
    cout << "\n====================================\n";

    cout << "Vertices: "
         << adj.size() << endl;

    cout << "Arestas: "
         << totalArestas << endl;

    cout << "Cores utilizadas: "
         << maiorCor + 1 << endl;
}

// main

int main() {

    vector<vector<int>> adj;

    int opcao;

    cout << "====================================\n";
    cout << " COLORACAO DE GRAFOS\n";
    cout << " Algoritmo de Campello e Maculan\n";
    cout << "====================================\n\n";

    cout << "1 - Digitar grafo manualmente\n";
    cout << "2 - Ler grafo de arquivo .txt\n\n";

    cout << "Opcao: ";
    cin >> opcao;

    switch (opcao) {

        case 1:

            adj = lerGrafoManual();
            break;

        case 2: {

            string nomeArquivo;

            cout << "\nNome do arquivo: ";
            cin >> nomeArquivo;

            adj = lerGrafoArquivo(nomeArquivo);

            break;
        }

        default:

            cout << "\nOpcao invalida.\n";
            return 1;
    }

    vector<int> cores =
        coloracaoCampelloMaculan(adj);

    exibirResultado(cores);

    exibirEstatisticas(adj, cores);

    return 0;
}
