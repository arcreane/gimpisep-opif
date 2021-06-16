#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/stitching.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//Différentes fonctions créées
int gimp_photo();
int gimp_video();

Mat newImage();
VideoCapture newVideo();

void darken(VideoCapture video, int valeur);
void contrast(VideoCapture video, int valeur);

Mat cannyEdgeDetection(Mat image);
Mat resizer(Mat image);
Mat darknen(Mat image);
Mat panoramaDirectory();
Mat panoramaImages();
Mat Erosion(Mat image);
Mat Dilatation(Mat image);
void save(Mat image, String filepath);
void saveImage(Mat image);


//Variables globales
Stitcher::Mode mode = Stitcher::PANORAMA;

/*
 * @author Thomas Borie
 * @author Antoine Chenu
 * @author Ludivine Ducamp
 * @author Louis de La Tullaye
 * @author Louis Gongora
 * Fonctionnalité de lancement du programme GIMP
*/
int main(int argc, char* argv[]) {

    cout << "Bienvenue dans le nouveau GIMP\n\n\n";

    cout << "Voulez-vous travailler avec une image ou une vidéo ?\n";
    cout << "1 pour l'image\n";
    cout << "2 pour la vidéo\n";
    cout << "Une toute autre réponse lancera le programme pour la photo.\n";
    int choixImageVideo = 0;
    cin >> choixImageVideo;
        switch (choixImageVideo) {
            case 1:
                gimp_photo();
                break;
            case 2:
                gimp_video();
                break;
            default:
                gimp_photo();
        }

    waitKey(1);
    destroyAllWindows();
    return 0;
}

/**
 * @author Thomas Borie
 * @author Antoine Chenu
 * @author Ludivine Ducamp
 * @author Louis de La Tullaye
 * @author Louis Gongora
 *
 * Fonction permettant d'effectuer différentes fonctionnalités sur une photo
 */
int gimp_photo() {
    //Recherche de la toute première image
    Mat result = newImage();

    //Si une image n'est pas trouvée, le programme s'arrête
    if (result.empty()) {
        std::cout << "Impossible de trouver une image.\n" << std::endl;
        return 0;
    }

    int choix = 0;                      //Variable correspondant au choix de l'utilisateur durant le programme
    String choixPanorama = "x";         //Variable correspondaut au choix de l'utilisateur dans le mode panorama
    String filePath;                    //Variable contenant le chemin de l'image
    Mat previousResult = result;        //Variable contenant l'image précédente utilisée.

    // Création de la fenêtre principale
    namedWindow("GIMP");
    //Boucle générale du programme
    while (choix != -1) {
        moveWindow("GIMP", 40, 30);
        cv::imshow("GIMP", result);
        cv::waitKey(10);

        //Proposition des différentes valeurs possibles
        cout << "\n\n\n\nChoisisez une entier positif\n";
        cout << "Choissisez 1 pour entrer dans le mode diaporama\n";
        cout << "Choissisez 2 pour entrer dans le mode Canny Edge Detection\n";
        cout << "Choissisez 3 pour entrer dans le mode Resize\n";
        cout << "Choissisez 4 pour entrer dans le mode Darknen\n";
        cout << "Choissisez 5 pour entrer dans le mode Erosion\n";
        cout << "Choissisez 6 pour entrer dans le mode Dilatation\n";


        cout << "Choissisez 10 pour ouvrir une nouvelle image\n";
        cout << "Choissisez 11 pour enresgitrer l'image\n";
        cout << "Choissisez 13 pour revenir a l'image precedente\n";

        cout << "Choissisez -1 pour finir le programme\n";

        cin >> choix;

        //Suivant les choix choisis, la bonne fonction sera lancée
        switch (choix) {
        case 1: //Mode panorama
            previousResult = result;
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
        case 2: //Mode Canny Edge Detection
            previousResult = result;
            result = cannyEdgeDetection(result);
            break;
        case 3: //Mode resize
            previousResult = result;
            result = resizer(result);
            break;
        case 4: //Mode darken
            previousResult = result;
            result = darknen(result);
            break;
        case 5: //Mode Erosion
            previousResult = result;
            result = Erosion(result);
            break;
        case 6: //Mode Dilatation
            previousResult = result;
            result = Dilatation(result);
            break;
        case 10: //Mode Nouvelle image
            result = newImage();
            break;
        case 11: //Enregistrer l'image
            saveImage(result);
            break;
        case 13: //Mode Image précedente
            result = previousResult;
            break;

        default: // Fin du programme
            choix = -1;
        }
    }
}

/**
 * @author Thomas Borie
 * @author Antoine Chenu
 * @author Ludivine Ducamp
 * @author Louis de La Tullaye
 * @author Louis Gongora
 *
 * Fonction permettant d'effectuer différentes fonctionnalités sur une vidéo
 */
