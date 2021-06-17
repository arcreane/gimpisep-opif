#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//Different functions created
int gimp_photo();
int gimp_video();

//Create image or Video
Mat newImage();
VideoCapture newVideo();

//Video
void darken(VideoCapture video, int valeur);
void contrast(VideoCapture video, int valeur);
void cannyEdgeDetectionVideo(VideoCapture video);
void resizer(VideoCapture video);
void erosion(VideoCapture video);
void dilatation(VideoCapture video);
void rotate(VideoCapture video);

//Image
Mat cannyEdgeDetection(Mat image);
Mat resizer(Mat image);
Mat darknen(Mat image);
Mat panoramaDirectory();
Mat panoramaImages();
Mat erosion(Mat image);
Mat dilatation(Mat image);
void save(Mat image, String filepath);
void saveImage(Mat image);
Mat faceDetection(Mat img);


//Globale variable
Stitcher::Mode mode = Stitcher::PANORAMA;

/*
 * @author Thomas Borie
 * @author Antoine Chenu
 * @author Ludivine Ducamp
 * @author Louis de La Tullaye
 * @author Louis Gongora
 * Functionality of launching the GIMP program
 */
int main(int argc, char* argv[]) {

    cout << "Welcome to the new GIMP\n\n\n\n";

    cout << "Do you want to work with an image or video?\n";
    cout << "1 for the image\n";
    cout << "2 for the video\n";
    cout << "Any other response will run the program for the picture.\n";
    int choiceImageVideo = 0;
    cin >> choiceImageVideo;
    switch (choiceImageVideo) {
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
 * Function to perform different functions on a photo
 */
int gimp_photo() {
    //Find the very first image
    Mat result = newImage();

    // If an image is not found, the program stops
    if (result.empty()) {
        std::cout << "Impossible de trouver une image.\n" << std::endl;
        return 0;
    }

    int choice = 0;                 //Variable corresponding to the user's choice during the program
    String choicePanorama = "x";     //Variable corresponding to the user's choice in the panorama mode
    String filePath;                //Variable containing the path of the image
    Mat previousResult = result;    //Variable containing the previous image used.

    // Creation of the main window
    namedWindow("GIMP");
    //General program loop
    while (choice != -1) {
        moveWindow("GIMP", 40, 30);
        cv::imshow("GIMP", result);
        cv::waitKey(10);

        //Proposal of the different possible values
        cout << "\n\n\n\nChoose a positive integer\n";
        cout << "Choose 1 to enter the slideshow mode\n";
        cout << "Choose 2 to enter Canny Edge Detection mode\n";
        cout << "Choose 3 to enter Resize mode\n";
        cout << "Choose 4 to enter Darknen mode\n";
        cout << "Choose 5 to enter Erosion mode\n";
        cout << "Choose 6 to enter the Dilatation mode\n";
        cout << "Choose 7 to enter the Face recognation\n";


        cout << "Choose 100 to open a new image\n";
        cout << "Choose 101 to save the image\n";
        cout << "Choose 102 to return to the previous image\n";

        cout << "else : end of the program\n";

        cin >> choice;

        //Depending on the choices made, the right function will be launched
        switch (choice) {
        case 1: //Mode panorama
            previousResult = result;
            cout << "Choose a to enter the name of the folder\n";
            cout << "Choose b to enter the name of the images one by one\n";
            cin >> choicePanorama;
            if (choicePanorama == "a")
                result = panoramaDirectory();
            else if (choicePanorama == "b")
                result = panoramaImages();
            else
                cout << "This does not match any choices. Please try again.";
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
            result = erosion(result);
            break;
        case 6: //Mode Dilatation
            previousResult = result;
            result = dilatation(result);
            break;
        case 7:
            result = faceDetection(result);
            break;
        case 100: //Mode New image
            result = newImage();
            break;
        case 101: //Save image
            saveImage(result);
            break;
        case 102: //Mode previous image
            result = previousResult;
            break;

        default: //End of the program
            choice = -1;
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
 * Function to perform different functions on a video
 */
int gimp_video() {
    VideoCapture result;

    int choice = 0;
    int bright = 0;
    float value_constrast = 0;


    //Proposition des différentes valeurs possibles
    while (choice != -1) {
        cout << "\n\n\n\nChoose a positive integer\n";
        cout << "Choose 1 to enter the Canny Edge detection\n";
        cout << "Choose 2 to enter the resizer\n";
        cout << "Choose 3 to enter the Darken\n";
        cout << "Choose 4 to enter the Erosion mode\n";
        cout << "Choose 5 to enter the Dilatation mode\n";
        cout << "Choose 6 to enter the Contrast\n";
        cout << "Choose 7 to enter the rotate Mode\n";


        cout << "else : end of the program\n";

        cin >> choice;
        switch (choice) {
        case 1:
            result = newVideo();
            // If an image is not found, the program stops
            if (!result.isOpened()) {
                cout << "Error opening video stream or file" << endl;
                return -1;
            }
            cannyEdgeDetectionVideo(result);
            break;
        case 2:
            result = newVideo();

            // If an image is not found, the program stops
            if (!result.isOpened()) {
                cout << "Error opening video stream or file" << endl;
                return -1;
            }
            resizer(result);
            break;
        case 3:
            result = newVideo();

            // If an image is not found, the program stops
            if (!result.isOpened()) {
                cout << "Error opening video stream or file" << endl;
                return -1;
            }
            do {
                // Initialization of values
                cout << "* Enter the Brightness value [-255 (Dark) to 255 (Bright)]: ";
                cin >> bright;
            } while (bright < -255 || bright > 255);
            darken(result, bright);
            break;
        case 4:
            result = newVideo();

            // If an image is not found, the program stops
            if (!result.isOpened()) {
                cout << "Error opening video stream or file" << endl;
                return -1;
            }
            erosion(result);
            break;
        case 5:
            result = newVideo();

            // If an image is not found, the program stops
            if (!result.isOpened()) {
                cout << "Error opening video stream or file" << endl;
                return -1;
            }
            dilatation(result);
            break;
        case 6:
            result = newVideo();

            // If an image is not found, the program stops
            if (!result.isOpened()) {
                cout << "Error opening video stream or file" << endl;
                return -1;
            }
            do {
                // Initialization of values
                cout << "* Enter the contrast value [Starting at 0]: ";
                cin >> value_constrast;
            } while (value_constrast < 0);
            contrast(result, value_constrast);
            break;
        case 7:
            result = newVideo();

            // If an image is not found, the program stops
            if (!result.isOpened()) {
                cout << "Error opening video stream or file" << endl;
                return -1;
            }
            rotate(result);
            break;
        default:
            choice = -1;
            break;
        }
    }
    result.release();

}

void rotate(VideoCapture video) {
    namedWindow("GIMP", WINDOW_AUTOSIZE);
    //create track bar
    int iAngle = 180;
    createTrackbar("Rotation", "GIMP", &iAngle, 360);


    while (true) {
        Mat frame;

        video >> frame;

        //Breaking the while loop at the end of the video
        if (frame.empty())
            break;

        //get the affine transformation matrix 2D
        Mat M = getRotationMatrix2D(Point(frame.cols / 2, frame.rows / 2), (iAngle - 180), 1);

        warpAffine(frame, frame, M, frame.size());

        imshow("GIMP", frame);

        if (waitKey(10) == 27) {
            cout << "Esc key is pressed by user. Stoppig the video" << endl;
            destroyWindow("GIMP");
            break;
        }
    }
}

/**
 * @author Ludivine Ducamp
 * Function to increase or decrease the contrast of a video
 */
void contrast(VideoCapture video, int valeur) {
    namedWindow("GIMP", WINDOW_AUTOSIZE);
    while (true) {
        Mat frame;
        // read a new frame from video
        video >> frame;

        //Breaking the while loop at the end of the video
        if (frame.empty())
            break;
        Mat image;
        //increase the brightness 
        frame.Mat::convertTo(image, -1, valeur, 0);

        imshow("GIMP", image);

        if (waitKey(10) == 27) {
            cout << "Esc key is pressed by user. Stoppig the video" << endl;
            destroyWindow("GIMP");
            break;
        }
    }
}


/**
 * @author Ludivine Ducamp
 * Function to increase or decrease the brightness of a video
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

        imshow("GIMP", image);

        if (waitKey(10) == 27)
        {
            cout << "Esc key is pressed by user. Stoppig the video" << endl;
            destroyWindow("GIMP");
            break;
        }
    }
}

/**
 * @author Louis de La Tullaye
 * Function allowing the canny edge detection of the video
 * This function asks the user which two values to enter (lower and upper threshold)
 */
void cannyEdgeDetectionVideo(VideoCapture video) {
    // Required parameters
    int upp_treshold, lower_treshold;
    std::cout << "Lower threshold : ";
    std::cin >> lower_treshold;
    std::cout << "Upper threshold : ";
    std::cin >> upp_treshold;

    namedWindow("GIMP", WINDOW_AUTOSIZE);

    while (true) {
        Mat frame;
        // read a new frame from video
        video >> frame;

        //Breaking the while loop at the end of the video
        if (frame.empty())
            break;

        Mat edges, GrayImage;

        cvtColor(frame, GrayImage, COLOR_BGR2GRAY);
        Canny(frame, edges, lower_treshold, upp_treshold);

        imshow("GIMP", edges);

        if (waitKey(10) == 27) {
            cout << "Esc key is pressed by user. Stoppig the video" << endl;
            destroyWindow("GIMP");
            break;
        }
        
    }
}

/**
 * @author Thomas Borie
 * Function to resize the video
 * This function asks the user what are the two values to enter (x and y)
 */
void resizer(VideoCapture video) {
    //result image
    Mat scale;
    // Required parameters
    double scaleX;
    double scaleY;

    cout << "Type a scale for X (ex : 0.6): ";
    cin >> scaleX;
    cout << "Type a scale for Y (ex : 0.6): ";
    cin >> scaleY;

    namedWindow("GIMP", WINDOW_AUTOSIZE);

    while (true) {
        Mat frame;
        // read a new frame from video
        video >> frame;

        //Breaking the while loop at the end of the video
        if (frame.empty())
            break;

        cv::resize(frame, frame, Size(round(scaleX * frame.cols), round(scaleY * frame.rows)), scaleX, scaleY, INTER_LINEAR);


        imshow("GIMP", frame);

        if (waitKey(10) == 27) {
            cout << "Esc key is pressed by user. Stoppig the video" << endl;
            destroyWindow("GIMP");
            break;
        }
    }
}

/*
 * @author Ludivine Ducamp
 * Function allowing the erosion of the video
 * Asks the user for the desired erosion value
*/
void erosion(VideoCapture video) {
    // Required parameters
    int size;
    cout << "Enter an erosion value\n";
    cin >> size;

    int erosion_type = 0;
    int erosion_elem = MORPH_RECT;

    while (true) {
        Mat frame;
        // read a new frame from video
        video >> frame;

        //Breaking the while loop at the end of the video
        if (frame.empty())
            break;

        Mat element = getStructuringElement(erosion_type,
            Size(2 * size + 1, 2 * size + 1),
            Point(size, size));
        erode(frame, frame, element);


        imshow("GIMP", frame);

        if (waitKey(10) == 27) {
            cout << "Esc key is pressed by user. Stoppig the video" << endl;
            destroyWindow("GIMP");
            break;
        }
    }

}

/*
 * @author Ludivine Ducamp
 * Function allowing the dilation of the image
 * Asks the user for the desired dilation value
*/
void dilatation(VideoCapture video) {
    // Required parameters
    int size;
    cout << "Enter a dilation value\n";
    cin >> size;

    int dilation_type = 0;
    int dilation_elem = MORPH_RECT;

    while (true) {
        Mat frame;
        // read a new frame from video
        video >> frame;

        //Breaking the while loop at the end of the video
        if (frame.empty())
            break;

        Mat element = getStructuringElement(dilation_type,
            Size(2 * size + 1, 2 * size + 1),
            Point(size, size));
        dilate(frame, frame, element);


        imshow("GIMP", frame);

        if (waitKey(10) == 27) {
            cout << "Esc key is pressed by user. Stoppig the video" << endl;
            destroyWindow("GIMP");
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
 * Function to search for a new video
 */
VideoCapture newVideo() {
    cout << "Please enter the absolute path of your video\n";
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
 * Function to search for a new image
 */
Mat newImage() {
    cout << "Please enter the absolute path of your image\n";
    String imagesPath;
    cin >> imagesPath;

    Mat result = imread(imagesPath, IMREAD_COLOR);
    return result;
}

void save(Mat image, String filepath) {}

/**
 * @author Antoine Chenu
 * Function to increase or decrease the brightness of an image
 * This function asks the user what value to enter between -255 and 255
 */
Mat darknen(Mat image) {
    //result image
    Mat new_image = Mat::zeros(image.size(), image.type());
    // Required parameters
    double alpha = 1.0;
    int beta = 0;


    do {
        // Initialization of values
        cout << "Enter the Brightness value [-255 (Dark) to 255 (Bright)]: \n";
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
 * Function to resize the image
 * This function asks the user what are the two values to enter (x and y)
 */
Mat resizer(Mat image) {
    //result image
    Mat scale;
    // Required parameters
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
 * Function allowing the canny edge detection of the image
 * This function asks the user which two values to enter (lower and upper threshold)
 */
cv::Mat cannyEdgeDetection(cv::Mat image) {
    //result image
    cv::Mat edges;
    // Required parameters
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
 * Function allowing the panorama of several images
 * Asks the user the name of each image within the same folder
*/
Mat panoramaImages() {
    // Required parameters
    vector<String> name;
    String names;
    bool fini = false; // Variable to know if all images are found


    while (fini == false) {
        cout << "Give the name of the different images within the folder\n";
        cin >> names;
        name.push_back(names);
        cin.clear();
        cout << "Are you finished? (1/0)\n\n";
        cin >> fini;
        cout << fini;
        cin.clear();
    }
    //Vector of different images
    vector<Mat> imgs;

    //Adding different images to this vector
    for (int i = 0; i < name.size(); i++) {
        Mat img = imread(name[i], IMREAD_COLOR);
        imgs.push_back(img);
    }

    //Image result
    Mat result;
    // Creating a Stitcher object with the panorama mode
    Ptr<Stitcher> stitcher = Stitcher::create(mode);
    // Stitch images
    Stitcher::Status status = stitcher->stitch(imgs, result);

    if (status != Stitcher::OK) {
        // See if the images can be pasted
        // If yes, we don't enter the loop.
        cout << "We can't stick them!\n";
        exit;
    }
    return result;
}

/*
 * @author Ludivine Ducamp
 * Function allowing the panorama of several images
 * Asks the user the name of each image within the same folder
*/
Mat panoramaDirectory() {
    cout << "Donner le chemin du dossier ou se trouve les images.\n";
    String imagesPath;
    cin >> imagesPath;

    vector<String> name;
    //Vector of different images
    vector<Mat> imgs;
    //Recovering all image names
    glob(imagesPath, name, true);
    //Adding different images to this vector
    for (int i = 0; i < name.size(); i++) {
        Mat img = imread(name[i], IMREAD_COLOR);
        imgs.push_back(img);
    }
    //Image result
    Mat result;
    // Creating a Stitcher object with the panorama mode
    Ptr<Stitcher> stitcher = Stitcher::create(mode);
    // Stitch images
    Stitcher::Status status = stitcher->stitch(imgs, result);

    if (status != Stitcher::OK) {
        // See if the images can be pasted
        // If so, we don't enter the loop.
        cout << "We can't stick them!\n";
        exit;
    }
    return result;
}

/*
 * @author Ludivine Ducamp
 * Function allowing the erosion of the image
 * Asks the user for the desired erosion value
*/
Mat erosion(Mat image) {
    //Image result
    Mat result;
    // Required parameters
    int size;
    cout << "Enter an erosion value\n";
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
 * Function allowing the dilation of the image
 * Asks the user for the desired dilation value
*/
Mat dilatation(Mat image) {
    //Image result
    Mat result;
    // Required parameters
    int size;
    cout << "Enter a dilation value\n";
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
 * Function to save the result image under the name save.jpg
 */
void saveImage(Mat image) {
    imwrite("save.jpg", image);
}


Mat faceDetection(Mat img) {
    CascadeClassifier cascade;

    cascade.load("E:/opencv/opencv/build/etc/haarcascades/haarcascade_frontalface_alt.xml");
    
    imshow("GIMP", img);

    vector<Rect> faces;
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);

    cascade.detectMultiScale(gray, faces);

    for (size_t i = 0; i < faces.size(); i++) {
        Rect r = faces[i];
        Scalar color = Scalar(255, 0, 0);
        rectangle(img, faces[i], color, 2);
    }
    imshow("GIMP", img);
    waitKey(10);
    return img;
}