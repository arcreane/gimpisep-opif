#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/stitching.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//Fonctions créées
Mat newImage();
Mat cannyEdgeDetection(Mat image);
Mat resizer(Mat image);
Mat darknen(Mat image);
Mat panoramaDirectory();
Mat panoramaImages();

//Variables utiles pour le mode panorama
Stitcher::Mode mode = Stitcher::PANORAMA;

int main(int argc, char* argv[]) {

    cout << "Bienvenue dans le nouveau GIMP\n";
    Mat result = newImage();

    int choix = 0;
    String choixPanorama = "x";

    namedWindow("GIMP");
    while (choix != -1) {
        moveWindow("GIMP", 40, 30);
        cv::imshow("GIMP", result);
        cv::waitKey(10);
        cout << "Choisisez une entier positif\n" ;
        cout << "Choissisez 1 pour entrer dans le mode diaporama\n";
        cout << "Choissisez 2 pour entrer dans le mode Canny Edge Detection\n";
        cout << "Choissisez 3 pour entrer dans le mode Resize\n";
        cout << "Choissisez 4 pour entrer dans le mode Darknen\n";
        cout << "Choissisez 10 pour ouvrir une nouvelle image\n";
        cout << "Choissisez -1 pour finir le programme\n";
        cin >> choix;
        switch (choix){
            case 1:
                cout << "Choissisez a pour entrer le nom du dossier \n";
                cout << "Choissisez b pour rentrer le nom des images une par une\n";
                cin >> choixPanorama;
                if (choixPanorama == "a")
                    result = panoramaDirectory();
                else if (choixPanorama == "b")
                    result = panoramaImages();
                else
                    cout << "Cela ne correspond à aucun choix. Veuillez recommencer.\n";
                break;
            case 2:
                result = cannyEdgeDetection(result);
                break;
            case 3:
                result = resizer(result);
                break;
            case 4:
                result = darknen(result);
                break;
            case 10:
                result = newImage();
                break;
            default:
                choix = -1;
        }
    }

    waitKey(1);
    return 0;
}

/*ALL*/
Mat newImage() {
    cout << "Veuillez rentrer le chemin absolu de votre image\n";
    String imagesPath;
    cin >> imagesPath;

    Mat result = imread(imagesPath, IMREAD_COLOR);
    return result;
}

/*
    Antoine Chenu
*/
Mat darknen(Mat image) {
    Mat new_image = Mat::zeros(image.size(), image.type());
    double alpha = 1.0;
    int beta = 0;

    /// Initialize values
    cout << "* Enter the Brightness value [-255 (Dark) to 255 (Bright)]: ";    cin >> beta;

    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            for (int c = 0; c < image.channels(); c++) {
                new_image.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha * image.at<Vec3b>(y, x)[c] + beta);
            }
        }
    }
    /*imshow("Darkness", new_image);
    cout << "Appuyez sur une touche pour sortir du mode panorama \n";
    waitKey(0);*/
    return new_image;
}
/*
    Thomas Borie
*/
Mat resizer(Mat image) {
    Mat scale;
    double scaleX;
    double scaleY;
    cout << "Type a scale for X (ex : 0.6): ";
    cin >> scaleX;
    cout << "Type a scale for Y (ex : 0.6): ";
    cin >> scaleY;
    // Scaling 
    cv::resize(image, scale, Size(round(scaleX * image.cols), round(scaleY * image.rows)), scaleX, scaleY, INTER_LINEAR);
    return scale;
}

/*
    Louis de La Tullaye
*/
cv::Mat cannyEdgeDetection(cv::Mat image) {
    cv::Mat edges;
    if (image.empty())
    {
        std::cout << "Could not open or find the image!\n" << std::endl;
    }
    int upp_treshold, lower_treshold;

    std::cout << "Lower threshold : ";
    std::cin >> lower_treshold;
    std::cout << "Upper threshold : ";
    std::cin >> upp_treshold;

    std::vector<cv::Mat> channels;
    cv::Mat temp;
    cv::cvtColor(image, temp, 1);
    cv::split(temp, channels);
    cv::Canny(image, edges, lower_treshold, upp_treshold);
    return edges;
}

/*
*   Ludivine Ducamp
    Fonction panorama qui va mettre en panorama un nombre d'images
    A l'aide du nom des images au sein même du dossier du projet
*/
Mat panoramaImages() {
    vector<String> name;
    String names;
    bool fini = false;
    while (fini == false) {
        cout << "Donner le nom des differentes images au sein du dossier\n";
        cin >> names;
        name.push_back(names);
        cin.clear();
        cout << "Avez-vous fini ? (1/0)\n";
        cin >> fini;
        cout << fini;
        cin.clear();
    }

    for (int i = 0; i < name.size(); i++) {
        Mat img = imread(name[i], IMREAD_COLOR);
        imgs.push_back(img);
    }

    Mat result;
    // Création d'un object Stitcher avec le mode panorama
    Ptr<Stitcher> stitcher = Stitcher::create(mode);
    // Stitch les images
    Stitcher::Status status = stitcher->stitch(imgs, result);

    if (status != Stitcher::OK) {
        // Regarde si les images peuvent être collées
        // Si oui, on ne rentre pas dans la boucle.
        cout << "Can't stitch images\n";
        exit;
    }
    return result;
    /*imshow("Panorama", result);
    cout << "Appuyez sur une touche pour sortir du mode panorama \n";
    waitKey(0);*/
}

/*
*   Ludivine Ducamp
* 
    Fonction panorama qui va mettre en panorama un nombre d'images
    A l'aide du chemin absolu du dossier
*/
Mat panoramaDirectory() {
    cout << "Donner le chemin du dossier ou se trouve les images.\n";
    String imagesPath;
    cin >> imagesPath;

    vector<String> name;
    glob(imagesPath, name, true);
    for (int i = 0; i < name.size(); i++) {
        Mat img = imread(name[i], IMREAD_COLOR);
        imgs.push_back(img);
    }

    Mat result;
    // Création d'un object Stitcher avec le mode panorama
    Ptr<Stitcher> stitcher = Stitcher::create(mode);
    // Stitch les images
    Stitcher::Status status = stitcher->stitch(imgs, result);

    if (status != Stitcher::OK) {
        // Regarde si les images peuvent être collées
        // Si oui, on ne rentre pas dans la boucle.
        cout << "Can't stitch images\n";
        exit;
    }
    /*imshow("Panorama", result);
    cout << "Appuyez sur une touche pour sortir du mode panorama \n";
    waitKey(0);*/
    return result;

}