int gimp_video() {
    //Recherche de la toute première vidéo 
    VideoCapture result;

    int choix = 0;
    int bright = 0;
    float value_constrast = 0;


    //Proposition des différentes valeurs possibles

    while (choix != -1) {
        cout << "\n\n\n\nChoisisez une entier positif\n";
        cout << "Choissisez 1 pour entrer dans le Darken\n";
        cout << "Choissisez 2 pour entrer dans le Contraste\n";
        cin >> choix;
        switch (choix) {
        case 1:
            result = newVideo();

            //Si une image n'est pas trouvée, le programme s'arrête
            // Check if camera opened successfully
            if (!result.isOpened()) {
                cout << "Error opening video stream or file" << endl;
                return -1;
            }
            do {
                /// Initialisation des valeurs
                cout << "* Enter the Brightness value [-255 (Dark) to 255 (Bright)]: ";
                cin >> bright;
            } while (bright < -255 || bright > 255);
            darken(result, bright);
            break;
        case 2 :
            result = newVideo();

            //Si une image n'est pas trouvée, le programme s'arrête
            // Check if camera opened successfully
            if (!result.isOpened()) {
                cout << "Error opening video stream or file" << endl;
                return -1;
            }
            do {
                /// Initialisation des valeurs
                cout << "* Enter the contrast value [Starting at 0]: ";
                cin >> value_constrast;
            } while (value_constrast < 0);
            contrast(result, value_constrast);
            break;
        default:
            cout << "\nThis is not a valid option !" << endl;
            break;
        }
    }
}

/**
 * @author Ludivine Ducamp
 * Fonction permettant d'augmenter ou de baisser le contraste d'une vidéo
 */
void contrast(VideoCapture video, int valeur) {
    namedWindow("GIMP", WINDOW_AUTOSIZE);
    while (true)
    {
        Mat frame;
        // read a new frame from video
        video >> frame;

        //Breaking the while loop at the end of the video
        if (frame.empty())
            break;
        Mat image;
        //increase the brightness 
        frame.Mat::convertTo(image, -1, valeur, 0);

        //Show above frames inside the created windows.
        imshow("GIMP", image);

        //wait for for 10 ms until any key is pressed.
        //If the 'Esc' key is pressed, break the while loop.
        //If the any other key is pressed, continue the loop
        //If any key is not pressed withing 10 ms, continue the loop
        if (waitKey(10) == 27)
        {
            cout << "Esc key is pressed by user. Stoppig the video" << endl;
            break;
        }
    }
}


/**
 * @author Ludivine Ducamp
 * Fonction permettant d'augmenter ou de baisser la luminosité d'une vidéo
 */
void darken(VideoCapture video, int valeur) {
    namedWindow("GIMP", WINDOW_AUTOSIZE);
    while (true)
    {
        Mat frame;
        // read a new frame from video
        video >> frame;

        //Breaking the while loop at the end of the video
        if (frame.empty())
            break;
        Mat image;
        //increase the brightness 
        frame.Mat::convertTo(image, CV_8U, 1, valeur);

        //Show above frames inside the created windows.
        imshow("GIMP", image);

        //wait for for 10 ms until any key is pressed.
        //If the 'Esc' key is pressed, break the while loop.
        //If the any other key is pressed, continue the loop
        //If any key is not pressed withing 10 ms, continue the loop
        if (waitKey(10) == 27)
        {
            cout << "Esc key is pressed by user. Stoppig the video" << endl;
            break;
        }
    }
}

/**
 * @author Thomas Borie
 * @author Antoine Chenu
 * @author Ludivine Ducamp
 * @author Louis de La Tullaye
 * @author Louis Gongora
 *
 * Fonction permettant d'aller chercher une nouvelle image
 */
VideoCapture newVideo() {
    cout << "Veuillez rentrer le chemin absolu de votre vidéo\n";
    String imagesPath;
    cin >> imagesPath;
    
    VideoCapture cap(imagesPath);

    return cap;
}

/**
 * @author Thomas Borie
 * @author Antoine Chenu
 * @author Ludivine Ducamp
 * @author Louis de La Tullaye
 * @author Louis Gongora
 *
 * Fonction permettant d'aller chercher une nouvelle image
 */
Mat newImage() {
    cout << "Veuillez rentrer le chemin absolu de votre image\n";
    String imagesPath;
    cin >> imagesPath;

    Mat result = imread(imagesPath, IMREAD_COLOR);
    return result;
}

void save(Mat image, String filepath) {}

/**
 * @author Antoine Chenu
 * Fonction permettant d'augmenter ou de baisser la luminosité d'une image
 * Cette fonction demande à l'utilisateur quelle valeur entrée entre -255 et 255
 */
Mat darknen(Mat image) {
    //Image résultat
    Mat new_image = Mat::zeros(image.size(), image.type());
    //Paramètres nécessaires
    double alpha = 1.0;
    int beta = 0;


    do {
        /// Initialisation des valeurs
        cout << "* Enter the Brightness value [-255 (Dark) to 255 (Bright)]: ";
        cin >> beta;
    } while (beta < -255 || beta > 255);

    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            for (int c = 0; c < image.channels(); c++) {
                new_image.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha * image.at<Vec3b>(y, x)[c] + beta);
            }
        }
    }
    return new_image;
}

