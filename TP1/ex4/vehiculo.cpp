
class Vehiculo {
private:
    int id;
    Categoria categoria;
    double dinero;

public:
    vector<Vehiculo> registrar() {
        ifstream archivo("autopista.in");
        string sentence, dinero, id, categoria
        int idPos, catPos, dinPos;
        while (getline(archivo, sentence)) {
            idPos = sentence.find("id:");
            catPos = sentence.find("cat:")
            dinPos = sentence.find("din:");
            "id:gatos;at:"
            id = sentence.substring(idPos + 2, catPos - idPos + 3)
            
            
        }
    }
}