#include <iostream>
#include <fstream>
#include <string>

struct Node {
    std::string plaka;
    std::string sehir;
    Node* sol;
    Node* sag;

    Node(const std::string& plaka, const std::string& sehir)
        : plaka(plaka), sehir(sehir), sol(nullptr), sag(nullptr) {}
};

void ekle(Node*& kok, const std::string& plaka, const std::string& sehir) {
    if (kok == nullptr) {
        kok = new Node(plaka, sehir);
    }
    else if (plaka < kok->plaka) {
        ekle(kok->sol, plaka, sehir);
    }
    else {
        ekle(kok->sag, plaka, sehir);
    }
}

void agacYazdir(Node* kok, int seviye = 0) {
    if (kok != nullptr) {
        agacYazdir(kok->sag, seviye + 1);
        for (int i = 0; i < seviye; i++) {
            std::cout << "   ";
        }
        std::cout << kok->plaka << std::endl;
        agacYazdir(kok->sol, seviye + 1);
    }
}

void preorderGezinme(Node* kok) {
    if (kok != nullptr) {
        std::cout << kok->plaka << " ";
        preorderGezinme(kok->sol);
        preorderGezinme(kok->sag);
    }
}

void inorderGezinme(Node* kok) {
    if (kok != nullptr) {
        inorderGezinme(kok->sol);
        std::cout << kok->plaka << " ";
        inorderGezinme(kok->sag);
    }
}

void postorderGezinme(Node* kok) {
    if (kok != nullptr) {
        postorderGezinme(kok->sol);
        postorderGezinme(kok->sag);
        std::cout << kok->plaka << " ";
    }
}

int derinlik(Node* kok) {
    if (kok == nullptr) {
        return 0;
    }
    else {
        int solDerinlik = derinlik(kok->sol);
        int sagDerinlik = derinlik(kok->sag);
        return 1 + std::max(solDerinlik, sagDerinlik);
    }
}

bool dengeliMi(Node* kok) {
    if (kok == nullptr) {
        return true;
    }
    int solDerinlik = derinlik(kok->sol);
    int sagDerinlik = derinlik(kok->sag);
    int fark = std::abs(solDerinlik - sagDerinlik);
    return (fark <= 1) && dengeliMi(kok->sol) && dengeliMi(kok->sag);
}

Node* plakaKoduAra(Node* kok, const std::string& plaka) {
    if (kok == nullptr || kok->plaka == plaka) {
        return kok;
    }
    if (plaka < kok->plaka) {
        return plakaKoduAra(kok->sol, plaka);
    }
    return plakaKoduAra(kok->sag, plaka);
}

int main() {
    std::setlocale(LC_ALL, "Turkish");
    std::ifstream dosya("sehir_plaka_kodlari.txt");
    Node* kok = nullptr;

    if (dosya.is_open()) {
        std::string satir;
        while (std::getline(dosya, satir)) {
            size_t virgulIndeksi = satir.find(',');
            std::string plaka = satir.substr(0, virgulIndeksi);
            std::string sehir = satir.substr(virgulIndeksi + 1);

            ekle(kok, plaka, sehir);
        }

        dosya.close();

        int agacDerinligi = derinlik(kok);
        bool dengeli = dengeliMi(kok);
        if (dengeli) {
            std::cout << "Derinlik: " << agacDerinligi << ", " << "Dengeli" << std::endl;
        }
        else {
            std::cout << "Derinlik: " << agacDerinligi << ", " << "Dengeli Deðil" << std::endl;
        }

        agacYazdir(kok);
        std::cout << std::endl;

        std::string plaka1, plaka2;
        std::cout << "1. ve 2. Þehir için plaka kodu giriniz: ";
        std::cin >> plaka1 >> plaka2;

        Node* sehir1 = plakaKoduAra(kok, plaka1);
        Node* sehir2 = plakaKoduAra(kok, plaka2);

        if (sehir1 != nullptr && sehir2 != nullptr) {
            std::cout << "Þehirler: " << sehir1->sehir << ", " << sehir2->sehir << std::endl;

            int fark = std::abs(derinlik(sehir1) - derinlik(sehir2));
            std::cout << "Aðaçtaki düzey farký: " << fark << std::endl;
        }
        else {
            std::cout << "Plaka kodlari bulunamadi." << std::endl;
        }

        std::cout << "Preorder: ";
        preorderGezinme(kok);
        std::cout << std::endl;

        std::cout << "Inorder: ";
        inorderGezinme(kok);
        std::cout << std::endl;

        std::cout << "PostOrder: ";
        postorderGezinme(kok);
        std::cout << std::endl;
    }
    else {
        std::cout << "Dosya açýlamadý." << std::endl;
    }

    return 0;
}