/**
 * @author Thomas Borie
 * Fonction permettant de resize l'image
 * Cette fonction demande à l'utilisateur quelles sont les deux valeurs à rentrer (x et y)
 */
Mat resizer(Mat image) {
    //Image résultat
    Mat scale;
    //Paramètres nécessaires
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

/**
 * @author Louis de La Tullaye
 * Fonction permettant le canny edge detection de l'image
 * Cette fonction demande à l'utilisateur quelles sont les deux valeurs à rentrer (lower et upper threshold)
 */
cv::Mat cannyEdgeDetection(cv::Mat image) {
    //Image résultat
    cv::Mat edges;
    //Paramètres nécessaires
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
 * @author Ludivine Ducamp
 * Fonction permettant le panoramra de plusieurs images
 * Demande à l'utilisateur le nom de chaque image au sein d'un même dossier
*/
Mat panoramaImages() {
    //Paramètres nécessaires
    vector<String> name;
    String names;
    bool fini = false; // Variable permettant de savoir si toutes les images sont trouvées


    while (fini == false) {
        cout << "Donner le nom des differentes images au sein du dossier\n";
        cin >> names;
        name.push_back(names);
        cin.clear();
        cout << "Avez-vous fini ? (1/0)\n\n";
        cin >> fini;
        cout << fini;
        cin.clear();
    }
    //Vector des différentes images
    vector<Mat> imgs;

    //Ajout des différentes images dans ce vecteur
    for (int i = 0; i < name.size(); i++) {
        Mat img = imread(name[i], IMREAD_COLOR);
        imgs.push_back(img);
    }

    //Image résultat
    Mat result;
    // Création d'un object Stitcher avec le mode panorama
    Ptr<Stitcher> stitcher = Stitcher::create(mode);
    // Stitch les images
    Stitcher::Status status = stitcher->stitch(imgs, result);

    if (status != Stitcher::OK) {
        // Regarde si les images peuvent être collées
        // Si oui, on ne rentre pas dans la boucle.
        cout << "On ne peut pas les coller!\n";
        exit;
    }
    return result;
}

/*
 * @author Ludivine Ducamp
 * Fonction permettant le panoramra de plusieurs images
 * Demande à l'utilisateur le chemin du dossier contenant les images
*/
Mat panoramaDirectory() {
    cout << "Donner le chemin du dossier ou se trouve les images.\n";
    String imagesPath;
    cin >> imagesPath;

    vector<String> name;
    //Vector des différentes images
    vector<Mat> imgs;
    //Récupération de toutes les noms des images
    glob(imagesPath, name, true);
    //Ajout des différentes images dans ce vecteur
    for (int i = 0; i < name.size(); i++) {
        Mat img = imread(name[i], IMREAD_COLOR);
        imgs.push_back(img);
    }
    //Image résultat
    Mat result;
    // Création d'un object Stitcher avec le mode panorama
    Ptr<Stitcher> stitcher = Stitcher::create(mode);
    // Stitch les images
    Stitcher::Status status = stitcher->stitch(imgs, result);

    if (status != Stitcher::OK) {
        // Regarde si les images peuvent être collées
        // Si oui, on ne rentre pas dans la boucle.
        cout << "On ne peut pas les coller!\n";
        exit;
    }
    return result;
}

/*
 * @author Ludivine Ducamp
 * Fonction permettant l'erosion de l'image
 * Demande à l'utilisateur la valeur de l'érosion voulue
*/
Mat Erosion(Mat image) {
    //Image résultat
    Mat result;
    //Paramètres nécessaires
    int size;
    cout << "Rentrez une valeur d'érosion\n";
    cin >> size;

    int erosion_type = 0;
    int erosion_elem = MORPH_RECT;
    Mat element = getStructuringElement(erosion_type,
        Size(2 * size + 1, 2 * size + 1),
        Point(size, size));
    erode(image, result, element);
    return result;
}

/*
 * @author Ludivine Ducamp
 * Fonction permettant la dilatation de l'image
 * Demande à l'utilisateur la valeur de la dilatation voulue
*/
Mat Dilatation(Mat image) {
    //Image résultat
    Mat result;
    //Paramètres nécessaires
    int size;
    cout << "Rentrez une valeur de dilatation\n ";
    cin >> size;

    int dilation_type = 0;
    int dilation_elem = MORPH_RECT;
    Mat element = getStructuringElement(dilation_type,
        Size(2 * size + 1, 2 * size + 1),
        Point(size, size));
    dilate(image, result, element);
    return result;
}

/**
 * @author Thomas Borie
 * @author Antoine Chenu
 * @author Ludivine Ducamp
 * @author Louis de La Tullaye
 * @author Louis Gongora
 *
 * Fonction permettant d'enregistrer l'image résultat sous le nom save.jpg
 */
void saveImage(Mat image) {
    imwrite("save.jpg", image);
}